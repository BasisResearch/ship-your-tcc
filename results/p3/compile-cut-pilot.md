# CompileCut pilot: a kernel-checked evaluator run of tcc's compile phase

Question: can `CompileCut_Statement` (Tcc/Capture/Sum/Cut.lean) be discharged by a
kernel-reducible RV64I evaluator, with one generic soundness proof against the
Sail model, instead of a per-static-block replay (~900 CPU-hours)?

**Verdict: go.** We measured about 1.5 ms of kernel time per step (about 2.5
CPU-hours for all 6,632,342 steps) and 16 MB of generated source. The
generic soundness proof covers every instruction kind the compile phase
executes. `CompileCut_Statement` is now proved from one premise,
`ResetSim_Statement` (see "What remains").

## P0: inventory

* We found no proved first-order step function for all of RV64I. What
  exists, all symbolic in register values and proved against Sail, and all
  reused here:
  * `Vsa.Sim.block_mem_sound` (37 straight-line kinds, write log);
  * `term_step_bt` (branches, `j`, `jr`);
  * `stepObs_jal`, and `stepObs_jalr` from Tcc/Boot/Step.lean;
  * the `ExecuteAlu` characterizations (`sltu`, `sltiu`, `sra`) with
    `stepObs_alu`;
  * `decodeN`/`decodeW` (one `rfl` per word).

  syi-perf's `SymExec` is iris-based (SWP) and was not needed.
* Opcodes, from one native `--trace-all` run (`scripts/compile_trace.py`,
  5 min 54 s):
  * 49 mnemonics, 46 evaluator kinds. `lui` and `auipc` become `li`; `j`
    and `jal` become `jal`; `jr` and `jalr` become `jalr`.
  * The full table is in `build/ccp/trace/ops.tsv`. The top kinds: `addi`
    1,177,182; `ld` 739,004; `sd` 546,134; `bne` 507,954; `add` 477,361;
    `beq` 335,725; `lbu` 324,801; `slli` 242,125.
  * Never executed: M, F, D, A, C, `lb`, `sraw`, `ecall`. There is one
    `fence`, the barrier after relocation.
  * No ALU or load instruction writes `x0`.
* The trace in numbers:
  * 19,515 static PCs and 10,466 distinct words;
  * 733,434 stores and 1,273,006 loads;
  * 1,254,714 bytes written, in 4,920 pages of 256 bytes;
  * no fetch from a stored byte (tcc never runs code it wrote before the
    cut).

## P1: evaluator and cost

**Evaluator.** `Tcc/Ccp/Eval.lean`, 366 lines, uses only `Nat` and
structural recursion:
* the PC;
* the 31 GPRs packed in one `Nat`;
* the memory: a search tree of fixed shape over the 8,218 pages of 256
  bytes the run touches, each page holding its bytes and a known-byte mask;
* the code: 19,515 pre-decoded instructions in a search tree.

Any access the soundness proof does not cover faults, for example an unknown
byte, a store into the code range, or a misaligned or out-of-window access.

Compiled (`#eval`), it replays all 6,632,342 steps in 43 s. It ends exactly
at the captured cut: PC, all GPRs, and every one of the ~1.95 MB of known
bytes.

**Generator.** `scripts/gen_ccp.py` writes:
* the code table, 1.27 MB;
* the reset memory, 2.1 MB, built from exactly the bytes `TccSumElf` pins
  (`elfImage` plus `elfAbsent`, 701,581 bytes);
* 267 checkpoint states, one every 25,000 steps. Each is an overlay delta on
  its predecessor (`DT`, `MT.overlay`): 9.7 MB in total, 180 KB at most;
* 266 segment theorems `segK : run code k stK = some stK+1`, closed by
  `kernel_rfl`;
* the join `run_all`;
* 305 table-proof blocks.

`kernel_rfl` assigns `Eq.refl`. Only the kernel checks it, and it compares
the shared memory subtrees by pointer.

**Kernel cost per step** (user CPU, net of fixed costs; the box was loaded):

| version | region | ms/step | MB/step |
|---|---|---|---|
| v1 (`if`/`decide` trees, Bool state compare) | reset (memset) | 2.3 | 0.30 |
| v2 (Bool-match trees, strict stores, overlay deltas, `kernel_rfl`) | reset | 1.67 | 0.19 |
| v2 | lexer (`next_nomacro`/`preprocess`, step 700k) | 1.62 | 0.19 |
| v2 | step 3.0M | 1.54 | 0.19 |
| v2 | step 4.5M | 1.63 | 0.19 |
| v2 | step 6.0M | 1.68 | 0.19 |

Notes on v1:
* Comparing whole states with a Bool function cost 10–13 s per chunk.
* Patch-list deltas hit the kernel's deep-recursion limit at about 3,000
  pages.

v2 fixes both.

Microbenchmarks (`build/ccp/k/mb_*`):
* about 10 µs per kernel reduction;
* a 15-level code-table lookup: 0.73 ms;
* a register read and write: 0.23 ms;
* GMP operations on literals: negligible.

The fetch dominates. Fetching once per basic block (about 4.6 steps) is the
obvious next 2–3× gain, but it is not needed.

**25,000-step segments, built one at a time** (`scripts/build_ccp.sh`):
* each takes 26–42 s user (mean 31.7 s over the first 47) and 5.8–8.9 GB of
  RSS;
* the 267 state modules take 77 s in total.

**Totals.** Extrapolated: 266 × 32 s ≈ 2.4 CPU-hours of kernel time for the
whole compile phase. Wall time is about 2.4 h serial, or about 40 min with 4
parallel builds (each needs about 7 GB). Generated source: 16 MB, against the
≤ 500 MB budget. Measured: FULL_RUN_PLACEHOLDER.

**Table correctness.** `Table/T<k>.lean` has 64 entries per block. Each entry
gets its fetch facts by `decide` and its decode fact by `decodeW`, through
`ccp_entry`. A block takes about 2.6 s, about 15 min for all 305:
TABLE_PLACEHOLDER.

## P2: soundness, generic and once (`Tcc/Ccp/Sound.lean`, 2,204 lines)

* `Sim c ab s cf` relates an evaluator state `s` to a machine configuration
  `cf`. It covers:
  * the PC and the 31 GPR pins;
  * every known byte, present with its value; on bytes that the ELF does not
    load but the run reads (`ab`), only the total read is known;
  * the code table's bytes present;
  * an empty console, an idle mailbox, and the tick.
* `TableOK c` holds when every entry's word decodes, under Sail, to what the
  evaluator runs (`EntryOK`, `DecOK`).
* `exec_sound`, `step_sound` and `run_sound : TableOK c → Sim c ab s cf →
  run c n s = some t → Reaches c ab cf t`. A fault-free evaluator run is a
  Sail `Steps` run between related configurations.
* Covered: all 46 executed kinds:
  * `li`, `jal`, `jalr`;
  * the six branches;
  * `ld lw lwu lh lhu lbu`;
  * `sd sw sh sb`;
  * `addi add sub and or xor andi ori xori slli srli srai sll srl sra slt
    slti sltu sltiu`;
  * `addiw addw subw slliw srliw sraiw sllw srlw`;
  * `fence` (iorw,iorw).

  Shared machinery:
  * generic `mstep`, `alu_sound`, `load_sound`, `store_sound`, `core_sound`,
    `tstep`, `branch_sound`, `sim_link`;
  * `Nat`/`BitVec` bridges: packed fields via `testBit`, the signed order via
    the sign flip, arithmetic shifts via `ashr_compl`, and byte reassembly.
* `sim_cut`: a related state whose PC, GPRs and view bytes are the captured
  ones gives `CapturedCut`.
* `Tcc.Ccp.SumCut.compileCut : ResetSim_Statement → CompileCut_Statement`.
  The three cut checks against `st266` (PC, 31 GPRs, 6,689 view bytes) are
  kernel `decide`s; the view check takes 136 s.
* `#print axioms run_sound` gives `[propext, Classical.choice, Quot.sound]`;
  `run_100000` (the first four segments) gives `[propext]`.

## What remains

* `ResetSim_Statement`, which says the reset configuration is related to
  `st0`. It needs two things:
  * Port ship-your-ocaml's `ElfReset.ready` chain: `GoodState`, zero GPRs,
    the loader's memory, empty console, idle mailbox. That is 25 files,
    about 960 lines.
  * The correspondence between `mem0` and the `TccSumElf` pin. One way:
    build `mem0` by a proved function of `elfImage`'s chunks, instead of a
    second literal. A brute-force `decide` over 701k bytes against
    `elfImage`'s chunk lists is estimated at 10–20 kernel-minutes.
* After that, add `TccCompileCut` to an on-demand gate. It is about 3
  CPU-hours, so not `check.sh`'s default build.

## Reproduce

```
make -C c BUILD=build FLAGS=0 PROG=tests/p3/sum.c ELF=build/p3/sum-flags0.elf   # runtime 9e1142dc96d8db10
scripts/compile_trace.py --elf c/build/p3/sum-flags0.elf --out build/ccp/trace  # 6 min
scripts/gen_ccp.py --trace build/ccp/trace --out Tcc/Ccp/Sum --chain 25000:6632342 --table-proof
scripts/build_ccp.sh                                                             # ~2.5 h serial
```
