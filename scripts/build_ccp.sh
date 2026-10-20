#!/usr/bin/env bash
# build_ccp.sh — build the CompileCut pilot's generated chain (lib TccCompileCut):
# the checkpoint states, then each 25,000-step segment ALONE (each needs ~7 GB;
# lake would run them in parallel), then the join `Tcc.Ccp.Sum.run_all`.
# Every lake build runs under the shared lock and a 12 GB cap (CLAUDE.md law 5).
#
#   scripts/build_ccp.sh [first-segment]
#
# Per-segment wall seconds, user seconds and max RSS (KB) go to
# build/ccp/full.tsv; a failing segment's log is kept as build/ccp/seg_fail_<i>.log.
set -uo pipefail
cd "$(dirname "$0")/.." || exit 2
LOCK="${SYT_LAKE_LOCK:-$HOME/Documents/code/ship-your-tcc/build/lake.lock}"
mkdir -p build/ccp
N=$(ls Tcc/Ccp/Sum/Seg/C*.lean | wc -l)
FIRST="${1:-0}"
lb() { flock "$LOCK" systemd-run --user --scope -q -p MemoryMax=12G lake build "$@"; }

/usr/bin/time -f "states: %e s wall, %U s user, %M KB" \
  bash -c "$(declare -f lb); LOCK='$LOCK'; lb Tcc.Ccp.Sum.St.S$N Tcc.Ccp.Sum.Code Tcc.Ccp.KernelRfl Tcc.Ccp.Lemmas" \
  > build/ccp/states.log 2>&1
tail -1 build/ccp/states.log

for i in $(seq "$FIRST" $((N - 1))); do
  /usr/bin/time -o build/ccp/seg_time.txt -f "%e	%U	%M" \
    bash -c "$(declare -f lb); LOCK='$LOCK'; lb Tcc.Ccp.Sum.Seg.C$i" > build/ccp/seg_last.log 2>&1
  rc=$?
  printf 'C%s\t%s\trc=%s\n' "$i" "$(cat build/ccp/seg_time.txt | tail -1)" "$rc" >> build/ccp/full.tsv
  [ "$rc" = 0 ] || cp build/ccp/seg_last.log "build/ccp/seg_fail_$i.log"
done

/usr/bin/time -f "join: %e s wall, %U s user, %M KB" \
  bash -c "$(declare -f lb); LOCK='$LOCK'; lb Tcc.Ccp.Sum.Seg.Join" > build/ccp/join.log 2>&1
tail -1 build/ccp/join.log
