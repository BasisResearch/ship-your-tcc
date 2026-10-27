# Preprocessor comparison acceptance — 2026-10-27

`pre_diff.py` now succeeds by default only when every selected input has
exact token equality with tcc. Matching errors and spacing classifications
are not agreements. `--diagnostic` records residuals without claiming success
of the equality obligation; crashes, malformed protocol, inconsistent exits,
timeouts and failure to lex successful oracle output still fail that mode.
GCC remains auxiliary unless `--require-gcc` is requested. Missing inputs,
empty selections/corpora and invalid job counts fail explicitly.

Validation: seven protocol tests pass, including injected crash/timeout,
mixed error/token records and failed output lexing. Four negative CLI checks
have the expected nonzero statuses. Six focused C11 reproducers have exact
tcc equality. The full diagnostic run covers 608 files: 600 tcc equal, one
previously diagnosed host signed-char difference, two undefined-behavior
examples, and five missing-fixture/header errors. GCC has 595 equal and six
differences; the same two UB examples and five fixture errors are separate.
The extra fifteen files relative to the 593-file pragma report are the new
RV64 configuration probes. No harness failures occurred. Source/executable
pins and raw classifications are in acceptance-pins.json and
acceptance-recheck.tsv. This strengthens evidence handling; it is not the
preprocessor theorem or a reason to exclude a supported program.

Commands: `python3 scripts/tests/test_pre_diff.py`,
`python3 scripts/pre_diff.py --jobs 4 --diagnostic --out results/pre/acceptance-recheck.tsv`,
and `scripts/check.sh --static-only`. The existing C11 reproducers were also
run in strict mode. The current cross compiler predates the separately
validated macro-operand patch increment; repeat affected comparisons when
that image is integrated. No sum compile-cut artifacts were regenerated.
