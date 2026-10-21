#!/usr/bin/env bash
# fs_probe_sail.sh — run one file-system probe under tcc on Sail (ground truth)
# and record stdout, exit code and wall time (results/csem/f1b-fs.md).
#
#   scripts/fs_probe_sail.sh OUTDIR prog.c
#
# Writes OUTDIR/<name>.sail.out, OUTDIR/<name>.sail.rc, OUTDIR/<name>.sail.time.
# The runner is the MAIN checkout's scripts/run_tcc_sail.sh (it has the built
# image; $TCC_SAIL_RUNNER overrides). Keep at most ~4 of these concurrent.
set -u
out=$1; src=$(readlink -f "$2")
runner=${TCC_SAIL_RUNNER:-$HOME/Documents/code/ship-your-tcc/scripts/run_tcc_sail.sh}
name=$(basename "$src" .c)
mkdir -p "$out"
t0=$(date +%s)
"$runner" "$src" > "$out/$name.sail.out" 2> "$out/$name.sail.err"
rc=$?
t1=$(date +%s)
echo "$rc" > "$out/$name.sail.rc"
echo "$((t1 - t0))" > "$out/$name.sail.time"
echo "$name rc=$rc $((t1 - t0))s"
