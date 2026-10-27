# Exact phase-4 token observation (2026-10-27)

`TccModel.preprocess` now runs the modeled tcc lexer, directive parser,
macro expansion and pragma effect parser in C11 memory-output mode. Its
outer `next()` calls disable TOK_NUM/TOK_STR conversion. The projection
preserves token kinds and exact spelling bytes: prefixes, quotes, literal
escapes, digraphs and UCNs. Adjacent strings remain separate. The image
filesystem and fixed epoch clock supply include and predefined-macro state.
`preprocessWith` permits independently supplied filesystem fixtures.

The direct reference includes pinned TinyCC 43c7708 plus repository patches,
compiled as an RV64 cross-target host observer. It reads no Lean/model/spec
output or token table. `scripts/build_pre_tokens_ref.py` generates only
`build/pre-tokens-ref`; vendor and production compiler sources stay pristine.
It calls internal `next()` and `get_tok_str`, never `tcc -E`. The host
configuration retains the image's bare-metal predefined type macros from
tccpp.c, unsigned char and epoch time, without the newlib-only phase-7
binary128 literal adapter. This is independent diagnostic execution, not a
Sail observation or a binary refinement proof.

Non-phase-4 pragmas retain bracketed payload tokens and their stream
positions. Their existing effect parser still runs: pack, library pragmas,
macro push/pop, once, counters and include state are exercised. Once and
macro push/pop leave no event. `_Pragma` can execute during eager argument
prescan; opaque observation metadata carries its spelling to the output
position. Event identities prevent a cached argument from reporting one
execution twice. Metadata is zero width when a later macro stringifies or
pastes the argument. It never enables phase-7 conversion on the output
stream. Ordinary compilation defaults to observation disabled.

For every input, the harness additionally runs the plain C observer and
plain modeled preprocessor. Erasing only the event records must recover
the plain token stream, and final pack pointer/value, counter and pragma
library count must agree. This finite check is required before a raw exact
match is counted. It does not prove noninterference of every compiler state
or every possible macro composition.

## Protocol and gates

`TCCPP 1` has byte-length-checked lowercase hex token records, explicit STATE
and END/count records, and a disjoint single ERROR record. A successful
empty stream needs a completion trailer. Process crashes, timeouts, unknown
records, partial/mixed output, malformed hex/length, unbalanced event markers
and inconsistent exits fail the harness. Literal escapes are never decoded;
only the protocol's hex or the spec driver's byte serialization is decoded.

Reproduce after the capped targets in `token-build.json` are built:

```
python3 scripts/build_pre_tokens_ref.py
python3 scripts/gen_pre_token_cases.py
python3 -m unittest discover -s scripts/tests -p 'test_*tokens*.py'
python3 scripts/pre_tokens_diff.py --diagnostic --auxiliary --jobs 4 \
  --out results/pre/token-focused.json build/pre-token-cases/*.c
python3 scripts/collect_corpus.py copy
python3 scripts/pre_tokens_diff.py --diagnostic --auxiliary --corpus --jobs 4 \
  --out results/pre/token-corpus.json
python3 scripts/freeze_pre_tokens.py
```

The default comparator requires exact raw AND spec equality on every input.
`--diagnostic` retains refusals and semantic residuals; it still fails on
protocol/harness failures and observation changes to plain compiler behavior.
Joint refusals are never counted as exact successes. The corpus collection
uses the existing manifests/generator, including all 200 Csmith inputs,
CompCert and TinyCC copies, tests2, pp tests, and all project C fixtures.

## Auxiliary printer evidence

Ordinary patched tcc/GCC `-E` remain a separate cfgTccE lane; cfgTccE omits
the parser declarations emitted by the raw -run configuration. The harness
retains the legacy phases-1–3 re-lex classification and also tokenizes printed
bytes with phase 3 alone (`pre_spec --lex-printed`). Reapplying physical phase
1 to output can change `"??="` or turn `"??/"` into a stray backslash.

A printer-inserted `\?` is classified as `printer-question-escape`, solely
when removing that protection accounts for otherwise corresponding literal
spellings. This is a diagnostic classification, never a raw equality rule.
A printer that leaves a trigraph spelling unprotected is classified as
`printer-unprotected-trigraph`, even if phase-3-only lexing recovers the
raw spelling. Both encodings are kept distinct from exact auxiliary equality.
Other differences and printer errors retain separate categories and contexts.
Host path rewriting and host date/time matching are confined to this auxiliary
lane; neither raw stream receives such normalization.

## Focused results

49 inputs: **41 exact raw model/reference matches**, **8 joint compiler
refusals**, zero raw mismatch, model limitation, observer/plain difference,
timeout or harness error. The spec has 36 exact agreements, 6 errors, 2
successful spec outputs rejected by both compilers, and 5 strict token-stream
residuals. All 15 protocol/golden test methods pass.

| retained spec residual | observation |
|---|---|
| ignored pragma macro operand | tcc retains `X`; spec expands to `7` |
| ignored pragma counter operand | tcc's following counter is `0`; spec's is `1` |
| reused argument containing `_Pragma` | one execution/event versus duplicated spec events |
| subsequent stringification | compiler produces `"word"` and an event; spec stringifies the event payload |
| subsequent token pasting | spec loses the event's closing position, consuming later tokens into it |
| invalid `pack(3)` | both compilers reject; spec leaves a successful pragma event |
| invalid `_Pragma(u8"...")` operand | both compilers reject; spec accepts |

The last two are rejection residuals, not exact successes. All seven are
retained with diagnostics or hexadecimal token contexts in the error and
residual JSON files. The eight negative fixtures also cover unterminated
comments/strings/macro invocations, missing includes, `#error`, and invalid
paste definitions.

Auxiliary focused results (49 attempts): tcc has 38 exact, 2
printer-question-escape, 2 other differences, 6 joint errors and 1 printer
error; GCC has 35 exact, 2 printer-unprotected-trigraph, 5 other differences,
5 joint errors, 1 spec error and 1 printer error. On this pinned xPack GCC
invocation the post-splice/stringification trigraph probes are printed
unprotected. Their raw streams still agree exactly with the model/tcc
internal observation. These printer classifications are not raw successes.

## Corpus results

618 inputs: **612 exact raw model/reference matches**, **6 joint compiler
refusals**, zero raw mismatch, model limitation, observer/plain difference,
timeout or harness error. The 612 include all 200 Csmith inputs. All raw
successes also pass both plain-run token/effect comparisons.

The spec agrees exactly on 610 inputs, errors on 7, and successfully emits
one stream rejected by both compilers. There are no token differences on
inputs accepted by all three. The eight retained cases are:

| input | retained result |
|---|---|
| `pp/03.c` | spec success, compiler-mode stray-backslash refusal |
| `pp/21.c` | spec rejects signed overflow in `#if`; both raw compilers accept |
| `pp/22.c` | spec rejects unterminated macro invocation; both raw compilers accept |
| `tests2/114_bound_signal.c` | missing semaphore header |
| `tests2/119_random_stuff.c` | missing sys/mman header |
| `tests2/145_winarm64_interlocked.c` | missing Windows header |
| `tests2/18_include.c` | fixture's relative parent include is unavailable |
| `tests2/95_bitfields.c` | fixture's recursive C-file include is unavailable |

These were not removed from the corpus or included in the success quota.
The last five are fixture/environment refusals; the first three require
separate spec/constraint rulings, not spelling normalization.

Auxiliary corpus results: tcc has 610 exact, 1 other difference, 2 spec errors
and 5 joint errors; GCC has 606 exact, 5 other differences, 2 spec errors and
5 joint errors. The tcc auxiliary difference is the already documented host
character signedness in `c/tests/pre/ifexpr.c`; the RV64 internal raw stream
agrees with the spec there.

`token-focused.json` and `token-corpus.json` retain every attempted row,
source/header/tool hashes, final-state observations, diagnostic text and
hexadecimal mismatch contexts. `token-errors.json` and `token-residuals.json`
separate errors/refusals from strict token differences. `token-build.json`
records the final matching capped build request/result, source manifest,
build-log hash and summaries. The strict default comparator is tested to
accept a positive fixture and reject both a joint refusal and a spec residual.

## Remaining connections

`TccModel.PreprocessCorrect_Statement` instantiates the existing exact
`CSem.Pre.TccppCorrect_Statement`; it is not proved. The spec still needs
repairs revealed by strict comparison, rather than a change to Supported or
an escape-decoding equality. In particular, compiler-mode tcc does not expand
ignored pragma operands: `#pragma unknown __COUNTER__` leaves the following
counter at 0. The current spec expands it and yields 1.

`TccModel.PragmaObservationEffects_Statement` and
`TccModel.PreprocessObservationNoninterference_Statement` state the open
instrumentation connections with named effect fields. The latter compares
whole ordinary and observed model runs, including their token projections.
F6 still owes the binary-to-model refinement. None of the comparisons, build
successes, or standard-axiom audits are a proof of those propositions.
The final audit prints only {propext, Classical.choice, Quot.sound} for the
interface and all three Statements. No axioms or proof budget changes were
introduced. The frozen sum artifacts and Supported are unchanged.

Failed approaches and superseded build attempts are in `tasks/DEAD-ENDS.md`
and `token-builds/`; only the matching final receipt is validation evidence.
