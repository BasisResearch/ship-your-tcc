#!/usr/bin/env bash
# csem_regen.sh — regenerate csem/lean_frontend/generated from the Lem model.
#
#   scripts/csem_regen.sh            regenerate in place (then commit the result)
#   scripts/csem_regen.sh --check    regenerate in a scratch copy and diff (Law 7)
#
# The generator is the fork's own recipe, `make lean-prelude-src` (csem/Makefile):
# lem (lem-lean at vendor/CSEM_REV, from the opam switch $CSEM_SWITCH built by
# scripts/csem_toolchain.sh) translates the LEM_SRC_LEAN list of csem/frontend
# to Lean, then the hand-written files listed in
# csem/lean_frontend/handwritten_copy.manifest are copied beside it (Lake
# compiles from generated/), and the lem-sync stamp
# csem/lean_frontend/lem_sync.sha256 is recorded. Everything under generated/
# is generated: edit the .lem sources or the hand-written files, never the
# output.
#
# Without the toolchain, --check exits 0 with a SKIP line (check.sh calls it
# only under --csem); the toolchain-free content checks are
# tools/check_handwritten_sync.sh and tools/check_lem_sync.sh --check-lean.
set -euo pipefail
source "$(dirname "$0")/csem_env.sh"
cd "$_repo"
check=0
[ "${1:-}" = "--check" ] && check=1
if ! csem_have_toolchain; then
  if [ $check = 1 ]; then echo "csem_regen: SKIP (no lem toolchain at $CSEM_SWITCH; scripts/csem_toolchain.sh switch)"; exit 0; fi
  echo "csem_regen: no lem toolchain at $CSEM_SWITCH (scripts/csem_toolchain.sh switch)"; exit 2
fi
want=$(sed -n 's/^lem-lean //p' vendor/CSEM_REV)
have=$(csem_opam lem -v 2>&1)
case "$have" in *"g${want:0:7}"*) ;; *) echo "csem_regen: lem is '$have', pin is $want"; exit 2;; esac

if [ $check = 0 ]; then
  csem_opam make -C csem lean-prelude-src
  echo "csem_regen: regenerated csem/lean_frontend/generated ($(ls csem/lean_frontend/generated/*.lean | wc -l) files)"
  exit 0
fi
tmp=$(mktemp -d "${TMPDIR:-/tmp}/csem-regen.XXXXXX")
trap 'rm -rf "$tmp"' EXIT
# the generator's inputs only (no generated/, no build trees)
mkdir -p "$tmp/csem/lean_frontend"
cp -a csem/Makefile csem/frontend csem/tools "$tmp/csem/"
cp -a csem/lean_frontend/*.lean csem/lean_frontend/handwritten_copy.manifest "$tmp/csem/lean_frontend/"
csem_opam make -s -C "$tmp/csem" lean-prelude-src >/dev/null
if diff -r "$tmp/csem/lean_frontend/generated" csem/lean_frontend/generated >"$tmp/diff" \
   && cmp -s "$tmp/csem/lean_frontend/lem_sync.sha256" csem/lean_frontend/lem_sync.sha256; then
  echo "csem_regen: OK (generated/ = lem($want) of csem/frontend + hand-written copies; $(ls csem/lean_frontend/generated/*.lean | wc -l) files)"
else
  head -40 "$tmp/diff"
  echo "csem_regen: FAIL: csem/lean_frontend/generated drifted from its generator (run scripts/csem_regen.sh)"
  exit 1
fi
