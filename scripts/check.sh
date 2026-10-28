#!/usr/bin/env bash
# check.sh — the gate for ship-your-tcc (after ship-your-lua's scripts/check.sh).
#
#   scripts/check.sh [--static-only | --compile-cut | --csem]
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
# (2c) CSem (csem/, our fork of cerberus-lean) text gates, no toolchain needed:
#     copy-set sync, lem-sync stamp, exec totality and purity, sorry token;
# --csem: the fork's gates on the 4.34 build (scripts/csem_gates.sh: build,
#     unit executables, axiom census, generator drift when lem is present, the
#     exec lane when the oracle is present) and scripts/axioms_csem.lean;
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
# after ship-your-lua's stage 3: comments and strings stripped first; raised
# elaboration limits are forbidden in Tcc/ (the copied Vsa/ layer has some)
python3 - <<'PY' || fail "forbidden tokens"
import os, re, sys
bad = []
pat = re.compile(r"\bsorry\b|^\s*axiom\b|native_decide|bv_decide|ofReduceBool|trustCompiler")
lim = re.compile(r"maxHeartbeats|maxRecDepth")
def strip(src):
    src = re.sub(r"/-.*?-/", "", src, flags=re.S)
    src = re.sub(r"--[^\n]*", "", src)
    return re.sub(r'"(?:\\.|[^"\\])*"', '""', src)
for d in ["Tcc", "Vsa", "VsaIris", "Spec", "csem/cparser"]:
    for dp, _, fs in os.walk(d):
        for f in fs:
            if not f.endswith(".lean"): continue
            p = os.path.join(dp, f); s = strip(open(p).read())
            for i, line in enumerate(s.splitlines(), 1):
                if pat.search(line) or (d in ("Tcc", "Spec", "csem/cparser") and lim.search(line)):
                    bad.append(f"{p}:{i}: {line.strip()}")
for b in bad: print(b)
sys.exit(1 if bad else 0)
PY

echo "== (2b) generator drift"
# the copied machine layer is the import closure, byte-identical to its source
python3 scripts/port_closure.py --check || fail "copied Vsa/ layer drifted (scripts/port_closure.py)"
# the captured cut's Lean files are regenerated from results/p3/sum-cut.json
# (Elf*.lean only when the captured ELF is at build/p3/sum.elf)
python3 scripts/gen_tcc_cut.py --check || fail "generated Tcc/Capture/Sum drifted (scripts/gen_tcc_cut.py)"
python3 scripts/gen_tcc_segs.py --check || fail "generated replay drifted (scripts/gen_tcc_segs.py)"
# printf's layout in the arena, from results/p3/sum-arena.tsv (and the ELF, when at build/p3/sum.elf)
python3 scripts/gen_printf_layout.py --check || fail "generated Tcc/Printf/Sum drifted (scripts/gen_printf_layout.py)"
# the CompileCut chain, table and pin proofs are regenerated from the committed
# trace digest results/p3/ccp-digest.json (the checkpoint states by hash)
python3 scripts/gen_ccp.py --check || fail "generated Tcc/Ccp/Sum drifted (scripts/gen_ccp.py)"
# Executable compiler model token numbers must follow the patched tcc headers.
python3 scripts/gen_model_tok.py --check || fail "generated Tcc/Model/Tok drifted"
python3 scripts/gen_ld_literal_tests.py --check || fail "generated binary128 literal tests drifted"
python3 scripts/gen_float_literal_tests.py --check || fail "generated direct literal tests drifted"
# CSem.Pre's tcc configuration and image file system (Spec/Pre/Tcc/), from the
# patched tcc sources, the cross tcc's -dM listing and c/build/fs.list
if [ -f c/build/fs.list ] && [ -x build/cross/riscv64-tcc ]; then
  scripts/tcc_src.sh build/tcc-src && python3 scripts/gen_pre_cfg.py --check \
    || fail "generated Spec/Pre/Tcc drifted (scripts/gen_pre_cfg.py)"
else
  echo "  (skipping gen_pre_cfg.py --check: no c/build/fs.list or build/cross/riscv64-tcc)"
fi

echo "== (2c) CSem text gates"
csem/tools/check_handwritten_sync.sh --quiet || fail "csem: hand-written copies drifted (scripts/csem_regen.sh)"
csem/tools/check_lem_sync.sh --check-lean >/dev/null || fail "csem: generated Lean is not the lem output of csem/frontend"
csem/scripts/check_exec_totality.sh || fail "csem: exec totality"
csem/scripts/check_exec_purity.sh || fail "csem: exec purity"
csem/scripts/check_sorry_token.sh || fail "csem: sorry token"

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
  common=$(git rev-parse --path-format=absolute --git-common-dir) || fail "git common directory"
  lock=${SYT_LAKE_LOCK:-$(dirname "$common")/build/lake.lock}
  mkdir -p "$(dirname "$lock")"
  # the generated replay first, a few parts at a time (each part is a 1-3 GB
  # Lean process; all of them at once would exceed the 12 GB cap)
  SYT_LAKE_LOCK="$lock" scripts/build_replay.sh || fail "replay build (scripts/build_replay.sh)"
  flock "$lock" systemd-run --user --scope -q -p MemoryMax=12G lake build || fail "lake build"
  # These executables are not default Lake targets. In particular, a rebase
  # can change their source without the default build refreshing the binaries
  # used by the F2/F3 differential harnesses.
  echo "== (5b) compiler model and phase-4 observers"
  flock "$lock" systemd-run --user --scope -q -p MemoryMax=12G \
    lake build tcc_model tcc_pre_tokens pre_spec Tcc.Model.PreAudit \
    || fail "compiler model/observer build"
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
  audit Tcc/Model/PreAudit.lean "$std"
  echo "== (6b) axioms of the patch-0002 audit"
  audit scripts/axioms_certora.lean "$std|CC.externalFunctionsSem|CC.inlineAssemblySem|CC.externalFunctionsSemDeterm|CC.inlineAssemblySemDeterm"
  if [ "${1:-}" = "--csem" ]; then
    # opt-in: the CSem libraries and the fork's gates (about 7 CPU-minutes from
    # scratch, peak 3.3 GB; results/csem/f0-validation.md), then the F0 statement
    echo "== (8) CSem gates (scripts/csem_gates.sh)"
    SYT_LAKE_LOCK="$lock" scripts/csem_gates.sh || fail "CSem gates (scripts/csem_gates.sh)"
    echo "== (8b) axioms of the F0 statement over CSem"
    audit scripts/axioms_csem.lean "$std"
    echo "== (8c) raw frontend build and axiom/purity census"
    flock "$lock" systemd-run --user --scope -q -p MemoryMax=12G lake build csem_raw CSemConeReport \
      || fail "raw frontend build"
    audit scripts/raw_frontend_audit.lean "$std"
    identity_checks=$(mktemp -d build/library-identity-gate.XXXXXX)
    python3 scripts/check_library_identity.py --artifacts "$identity_checks/run" \
      || fail "resolved library identity (artifacts: $identity_checks/run)"
    echo "resolved library identity artifacts: $identity_checks/run"
  fi
  if [ "${1:-}" = "--compile-cut" ]; then
    # opt-in: the evaluator run of tcc's compile phase (TccCompileCut, ~2.5 CPU-hours
    # from scratch) and the closed end-to-end theorem
    echo "== (7) CompileCut (scripts/build_ccp.sh)"
    SYT_LAKE_LOCK="$lock" scripts/build_ccp.sh || fail "CompileCut build (scripts/build_ccp.sh)"
    echo "== (7b) axioms of the CompileCut proofs"
    audit scripts/axioms_ccp.lean "$std"
  fi
else echo "== (5,6) no Lean project yet (Phase 3)"; fi
echo "check: OK"
