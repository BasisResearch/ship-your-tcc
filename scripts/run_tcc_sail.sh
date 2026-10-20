#!/bin/bash
# tcc_sail oracle: `tcc -run /prog.c` on the executable Sail model, with
# prog.c embedded in the bare-metal tcc image (c/Makefile, Phase 1).
#
#   scripts/run_tcc_sail.sh prog.c
#
# Oracle contract (scripts/oracles.toml):
#   stdout     the program's stdout (emulator noise filtered as in
#              run_xgcc_sail.sh). tcc runs with -w so no warning shares
#              the console with the program.
#   exit code  the program's exit status; 124 = step budget exhausted;
#              125 = tcc rejected the program (it never reached the jump
#              into the generated code: tcc's message goes to stderr) or
#              the emulator failed
#   $ORACLE_META: steps=<total>, compile_steps=<_start .. tcc_baremetal_run>,
#              program_steps=<tcc_baremetal_enter .. _exit's tohost store>,
#              libm=1 when the program was linked with -lm (below)
#
# libm: an archive named before /prog.c pulls nothing in, so -lm goes after
# it (gen_embed.sh --largs). It costs steps (libm's archive index), so it is
# added only when tcc first stops with an unresolved reference: such a run
# never reached the program, and programs that link without it are run
# exactly as before.
#
# The runtime (everything but .embed) is built once in c/build (under a lock)
# and is the same bytes for every program; per program only embed.S is
# generated, assembled and linked.
# Env: MAX_STEPS (default 100000000), EMU, TCC_ARGS (extra tcc arguments as
#      C string literals each followed by a comma; default '"-w",'),
#      TCC_LIBS (libraries after /prog.c, e.g. '"-lm",'; default: none, with
#      the -lm retry above), KEEP_WORK=1.
set -u
src=$(readlink -f "$1")
root=$(cd "$(dirname "$0")/.." && pwd)
C=$root/c
TC=$(ls -d "$HOME"/toolchains/xpack-riscv-none-elf-gcc-15.2.0-*/bin | head -1)
XGCC=$TC/riscv-none-elf-gcc
NM=$TC/riscv-none-elf-nm
OBJDUMP=$TC/riscv-none-elf-objdump
EMU=${EMU:-$HOME/Documents/code/ship-your-lua/riscv-lean/lean_emulator/.lake/build/bin/lean_riscv_emulator}
MAX_STEPS=${MAX_STEPS:-100000000}
TCC_ARGS=${TCC_ARGS:-'"-w",'}
CFLAGS="-std=gnu11 -O2 -Wall -march=rv64i -mabi=lp64 -mcmodel=medany"
work=$(mktemp -d "${TMPDIR:-/tmp}/tcc_sail.XXXXXX")
if [ -n "${KEEP_WORK:-}" ]; then echo "work: $work" >&2; else trap 'rm -rf "$work"' EXIT; fi
meta() { [ -n "${ORACLE_META:-}" ] && echo "$1" >> "$ORACLE_META"; return 0; }

# the runtime objects and the file-system list, built once
RT="$C/build/crt0.o $C/build/htif.o $C/build/main.o $C/build/tcc.o"
(
  flock 9
  make -s -C "$C" BUILD=build FLAGS=0 ELF=build/base.elf build/base.elf > "$work/make.log" 2>&1
) 9> "$C/.build.lock" || { echo "tcc_sail: image build failed" >&2; tail -20 "$work/make.log" >&2; exit 125; }

TCC_LIBS=${TCC_LIBS:-}
attempt() {
largs=()
[ -n "$TCC_LIBS" ] && largs=(--largs "$TCC_LIBS")
sh "$C/src/gen_embed.sh" --cargs "$TCC_ARGS" "${largs[@]}" --flags 0 --list "$C/build/fs.list" \
    "/prog.c=$src" > "$work/embed.S" &&
"$XGCC" $CFLAGS -c -o "$work/embed.o" "$work/embed.S" &&
"$XGCC" $CFLAGS -nostartfiles -T "$C/src/link.ld" -o "$work/prog.elf" \
    $RT "$work/embed.o" -lm > "$work/ld.log" 2>&1 ||
  { echo "tcc_sail: image link failed" >&2; cat "$work/ld.log" >&2; return 1; }

sym() { printf '0x%x' 0x$("$NM" "$work/prog.elf" | awk -v s="$1" '$3==s{print $1}'); }
run=$(sym tcc_baremetal_run); enter=$(sym tcc_baremetal_enter)
exitsd=$("$OBJDUMP" -d --disassemble=_exit "$work/prog.elf" | awk '$3=="sd"{sub(":","",$1); print "0x"$1; exit}')
printf '%s\n' "$run" "$enter" "$exitsd" > "$work/pcs"

"$EMU" "$work/prog.elf" --trace-pcs "$work/pcs" --max-steps "$MAX_STEPS" \
    > "$work/raw" 2> "$work/trace"
erc=$?
# trace rows: T <step> <pc> ...
s_run=$(step_at "$(hex $run)"); s_enter=$(step_at "$(hex $enter)"); s_exit=$(step_at "$(hex $exitsd)")
}
step_at() { awk -F'\t' -v p="$1" '$1=="T" && $3==p {print $2; exit}' "$work/trace"; }
hex() { printf '%x' "$1"; }
attempt || exit 125
if [ -z "$s_enter" ] && [ -z "$TCC_LIBS" ] && ! grep -q 'TRACE-FUEL-OUT' "$work/trace" &&
   grep -aq "unresolved reference" "$work/raw"; then
  TCC_LIBS='"-lm",'; meta "libm=1"
  attempt || exit 125
fi

python3 -I "$root/scripts/sail_console.py" "$work/raw" > "$work/out"

[ -n "$s_run" ] && meta "compile_steps=$s_run"
if [ -n "$s_enter" ] && [ -n "$s_exit" ]; then meta "program_steps=$((s_exit - s_enter + 1))"; fi
if grep -q 'TRACE-FUEL-OUT' "$work/trace"; then
  meta "steps=$MAX_STEPS"; meta "fuel_out=1"
  if [ -z "$s_enter" ]; then echo "tcc_sail: step budget exhausted while compiling" >&2; fi
  cat "$work/out"; echo "tcc_sail: step budget $MAX_STEPS exhausted" >&2; exit 124
fi
if [ -z "$s_enter" ]; then
  # tcc stopped before running the program: its diagnostics are the console
  echo "tcc_sail: tcc did not run the program (exit $erc):" >&2
  head -c 4000 "$work/out" >&2; exit 125
fi
if [ -z "$s_exit" ]; then
  echo "tcc_sail: emulator stopped without a traced program exit (exit $erc)" >&2
  exit 125
fi
meta "steps=$((s_exit + 1))"
cat "$work/out"
exit $erc
