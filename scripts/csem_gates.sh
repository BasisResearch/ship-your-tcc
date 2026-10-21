#!/usr/bin/env bash
# csem_gates.sh — the fork's own gates, on the root project's 4.34 build
# (scripts/check.sh --csem runs this).
#
#   scripts/csem_gates.sh [--no-build] [--no-lane]
#
# After csem/scripts/test_unit.sh (the fork's row-1 gate), with paths for the
# root Lake project:
#   1. copy-set sync and Lean lem-sync stamp (csem/tools)
#   2. build: LemLib, LemLibTests, CerberusLean, CerberusLeanTest, csem_run,
#      the fork's unit executables, TccCSem (scripts/csem_build.sh: 12 GB cap,
#      shared Lake lock)
#   3. the unit executables, with test_unit.sh's arguments, from
#      csem/lean_frontend
#   4. exec purity (+ selftest plants), exec totality (+ plants), the axiom
#      census (csem/scripts/check_theorem_axioms.sh), the sorry-token census
#   5. generator drift (scripts/csem_regen.sh --check; SKIP without lem)
#   6. the exec lane against the OCaml oracle on csem/tests/minimal
#      (scripts/csem_lane.py; SKIP without the oracle)
# Fails on the first failing stage.
set -uo pipefail
source "$(dirname "$0")/csem_env.sh"
cd "$_repo"
build=1; lane=1
for a in "$@"; do
  case "$a" in --no-build) build=0;; --no-lane) lane=0;; *) echo "usage: $0 [--no-build] [--no-lane]"; exit 2;; esac
done
fail() { echo "csem_gates: FAIL: $*"; exit 1; }
export LC_ALL=C CERB_MEM_MAX=12G CSEM_LEMLEAN

UNIT=(core-parser-test effects-proof-test totality-proof-test fresh-int-test pp-test
      fuel-exemplar-test monadic-failstop-test allocator-soundness-test float-literal-test
      are-compatible-test many-restatement-test opaque-failure-test enum-data-test
      run-digest-test cabs-import-test match-pattern-arity-test memory-access-test
      reconstruct-legacy-test pnvi-arms-test)

echo "== csem (1) copy-set sync, lem-sync stamp"
csem/tools/check_handwritten_sync.sh || fail "hand-written copies drifted (scripts/csem_regen.sh)"
csem/tools/check_lem_sync.sh --check-lean || fail "lem-sync stamp"

if [ $build = 1 ]; then
  echo "== csem (2) build"
  scripts/csem_build.sh LemLib LemLibTests CerberusLean CerberusLeanTest csem_run TccCSem "${UNIT[@]}" \
    || fail "CSem build"
fi

echo "== csem (3) unit executables"
for t in "${UNIT[@]}"; do
  args=()
  case "$t" in
    monadic-failstop-test) args=(2 17);;
    match-pattern-arity-test|pnvi-arms-test) args=(17);;
    memory-access-test) args=(17 0 on);;
  esac
  if (cd csem/lean_frontend && LEAN_ABORT_ON_PANIC=1 "$_repo/.lake/build/bin/$t" "${args[@]}" >/dev/null 2>&1); then
    echo "  ok   $t"
  else
    fail "unit executable $t (run: cd csem/lean_frontend && $_repo/.lake/build/bin/$t ${args[*]})"
  fi
done

echo "== csem (4) purity, totality, axiom census, sorry token"
csem/scripts/check_exec_purity.sh --selftest | tail -1 || fail "exec purity selftest"
csem/scripts/check_exec_purity.sh || fail "exec purity"
csem/scripts/check_exec_totality.sh || fail "exec totality"
python3 csem/scripts/test_exec_totality.py | tail -1 || fail "exec totality plants"
csem/scripts/check_theorem_axioms.sh | grep '^check_theorem_axioms' || fail "axiom census"
[ "${PIPESTATUS[0]}" = 0 ] || fail "axiom census"
csem/scripts/check_sorry_token.sh || fail "sorry token"

echo "== csem (5) generator drift"
scripts/csem_regen.sh --check || fail "generated Lean drifted"

if [ $lane = 1 ]; then
  echo "== csem (6) exec lane vs the OCaml oracle (csem/tests/minimal)"
  if [ -x "$CSEM_ORACLE" ]; then
    python3 scripts/csem_lane.py | tail -1
    [ "${PIPESTATUS[0]}" = 0 ] || fail "exec lane differs from the fork's baseline"
  else
    echo "  SKIP (no oracle at $CSEM_ORACLE; scripts/csem_toolchain.sh)"
  fi
fi
echo "csem_gates: OK"
