# Combined local replay-erasure adoption, 2026-10-28

The reviewed worker theorem `2da9a71` is applied as `bb48cc3` on adopted
array-decay main `278a362`. `Ghost.associationReplayErasure` proves the
unchanged `AssociationReplayErasure_Statement` for arbitrary recursion
interface, saved words and compiler state, including parser diagnostics and
appended-source detection. No parser-success or construction premise is added.
The shared `erase_syntaxFields` law factors the syntax-only record update.

The combined `scripts/check.sh --csem` receipt `replay-integrated-1` passes
with **3,646 frozen source inputs**, including the explicit observer build,
ordinary-budget proof compilation and standard-axiom audit. Both new theorems
use only `propext`, `Classical.choice` and `Quot.sound`. All inherited gate
regressions pass; their native runs do not replace the kernel-checked proof
and are not pooled into historical coverage totals.

The source map differs from the passing combined array checkpoint in exactly
`Tcc/Model/GhostDriver.lean` and `Tcc/Model/GhostAudit.lean`. The compiler,
model actions, CSem definitions and comparison drivers are unchanged. The
root array image/executable refresh receipt is retained separately; it pins
the prior adopted runtime sources, not the new proof source.

[The worker report](ghost-replay-erasure.md) retains its own failed and passing
focused/full builds, 20 archive members and 3,377 source pins. This combined
archive retains eleven additional captures: requests, receipts, complete gate
log, the two changed proof/audit sources and the prior main artifact refresh.
Run `python3 results/model/replay-integration-2026-10-28/verify.py --sources`
on this snapshot to check every archive member, source pin, audit line and
exact two-file delta from the array integration.

Only the local snapshot-restoration contract closes. **Eleven ghost contracts
remain open**, including construction, saved-word transport, whole-run erasure,
parser/normalization agreement, physical origins and injected authority.
`TypeAlignmentRepr_Statement`, F1/F2 obligations, fuel, binary refinement and
the whole-compiler theorem remain open. Supported/Fits and the no-refusal
headline are unchanged; sum cuts remain frozen.

Slot 1 continues bitfields and shared compound-lvalue lowering. Slot 2 now
repairs assertion message encodings. Four silently accepted invalid bitfield
constructs have a separate queued compiler/model task, dependent on review of
the F1 native captures; they do not count as valid-program coverage.
