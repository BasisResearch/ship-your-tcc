#!/usr/bin/env bash
# check.sh — the gate for ship-your-tcc (after ship-your-lua's scripts/check.sh).
#
#   scripts/check.sh [--static-only]
#
# (1) ISA: tcc's output for the corpus and the tcc ELF contain no M/F/D/Q/A/C
#     instruction (scripts/check_isa.sh, scripts/test_tcc_patch.sh);
# (2) forbidden tokens outside comments in Tcc/, Vsa/, VsaIris/: sorry, axiom
#     declarations, native_decide, bv_decide, ofReduceBool, trustCompiler;
# (3b) proof discipline (scripts/check_discipline.py, scripts/discipline_rules.tsv);
# (3c) the abstraction gate (abstractions/gate.py, abstractions/clusters.tsv);
# (4) generated files are generated: vendor/certora-export-patched = pristine
#     vendor/certora-export + patches/certora/*-cclib-*.patch (scripts/cclib_src.sh),
#     Tcc/Prog/*/Clight.lean from clightgen's output (scripts/gen_prog_clight.sh),
#     Tcc/Audit/StepOrig.lean from the pristine Clight.lean (scripts/gen_cclib_audit.sh);
# (5) lake build (skipped with --static-only, and until the Lean project exists;
#     capped at 12 GB as on ship-your-lua, serialised on $SYT_LAKE_LOCK);
# (6) #print axioms of the key theorems ⊆ {propext, Classical.choice, Quot.sound}
#     (scripts/axioms.lean); (6b) the patch-0002 audit (scripts/axioms_certora.lean)
#     may add exactly Certora's four external-call axioms.
set -uo pipefail
cd "$(dirname "$0")/.." || exit 2
fail() { echo "check: FAIL: $*"; exit 1; }

echo "== (1) ISA"
[ -x scripts/check_isa.sh ] || fail "scripts/check_isa.sh missing or not executable"
shopt -s nullglob
isa_files=(build/isa/*.o build/test-tcc/*.tcc.o build/test-tcc/*.tcc.elf
           build/test-tests2/*.o build/test-tests2/*.elf)
shopt -u nullglob
for image in c/tcc-riscv-htif.elf c/build/base.elf; do
  [ ! -f "$image" ] || isa_files+=("$image")
done
if [ ${#isa_files[@]} -gt 0 ]; then
  scripts/check_isa.sh "${isa_files[@]}" || fail "M/F/D/A/C instruction in tcc output or image"
else echo "  (no tcc objects or images yet: run scripts/test_tcc_patch.sh)"; fi

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

echo "== (4) generated files"
scripts/cclib_src.sh --check || fail "vendor/certora-export-patched is not pristine + patches"
scripts/gen_prog_clight.sh --check || fail "Tcc/Prog/*/Clight.lean or c/tests/p3/*.clight.lean stale"
scripts/gen_cclib_audit.sh --check || fail "Tcc/Audit/StepOrig.lean stale"

[ "${1:-}" = "--static-only" ] && { echo "check: static stages OK"; exit 0; }
if [ -f lakefile.toml ] || [ -f lakefile.lean ]; then
  echo "== (5) lake build"
  lock=${SYT_LAKE_LOCK:-build/lake.lock}
  mkdir -p "$(dirname "$lock")"
  flock "$lock" systemd-run --user --scope -q -p MemoryMax=12G lake build || fail "lake build"
  # audit <file> <allowed-axiom-regex>: every #print axioms row within the set
  audit() {
    [ -f "$1" ] || fail "$1 missing"
    if ! out=$(lake env lean "$1" 2>&1); then
      printf '%s\n' "$out"
      fail "axiom audit $1 did not elaborate"
    fi
    # #print axioms wraps long lists over several lines: join them first
    rows=$(printf '%s\n' "$out" | tr '\n' ' ' | sed -E "s/ +/ /g; s/ ?'([^' ]+)' /\n'\1' /g" | grep -E "^'[^']+' (depends on axioms:|does not depend on any axioms)" | sed -E 's/ +$//' || true)
    [ -n "$rows" ] || fail "axiom audit $1 reported no theorems"
    printf '%s\n' "$rows"
    printf '%s\n' "$rows" | grep -vE "^'[^']+' (does not depend on any axioms|depends on axioms: \[(($2)(, ($2))*)?\])$" && fail "non-standard axioms in $1"
    return 0
  }
  std='propext|Classical.choice|Quot.sound'
  echo "== (6) axioms"
  audit scripts/axioms.lean "$std"
  echo "== (6b) axioms of the patch-0002 audit"
  audit scripts/axioms_certora.lean "$std|CC.externalFunctionsSem|CC.inlineAssemblySem|CC.externalFunctionsSemDeterm|CC.inlineAssemblySemDeterm"
else echo "== (5,6) no Lean project yet (Phase 3)"; fi
echo "check: OK"
