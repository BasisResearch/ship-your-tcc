# ship-your-tcc

Our C semantics builds on **[Cerberus](https://github.com/rems-project/cerberus),
developed by the Cambridge REMS team**, and
**[cerberus-lean](https://github.com/OathTech/cerberus-lean), the Lean port by
Mike Dodds / Oath**. Their semantics, port and validation machinery are the
foundation of the CSem work here. Our local changes and third-party licences
are recorded in [ATTRIBUTION.md](ATTRIBUTION.md); upstream candidates for
Kiran to relay are tracked in [CSEM-UPSTREAM.md](CSEM-UPSTREAM.md).

`tcc -run prog.c` on the executable Sail RISC-V model, a three-way
differential of tcc against CompCert's interpreter and Certora's Lean port of
Clight, and (Phase 3) per-program end-to-end theorems in Lean that tcc's
output on Sail produces exactly the outputs Certora's Clight semantics gives
the program. Plan: BRIEF.md. Ledger and exit criteria: PHASES.md. Discipline:
CLAUDE.md. Provenance: ATTRIBUTION.md.

The approved target is now whole-compiler correctness: `tcc -run` interprets
C source through compilation and execution. DESIGN-COMPILER.md specifies
`tcc_run_correct` against CSem, with `Supported` and `Fits` premises and no
refusal disjunct. The existing per-program theorem is its non-vacuity witness;
STATUS.md records the active F0–F3 work.

Current F0–F3 validation: exact phase-4 observation and nested designators are
integrated; the frozen broad compiler comparison records 1,345 matching arenas out of
1,815 inputs, with 463 refusals and seven assembly gaps separate. The shared
fixture repair recovers three former refusals, each matching a complete native
Sail arena (results/model/corpus-fixtures-2026-10-28.md). Native observations
also include the repaired 78-case anonymous-member batch. CSem now uses pure binary32/64/128 values, with
zero native floating in its checked cones. Its anonymous-member repair passes
92 focused checks and the combined full gate. C11 bitfield representation and
single-evaluation compound lowering are now adopted after the full combined
gate: 82 required semantic checks and a fresh 80-case native batch pass. GNU
member attributes, enum bitfield policy, aggregate-value initialization and
concrete memory/volatile-event transport remain open
([bitfield integration](results/csem/bitfields-integration-2026-10-28.md)). The raw-source frontend reaches
positioned Cabs with retained provenance; its 18 independent comparisons and
combined gate pass. Runtime-library identity now reaches the semantic consumers
with preserved C-visible labels: all 24 checks and 5,580 consumer probes pass
after the allocation/symbol-supply integration. Reserving symbol zero repairs
the two earlier pointer failures (results/csem/function-redeclaration.md). Pack/layout and transport proofs remain open. F1–F3 and the whole-compiler theorem
remain open; newlib binary128 formatting/sqrt and other
recorded semantic gaps still require work. The runtime allocation oracle is
adopted after the combined gate: arbitrary checked placements enter CSem's
outcome set, with generic placement contracts and 21 focused checks. The
machine-memory simulation remains open (results/csem/allocation-oracle.md).

Compiler-action Cabs now agrees with independent raw preprocessing/parsing on
50/56 fixtures. All 18 compiler-injected opening controls and 58 physical-origin
controls pass, with complete header aliases and user asm exclusions preserved.
The combined gate, fifteen erasure pairs and eighteen independent fast arenas
pass; a new complete native Sail arena matches the static/builtin memcpy
shadowing control. Runtime coverage is still five bodies, fifteen missing targets and
one alloca intrinsic duty. The local universal replay-erasure theorem is proved
and adopted after the combined gate; eleven ghost contracts and the four new compiler-environment/binding
contracts remain open
([binding integration](results/model/binding-integration-2026-10-28.md),
[local replay proof](results/model/replay-integration-2026-10-28.md)).
Generic/static-assert compiler actions retain selected and unselected syntax.
Assertion messages now accept all five C11 literal encodings: 65 complete Cabs
controls agree in all three scopes, 64 new fast arenas match, and 61 diagnostic
refusals remain separate. The worker full CSem gate and fresh combined default
gate pass on identical semantic inputs. A fresh complete 4 MiB native assertion
arena and separate successful full program run validate the rebuilt image
([assertion integration](results/model/assertion-messages-integration-2026-10-28.md)).
The original generic suite now has 22 valid controls, 11 invalid controls and
one separate no-message extension; its historical wide-message refusal is retained.
GNU conditional/typeof/choose construction is adopted: 50 exact controls in
three scopes and six complete native arena comparisons pass with the combined
gate. Separate raw semantic probes retain GNU conditional lowering and VLA
failures; their repair tasks remain open
([GNU integration](results/model/gnu-expression-integration-2026-10-28.md)).
GNU declaration syntax/layout has 23 semantic controls and six actual-header
parses. Compiler typedef alignment is now
repaired: three full compiler Sail probes succeed and two new complete native
arenas match the model. The combined gate passes, with 46/56 exact raw Cabs
controls on model-produced positioned tokens; the representation proof and remaining gaps stay open
(results/model/typedef-integration-2026-10-28.md). The C generator's unary-minus
lexing defect is repaired: a separate fresh 1,200-input batch has 1,200 identical
arenas and no refusals, with the full gate passing. Historical corpus counts
remain unchanged (results/model/cgen-negation-2026-10-28.md). An explicit upstream
two-file inline invocation now matches a complete native Sail arena, with its
separate standalone refusals retained; multi-unit ghost provenance stays open
(results/model/corpus-invocations.md).

CSem typedef alignment and early array constraints are now repaired through
shared Lem typing and a distinct GNU expression-alignof constructor. The
combined gate and 71-case matrix pass, keeping 53 defined agreements separate
from invalid constraints, function-policy/parser gaps and the then-open
array-decay compiler discrepancy. The three original wrong results have independent full
compiler and cross-GCC Sail observations; the compiler decay repair is now adopted
([alignment integration](results/model/alignment-integration-2026-10-28.md)).

The array-decay repair passes the combined gate, 210 positive/27 negative C11
checks and 29 successful focused fast arenas. Three dedicated native cases
and the inherited generic batch each match all 4 MiB and execute to zero on
the rebuilt compiler image. Alignment now has 54 defined agreements; the
original wrong result remains retained separately
([array integration](results/model/array-integration-2026-10-28.md)).

The latest preprocessing corpus has 618 exact model/reference and 616 exact
spec/reference agreements across 622 inputs. The image's opened-path once
cache and include fixtures now agree; residuals and full gate/Sail evidence
are recorded in [the preprocessing review](results/pre/once-cache-2026-10-28.md).

## The target: RV64I

The shared executable Sail model implements RV64I only. `mul`, `div`, atomics,
F, D and `fence.i` all trap as illegal instructions (`results/isa-probes.json`,
`scripts/probe_sail.py`). So:

* tcc's riscv64 backend is patched to emit only RV64I and to call libgcc
  helpers for multiply, divide, remainder, float, double and long double, under
  the lp64 soft-float ABI (`patches/0001-tcc-rv64i-soft.patch`, 335 lines, all
  under `TCC_RV64I_SOFT`; without the macro the patched tcc emits
  byte-identical objects, `scripts/check_patch_neutral.sh`). Design and helper
  list: `results/tcc-patch.md`.
* tcc itself is built like ship-your-lua's ELF: xPack GCC 15.2.0 + newlib 4.5.0,
  `-march=rv64i -mabi=lp64 -mcmodel=medany -O2`.
* No `fence.i` after relocation, unlike the brief: the model traps on it and
  has no instruction cache. The post-relocation sync is a plain `fence`.
* `scripts/check_isa.sh` fails on any M, F, D, Q, A or C instruction. Initial
  checks covered 146 tcc objects and `libtcc1.a`. The corrected image has
  107,042 instructions; its image checks and every Phase 1 generated-code
  check pass, with no ecall or fence.i.

## Phase 1: `tcc -run` on Sail (done)

`c/Makefile` builds `c/tcc-riscv-htif.elf`: tcc (43c7708 + `patches/`) with
newlib and the HTIF console. Its in-image read-only file system (154 files,
7.35 MB) holds tcc's and newlib's headers, `libc.a`, `libm.a`, `libgcc.a`,
`libtcc1.a`, `runmain.o` and `/prog.c`. The file system is ship-your-ocaml's,
in a fixed-address `.embed` region. Symbols resolve through tcc's archive
loader, with no dlsym; only newlib's system-call layer is supplied by
`tcc_add_symbol`. Generated code goes in a static 4 MiB arena at 0x86400000.
Everything except `.embed` is byte-identical for every program (runtime hash
`9e1142dc96d8db10` for the corrected runtime).

All seven programs were rerun on 2026-10-20 after fixing syscall errno
propagation. The earlier measurements are retained in
`results/review/phase1-before-runtime-fix.tsv`.

| program | result | compile-phase steps | program steps | total steps |
|---|---|---|---|---|
| hello | match | 11,278,132 | 2,358 | 11,297,870 |
| loops | match (exit 6) | 11,458,907 | 14,190 | 11,504,582 |
| structs | match | 12,004,798 | 10,740 | 12,052,526 |
| fnptr | match | 12,520,169 | 17,471 | 12,578,737 |
| printf | match (exit 3) | 12,391,341 | 41,160 | 12,461,079 |
| doubles | match | 11,856,735 | 60,082 | 11,952,315 |
| headers (18 standard headers) | match | 18,874,501 | 38,207 | 18,966,251 |

* "match" means stdout and exit code are identical to the same program built
  by the host cc (`c/tests/run.sh`, `results/phase1.tsv`, `results/phase1/`).
* The compile phase runs from `_start` to `tcc_baremetal_run`. The program
  phase runs from `tcc_baremetal_enter`, the jump into generated code, to the
  tohost store in `_exit`. Total also counts the optional code dump between
  the two.
* The emulator runs at about 30–47k steps/s, so hello takes about 6 minutes.
  The three-job rerun took 431 seconds for hello on the shared box.
* The syscall regression now returns `ENOENT` for a missing file (previously
  wrong errno): 11,655,557 compile steps and 4,000 program steps. The raw
  newlib syscall errno slot is shared; each libc retains its own `_reent`.
* Where hello's compile phase goes: 50% compiling (mostly lexing the stdio.h
  header chain), 36% loading about 87 libc/libgcc archive members, 11%
  relocation, 2% init. Profiling cut it from 24.5M to 11.3M steps; details in
  `results/phase1/profile-hello.txt`.

Codegen test corpus (`c/tests/tcc/`, `scripts/test_tcc_patch.sh`): 43/43
programs give the same stdout and exit code as xPack gcc -O0 rv64i on Sail and
as host gcc. tcc's tests2: 101/104 match `.expect`; none of the three failures
comes from the code generator (`results/tcc-patch.md`).

## Phase 2: three-way differential (done)

Oracles (`scripts/oracles.toml`, runner `scripts/diff3.py`):

| oracle | what | script |
|---|---|---|
| `tcc_sail` | `tcc -run /prog.c` on Sail | `scripts/run_tcc_sail.sh` |
| `lean_clight` | Certora's Lean Clight interpreter (`doStep`) on `clightgen -lean`, plus an external-call model for printf/putchar/puts/exit; printf is `Tcc.Clight.format`, the pure formatter Phase 3 assumes | `scripts/run_lean_clight.sh` |
| `ccomp_interp` | Certora/clean `ccomp -interp` (CompCert 3.17) | `scripts/run_ccomp_interp.sh` |
| `gcc_native` | host gcc -O0 -funsigned-char (sanity check) | `scripts/run_gcc_native.sh` |
| `xgcc_sail` | xPack gcc -O0 rv64i on Sail (same newlib printf as tcc) | `scripts/run_xgcc_sail.sh` |

* Certora/clean is at 123f7f7, configured `rv64-linux` against the xPack
  newlib headers (`results/certora.md`).
* Corpus: 200 Csmith programs (common subset, UB-checked; `results/corpus.md`),
  72 CompCert test programs and 73 tcc tests2 programs.
* All five oracles agree on all 200 Csmith programs. Across the full corpus,
  301 programs finish on all three primary oracles: 298 agree and three
  disagree. The other 44 are outside the common executed subset: the
  CompCert interpreters get stuck on or reject them.
* tcc/Sail completes all 345 programs, including the nine that need libm,
  and matches native gcc on 344 of them. The one mismatch is the union
  initializer below. Median compile phase: 8.6M steps (max 21.4M). Median
  program phase: 52k steps (max 15.0M). The run used 40.9 emulator-hours in
  total.
* Two Lean-port discrepancies have small reproducers in `results/repro/`:
  * converting a 64-bit integer to `float` rounds twice, through double;
  * converting `double` to `unsigned long` saturates as if signed:
    `(unsigned long)1e19` gives 9223372036854775807.

* One TinyCC union-designator discrepancy also reproduces on pristine
  native TinyCC; it is outside the new RV64I backend.

All numbers come from one complete rerun on 2026-10-20, on a single
harness version and the corrected runtime. Counts, exclusions, harness
fixes and reproducers are in [DIFFERENTIAL.md](DIFFERENTIAL.md).

## Phase 3: one per-program end-to-end theorem

The program P is `c/tests/p3/sum.c`, a loop and a printf that prints `55\n`.
For P the end-to-end theorem is proved with no machine premise:
`Tcc.EndToEnd.tcc_run_sum_closed` (in the `TccCompileCut` library) says that,
for every external-call semantics with the printf premises, `tcc -run sum.c`
from reset on Sail halts printing exactly what Certora's Lean Clight of `sum.c`
prints, with the same exit code. `Tcc.EndToEnd.tcc_run_sum_derived` has no
premise at all: there the external-call semantics is `sumX`, whose printf is
read off the image (see "printf" below). The two sections below cover the Clight
side and the machine side. Each obligation's status is in PHASES.md. The
earlier proof audit is in
[results/proof-readiness.md](results/proof-readiness.md), and the
independent native capture of the handoff state (6,632,342 steps) is in
[results/capture-cut.md](results/capture-cut.md).

`CompileCut_Statement`, which says tcc's compile phase on Sail reaches the
captured cut, is proved as `Tcc.Ccp.Sum.compileCut_proved` in the
`TccCompileCut` library. The proof has five parts:

* a first-order RV64I evaluator (`Tcc/Ccp/Eval.lean`) runs the whole
  compile phase inside the kernel, in 266 generated segments of 25,000 steps
  each, at 1.24 ms per step: 2.3 CPU-hours (`scripts/build_ccp.sh`, or
  `scripts/check.sh --compile-cut`), from 16 MB of generated source;
* one generic theorem, `Tcc.Ccp.run_sound`, turns an evaluator run into
  Sail `Steps`. It is proved for every instruction kind the run executes;
* a generated proof that the code table decodes, under Sail, as the
  evaluator runs it;
* generated proofs that the evaluator's reset memory and code are exactly
  the bytes `TccSumElf` pins. The pin is now a search tree of chunks
  (`Tcc/Boot/ChunkTree.lean`);
* the reset facts, from ship-your-ocaml's reset chain, ported into
  `Tcc/Boot/Startup`.

The generated files are checked without the native trace:
`scripts/gen_ccp.py --check` regenerates them from the committed digest
`results/p3/ccp-digest.json`, and checks the checkpoint states by hash.

Measurements and design are in
[results/p3/compile-cut-pilot.md](results/p3/compile-cut-pilot.md).

### Clight side (done)

For P = `c/tests/p3/sum.c`, `Tcc.Prog.Sum.prints` proves, for every
external-call semantics `X` with the printf premise `PrintfModel X` and
`ExtDeterm X`, that `clightgen(P)` terminates printing "55\n" with exit code 0
and that every terminating run does the same. Certora's Lean Clight is
vendored pristine (`vendor/certora-export`) and patched so that external calls
are a parameter instead of axioms (`patches/certora/0002-cclib-extsem-param.patch`,
built from the generated `vendor/certora-export-patched`); printf is a pure
Lean formatter (`Tcc/Clight/Printf.lean`). All on Lean 4.34.0 with the root
`lakefile.toml`; `lake build` takes about 75 s from scratch (the run of
P is one 21 s `decide +kernel`), under 1.2 GB.

### Machine side (3a and 3b proved for P)

`Tcc.EndToEnd.tcc_run_sum`: for every external-call semantics `X` with
`PrintfModel X` and `ExtDeterm X`, **given `CompileCut_Statement`**, from
reset on the tcc image with P embedded, the machine halts with console
`"55\n"` and exit code 0, Clight's P prints those bytes and returns 0, and the
codes agree (`Agree`); every halting run of the machine is that one
(`tcc_run_sum_halts`). `tcc_run_sum_closed` and
`tcc_run_sum_halts_closed` discharge the premise with `compileCut_proved`.

* **The cut** (`Tcc.Capture.Sum.TccSumCut`): PC = `tcc_baremetal_enter`, every
  GPR as captured, `GoodState`, an idle HTIF mailbox, an empty console, and a
  partial view of the 6,689 bytes the 2,470-step program phase reads before
  writing them (660 bytes of host code, checked equal to the ELF's loaded bytes
  by one kernel `decide` per chunk; 6,029 bytes the compile phase stored: P's
  code, tcc's relocated libc members and data, which are observations). From
  one `--trace-all` run of the emulator on the `FLAGS=0` image
  (`scripts/tcc_cut.py`, 7 minutes; memory reconstructed from the ELF and every
  store, checked against every load) and `scripts/gen_tcc_cut.py`; the
  independent native capture (`results/p3/sum-cut/`) agrees on the step count,
  all 31 GPRs and the control registers. `TccSumElf` pins the ELF by its entry,
  `.tohost` and the 701,193 bytes the compile phase reads (`Tcc/Capture/Sum/Elf/`,
  1.5 MB of generated data, elaborated only).
* **The program phase** (`Tcc.Prog.Sum.Machine.sum_halts : SumHalts_Statement`):
  a generated replay of all 2,470 steps (`scripts/gen_tcc_segs.py`,
  `Tcc/Prog/Sum/Machine/`): 877 items between rows `RunRow` (GPR pins, the
  program phase's write log over the cut's view, the console), straight-line
  blocks (at most 6 instructions, 28 byte pins each) by one `#derive_case` per static block and `Tcc.Boot.row_step`, calls
  (`row_jalr`, `row_jal`), `sltu`, a nop, the three console stores
  (`row_putchar`) and the exit store (`row_exit`) by the one-instruction rules of
  `Tcc/Boot/Step.lean`, every side condition a kernel `decide`. Building it
  takes about 20 minutes in batches of four parts (`scripts/build_replay.sh`,
  1–3 GB per Lean process).
* **The machine layer** is ship-your-lua's `Vsa/` (180 modules, the import
  closure of the segment layer) on its `riscv-lean/`, generalised for code
  above `.tohost`: a fetch needs `FetchOK` (RAM, off the HTIF mailbox) instead of
  `pc + 4 ≤ tohostAddr`, a store `StoreWin` instead of `tohostAddr + 16 ≤ a`, and
  code pins are on the memory current at each instruction (20 modified files,
  `scripts/port_closure.py` `MODIFIED`).

### printf (PHASES `PrintfSpec_Statement`)

P's printf is newlib's, relocated by tcc into its arena (`printf` at
`0x86400148`, `_vfprintf_r` 3,493 instructions). The inventory
([results/p3/printf-inventory.md](results/p3/printf-inventory.md),
`scripts/printf_inventory.py`) places every loaded member by its bytes: P's
one call runs 1,657 steps over 1,251 distinct instructions. None of
ship-your-lua's stdio proofs apply (GNU ld relaxes calls there, tcc does not;
only six leaf routines are word-identical), and its printf proofs cover one
format each. stdout is line buffered in this image, so a call's observable is
the console followed by `stdout`'s pending buffer (`stdoutBytes`), not the
console.

* **Stated, open**: `Tcc.Printf.PrintfSpec_Statement` (`Tcc/Printf/Spec.lean`),
  general over format strings and arguments of the integer subset, for the
  relocated code at its arena addresses (`Tcc/Printf/Sum/Layout.lean`,
  generated by `scripts/gen_printf_layout.py` from the arena at the cut,
  `results/p3/sum-arena.tsv`): the call returns `a0` = the output's length,
  keeps `sp` and the callee-saved registers, extends `stdoutBytes` by exactly
  `Tcc.Clight.format`, and writes only its footprint. Floats are outside it.
* **Proved**: P's call is such a call (`Tcc.Printf.Sum.call0`, from the
  replay rows, `Calls.lean` from `scripts/gen_tcc_segs.py`). `Tcc.Printf.machX`
  is an external-call semantics whose printf steps exactly on the calls an
  image makes, with the image's return value and output; it is deterministic
  (`machX_extDeterm`) and total on those calls (`machX_model`), and the open
  spec would make it answer every call the image reaches (`imgMakes_of_spec`).
  At `sumX = machX` for this image, `tcc_run_sum_derived` closes P with no
  premise.
* **Tested**: `scripts/printf_matrix.py` compares `format` with the arena's
  newlib case by case: of 7,490 integer-subset formats (every flag set, widths,
  precisions, every length modifier, extreme values), `format` is defined on
  5,996, and all 5,996 agree in bytes and return value under `tcc -run` on
  Sail (186.7M steps) and under xgcc on Sail
  ([results/p3/printf-matrix.md](results/p3/printf-matrix.md)).

**Reproducing the image.** The runtime's bytes depend on the build directory:
tcc's sources are compiled from `$(BUILD)/tcc-src`, and the `__FILE__` strings
of its assertions land in `.rodata`, moving every later address (`.tohost`
included). `make -C c BUILD=build FLAGS=0 PROG=tests/p3/sum.c ELF=…` (the
relative `build`, as `c/tests/run.sh` and `scripts/run_tcc_sail.sh` use)
reproduces runtime hash `9e1142dc96d8db10` and `.tohost = 0x8007cec0`; an
absolute `BUILD` path gives a different image. `scripts/tcc_cut.py` refuses any
other runtime hash.

## Next: compiler correctness (plan)

[DESIGN-COMPILER.md](DESIGN-COMPILER.md) plans the general theorem. For
every C source P with no undefined behaviour under CSem, `tcc -run` on Sail
behaves as one of CSem(P)'s allowed outcomes. CSem is our fork of
cerberus-lean (Cabs → Ail → Core in Lean), with a Lean C grammar,
preprocessing semantics and tcc's RV64 lp64 parameters. The proof has two
layers: tcc's binary computes a Lean model of tcc (Layer A), and that model's
code simulates Core (Layer B). Certora's Clight and CompCert become testing
oracles. The plan is approved; PHASES.md (Phase 4) is the ledger.

**F0 (done): the fork in place.** `csem/` holds cerberus-lean @e7e9bb4 with
LemLib @2d3a492, its Lean generated from Lem and committed
(`scripts/csem_regen.sh --check` reproduces it), ported to Lean 4.34.0 by
changing three proofs (`csem/PORT-4.34.md`). The CSem libraries build in the
root project in 379 s (646 CPU-s, 3.3 GB peak) and pass the fork's own gates
(`scripts/check.sh --csem`). On the fork's minimal corpus CSem agrees with the
Cerberus OCaml oracle exactly as the fork's baseline records (90 MATCH,
18 UB_MATCH, 5 oracle skips). `scripts/csem_run.sh [--libc] prog.c` is the CSem
oracle; it gives `sum.c` the outcome `55\n`, exit 0. `Tcc/CSem/Statement.lean`
states `TccRunCorrect_Statement` with the headline shape over the fork's
current pipeline; its front end, resource limits and image predicate are still
parameters owed by F1–F6 (results/csem/f0-validation.md).

F2 now exposes `TccModel.preprocess` as exact phase-4 tokens, with a dedicated
internal-token observer independent of `-E`. The focused run has 41 exact raw
matches and 8 joint refusals; the 618-input corpus has 612 exact raw matches
(including all 200 Csmith inputs) and 6 joint refusals. All 15 protocol/golden
tests pass. Spec residuals, printer encoding classifications and open typed
connections are retained in [the token-stream report](results/pre/token-stream.md);
this does not prove the preprocessing theorem.

## Layout
* `vendor/tinycc`: pristine tinycc 43c7708.
* `patches/`: the tcc patches.
* `c/`: the bare-metal image and its tests.
* `scripts/`: builds, oracles, checks.
* `corpus/`: Phase 2 programs.
* `lean/ClightRun`: the Lean Clight runner (Phase 2 oracle), built by the root project (`lake build clight_run`).
* `vendor/certora-export`, `vendor/certora-export-patched`: Certora's CCLib,
  pristine and patched (generated by `scripts/cclib_src.sh`).
* `Tcc/`: Phase 3 Lean (`Clight/`: printf, runner, premises; `Prog/Sum/`: P;
  `Audit/`: the patch audit; `Boot/`: the cut interface, the replay rows and
  the one-instruction rules; `Capture/Sum/`: P's captured cut, generated;
  `Prog/Sum/Machine/`: the replay of P's program phase, generated;
  `EndToEnd/`: the end-to-end theorem; `Printf/`: printf's machine spec, the
  printf read off the image, P's call).
* `Vsa/`, `riscv-lean/`: the machine layer and the Sail model, copied from
  ship-your-lua.
* `csem/`: CSem, our fork of cerberus-lean (`csem/PORT-4.34.md`);
  `Tcc/CSem/`: the F0 statement over it.
* `results/`: measurements.
* `abstractions/`: the abstraction gate.
* `scripts/check.sh`: the gate.

The adopted 2026-10-27 repin (Cerberus 704c4e69 / Lem e6d8db56, Lean 4.34) implements C11 union
selected-member initialization in the shared Lem source. Patched OCaml and
regenerated Lean match GCC on 34 focused checks (33 conforming C11 cases and
one inherited excess-initializer control); all 35 focused parser inputs agree.
Fresh patched tcc/Sail passes the existing union test and original override
reproducer. The full gates and combined conforming Sail suite are recorded in
[the union repair report](results/csem/union-init-2026-10-27.md), which also
lists the remaining anonymous-layout, aggregate-expression and inactive-tail
gaps. A valid nested-continuation case remains a separate tcc refusal,
recorded as a compiler/model repair obligation. Lead review and the integrated full gate pass; Supported/Fits and the frozen
sum cuts are unchanged.

The current exact-token preprocessing/designator integration passes the
strengthened gate and 147 positive/14 invalid C11 checks. Fresh comparisons
have 1,339 independent fast arenas and three complete native Sail arena
matches; refusals and assembly gaps stay separate. See
[the integration report](results/pre/combined-integration-2026-10-27.md).
Preprocessing correctness and binary refinement remain open obligations.

2026-10-27 IEEE recovery: CSem's floating execution now uses pure configured
binary32/64/128 values and exact object bytes. The semantic kernel/runtime
cones contain zero native floating constants; the 113-program baseline is
unchanged. Fresh arithmetic, literal, printf and Sail evidence, with the
retained target literal-accuracy, binary128 printf and sqrtl discrepancies,
is in [the recovery report](results/csem/ieee-2026-10-27.md). Image/library and
conversion-field proof connections remain explicit obligations in PHASES.md.

The direct source-literal repair extends patch 0004 to binary32/64/128 decimal
and hexadecimal conversion. Exact-rational boundary and stress fixtures,
independent arena checks and retained native Sail evidence are in
[results/model/direct-float-fixed-2026-10-27.md](results/model/direct-float-fixed-2026-10-27.md).
The original failing image remains documented separately; the compiler/model
literal refinement theorem and public libc conversions remain open.
