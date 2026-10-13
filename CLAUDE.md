# ship-your-tcc — discipline

`tcc -run prog.c` on the executable Sail RV64I model (Phase 1), a three-way
differential against Certora's Lean Clight and `ccomp -interp` (Phase 2), and
per-program end-to-end theorems in Lean (Phase 3). The plan is BRIEF.md; the
ledger is PHASES.md. The proof discipline is ship-your-lua's (itself
ship-your-interpreter's), copied rather than re-derived.

## Before any work
* Read BRIEF.md (including the 2026-10-13 update) and PHASES.md.
* **Reuse, don't rebuild.** The bare-metal build, generators, MachCSL, the boot
  witness and the libgcc / soft-float / dlmalloc / newlib-stdio proofs live in
  ~/Documents/code/ship-your-lua (most advanced copy) and syi. Copy with a row
  in ATTRIBUTION.md. Before reusing a proof about a library routine, check the
  routine's instruction bytes are identical in our image (same xPack
  GCC 15.2.0 + newlib 4.5.0 rv64i/lp64 multilib).
* Before proof work, run ship-your-lua's `scripts/abs_inventory.sh` and reuse
  by name; read its CLAUDE.md "Mandatory tool per task shape" table, which
  applies here verbatim once the layer is copied.

## The target
* RV64I only: no M, F, D, Q, A or C instruction may execute. tcc's riscv64
  backend is patched (`patches/0001-tcc-rv64i-soft.patch`, macro
  `TCC_RV64I_SOFT`) to call libgcc helpers, lp64 soft-float ABI.
  `scripts/check_isa.sh` fails on any forbidden instruction; check.sh stage 1.
* `vendor/tinycc` is pristine at 43c7708 (`vendor/TINYCC_REV`). Changes are
  patches in `patches/`, applied to a copy by `scripts/tcc_src.sh`.

## Laws (ship-your-lua CLAUDE.md "Laws", unchanged)
1. **Elaboration budget.** Never raise `maxHeartbeats`/`maxRecDepth` (R2). A
   timeout means the construction is wrong: ground literals first, then more
   abstraction; reflect on the first-order write log, never whnf the Sail state.
2. **No `sorry`/`axiom`/`native_decide`/`bv_decide`.** An unproved theorem is
   a `def …_Statement : Prop` listed in PHASES.md; a genuine gap is a named,
   typed premise with a doc comment saying what supplies it. Check with
   `#print axioms` (⊆ {propext, Classical.choice, Quot.sound}; check.sh stage 6).
3. **Duplication is a signal.** Two similar proofs: factor before the third.
   The abstraction gate (`abstractions/gate.py`, `abstractions/clusters.tsv`)
   fails when a cluster's per-case cost stops falling: run /abstraction-discovery.
4. **Infeasible steps** return the machine-checked obstruction, not a workaround.
5. **Builds.** One `lake build` at a time, always under
   `systemd-run --user --scope -p MemoryMax=12G` (OOM kills on this box, Oct 13).
6. **Hide complexity by shape.** Named-field structures, not ∃/∧ towers (R6, R7).
7. **Generated files are generated.** Change the generator, never the output.

## Machine (aws-dev, shared)
`free -g` before heavy builds (wait if < 25 GB available); cap heavy jobs with
systemd-run; at most ~8 concurrent Sail runs (~120 MB each); keep disk under
~60 GB; never touch other projects' directories or kill others' processes.

## Gate
`scripts/check.sh [--static-only]`: ISA, forbidden tokens, discipline
(`scripts/check_discipline.py` + `scripts/discipline_rules.tsv`), abstraction
gate, lake build, axioms. Extend the discipline by appending TSV rules.

## Documentation
README.md: plan and numbers. PHASES.md: obligations and exit criteria.
DIFFERENTIAL.md: Phase 2 results. ATTRIBUTION.md: provenance. Git holds history.
