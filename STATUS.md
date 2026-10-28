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

The lead runs external `gpt-6.1-sol` workers via `codexwork.sh`, at most two
at a time, following MANAGER.md. Slot 1 now handles per-member GNU bitfield attributes;
slot 2 retains complete multi-translation-unit ghost records. The reviewed
C11 bitfield and GNU expression checkpoints are awaiting combined adoption. Assertion-message encodings are adopted. Local replay erasure is proved
and adopted. Array-to-pointer alignment decay is adopted after the combined image/gate validation. Complete _Generic/_Static_assert syntax is adopted after its combined gate
and independent raw/native checks. Binding meaning and actual model-owned compiler-injected
opening authority are adopted after their combined gate and independent raw
comparison below. Physical-origin maps remain adopted with separate evidence.
The earlier anonymous-member, IEEE, GNU syntax/layout, allocation and typedef
alignment checkpoints remain adopted with their separately pinned evidence.
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
At that earlier checkpoint, ghost GNode was not yet typed Cabs. Current
compiler-action Cabs coverage is recorded in the later integrations below;
fuel adequacy, compiler/model proofs and all-corpus Sail equality remain open. Older pre-C11 counts used a defective cache
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
Typed ghost Cabs is adopted as an incomplete instrumentation checkpoint;
ordinary source mapping and complete construction remain required.
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
and 5,580 semantic consumer checks. The two pointer failures were subsequently diagnosed as a local zero-symbol
collision and repaired in the adoption below; their original captures remain.
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
F3 typed ghost checkpoint (2026-10-28): modeled compiler actions now retain
typed Cabs fragments and explicit gaps. Combined full gate, 44 required exact
shared-token controls and seven finite ghost-on/off checks pass. The broad
run retains 1,345/1,815 successes and its stated residuals. Fresh model runs
match three retained complete Sail arenas; these are not new Sail executions.
Three ordinary strict controls still fail on model macro provenance and actual
injected-header syntax. Slot 2 continues those model constructions. All five
erasure/agreement/total-construction Statements remain unproved; no refusal
escape or Supported restriction is added. See results/model/ghost-cabs.md.

CSEM-10 adoption (2026-10-28): both Lean C-translation entries now reserve
symbol zero, preventing a valid function pointer from becoming NULL on storage.
The combined full CSem gate passes with 2,304 unchanged tracked input hashes;
24 library-identity cases and 5,580 consumer checks pass. Thirty common
redeclaration rows pass, with one historical proxy gap and inherited CSEM-11
constraint acceptance kept separate. The 28 cross-compiled Sail program pairs
and one distinct full tcc-run observation remain pinned evidence. See
results/csem/function-redeclaration.md. Slot 1 now implements GNU attributes;
CSEM-11's general constraint repair and concrete bitfields are queued. Function
address/supply invariants and all whole-compiler proofs remain open.

Main checkout refresh (2026-10-28): the explicit compiler/ghost/preprocessor
and CSem executables were rebuilt under the shared 12 GB lock, then checked
on the repaired pointer case, one compiler arena and one exact typed fixture.
The older pinned binaries were hash-verified in a recovery archive before
replacement. This adds no Sail coverage or theorem; operational evidence is
results/review/main-artifact-refresh-2026-10-28/.

Current review (2026-10-28): slot 1 retains GNU attributes and injected asm
bindings in the independent parser; aggregate alignment now has its own tag
fields, separate from member alignment. Slot 2 extends model-owned macro
origins and ordinary typed construction. Both candidates remain under review,
with failed build attempts retained; neither changes the adopted counts above.
Exact carrier agreement, include provenance and combined validation remain.
The injected builtin aliases need an explicit runtime binding connection
(tasks/f1-injected-bindings.md), not removal of header declarations or a
Supported restriction. A fresh RV64 comparison also isolates lost typedef
alignment in tcc's `_Alignof(T)` queries; physical layout is correct.
tasks/f3-typedef-alignment.md queues the compiler/model repair. The independent
GNU expression-alignof difference remains separate. Full `tcc -run` on Sail
reproduces the query error after 13,481,097 compile steps; the two cross-program
runs and this full-image run are separately pinned in
results/review/gnu-typedef-alignment-2026-10-28.md. Static gate passes.

Macro redefinition provenance (2026-10-28): the spec now installs the latest
permitted definition even when replacement tokens match. Independent probes
repair stale runtime-library membership in both directions and repeated-include
ancestry. The full gate passes with 2,297 unchanged input hashes, 12 raw methods,
21 token methods and 24 library checks/5,580 consumer probes. Before/after
captures and the failed initial build command are retained in
results/pre/macro-redefinition-2026-10-28.md. The model-origin checkpoint
291bbb2 is committed for combined review; its main adoption is still pending.


Model-origin adoption (2026-10-28): the combined full gate passes with 2,722
unchanged source pins, 17 exact origin/opening controls plus three omission
guards, 44 exact shared-token Cabs controls, seven finite ghost-on/off checks
and 1,345/1,815 matching fast arenas. The 463 refusals and seven assembly gaps
remain separate; no other broad errors occur. Three fresh model comparisons
match retained complete Sail captures; no new Sail runs are claimed. Strict
ordinary model/spec Cabs remains 0/3 because the independent GNU header parser
is not yet adopted. Trigraph/splice physical maps and all six typed proof
Statements remain open. Failed setup runs and final exact captures are in
results/model/ghost-origins-integration-2026-10-28.md. Slot 1 has handed off GNU
attributes/layout and proceeds to injected bindings; slot 2 repairs
compiler/model typedef alignment. Sum cuts
remain frozen. The physical shadow-map follow-up is queued as
tasks/f3-physical-origin-maps.md; no third worker is running. The old parser
captures were archived only after every member
was hash-verified; the storage record is results/review/parser-ghost-capture-archive-2026-10-28.json.

GNU review follow-up: accepting located asm-label carriers makes the legacy
Supported visitor's parser-rejection assumption stale. The binding task must
explicitly exclude user asm labels while admitting only genuine injected
bindings through resolved identity/content, with actual runtime meaning.
Blanket exclusion would make ordinary raw Supported vacuous; blanket inclusion
would admit DESIGN section 5 constructs. This is required implementation work
in tasks/f1-injected-bindings.md, not a request to change the user-approved scope.

GNU handoff (2026-10-28): the external Sol worker hit a model-capacity error
after its full gate passed. Lead packaging verifies all 2,190 source pins and
746 capture members; checkpoint 44dc780 is retained. Its 23 focused semantic
agreements, six actual-header parses and concrete oracle results are recorded
in results/csem/gnu-attributes.md on the review branch. The combined main/origin/
GNU gate is running with 3,573 frozen inputs in sol-c11-line. Slot 1 restarts
on tasks/f1-injected-bindings.md from that reviewed combination; slot 2 still
owns typedef alignment. GNU adoption and ordinary raw semantic support are
not claimed yet. No model substitution or new user decision was needed.

GNU adoption (2026-10-28): combined full gate passes with 3,573 unchanged
source pins. Twenty-three semantic/GCC/full-text-Cabs controls and six actual
header parses pass; ordinary model/spec Cabs is now 3/3 exact and shared-token
Cabs is 45/56. Seventeen origin controls, three omission guards and both
seven-case byte regressions pass. Eleven Cabs nonsuccesses, two physical-map
gaps and semantic GNU residuals remain explicit. The gate now rebuilds the
standalone text parser and runs GNU/header checks; its earlier stale-parser
failure is preserved. Evidence is results/csem/gnu-integration-2026-10-28.md.
Slot 1's injected-binding meaning and Supported visitor connection are still
required; slot 2 continues compiler/model typedef alignment. This adoption
closes neither whole-C coverage nor any of the six ghost proof Statements.
No new native/broad observation or sum regeneration is claimed.

Typedef alignment adoption (2026-10-28): the combined full gate passes with
3,473 frozen source hashes and 104 unchanged supplementary shared Lem inputs.
Three full compiler Sail probes now succeed; two new complete 4 MiB native
arenas match both worker and combined models. The frozen historical corpus
retains 1,345/1,815 successes, 463 refusals and seven assembly gaps. Fresh
combined controls give 45/56 shared-token and 46/56 raw Cabs agreements,
17 exact origins, seven erasure and seven independent arena passes. The new
TypeAlignmentRepr_Statement remains open under F6. See
results/model/typedef-integration-2026-10-28.md. GNU CSem propagation/constraints
are queued in tasks/f1-gnu-alignment-semantics.md; no scope restriction is added.
Both workers had capacity interruptions, with source and failed receipts
preserved. Slot 1 resumed injected bindings after its passing Lean build and
isolated oracle build; it remains unadopted. Slot 2 next takes physical-origin
maps. Sum cuts remain frozen and no user decision is pending.

Array-constraint triage (2026-10-28): two minimal plain C11 array typedefs
are accepted by pristine pinned Cerberus and current checked Lean; direct
function arrays crash both. GCC/patched tcc reject the invalid inputs. CSEM-13
records this inherited defect with ten cases and 173 verified captures;
CSEM-12's GNU propagation issues remain separate. Both Sol slots are active:
injected bindings and physical-origin maps. No third worker or external report.

Generator repair (2026-10-28): unary minus now parenthesizes its operand,
avoiding accidental decrement tokens before negative floating literals. All
1,200 newly generated sources pass GCC syntax checking and independent arena
comparison with zero refusals/errors. The full gate passes with 3,577 frozen
inputs; its missing-cache-link first attempt is retained. Historical 1,815
sources and 463 refusal diagnostics are separately archived, and the historical
1,345-success count is unchanged. See results/model/cgen-negation-2026-10-28.md.
Main executables and base image were refreshed under the cap; no sum cuts moved.
The raw Cabs mode compares parser construction on model-produced positioned
tokens; it does not establish independent raw preprocessing. The queued
binding-origin task requires an additional independent Raw.checked cfgTcc
comparison. Intended multi-file corpus invocations are also queued. Both
workers remain active; their next combined reviews are still required.

Physical-origin adoption (2026-10-28): the combined full gate passes with
3,580 unchanged source pins. All 58 independent origins and four omission
guards pass; the two finite physical-map regressions are retired. Shared Cabs
remains 45/56 and the model-token parser component 46/56, with gaps explicit.
Fifteen ghost on/off pairs and fifteen independent fast reference arenas pass.
Three new typed physical-transform/view obligations remain unproved alongside
the six existing ghost obligations. Evidence:
results/model/physical-integration-2026-10-28.md. No new native Sail or broad
coverage is claimed, and sum cuts stay frozen. Slot 1 now owns CSem alignment/
array constraints; slot 2 owns injected-opening authority and independent raw
frontend comparison. The reviewed binding checkpoint is not yet adopted.

Explicit corpus invocation checkpoint (2026-10-28): the upstream 104 inline
pair now has a pinned two-translation-unit manifest. The combined full gate
passes (3,583 inputs); one successful arena matches the fast reference and
all 4 MiB on native Sail after 11,459,979 compiler steps. Both standalone files
still refuse independently, and historical broad counts remain unchanged.
Complete per-unit ghost construction/opening provenance remains open; the
manifest does not establish that connection. Other upstream executable-link,
TLS, pthread and snippet-driver invocations remain classified duties. Evidence:
results/model/corpus-invocations.md. No sum cuts are regenerated.

Binding/authority adoption (2026-10-28): the combined full gate passes with
3,597 frozen inputs. Eighteen focused controls compare complete model Cabs,
origins and opening graphs with independent Raw.checked, with all 21 authentic
aliases retained and six perturbation guards. Independent raw coverage is
45/56, with eleven explicit nonsuccesses; shared/model-token components remain
45/56 and 46/56 exact. All 58 physical controls, fifteen erasure pairs and eighteen fast
arenas pass. One new native Sail arena for static/builtin memcpy shadowing
matches all 4 MiB after 3,455,952 compiler steps. The existing two-file inline
manifest still passes. See results/model/binding-integration-2026-10-28.md.
Four F1 environment/binding/runtime contracts and two F3 authority contracts
remain unproved. Five ordinary runtime bodies exist; fifteen targets and the
alloca intrinsic remain missing. Whole-C grammar, pack layout, multi-TU ghost
construction, fuel and binary refinement remain work. Kiran's patch/bug list
is updated without external contact. Slot 1 owns alignment/array constraints;
slot 2 owns C11 generic/static-assert syntax. Sum cuts remain frozen.

CSem alignment/array adoption (2026-10-28): the combined full gate passes with
3,640 frozen inputs. The 71-case matrix retains 53 defined agreements,
12 constraint diagnostics, two function-policy gaps, three parser-placement
limits and one compiler array-decay discrepancy. All71 raw/text Cabs and
68 Menhir-accepted Cabs/digests agree. Fresh combined independent raw remains
45/56, with18 authority/six guards,58 physical/four guards,15 erasure pairs,
18 independent fast arenas and the explicit inline pair passing. Rebuilt C11
host/cross checks pass181positive22negative. The first postcheck schema error
and stale host binary are preserved separately from accepted results. See
results/model/alignment-integration-2026-10-28.md and the worker's distinct
full compiler/cross-GCC Sail observations. Universal alignment/transport and
whole-C obligations remain open; no new native/broad lead run or sum cuts.
Slot1 now works on bitfields. Slot2 repairs the compiler/model array decay in
tasks/f3-array-alignment-decay.md; generic/static-assert syntax is reviewed
and awaits the next combined gate. Wide assertion messages are queued in
tasks/f3-static-assert-messages.md. No scope restriction or user decision.

C11 generic/static-assert adoption (2026-10-28): the combined full gate passes
with 3,643 frozen inputs, including the new GNU expression-alignment Cabs
interface. All 21 focused valid controls are exact in three distinct scopes;
12 invalid diagnostics and one valid wide-message refusal remain separate.
Original 56 counts advance to 47 shared, 48 model-token component and 47 genuinely
independent Raw.checked agreements. All 18 authority/six guards, 58 physical/
four guards, 15 erasure pairs, 18 fast arenas and the inline pair pass. A fresh
complete native Sail batch matches all 4 MiB at 3,487,048 steps; a separate full
compiler/program run returns zero (126 program steps). See
results/model/unevaluated-integration-2026-10-28.md. The new
Ghost.AssociationReplayErasure_Statement brings open ghost contracts to 12;
finite observations prove none of them. Its proof task is queued separately.
Slot 1 continues bitfields; slot 2 repairs array decay. Wide assertion messages,
remaining grammar, pack layout, runtime bodies, multi-TU, fuel and binary
refinement remain open. Historical broad counts and frozen sum cuts are unchanged.

Compound-assignment triage (2026-10-28): CSEM-14 is independently inherited
from pristine 704c4e69. A minimal lvalue-callback counter returns 2 in both
semantic engines versus 1 in GCC/patched tcc; three related defined C inputs
are spuriously UB035, two value checks give wrong defined results, and simple
assignment is a successful control. The 140-file evidence is
results/csem/compound-lvalue-2026-10-28.md. Slot 1's bitfield work must repair
shared lvalue single-evaluation, not only new bitfield cases. No source-scope
restriction, native observation, changed pin or external contact is claimed.

Array-decay adoption (2026-10-28): the combined full gate passes with 3,646
frozen inputs. The compiler/model now clear outer array-alias alignment only
when deriving a pointer, preserving referent and pointer-alias metadata. C11
checks pass 210 positives and 27 negatives; 29 new fast arenas succeed with
five separate constraint refusals. The 71-case semantic matrix now has 54
defined agreements, 12 diagnostics, two policy and three placement gaps. All
legacy independent provenance/erasure/arena controls pass. Three dedicated
native cases plus the inherited generic batch each match all 4 MiB and execute
to zero on the rebuilt image. Their distinct step counts and 52 retained arena
pairs are in results/model/array-integration-2026-10-28.md. The setup race and
serialized retry are preserved honestly. TypeAlignmentRepr and all twelve
ghost contracts remain open here; the local replay-erasure proof is in review.
Slot 1 continues bitfields/shared CSEM14 lowering; slot 2 proves replay erasure.
No scope reduction, broad-count pooling, sum regeneration or user decision.

Local replay-erasure adoption (2026-10-28): Ghost.associationReplayErasure
proves the unchanged AssociationReplayErasure_Statement for arbitrary parser
interface, saved words and state, including diagnostics, without added
premises. The combined full CSem gate passes with 3,646 frozen inputs; the
only changed proof/audit sources are GhostDriver and GhostAudit. Both new
laws use only the three allowed standard axioms. See
results/model/replay-integration-2026-10-28.md and its separate worker evidence.
Eleven ghost contracts remain open: this closes local snapshot restoration,
not saved-word coverage, complete construction, whole-run erasure or binary
refinement. Runtime semantics, Supported/Fits, historical corpus totals and
frozen sum cuts are unchanged. Slot 1 continues bitfields/shared CSEM-14;
slot 2 repairs wide assertion messages. A separate queued task,
tasks/f3-bitfield-constraints.md, covers four silently accepted invalid C11
bitfield forms; their native discrepancy captures await F1 review/adoption.

Assertion-message adoption (2026-10-28): all five C11 literal encodings and
compatible adjacent strings now pass in the patched compiler/model. There are
65 complete Cabs controls in each of three comparison scopes,
64 new successful fast arenas and 61 separate diagnostic refusals. The reviewed
worker full CSem gate passes 3,379 inputs, all identical in the combined tree;
269 prior-main inputs are unchanged. The fresh combined default gate passes
3,648 pins. One additional complete native 4 MiB assertion arena and separate
full program run pass on the rebuilt integration image; the worker's five
native observations remain separately pinned. See
results/model/assertion-messages-integration-2026-10-28.md. The valid wide-message
residual is retired; original refusal evidence is retained. The generic suite
now has 22 valid controls, 11 invalid controls and one no-message extension.
Eleven ghost proof contracts and F1–F3 completion remain open; sum cuts stay
frozen. Slot 2 handles GNU expression syntax. Slot 1 continues bitfields/shared
compound lowering, including the review-discovered character-copy defect;
GNU trailing bitfield attributes are queued in tasks/f1-bitfield-attributes.md.

Review checkpoint, 2026-10-28: C11 bitfields and the shared CSEM-14 compound
lvalue repair are committed for integration. The corrected combined gate has
passed all 82 required semantic cases and is running native controls. Its first
failure was an old csem_raw executable consumed before rebuild; the gate ordering
is repaired and the failed receipt is retained. Independent combined layout,
control and 58-case continuation checks pass. A fresh 80-case full compiler/program
Sail batch matches GCC on every output row. Adoption is pending the completed
full gate and portable archive verification; no whole-proof completion is claimed.
GNU conditional/typeof/choose construction checkpoint 7e5f0c7 passes its full
worker gate (3,381 frozen inputs); the lead independently verified all 15,217
archive members, including six complete native arena exports. Its main adoption
also awaits combination. Slots now handle per-member bitfield attributes and
ordered per-TU ghost records. Enum bitfields and four compiler bitfield diagnostic
deviations remain separate follow-ups. No sum cuts have been regenerated.

## Needs user

None currently. Implementation defects and unfinished obligations are work
items, not requests to weaken the target.
