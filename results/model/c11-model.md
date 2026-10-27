# C11 compiler-model recovery, 2026-10-27

Current handoff status: **request c11-model-5 is pending**. Its source is
frozen. Counts below validate the earlier successful c11-model-4 executable;
they do not validate the subsequent option-state changes. The static gate
passes on the later source. Before the next final build, append a NUL sentinel
in split_option_args (its input from Bytes.cstrAt excludes NUL); this was found
during the source freeze and has not yet been applied. Wait for the lead's
matching build result before editing any frozen source.
Also preserve separate -Wl/-Wp dispatch before the new generic -W flag
branch; those are linker/preprocessor argument forms, not warning names.

The checkpoint `5d4edbb47d5ea8eef7a8d845118c5e5a691d0144` was unvalidated.
This continuation fixes its Lean syntax and the reference fingerprint, then
checks the independently computed model arenas against patched TinyCC 43c7708
under `-std=c11`. The reference is **rv64emu diagnostic**, not Sail arena
equality. The F3 Sail arena-equality exit requirement remains pending.

## Implementation and build

The checkpoint models patches 0003 and 0004: UTF element types and phase-6
prefix selection, raw digraph spellings until parser conversion, isolated
`_Pragma` streams and rescanning, UCN spelling plus canonical identifier
identity, relative `#line`, literal-only stringification escaping and union
member invalidation including descendant records/relocations. Both drivers
prepend `-std=c11`; the differential supplies `-w` identically. The compiler's
exact binary128 patch is unchanged, and the model continues to use independent
exact rational conversion (`SF.ofDecimal SF.F128`). No reference bytes enter
the model. No library proof or new sister-project machinery was imported.

Reserved `prefix` locals in PpDir/PpMacro became `literalPrefix`, and the
isolated pragma state updates use valid multiline structure layout. Generated
Tok.lean passes `python3 scripts/gen_model_tok.py --check`; all 733 old decimal
token constants retain their values. New constants occupy 0xd0–0xd9 and
`_Pragma` is allocated by spelling after architecture keywords.

`runtime_key` uses checked objcopy output in a temporary regular file,
excluding `.embed` from the runtime binary. It hashes that binary, every
ordered fs.list image path and host-file content, fs.list itself,
gen_embed.sh, ref_arena.sh, rv64emu.c and model_diff.py. Per-case keys also
hash source, extra-file paths/bytes and arguments. Schema `independent-c11-v3`
invalidates prior reference caches; only this fresh namespace is used here.

The lead built request **c11-model-4**, verifying its source hashes, with:

```
free -g # 83 GiB available when requested; require at least 25
flock "$SYT_LAKE_LOCK" systemd-run --user --scope -p MemoryMax=12G lake build tcc_model
```

Result: **58 jobs, success** (`build/worker-build-c11-model-4.log`, matching
`build/worker-build.result`). Earlier requests 2/3 failed on the reserved local
and pragma structure syntax. No uncapped Lean build, budget increase or proof
escape was used. Compiler patches, CSem, Supported, STATUS.md, PHASES.md and
the sum compile-cut artifacts are unchanged.

The broader generated run exposed five pre-existing model errors for
`#pragma comment(option)` and one transient `image symbol errno missing`.
The latter came from concurrent harness invocations truncating image.syms
while model processes were reading it. The harness now generates a checked
temporary symbol file and publishes it with os.replace. Its changed hash
invalidates the previous cache namespace again; 2,485 fresh reference inputs
were collected under key
`a389d2ea0190cb60079b61a2339bb8a0c71faa966c7d70456c6ae52759b6d25f`
(1,671 handoffs, 564 refusals, 250 harness errors). These are reference
records, not model arena-equality counts.

Pending request c11-model-5 adds libtcc's named FlagDef tables and set_flag
semantics (no-, error=, -Wall propagation and inverted flags), routes pragma
options through the main option parser via Rec and implements reversed
argument-token replay in unary_call. c11-options.c is the focused pragma
state/argument-order fixture. Final model comparisons must be repeated after
the capped final build; the new harness key prevents old reference entries
from contributing.

## Validation commands and counts

```
scripts/check.sh --static-only
python3 scripts/gen_model_tok.py --check
python3 results/model/gen_c11_fixtures.py
python3 scripts/model_diff.py c11 results/repro/tcc_pp_digraphs.c results/repro/tcc_pp_line_relative_name.c results/repro/tcc_pp_pragma_operator.c results/repro/tcc_pp_stringize_escapes.c results/repro/tcc_pp_ucn_identifier.c results/repro/tcc_union_designator_override.c build/c11-model-focused/*.c -j 4 --no-report --json-out results/model/c11-focused.json -v
python3 scripts/model_diff.py results/model/c11-state.c results/model/c11-pragma-operand.c -j 2 --no-report --json-out results/model/c11-state.json -v
python3 scripts/model_diff.py tests run tcc p3 compcert tcctests csmith tests2 model c11 -j 4 --no-report --json-out results/model/c11-corpus.json
python3 scripts/gen_model_corpus.py cgen 1000 --seed 1
python3 scripts/model_diff.py gen-cgen -j 4 --no-report --json-out results/model/c11-generated-cgen.json
```

Static gate passes. The 29 focused cases give **23 identical successful
arenas and 6 matching invalid-UCN refusals**, zero differences/model/harness
errors. These include all six C11 tests, all six original defect reproducers,
raw-byte repeated stringification and nine UCN refill placements. The fixture
generator reproduces all 17 additional inputs byte for byte against their
recorded hashes. The combined state regression adds **one identical arena**:
`once` after `#line`, pragma include/pack restoration, suspended pop_macro
rescanning, UCN parameter identity and phase-6 lookahead across pragma macros.

The non-generated corpus gives **555 attempted, 519 identical arenas,
28 matching refusals, 6 unsupported assembly cases, 2 harness errors**, zero
arena differences and zero other model errors (19 s wall using fresh reference
cache entries). The harness errors are tests2/101_cleanup (400M steps before
handoff) and tests2/141_riscv_asm (forbidden floating instruction after handoff).
Refusals are diagnostics, never successful arena coverage.

The c11-model-4 generated runs covered 1,930 inputs: **701/1,000 cgen** and
**350/400 Csmith** identical arenas, meeting 1,000 successful generated arenas
with **1,051** even before mutants/dtsplit. Cgen also had 101 matching refusals
and 198 harness errors; Csmith had 50 harness errors. Mutants: 80 identical,
319 refusals, one transient symbol-read failure. Dtsplit: 18 identical,
96 refusals, 11 unsupported assembly, five option model errors. There were
zero arena mismatches. The six errors motivated the pending changes above.
The fresh 156-vector binary128 stress input also had one identical arena
(`c11-binary128-stress.json`), with MAX_STEPS=4000000000. Its cache key must be
refreshed too after the harness change.

Additional harness checks (`build/c11-runtime-key-check.log`,
`build/c11-harness-check.log`) verify stable keys, invalidation by embedded
header bytes/image paths, rejection of failed objcopy with partial output,
one-step emulator exhaustion as a harness error and failure on missing/empty
corpora. No limit/trap is counted as a compiler exit or arena success.

## Residual cases, ledger text and dead ends

`c11-pragma-operand.c` is a newly minimized **patched-compiler refusal on valid
C11**, mirrored by the model: an object-like macro containing
`_Pragma(PACK_TEXT)` with a macro operand reports `string literal in _Pragma
expected`. `gcc -std=c11 -Wall -Wextra -c` accepts it. This remains a compiler
counterexample to the no-refusal headline on supported, fitting programs;
Supported was not weakened and patch 0003 was not altered. It is kept separate
from the positive state fixture and successful arena counts.

Dead ends for the lead: do not use objcopy `/dev/stdout` (partial .text then
Illegal seek), old reference caches, or old checkpoint binaries as evidence.
Do not repeat the reserved `prefix` locals or pragma record-layout errors.
Do not reinterpret emulator limits/traps, matching refusals or unsupported
assembly as successful independent arenas. The existing binary128 gdtoa
wrapper dead ends remain in tasks/DEAD-ENDS.md; patch 0004 stays exact.

Ghost GNode remains a lossy trace, **not Cabs**. Fuel adequacy, compiler/model
proofs and complete Sail arena equality remain open. The lead should preserve
these qualifications when updating the F3 ledger. No push or main merge was
performed.

Git staging failed with `Read-only file system` creating
`.git/worktrees/agent-a05464165befb7074/index.lock`. The diff is left for the
lead's ordinary commit; no alternate index/object store was used. Suggested
ledger text at this handoff: C11 model checkpoint now has a capped successful
build and 24 focused positive arena agreements plus negative-case agreement;
1,051 generated cgen/Csmith arenas agree under the first corrected cache key.
Later option-state/atomic symbol fixes are source-frozen in request 5 and
await final capped build and fresh full comparison. Keep the compiler pragma
operand refusal, residual harness/assembly classifications, ghost-Cabs gap
and pending Sail arena equality explicit. STATUS.md/PHASES.md were not edited.

Exact image, patch, source, executable and 153 embedded filesystem input pins
are in c11-pins.json. The pending source hashes still match request 5. Its
binary pin explicitly identifies the earlier request-4 executable. The next
validation command after releasing the freeze, fixing the two parser audit
items and completing the capped build is:

```
python3 scripts/model_diff.py all -j 8 --no-report --json-out results/model/c11-all.json --require-success 1000
python3 scripts/model_diff.py c11 results/repro/tcc_pp_*.c results/repro/tcc_union_designator_override.c build/c11-model-focused/*.c results/model/c11-state.c results/model/c11-options.c results/model/c11-pragma-operand.c -j 4 --no-report --json-out results/model/c11-focused-final.json
MAX_STEPS=4000000000 python3 scripts/model_diff.py build/ld-stress.c -j 1 --no-report --json-out results/model/c11-binary128-stress.json
```

Run these sequentially. Verify at least 1,000 `same` entries in gen-cgen plus
gen-csmith independently of the overall CLI success threshold. Do not hide
the expected separate assembly/harness categories behind the CLI return code.
Repeat the static gate and publish final counts/pins only for that executable.
