# GNU alignment semantics and C11 array constraints

This checkpoint repairs the shared CSem handling of declared typedef alignment
and early array constraints. The target remains C11 with the approved GNU
extensions, over whole-compiler Supported/Fits. It does not change the compiler,
its model, object size/layout, injected binding identities, or frozen sum cuts.
Finite executions below do not prove compiler correctness or universal transport.

The starting evidence is preserved unchanged in
[typedef-alignment.md](../model/typedef-alignment.md) and
[array-constraints-2026-10-28.md](array-constraints-2026-10-28.md). Both archives
and their complete independently hashed member maps were verified: 2,350 and
173 members respectively. The 26 original semantic inputs, three placement
probes and all ten array-matrix inputs are copied byte-for-byte into
`c/tests/csem-alignment/`. Attributes remain on every tested input.

## Repair and interface

* Shared `GenTyping` carries type alignment in typed expression annotations,
  independently of generic width/sign categories. It preserves aliases through
  dereference/returns, promotions that retain the type, common types, shifts,
  assignment, selected generic expressions and statement expressions. Casts,
  comparisons, logical operations and pointer derivation clear outer metadata;
  pointer referents retain their complete types. The two `translation_aux`
  type-query definitions consume the annotation. This changes alignment alone.
* `get_sigma_sofar` retains each external object's already stored alignment.
  Direct GNU identifier queries read that declaration field; type queries and
  member/address expressions never borrow an enclosing object's alignment.
* `CabsEalignof_expr` is distinct from C11 `CabsEalignof(type)`. Independent Lean
  and isolated Menhir adapters retain the unevaluated expression. The Supported
  visitor recursively visits it; it introduces no exclusion or refusal escape.
  Expression queries lower to existing Ail alignment operations after typing.
* Array validation runs before completeness queries and parameter adjustment,
  including unused typedefs, pointers to array types, members and type names.
  Function elements diagnose before `is_complete` can panic. Incomplete elements
  diagnose, and GNU alias element size must be divisible by its declared
  alignment. Ordinary function-pointer/integer arrays and unused incomplete
  extern arrays retain acceptance.

Lean is regenerated from shared Lem and handwritten adapters. The isolated
OCaml build takes the same shared typing/constraint/AST sources. Its generator
projects **only the two exact shared type-query definitions** into pristine
`translation_aux` after the adopted symbol-proxy patch. This avoids importing
unrelated Lean-only IEEE constructors; no floating operation is stubbed.
Every source, patch, projection generator and resulting overlay is pinned.

The independent Menhir extension preserves trailing array/function attributes.
Prefix/middle declaration placements that lack its attachment carrier remain
explicit frontend limitations; it refuses GNU effects rather than discarding
those attributes. Neither that refusal nor raw/text transport equality is a
semantic agreement or independent raw-preprocessing proof.

## Historical array matrix

The original ten rows and precise categories remain in their original report.
The following descriptions are historical, not renamed post-repair results:

| case | historical checked Lean | historical pristine OCaml |
|---|---|---|
| direct function array, file scope | panic | internal failure |
| direct function array, block scope | panic | internal failure |
| direct void array, file scope | elaboration error | constraint diagnostic |
| direct void array, block scope | elaboration error | UB059 classification |
| function-pointer array | defined zero | defined zero |
| integer array | defined zero | defined zero |
| incomplete unused extern array | defined zero | defined zero |
| GNU overaligned-size element | defined zero | parser rejection |
| array typedef of function typedef | defined zero | defined zero |
| array typedef of void typedef | defined zero | defined zero |

The two plain typedef acceptances and direct function-array failures are
independently inherited C11 defects (CSEM-13). GNU attribute placement failures
in historical Menhir do not establish inheritance of our GNU semantic defects.

## Validation

The focused maintained comparison has **71 cases**: 53 defined-zero engine
agreements, 12 separately counted constraint diagnostics, two function-policy
obstructions, three independent Menhir placement limitations and one separate
compiler decay discrepancy. Every case has exact raw/text Cabs agreement;
all 68 cases accepted by Menhir have exact whole Cabs/digest agreement with
it, with independent parsing and execution of the unstripped positioned input.
These are parser/component checks, not independent raw preprocessing proofs.

Current raw Lean gives defined zero on 24/26 original semantic inputs; the two
function-query errors remain distinct. In particular all three original
one-results and all five parser failures are repaired. The original ten-case
matrix now has seven constraint diagnostics and three defined-zero controls
in each repaired engine. Five additional nested/type-name/parameter array
violations also diagnose. Invalid inputs add no defined-C coverage.

Lean receipts `alignsem-lean-4` and `alignsem-source-freeze-5` pass with 3,367
verified source pins. Isolated `alignsem-oracle-5` passes on the same semantic
sources; its artifacts/overlay verify at key
`ee06b1562b7a991a3aacb8faac82698828228a6aa3290fa420e7c2276135f0b9`.
The full repository/CSem gate `alignsem-full-1` passes with the same 3,367
verified source pins. It runs GNU declarations/raw headers and the maintained
alignment comparison, library identity, declaration/redeclaration,
anonymous-member, injected-binding, generator drift, purity/totality and axiom
gates. Native floating remains absent from both measured execution cones;
existing frontend/seam and universal adequacy obligations remain open.

The extended native matrix has 59 valid GNU/ordinary controls and 12 invalid
array constraints. GCC returns zero on all 59; patched tcc returns zero on 58,
with the extra array-decay discrepancy retained separately. Both reject all
12 invalid constraints. Invalid inputs, function-policy errors and parser
limitations never contribute successful CSem comparison coverage.

The freshly rebuilt compiler C11 lane passes 181 positive and 22 negative
cases. Current host GCC and patched tcc return zero on all 26 original GNU
inputs, including the two function-query controls that CSem still diagnoses.

The three original defined-one discrepancies each now have fresh, independent
machine observations on their unchanged source bytes:

| source | full tcc-run Sail exit | compile / program / total steps | cross-GCC Sail exit / steps |
|---|---:|---:|---:|
| deref | 0 | 3,385,413 / 89 / 3,385,527 | 0 / 837 |
| unary-types | 0 | 3,399,722 / 89 / 3,399,836 | 0 / 845 |
| common-types | 0 | 3,393,431 / 89 / 3,393,545 | 0 / 841 |

These full tcc-run observations execute the compiler itself on Sail. The
cross-GCC rows execute cross-compiled programs. Neither is a fast-arena
comparison, and no counts from historical captures are pooled into them.

## Precise remaining obstructions

The original two function-query elaboration errors remain separate. C11 Ail
rejects alignment queries on a function type; the GNU target rule still needs
its explicit meaning/transport connection. Host GCC and tcc report alignment
1, while RV64 GCC reports 4 (the independently pinned historical raw values).
Neither policy is silently made C11 meaning, and the valid GNU compiler
controls are not counted as CSem successes.

The original explicitly aligned-member expression question also remains
separate: the retained host GCC output is `16 1 16 1 16 16`, versus patched
tcc's `16 1 16 1 16 1`. This checkpoint does not claim its GNU meaning link.

An additional, minimized compiler discrepancy concerns an array typedef with
alignment 32 and size 8. Both native engines report 32 for `typeof(a)`, 32 for
`__alignof__(a)` and 8 for `__alignof__(&a)`. For `typeof(a+0)`, GCC reports 8
but current patched tcc reports 32. On the exact same source, independent
cross-GCC Sail exits 8 and the full compiler on Sail exits 32. CSem's pointer
type must not inherit that array alignment. This is separate compiler/model
follow-up, not a reason to change CSem, reject the valid source, or narrow
Supported/Fits.

Existing open obligations include `Tcc.Model.TypeAlignmentRepr_Statement
imageBoundary`; `Tcc.Model.Ghost.{Erasure_Statement, RawErasure_Statement,
ParserAgreement_Statement, RawParserAgreement_Statement,
RawConstruction_Statement, NormalizationAgreement_Statement}`; the existing
`PhysicalOriginMap_Statement`/`PhysicalReadView_Statement`; and
`Tcc.CSem.Raw.{ParserConnection_Statement, LibraryConnection_Statement,
BindingResolution_Statement, RuntimeBindingCoverage_Statement,
PragmaConnection_Statement}`. The new expression constructor
and Ail type annotations need those universal connections too. F4/F5/F6 and
fuel adequacy remain open. The model's expression-query Cabs construction is
not extended here (the model/ghost lane is independently owned); these finite
spec executions do not supply its parser agreement. No new library or machine
proof is reused or asserted byte-identical. The Lua inventory and mandatory
table were consulted before work.

## Retained failures

`alignsem-lean-1` was submitted before its outstanding generator completed;
the generated reifier/stamp changed during its freeze. Its failed receipt is
not accepted build evidence. `alignsem-lean-2` verifies its inputs but rejects
mutual termination inference. Direct record-pattern structural variants are
also retained failed generator attempts; bottom-up shared typing eliminates
that reconstruction. No budgets were raised.

The explicit local capped oracle1 attempt fails at the denied systemd bus.
Oracle2 rejects duplicate symbol-proxy hunks; oracle3 rejects unrelated
Lean-only IEEE constructors from a wholesale module copy. Their failed keys,
logs and exact requests/receipts remain. Oracle4 then requires the handwritten
error renderer's exhaustive case for the new array-alignment constraint; its
failed receipt is preserved too. The maintained projection generator
supplies the shared semantic delta without mutating any shared cache.
The first raw repair run is retained: the three original semantic differences
are zero, but three global expression-query controls still return one because
the provisional declaration table dropped alignment. The final repair retains
the actual stored alignment there. These are recorded in `tasks/DEAD-ENDS.md`
so they are not reassigned as repair strategies.


[manifest.json](gnu-alignment-semantics/manifest.json) independently hashes
[observations.tar.gz](gnu-alignment-semantics/observations.tar.gz), every member,
and the standalone reports. [verify.py](gnu-alignment-semantics/verify.py)
checks all 6,746 current archive members, the 3,367-pin final source receipt, all 26 original
sources, the unchanged ten-case historical matrix, exact AST checks and the
separate Sail execution categories. Commands, raw stdout/stderr, complete
images/traces, actual executable/source pins, all failed requests/receipts,
Lua inventory and full-gate artifacts are retained. No shared cache or other
worktree was modified; no push/main merge or sum-cut regeneration occurred.
The checkpoint is for lead review. STATUS/PHASES adoption remains lead-owned.

The ordinary `git add` attempt is retained as `git-add.stderr`: this worker
has read-only Git metadata. Following MANAGER.md, the exact validated paths
and content hashes are handed to the lead for the review commit; no alternate
index/object-store workaround or push/main merge is used.
