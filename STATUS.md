# Current status

Takeover in progress, resumed 2026-10-28. The approved target is
`tcc_run_correct` in DESIGN-COMPILER.md: whole compiler, `Supported` and
`Fits` premises, no refusal disjunct. F0 is landed; F1–F3 remain open.

## Decisions in force

- CSem remains C11. Invoke tcc with `-std=c11`; patch its deviations with
  reproducers in `patches/0003`: string prefixes, digraphs, `_Pragma`, UCN
  identifiers, relative `#line`, stringification escaping, and union
  designated initializer overrides.
- Keep the existing `sum.c` proof as the non-vacuity witness. Regenerate
  its compile-cut chain once the compiler image is stable, not during F0–F3.
- Review and recover previous worktrees; commit and push verified increments
  to `origin/main`, without force pushes.

## Active work

Mike Dodds's upstream follow-up is tracked in CSEM-UPSTREAM.md. README and
ATTRIBUTION prominently credit Cerberus / Cambridge REMS and cerberus-lean /
Mike Dodds / Oath. The scheduled repin evaluation is complete (results/csem/repin-2026-10-27.md);
the repaired repin is adopted at Cerberus 704c4e69 / Lem e6d8db56, retaining Lean 4.34. Kiran owns
external bug reporting; no external contact is authorized or planned.

| lane | recovery source | review and next check |
|---|---|---|
| C11 patches | new `codex-c11` worktree; existing F2 reproducers | patch 0003 integrated: 7/7 full compiler Sail reproducers; combined 0003/0004 host/cross tests and main gate pass |
| F1 grammar | `agent-a31b97b8f1c090141` | integrated: main capped build and full comparison pass, 614 exact Cabs agreements / 664 inputs; 41 joint rejections, 9 preprocessing failures |
| F1 pure seams | `agent-a249bbaa5b1df80ec` | digest retirement integrated; main's full CSem gate passes: 19 unit executables, 113 baseline oracle results; remaining partial/native seams inventoried |
| F1 ABI/allocation | `agent-a4ab6bbd8c0be5325` | RV64 reader and bounded allocation oracle adopted after full gates; fresh p04/p09/p10 checks close the earlier IEEE/stdin probe failures; bitfields and GNU enum coverage remain |
| F1 filesystem | `agent-ad868c434f5f27398` | integrated after full gate: descriptor fstat, structural state ordering and link proxy repaired; eight exact Sail byte/exit agreements, repeated with RV64 config; mkdir remains a separate missing-library gap |
| F1 IEEE | `agent-a45cb08cf87ce958c` | integrated after worker and combined full gates: pure 32/64/128 bits, zero native floating in both cones, unchanged 113-case baseline; literal/printf/sqrtl gaps and connection Statements remain open |
| F3 compiler model | `agent-a05464165befb7074` (recovery merged) | patched C11 model integrated after capped build and combined full gate; 1,345 matching fast arenas / 1,815 inputs, including 1,082 C-generator and 80 mutant successes; all three repaired include fixtures and the 78-case anonymous-member batch have full Sail evidence; all-corpus Sail equality remains open |

F3 review found reference-derived long-double bytes supplied to the model.
Such runs cannot establish independent byte equality. That input is now
removed. Patch 0004 fixes compiler literals using exact integer rounding;
public libc `strtold` still needs correction in the library lane. Matching refusals likewise do not satisfy the F3 success criterion.

The lead now runs external `gpt-6.1-sol` workers via `codexwork.sh`, at most
two at a time, following MANAGER.md. Slot 1 now handles the isolated CSEM-10 function-pointer redeclaration defect;
library identity and bounded allocation are adopted. Slot 2
continues ghost Cabs from modeled compiler parser actions. Both resumed after
the interruption; the latest continuation status is below. The anonymous-member repair is adopted with IEEE after its combined gate.
The pure IEEE implementation is integrated after the combined gate; the compiler/model
anonymous-container qualifier repair is also adopted. Exact-token/pragma integration is now
validated with the nested-designator repair: strengthened full gate, 147
positive/14 invalid C11 checks, 18 protocol tests, 64 focused positives and
17 joint rejections. The 619-input corpus has 613 raw/611 spec agreements;
residuals remain separate. Fresh broad comparison has 1,339 matching arenas,
466 refusals and seven assembly gaps, zero mismatches/model/harness errors.
Three fresh complete 4 MiB native Sail arenas agree on this combined image
(results/pre/combined-integration-2026-10-27.md). All three preprocessing
correctness/noninterference Statements remain open. The default gate now
explicitly rebuilds the previously non-default model/observer executables;
the stale-binary diagnostic and corrected rerun are retained.
The line repair is integrated, and the scheduled CSem repin plus selected-member
union initializer repair is adopted after its integrated full gate. The compiler-handoff collector and strict model_diff consumer are integrated.
Fresh 1,412-input comparison: 1,308 arenas (1,299 generated), 104 refusals,
zero mismatches/harness/model errors (results/model/handoff-integration.md). The RV64 reader and image filesystem are
integrated; their fresh probes and remaining semantic gaps are documented.
Built-in agents remain stopped.

The patched C11 model's frozen request-6 build passes, followed by the full
combined ABI/filesystem gate. Fresh independent fast-reference results:
34 focused arenas, 1,670/2,485 broad arenas (including 1,051 cgen/Csmith), and
one 156-vector binary128 stress arena; zero mismatches/model errors. The 548
compiler refusals, 17 unsupported-assembly cases and 250 harness failures
remain separate. Most harness failures execute the generated program after
handoff, which the new collector addresses. See results/model/c11-model.md.
Ghost GNode is not yet Cabs; fuel adequacy, compiler/model proofs and full
Sail arena equality remain open. Older pre-C11 counts used a defective cache
key and remain historical diagnostics. The inherited assembler drafts are
preserved in build/recovery/assembler/. The extra macro-operand compiler repair
passes host/cross tests and full Sail (results/review/c11/pragma-macro-operand/).

F2 recheck after patch 0003: 585 token-equal inputs / 593 after UCN identity and `_Pragma`
operand/effect repairs. The cross-host signed-character difference is already
diagnosed; two UB examples and five fixture errors remain separately classified.
The comparator now requires exact equality by default and fails on malformed
protocol/crashes. Its 608-input diagnostic rerun has 600 exact tcc agreements
and the same eight residuals (results/pre/acceptance-contract.md). The trigraph patch/config/model repair is integrated after the full gate:
48 positive and 6 negative C11 cases, 28 independent arenas, and one complete
4 MiB Sail arena match (results/model/trigraph-sail-arena.md). Three further
complete Sail arenas cover pragma state, UTF literals and union overrides
(results/model/sail-c11-batch.md).
Further C11 coverage remains work, not a change to the contract. Macro argument/paste locations are repaired in the spec: 38/38 focused
agreements with patched tcc/GCC; 609/617 exact tcc corpus agreements, with
the same eight residuals (results/pre/line-spec-repaired.md). The include
filesystem also incorrectly normalizes away missing directories before lookup
(results/pre/path-resolution.md); the shared CerbFS traversal/initializer repair passes 12 guards and 11 independent
image/model observations and is integrated after the full gate (results/model/image-files-review.md).
The directory-read-as-EOF compiler/model defect is now repaired and gated:
13 image/model observations, 16 host/cross checks, and two full Sail executions
(results/pre/include-read-repair.md).

Builds are serialized across these lanes, capped at 12 GB. The two inherited
`results/review/*.json` handoffs are retained as dated records; all 345 hashes
in `phase2-snapshot-current.json` match the committed Phase 2 result files.
Parser recovery builds in main and the repository gate passes. Approved
GNU forms, K&R definitions and connection to F2/raw-source CSem remain open.
Inherited hard-linked Lake caches have been isolated for active lanes;
main's CSem objects were rebuilt before accepting the digest retirement.


The line-repair broad review has 1,419 identical fast arenas / 1,556 inputs,
129 refusals, seven assembly gaps and one recorded runner instruction-limit
failure. A separate 529-million-step cleanup run now matches the model,
raising successful coverage to 1,420 (results/model/cleanup-followup.md);
no byte differences or model errors
(results/model/line-broad-review.md). The broad run correctly exits nonzero
for the remaining gaps. The IEEE draft review and follow-up task are recorded
in results/csem/ieee-recovery-review.json and tasks/f1-ieee-recovery.md.

The repaired CSem union suite also exposes tcc's valid nested-designator
continuation refusal (results/repro/tcc_designator_continuation.c). The compiler/model repair now passes its integrated gate: 144 positive/14
invalid checks, 58 Sail/GCC program comparisons, 62 focused and 1,306 broad
independent arenas, and three full compiler Sail runs including the complete
33-case conforming union suite (results/model/designator-continuation.md).
498 refusals, seven assembly gaps and six CSem gaps remain separate. This
does not change Supported/Fits. The repaired repin and its integrated full gate pass;
remaining aggregate-value, bitfield and aggregate-value gaps remain explicit; later IEEE/anonymous repairs are recorded below.

The IEEE and anonymous-member lanes have exposed further compiler/library
discrepancies. Follow-up tasks are `tasks/f3-direct-float-literals.md` and
`tasks/f3-anonymous-members.md`; each requires minimized independent evidence
and preserves the whole-C contract. Newlib binary128 printf/sqrtl remain F7
obligations, separate from pure IEEE arithmetic.

Direct literal repair: all four original same-program counterexamples now
match exact expected bits on full Sail; the independent 4 MiB handoff arena
also matches. The 189/789-case generated checks and fresh broad comparison
are retained in results/model/direct-float-fixed-2026-10-27.md. The full gate and 15-case native boundary probe pass;
the repair is adopted. Raw-frontend UCN identity is implemented; pack-layout
consumption remains explicitly unimplemented. Anonymous-member CSem review
now distinguishes private container paths from source-visible names. Worker receipts
now surface these lead notes before finalization.

Raw frontend integration is adopted after its combined full CSem gate: pure
raw bytes reach provenance-carrying positioned tokens/Cabs with UCN/UTF-8
identity and pure per-TU digest. All 22 focused methods and 18 independent
full-Cabs comparisons pass. The current 622-input preprocessing corpus has
616 raw/614 spec exact agreements, with residuals separate. The checked entry
explicitly diagnoses missing pack consumption and library-identity mismatch;
these remain whole-C obligations, not Supported restrictions or a universal
F0 substitution. Four named connection Statements remain open. Fresh pins
and logs are in results/csem/raw-frontend/integration/. Allocation recovery
is assigned to external Sol slot 1; the anonymous CSem repair is adopted with IEEE after its combined gate. Slot 2 is assigned the independent ghost-Cabs task.
Runtime-library identity consumption is queued as tasks/f1-library-identity.md.

Compiler anonymous-container qualifiers are repaired and gated: old full Sail
selects the wrong const/volatile pointer types; repaired Sail and GCC agree.
The extended array/pointer/nesting/evaluation fixture passes, and invalid
const assignment now diagnoses. Fresh 1,342/1,815 independent arenas and one
complete native arena match, with 466 refusals and seven assembly gaps kept
separate (results/model/anonymous-members.md).

CSem anonymous containers are now adopted after the combined full gate and
235-file regeneration: 92 focused agreements, 34 union checks and 54/58
continuation agreements, with four explicit bitfield gaps. Public member
lookup rejects guessed private container names. Both adapters use the passed
union-layout table; the inherited fuel proof is adapted without larger bounds.
Evidence is in results/csem/anonymous-members/integration/, and CSEM-UPSTREAM.md
records CSEM-07/PATCH-08 and CSEM-09/PATCH-09 for Kiran. No external report sent.

Latest checkpoint (2026-10-28): the opened-path pragma-once policy and corpus
fixture materialization pass the combined full CSem gate. The 622 inputs now
give 618 raw/616 spec exact agreements, zero differences/harness failures;
six residual spec classifications remain explicit. Fourteen preprocessing,
eleven raw frontend and seven protocol methods pass, plus the full Sail once
reproducer. See results/pre/once-cache-2026-10-28.md. Sum cuts remain frozen.

Both external Sol processes stopped at their usage limit during the interruption;
their source changes are preserved. Both slots restarted successfully on Oct 28:
slot 1 handles library identity and slot 2 resumes ghost Cabs. Allocation
checkpoint ccc00ed and its combined raw/anonymous/once integration pass;
the reviewed implementation is adopted with native placement evidence.
Ghost Cabs remains work in progress; ordinary source mapping is required.
No unfinished checkpoint is claimed as an adopted implementation.
The next independent CSem coverage task is tasks/f1-bitfields.md (CSEM-08):
representation, target layout and semantic accesses, including unnamed padding.
It is queued, not a third active worker. Fresh p04/p09/p10 configuration
checks now agree exactly between CSem and full Sail: binary128 precision,
binary32 rounding/conversion and stdin EOF/feof. Exact captures are in
results/csem/config-followup-2026-10-28.md; the earlier failures remain dated
evidence. This does not recertify the other twelve configuration probes.

Allocation adoption (2026-10-28): arbitrary checked placement functions now
range over CSem.outcomes; default bump execution is retained. The combined
full gate, 21 allocation checks, six default/historical comparisons and 11
raw frontend tests pass, with all 2,293 source hashes verified. Generic
placement/dispatch proofs use standard axioms. Invalid replies have no C
outcome and remain separate from UB/refusal. BumpPlacementPre, frontend
constant-expression mini-runs, memory simulation and eventual fuel adequacy
remain explicit F4/F5 duties. See results/csem/allocation-oracle.md.

Library identity adoption (2026-10-28): the combined full CSem gate passes,
with 2,291 unchanged tracked input hashes, 11 raw methods, 22 identity passes
and 5,580 semantic consumer checks. The two inherited pointer failures remain
explicit; slot 1 diagnoses their independently established local Lean defect.
The default/historical allocation comparisons still pass. See
results/csem/library-identity.md. GNU declaration attributes in the actual
injected tccdefs are queued next in tasks/f1-gnu-attributes.md; this independent
spec work complements slot 2's model-owned macro provenance/ghost construction.
The existing bitfield task remains queued. None is a Supported exclusion.

F3 fixture adoption (2026-10-28): F2/F3 now share explicit source/header
aliases. Three formerly matching refusals become successful compilations and
complete native Sail arena matches. The final broad run has 1,345 successes,
463 refusals and seven assembly gaps / 1,815 inputs, with no other differences
or errors. Its exit 1 preserves the assembly gaps. The static gate and focused
protocol checks pass; source/input/image pins and earlier timeout/overlap
failures are retained in results/model/corpus-fixtures-2026-10-28.md.
Whole-corpus native coverage and ghost/compiler proofs remain open.

Comparison isolation (2026-10-28): model_diff now serializes invocations in
one worktree and rejects duplicated selected paths. Two overlapping real
comparisons, protocol tests and the maintained static gate pass. The prior
stopped overlap remains a diagnostic, not accepted evidence. See
results/review/model-comparison-isolation-2026-10-28.md.

## Needs user

None currently. Implementation defects and unfinished obligations are work
items, not requests to weaken the target.
