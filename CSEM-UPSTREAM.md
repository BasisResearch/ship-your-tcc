# Cerberus / cerberus-lean follow-up for Kiran

Maintained from 2026-10-27 following Mike Dodds's message. Kiran owns external
communication; no report has been sent by this project. The initial entries concern
our e7e9bb4 / 2d3a492 import; the current-upstream checks below concern the
adopted pin in `vendor/CSEM_REV`. The scheduled repin evaluation is complete;
distinguish its pristine-upstream captures from the locally repaired fork
before calling an item an upstream bug. Keep fixed, superseded and rejected entries with their evidence.

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

The tcc C11 deviations (patch 0003), compiler literal-rounding repair
(patch 0004), remaining newlib binary128 library defects (F7), our F2 UCN
and `_Pragma` repairs, and our model cache-key bug
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

Lead adoption, 2026-10-27: the selected-member repair and the rebased integrated
full gate pass, so main now takes 704c4e69 / e6d8db56 on Lean 4.34. The initial
defer decision remains above as dated history. The repaired union refusal no
longer blocks adoption. General aggregate expressions, anonymous-union layout,
IEEE and parser/totality work remain explicit. The valid-C tcc continuation
counterexample is owned by tasks/f3-designator-continuation.md. No external
message was sent; Kiran retains the bug-list handoff.


### Continuation review: separate layout gaps

The tcc continuation repair now passes 144 positive/14 invalid checks, 58
Sail/GCC program comparisons, 62 focused and 1,306 broad independent model
arenas. Its three full compiler Sail runs include all 33 conforming union
repair cases. See `results/model/designator-continuation.md`; the final integrated gate
with the adopted CSem pin passes.

**CSEM-07 — anonymous member continuation/layout.** The complete generator
case `designator_anonymous_static.c` initializes an anonymous union containing
an anonymous struct with `.a[0]=1,2,3,4`. GCC and patched tcc return zero;
pristine current OCaml and the repaired OCaml/Lean pair return one. Both
storage variants fail locally. This is separate from the selected-member
repair. Further complete sources/captures are in
`results/model/designator/layout-minimized/`: a scalar `.a=1,2,3`
initializer leaves `s.z` at zero upstream (GCC: three), while explicit
`.a=1,.b=2,.z=3` returns three in both. Without initialization, the anonymous
aggregate has size 20/offset 16 upstream versus size 16/offset 12 on GCC;
these layout mismatches are recorded separately from the wrong initialized value.

**CSEM-08 — bitfield coverage limitation.** The `padding` and
`trailing_padding` cases, in both storage variants, are refused during CSem
desugaring with an explicit unsupported-bitfields diagnostic. Even the latter
has only an unnamed padding field (`struct A {int x; unsigned :3;}`). This is
an acknowledged coverage gap, not a claim that upstream silently computes
incorrect bitfield values; it remains required by our whole-compiler target.

`results/model/designator/pristine-layout-gaps.json` contains complete sources,
pristine pin/binary hash and separate raw captures for these checks.
`designator/csem.json` records the corresponding local repaired-engine results:
52 exact defined agreements / 58, six semantic gaps. No external report sent.

2026-10-27 local IEEE recovery (not sent upstream): native floating execution
is replaced in this fork by configured pure 32/64/128 interchange values.
Shared Lem AST/translation/formatting sources now retain real formats through
casts, arguments, arithmetic and printf. New floating primitives have pure
Lean target interpretations; the existing native OCaml oracle remains a
baseline testing oracle, not the RV64 floating specification. The recovered
NaN-conversion policy was corrected from fresh raw Sail evidence. The helper
proof connection, conversion-field proof, literal-accuracy discrepancy and
nine finite newlib sqrtl discrepancies remain explicit in PHASES.md and
`results/csem/ieee-2026-10-27.md`; there is no all-C correctness claim.

### Anonymous containers and explicit union layout tables, 2026-10-27

**CSEM-07 / PATCH-08.** The reviewed local repair preserves physical anonymous
aggregate containers and resolves promoted names through qualified member
paths. This fixes the scalar continuation and size/offset counterexamples
above. Source lookup rejects guessed physical container names; trusted
initializer projections use explicitly marked internal paths. The worker
checkpoint passes 78 defined probes and 14 invalid controls on strict GCC,
patched OCaml and Lean, plus the unchanged parser/execution baselines.
`results/csem/anonymous-members.md` records exact sources and pin-addressed
observations. Combined validation with the adopted IEEE semantics now passes: 235-file
regeneration, the full gate, all 92 focused checks and 54/58 continuation
agreements with four explicit bitfield gaps. The repair is adopted locally.

**CSEM-09 / PATCH-09 — union layout consults an unset ambient table.** Both
union arms of sizeof/alignment in upstream `memory/concrete/impl_mem.ml`
consult `Tags.tagDefs ()` despite receiving the complete `tagDefs` argument.
Elaboration-time offsetof involving a union can therefore crash before the
runtime table exists. The four shared anonymous-member sources exposed this
independent adapter defect. `before-layout-adapter.json` in the anonymous
report directory retains the failure. `patches/csem/0001-union-layout-tags.patch`
changes both OCaml lookups to the explicit argument; `CerbMem.lean` has the
same repair. The existing Lean fuel-stability proof is adapted to that table
without larger bounds or new hypotheses. This is a small upstream candidate
separate from the anonymous-container representation change.

CSEM-06 aggregate-expression initialization and CSEM-08 bitfields remain open.
The separate tcc anonymous-container qualifier defect is repaired in our
compiler/model; its complete 78-case batch now passes full Sail
(`results/model/anonymous-members.md`). The older CSem worker's four compiler
counterexamples remain historical captures. No external message has been
sent; Kiran retains the report handoff.

2026-10-28 validation follow-up: the adopted IEEE/filesystem repairs now
match fresh full tcc/Sail runs on the original binary128-precision,
binary32-conversion and stdin-EOF probes. See
results/csem/config-followup-2026-10-28.md for all three captures and pins.
This updates local repair evidence; it is not a new pristine-upstream check.

**PATCH-10 — explicit runtime allocation resolutions (2026-10-28).** The local
fork keeps the bump runner as its default and adds checked finite RV64
placement functions, generic placement contracts, invalid-resolution
classification and lifetime byte clearing on address reuse. The semantic
outcome set quantifies these functions; finite CLI replays are only tests.
The combined gate and focused evidence are in results/csem/allocation-oracle.md.
This is a compiler-verification interface extension, not a claim that the
upstream deterministic allocator violates its documented baseline. Frontend
mini-runs and machine-memory simulation remain separate obligations. No report
has been sent externally.
**CSEM-10 — local Lean fork: redeclaration loses a function pointer.** The valid
neutral reproducer is `static int mine(int); static int mine(int x){return
x+10;} static int mine(int); int main(void){int (*f)(int)=mine;return
f(7)!=17;}`. The identity candidate and an independently pinned adopted
anonymous-member integration Lean binary both report exactly
``ill-formed program: `null function pointer'``. The pristine pinned OCaml
engine and strict C11 GCC return zero on the identical source. A direct call
after those declarations, and a function pointer without the extra
redeclarations, both return zero in the candidate. This is a local Lean-fork
defect, not established as an upstream OCaml defect or a library-name rule.
The proxy-named internal-linkage control is retained separately. See
`results/csem/library-identity.md` and its pointer baseline artifacts; Kiran
can inspect the minimal neutral source before reporting. No Supported/Fits
restriction or semantic-success count retires it.

CSEM-10 repair, 2026-10-28: the first prototype retains symbol number zero
through valid redeclarations in the local zero-seeded Lean supply. The concrete
memory adapter serializes that symbol as address zero; reconstruction takes
the NULL arm before consulting the function map. Pinned OCaml has consumed
runtime symbols already (`mine` is 483 with the identical positioned input; 502 with the ordinary
builtin-injecting preprocessor); there is no redeclaration symbol loss.
Both local pipeline entries now share `CerberusFresh.initialSymbolSupply = 1`,
reserving NULL without changing translation, proxy filtering or memory encoding.
Focused comparisons pass on the neutral case and declaration-order/storage
matrix; independent historical proxying and a function/object constraint gap
remain separate. Full validation, exact diagnosis, original two residuals and
ledger suggestions are retained in `results/csem/function-redeclaration.md`.
This corrects a local Lean entry seam, not the pinned OCaml sources.

**CSEM-11 — inherited function/object redeclaration constraint accepted.** Minimal
strict-C11-invalid source: `int mine(int); int mine; int main(void){return 0;}`.
Pristine pinned Cerberus/cerberus-lean
`704c4e697845127bced66c93fa0763eb51527ca4` (Lem
`e6d8db5680962f49060257d6c2bcb13c284b910f`) and both local Lean paths accept
it with `Defined {value: "Specified(0)", stdout: "", stderr: "", blocked: "false"}`.
Strict GCC rejects it as `‘mine’ redeclared as different kind of symbol`;
patched cross-tcc rejects it as `incompatible types for redefinition of 'mine'`.
It violates the C11 6.7p4 same-scope/same-namespace compatible-type constraint.
This is independently inherited from the pinned OCaml frontend; it is separate
from local CSEM-10 and from three correctly rejected function constraints.
Exact source, positioned Cabs, compiler commands, streams and oracle executable
hash are retained under `results/csem/function-redeclaration/after/invalid-object*`
(large streams in `after/streams.tar.xz`) and `sail/invalid-object*`. Neither acceptance is counted as defined-C coverage;
Supported/Fits are unchanged. No report has been sent externally.
