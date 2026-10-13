#!/bin/bash
# Run both Certora oracles (and host gcc -O0 -funsigned-char as a sanity check)
# on a set of C programs and compare stdout + exit status.
#   scripts/certora_smoke.sh [prog.c ...]     (default: c/smoke/*.c)
set -uo pipefail
here=$(cd "$(dirname "$0")" && pwd)
. "$here/certora_env.sh"
progs=("$@")
[ ${#progs[@]} -eq 0 ] && progs=("$SYT_ROOT"/c/smoke/*.c)
tmp=$(mktemp -d "${TMPDIR:-/tmp}/csmoke.XXXXXX"); trap 'rm -rf "$tmp"' EXIT
ms() { echo $(( ($2 - $1) / 1000000 )); }
printf '%-14s %-6s %-6s %-6s %-9s %-9s %-9s %s\n' prog interp lean gcc interp_ms lean_ms steps verdict
for p in "${progs[@]}"; do
  n=$(basename "$p" .c)
  t0=$(date +%s%N); "$here/run_ccomp_interp.sh" "$p" > "$tmp/$n.cc" 2> "$tmp/$n.cc.err"; rc1=$?
  t1=$(date +%s%N); "$here/run_lean_clight.sh" --stats "$p" > "$tmp/$n.lean" 2> "$tmp/$n.lean.err"; rc2=$?
  t2=$(date +%s%N)
  gcc -O0 -w -funsigned-char -o "$tmp/$n.exe" "$p" 2>/dev/null && { "$tmp/$n.exe" > "$tmp/$n.gcc"; rc3=$?; } || rc3=NA
  steps=$(sed -n 's/.*steps=\([0-9]*\).*/\1/p' "$tmp/$n.lean.err" | head -1)
  v=AGREE
  if [ "$rc1" != "$rc2" ] || ! cmp -s "$tmp/$n.cc" "$tmp/$n.lean"; then v=DIFF-interp-lean; fi
  if [ "$rc3" != NA ] && { [ "$rc1" != "$rc3" ] || ! cmp -s "$tmp/$n.cc" "$tmp/$n.gcc"; }; then v="$v,DIFF-gcc"; fi
  printf '%-14s %-6s %-6s %-6s %-9s %-9s %-9s %s\n' "$n" "$rc1" "$rc2" "$rc3" "$(ms $t0 $t1)" "$(ms $t1 $t2)" "${steps:--}" "$v"
  if [ "$v" != AGREE ] || [ -n "${VERBOSE:-}" ]; then
    echo "  --- ccomp -interp (rc=$rc1)"; sed 's/^/  | /' "$tmp/$n.cc"; grep -v '^\[run_ccomp_interp\] total' "$tmp/$n.cc.err" | sed 's/^/  ! /'
    echo "  --- lean clight (rc=$rc2)"; sed 's/^/  | /' "$tmp/$n.lean"; grep -v 'steps=\|clightgen_ms' "$tmp/$n.lean.err" | sed 's/^/  ! /'
    [ "$rc3" != NA ] && { echo "  --- gcc (rc=$rc3)"; sed 's/^/  | /' "$tmp/$n.gcc"; }
  fi
done
