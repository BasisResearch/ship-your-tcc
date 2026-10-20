# Phases, obligations and exit criteria

See BRIEF.md for the full plan. Status is updated as work lands.

## Phase 1: `tcc -run` on Sail (RV64I)
| item | exit criterion | status |
|---|---|---|
| 1.1 tinycc pinned | mob 43c7708 vendored with COPYING (LGPL-2.1) | done |
| 1.2 RV64I codegen | `TCC_RV64I_SOFT` patch; corpus runs on Sail and matches gcc; `check_isa.sh` clean on all tcc output | done (43/43; tests2 101/104, no codegen failures) |
| 1.3 bare-metal image | tcc ELF (rv64i/lp64, newlib, HTIF), in-image FS (headers, libtcc1.a, libc.a, libgcc.a), static arena, archive loader, `fence.i`, argv `tcc -run /prog.c` (no fence.i: the model traps on it; plain fence) | done |
| 1.4 programs | hello, loops, structs, fnptr, doubles, printf run on Sail; outputs and compile/program step counts recorded | done (7/7 rerun after errno fix on 2026-10-20; README); errno and explicit-libm regressions also pass |

## Phase 2: three-way differential
| item | exit criterion | status |
|---|---|---|
| 2.1 Certora/clean | `ccomp -interp`, `clightgen -lean`, Lean CCLib interpreter built | done (results/certora.md) |
| 2.2 corpus | Csmith (common subset, UB-free per `ccomp -interp`), CompCert test/c, tcc tests2 | done: 345 programs, results/corpus.md |
| 2.3 differential | every program attempted on tcc/Sail, Lean Clight, `ccomp -interp`, gcc -O0; each comparable disagreement classified and minimised | measured: 345 attempted, 301 common, 298 agree, 3 reduced discrepancies; 44 excluded, mixed harness versions documented |
| 2.4 report | DIFFERENTIAL.md | done; publication approved 2026-10-20; Cove MCP unavailable in this session |

## Phase 3: one per-program end-to-end theorem
The program is `c/tests/p3/sum.c`, printing `55\n`. No end-to-end theorem is
claimed. The following are planned obligation names; they are not yet Lean
declarations. When formalized, unproved claims must be `def …_Statement : Prop`.
The reuse audit is in `results/proof-readiness.md`.
| obligation | supplies | status |
|---|---|---|
| `CompileCut_Statement` (3a) | reset-to-cut execution plus full generated code/data at the cut | open: OCaml `WhileMin.loaded_fillZero` establishes snapshot validity only; startup remains unproved there too |
| `CodeRefinesClight_Statement` (3b) | C refines Certora's Lean Clight of P (syi MachCSL + generators) | open: native arena/register capture validated; heap/stack witness and Lean 4.32.1/4.34 integration needed |
| `PrintfSpec_Statement` | printf machine spec linked to a parameterized Clight external-call model (after ship-your-lua `fwrite`) | open: unchanged Certora external axioms conflict with the standard-only axiom audit |

Use `FLAGS=0` for an exact-output theorem; the existing `sum.raw` includes
debug code-dump output. Captured bytes, execution measurements and conditional
statement definitions do not close these obligations.

## Phase 4 (stretch)
Not started.
