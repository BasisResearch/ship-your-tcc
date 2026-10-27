# Combined exact phase-4 observer/spec integration — 2026-10-27

Current validation evidence is in [observer-integration-2026-10-27/](observer-integration-2026-10-27/).
The historical token-stream/pragma-spec reports and observer-repair failures
remain dated evidence; their numbers do not certify these source hashes.
No preprocessing correctness, observation noninterference, or binary/model
refinement theorem is closed by these comparisons.

## Repair and correspondence

Patch 0003 keeps emitted `_Pragma` directives as opaque token-string metadata:
a leading NUL and E tag, a little-endian 64-bit identity, serialized int32
token words, and a trailing NUL. Source literals cannot have this spelling.
The identity survives argument caching and copying; the `-E` output boundary
emits its tokens once. The contents are copied with preprocessing disabled,
so ignored operands cannot advance `__COUNTER__`. Compile-mode effects still
execute at the original operator/directive, before later macro expansion.

Operand parsing lifts inner events before checking its single string literal.
The shared argument traversal lifts metadata before #/## transformations,
retaining semantic whitespace, placemarkers and token locations. Ordinary
replacement lists retain event positions. Function-macro call lookahead skips
metadata between the name and `(`; if it exhausts a rescan frame, its peek
buffer retains those events until the expansion decision. This fixes both
within-frame and across-frame rescans without moving argument events.

`PpMacro`, `State`, and `Types` mirror the compiler's construction, byte layout,
argument handling and lookahead. The observer generator reuses this traversal
and adds only its independent observation/effect replay. Reference/model
arenas and token data are collected independently. The spec independently
keeps opaque events zero width in call lookahead, and its generated `cfgTccE`
now preserves generic pragma operands while expanding handled macro-stack
operands. Pack validation remains a compile-mode check, distinct from `-E`.
No C11 meaning, Supported/Fits, source acceptance criterion, resource budget,
or frozen sum compile-cut artifact was changed.

## Pins and commands

Base checkout: `51cd04f` (reviewed observer checkpoint). TinyCC:
`43c7708b85681a2fd4451c8a541af4494a8919b2`, with active patches 0001–0004.
CSem: `704c4e697845127bced66c93fa0763eb51527ca4`; Lem:
`e6d8db5680962f49060257d6c2bcb13c284b910f`; Lean 4.34.0.
The adopted toolchain symlink was read only; no opam/pristine mutation or repin.
Exact source, header, tool, runtime-image and observation hashes are retained
in the build requests, per-lane JSON and final manifest.

The local capped request was denied by the systemd sandbox after acquiring
the shared lock; no uncapped fallback was used. Lead-service request
`f2-observer-integration-6` builds `tcc_pre_tokens pre_spec tcc_model
Tcc.Model.PreAudit` under the absolute root `build/lake.lock` and
`systemd-run --user --scope -p MemoryMax=12G`, with >=25 GiB available.
The fresh full gate has its own frozen request/result, not an old receipt.
Git staging was also denied by the read-only worktree metadata. The final
verified file manifest/commit request is handed to the lead; no alternate
index, object store, or writable-metadata bypass is used.

Reproduction commands (run in this worktree):

```
python3 scripts/build_pre_tokens_ref.py
python3 scripts/gen_pre_token_cases.py
python3 scripts/test_tcc_c11.py
python3 -m unittest discover -s scripts/tests -p 'test_*tokens*.py'
python3 scripts/pre_tokens_diff.py --diagnostic --corpus --auxiliary --jobs 4 --out CURRENT/corpus.json
python3 scripts/model_diff.py --no-report --require-success 3 -j 2 --json-out CURRENT/arenas.json results/repro/tcc_pragma_ignored_counter.c results/repro/tcc_pragma_nested_operand.c c/tests/c11/pragma_output.c
scripts/check.sh --csem
python3 scripts/freeze_pre_observer.py
```

`CURRENT` denotes `results/pre/observer-integration-2026-10-27`. The strict
focused report records its complete expanded argv: every generated case whose
name does not begin `error-`, including all three repaired observer failures.
The separate diagnostic negative argv includes every `error-` case. No
observer residual set is filtered out. Sail commands, explicit `-std=c11`
arguments, timeout, traces, step metadata and compressed actual input ELFs
are retained in `sail.json` and `sail-{ignored,nested}/`.

## Results

All registered C11 cases pass: **89 positives and six negatives**, including
all 88 inherited positives and both newly registered runtime reproducers.
Every positive retains direct tcc execution, GCC execution, `-E` round trip,
cross compilation and RV64I checks. The added `pragma_output.c` checks ordered
single-emission output through nested operands, reuse, #, ## and both rescan
boundaries, plus pack and ignored/ordinary counter behavior. All **18** raw
protocol/golden/integration test methods pass without skips.

| lane | retained result |
|---|---|
| strict generated positives | **64/64 exact model/reference/spec** (the required 62 plus two new rescan cases); both observer/plain token/effect checks pass |
| negative controls | **17/17 joint compiler refusals and spec diagnostics**, outside the positive quota |
| full diagnostic corpus | **619 attempted: 613 exact raw / six joint refusals; 611 exact spec / eight explicit residuals** |
| independent compiler/model arenas | **3/3 equal**, including both runtime reproducers and the new output fixture; actual reference/model bytes stored separately |
| full compiler Sail | **2/2** reproducers exit 0, empty console, traced handoff and exit, no fuel exhaustion |

The corpus retains every historical input (612 raw/610 spec successes in 618
attempts) and adds `pragma_output.c` as one exact raw/spec success. All 200
Csmith inputs remain exact. No observer/plain interference, raw mismatch,
model limitation, timeout or harness failure occurs in any current raw lane.
The same eight residuals remain in `residuals.json`:

| input | independent classification |
|---|---|
| pp/03.c | phase-4/constraint boundary: spec retains a backslash; compiler-mode lexer refuses the stray backslash |
| pp/21.c | undefined signed overflow/invalid signed shift in #if; spec diagnoses, compilers accept |
| pp/22.c | directives inside a macro argument list (C11 §6.10.3p11 undefined behavior); spec diagnoses the unterminated invocation |
| tests2/114_bound_signal.c, 119_random_stuff.c, 145_winarm64_interlocked.c | unavailable platform headers |
| tests2/18_include.c, 95_bitfields.c | unavailable relative/recursive C-file include fixtures |

Auxiliary `-E` token encodings are separate: focused tcc has 62 exact and two
question-escape encodings; corpus tcc has 611 exact, one host character-
signedness difference and seven error classifications. GCC's focused lane
has 47 exact, 12 differences, three printer errors and two unprotected-trigraph
encodings; its corpus lane has 607 exact, five differences and seven error
classifications. These never replace the strict raw equality or the 89
compiler round trips. The diagnostic negatives include six successful
preprocess-only streams, because `-E` intentionally defers pack validation.

Sail steps (same final runtime pin as the independent arena comparison):
ignored-counter: **3,379,795** to the compiler-run cut, **89** program steps,
**3,379,909** through exit; nested-macro: **3,374,177**, **89**, **3,374,291**.
The scripts retain their historical `compile_steps` name for that run cut;
the next traced PC is the actual generated-code entry. These are observations,
not kernel-certified reachability.

Final full **`scripts/check.sh --csem` passes**, frozen request/result
`f2-observer-integration-gate-1`; `gate.log` ends `check: OK`. ISA, forbidden
tokens, discipline, abstraction, generated-file drift, default build, CSem
gates and axiom checks all pass. The target/audit build's four preprocessing
rows and the final relevant CSem cones use only `{propext, Classical.choice,
Quot.sound}`. The separate, pre-existing Certora audit retains its explicitly
permitted external-call axioms; no new axiom was introduced.

An additional retained strict `rescan-locations` probe matches all three raw
streams and tcc's auxiliary encoding, preserving multiline argument/invocation
locations through both frame cases and preserving an event after a macro that
is not invoked. Its GCC printer difference stays diagnostic. The frozen
validator passes on the current source pins and independently retained
observations.

## Retained failures and limits

`history/` preserves the original observer/plain failures and stale-spec
checkpoint, C11 -E failures, the rejected sink approach, local systemd denial,
and capped failed builds 3–5. Builds 3/4 used the wrong token namespace; build
5 had mutable-variable shadowing. These were corrected without budget changes.
The Lua abstraction inventory/table was consulted; no library proof was reused
or asserted instruction-identical. Attribution records the moved macro traversal.

GCC's printer retains an operator in this expanded stringification, duplicates
a repeated pragma, and retains a function call across the pragma. The new
runtime fixture supplies an ordinary identity function for that call, so GCC
can independently check the program's result. Its printer differences remain
explicit auxiliary classifications. tcc's exact ordinary tokens are checked
against the model and spec; no GCC bytes enter either implementation.

Suggested ledger text (lead-owned STATUS/PHASES were not edited): combined
F2 observation/spec and -E regression integration validated by the attached
strict lanes and full gate. Three preprocessing correctness/effects/
noninterference Statements remain open; no proof closure or Supported
restriction is claimed.
