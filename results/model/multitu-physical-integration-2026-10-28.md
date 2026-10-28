# Ordered translation units and pure physical-transform integration

Candidate `75a2df8` combines reviewed multi-TU checkpoint `96c520e` and physical
proof checkpoint `b42f766` with adopted member attributes. The full frozen
`scripts/check.sh --csem` gate passes for **4,162 input paths**, followed by
independent postchecks. This inventory includes tracked configuration,
assembly, expected-output and build files plus established runtime fixtures.
The evidence directory retains every pinned source blob and the actual logs.

## Proofs and compiler observations

The unchanged `PhysicalOriginMap_Statement` and `PhysicalReadView_Statement`
are now proved for arbitrary `ByteArray` inputs. Block erasure and shape have
public proofs too. All 36 helper and four public theorems pass the permitted
standard-axiom audit. A shared trace of actual model-loop iterations supplies
byte erasure, physical ordering, nonempty bounded slices and independent CRLF
transport. Splice/refill and the whole `PhysicalTransformObligations` structure
remain open. The original proof report retains the exact theorem scope.

Compiler handoff preserves ordered per-TU source/configuration, tokens, Cabs,
physical openings, origin maps and injected authority. The eight-command
comparison retains **15 TU records**, four completely checked bundles, six
independent fast-reference arena matches and two separately classified compiler
diagnostics. Pack-layout and guarded-header opening-schedule differences remain
explicit; neither is normalized into agreement. Deferred replay uses its
local erasure proof. Whole-command `MultiTUConstruction_Statement` and
`MultiTUErasure_Statement` remain open.

The original 56-case syntax suite gives **51 shared / 52 component / 51
independent exact agreements**. The other cases remain explicit nonsuccesses.
All 58 physical-origin cases, four guards and 15 ghost on/off checks pass.
For the attribute merge, all **42 defined** cases have exact independent Cabs;
the adopted predecessor had 36. The separate eight GCC placement-diagnostic
sources remain diagnostics. Two of those are exact under tcc extension syntax,
for 44/50 syntax agreements overall. No previously exact case regresses.
The full gate also retains the established member layout/bitfield checks.

## Fresh native two-file execution

The unchanged upstream 104 invocation compiles `/104+_inline.c` before
`/prog.c`. Its complete **4 MiB** native Sail arena and handoff registers match
the model. The exact captured compiler ELF is then executed through program
completion: exit zero, byte-exact expected output, **11,461,362 compiler +
64,583 program = 11,525,970 total steps**. Handoff capture step 11,461,387 is a
separate capture coordinate, not the compiler tally.

Actual ELF SHA-256:
`5f24036ee45b85c6cc560cfd4c42eeee3f3271ed37ec78e93e41fe57f0778f4b`.
Both primary/companion bytes, pinned command configuration, registers,
model/native arenas, trace and executable are retained. The six inherited
full-gate native arena exports and their full program executions are separate
controls, giving seven complete native captures in this integration archive.
No historical 104 capture is relabelled as this execution.

## Evidence and remaining obligations

Run `python3 results/model/multitu-physical-integration-2026-10-28/verify.py`.
`--sources` also checks the current checkout against all 4,162 frozen inputs.
The checker verifies archive members, gate completion, all source objects,
ordered-TU comparisons, 40 theorem audits, exact syntax/physical observations,
complete native arena bytes/registers and the same-ELF 104 execution.
Earlier worker manifests remain separately referenced rather than retested
under an invented source revision. Finite comparisons are not universal proofs.

There is no new C-image patch in this integration. Whole compiler refinement,
read/refill/host transport, raw construction and whole-run erasure, complete
frontend coverage and the remaining F0–F3 connections stay open as recorded in
PHASES.md. Enum semantics and GNU conditional shared lowering are separate
worker checkpoints, not part of this 4,162-input gate. `Supported`/`Fits` and
the whole-language target are unchanged; the sum compile-cut chain is frozen.

Portable verification passed: **10,842 regular members**, all4,162 frozen
inputs,40 physical theorem audits and the finite observations above. The
lead independently executes the checker with `--sources` before adoption.

Post-gate tool-only difference: adopting current main also brings the worker
launcher ancestry fix. `codexwork.sh` is the sole changed path among the4,162
pins; it is not a compiler, semantic or generator input. Its exact old gate
bytes remain archived; the exact adopted script and both hashes are retained
under `post-gate`/`post_gate_tools`. `--sources` requires all other input bytes
to match and explicitly checks this labelled launcher difference. `bash -n`
and the final static gate check the adopted tree. This does not retroactively
change the source set claimed by the completed full gate.
