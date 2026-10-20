#!/usr/bin/env bash
# csem_toolchain.sh — build the CSem generator toolchain and the OCaml oracle.
#
#   scripts/csem_toolchain.sh [switch|oracle|all]     (default: all)
#
# switch: an opam switch at $CSEM_SWITCH with OCaml 5.4.0, lem pinned to
#   lem-lean at vendor/CSEM_REV (the fork's README recipe), and the deps of
#   cerberus-lib.opam/cerberus.opam; pristine checkouts of both repositories at
#   their pins under build/csem-src.
# oracle: the Cerberus OCaml driver built in the pristine checkout (the fork's
#   README: make prelude-src, dune build main.exe + cerberus-lib.install +
#   cerberus.install). It is the `--cabs-json` front end of csem_run and the
#   oracle of scripts/csem_lane.sh.
# Everything lands outside git (build/ of the main checkout); heavy steps run
# under a 12 GB systemd scope. Measured 2026-10-20: switch ~20 min, 765 MB.
set -euo pipefail
source "$(dirname "$0")/csem_env.sh"
what=${1:-all}
up_rev=$(sed -n 's/^cerberus-lean //p' "$_repo/vendor/CSEM_REV")
lem_rev=$(sed -n 's/^lem-lean //p' "$_repo/vendor/CSEM_REV")
cap() { systemd-run --user --scope -q -p MemoryMax=12G "$@"; }

checkout() {  # checkout <url> <dir> <rev>
  [ -d "$2/.git" ] || git clone -q "$1" "$2"
  git -C "$2" fetch -q origin 2>/dev/null || true
  git -C "$2" checkout -q "$3"
  [ "$(git -C "$2" rev-parse HEAD)" = "$3" ] || { echo "csem_toolchain: $2 not at $3"; exit 1; }
}

if [ "$what" = switch ] || [ "$what" = all ]; then
  mkdir -p "$(dirname "$CSEM_UPSTREAM")"
  checkout https://github.com/OathTech/cerberus-lean.git "$CSEM_UPSTREAM" "$up_rev"
  checkout https://github.com/OathTech/lem-lean.git "$CSEM_LEMLEAN" "$lem_rev"
  export OPAMJOBS=${OPAMJOBS:-8}
  [ -d "$CSEM_SWITCH/_opam" ] || cap opam switch create "$CSEM_SWITCH" ocaml-base-compiler.5.4.0 --no-switch --no-install -y
  cap opam pin add --switch="$CSEM_SWITCH" lem "git+https://github.com/OathTech/lem-lean.git#$lem_rev" --yes --no-depexts
  cap opam install --switch="$CSEM_SWITCH" --deps-only "$CSEM_UPSTREAM/cerberus-lib.opam" "$CSEM_UPSTREAM/cerberus.opam" --yes --no-depexts
  csem_opam lem -v
fi

if [ "$what" = oracle ] || [ "$what" = all ]; then
  cd "$CSEM_UPSTREAM"
  cap opam exec --switch="$CSEM_SWITCH" -- make prelude-src
  cap opam exec --switch="$CSEM_SWITCH" -- dune build -j 8 backend/driver/main.exe cerberus-lib.install cerberus.install
  "$CSEM_ORACLE" --version
fi
