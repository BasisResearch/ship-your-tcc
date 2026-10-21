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
| F1 pure seams | `agent-a249bbaa5b1df80ec` | review pure MD5 and per-TU digest parameter; regenerate and retain fork gates |
| F1 ABI/allocation | `agent-a4ab6bbd8c0be5325` | review configuration/oracle; generated copies are incomplete, do not merge results blindly |
| F1 filesystem | `agent-ad868c434f5f27398` | review actual image path behavior and FS tests |
| F1 IEEE | `agent-a45cb08cf87ce958c` | review standalone arithmetic; connection to CSem remains open |
| F3 compiler model | `agent-a05464165befb7074` (six unmerged commits plus draft changes) | build passes; audit comparison independence, then corpus and Sail |

F3 review found reference-derived long-double bytes supplied to the model.
Such runs cannot establish independent byte equality. Remove that input
from acceptance tests and investigate the reported newlib literal conversion
defect. Matching refusals likewise do not satisfy the F3 success criterion.

Builds are serialized across these lanes, capped at 12 GB. The two existing
untracked `results/review/*.json` files in main are preserved pending review.

## Needs user

None currently. Implementation defects and unfinished obligations are work
items, not requests to weaken the target.
