#!/bin/bash
# F3 fast reference: the tcc -run arena of a program, from the SAME image the
# Sail model runs (c/build runtime + gen_embed.sh's .embed for /prog.c, as in
# scripts/run_tcc_sail.sh), executed by build/rv64emu (scripts/rv64emu.c).
#
#   scripts/ref_arena.sh prog.c OUT.arena [OUT.console]
#
# Writes OUT.arena (rv64emu's TCCARENA format) when tcc reaches the hand-off,
# else no arena; the console (tcc's diagnostics before program entry)
# goes to OUT.console (default /dev/null). Exit status: 0 when the arena was
# written, 125 when tcc stopped first (the console says why), 2 on harness
# errors (124 for emulator limits, 126 for traps). Host success means compiler
# handoff, never a generated-program exit. With TCC_LIBS unset, unresolved
# references are retried with -lm, as run_tcc_sail.sh does.
# Env: MAX_STEPS (default 400000000), TCC_ARGS (default '"-w",'),
#      TCC_LIBS (libraries after /prog.c, as gen_embed.sh --largs),
#      EXTRA_FILES (more embedded files, space-separated /path=hostfile
#      specs, e.g. EXTRA_FILES='/prog.o=x.o' TCC_ARGS='"-w","/prog.o",'),
#      ELF_OUT (keep the per-program ELF there).
set -uo pipefail
[ "$#" -ge 2 ] && [ "$#" -le 3 ] || { echo "usage: ref_arena.sh prog.c OUT.arena [OUT.console]" >&2; exit 2; }
src=$(readlink -f "$1"); out=$2; console=${3:-/dev/null}
rm -f "$out" || exit 2
root=$(cd "$(dirname "$0")/.." && pwd)
C=$root/c
TC=$(ls -d "$HOME"/toolchains/xpack-riscv-none-elf-gcc-15.2.0-*/bin | head -1)
XGCC=$TC/riscv-none-elf-gcc
CFLAGS="-std=gnu11 -O2 -Wall -march=rv64i -mabi=lp64 -mcmodel=medany"
TCC_ARGS=${TCC_ARGS:-'"-w",'}
MAX_STEPS=${MAX_STEPS:-400000000}
EMU=$root/build/rv64emu
mkdir -p "$root/build" || exit 2
[ -x "$EMU" ] && [ "$EMU" -nt "$root/scripts/rv64emu.c" ] || gcc -std=c11 -O2 -Wall -Wextra -o "$EMU" "$root/scripts/rv64emu.c" || exit 2
work=$(mktemp -d "${TMPDIR:-/tmp}/ref_arena.XXXXXX")
trap 'rm -rf "$work"' EXIT
RT="$C/build/crt0.o $C/build/htif.o $C/build/main.o $C/build/tcc.o"
(
  flock 9
  make -s -C "$C" BUILD=build FLAGS=0 ELF=build/base.elf build/base.elf > "$work/make.log" 2>&1
) 9> "$C/.build.lock" || { echo "ref_arena: image build failed" >&2; tail -20 "$work/make.log" >&2; exit 2; }

attempt() {
  local largs=()
  [ -n "${1:-}" ] && largs=(--largs "$1")
  local extra=()
  [ -n "${EXTRA_FILES:-}" ] && read -r -a extra <<< "$EXTRA_FILES"
  sh "$C/src/gen_embed.sh" --cargs "$TCC_ARGS" "${largs[@]}" --flags 0 --list "$C/build/fs.list" \
      "/prog.c=$src" "${extra[@]}" > "$work/embed.S" &&
  "$XGCC" $CFLAGS -c -o "$work/embed.o" "$work/embed.S" &&
  "$XGCC" $CFLAGS -nostartfiles -T "$C/src/link.ld" -o "$work/prog.elf" \
      $RT "$work/embed.o" -lm > "$work/ld.log" 2>&1 ||
    { echo "ref_arena: image link failed" >&2; cat "$work/ld.log" >&2; exit 2; }
  rm -f "$work/arena"
  "$EMU" "$work/prog.elf" --max-steps "$MAX_STEPS" --stop-at-handoff --arena-out "$work/arena" > "$work/console" 2> "$work/emu.log"
  erc=$?
}
attempt "${TCC_LIBS:-}"
# Only a completed compiler exit may trigger the library retry.
if [ "$erc" -ne 0 ] && grep -Eq '^rv64emu: steps=[0-9]+ compile_steps=- exit=-?[0-9]+ status=exit$' "$work/emu.log" &&
   [ -z "${TCC_LIBS:-}" ] && grep -aq "unresolved reference" "$work/console"; then
  attempt '"-lm",'
fi
cp "$work/console" "$console" || exit 2
if [ -n "${ELF_OUT:-}" ]; then cp "$work/prog.elf" "$ELF_OUT" || exit 2; fi
cat "$work/emu.log" >&2
# Emulator exhaustion/traps are harness failures, not compiler refusals.
[ "$erc" = 124 ] && exit 124
[ "$erc" = 126 ] && exit 126
# An arena file alone is insufficient: require exactly one completion record.
[ "$(grep -c '^rv64emu: steps=' "$work/emu.log")" = 1 ] || {
  echo "ref_arena: missing or duplicate completion record" >&2; exit 2;
}
if [ "$erc" = 0 ] && grep -Eq '^rv64emu: steps=[0-9]+ compile_steps=[0-9]+ exit=- status=handoff$' "$work/emu.log" &&
   [ -s "$work/arena" ]; then
  # Check the serialized length as well as the emulator's checked writes.
  python3 - "$work/arena" <<'PY' || { echo "ref_arena: invalid arena record" >&2; exit 2; }
import re, sys
with open(sys.argv[1], 'rb') as f:
    header = [f.readline(256) for _ in range(5)]
    patterns = [rb'TCCARENA 1\n', rb'base [0-9a-f]+\n', rb'entry [0-9a-f]+\n',
                rb'code [0-9a-f]+ [0-9a-f]+\n', rb'size [0-9a-f]+\n']
    if not all(re.fullmatch(p, h) for p, h in zip(patterns, header)):
        sys.exit(1)
    size = int(header[4].split()[1], 16)
    if size > 4 * 1024 * 1024 or len(f.read(4 * 1024 * 1024 + 1)) != size:
        sys.exit(1)
PY
  cp "$work/arena" "$out" || { rm -f "$out"; exit 2; }
  exit 0
fi
record=$(grep '^rv64emu: steps=' "$work/emu.log")
if [[ "$record" =~ ^rv64emu:\ steps=[0-9]+\ compile_steps=-\ exit=(-?[0-9]+)\ status=exit$ ]] &&
   [ "$erc" -ne 0 ] && [ "$erc" -eq "$(( BASH_REMATCH[1] & 255 ))" ] && [ ! -e "$work/arena" ]; then
  exit 125
fi
echo "ref_arena: incomplete or inconsistent compiler handoff" >&2
exit 2
