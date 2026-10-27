# Cerberus / cerberus-lean follow-up for Kiran

Maintained from 2026-10-27 following Mike Dodds's message. Kiran owns external
communication; no report has been sent by this project. The initial entries concern
our e7e9bb4 / 2d3a492 import; the current-upstream checks below concern the
repin candidate in `vendor/CSEM_REV`. Reproduce on
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

## Current-upstream checks, 2026-10-27

The lead-fetched `mdd/cerberus-lean` snapshot is
`704c4e697845127bced66c93fa0763eb51527ca4` (2026-10-26 17:26:39 UTC),
with lem-lean `mdd/lean-backend`
`e6d8db5680962f49060257d6c2bcb13c284b910f` (2026-10-22 01:33:30 UTC).
Its matching OCaml oracle was built in the repin worktree's isolated opam
switch. These checks are local; nothing has been sent upstream.

Small complete reproducers and raw old/current oracle captures are in
`results/csem/repin-2026-10-27/probes/` and `upstream-probes/`.

| entry | current-upstream disposition |
|---|---|
| CSEM-01 / PATCH-01 | Native MD5 and the per-TU mutable digest/barrier remain upstream. The local explicit digest reader was replayed and validated on the candidate. The new structured-stop protocol improves failure attribution, not kernel transparency. Native floats and partial parsers remain. |
| CSEM-02 | `gnu-cleanup.c` still fails parsing at `__attribute__`; `knr.c` still reports unsupported K&R declarations, on both old and current OCaml oracles. These remain scope gaps relative to our approved target. |
| CSEM-03 / PATCH-02 | Upstream now supplies typed `of_decide_eq_true` bridges at the Utils/Defacto sites, the extra contradiction simplifications in memory erasure, and zeta reduction in `toList_bal`. All three groups compile on 4.34; the old port patches are absorbed, with our existing reader parameters retained. |
| CSEM-04 | `link-header.c` still reports undeclared `link` with the current pristine runtime. Our image-runtime header/proxy correction remains necessary. |
| PATCH-03 / PATCH-04 / PATCH-05 | Explicit RV64 implementation parameters, image FS and Lean C grammar remain local work. Upstream still refuses filesystem operations rather than modelling this image. |
| CSEM-05 | **Pristine current upstream:** `union-second.c` remains refused (exit 125); 23/34 focused inputs are refused. **Local repair:** shared `desugaring_init.lem` carries selected member names through traversal, ordered subtree overrides and value construction. Patched OCaml and generated Lean match GCC on all 34 focused checks (33 C11 cases plus one inherited excess-initializer control). See `results/csem/union-init-2026-10-27.md`. Anonymous-member representation and aggregate-expression initialization remain separate gaps; no Supported restriction was added. |
| upstream arithmetic fix | `bool-float.c` changes from `Specified(0)` to the C11-required `Specified(1)` for `(_Bool)0.5`. `bool-pointer-call.c` remains `Specified(1)` in the default calling convention. |
| upstream overlap refusal | `memcpy-overlap.c` changes from a defined answer to an explicit refusal. The overlapping call has undefined C behaviour; a refusal is not a successful UB classification. |

The upstream axiom gate also fixes a previously vacuous check for modern
per-declaration native-decision axioms and applies an exact, name-independent
standard-axiom allowlist. Keep that strengthening when replaying our root-Lake
integration. The full repin decision and measured gates belong in
`results/csem/repin-2026-10-27.md`.

### Additional repin findings

* **PATCH-06, total stop encoding (local candidate).** Current upstream
  `CerbStop.render` uses generic `Json.compress`. A fresh cone traversal adds
  one kernel partial and two runtime partials (`Json.compress.go` and
  `JsonNumber.countDigits.loop`). The candidate shares one strings/nulls/arrays
  encoding description between `toJson` and total wire rendering, reusing
  Lean's total string escaping. Its cone returns to the fresh baseline
  43/100 partials. `Unit.StopTest` compares wire bytes with upstream's printer
  on seven samples including Unicode and every control byte; the existing
  cross-language codec/capture suite remains the gate. This is independent
  of the image FS/RV64 choices and is a useful upstream-sized patch.
* The repin exposed **our** stale `--libc` argument cache: old Cabs metadata
  named the old runtime prefix, which current upstream correctly refused.
  The wrapper now always asks a locked, content-addressed preparation helper
  keyed by the oracle, pins, source/Core bytes, host preprocessor and resolved
  runtime identity/content. Cached artifact hashes are checked. The exact
  runtime-location guard is unchanged. Do not report that cache defect as an
  upstream regression.

The final 2026-10-27 candidate passed the root full gate on Lean 4.34,
regenerated all 234 files with the matching generator, retained the 113-row
oracle baseline, and preserved the focused configuration/FS/parser results.
Both current OCaml (exit 125) and the repinned Lean (exit 134) refuse
`union-second.c`; the old OCaml oracle returns 7. Recommendation: retain the
active main pin until this valid C11 case is handled, rather than weakening
Supported or the theorem. The tested candidate remains available for lead
review; main was neither pushed nor merged by this worker.

Lead disposition: evaluation completed and candidate checkpoint
`1959dbfec1ee342afee03cec7b4a11de4bf24249` is preserved. Main keeps the old
pin until the valid-union regression is repaired by
`tasks/f1-union-initializers.md`; this requires implementation, not a change
to Supported or a user decision. Kiran's list is ready for him to relay.
No external message has been sent.

### CSEM-05 local semantic repair, 2026-10-27

PATCH-07 is the shared initializer change in `desugaring_init.lem`, generated
with the candidate's matching Lem pin. Non-first selection is implemented,
including same-member descendant accumulation, member switching, replacement
of braced aggregates, nested unions/structs/arrays, omitted descendants,
character strings and incomplete outer arrays. Nested list cursor ownership
and name-based path comparison are necessary parts of this repair.

The local OCaml engine is a separate content-addressed build that overlays
that exact shared source onto pristine upstream; it does not overwrite the
pristine oracle or replay unrelated Lean filesystem seams. Pristine outcomes
remain separately labelled in the report. The historical repin recommendation
above describes the unmodified candidate; adoption still requires the repaired
candidate's gates and lead review. Kiran owns external communication; nothing
has been sent upstream.

CSEM-06 (separate, freshly checked): general aggregate-expression initializer
handling remains incomplete. `residuals/aggregate-value-first.c` uses the first
union member and therefore never needs the retired non-first guard. Pristine
current OCaml, patched OCaml and Lean all reject `.s=p` with a typed scalar
initialization error; GCC returns 7. Nested union-valued initialization is
likewise rejected locally. With a subsequent `.s.a=3`, GCC/model return 0 but
Clang returns 7; that agreement with GCC does not establish correct aggregate
copy semantics. Exact probes and captures are in the union repair report.

The focused validation also found a separate **tcc**, not Cerberus, valid-C
positional-continuation refusal, minimized without a union. Both patched CSem
engines and strict GCC return 0; patched tcc/Sail refuses. The lead queued its
compiler/model repair separately and explicitly permits review/adoption of
this CSem candidate on its own semantics and gates. Keep the counterexample;
never narrow Supported or count the refusal as agreement.
