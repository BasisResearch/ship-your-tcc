# csem/: our fork of cerberus-lean, and its port to Lean 4.34.0

`csem/` is OathTech/cerberus-lean at the pin in `vendor/CSEM_REV` (e7e9bb4,
Lean 4.32.2), the part `scripts/csem_import.sh` selects, plus LemLib
(lem-lean `lean-lib/` at 2d3a492) in `csem/lemlib`. Our changes are ordinary
commits on top; this file lists every one. `scripts/csem_import.sh --check
UPSTREAM LEMLEAN` lists the files that differ from a pristine import.

* **Layout.** As upstream: `frontend/` (Lem model), `lean_frontend/`
  (hand-written seams, `generated/`, `test/`, `corpus/`, `native/`), `scripts/`
  and `tools/` (gates), `runtime/` (Core library, `.impl` file, libc),
  `parsers/c` (menhir grammar and lexer, F1's reference), `tests/minimal` and
  `tests/fixtures`. `lean_frontend/lakefile.toml` is kept for reference only:
  the libraries build in the root project's `lakefile.toml` (`LemLib`,
  `LemLibTests`, `CerberusLean`, `CerberusLeanTest`, the fork's unit
  executables, `csem_run`; none a default target).
* **Generated.** `lean_frontend/generated/` = `lem -lean` of the
  `LEM_SRC_LEAN` list plus copies of the files in
  `lean_frontend/handwritten_copy.manifest` (the fork's `make
  lean-prelude-src`), by `scripts/csem_regen.sh`; `--check` regenerates in a
  scratch copy and diffs. Generator: lem `lean-backend-v0.1.0-alpha.1-65-g2d3a492`
  under OCaml 5.4.0 (opam switch `build/csem-opam`, `scripts/csem_toolchain.sh`).

## Changes for Lean 4.34.0

No option was added: no `maxHeartbeats`, `maxRecDepth` or other option is set
anywhere in `csem/` (upstream sets none either). Three proofs needed changes,
all from one 4.34 behaviour change of `split` on `if`s.

| file | change | why |
|---|---|---|
| `lean_frontend/Utils_lemMeasureProofs.lean` (3 sites), `lean_frontend/Defacto_memory_lemMeasureProofs.lean` (2 sites) | `split` → `refine ite_congr rfl (fun _ => ?_) (fun _ => ?_)` | on the `lem_if` term `if decide (p) = true then … else …` (both sides of a fuel-stability equation), 4.34's `split` case-splits but then fails to simplify the target with the new hypothesis (`trace.split.failure`: "failed to simplify target using new hypotheses") and gives up; `ite_congr` yields the same two goals with the same hypotheses, so the branch proofs are unchanged |
| `lean_frontend/CerbMemAccessFacts.lean` (`loadM_erasure`) | `not_true_eq_false, Bool.false_eq_true` added to the two `simp_all only` sets of the closing loop | the loop's `split`s now leave the contradictory branches with hypotheses `¬True` / `false = true` that 4.32's `split` simplified away; the extra lemmas close them |
| `lemlib/LemLibPmapLaws.lean` (`toList_bal`) | the `repeat' split … omega` script replaced by the explicit case analysis on the two height guards and the four `match`es of `bal` | 4.34's `split` takes the `match`es before the `if`s and stops on the height guards, so `omega` met unreduced `match` terms; the statement is unchanged |

## Other changes (ours, not the port)

| file | change | why |
|---|---|---|
| `lean_frontend/Main.lean` → `lean_frontend/CerbDriver.lean` | renamed; `def main` → `def cerbMain`; `handwritten_copy.manifest` lists `CerbDriver.lean` | the root package already has a module `Main` (`lean/ClightRun`); `csem_run` wraps the driver |
| `lean_frontend/CSemRun.lean` (new, in the manifest) | `csem_run`'s `main`: a `.c` argument goes through `cerberus --cabs-json` (`$CSEM_ORACLE`) and then `cerbMain --batch`; any other argument list is `cerbMain` unchanged | the CSem oracle for F1's `scripts/diff3.py` |
| `native-lake/lakefile.lean` (new) | `extern_lib cerbnative` compiling `lean_frontend/native/md5.c` with `leanc` | upstream links `native/md5.o` by `moreLinkArgs` after `make lean-native-obj`; a TOML lakefile cannot declare an `extern_lib`, so it is a path package the root project requires |
| `scripts/check_sorry_token.sh` | LemLib scanned at `csem/lemlib` | LemLib is vendored, not a Lake package copy |
| `scripts/check_theorem_axioms.sh` | probes run with `lake env lean` from the root project (`$CSEM_LAKE_ROOT`, default `csem/..`); LemLib at `csem/lemlib`; the lem-lean clone for leg 4's scaffold assertion at `$CSEM_LEMLEAN` | the semantics builds in the root Lake project |

Gates not ported (they need upstream's in-tree OCaml build and
`lean_frontend` Lake package, or name `Main.lean`): `check_fuel_forms.sh`,
`check_failure_reach.sh`, `check_no_fuel_numerals.sh` (its allowlist names
`Main.lean`), `check_lakefile_roots.sh`, `check_pin_sites.sh`,
`check_fork_drift.sh`, `check_fixture_freeze.sh`, and the lanes other than the
exec lane (`scripts/csem_lane.py` reimplements that one; the fork's
`test_exec.sh` stays runnable in the pristine upstream checkout).
