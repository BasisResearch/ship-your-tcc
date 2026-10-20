#!/bin/bash
# Build the generated replay (Tcc/Prog/Sum/Machine/Part*.lean) a few parts at a
# time: lake has no job limit, and every part is a Lean process of 1-3 GB, so
# building all of them at once under the 12 GB cap would be OOM-killed.
#   scripts/build_replay.sh [parts-per-batch]
set -u
cd "$(dirname "$0")/.." || exit 2
B=${1:-4}
LOCK=${SYT_LAKE_LOCK:-$HOME/Documents/code/ship-your-tcc/build/lake.lock}
mkdir -p "$(dirname "$LOCK")"
run() { flock "$LOCK" systemd-run --user --scope -q -p MemoryMax=12G lake build "$@"; }
run Tcc.Prog.Sum.Machine.Defs Tcc.Prog.Sum.Machine.Blocks Tcc.Boot.Step Tcc.Capture.Sum.Cut || exit 1
parts=$(ls Tcc/Prog/Sum/Machine/Part*.lean | sed 's|Tcc/Prog/Sum/Machine/\(Part[0-9]*\)\.lean|Tcc.Prog.Sum.Machine.\1|' | sort -V)
# every batch is built even after a failure, so one pass reports every failing part
fail=0
batch=()
for p in $parts; do
  batch+=("$p")
  if [ ${#batch[@]} -ge "$B" ]; then run "${batch[@]}" || fail=1; batch=(); fi
done
[ ${#batch[@]} -eq 0 ] || run "${batch[@]}" || fail=1
[ $fail = 0 ] || exit 1
run Tcc.Prog.Sum.Machine.Replay Tcc.EndToEnd.Sum
