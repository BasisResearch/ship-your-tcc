#!/bin/bash
# Usage: scripts/tcc_src.sh <outdir>
#
# Makes <outdir> a copy of the pinned vendor/tinycc (43c7708) with
# patches/*.patch applied in name order. Idempotent: <outdir>/.tcc_src_stamp
# records a hash of vendor/tinycc's files plus the patches; if it matches,
# nothing is done, otherwise <outdir> is recreated from scratch (so never
# point it at a directory holding anything else).
#
# The patches are git-format diffs against vendor/tinycc (paths a/<file>,
# b/<file>); they are applied with `patch -p1` so no git is needed.
set -euo pipefail
here=$(cd "$(dirname "$0")/.." && pwd)
out=${1:?usage: $0 <outdir>}
shopt -s nullglob
patches=("$here"/patches/*.patch)

stamp=$( { cat "$here/vendor/TINYCC_REV"; (cd "$here/vendor/tinycc" && find . -type f | LC_ALL=C sort | xargs sha256sum); for p in "${patches[@]}"; do basename "$p"; cat "$p"; done; } | sha256sum | cut -d' ' -f1)
if [ -f "$out/.tcc_src_stamp" ] && [ "$(cat "$out/.tcc_src_stamp")" = "$stamp" ]; then
  exit 0
fi
if [ -e "$out" ] && [ ! -f "$out/.tcc_src_stamp" ] && [ -n "$(ls -A "$out" 2>/dev/null)" ]; then
  echo "tcc_src.sh: refusing to overwrite non-empty $out (no stamp)" >&2
  exit 1
fi
rm -rf "$out"
mkdir -p "$out"
cp -a "$here/vendor/tinycc/." "$out/"
for p in "${patches[@]}"; do
  patch -s -p1 -d "$out" --no-backup-if-mismatch < "$p"
done
echo "$stamp" > "$out/.tcc_src_stamp"
