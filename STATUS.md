# Current status

Takeover in progress, 2026-10-21. The approved target is
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

| lane | recovery source | review and next check |
|---|---|---|
| C11 patches | new `codex-c11` worktree; existing F2 reproducers | patch and run each reproducer |
| F1 grammar | `agent-a31b97b8f1c090141` | review total parser and Cabs comparison; build and compare with Menhir |
| F1 pure seams | `agent-a249bbaa5b1df80ec` | digest retirement integrated; main's full CSem gate passes: 19 unit executables, 113 baseline oracle results; remaining partial/native seams inventoried |
| F1 ABI/allocation | `agent-a4ab6bbd8c0be5325` | review configuration/oracle; generated copies are incomplete, do not merge results blindly |
| F1 filesystem | `agent-ad868c434f5f27398` | review actual image path behavior and FS tests |
| F1 IEEE | `agent-a45cb08cf87ce958c` | review standalone arithmetic; connection to CSem remains open |
| F3 compiler model | `agent-a05464165befb7074` (recovery merged) | patch 0004 integrated: main gate, capped model build, 45 targeted arena comparisons and 156-vector stress arena pass; broader comparison running |

F3 review found reference-derived long-double bytes supplied to the model.
Such runs cannot establish independent byte equality. That input is now
removed. Patch 0004 fixes compiler literals using exact integer rounding;
public libc `strtold` still needs correction in the library lane. Matching refusals likewise do not satisfy the F3 success criterion.

The lead now runs external `gpt-6.1-sol` workers via `codexwork.sh`, at most
two at a time, following MANAGER.md. Both Sol workers have handed off their results: patch 0003 passes its
seven full-compiler Sail reproducers; the parser now has 614 exact Cabs
agreements, 41 joint rejections and nine preprocessing failures. Lead
review/integration is in progress. Built-in
agents have checkpointed and stopped. Parser recovery is integrated:
the recovered baseline had 375/664 agreements, before the token-dispatch fix.

F3 recovery includes 398 independent identical arenas out of 401 inputs
against the fast reference, two binary128 mismatches and one matching
refusal. This is not the Sail exit criterion. The inherited unfinished
assembler modules are preserved in `build/recovery/assembler/` outside the
model's import closure.

Builds are serialized across these lanes, capped at 12 GB. The two inherited
`results/review/*.json` handoffs are retained as dated records; all 345 hashes
in `phase2-snapshot-current.json` match the committed Phase 2 result files.
Parser recovery builds in main and the repository gate passes; its first
comparison is diagnostic, with grammar and comparator gaps still open.
Inherited hard-linked Lake caches have been isolated for active lanes;
main's CSem objects were rebuilt before accepting the digest retirement.

## Needs user

None currently. Implementation defects and unfinished obligations are work
items, not requests to weaken the target.
