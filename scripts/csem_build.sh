#!/usr/bin/env bash
# csem_build.sh — build CSem targets under the 12 GB cap, serialised on the
# shared Lake lock, and report wall time and the scope's peak memory.
#
#   scripts/csem_build.sh [TARGET...]    (default: the CSem libraries and csem_run)
#
# The peak is the cgroup's memory.peak (all Lean workers of the build together),
# read from inside the transient scope just before it exits.
set -uo pipefail
cd "$(dirname "$0")/.."
targets=("$@")
[ ${#targets[@]} -gt 0 ] || targets=(LemLib LemLibTests CerberusLean csem_run)
lock=${SYT_LAKE_LOCK:-$HOME/Documents/code/ship-your-tcc/build/lake.lock}
mkdir -p "$(dirname "$lock")"
start=$(date +%s)
flock "$lock" systemd-run --user --scope -q -p MemoryMax=12G bash -c '
  lake build "$@"; rc=$?
  cg=$(cut -d: -f3 /proc/self/cgroup)
  [ -r "/sys/fs/cgroup$cg/memory.peak" ] && echo "csem_build: peak $(( $(cat /sys/fs/cgroup$cg/memory.peak) / 1048576 )) MiB"
  exit $rc' _ "${targets[@]}"
rc=$?
echo "csem_build: ${targets[*]}: exit $rc, wall $(( $(date +%s) - start )) s"
exit $rc
