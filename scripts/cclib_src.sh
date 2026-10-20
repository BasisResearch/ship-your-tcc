#!/bin/bash
# Usage: scripts/cclib_src.sh <outdir>
#        scripts/cclib_src.sh --check     (regenerate into a temp dir and diff
#                                          against vendor/certora-export-patched)
#
# Makes <outdir> a copy of the pristine vendor/certora-export (Certora/clean
# export/ @ vendor/CERTORA_REV) with patches/certora/*-cclib-*.patch applied in
# name order (after scripts/tcc_src.sh). The root lakefile builds CCLib and
# Clightdefs from the committed output vendor/certora-export-patched, so a fresh
# clone builds with plain `lake build`; check.sh runs --check, so the committed
# copy is always exactly pristine + patches.
#
# The cclib patches are git diffs against the Certora checkout (paths
# a/export/<file>), so they also apply to build/certora/src with -p1; here they
# are applied with `patch -p2` from inside the copy. <outdir>/.cclib_src_stamp
# records a hash of the inputs; if it matches nothing is done, otherwise <outdir>
# is recreated (so never point it at a directory holding anything else).
set -euo pipefail
here=$(cd "$(dirname "$0")/.." && pwd)
if [ "${1:-}" = "--check" ]; then
  tmp=$(mktemp -d "${TMPDIR:-/tmp}/cclib_src.XXXXXX")
  trap 'rm -rf "$tmp"' EXIT
  "$0" "$tmp/out"
  if ! diff -r "$tmp/out" "$here/vendor/certora-export-patched" > "$tmp/diff"; then
    head -40 "$tmp/diff" >&2
    echo "cclib_src.sh: vendor/certora-export-patched differs from vendor/certora-export + patches" >&2
    exit 1
  fi
  exit 0
fi
out=${1:?usage: $0 <outdir> | --check}
shopt -s nullglob
patches=("$here"/patches/certora/*-cclib-*.patch)
src="$here/vendor/certora-export"

stamp=$( { cat "$here/vendor/CERTORA_REV"; (cd "$src" && find . -type f | LC_ALL=C sort | xargs sha256sum); for p in "${patches[@]}"; do basename "$p"; cat "$p"; done; } | sha256sum | cut -d' ' -f1)
if [ -f "$out/.cclib_src_stamp" ] && [ "$(cat "$out/.cclib_src_stamp")" = "$stamp" ]; then
  exit 0
fi
if [ -e "$out" ] && [ ! -f "$out/.cclib_src_stamp" ] && [ -n "$(ls -A "$out" 2>/dev/null)" ]; then
  echo "cclib_src.sh: refusing to overwrite non-empty $out (no stamp)" >&2
  exit 1
fi
rm -rf "$out"
mkdir -p "$out"
cp -a "$src/." "$out/"
for p in "${patches[@]}"; do
  patch -s -p2 -d "$out" --no-backup-if-mismatch < "$p"
done
echo "$stamp" > "$out/.cclib_src_stamp"
