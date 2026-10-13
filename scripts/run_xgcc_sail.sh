#!/bin/bash
# xgcc_sail oracle: compile prog.c with the xpack riscv-none-elf-gcc for
# plain RV64I/lp64 (-O0, newlib, libgcc soft mul/div/float), link it with
# the bare-metal harness in c/harness/ (crt0, HTIF console, link.ld, adapted
# from ship-your-lua), and run it on the executable Sail model (the Lean
# emulator from ship-your-lua).
#
#   scripts/run_xgcc_sail.sh prog.c
#
# Oracle contract (see scripts/oracles.toml):
#   stdout     the program's stdout (emulator noise filtered out)
#   exit code  the program's exit status; 124 = step budget exhausted
#              (MAX_STEPS) or wall timeout; 125 = could not compile/run
#   $ORACLE_META (if set): key=value lines; here steps=<Sail steps to the
#              tohost store in _exit> and text_bytes=<.text size>
#
# Env: MAX_STEPS (default 30000000), EMU, XGCC, XGCC_EXTRA (extra cflags),
#      KEEP_WORK=1 (keep the work dir, path printed on stderr).
set -u
src=$(readlink -f "$1")
root=$(cd "$(dirname "$0")/.." && pwd)
H=$root/c/harness
TC=$(ls -d "$HOME"/toolchains/xpack-riscv-none-elf-gcc-15.2.0-*/bin | head -1)
XGCC=${XGCC:-$TC/riscv-none-elf-gcc}
NM=$TC/riscv-none-elf-nm
SIZE=$TC/riscv-none-elf-size
EMU=${EMU:-$HOME/Documents/code/ship-your-lua/riscv-lean/lean_emulator/.lake/build/bin/lean_riscv_emulator}
MAX_STEPS=${MAX_STEPS:-30000000}
work=$(mktemp -d "${TMPDIR:-/tmp}/xgcc_sail.XXXXXX")
if [ -n "${KEEP_WORK:-}" ]; then echo "work: $work" >&2; else trap 'rm -rf "$work"' EXIT; fi
meta() { [ -n "${ORACLE_META:-}" ] && echo "$1" >> "$ORACLE_META"; return 0; }

# -std=gnu11 -fpermissive: gcc 15 defaults to C23 and makes old-style
# declarations / implicit int errors; the corpus is C99/C11 (as ccomp and tcc).
if ! "$XGCC" -O0 -std=gnu11 -fpermissive -w -march=rv64i -mabi=lp64 -mcmodel=medany \
    ${XGCC_EXTRA:-} -nostartfiles -T "$H/link.ld" -o "$work/prog.elf" \
    "$H/crt0.S" "$H/htif.c" "$src" -lm -lc -lgcc -lc > "$work/cc.log" 2>&1; then
  echo "xgcc_sail: compile failed" >&2; head -20 "$work/cc.log" >&2; exit 125
fi
pc=$("$NM" "$work/prog.elf" | awk '$3=="_exit"{print $1}')
[ -n "$pc" ] || { echo "xgcc_sail: no _exit symbol" >&2; exit 125; }
printf '0x%x\n' $(( 0x$pc + 16 )) > "$work/pcs"
meta "text_bytes=$("$SIZE" -A "$work/prog.elf" | awk '$1==".text"{print $2}')"

"$EMU" "$work/prog.elf" --trace-pcs "$work/pcs" --max-steps "$MAX_STEPS" \
    > "$work/raw" 2> "$work/trace"
erc=$?
steps=$(awk -F'\t' '$1=="T"{print $2+1}' "$work/trace" | tail -1)
# In --trace-pcs mode the emulator's stdout is the HTIF console bytes only
# ("TODO: cancel_reservation" and the trace rows go to stderr). The plain
# mode also prints "PC = 0x..." / "htif_tohost = 0x..." first and
# "SUCCESS" / "FAILURE..." last (ship-your-lua's difftest.sh greps them
# out); strip exactly those positions, never lines in the middle, so a
# program that prints "SUCCESS" itself is not mangled.
python3 -I - "$work/raw" <<'PY'
import re, sys
d = open(sys.argv[1], 'rb').read()
d = re.sub(rb'\A(PC = 0x[0-9a-fA-F]+\n)?(htif_tohost = 0x[0-9a-fA-F]+\n)?', b'', d)
d = re.sub(rb'(SUCCESS\n|FAILURE[^\n]*\n)\Z', b'', d)
d = d.replace(b'TODO: cancel_reservation\n', b'')
# The emulator collects console bytes in a Lean String, one code point per
# byte, and prints it UTF-8 encoded: byte 0xfe comes out as C3 BE. Undo
# that so stdout is the program's bytes (UTF-8 text printed by the program
# would otherwise be double-encoded and binary output mangled).
try:
    t = d.decode('utf-8')
    if all(ord(c) < 256 for c in t):
        d = t.encode('latin-1')
except UnicodeDecodeError:
    pass
sys.stdout.buffer.write(d)
PY
if grep -q 'TRACE-FUEL-OUT' "$work/trace"; then
  meta "steps=$MAX_STEPS"; meta "fuel_out=1"
  echo "xgcc_sail: step budget $MAX_STEPS exhausted" >&2; exit 124
fi
if grep -q 'Error' "$work/trace" "$work/raw" && [ -z "$steps" ]; then
  echo "xgcc_sail: emulator error" >&2; grep 'Error' "$work/trace" "$work/raw" | head -5 >&2; exit 125
fi
meta "steps=${steps:-}"
exit $erc
