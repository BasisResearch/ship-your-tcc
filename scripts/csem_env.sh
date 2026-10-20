# csem_env.sh — shared paths for the CSem (csem/) scripts; source it.
#
# CSEM_SWITCH   the opam switch with OCaml 5.4.0, lem pinned to lem-lean at
#               vendor/CSEM_REV, and the Cerberus OCaml deps (built by
#               scripts/csem_toolchain.sh; outside git, under the main
#               checkout's build/ so worktrees share it)
# CSEM_UPSTREAM a pristine OathTech/cerberus-lean checkout at the pin, where the
#               OCaml oracle (`cerberus`) is built
# CSEM_ORACLE   the oracle binary; CSEM_RUNTIME its install prefix (the
#               `--runtime` both engines are given)
_repo=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
_main=$(cd "$_repo" && git rev-parse --path-format=absolute --git-common-dir 2>/dev/null | sed 's#/\.git$##')
[ -n "$_main" ] || _main=$_repo
: "${CSEM_SWITCH:=$_main/build/csem-opam}"
: "${CSEM_UPSTREAM:=$_main/build/csem-src/cerberus-lean}"
: "${CSEM_LEMLEAN:=$_main/build/csem-src/lem-lean}"
: "${CSEM_ORACLE:=$CSEM_UPSTREAM/_build/default/backend/driver/main.exe}"
: "${CSEM_RUNTIME:=$CSEM_UPSTREAM/_build/install/default}"
: "${CSEM_LEAN_BIN:=$_repo/.lake/build/bin/csem_run}"
export CSEM_SWITCH CSEM_UPSTREAM CSEM_LEMLEAN CSEM_ORACLE CSEM_RUNTIME CSEM_LEAN_BIN
csem_have_toolchain() { [ -x "$CSEM_SWITCH/_opam/bin/lem" ]; }
csem_opam() { opam exec --switch="$CSEM_SWITCH" -- "$@"; }
