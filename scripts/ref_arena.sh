#!/bin/bash
# F3 fast reference: the tcc -run arena of a program, from the SAME image the
# Sail model runs (c/build runtime + gen_embed.sh's .embed for /prog.c, as in
# scripts/run_tcc_sail.sh), executed by build/rv64emu (scripts/rv64emu.c).
#
#   scripts/ref_arena.sh prog.c OUT.arena [OUT.console]
#
# Writes OUT.arena (rv64emu's TCCARENA format) when tcc reaches the hand-off,
# else no arena; the console (tcc's diagnostics, then the program's output)
# goes to OUT.console (default /dev/null). Exit status: 0 when the arena was
# written, 125 when tcc stopped first (the console says why), 2 on harness
# errors. With TCC_LIBS unset, a program that fails to link with an
# unresolved reference is retried with -lm, as run_tcc_sail.sh does.
# Env: MAX_STEPS (default 400000000), TCC_ARGS (default '"-w",'),
#      TCC_LIBS (libraries after /prog.c, as gen_embed.sh --largs),
#      EXTRA_FILES (more embedded files, space-separated /path=hostfile
#      specs, e.g. EXTRA_FILES='/prog.o=x.o' TCC_ARGS='"-w","/prog.o",'),
#      ELF_OUT (keep the per-program ELF there).
set -u
src=$(readlink -f "$1"); out=$2; console=${3:-/dev/null}
root=$(cd "$(dirname "$0")/.." && pwd)
C=$root/c
TC=$(ls -d "$HOME"/toolchains/xpack-riscv-none-elf-gcc-15.2.0-*/bin | head -1)
XGCC=$TC/riscv-none-elf-gcc
CFLAGS="-std=gnu11 -O2 -Wall -march=rv64i -mabi=lp64 -mcmodel=medany"
TCC_ARGS=${TCC_ARGS:-'"-w",'}
MAX_STEPS=${MAX_STEPS:-400000000}
EMU=$root/build/rv64emu
[ -x "$EMU" ] && [ "$EMU" -nt "$root/scripts/rv64emu.c" ] || gcc -O2 -o "$EMU" "$root/scripts/rv64emu.c" || exit 2
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
  rm -f "$out"
  "$EMU" "$work/prog.elf" --max-steps "$MAX_STEPS" --arena-out "$out" > "$work/console" 2> "$work/emu.log"
  erc=$?
}
attempt "${TCC_LIBS:-}"
if [ ! -f "$out" ] && [ -z "${TCC_LIBS:-}" ] && grep -aq "unresolved reference" "$work/console"; then
  attempt '"-lm",'
fi
cp "$work/console" "$console"
[ -n "${ELF_OUT:-}" ] && cp "$work/prog.elf" "$ELF_OUT"
cat "$work/emu.log" >&2
[ -f "$out" ] && exit 0
# Emulator exhaustion/traps are harness failures, not compiler refusals.
[ "$erc" = 124 ] && exit 124
[ "$erc" = 126 ] && exit 126
exit 125
