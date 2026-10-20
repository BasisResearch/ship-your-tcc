#!/usr/bin/env bash
# build_ccp.sh — build the CompileCut library (TccCompileCut): the checkpoint
# states, then each heavy generated module ALONE (a 25,000-step segment needs
# ~7 GB; lake would run them in parallel): the segments, the table blocks, the
# pin blocks; then the joins, Tcc.Ccp.SumCut (compileCut_proved) and
# Tcc.Ccp.EndToEnd (tcc_run_sum_closed). Every lake build runs under the shared
# lock and a 12 GB cap (CLAUDE.md law 5). About 2.5 CPU-hours from scratch;
# already-built modules are skipped by lake.
#
#   scripts/build_ccp.sh
#
# Per-module wall seconds, user seconds and max RSS (KB) go to
# build/ccp/full.tsv; a failing module's log is kept as build/ccp/fail_<module>.log.
set -uo pipefail
cd "$(dirname "$0")/.." || exit 2
LOCK="${SYT_LAKE_LOCK:-$HOME/Documents/code/ship-your-tcc/build/lake.lock}"
mkdir -p build/ccp
: > build/ccp/full.tsv
status=0
lb() {  # one lake build, timed
  /usr/bin/time -o build/ccp/mod_time.txt -f "%e	%U	%M" \
    flock "$LOCK" systemd-run --user --scope -q -p MemoryMax=12G lake build "$@" > build/ccp/mod_last.log 2>&1
  local rc=$?
  printf '%s\t%s\trc=%s\n' "$*" "$(tail -1 build/ccp/mod_time.txt)" "$rc" >> build/ccp/full.tsv
  if [ "$rc" != 0 ]; then cp build/ccp/mod_last.log "build/ccp/fail_${1##*.}.log"; status=1; fi
  return $rc
}
N=$(ls Tcc/Ccp/Sum/Seg/C*.lean | wc -l)
lb "Tcc.Ccp.Sum.St.S$N" Tcc.Ccp.Sum.Code Tcc.Ccp.KernelRfl Tcc.Ccp.Lemmas Tcc.Ccp.Sound Tcc.Capture.Sum.Elf \
  || exit 1
for i in $(seq 0 $((N - 1))); do lb "Tcc.Ccp.Sum.Seg.C$i"; done
for f in $(ls Tcc/Ccp/Sum/Table/T*.lean | sort -V); do lb "Tcc.Ccp.Sum.Table.$(basename "$f" .lean)"; done
for f in $(ls Tcc/Ccp/Sum/Pin/[MC]*.lean | sort -V); do lb "Tcc.Ccp.Sum.Pin.$(basename "$f" .lean)"; done
[ "$status" = 0 ] || { echo "build_ccp: failures (build/ccp/full.tsv)"; exit 1; }
lb Tcc.Ccp.Sum.Seg.Join Tcc.Ccp.Sum.Table.All Tcc.Ccp.Sum.Pin.All || exit 1
lb Tcc.Ccp.SumCut || exit 1
lb Tcc.Ccp.EndToEnd || exit 1
awk -F'\t' '{w += $2; u += $3} END {printf "build_ccp: %d builds, %.0f s wall, %.0f s user\n", NR, w, u}' build/ccp/full.tsv
