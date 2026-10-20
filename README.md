# ship-your-tcc

`tcc -run prog.c` on the executable Sail RISC-V model, a three-way
differential of tcc against CompCert's interpreter and Certora's Lean port of
Clight, and (Phase 3) per-program end-to-end theorems in Lean that tcc's
output on Sail produces exactly the outputs Certora's Clight semantics gives
the program. Plan: BRIEF.md. Ledger and exit criteria: PHASES.md. Discipline:
CLAUDE.md. Provenance: ATTRIBUTION.md.

TinyCC is a compiler, not an interpreter, so the reachable theorem is per
program (translation validation), not for all programs.

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
* `scripts/check_isa.sh` fails on any M, F, D, Q, A or C instruction. Clean on
  146 tcc objects, `libtcc1.a`, the tcc image (107,059 instructions; no ecall,
  no fence.i) and the generated code of every Phase 1 program.

## Phase 1: `tcc -run` on Sail (done)

`c/Makefile` builds `c/tcc-riscv-htif.elf`: tcc (43c7708 + `patches/`) with
newlib and the HTIF console. Its in-image read-only file system (154 files,
7.35 MB) holds tcc's and newlib's headers, `libc.a`, `libm.a`, `libgcc.a`,
`libtcc1.a`, `runmain.o` and `/prog.c`. The file system is ship-your-ocaml's,
in a fixed-address `.embed` region. Symbols resolve through tcc's archive
loader, with no dlsym; only newlib's system-call layer is supplied by
`tcc_add_symbol`. Generated code goes in a static 4 MiB arena at 0x86400000.
Everything except `.embed` is byte-identical for every program (runtime hash
`ed7b4811aaf2d9d0`).

| program | result | compile-phase steps | program steps | total steps |
|---|---|---|---|---|
| hello | match | 11,302,936 | 2,362 | 11,322,678 |
| loops | match (exit 6) | 11,482,402 | 14,194 | 11,528,081 |
| structs | match | 12,034,957 | 10,744 | 12,082,689 |
| fnptr | match | 12,545,182 | 17,475 | 12,603,754 |
| printf | match (exit 3) | 12,456,606 | 41,164 | 12,526,348 |
| doubles | match | 11,881,662 | 60,086 | 11,977,246 |
| headers (18 standard headers) | match | 18,898,002 | 38,211 | 18,989,756 |

* "match" means stdout and exit code are identical to the same program built
  by the host cc (`c/tests/run.sh`, `results/phase1.tsv`, `results/phase1/`).
* The compile phase runs from `_start` to `tcc_baremetal_run`. The program
  phase runs from `tcc_baremetal_enter`, the jump into generated code, to the
  tohost store in `_exit`. Total also counts the optional code dump between
  the two.
* The emulator runs at about 30–47k steps/s, so hello takes about 6 minutes.
* Where hello's compile phase goes: 50% compiling (mostly lexing the stdio.h
  header chain), 36% loading about 87 libc/libgcc archive members, 11%
  relocation, 2% init. Profiling cut it from 24.5M to 11.3M steps; details in
  `results/phase1/profile-hello.txt`.

Codegen test corpus (`c/tests/tcc/`, `scripts/test_tcc_patch.sh`): 43/43
programs give the same stdout and exit code as xPack gcc -O0 rv64i on Sail and
as host gcc. tcc's tests2: 101/104 match `.expect`; none of the three failures
comes from the code generator (`results/tcc-patch.md`).

## Phase 2: three-way differential (in progress)

Oracles (`scripts/oracles.toml`, runner `scripts/diff3.py`):

| oracle | what | script |
|---|---|---|
| `tcc_sail` | `tcc -run /prog.c` on Sail | `scripts/run_tcc_sail.sh` |
| `lean_clight` | Certora's Lean Clight interpreter (`doStep`) on `clightgen -lean`, plus an external-call model for printf/putchar/puts/exit | `scripts/run_lean_clight.sh` |
| `ccomp_interp` | Certora/clean `ccomp -interp` (CompCert 3.17) | `scripts/run_ccomp_interp.sh` |
| `gcc_native` | host gcc -O0 -funsigned-char (sanity check) | `scripts/run_gcc_native.sh` |
| `xgcc_sail` | xPack gcc -O0 rv64i on Sail (same newlib printf as tcc) | `scripts/run_xgcc_sail.sh` |

* Certora/clean is at 123f7f7, configured `rv64-linux` against the xPack
  newlib headers (`results/certora.md`).
* Corpus: 200 Csmith programs (common subset, UB-checked; `results/corpus.md`),
  72 CompCert test programs and 73 tcc tests2 programs.
* Before tcc was plugged in, all four other oracles agreed on all 200 Csmith
  programs. Two Lean-port bugs turned up, reduced to UBSan-clean reproducers
  in `results/repro/`:
  * converting a 64-bit integer to `float` rounds twice, through double;
  * converting `double` to `unsigned long` saturates as if signed:
    `(unsigned long)1e19` gives 9223372036854775807.

The results go in DIFFERENTIAL.md.

## Phase 3: per-program theorem (Clight side done)

For P = `c/tests/p3/sum.c`, `Tcc.Prog.Sum.prints` proves, for every
external-call semantics `X` with the printf premise `PrintfModel X` and
`ExtDeterm X`, that `clightgen(P)` terminates printing "55\n" with exit code 0
and that every terminating run does the same. Certora's Lean Clight is
vendored pristine (`vendor/certora-export`) and patched so that external calls
are a parameter instead of axioms (`patches/certora/0002-cclib-extsem-param.patch`,
built from the generated `vendor/certora-export-patched`); printf is a pure
Lean formatter (`Tcc/Clight/Printf.lean`). All on Lean 4.34.0 with the root
`lakefile.toml`; `lake build` takes about 75 s from scratch (the run of
P is one 21 s `decide +kernel`), under 1.2 GB. The machine side (3a, 3b) is
open: PHASES.md.

## Layout
* `vendor/tinycc`: pristine tinycc 43c7708.
* `patches/`: the tcc patches.
* `c/`: the bare-metal image and its tests.
* `scripts/`: builds, oracles, checks.
* `corpus/`: Phase 2 programs.
* `lean/ClightRun`: the Lean Clight runner (Phase 2 oracle, Lean 4.32.1).
* `vendor/certora-export`, `vendor/certora-export-patched`: Certora's CCLib,
  pristine and patched (generated by `scripts/cclib_src.sh`).
* `Tcc/`: Phase 3 Lean (`Clight/`: printf, runner, premises; `Prog/Sum/`: P;
  `Audit/`: the patch audit).
* `results/`: measurements.
* `abstractions/`: the abstraction gate.
* `scripts/check.sh`: the gate.
