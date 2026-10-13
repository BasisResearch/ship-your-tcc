# Source me:  . scripts/certora_env.sh
# Paths for the Certora/clean (CompCert 3.17 fork) oracles built by
# scripts/build_certora_ccomp.sh and scripts/build_certora_lean.sh.
_certora_here=$(cd "$(dirname "${BASH_SOURCE[0]:-$0}")" && pwd)
export SYT_ROOT=$(cd "$_certora_here/.." && pwd)
export CERTORA_DIR="$SYT_ROOT/build/certora"
export CERTORA_SRC="$CERTORA_DIR/src"                 # git checkout of Certora/clean
export CERTORA_COMMIT=123f7f7767e215aceb4821f0d044336d78103ade
export CCOMP="$CERTORA_DIR/install/bin/ccomp"
export CLIGHTGEN="$CERTORA_DIR/install/bin/clightgen"
export CERTORA_LEAN_PROJECT="$CERTORA_SRC/export"     # Lake project: CCLib, Clightdefs
export CLIGHT_RUN="$SYT_ROOT/lean/ClightRun/.lake/build/bin/clight_run"
# clight_run elaborates the clightgen output in-process: it needs Lean 4.32.1's
# sysroot (Init/Lean .oleans) and the CCLib .oleans.
export LEAN_SYSROOT="${CERTORA_LEAN_SYSROOT:-$HOME/.elan/toolchains/leanprover--lean4---v4.32.1}"
export CLIGHT_LEAN_PATH="$CERTORA_LEAN_PROJECT/.lake/build/lib/lean"
export CERTORA_OPAM_SWITCH=certora-clean
# OCaml/Coq env of the switch (only needed to rebuild ccomp/clightgen)
certora_opam_env() { eval "$(opam env --switch=$CERTORA_OPAM_SWITCH --set-switch)"; }
# Flags passed to BOTH ccomp -interp and clightgen (keep them identical):
#   -fstruct-passing : struct/union arguments and results by value (else rejected)
# Deliberately NOT passed:
#   -flongdouble     : would make long double = double (8 bytes); tcc/gcc on rv64
#                      use 16-byte IEEE quad, so reject such programs instead
#   -fbitfields      : ignored by CompCert 3.17 (bit fields are native)
#   -funstructured-switch, -fpacked-structs, -finline-asm : off (not in the Lean port's tests)
# -fvararg-calls and -funprototyped are on by default.
export CERTORA_CFLAGS="${CERTORA_CFLAGS:--fstruct-passing}"
unset _certora_here
