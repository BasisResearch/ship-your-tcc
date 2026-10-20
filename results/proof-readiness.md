# Phase 3 reuse audit

The measured program `c/tests/p3/sum.c` sums 1 through 10 and prints `55\n`.
Its current Phase 1 record has 6,632,317 compile steps, 4,189 dump steps and
2,470 program steps. These measurements establish a candidate, not a theorem.
There is no root machine-proof Lake project or checked end-to-end theorem.
The current ccomp and Lean interpreters both execute the candidate to
`55\n`; Lean takes 99 interpreter steps. The generated Clight and these
outputs are saved under `results/review/`. These runs do not establish
machine-code refinement.

The audit inspected ship-your-lua at ab205a6 and ship-your-ocaml at 7222d01e,
and ran Lua's required `scripts/abs_inventory.sh` read-only.

## Captured cut

OCaml's `OCaml/Vm/Boot/WhileMin.lean` explicitly says
`WhileMin.loaded_fillZero` proves captured snapshot validity and leaves the
4,269,257-step startup execution open. Its PHASES.md says the same.
`boot_cut.py` and `gen_boot_log.py` extract observational data; chunked
memory-log checks do not establish that the CPU executed the trace.
Our `CompileCut_Statement` must separately establish reset-to-cut execution.

Reusable OCaml infrastructure is
`Vsa/Sim/Boot/{Log,Checks,Image,Bytes}.lean`, `WriteLogRead.lean`,
`RamReadBytes.lean` and `Densify/Resp.lean`. Relevant certificates include
`StoresChecked.chunk/join`, `RunTree.Checked.leaf/node`, `LogOk.of_checks`,
`writeLog_view`, `observedMem_bytes_stored`, and `bytesT_memEqv`.
The capture scripts are `boot_cut.py`, `gen_boot_log.py`,
`boot_certificate.py`, `gen_boot_dump.py`, and `gen_boot_registers.py`.
Source signatures were inspected; the upstream WhileMinLoaded olean was
absent, so this audit does not claim a fresh successful proof build.

The earlier `sum.code.bin` contains only main's 136 bytes. Its printf target
at `0x86400148`, format data at `0x86410b68`, and runtime state also need
capture and specifications. The machine observation includes console bytes:
`sum.raw` starts with debug code-dump lines. An exact `55\n` theorem needs
the `FLAGS=0` image, or an explicitly justified observation projection.

The new native `FLAGS=0` observation in `p3/sum-cut/` now captures the full
4 MiB arena with byte presence and all 176 defined registers, at step
6,632,342 and PC `0x80001b48`. It has 77,864 present arena bytes and an empty
pre-cut console. Main's 136 bytes match the earlier dump exactly. Heap and
stack memory outside the arena are not exported. This resolves the narrow
arena/register extraction task, while reset-to-cut execution certification
and snapshot validity in the kernel remain open. See `capture-cut.md`.

The stock emulator exposes `--trace-pcs`, `--trace-all` and `--max-steps`,
but no memory-export or stop-at-PC option. OCaml's generated `bootdump`
prints registers and checks supplied expected memory; it does not export
the full arena, and a built executable was not found. The new isolated
helper reuses that loop and enumeration, reads only existing dependencies,
and writes its own build products in this checkout. It resolves the cut PC
from the exact `FLAGS=0` ELF and records hashes and an observation-only
status. It introduces no root proof project or semantic predicates.

## Smallest useful machine lemma

Prove the generated loop reaches the printf seam at `0x8640006c` with
`a1 = 55` and `a0 = 0x86410b68`, under explicit loaded-byte and stack
preconditions. A loop invariant is `s = i*(i-1)/2`, `1 ≤ i ≤ 11`, with
rank `11-i`. The 34-instruction main uses RV64I `addw`/`slti`, no libgcc.

Reuse Lua's `Vsa.Sim.DeriveCase`, `SegToTripleFramed`, `DeriveLoop`,
`DeriveCallSeg`, `FnSummary`, `FrameMeta`, `VsaIris.MachWP`, and the existing
`scripts/syi/{disasm_to_segment.py,gen_segment.py,gen_fn.py}` after retargeting
the image data. Library specifications require instruction identity before
reuse. A loop lemma would be a useful partial result; it would leave startup,
printf and the Clight bridge open.

## Semantics boundary

The machine layer uses Lean 4.34; Certora's CCLib uses 4.32.1. Their oleans
cannot simply be imported into one project. A source integration and
dependency compatibility check are required under the single-build, 12 GB
Lean cap.

Certora's `CCLib/Events.lean` declares `externalFunctionsSem` and
`inlineAssemblySem` axioms and corresponding determinism axioms. These enter
`CC.Step`'s axiom closure through its type. Adding a printf behavior
hypothesis alone does not remove them. The bridge needs an explicitly
parameterized external-call model and a named machine-to-model premise,
while keeping the standard-only `#print axioms` audit. Replacing Clight
with an opaque predicate would not satisfy the requested theorem.

All three Phase 3 obligations remain open in PHASES.md. No new axioms,
admitted proofs or claimed compiler-correctness theorem were added.
