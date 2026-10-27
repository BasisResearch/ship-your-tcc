#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
export CSEM_SWITCH="$PWD/build/csem-704c4e69-e6d8db56/opam"
export CSEM_UPSTREAM="$PWD/build/upstream-cerberus"
export CSEM_LEMLEAN="$PWD/build/upstream-lem"
export OPAMJOBS=4
[ "$(git -C "$CSEM_LEMLEAN" rev-parse HEAD)" = e6d8db5680962f49060257d6c2bcb13c284b910f ]
[ "$(git -C "$CSEM_UPSTREAM" rev-parse HEAD)" = 704c4e697845127bced66c93fa0763eb51527ca4 ]
[ -d "$CSEM_SWITCH/_opam" ] || opam switch create "$CSEM_SWITCH" ocaml-system.5.4.0 --no-switch --yes
opam pin add --switch="$CSEM_SWITCH" lem "$CSEM_LEMLEAN" --yes --no-depexts
opam install --switch="$CSEM_SWITCH" --deps-only "$CSEM_UPSTREAM/cerberus-lib.opam" "$CSEM_UPSTREAM/cerberus.opam" --yes --no-depexts
opam exec --switch="$CSEM_SWITCH" -- lem -v
opam exec --switch="$CSEM_SWITCH" -- make -C "$CSEM_UPSTREAM" prelude-src
(cd "$CSEM_UPSTREAM" && opam exec --switch="$CSEM_SWITCH" -- dune build -j 4 backend/driver/main.exe cerberus-lib.install cerberus.install)
