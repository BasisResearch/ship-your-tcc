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
Mike Dodds / Oath. Repin evaluation is queued in tasks/csem-repin.md after
the two current Sol tasks land; it will not disrupt their work. Kiran owns
external bug reporting; no external contact is authorized or planned.

| lane | recovery source | review and next check |
|---|---|---|
| C11 patches | new `codex-c11` worktree; existing F2 reproducers | patch 0003 integrated: 7/7 full compiler Sail reproducers; combined 0003/0004 host/cross tests and main gate pass |
| F1 grammar | `agent-a31b97b8f1c090141` | integrated: main capped build and full comparison pass, 614 exact Cabs agreements / 664 inputs; 41 joint rejections, 9 preprocessing failures |
| F1 pure seams | `agent-a249bbaa5b1df80ec` | digest retirement integrated; main's full CSem gate passes: 19 unit executables, 113 baseline oracle results; remaining partial/native seams inventoried |
| F1 ABI/allocation | `agent-a4ab6bbd8c0be5325` | RV64 configuration reader integrated after capped build/full gate; 10/15 probes agree, float/bitfield/enum/stdin gaps remain; allocator recovery still open |
| F1 filesystem | `agent-ad868c434f5f27398` | integrated after full gate: descriptor fstat, structural state ordering and link proxy repaired; eight exact Sail byte/exit agreements, repeated with RV64 config; mkdir remains a separate missing-library gap |
| F1 IEEE | `agent-a45cb08cf87ce958c` | review standalone arithmetic; connection to CSem remains open |
| F3 compiler model | `agent-a05464165befb7074` (recovery merged) | patch 0004 validated on main; Sol slot 1 now mirrors patch 0003 and C11 arguments |

F3 review found reference-derived long-double bytes supplied to the model.
Such runs cannot establish independent byte equality. That input is now
removed. Patch 0004 fixes compiler literals using exact integer rounding;
public libc `strtold` still needs correction in the library lane. Matching refusals likewise do not satisfy the F3 success criterion.

The lead now runs external `gpt-6.1-sol` workers via `codexwork.sh`, at most
two at a time, following MANAGER.md. Slot 1 finishes the patched C11 model and mirrors the additional pragma-operand
repair; slot 2 makes fast arena collection stop at compiler handoff. The RV64
configuration reader is integrated; its fresh probe residuals are documented
in results/csem/f1-config.md. The lead reviews/builds worker results and
completes filesystem recovery and F2 checks.
Built-in agents remain stopped.

Before patch 0003, the binary128-fixed model gave 1,557 identical arenas /
2,079 inputs against the fast reference: 499 matching refusals, 23 explicit
unsupported-assembly/pragma errors, no mismatches. Original 401-input corpus:
400 identical arenas, one refusal. Generated successful arenas total 1,043;
none of these fast-runner counts satisfies the pending Sail criterion.
Review also found an unchecked objcopy failure in the runtime cache key;
these historical counts remain diagnostic until a fresh, correctly keyed rerun.
The 156-vector binary128 stress program also has an identical arena. The
inherited assembler drafts remain in `build/recovery/assembler/`.

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
