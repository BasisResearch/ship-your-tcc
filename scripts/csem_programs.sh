#!/usr/bin/env bash
# csem_programs.sh — run C programs through csem_run (CSem, Lean 4.34) and the
# Cerberus OCaml oracle (--nolibc, exhaustive, the same mode), side by side.
#
#   scripts/csem_programs.sh [--libc] [FILE.c...]   (default: c/tests/p3/sum.c c/tests/run/*.c)
#
# --libc: both sides link the C library (oracle without --nolibc; csem_run with
# the fork's --libc artifacts from scripts/csem_libc_prep.sh).
#
# Prints one block per program: the oracle's verdict lines, csem_run's, the
# exit codes, wall times, and whether the verdict sequences agree.
set -uo pipefail
source "$(dirname "$0")/csem_env.sh"
cd "$_repo"
libc=()
nolibc=(--nolibc)
if [ "${1:-}" = "--libc" ]; then
  shift
  mapfile -t libc < <(scripts/csem_libc_prep.sh)
  nolibc=()
fi
files=("$@")
[ ${#files[@]} -gt 0 ] || files=(c/tests/p3/sum.c c/tests/run/*.c)
scratch=$(mktemp -d "${TMPDIR:-/tmp}/csem-programs.XXXXXX")
trap 'rm -rf "$scratch"' EXIT
T=${TIMEOUT_SECS:-120}
for f in "${files[@]}"; do
  s=$(date +%s.%N)
  o=$(timeout "$T" "$CSEM_ORACLE" --runtime="$CSEM_RUNTIME" "${nolibc[@]}" --exec --batch --mode=exhaustive "$f" 2>&1); orc=$?
  m=$(date +%s.%N)
  l=$(LEAN_ABORT_ON_PANIC=1 TMPDIR=$scratch timeout "$T" "$CSEM_LEAN_BIN" "${libc[@]}" "$f" 2>&1); lrc=$?
  e=$(date +%s.%N)
  ov=$(printf '%s\n' "$o" | grep -E '^(Defined|Undefined|Error) \{')
  lv=$(printf '%s\n' "$l" | grep -E '^(Defined|Undefined|Error) \{')
  [ "$ov" = "$lv" ] && [ -n "$ov" ] && agree=agree || agree=DIFFER
  printf '== %s: %s (oracle exit %s, %.1fs; csem_run exit %s, %.1fs)\n' "$f" "$agree" "$orc" \
    "$(echo "$m - $s" | bc)" "$lrc" "$(echo "$e - $m" | bc)"
  printf '  oracle:   %s\n' "$(printf '%s\n' "${ov:-$(printf '%s' "$o" | tail -2)}" | sort | uniq -c | head -3)"
  printf '  csem_run: %s\n' "$(printf '%s\n' "${lv:-$(printf '%s' "$l" | tail -2)}" | sort | uniq -c | head -3)"
done
