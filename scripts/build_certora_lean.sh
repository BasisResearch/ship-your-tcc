#!/bin/bash
# Build the Lean side, capped at 12G and 8 cores (shared box).
#   scripts/build_certora_lean.sh            -> Certora export/: CCLib + Clightdefs
#   scripts/build_certora_lean.sh runner     -> the root project's clight_run executable
#                                               (lean/ClightRun, on the vendored patched CCLib)
set -euo pipefail
here=$(cd "$(dirname "$0")" && pwd)
root=$(cd "$here/.." && pwd)
common=$(git -C "$root" rev-parse --path-format=absolute --git-common-dir)
lock=${SYT_LAKE_LOCK:-$(dirname "$common")/build/lake.lock}
mkdir -p "$(dirname "$lock")"
case "${1:-cclib}" in
  cclib)  dir="$root/build/certora/src/export"; targets=(CCLib Clightdefs) ;;
  runner) dir="$root"; targets=(clight_run) ;;
  *) echo "usage: $0 [cclib|runner]" >&2; exit 2 ;;
esac
cd "$dir"
start=$(date +%s)
flock "$lock" systemd-run --user --scope -q -p MemoryMax=12G \
  taskset -c 0-7 env LEAN_NUM_THREADS=4 lake build "${targets[@]}"
echo "lake build ${targets[*]} in $dir took $(( $(date +%s) - start ))s"
