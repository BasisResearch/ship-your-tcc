#!/bin/bash
# Populate corpus/{compcert,tcc}/ from their sources.
#
#   scripts/collect_corpus.sh            fetch sources if missing, copy the
#                                        programs the committed manifests include
#   scripts/collect_corpus.sh --refresh  also re-run every check and rewrite
#                                        corpus/{compcert,tcc}/manifest.tsv
#
# Sources:
#   build/compcert-tests  AbsInt/CompCert-small-tests @ COMPCERT_TESTS_REV,
#                         the commit CompCert v3.17 pins as its test/ submodule
#                         (Certora/clean is a CompCert 3.17 fork)
#   vendor/tinycc/tests   the vendored tinycc (vendor/TINYCC_REV)
# corpus/csmith/ is committed as is (scripts/gen_csmith.sh regenerates it).
# The copied .c files are gitignored; the manifests are committed.
set -eu
root=$(cd "$(dirname "$0")/.." && pwd)
COMPCERT_TESTS_URL=https://github.com/AbsInt/CompCert-small-tests
COMPCERT_TESTS_REV=081175df374260304a5cd78dd3dfaded93dceb41
d=$root/build/compcert-tests
if [ ! -d "$d/.git" ]; then
  git clone -q "$COMPCERT_TESTS_URL" "$d"
fi
git -C "$d" checkout -q "$COMPCERT_TESTS_REV"
if [ "${1:-}" = --refresh ]; then
  python3 "$root/scripts/collect_corpus.py" manifest
fi
python3 "$root/scripts/collect_corpus.py" copy
