# Phases, obligations and exit criteria

See BRIEF.md for the full plan. Status is updated as work lands.

## Phase 1: `tcc -run` on Sail (RV64I)
| item | exit criterion | status |
|---|---|---|
| 1.1 tinycc pinned | mob 43c7708 vendored with COPYING (LGPL-2.1) | done |
| 1.2 RV64I codegen | `TCC_RV64I_SOFT` patch; corpus runs on Sail and matches gcc; `check_isa.sh` clean on all tcc output | done (43/43; tests2 101/104, no codegen failures) |
| 1.3 bare-metal image | tcc ELF (rv64i/lp64, newlib, HTIF), in-image FS (headers, libtcc1.a, libc.a, libgcc.a), static arena, archive loader, `fence.i`, argv `tcc -run /prog.c` (no fence.i: the model traps on it; plain fence) | done |
| 1.4 programs | hello, loops, structs, fnptr, doubles, printf run on Sail; outputs and compile/program step counts recorded | done (7/7, README) |

## Phase 2: three-way differential
| item | exit criterion | status |
|---|---|---|
| 2.1 Certora/clean | `ccomp -interp`, `clightgen -lean`, Lean CCLib interpreter built | done (results/certora.md) |
| 2.2 corpus | Csmith (common subset, UB-free per `ccomp -interp`), CompCert test/c, tcc tests2 | done: 345 programs, results/corpus.md |
| 2.3 differential | every program run on tcc/Sail, Lean Clight, `ccomp -interp`, gcc -O0; each disagreement classified and minimised | running |
| 2.4 report | DIFFERENTIAL.md | pending |

## Phase 3: one per-program end-to-end theorem
Obligations (each a `def …_Statement : Prop` until proved; none yet stated):
| obligation | supplies | status |
|---|---|---|
| `CompileCut_Statement` (3a) | tcc's compile phase reaches the jump into C at a fixed address with code bytes C (captured-cut witness, after ship-your-ocaml `WhileMin.loaded_fillZero`) | not started |
| `CodeRefinesClight_Statement` (3b) | C refines Certora's Lean Clight of P (syi MachCSL + generators) | not started |
| `PrintfSpec_Statement` | printf machine spec linked to Clight's external-call axiom (after ship-your-lua `fwrite`) | not started |

## Phase 4 (stretch)
Not started.
