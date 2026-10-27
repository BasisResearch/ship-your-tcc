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
Mike Dodds / Oath. Repin evaluation is queued in tasks/csem-repin.md now that
the original RV64-config and C11-model tasks have landed; it uses a fresh worktree. Kiran owns
external bug reporting; no external contact is authorized or planned.

| lane | recovery source | review and next check |
|---|---|---|
| C11 patches | new `codex-c11` worktree; existing F2 reproducers | patch 0003 integrated: 7/7 full compiler Sail reproducers; combined 0003/0004 host/cross tests and main gate pass |
| F1 grammar | `agent-a31b97b8f1c090141` | integrated: main capped build and full comparison pass, 614 exact Cabs agreements / 664 inputs; 41 joint rejections, 9 preprocessing failures |
| F1 pure seams | `agent-a249bbaa5b1df80ec` | digest retirement integrated; main's full CSem gate passes: 19 unit executables, 113 baseline oracle results; remaining partial/native seams inventoried |
| F1 ABI/allocation | `agent-a4ab6bbd8c0be5325` | RV64 configuration reader integrated after capped build/full gate; 10/15 probes agree, float/bitfield/enum/stdin gaps remain; allocator recovery still open |
| F1 filesystem | `agent-ad868c434f5f27398` | integrated after full gate: descriptor fstat, structural state ordering and link proxy repaired; eight exact Sail byte/exit agreements, repeated with RV64 config; mkdir remains a separate missing-library gap |
| F1 IEEE | `agent-a45cb08cf87ce958c` | review standalone arithmetic; connection to CSem remains open |
| F3 compiler model | `agent-a05464165befb7074` (recovery merged) | patched C11 model integrated after capped build and combined full gate; 1,670 independent fast arenas including 1,051 generated successes; Sail arena equality remains open |

F3 review found reference-derived long-double bytes supplied to the model.
Such runs cannot establish independent byte equality. That input is now
removed. Patch 0004 fixes compiler literals using exact integer rounding;
public libc `strtold` still needs correction in the library lane. Matching refusals likewise do not satisfy the F3 success criterion.

The lead now runs external `gpt-6.1-sol` workers via `codexwork.sh`, at most
two at a time, following MANAGER.md. Slot 1 repairs the newly confirmed C11
trigraph deviation in tcc/config/model; slot 2 is next for the queued upstream
repin evaluation. The lead integrates the validated compiler-handoff collector
and hardens its model_diff consumer. The RV64 reader and image filesystem are
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
and the same eight residuals (results/pre/acceptance-contract.md). A newly
confirmed trigraph deviation needs a compiler patch and model reflection;
remaining C11 coverage is work, not a change to the contract.

Builds are serialized across these lanes, capped at 12 GB. The two inherited
`results/review/*.json` handoffs are retained as dated records; all 345 hashes
in `phase2-snapshot-current.json` match the committed Phase 2 result files.
Parser recovery builds in main and the repository gate passes. Approved
GNU forms, K&R definitions and connection to F2/raw-source CSem remain open.
Inherited hard-linked Lake caches have been isolated for active lanes;
main's CSem objects were rebuilt before accepting the digest retirement.

## Needs user

None currently. Implementation defects and unfinished obligations are work
items, not requests to weaken the target.
