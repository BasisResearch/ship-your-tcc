Allocation oracle recovery, 2026-10-27; worker checkpoint reviewed 2026-10-28.
The frozen worker gate and focused comparisons pass. Combined integration
with raw/anonymous/once-cache semantics now also passes, as recorded below.

Pins: Cerberus `704c4e697845127bced66c93fa0763eb51527ca4`, Lem
`e6d8db5680962f49060257d6c2bcb13c284b910f`, Lean 4.34.0. C11, the repin,
RV64 reader, image FS and adopted IEEE/literal semantics are retained.
The frozen sum compile-cut is unchanged.

The recovery JSON was read before adapting the draft interface. Only its
request/policy/live-range/dispatch design is reused, with attribution; no
recovery generated files or old bump proof claims were imported. Lua's
abstraction inventory and mandatory tool table were consulted. No machine
routine proof is reused and no instruction-identity claim is made.

Acceptance requires a nonnegative size, positive supported power-of-two
alignment, positive aligned address, representable RV64 one-past end, and
byte disjointness from every live range. All Int arithmetic precedes any
pointer conversion; oversized values never wrap into accepted placement.
Zero-size intervals occupy no bytes and may share a base with live storage.
Freed records remain for provenance diagnostics but impose no placement
constraint. Reuse receives a new allocation ID. Oracle heap-region reuse
removes old byte entries so uninitialized memory cannot inherit a previous
lifetime's initialized bytes. Default bump allocation retains its old body,
kill ordering, cursor updates and region byte behavior.

A missing or invalid answer kills that resolution explicitly and marks
`allocationRejected`. It is neither a fabricated out-of-memory answer nor
C UB nor an ordinary model refusal. The semantic leaf is `invalidAllocation`;
no halting outcome is derived from that leaf. `NoUB` quantifies over arbitrary
policies and still rejects UB reached on any valid prefix. Invalid future
answers cannot erase an earlier UB. Core evaluation order remains the
existing exhaustive ND tree, separate from the fixed allocation policy.

`outcomes` existentially quantifies an arbitrary policy before the eventual
fuel bound. No finite placement list defines semantics. CLI
`--alloc-addresses` is a finite diagnostic replay: list exhaustion rejects
the replay. Runtime allocations use this policy; frontend const-expression
mini-runs retain their existing execution, a separate F1/F4 connection.

Validation: full `scripts/check.sh --csem`, focused probes, cone and axiom
checks pass under frozen request `allocation-oracle-full-3`. Systemd bus access is
denied in the worker sandbox; the capped lead service is used. No uncapped
Lean invocation is made.

Checked worker evidence (1,615 frozen source hashes reverified by the lead):

* Capped request `allocation-oracle-4` completed with exit 0 and verified
  source hashes. It builds the production driver, TccCSem, the legacy
  allocator test and the new allocation-oracle test. All 19 adversarial
  allocator checks pass, including negative requests, end overflow, empty
  ranges, dead/live boundaries and real region/free/reallocation with byte
  clearing and fresh provenance. The four inherited bump cases pass.
* Twenty focused C checks pass: three policies each on printed addresses,
  integer address comparisons, heap lifetime and evaluation order; one forced
  heap-address reuse; three independent UB classifications; four invalid
  replay controls. Ascending/descending placements give 11/22 for `order.c`.
  Each allocation policy retains both 12 and 21 for indeterminately sequenced
  calls. The same-address heap replay prints `reuse=1 value=19`.
* `scripts/allocation_default_check.py` gives exact ordered verdict equality
  with the pinned OCaml oracle on all five C probes under the same default
  implementation. The RV64 implementation is used for the independent Sail
  pointer comparison. The unchanged source hash of
  `c/tests/cfg/p12_pointer.c` is verified before comparing the current CSem
  result to its retained Sail observation: `null=0 bits=64 roundtrip=1\n`,
  exit 0; the historical run is 12,883,397 steps, measured
  2026-10-27T18:15:13Z. This is explicitly historical image evidence, not a
  recertification of those image pins against later compiler patches.
* `default-preservation-source.json` verifies that the original allocator
  body equals `bumpAllocator` after normalizing only its name, comments and
  whitespace. `allocator_default_preserved` proves exact dispatch equality
  for arbitrary requests/states with bump policy, including all kill arms.
* General proofs: `oracleAnswerOk_sound`, `oracleAllocate_active_sound`,
  `allocator_oracle_active_sound`, `oracleAllocate_accepts`,
  `oracleAllocate_rejects`, and `allocator_active_placement_sound`. The last
  theorem states bump's required `BumpPlacementPre` explicitly; it never
  pretends arbitrary synthetic bump states are bounded/fresh. Its supplier
  is F4's reachable memory invariant. Oracle soundness needs no bump cursor
  invariant. No elaboration option was raised. Axiom audits are part of the
  full gate.
* `halts_of_allocation_resolution` connects any fixed-policy stable verdict
  to the actual named-field outcome witness. Finite execution tests do not
  prove an eventual-fuel witness for an arbitrary C source.

Proposed ledger text (STATUS.md and PHASES.md are intentionally untouched):

“F1 runtime allocation oracle: checked finite RV64 placement functions are
quantified by CSem.outcomes, with the ordinary bump execution retained as
one resolution. Invalid answers are rejected independently of C UB and
model refusal; no OOM answer is invented. Generic acceptance soundness,
checked-answer acceptance/rejection, default dispatch preservation and active placement
freshness/alignment/bounds are proved. Bump's cursor premise is the named
BumpPlacementPre supplied by F4's reachable memory invariant. Focused tests
cover independent evaluation order, UB, pointer observations and lifetime
reuse. F4/F5 still owe the machine/Core memory simulation and eventual fuel
adequacy, under the existing TccRunCorrect_Statement and explicit dynamic
limits. Frontend constant-expression mini-runs retain the existing bump
execution. F4 still owes the existing single-branch divergence adequacy;
no whole-compiler correctness theorem is claimed.”

Dead ends retained for the lead:

* Do not recover the old draft wholesale: it lacks finite pointer bounds,
  interprets `none` as fabricated OOM, names absent theorems, and carries
  stale generated files. This patch reuses only the reviewed interface.
* Request 1 failed only in two proof scripts: explicit reflexivity is needed
  after default dispatch reduction, and a local `let` must reduce before
  splitting oracle cases. Request 2 compiled the proofs but caught Lean
  record-layout syntax in the test. Request 3 caught unqualified Int order
  lemmas in the generic dispatcher proof. Request 4 supersedes them.
* The first full gate rejected anonymous existential growth under R7. The
  outcome witnesses were replaced by named-field structures, without a
  discipline exception or weakening the predicate.
* Comparing the RV64 reader's 16-byte malloc alignment with the OCaml
  default's 8-byte alignment yields different bump addresses. That failed
  comparison is retained in `abi-mismatched-baseline-diagnostic.json`.
  Correct same-implementation default comparisons pass; do not change the
  allocator or normalize printed addresses to hide the ABI distinction.
* Reuse must clear old heap-region bytes under oracle execution. Upstream's
  no-write region allocation was valid only when an address was never reused;
  importing it unchanged would silently inherit initialized bytes.

The second full gate built the legacy replay, compiler/model executables and
new outcome structures, then caught an inherited `FuelExemplar.allocator_errno`
proof that unfolded only the old allocator definition. Its repaired simp set
also unfolds the unchanged `bumpAllocator`; no statement or budget changed.
The lead requested all new proof constants in the ordinary-axiom audit:
a source-name comparison verifies all nine are explicitly printed, together
with both adapted legacy allocator proofs. Frozen full request 3 supersedes
the failed first two full requests. The lead's reviewed limitations
(`BumpPlacementPre`, frontend mini-runs, historical image scope) are retained.

Fresh native allocation observation: capped request `allocation-oracle-sail-2`
completed with exit 0 and verified frozen inputs. The normal C11 tcc/Sail
path on `global-address.c` prints exactly
`object=2252413768 value=7\n`, exit 0. It executes 7,169,634 steps
(7,163,340 compile steps, 6,269 program steps); the native ELF SHA256 is
`d891902f1b93280e9e2f40e78651eafa6e58ed4de18e9239a43c7e62f551d135`.
`global-address.tcc.json` records the source, runtime/toolchain/library/image
pins, raw output, metadata and ELF hash, using the existing `cfg_probes`
measurement helper. Runtime input pins are checked unchanged after the run.
`native-probe.py` retains the exact helper script bytes from the frozen
request (the original invocation path was `build/allocation-sail.py`).

The same C source computes the identical observation in CSem when its global
object's oracle placement is 2252413768. CSem still initializes the value 7,
evaluates the program and runs its printf path. Only allocation addresses are
parameters; no reference bytes or stdout bytes enter execution. This is a
focused observation agreement, not a full machine/Core memory simulation.
`native-placement.json` and `focused-native.log` retain all 21 passing focused
checks, including this fresh target placement. The historical pointer probe
above remains separately labeled historical.

The worker stopped at its usage limit after the successful third full gate.
The lead verified all frozen source hashes, the three focused-result binary
hashes and the completed receipt before preserving this checkpoint. Its
combined integration is a separate required check; no earlier failed gate
is accepted as validation.

Combined adoption, 2026-10-28: the regenerated candidate passes the full CSem
gate with raw frontend, anonymous containers, IEEE and image once-cache semantics.
All 2,293 frozen source hashes are verified. The 21 allocation checks, six
default/historical comparisons, eleven raw frontend methods and 92 anonymous
member controls pass; both semantic cones contain no native floating and all
new proof audits use only the standard axioms. The explicit bump invariant,
frontend mini-run and F4/F5 obligations above remain required.

The first combined gate wrote its fresh checks over earlier result files.
Those outputs were retained separately and the historical files restored.
The gate now supplies an explicit build/ output directory to both allocation
check scripts. The final full gate and 21-case native-placement replay pass
with that correction; semantic sources are unchanged. The exact independent
OCaml overlay/artifact manifests, final logs and source/binary pins are in
allocation-oracle/integration/. This establishes the reviewed F1 checkpoint,
not the machine/Core memory simulation or tcc_run_correct.
