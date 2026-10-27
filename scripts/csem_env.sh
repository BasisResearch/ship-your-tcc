# csem_env.sh — shared paths for the CSem (csem/) scripts; source it.
#
# CSEM_SWITCH   the opam switch with OCaml 5.4.0, lem pinned to lem-lean at
#               vendor/CSEM_REV, and the Cerberus OCaml deps (built by
#               scripts/csem_toolchain.sh; outside git, under this worktree
#               in a pin-addressed directory so another lane is never repinned)
# CSEM_UPSTREAM a pristine OathTech/cerberus-lean checkout at the pin, where the
#               OCaml oracle (`cerberus`) is built
# CSEM_ORACLE   the oracle binary; CSEM_RUNTIME its install prefix (the
#               `--runtime` both engines are given)
_repo=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
_main=$(cd "$_repo" && git rev-parse --path-format=absolute --git-common-dir 2>/dev/null | sed 's#/\.git$##')
[ -n "$_main" ] || _main=$_repo
_csem_up_pin=$(sed -n 's/^cerberus-lean //p' "$_repo/vendor/CSEM_REV")
_csem_lem_pin=$(sed -n 's/^lem-lean //p' "$_repo/vendor/CSEM_REV")
_csem_local="$_repo/build/csem-${_csem_up_pin:0:8}-${_csem_lem_pin:0:8}"
: "${CSEM_SWITCH:=$_csem_local/opam}"
: "${CSEM_UPSTREAM:=$_csem_local/cerberus-lean}"
: "${CSEM_LEMLEAN:=$_csem_local/lem-lean}"
: "${CSEM_ORACLE:=$CSEM_UPSTREAM/_build/default/backend/driver/main.exe}"
: "${CSEM_RUNTIME:=$CSEM_UPSTREAM/_build/install/default}"
: "${CSEM_LEAN_BIN:=$_repo/.lake/build/bin/csem_run}"
export CSEM_SWITCH CSEM_UPSTREAM CSEM_LEMLEAN CSEM_ORACLE CSEM_RUNTIME CSEM_LEAN_BIN
csem_have_toolchain() { [ -x "$CSEM_SWITCH/_opam/bin/lem" ]; }
csem_opam() { opam exec --switch="$CSEM_SWITCH" -- "$@"; }
