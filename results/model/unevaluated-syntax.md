# Complete compiler-action generic/static-assert syntax

The original `generic` and `static-assert` fixtures now construct complete
Cabs independently of `Raw.checked`. The model retains the controlling
expression, every association (selected and unselected), association type
syntax, assertion condition, every message literal and their original
locations/identities. No CSem grammar, C11 meaning, Supported/Fits exclusion,
compiler patch, runtime image or frozen sum cut is changed.

`skip_or_save_block` now retains an observation-only token string for skipped
associations when syntax capture is enabled. Its saved word offsets point to
actual model token occurrences, including the association's terminating
lookahead. Each association goes through the existing modeled expression
parser on an isolated state. Only typed fragments and syntax audits escape;
sections, symbols, value stack, identifiers, files and compiler diagnostics
are restored. A missing construction or an appended source occurrence is an
explicit construction gap. The real selected expression executes through the
original compiler path with duplicate syntax capture suppressed; the retained
input is restored on success and diagnostic exit.

Replay lookup precedes virtual-fixture EOF handling, and both physical and
shared-token runners maintain the word cursor. Identical token strings never
select a saved map by structural equality. Association type syntax is wrapped
before CType folding. Cabs associations retain the pinned grammar's reversed
list order without erasing syntax or normalizing locations.

`do_Static_assert` now wraps its actual condition/message actions into a typed
`Declaration_static_assert`. Existing declaration transport carries that node
to external declarations and block items; aggregate construction carries it
as `Struct_assert`, including nested structs/unions. Raw string tokens supply
literal spellings and locations, rather than the compiler's decoded diagnostic
message. No independent spec Cabs or returned origins construct model values.

## Validation

The three comparison scopes are distinct:
shared tokens test parser actions, model-token component tests the independent
grammar on model tokens, and independent raw calls `Raw.checked cfgTcc` on
source/image files. The latter compares full Cabs, raw spellings, recursive
origins, opening graph, all 21 injected bindings and exclusion metadata.

| Final source-pinned comparison | Observation |
|---|---|
| Focused complete syntax, each of the three scopes | 21/21 exact |
| Invalid focused inputs | 12 diagnostic observations; no successes |
| Valid wide-message compiler residual | 1 retained refusal; no success |
| Original 56 shared-token suite | 47 exact / 9 nonsuccesses |
| Original 56 model-token parser component | 48 exact / 8 nonsuccesses |
| Original 56 independent Raw.checked suite | 47 exact / 9 nonsuccesses |
| Injected authority / perturbation guards | 18 exact / 6 guards pass |
| Physical origins/openings/logical view / omission guards | 58 exact / 4 guards pass |
| Inherited ghost on/off | 15 identical arena/console/status/stdout/stderr pairs |
| Inherited independent fast arenas | 18/18 exact |
| Defined executable batch syntax | Exact in all three scopes and full recursive provenance |
| Native Sail handoff arena | All 4,194,304 bytes exact at 3,487,048 steps |
| Separate full compiler/program Sail run | Exit 0, empty stdout; 3,483,903 compile steps / 126 program steps |

Final explicit build **unevaluated-lean-5** and fresh full CSem gate
**unevaluated-full-1** both return exit 0 with **3,331 verified unchanged source
pins**. Each Lean build uses the shared root `build/lake.lock` and systemd
MemoryMax=12G after the memory check reported at least 25 GB available. The gate
includes the maintained focused comparator and, when the isolated native tool
is present, the native arena/on-off/execution batch. The native tool and its
build manifest have separate exact byte pins. Default semantic, purity,
standard-axiom, discipline, abstraction and generator checks all pass.

The native arena observation and full program run have separate images,
commands and metadata: the former uses the comparator's fixture filesystem,
the latter the ordinary executable invocation. Neither observation is a
reachability/refinement proof. Historical broad 1,815 / generated 1,200 results are
not recomputed or pooled, and sum cuts are never regenerated.

The [portable evidence package](unevaluated-syntax/evidence.tar.xz) retains all
final, failed and superseded captures, source snapshots, receipts/logs and
native cut/execution artifacts. Run `python3
results/model/unevaluated-syntax/verify.py --source-root .` to verify every
member hash, the distinct counts and the frozen source manifest. All 6,830
members verify; the xz package is 19.0 MB. The generator
`freeze.py` records its exact input membership; no failed observation is pruned.

The focused controls include nested selected/unselected generics, default-first
and no-default lists, typedef/pointer controls, unevaluated increments/calls,
unselected side effects, string associations, adjacent assertion messages,
file/block/struct/union/nested declaration contexts, generic assertions,
array/local declaration expressions, macros, #line and includes. Twelve
invalid controls are diagnostic observations: false assertions in three
contexts, duplicate defaults/matches, no match, missing colon/value, malformed
unselected expression, trailing comma and invalid assertion messages.
Matching diagnostics contribute no successful coverage.

## Explicit residuals and universal duties

A separate valid-C residual is `_Static_assert(1,L"wide")`: strict GCC and
independent Raw.checked accept it, while the unchanged compiler/model reject
its wide string message. It is preserved outside positive and invalid counts,
with no new Supported exclusion. Other grammar residuals remain: GNU omitted
conditional middle operand, typeof, choose, K&R definitions, deferred inline
attachment, GNU local-label representation and pack-layout consumption. The
old invalid-initializer/declarator fixtures remain diagnostic controls.

| Independent 56 residual | Model | Independent Raw.checked |
|---|---|---|
| gnu-conditional | constructs Cabs | raw checked error |
| typeof | missing specifier action | constructs Cabs |
| choose | missing expression action | constructs Cabs |
| invalid-initializer / invalid-declarator | compiler rejection | raw checked error |
| kr-definition | missing K&R construction | raw checked error |
| inline-unused | deferred replay attachment gap | constructs Cabs |
| local-labels | missing Cabs representation | raw checked error |
| pack | constructs Cabs | unimplemented pack-layout consumption |

No Cabs datatype constructor changes are introduced. Slot 1's upcoming
`CabsEalignof_expr` still requires the lead's combined traversal/interface
review; none of that changing semantic code is copied into this worktree.

`AssociationReplayErasure_Statement` is a new explicitly open local universal
noninterference obligation, audited at normal budgets. F3/F6 must also prove
scanner saved-word coverage and exact replay/type/declaration construction
through `RawConstruction_Statement` and `InjectedAuthorityTransport_Statement`;
the latter quantifies every Supported/Fits source with successful construction
and independent checked parsing, without a refusal alternative. Existing
whole-state error/success erasure, parser/normalization agreement, lexical
identity, physical/logical-view, image reachability and binary refinement
obligations remain open. F1/F7 runtime alias meanings/body coverage, bitfields,
GNU alignment semantics and complete multi-TU ghost construction are separate.
Finite comparisons and native arenas prove none of these universal duties.

Lua's abstraction inventory and mandatory task table were read first; existing
model scanner/actions, typed Cabs, Phase7 conversion and independent comparator
are reused by name with attribution. No library machine proof was copied, so
no library instruction-byte identity claim is made.

## Failed and intermediate attempts

Build 1's original two independent fixtures passed while shared generic replay
failed. Build 2 exposed the fixture word-cursor gap; its complete focused
captures retain 21 raw matches and five shared matches, plus the valid wide
message refusal. Build 3 repairs that cursor and passes all 21 focused positives
in all three scopes. These are intermediate snapshots. An overlapping retry
collided with an existing output directory and damaged the combined console
log; individual captures and the report remain, and the final run uses a new
directory. Early receipt polling used the request-specific filename before
the verifier archived the service's matching current result.

Build 4 fails on Lean record-field layout, with matching verified exit 1; no
budget is raised. The direct native attempt fails on the sandbox systemd bus
and contributes no Sail coverage. All failures, receipts and superseded
captures are retained. See also tasks/DEAD-ENDS.md; do not resend those routes.

Ordinary final `git add` is blocked creating the worktree's `index.lock`
(read-only filesystem). The scoped files are handed to the existing lead commit
service with exact hashes after all validation; no alternate Git store, push
or main merge is used. This infrastructure failure does not invalidate the
successful frozen build/gate or native observations.
