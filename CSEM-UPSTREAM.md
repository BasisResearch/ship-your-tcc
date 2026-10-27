# Cerberus / cerberus-lean follow-up for Kiran

Maintained from 2026-10-27 following Mike Dodds's message. Kiran owns external
communication; no report has been sent by this project. These entries concern
the pin in `vendor/CSEM_REV`, not necessarily current upstream. Reproduce on
current upstream during the queued repin evaluation before calling an item an
upstream bug. Keep fixed, superseded and rejected entries with their evidence.

## Bugs and limitations to triage

| ID | observed at our pin | evidence / reproducer | status and next action |
|---|---|---|---|
| CSEM-01 | Partial frontend/Core parsers and native execution seams remain in the semantic dependency cone | `results/csem/f1b-seams.md`; `scripts/csem_cone.lean` | Trust/totality limitation, not a demonstrated wrong result. Compare with upstream's new totalization work; inventory does not inspect `implemented_by` bodies. |
| CSEM-02 | Menhir rejects several GNU forms and K&R declaration lists in our compiler corpus | `results/csem/f1a-parser.md`, `results/csem/f1a-oracle.tsv`, `csem/GRAMMAR.md` | Compatibility gaps; minimize each retained row and distinguish intended upstream scope from a bug before reporting. Joint rejection is not a correctness result. |
| CSEM-04 | Pinned POSIX unistd.h comments out link although a Core proxy exists; its result mapping also needs loaded-integer treatment | `results/repro/csem_link_header.c`; `results/csem/f1-filesystem.md` | Pristine runtime fails desugaring; corrected image runtime returns Defined(0). Recheck current upstream before Kiran reports. |
| CSEM-03 | Three proof groups require adaptation from Lean 4.32.2 to 4.34.0 | Exact sites and failing tactic behavior in `csem/PORT-4.34.md` | Toolchain compatibility, not semantic failure. Check whether upstream already has equivalent fixes. |

## Fork patches that may be useful upstream

| ID | candidate | evidence and state |
|---|---|---|
| PATCH-01 | Total Lean MD5 and explicit per-translation-unit `Digest` reader, replacing native MD5/global state | Landed: `f6c474d`, `b1f4cbb`; `results/csem/f1b-seams.md`: 18 vectors, cross-TU identity, 19 unit executables and 113 oracle classifications. A seam retirement, not a claim that upstream MD5 returns incorrect hashes. |
| PATCH-02 | Lean 4.34 proof adaptations without raised budgets | Landed; `csem/PORT-4.34.md` identifies source changes and rationale. Offer separately from our repository layout changes. |
| PATCH-03 | Explicit implementation-configuration reader for RV64I/lp64 | Integrated after review, capped build and full gate; `results/csem/f1-config.md`. Compare upstream configuration abstractions before proposing. Floating execution and bitfields remain open. |
| PATCH-04 | Pure image filesystem with descriptor-based `fstat` | Integrated after full fork gate and eight exact Sail probes; descriptor fstat, state ordering and link proxy corrections in results/csem/f1-filesystem.md. Image-specific choices need separation before proposing upstream. |
| PATCH-05 | Lean C parser and fail-closed Cabs comparison harness | Landed executable parser: 614 exact agreements / 664 inputs; remaining forms and F2 integration open. This is our new frontend, not an upstream parser correctness claim. |

The tcc C11 deviations (patch 0003), newlib binary128 conversion defect
(patch 0004 / F7), our F2 UCN and `_Pragma` repairs, and our model cache-key bug
belong to other components. Do not report them as cerberus-lean bugs.

## Scheduled repin evaluation

Owner: lead; task: `tasks/csem-repin.md`. Queue condition: after the current
Sol slot 1 C11-model task and slot 2 RV64-configuration task have landed through
the gate, before assigning another CSem semantic-change task. Use a separate
worktree and an available slot (maximum two); do not change either active
worktree or the main pin while they are in flight. Evaluate then-current
upstream, replay our fork changes, and record exact commits, absorbed patches,
remaining bugs, gate results and rollback point. This is a repository task
schedule, not an external notification or an automatic unreviewed pin update.

Started 2026-10-27 after both prerequisite integrations: external Sol slot 2,
branch sol/csem-repin. The main pin remains unchanged pending the report and
review. Slot 1's trigraph repair is independent of this CSem evaluation.
