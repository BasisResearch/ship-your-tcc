#!/bin/bash
# F3 spot check: the fast reference (build/rv64emu via scripts/ref_arena.sh)
# against the executable Sail model (scripts/run_tcc_sail.sh) on the same
# image: the compile phase's step count, the console bytes and the exit
# status must be equal. One line per program; results appended to
# results/model/sail-spot.tsv. At most 4 programs run at once (each Sail run
# takes minutes and ~120 MB).
#
#   scripts/sail_spot.sh prog.c...
set -u
root=$(cd "$(dirname "$0")/.." && pwd)
out=$root/results/model/sail-spot.tsv
mkdir -p "$root/results/model" "$root/build/sail_spot"
[ -f "$out" ] || printf 'prog\tsail_compile_steps\temu_compile_steps\tsail_exit\temu_exit\tconsole_equal\tverdict\n' > "$out"
one() {
  local src=$1 n w
  n=$(basename "$src" .c)
  w=$root/build/sail_spot/$n
  mkdir -p "$w"
  ORACLE_META=$w/meta "$root/scripts/run_tcc_sail.sh" "$src" > "$w/sail.out" 2> "$w/sail.err"
  local se=$?
  local sc=$(sed -n 's/^compile_steps=//p' "$w/meta" | head -1)
  "$root/scripts/ref_arena.sh" "$src" "$w/emu.arena" "$w/emu.console" 2> "$w/emu.err"
  local ec=$(sed -n 's/.*compile_steps=\([0-9]*\).*/\1/p' "$w/emu.err" | head -1)
  local ee=$(sed -n 's/.*exit=\([0-9-]*\).*/\1/p' "$w/emu.err" | head -1)
  # the Sail runner's stdout is the console with emulator noise removed
  local eq=no
  cmp -s "$w/sail.out" "$w/emu.console" && eq=yes
  local v=same
  [ "$sc" = "$ec" ] && [ "$eq" = yes ] && [ "$se" = "$((ee & 255))" ] || v=DIFF
  printf '%s\t%s\t%s\t%s\t%s\t%s\t%s\n' "$n" "$sc" "$ec" "$se" "$ee" "$eq" "$v" >> "$out"
  echo "$n $v"
}
export -f one
export root out
printf '%s\n' "$@" | xargs -P 4 -I{} bash -c 'one "$@"' _ {}
