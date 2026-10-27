# Lead review of line-provenance model — 2026-10-27

The line-provenance compiler/model change was checked on **1,556 inputs**,
with **1,419 identical successful arenas** and zero byte differences or
model errors. This includes 899 successful arenas from 1,000 generated cgen
inputs and all 200 retained Csmith inputs. These are independent fast RV64I
observations; the focused compiler Sail run is recorded in line-build.json.

The other results remain separate: 129 matching compiler refusals, seven
unsupported assembly cases, and one reference instruction-limit failure in
TinyCC's `tests/tests2/101_cleanup.c`. That same pre-handoff failure is
recorded in the earlier c11-all.json and c11-residuals.json. It remains work,
not a supported-program success or a reason to narrow Supported. The broad
command therefore exits nonzero, as required by the strict harness.

The source was frozen worker commit `703cb9154ca30fa8109db36934f11ae64e405716`
before rebasing onto main. `line-broad-review.json` pins the compiler patches,
runtime, model binary, harness and every input. Source groups were `tests`,
`run`, `tcc`, `p3`, `csmith`, `tests2`, `model`, `c11`, plus the retained
1,000 cgen inputs and 72 CompCert / 73 tcc corpus files from the original
model recovery worktree. Four fast interpreters ran alongside three native
Sail captures, within the project's eight-run limit. No reference bytes
were supplied to the model.

The first attempt found empty generated/linked corpus locations in the new
worktree and failed before running any cases. The rerun names the preserved
absolute source paths explicitly; it does not silently skip absent corpora.
