# Current status

Takeover in progress, resumed 2026-10-27. The approved target is
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
| F1 ABI/allocation | `agent-a4ab6bbd8c0be5325` | RV64 configuration reader integrated after capped build/full gate; 10/15 probes agree, float/bitfield/enum/stdin gaps remain; allocator recovery still open |
| F1 filesystem | `agent-ad868c434f5f27398` | integrated after full gate: descriptor fstat, structural state ordering and link proxy repaired; eight exact Sail byte/exit agreements, repeated with RV64 config; mkdir remains a separate missing-library gap |
| F1 IEEE | `agent-a45cb08cf87ce958c` | review standalone arithmetic; connection to CSem remains open |
| F3 compiler model | `agent-a05464165befb7074` (recovery merged) | patched C11 model integrated after capped build and combined full gate; 1,670 independent fast arenas including 1,051 generated successes; four full Sail arenas now agree; all-corpus Sail equality remains open |

F3 review found reference-derived long-double bytes supplied to the model.
Such runs cannot establish independent byte equality. That input is now
removed. Patch 0004 fixes compiler literals using exact integer rounding;
public libc `strtold` still needs correction in the library lane. Matching refusals likewise do not satisfy the F3 success criterion.

The lead now runs external `gpt-6.1-sol` workers via `codexwork.sh`, at most
two at a time, following MANAGER.md. Slot 1 repairs exact pragma specification residuals; slot 2 repairs valid
nested-designator continuation in the compiler and model. The lead holds the
raw-token integration while correcting three newly found observer failures.
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
continuation refusal (results/repro/tcc_designator_continuation.c). The next
compiler/model task is tasks/f3-designator-continuation.md; this does not
change Supported/Fits. The repaired repin and its integrated full gate pass;
remaining aggregate-value, anonymous-layout and IEEE gaps remain explicit.

## Needs user

None currently. Implementation defects and unfinished obligations are work
items, not requests to weaken the target.
