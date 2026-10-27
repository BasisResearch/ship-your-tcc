# Anonymous struct/union members in CSem

CSEM-07 is repaired in this candidate: nested layout and initializer current
objects remain physical containers, while source names resolve through them.
The final frozen `scripts/check.sh --csem` passes. This is ready for lead
review, not a compiler/model refinement theorem or all-C11 coverage claim.
Supported, Fits, STATUS/PHASES, the selected-member union repair and frozen
sum compile cuts are unchanged. No push or main merge was performed.

## What changed

`desugar_struct_declaration` previously returned an anonymous declaration's
children directly to its parent's physical member list. That lost union
storage and initializer continuation boundaries. Now one physical member
retains the nested type; an internal attribute marks promotion. Printable
names preserve Core text parsing and are chosen outside the direct and
promoted declaration namespace. Existing type/layout operations see nested
structs and unions rather than extra consecutive fields.

`Ctype.member_paths` records containing edges, types, qualifiers and source
visibility. Total bounded iteration uses one plus the number of definitions;
a complete by-value member chain cannot repeat a tag. Typing lowers visible
`.`/`->` access to physical operations, evaluating the base once and combining
qualifiers along the path. `offsetof` sums ordinary per-edge offsets.
Initializer source lookup filters visibility, then consumes the resolved
physical path directly using the existing brace and union cursor machinery.
Selected-member subtree overrides and same-member accumulation are retained.

Physical names are **not source members**. Lead review found that the first
printable-name version accepted a guessed name: GCC diagnosed it, while both
local engines returned one (`hidden-before.c/json`). Public lookup now filters
visibility. Internally generated member projections carry an explicit,
unspellable IR annotation; trusted traversal does not re-enter source lookup.
Eight negative probes guess member, arrow, designator and `offsetof` names in
both storage modes. Ordinary declared names resembling internals remain valid.

The offset probes exposed an inherited adapter defect: union sizeof/alignment
consulted an unset ambient table despite receiving the complete explicit table.
`CerbMem.lean` and the isolated OCaml two-line patch now use that argument,
as the struct and recursive branches already do. Before-repair crashes remain
in `before-layout-adapter.json`. The existing `layout_stable_aux` fuel proof
is adapted to the explicit union table; wrapper statements, hypotheses and
numerical bounds are unchanged. No budget increase or new proof premise was
introduced.

## Final observations

| check | result |
|---|---|
| new paired GCC / same-source OCaml / Lean checks | **92/92**: 78 defined C11 probes, 14 invalid controls |
| Core text from each positive OCaml elaboration | **78/78** parses on Lean |
| existing union checks | **34/34**, retaining 33 C11 inputs plus the inherited excess-initializer control |
| existing continuation checks | **54/58** defined agreements; four explicit bitfield gaps |
| strict GCC positives / invalid controls | **78 return zero / 14 diagnosed** |
| patched tcc vs RV64 GCC and host GCC on Sail | **74/78**; four retained qualifier counterexamples; all object/image ISA checks pass |
| existing continuation tcc/GCC Sail comparisons | **58/58** |
| conforming union batch on tcc/GCC Sail | **33 cases**, exit zero |
| focused Cabs comparison | **93/93** exact agreements, including the complete batch |
| full Cabs corpus | **614 AGREE, 41 BOTH_REJECT, 9 CPP_FAIL / 664**, unchanged classifications |
| existing CSem execution baseline | **90 MATCH, 18 UB_MATCH, 5 CERB_SKIP / 113**, unchanged |
| final capped build and full gate | **pass**, matching frozen receipts; CSem build peak 4,583 MiB |
| regeneration / ordinary axioms | **234 byte-identical files**, all unit/purity/totality gates pass; theorem cones use only the standard three axioms |

The minimal scalar source returns `s.z = 3` on GCC, patched OCaml and Lean;
pristine returns zero. Explicit designators return three on both old and new
engines. The separate array layout now gives sizeof **16** and `z` offset
**12**, versus pristine **20/16**. `minimized-and-residuals.json` retains the
exact complete sources, independent expected values and separate captures.
The initialized-value counterexample needs no layout-dependent expectation.

Fresh full compiler native Sail observations (empty stdout, traced exits):

| source | compile / program / total steps | exit |
|---|---|---|
| original scalar continuation source | 3,321,247 / 90 / 3,321,362 | **3**, matching GCC |
| complete 78-positive batch | 7,444,688 / 2,958 / 7,447,671 | **59**, identifying the first qualifier counterexample; GCC gives zero |

These execute the compiler and generated program. `native.json`, retained
traces and source copies pin the ELF, embedding, runner and emulator. Runs
use traced 100-million-step limits and 900-second wall bounds. These are
native Sail observations, not kernel reachability proofs or arena-refinement
claims. The failed complete batch is not counted as successful validation.

## Pins, generation and provenance

Cerberus is `704c4e697845127bced66c93fa0763eb51527ca4`; Lem is
`e6d8db5680962f49060257d6c2bcb13c284b910f`; Lean remains **4.34.0**.
The isolated oracle overlays exactly the four shared Lem sources and applies
`patches/csem/0001-union-layout-tags.patch` to its own copy. Its content key
also pins the Lean adapter. Copied sources, the patched OCaml adapter and
binary are hash-checked. Pristine upstream, existing oracle directories and
shared opam are never modified. Attribution remains Cerberus/REMS/Oath plus
local changes under the surrounding BSD-2-Clause license; ATTRIBUTION.md
records this repair and harness reuse.

| source | SHA-256 |
|---|---|
| `csem/frontend/model/ctype.lem` | `88c303cd5f5ca2bc039a5531f3dfd4cb445a0086994999ae6146afa7f0d5c578` |
| `csem/frontend/model/cabs_to_ail.lem` | `09bd43a3c76cae4ad9fff141510573dada7c6d105c3f06b00f0679c3ee6c5092` |
| `csem/frontend/model/ail/genTyping.lem` | `bb3033b35d0f8a3a0d835922dd118c5c9d2be27530f89cc9de0fbf7ca9d5201f` |
| `csem/frontend/model/desugaring_init.lem` | `196326bd12e40e7573efd65fb0acb4a07a40b60599f0e7fb424be2063e1f65d2` |
| `csem/lean_frontend/CerbMem.lean` | `947af51f0415c6096683d9070b35be2ee6123fc5f61b09fba940cbf131479ce4` |
| `csem/lean_frontend/CerbMem_lemMeasureProofs.lean` | `f4bdafa5bfa402e5f3628199be494dad118aec2e807ca42b0429b2c505d98ef6` |
| `patches/csem/0001-union-layout-tags.patch` | `26c575e3aaeac9a525bef796840a89fe5877284f02d2188e2e24a975c8ec7843` |

Binary SHA-256 pins (absolute paths and commands are in `validation.json`):

- Patched OCaml: `797230dfc5e086cd222202ab5117e35696aab3cb687e7f6312e2070ab11a226f`.
- Generated Lean: `3d158e630cce61e8f9793e3325cba4c96df141275f196ebbb7e67d556a68f41d`.
- Pristine OCaml: `a71dd0968846b72196506a7ce78cd300cb39c6a8a8b10e87945a44c30517d091`.

`build-07-request/result`, `oracle-05-request/result` and
`gate-02-request/result` are the final matching manifests. Compressed logs
retain cap and verification evidence. Every build used the common root lock
and MemoryMax=12G after a >=25 GiB memory check. The sandbox's denied systemd
bus was serviced by the lead; no uncapped fallback was used.
`freeze_anonymous_validation.py` rejects source/binary drift, missing gate
success and changed classifications. The Lua inventory/tool table were read
before the proof adaptation (inventory hash in validation.json). No library
machine proof was copied or claimed instruction-identical.

This worktree's frozen base is `71c59030a00c932571bc23c18e6b5be74422f80c`.
Per lead instruction it was not rebased while dirty/frozen. Main's independent
IEEE adoption (`2f2dd9e`) must be preserved during lead integration, followed
by regeneration of the combined **235-file** tree and its combined gate.
The present 234-file result describes this exact branch, not that future tree.

## Remaining gaps and ledger text for the lead

- CSEM-07: physical anonymous containers, public qualified paths, layout,
  initialization and hidden-name rejection repaired; all 92 focused checks
  and the final gate pass. Finite observations do not prove compiler/model
  refinement or all-C11 coverage.
- CSEM-08: both storage variants of `padding` and `trailing_padding` still
  refuse with explicit unsupported-bitfield diagnostics. All four remain in
  the 58-case ledger; no Supported exclusion or bitfield relaxation was added.
- CSEM-06: `aggregate-value-first`, `aggregate-value` and `nested-union-value`
  still fail typing on both models, while GCC gives 7, 2 and 7. The override
  variant agrees with GCC at zero but Clang gives seven; that does not retire
  general aggregate-expression semantics. Whole-struct copy controls pass.
- Compiler follow-up: `const_pointer_type` and `volatile_pointer_type`, each
  in automatic/static storage, return one on tcc/Sail versus zero on GCC and
  both models. Named-container, direct-const-field and const-object controls
  pass on tcc. An invalid const-container assignment also lacks a diagnostic;
  const-object assignment does get a warning, which is a diagnostic. These
  exact sources and raw results are retained for the lead's compiler task.
- Parser joint rejections, preprocessing failures, unknown coverage and other
  whole-compiler obligations remain open. Matching refusals are never counted
  as successful C11 execution. STATUS/PHASES were not edited.

## Dead ends

Flattening or offset-only patches lose storage/current-object boundaries.
Unspellable physical field names break Core parsing; printable names alone
still leak unless source lookup filters visibility. Merely filtering all
lookup breaks trusted initializer paths; consume their physical edges directly.
The two adapter union lookups must use the explicit table, and the existing
fuel potential must follow that same table. `__builtin_offsetof` was a harness
parser mismatch: use `<stddef.h>`'s standard `offsetof`. Positive source-name
controls use legal identifiers; guessed private-name negatives deliberately
remain invalid. A wildcard for positive automatic/static fixtures also picked
up the new negative controls; the final Sail command enumerates positives
explicitly and retains all negative diagnostics separately.

Requests 02/03 were rejected for source drift and are superseded, not final
validation. Build 05 exposed the old fuel potential's ambient-table premise;
build 06 fixes it without changing bounds. Gate 01 passed its earlier matrix
but lacked guessed-name controls; gate 02 is the final public-lookup repair.
Wait for a matching completed receipt before changing frozen inputs, including
when an early target error is already visible. These dead ends are also in
`tasks/DEAD-ENDS.md`.


## Adopted combined validation

The reviewed checkpoint is rebased with the adopted IEEE semantics and raw
frontend (`9574f45`). All **235 generated Lean files** are regenerated through
the pinned generator. The isolated OCaml engine overlays the same four shared
anonymous-member sources plus the explicit-table adapter patch; it remains an
integer/layout oracle, not an RV64 floating specification. Its new content key
pins the combined Lean adapter, and copied sources/binaries are verified.

The combined full `scripts/check.sh --csem` passes, including **92/92** new
focused checks, **34/34** union checks, pure-IEEE and raw-frontend cone audits,
standard axioms and generator identity. A fresh continuation run gives
**54/58** agreements; the same four bitfield cases remain explicit gaps.
`anonymous-members/integration/` freezes 2,285 source/input hashes, logs,
current executable pins, paired outcomes and adapter/source manifests.
The original pre-IEEE branch's observations above remain historical evidence.

The rebuilt compiler `.text` is byte-identical to main's adopted qualifier
repair image. Its separate complete 78-case full Sail batch now passes
(results/model/anonymous-members.md, commit a727c78); the old failed batch
above is retained as a counterexample to the previous image. No new compiler
refinement, all-C correctness or kernel reachability proof is inferred.
CSEM-06 aggregate-expression initialization and CSEM-08 bitfields remain open.
