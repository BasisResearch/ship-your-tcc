#!/usr/bin/env bash
# csem_libc_prep.sh — the two C-library artifacts of the fork's --libc mode,
# for csem_run (after csem/scripts/libc_prep.sh, whose header is the contract):
#
#   1. libc.core: the oracle's unlinked libc Core dump
#      (cerberus --nolibc --pp=core of the oracle's libc.co), checked against
#      the fork's content-hash pin tests/libc/libc.core.sha256 of the pristine
#      upstream checkout;
#   2. the 12 libc metadata TUs as cabs-json, in the oracle's link order.
#
#   scripts/csem_libc_prep.sh [OUTDIR]     (default: build/csem-libc)
#
# Prints the csem_run arguments (--libc FILE --libc-tu J1 ... --libc-tu J12)
# on stdout, one per line. Outputs are oracle-produced and not committed.
set -euo pipefail
source "$(dirname "$0")/csem_env.sh"
out=${1:-$_repo/build/csem-libc}
mkdir -p "$out"
up=$CSEM_UPSTREAM
pin_sha=$(awk '{print $1}' "$up/tests/libc/libc.core.sha256")
"$CSEM_ORACLE" --runtime="$CSEM_RUNTIME" --nolibc --pp=core --pp_core_out="$out/libc.core" \
  "$up/_build/default/runtime/libc/libc.co" >/dev/null
got=$(sha256sum "$out/libc.core" | awk '{print $1}')
[ "$got" = "$pin_sha" ] || { echo "csem_libc_prep: libc.core dump $got differs from the fork's pin $pin_sha" >&2; exit 1; }
echo --libc
echo "$out/libc.core"
for tu in ctype stdio stdlib string time utime unistd stat uio internal vfscanf signal; do
  (cd "$up/runtime/libc" && "$CSEM_ORACLE" --runtime="$CSEM_RUNTIME" --nolibc --cabs-json \
     -I include -I include/posix "src/$tu.c" > "$out/libc_$tu.json")
  echo --libc-tu
  echo "$out/libc_$tu.json"
done
