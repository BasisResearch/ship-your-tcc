#!/usr/bin/env bash
# csem_import.sh — (re)import our fork's upstream material into csem/.
#
#   scripts/csem_import.sh [--check] UPSTREAM_CERBERUS_LEAN LEM_LEAN
#
# UPSTREAM_CERBERUS_LEAN is a checkout of OathTech/cerberus-lean at the pin in
# vendor/CSEM_REV; LEM_LEAN a checkout of OathTech/lem-lean at the LemLib pin
# in the same file. The selection below is what the Lean pipeline, its
# generation from Lem, the fork's gates and its minimal differential lane need
# (DESIGN-COMPILER.md §2, phase F0); the 165 MB of CHERI, web, docs history,
# Coq, BMC and large test corpora are not imported. csem/lean_frontend/generated
# is NOT imported: it is produced by scripts/csem_regen.sh (Law 7).
#
# This script records the import; our changes are ordinary commits on top
# (csem/PORT-4.34.md lists them), so after the first import it is only used
# with --check against a pristine checkout to list what we changed.
set -euo pipefail
cd "$(dirname "$0")/.."
check=0
[ "${1:-}" = "--check" ] && { check=1; shift; }
up=${1:?usage: csem_import.sh [--check] UPSTREAM_CERBERUS_LEAN LEM_LEAN}
lem=${2:?usage: csem_import.sh [--check] UPSTREAM_CERBERUS_LEAN LEM_LEAN}
want_up=$(sed -n 's/^cerberus-lean //p' vendor/CSEM_REV)
want_lem=$(sed -n 's/^lem-lean //p' vendor/CSEM_REV)
got_up=$(git -C "$up" rev-parse HEAD)
got_lem=$(git -C "$lem" rev-parse HEAD)
[ "$got_up" = "$want_up" ] || { echo "csem_import: $up is at $got_up, pin is $want_up"; exit 1; }
[ "$got_lem" = "$want_lem" ] || { echo "csem_import: $lem is at $got_lem, pin is $want_lem"; exit 1; }

dest=csem
if [ $check = 1 ]; then dest=$(mktemp -d); trap 'rm -rf "$dest"' EXIT; fi

copy() {  # copy <src-root> <relative path>... into $dest, preserving paths
  local root=$1; shift
  for p in "$@"; do
    mkdir -p "$dest/$(dirname "$p")"
    cp -a "$root/$p" "$dest/$p"
  done
}

# Cerberus: licence, notices, the Lem generation recipe
copy "$up" LICENSE THIRD_PARTY_FILES.md Makefile
# the Lem model the Lean is generated from (frontend + the concurrency
# modules the Makefile's LEM_SRC names)
mkdir -p "$dest/frontend/model" "$dest/frontend/concurrency"
cp -a "$up"/frontend/model/*.lem "$dest/frontend/model/"
cp -a "$up"/frontend/model/ail "$dest/frontend/model/"
cp -a "$up"/frontend/concurrency/*.lem "$dest/frontend/concurrency/"
# the C parser (menhir grammar, lexer, typedef feedback): F1's reference
copy "$up" parsers/c
# the Core standard library, the implementation file and the C library the
# Lean driver loads at run time (the oracle installs the same files)
copy "$up" runtime/libcore runtime/libc runtime/dune
# the Lean frontend: hand-written seams, copy manifest, native digest, unit
# tests and gate executables, the hash-frozen differential fixtures, and the
# top-level design/validation records (docs/ history is not imported)
mkdir -p "$dest/lean_frontend"
cp -a "$up"/lean_frontend/*.lean "$dest/lean_frontend/"
for f in README.md DESIGN.md VALIDATION.md SUPPORTED.md CONTRACT.md TODO.md \
         handwritten_copy.manifest lakefile.toml lake-manifest.json lean-toolchain; do
  copy "$up" "lean_frontend/$f"
done
copy "$up" lean_frontend/native/md5.c lean_frontend/test lean_frontend/corpus
# gates and lanes (scripts/ whole: the gates source each other), freshness
# and sync tools
copy "$up" scripts
copy "$up" tools/check_handwritten_sync.sh tools/check_lem_sync.sh tools/check_driver_fresh.sh
# the corpora the gates and the minimal lane need
copy "$up" tests/minimal tests/fixtures
# LemLib (lem-lean lean-lib/) with Lem's licence; vendored (csem/lemlib)
mkdir -p "$dest/lemlib"
cp -a "$lem"/lean-lib/LemLib "$lem"/lean-lib/*.lean "$lem"/lean-lib/NOTICE.md \
      "$lem"/lean-lib/README.md "$dest/lemlib/"
cp -a "$lem"/LICENSE "$dest/lemlib/LICENSE"

if [ $check = 1 ]; then
  # list paths that differ from the pristine import (our commits on top);
  # generated/ and our own files are not part of the import
  diff -rq "$dest" csem | grep -v -e 'Only in csem' || true
fi
