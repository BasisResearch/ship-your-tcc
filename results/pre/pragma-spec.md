# Spec pragma handling against the exact phase-4 observer

This repairs the five strict token residuals and two rejection residuals in
[token-stream.md](token-stream.md), without changing either raw observer,
Supported, the frozen sum cuts, or any spelling equality. It also adds
independent regression cases for operand expansion, event ordering, pack,
once, macro push/pop, and ordinary expansion and locations. Correctness and
observation noninterference remain typed Statements; no refinement is claimed.

## Independent causes and repairs

| Probe | Cause | Repair |
|---|---|---|
| `#pragma unknown X` | global pragma expansion replaces an ignored operand | `PreCfg.pragmaExpand` selects the pragma name; ignored operands remain raw |
| `#pragma unknown __COUNTER__` | the same expansion advances an observable counter | preserve the operand and following `0`; ignored nested operators stay unexecuted |
| repeated argument containing `_Pragma` | copied bracket tokens report one execution twice | one opaque event identity survives argument caching; output renders its first surviving occurrence once |
| subsequent `#` | payload markers enter stringification | lift existing events before the transformed result; stringify only semantic tokens |
| subsequent `##` | paste consumes an event boundary | lift the opaque event; paste semantic tokens and placemarkers |
| two transformed arguments | source-order lifting disagrees with tcc's retained event positions | configured right-to-left argument-event lifting, preserving each argument's internal order and already executed effects |
| `_Pragma(u8"...")`, `u`, `U` | operand validation accepts every string prefix | accept ordinary and `L` operands; diagnose the other prefixes |
| `pack(3)` | no implementation-defined pack validation | configured compiler-mode pack validation and stack, including underflow, overflow, reset and integer literal choices |
| `push_macro(NAME)` / `pop_macro(NAME)` | raw-only recognition misses macro-replaced names | expand recognized operands, then execute the resulting phase-4 action; malformed forms diagnose |

`PragmaEvent` holds an identity and exact payload tokens, separately from the
semantic token's spelling. Expansion carries it as one opaque item. Only the
phase-4 output boundary renders `pragmaBegin`, payload, `pragmaEnd`. The
expansion context threads identities, pack state, once files, macro tables,
push/pop state and counters through argument prescan and rescanning. A cached
event does not rerun an effect. `_Pragma("once")` records the actual file path
at execution, including during argument prescan. The output ordering choice
affects retained event payloads; it does not reorder already executed effects.

`tcc -E` has its own cfg: its generic pragma printer expands operands and does
not run the compiler-mode pack validator. That auxiliary configuration is
kept distinct from the raw compiler configuration. No printer output or C
literal escape decoding is used for a strict success here.

## Validation

Final frozen request/result: `pragma-builds/request-5.json` and
`pragma-builds/result-5.json`. The lead service verified the source hashes and
ran the shared absolute Lake lock with `systemd-run --user --scope -p
MemoryMax=12G`. Targets: `CSemPre`, `pre_spec`, `tcc_pre_tokens`,
`Tcc.Model.PreAudit`. The build includes the standard C11 examples and new
Lean guards. The audit prints only `{propext, Classical.choice, Quot.sound}`
for `preprocess` and the three existing correctness/effects/noninterference
Statements. There are no proof-budget changes or new axioms.

The static gate passes, including generator drift, forbidden tokens,
discipline and abstraction checks. All **18 Python protocol/golden/integration
test methods pass**. They now require exact spec equality as well as raw
model/reference equality for positive golden fixtures, and retain tests that
the strict comparator rejects refusals and a signed-overflow spec residual.

The 79 focused inputs are all retained:

| Lane | Result |
|---|---|
| strict positives | **59/59 exact spec/model/reference token streams**, with both observer/plain token/effect checks passing |
| intentional negatives | **17/17 joint compiler refusals and spec diagnostics**, counted separately |
| observer residuals | **3 failures of observer/plain noninterference**, explicitly outside the success quota |

The strict positives include ignored counters/operators, repeated empty and
nonempty arguments, raw and expanded stringification, paste on either side,
two transformed arguments, macro-expanded pragma operands, pack literals and
nesting, once in directives/operators/prescan, push/pop names and rescanning,
ordinary counters, macro recursion, stringify/paste, and multiline locations.
Nested operator operands additionally have an independent successful Lean
spec guard; they cannot count as successful raw observations for the reason
below.

The full **618-input corpus** gives **612 exact raw model/reference matches**
and six joint refusals, with no raw mismatch, harness failure or observation
change to plain execution. There are **610 exact spec agreements**, seven
spec errors and one spec stream refused by the compiler. All 200 Csmith inputs
remain exact. The same eight corpus residuals are retained and classified:

| Input | Classification |
|---|---|
| `pp/03.c` | spec retains an `other` backslash; compiler-mode lexer refuses the stray backslash; separate phase-4/constraint boundary residual |
| `pp/21.c` | signed overflow/invalid signed shift in `#if`, rejected by spec and accepted by compiler; undefined-behavior fixture |
| `pp/22.c` | directives occur inside `TRACE`'s macro argument list (§6.10.3p11 undefined behavior); spec reports an unterminated invocation at the directive boundary, compiler accepts |
| `tests2/114_bound_signal.c` | unavailable semaphore header, fixture/environment refusal |
| `tests2/119_random_stuff.c` | unavailable sys/mman header, fixture/environment refusal |
| `tests2/145_winarm64_interlocked.c` | unavailable Windows header, fixture/environment refusal |
| `tests2/18_include.c` | unavailable relative parent include, fixture/environment refusal |
| `tests2/95_bitfields.c` | unavailable recursive C-file include, fixture/environment refusal |

## Newly exposed observer failures

These are independent of the repaired spec mismatches. They are reproduced
by both existing decorated observers and their plain modes; neither observer
was modified to obtain a spec pass.

* `_Pragma(_Pragma("unknown inner") "unknown outer")` is accepted by both
  plain implementations and refused by both observed implementations. The
  spec succeeds with two ordered events. A macro-mediated version has the
  same independent observed/plain acceptance difference.
* `_Pragma("unknown __COUNTER__") __COUNTER__` ends with counter **1** under
  observation and **2** in plain mode. The decorated streams agree with each
  other and with the spec; that equality is insufficient for success because
  observation changes ordinary compiler behavior.

`pragma-observer-residuals.json` preserves the failing diagnostics and state
comparisons. Its comparator exits **1**, as required for a noninterference
failure. The tests explicitly require those failures to remain visible.
Repairing them is separate observer work; no universal noninterference claim
is made. `pragma-residuals.json` classifies every non-successful attempted row,
including all negative fixtures, these three failures and the eight corpus
residuals. `pragma-build.json` pins the matching final receipt, reports, logs,
input/header hashes and their summaries. Earlier build attempts and dead ends
are retained in `pragma-builds/` and `tasks/DEAD-ENDS.md`.

## Reproduce

After the final capped targets are available:

```
python3 scripts/build_pre_tokens_ref.py
python3 scripts/gen_pre_token_cases.py
python3 -m unittest discover -s scripts/tests -p 'test_*tokens*.py'
```

Select strict positives from `gen_pre_token_cases.CASES`, excluding names
beginning `error-` and the explicit `OBSERVER_RESIDUALS` set. Pass that exact
selection to `pre_tokens_diff.py --out results/pre/pragma-focused.json` with
no diagnostic flag. Run the negatives with `--diagnostic`; run the observer
residuals with `--diagnostic` and retain their nonzero exit. Run the existing
full corpus selection with `--diagnostic --corpus`. No case is removed from
those recorded diagnostic lanes. With test/static logs at
`build/pragma-{tests,static}.log` and the final frozen build receipt, run
`python3 scripts/freeze_pre_pragmas.py` to validate and archive the evidence.
