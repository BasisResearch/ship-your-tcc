# C11 compiler-model recovery, 2026-10-27

Final validation uses **successful capped build c11-model-6**, with its frozen
source hashes verified before and after the comparisons. **c11-model-5 failed**
at the premature doc-comment terminator in Libtcc; it was never validated.
All counts here come from fresh model executions after request 6. Earlier
request-4 comparisons and reference-only prefill records are historical and
are excluded. The fast reference is **rv64emu diagnostic**; complete Sail
arena equality remains open.

## Changes and review

Reviewed the interrupted C11 and option-state changes from checkpoints
0f99b7b and ff16a14. Retained UTF element types and phase-6 concatenation,
raw digraph spelling, isolated pragma streams and rescanning, UCN spelling
and canonical identity, relative #line, literal-only stringification escaping,
and union-member/descendant/relocation invalidation. Tok.lean remains generated
by scripts/gen_model_tok.py; the 733 old decimal token constants retain their
values, new constants occupy 0xd0–0xd9, and `_Pragma` is allocated by spelling
after architecture keywords.

Fixed Libtcc's doc comment, explicitly NUL-terminated split_option_args input
(Bytes.cstrAt omits the C sentinel), and made escaped quote/backslash cursor
consumption explicit. Quoted, unmatched-quote, escaped-quote, escaped-backslash
and trailing-backslash regressions all compile to identical arenas. Retained
libtcc's FlagDef tables, no-/error= forms, warning masks, -Wall propagation and
inverted flags; pragma options call the main option parser through Rec/Tie.
Reversed argument-token replay agrees on the focused nested-call fixture and
the corpus cases. Separate -Wl,/-Wp, dispatch precedes generic -W flags:
pragma -Wp, is ignored as in C; unimplemented linker options and command-line
-Wp, remain explicit model gaps rather than silently becoming warnings.

The lead's pragma-operand repair (9b0813f, integrated here as ea62589) is
mirrored exactly: pragma_operator accepts and returns the caller's active-macro
list, expands identifier operands with macro_subst_tok when not suppressed,
then begins their token stream and resumes next(). Direct-source calls pass
an empty active list. The compiler patch was not changed to fit the model.
Both the minimized c11-pragma-operand.c and the lead's object/function/nested
operand reproducer now have successful identical arenas. A recursive operand
still gives a matching refusal, testing recursion suppression separately.
The former valid-C11 pragma-operand counterexample is repaired, not excluded
from Supported. The lead's full compiler Sail validation is recorded in
[the pragma-operand report](../review/c11/pragma-macro-operand/README.md).

The independent exact binary128 conversion remains SF.ofDecimal SF.F128;
no reference bytes enter the model. Patch 0004 is unchanged. The corrected
cache fingerprint uses checked objcopy into a regular temporary file and
hashes runtime bytes without .embed, every ordered fs.list path/content,
fs.list itself, gen_embed.sh, ref_arena.sh, rv64emu.c and model_diff.py.
Per-case keys include source, extra-file paths/content and arguments.
Atomic checked publication of image.syms is retained. No sister-project
proof was imported; the existing ATTRIBUTION.md TccModel row applies. The
sister-project abstraction inventory was run and its mandatory tool table
read. No proof budgets or proof escapes were introduced.

## Build and final comparisons

Source base: ea62589d8b2fbc46c8c9a3a0c83ba0bf67a9c924 plus the request-6
SHA256 manifest. The worker reported 83 GiB available before requesting the
build. The lead verified the frozen hashes and ran:

```
free -g # require at least 25 GiB available
flock "$SYT_LAKE_LOCK" systemd-run --user --scope -p MemoryMax=12G lake build tcc_model
```

**58 jobs succeeded.** Request, result and complete build log are retained in
c11-build-6.json and c11-build-6.log. No uncapped Lean build was substituted.
Static gate and token generator drift checks pass. The static gate checks the
existing sum artifacts for drift; none was regenerated or edited. Compiler
patches, CSem, Supported, Fits, STATUS.md and PHASES.md are unchanged.

The sequential final commands were:

```
python3 results/model/gen_c11_fixtures.py
python3 scripts/model_diff.py c11 results/repro/tcc_pp_digraphs.c results/repro/tcc_pp_line_relative_name.c results/repro/tcc_pp_pragma_operator.c results/repro/tcc_pp_stringize_escapes.c results/repro/tcc_pp_ucn_identifier.c results/repro/tcc_union_designator_override.c results/repro/tcc_pragma_macro_operand.c build/c11-model-focused/*.c results/model/c11-state.c results/model/c11-options.c results/model/c11-pragma-operand.c -j 4 --no-report --json-out results/model/c11-focused-final.json -v
python3 scripts/model_diff.py all -j 8 --no-report --json-out results/model/c11-all.json --require-success 1000
MAX_STEPS=4000000000 python3 scripts/model_diff.py build/ld-stress.c -j 1 --no-report --json-out results/model/c11-binary128-stress.json -v
python3 results/model/freeze_c11_validation.py
scripts/check.sh --static-only
```

The stress input is reproducible with
`python3 scripts/gen_ld_literal_tests.py --stress-out build/ld-stress.c`.
The generated corpus is the retained cgen/Csmith/mutants/dtsplit corpus;
every attempted input is SHA256-pinned in c11-all.json. Both drivers pass
`-std=c11` and identical `-w` arguments.

| final run | attempted | identical successful arenas | matching refusals | unsupported asm | harness failures |
|---|---:|---:|---:|---:|---:|
| focused C11/reproducers/state/options | 41 | 34 | 7 | 0 | 0 |
| non-generated corpus | 555 | 519 | 28 | 6 | 2 |
| generated cgen | 1,000 | 701 | 101 | 0 | 198 |
| generated Csmith | 400 | 350 | 0 | 0 | 50 |
| generated mutants | 400 | 81 | 319 | 0 | 0 |
| generated dtsplit | 130 | 19 | 100 | 11 | 0 |
| all corpus/generated | 2,485 | 1,670 | 548 | 17 | 250 |
| binary128 stress (156 vectors) | 1 | 1 | 0 | 0 | 0 |

**Zero arena mismatches, zero model errors, zero skipped inputs.** Cgen plus
Csmith alone supply **1,051 successful generated arenas**, independently of
the CLI's overall threshold. The all-corpus command returns 1 because asm and
harness cases remain visible; the focused and stress commands return 0.
No refusal, unsupported case or harness failure contributes to arena success.

c11-focused-final.json, c11-all.json and c11-binary128-stress.json retain every
case and exact expanded command. c11-residuals.json retains every non-success
classification. c11-pins.json pins the final sources, executable, patches,
image, emulator, symbols, generators, reports and all 153 embedded filesystem
inputs. freeze_c11_validation.py verifies every report input/executable hash,
the current runtime key, the frozen build-source hashes and the separate
1,000-generated-success requirement before publishing pins/residuals.
Final runtime key:
`e9bab6da0bd738add1cc18cb839ff66de31880e3c9f75b8f337eca4e51079254`.
Final tcc_model SHA256:
`c275c4219a67c90d84a127bbd873121855169586204fca13ec7ebd6fd679f7d0`.

## Residuals, ledger text and dead ends

The 250 harness failures are 249 emulator limits and one instruction trap.
Tests2/101_cleanup reaches 400M steps before handoff. Tests2/141_riscv_asm
traps on forbidden floating instruction 0xfe842507 after handoff. All 198 cgen
and 50 generated-Csmith limits occur after handoff; they remain excluded from
arena agreement under the present harness. A separate worker owns
scripts/ref_arena.sh and scripts/rv64emu.c; this task did not edit them.
All 548 compiler refusals and 17 unsupported assembly cases are retained by
name, with their diagnostics/classifications, in c11-residuals.json. Matching
refusals are not a correctness escape for valid Supported/Fits C11 programs.
The whole-compiler goal and its no-refusal headline are unchanged.

Suggested lead ledger text: patched-C11 TccModel builds under the shared
lock/12 GB cap with request-6 source hashes verified. Final independent fast
reference comparisons have 34 focused arena successes, 1,670/2,485 full-run
arena successes, including 1,051 cgen/Csmith successes, and one 156-vector
binary128 stress arena; zero mismatches/model errors. Keep the separate 548
refusals, 17 asm cases and 250 harness failures visible. Pragma macro operands
now reflect the repaired compiler and have successful arenas. Ghost GNode is
still a lossy trace, not Cabs; fuel adequacy, compiler/model proofs and full
Sail arena equality remain open. This is diagnostic F3 progress, not its
Sail-equality exit or the whole-compiler theorem.

Dead ends: request 5 failed; never cite it as validation. Do not use objcopy
/dev/stdout (partial output plus Illegal seek), stale reference caches,
pre-edit binaries, or aggregate agreement including refusals. Do not execute
replacement-stream pragma operands without the caller's active-macro list,
or let generic -W swallow -Wl,/-Wp,. Do not rely on an out-of-bounds ByteArray
read for C's NUL sentinel or skip an extra byte after an escaped option byte.
Do not publish image.syms by truncating the live file. Do not reintroduce the
reserved prefix locals or malformed pragma record layout. The binary128
gdtoa-wrapper dead ends remain in tasks/DEAD-ENDS.md; preserve exact patch 0004.
Git metadata and the systemd user bus are read-only/inaccessible in this
worker sandbox; keep the lead handoff protocol, with no alternate object
store and no uncapped build. No push or merge of main was performed.

Final commit handoff: ordinary `git add` was denied while creating the real
worktree index.lock (`Read-only file system`). The complete reviewed diff is
ready for the lead's ordinary commit; exact paths, SHA256s and the proposed
message are in build/worker-commit.request, also referenced by the serviced
build request. No alternate Git metadata location was used.

Lead integration: after replay on main's RV64 configuration and filesystem
model, the combined `scripts/check.sh --csem` passes (633 targets, 19 unit
executables, 230 generated files reproduced, 113 baseline classifications and
standard-only axiom cones). This does not upgrade fast-reference evidence to
Sail arena equality. No frozen sum compile-cut artifacts were regenerated.
