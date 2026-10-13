#!/usr/bin/env bash
# check.sh — the gate for ship-your-tcc (after ship-your-lua's scripts/check.sh).
#
#   scripts/check.sh [--static-only]
#
# (1) ISA: tcc's output for the corpus and the tcc ELF contain no M/F/D/Q/A/C
#     instruction (scripts/check_isa.sh, scripts/test_tcc_patch.sh --isa-only);
# (2) forbidden tokens outside comments in Tcc/, Vsa/, VsaIris/: sorry, axiom
#     declarations, native_decide, bv_decide, ofReduceBool, trustCompiler;
# (3b) proof discipline (scripts/check_discipline.py, scripts/discipline_rules.tsv);
# (3c) the abstraction gate (abstractions/gate.py, abstractions/clusters.tsv);
# (5) lake build (skipped with --static-only, and until the Lean project exists;
#     capped at 12 GB as on ship-your-lua);
# (6) #print axioms of the key theorems ⊆ {propext, Classical.choice, Quot.sound}.
set -uo pipefail
cd "$(dirname "$0")/.." || exit 2
fail() { echo "check: FAIL: $*"; exit 1; }

echo "== (1) ISA"
if [ -x scripts/check_isa.sh ] && [ -d build/isa ]; then
  scripts/check_isa.sh build/isa/*.o || fail "M/F/D/A/C instruction in tcc output"
else echo "  (no build/isa objects yet: run scripts/test_tcc_patch.sh)"; fi

echo "== (2) forbidden tokens"
dirs=$(ls -d Tcc Vsa VsaIris 2>/dev/null)
if [ -n "$dirs" ]; then
  hits=$(grep -rnE --include='*.lean' '(^|[^A-Za-z_.])(sorry|native_decide|bv_decide|ofReduceBool|trustCompiler)\b|^\s*axiom\s' $dirs | grep -vE '^\S+:[0-9]+:\s*--' || true)
  [ -z "$hits" ] || { echo "$hits"; fail "forbidden tokens"; }
fi

echo "== (3b) discipline"
python3 scripts/check_discipline.py || fail "discipline"
echo "== (3c) abstraction gate"
python3 abstractions/gate.py || fail "run /abstraction-discovery"

[ "${1:-}" = "--static-only" ] && { echo "check: static stages OK"; exit 0; }
if [ -f lakefile.toml ] || [ -f lakefile.lean ]; then
  echo "== (5) lake build"
  systemd-run --user --scope -q -p MemoryMax=12G lake build || fail "lake build"
  echo "== (6) axioms"
  [ -f scripts/axioms.lean ] || fail "scripts/axioms.lean missing"
  out=$(lake env lean scripts/axioms.lean 2>&1)
  echo "$out" | grep -E "depends on axioms" | grep -vE "\[(propext|Classical.choice|Quot.sound)(, (propext|Classical.choice|Quot.sound))*\]" && fail "non-standard axioms"
else echo "== (5,6) no Lean project yet (Phase 3)"; fi
echo "check: OK"
