# Local universal association replay erasure

`Tcc.Model.Ghost.associationReplayErasure` inhabits the unchanged
`AssociationReplayErasure_Statement` for the actual `genericAssociationSyntax`.
It quantifies over arbitrary `R : Rec`, `saved : Ghost.SavedTokens` and
`st : TccSt`. There is no parser-success, construction, saved-word validity or
conclusion premise. This closes only the local snapshot-restoration duty.

The reusable `erase_syntaxFields` law proves that arbitrary replacement of
`ghost`, `ghostAudit`, `ghostNormalizations`, `ghostDrops` and `ghostTokenErrors`
preserves the existing single `Ghost.erase` result. It is proved by record
reduction once. The replay proof splits its internal parser's arbitrary
result and its shape comparison. Parser success, appended-source detection
and parser diagnostics all return `.ok` on the original snapshot with only
these syntax fields replaced. Equality elimination rules out an outer error.
In particular, parser errors become construction gaps while the original
compiler console, errors, files, token words, symbol/section tables, stack,
resource counters and all other compiler fields are restored.

No implementation, CSem meaning, Supported/Fits definition, compiler source,
image or sum cut changes. The proof does not evaluate a program run, unfold
Sail state, raise budgets, or use differential counts as a substitute.
`GhostAudit` explicitly audits both new theorems: each depends only on
`propext`, `Classical.choice` and `Quot.sound`.

## Validation and retained evidence

| Receipt | Scope | Exit |
|---|---|---|
| `replay-erasure-local-1` | root-locked, capped local attempt; systemd bus denied | 1 before Lean |
| `replay-erasure-lean-1` | first focused proof; match-splitting failure retained | 1 |
| `replay-erasure-lean-2` | `lake build Tcc.Model.GhostDriver Tcc.Model.GhostAudit` | 0 |
| `replay-erasure-full-1` | `scripts/check.sh --csem`, including explicit observer builds and axiom audits | 0 |

Both completed service builds and the full gate have matching receipts with
**3,377 unchanged source pins** verified by script. The focused passing build
and full gate use identical pinned sources. All Lean builds use the shared
root `build/lake.lock` and systemd `MemoryMax=12G`; memory checks before the
requests reported 95–96 GiB available. Static/discipline/abstraction,
generator drift, CSem totality/purity, standard-axiom and semantic/regression
stages pass. The normal gate's comparisons remain regression observations;
the universal result is the kernel-checked theorem itself.

Run `python3 results/model/ghost-replay-erasure/verify.py --source-root .`
to verify the [evidence archive](ghost-replay-erasure/evidence.tar.xz).

The portable evidence package retains the failed and passing requests,
matching receipts, full logs, failed/final proof snapshots, Lua inventory and
lead note. Its manifest verifier checks every member, request IDs/statuses,
standard-axiom audit lines and final source pins. It does not certify missing
compiler theorems.

## Remaining duties and precise ledger suggestion

For PHASES.md's F3/F6 ghost ledger, mark only
`Ghost.AssociationReplayErasure_Statement` proved by
`Ghost.associationReplayErasure`, linked to this report and its passing
focused/full receipts. Keep the named Statement for its existing consumers.
The lead owns adoption and the ledger edit.

Saved-word coverage and complete compiler-action syntax still require the
scanner/parser induction and `RawConstruction_Statement` plus
`InjectedAuthorityTransport_Statement`; this proof accepts arbitrary saved
maps and establishes neither. `Erasure_Statement`, `RawErasure_Statement`
and `InjectedAuthorityErasure_Statement` remain whole-run duties, including
error behavior. Fixture/raw parser and normalization agreement, physical
origin/read-view laws, image reachability, fuel adequacy and binary refinement
remain separate and open. No whole-compiler correctness claim follows.

## Dead ends

The direct root-locked systemd-12GB attempt fails at the sandbox's user scope
bus before Lean starts. No uncapped build is used. Frozen lead service builds
retain the required root lock and cap.

`replay-erasure-lean-1` fails because a bare `split` chooses the outer result
match; the parser match remains in an equality hypothesis. Split that
hypothesis and its shape check before eliminating result equalities. The
field-update law already compiled. `replay-erasure-lean-2` supplies the
corrected proof at ordinary budgets. The failed source/receipt/log are
retained; no success premise or implementation rewrite is needed. These
routes are also recorded in tasks/DEAD-ENDS.md.

Lua's abstraction inventory and mandatory tool table were consulted before
proof work. The existing `Ghost.erase` and `genericAssociationSyntax` are
reused by name with attribution; no machine-library proof was copied, so no
library instruction-byte identity claim is made.

Ordinary staging is blocked creating
`.git/worktrees/sol-f2-tokens/index.lock` on read-only Git metadata. The
existing MANAGER.md lead commit service receives exactly the eight scoped
path hashes after validation. No alternate Git index/object store, push or
main merge is used; this infrastructure failure requires no new proof task.
