#!/bin/bash
# Run the F2 preprocessing repros (results/repro/tcc_pp_*.c) under `tcc -run`
# on the Sail image (scripts/run_tcc_sail.sh); prints each program's output
# and exit status (125: tcc rejected the program).
set -u
root=$(cd "$(dirname "$0")/.." && pwd)
for f in "$root"/results/repro/tcc_pp_*.c; do
  echo "== $(basename "$f")"
  timeout 3600 "$root/scripts/run_tcc_sail.sh" "$f"
  echo "exit=$?"
done
