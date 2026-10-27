# Exact preprocessing and designator integration — 2026-10-27

The exact phase-4 interface and pragma repairs are integrated with the nested
initializer cursor repair and adopted CSem pin. Current observations are in
[combined-integration-2026-10-27/](combined-integration-2026-10-27/).
The earlier observer report describes the implementation and its separate
full compiler Sail runs; it is evidence for its dated source/image hashes.
These checks do not close preprocessing or binary-refinement theorems.

## Current checks

- `scripts/test_tcc_c11.py`: 147 positive cases and 14 expected rejections,
  with host, GCC, preprocessing round-trip, cross compilation and ISA checks.
- Strict raw-token comparison: 64 focused positives agree among spec, model
  and independent patched-C observer. All 17 negative controls are jointly
  refused and diagnosed by the spec, outside the positive count.
- Corpus: 619 inputs, 613 exact raw streams/six joint refusals, 611 exact
  spec streams/one compiler refusal/seven spec errors. The historical eight
  spec residuals remain classified in the observer integration report.
- All 18 raw protocol/integration test methods pass after rebuilding.
- Fresh independent fast arenas: 1,339 equal/1,812 attempted, including
  1,082 successful generated C programs. There are 466 matching refusals and
  seven unsupported assembly cases, zero mismatches/model/harness errors.
  The command correctly exits 1 for the assembly gaps. No refusal contributes
  to the successful arena quota. Input hashes, not previous corpus counts,
  identify this freshly generated population.
- Native Sail matches all 4,194,304 arena bytes and handoff registers for
  ignored-counter, nested-pragma and nested-designator cases, at 3,383,440,
  3,377,822 and 3,398,235 steps. The exporter rechecked and retained complete
  compressed captures plus independent model arenas for offline comparison.
- The strengthened `scripts/check.sh --csem` passes: explicit compiler-model
  and observer builds, preprocessing axiom audit, default build, CSem tests,
  234-file generator reproduction and standard axiom checks. The unchanged
  113-case baseline remains 90 MATCH, 18 UB_MATCH and five CERB_SKIP.

The reports retain expanded commands, exact input/tool/image hashes and
classifications. C11 command logs, the full gate, explicit target build,
unit tests and broad comparator logs are retained compressed. The reviewed
code revision and source manifest are in `validation.json`.

## Stale executable caught during review

The old default gate did not build the non-default `tcc_model` executable.
After rebasing, the first combined broad run therefore used the earlier
model with the new compiler. It disagreed on an incomplete int array
initialized with a string. Both minimal implicit-int and explicit-int cases
agree after the explicit target rebuild, without any semantic edit. Valid
controls still match. The initial run and its binary hash are retained under
`history/`; its source associations are not accepted as validation.

The gate now explicitly builds `tcc_model`, `tcc_pre_tokens`, `pre_spec` and
`Tcc.Model.PreAudit`, and checks the latter's printed axioms. All token-stream
and arena comparisons and the unit suite were repeated after that build.
This is a build-coverage repair, not an exception to exact comparison.

## Open obligations

`TccModel.PreprocessCorrect_Statement`, `PragmaObservationEffects_Statement`
and `PreprocessObservationNoninterference_Statement` remain unproved. F1's
provenance-carrying raw frontend, F3 ghost Cabs/fuel adequacy and whole-corpus
native Sail coverage remain work. Supported/Fits and the frozen sum cuts are
unchanged; compiling sum as a differential input does not regenerate its cuts.
