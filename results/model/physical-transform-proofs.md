# Pure physical-transform proofs

`Ghost.physicalOriginMap` proves the unchanged
`Ghost.PhysicalOriginMap_Statement`, and `Ghost.physicalReadView` proves the
unchanged `Ghost.PhysicalReadView_Statement`, for every `ByteArray`.
`Ghost.physicalBlockErasure` and `Ghost.physicalBlockShape` discharge the two
pure block fields of `PhysicalTransformObligations`. Its splice/refill fields
remain obligations; no instance of that whole structure is asserted.

The first passing focused build/audit is `physical-proof-11`: exit 0,
3,374 frozen source hashes verified. All 36 helper theorems and four public
theorems are individually registered in GhostAudit; their axioms are subsets
of `{propext, Classical.choice, Quot.sound}`. No elaboration limits changed.
The full frozen `scripts/check.sh --csem` gate also passes (`physical-proof-full-1`,
exit 0). It has the same 3,374 primary pins. An additional 399 unchanged-base
supplementary pins are verified separately, as described below.

## What is proved

- Block byte erasure holds for arbitrary origin arrays, including empty and
  malformed metadata. Shape equality uses exactly the existing premise that
  origin and input sizes agree, including size zero.
- Mapped entries occur in original physical order. Each has a nonempty,
  bounded half-open range, and translating exactly that original byte slice
  yields exactly its recorded byte. Original byte coordinates are preserved.
- Normalizing only physically adjacent CRLF in the actual block shadow gives
  exactly the byte/start/stop observation of the independent located phase 1.
  The proof generalizes the spec's accumulator and complete source point,
  rather than discarding an offset difference by normalization.
- Inputs are arbitrary bytes. Empty and short inputs, all nine trigraphs,
  unmatched and overlapping question marks, CRLF, lone CR, CR at EOF and an
  absent terminal newline need no fixture premise. Neither phase-one side
  fabricates a terminal LF. The later logicalView/phase-two terminal-LF policy
  is separate from this equality.

## Construction and unchanged behavior

The focused proof module builds a proof-only trace of the model's actual
iterations. One `loop_trace` induction characterizes both output bytes and
origin annotations, including the accumulator and the original cursor.
`block_loop` checks its equivalence to the existing imperative loop.
`block_trace` is the shared observation equation used for erasure, shape and
physical maps. Named `CellProperties` and `TraceProperties` bundle ordering,
bounds, removal flags and exact slice meaning before deriving either public
contract. Source-list transport and the independent CRLF comparison reuse the
same model table and trace. `replacement_not_newline` proves that trigraphs
cannot emit CR or LF, allowing the two independent phase-one orders to agree.
The spec never supplies model origins.

`PpLex.lean`, `Ghost.lean` and `Spec/Pre/Phase12.lean` are byte-identical to the
base revision. Both public Statement bodies are also byte-identical. The only
changes to GhostDriver are the proof import, four theorem wrappers and two
comment descriptions. There is no runtime refactor, generated-output edit,
compiler patch, CSem semantic change, source restriction, refusal alternative
or sum-cut regeneration.

Lua inventory and the mandatory task-shape table were read before proving.
The retained inventory is at Lua 0bb1880. No matching pure trigraph theorem
was available; standard-library loop/list/array/slice lemmas are reused by
name and recorded in ATTRIBUTION.md.

## Scope still open

Actual reads, held-byte refill, the host interface, successful splice/unget
transport, arbitrary Rec erasure, whole-run ghost erasure, whole-unit
construction, parser/compiler refinement and the whole-compiler theorem are
not proved here. Their existing typed obligations remain explicit and
unchanged. No finite comparison or axiom census is offered as a proof of those
contracts. Lead-owned main ledgers and adoption are untouched.

## Failed approaches retained

The local systemd attempt stopped at the sandbox user bus before Lean ran.
The lead's frozen service supplied every Lean build under the shared root
Lake lock and 12 GB cap. Requests 1–10 remain failed receipts with exact source
snapshots and logs. The useful dead ends are in tasks/DEAD-ENDS.md: legacy
range names, explicit Id reduction, nonlooping append laws, exact equation
selection, the real ByteArray.toList loop, and factoring the generated numeric
matcher equality before the loop proof. No failed receipt is passing evidence.

## Validation and portable evidence

Focused: `physical-proof-11`, exit 0, 3,374 verified source hashes and 40
standard-axiom theorem audits. Full frozen `scripts/check.sh --csem`:
`physical-proof-full-1`, exit 0, with all 3,374 primary pins verified before and
after execution. Its existing optional native controls ran; this report adds
no standalone native-coverage or broad-corpus claim.

The request helper's inherited suffix inventory omits shared Lem/OCaml/Core/
implementation sources, metadata and result-hosted runtime fixtures. Lead
independently pinned 399 supplementary blobs, each byte-identical to immutable
base `2d6d28dfb7c1ea7fc587aaf9f6144caf1d80d3b8`. All 399 current and Git-blob
hashes were verified again after the gate. They are separate immutable-base
evidence, not an expanded set claimed to have been frozen by the original
service. The initial 134-entry supplement is retained as superseded evidence.
The portable package includes both manifests, the lead pinning helper and
all 399 supplementary source bytes. Future helpers should reuse the canonical
broader inventory.

The maintained frozen-request helper is
`scripts/physical_proof_build_request.py`; it verifies all source hashes and
archives compact request-specific receipts before sources can change.


[The portable package](physical-transform-proofs/pins.json) contains every
requested source version for all eleven focused attempts and the full gate,
the completed receipts/logs, supplementary source bytes, the Lua inventory/
mandatory table and the staging failure. Content-addressed source objects
preserve every path/hash in each request without duplicating identical bytes.
All failed attempts remain exit 1; only focus 11 and full 1 are successes.

Run the independent artifact verifier without building Lean:

```sh
python3 results/model/physical-transform-proofs/verify.py
```

It verifies the archive and every regular member, all primary source objects
for every receipt, the 399 supplementary objects, the 40 named axiom audits,
identical focused/full primary manifests and unchanged public Statement bodies
and executable/spec transforms. It checks evidence integrity, not a replacement
for the retained kernel build. All finite gate output directories remain in
build/; the portable archive's scope is proof/build inputs, logs and receipts.

Git staging is denied by this sandbox's read-only worktree metadata. The
scoped frozen lead commit handoff supplies the review commit, with no alternate
index/object store or main integration. Main ledgers/adoption remain lead-owned.

Final portable verification: **3,560 regular archive members, 12 completed
receipts, 3,374 primary inputs, 399 separately verified supplementary inputs,
and 40 standard-axiom theorem audits**. The archive is 5,720,528 bytes.
