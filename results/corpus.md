# Phase 2 corpus and differential harness

Status 2026-10-13: 345 programs (Csmith 200, CompCert 72, tcc 73). Four of the
five oracles run: gcc_native, xgcc_sail, ccomp_interp and lean_clight.
tcc_sail is a placeholder until lane B's `scripts/run_tcc_sail.sh` lands.
All four agree on every Csmith program. Two disagreements remain, and both
are bugs in the Lean port (see "Results").

## Corpus

| source | candidates | included | where |
|---|---|---|---|
| Csmith 0cdc710 (`scripts/CSMITH_REV`) | 336 seeds | 200 | `corpus/csmith/*.c`, committed (2.3 MB), with `manifest.tsv`, `rejected.tsv`, `GENERATOR` |
| CompCert-small-tests 081175d (the `test/` pin of CompCert v3.17): `c/` benchmarks + `regression/` | 24 + 106 | 72 (23 + 49) | `corpus/compcert/manifest.tsv` committed; the `.c` files are copied by `scripts/collect_corpus.sh` |
| tinycc 43c7708 `tests/tests2/*.c` + `tests/tcctest.c` | 140 + 1 | 73 | `corpus/tcc/manifest.tsv` committed; the `.c` files are copied likewise |

Every CompCert/tcc candidate has a manifest row with these columns: status,
reason, shrink edit, reference output, the library functions it references
(`externals`), host x86-64 instruction count (`native_kinstr`) and line
count. To regenerate everything:
`scripts/collect_corpus.sh --refresh; scripts/gen_csmith.sh 200 corpus/csmith`.

### Exclusions (manifest `reason` column)

**CompCert (58 excluded):**
- 26 have no `main` (compile-only tests).
- 9 use builtins (8 GCC, 1 `__builtin_annot`).
- 7 use `volatile`, which the Lean port doesn't cover.
- 3 print with `%a` (outside the Lean printf model).
- 2 use OS headers (`uchar.h`).
- 5 are attribute, section or alignment tests that upstream only compiles.
- 1 reads a file (`knucleotide`).
- 1 has processor-dependent NaN payloads (`NaNs`).
- 1 is a known CompCert failure (`funct1`).
- 1 is rejected by gcc (`packedstruct2`).
- 1 gives gcc output that differs from the reference (`parsing`).
- 1 uses extended asm.

**tcc (68 excluded):**
- Toolchain-specific tests: bounds checker, `-dt` snippets, two translation units, DLLs, TLS, arm64/i386/x86/riscv asm, Windows, `-mms-bitfields`. These are the upstream SKIP/NORUN lists plus the platform-specific ones.
- 5 use long double, which CompCert rejects; tcc on RV64 uses binary128.
- 4 use `typeof` and 4 use statement expressions; CompCert rejects both.
- 4 use VLAs, 3 atomics, 2 packed structs, 2 `volatile`, 1 inline asm, 1 cleanup attribute, 1 computed goto.
- 3 need argv, stdin or local includes.
- 1 has `void main` (`137`).
- `tcctest.c` needs tcc's build tree (config.h, tcclib.h).
- `t_27_sizeof` passes `size_t` to `%d`. That is undefined behaviour, and diff3 found it: ccomp_interp prints `<int argument expected>`.

The full lists of patterns are `EXCLUDE_PATTERNS` and `MANUAL_EXCLUDE` in
`scripts/collect_corpus.py`.

**argc/argv.** `ccomp -interp` calls `main(0, NULL)`. gcc_native and the
Sail harness pass `argc=1, argv={"prog"}`. A program is kept only if it
can't tell these apart: no `argv[0]`, and no test of argc against 0 or 1.

**Size shrinking.** 25 CompCert programs were too slow for Sail at their
default size, which is 0.1 to 2 billion native instructions. The cutoff is
5M native instructions; Sail runs at about 45k steps/s. Each of these
programs is edited at copy time, for example `fib` n 35→15, `nbody`
1000000→20 steps, `sha3` DATALEN 100000→1000, `int64` 100→20 random cases,
`casts3` 1000000→1000. The edit is recorded in the `shrink` column. The
reference output is then not compared; diff3 compares the oracles with each
other.

The shrink of `bisect` needs a second edit: main prints `S[1..19]`, so
shrinking only n (500→8) made it read past the array (UB). diff3 showed this
as a gcc_native/xgcc_sail disagreement, and the print loop now shrinks too.

### Common-subset coverage

ccomp -interp implements only printf (plus malloc/free and builtin memcpy).
The Lean runner also implements putchar, puts and exit. So most CompCert
benchmarks get stuck in both semantics at the first libm or string call
(`sqrt`, `memset`, `strlen`, `calloc`, ...). Bitfield-local programs get
stuck in both as well. In those cases the outcome is `stuck` or `n/a`, never
a disagreement.

| corpus | all four oracles ok | ccomp stuck/n-a | Lean stuck/n-a |
|---|---|---|---|
| csmith | 200 / 200 | 0 | 0 |
| compcert | 44 / 72 | 26 / 1 | 26 / 2 |
| tcc | 57 / 73 | 6 / 10 | 6 / 10 |

## Csmith configuration

`scripts/gen_csmith.sh N OUTDIR` runs `scripts/csmith_one.sh` for each seed.
Seeds start at `BASE_SEED` (default 1) and run as parallel batches. It
keeps the first N seeds that pass, so the output is deterministic.

Flags:
```
--ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct
--no-bitfields --no-builtins --no-inline-function --concise
--max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4
--max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4
--max-union-fields 3 --max-pointer-depth 2
```
Safe math stays on: the wrappers avoid signed overflow, oversized shifts and
division by zero. Floats stay off (the default). Unions, structs, pointers,
goto, 64-bit math, mul/div and comma operators stay on. `--ccomp` drops
bitfields and adds explicit casts. Bitfield locals are also stuck in
CompCert's semantics.

**Flattening (`scripts/csmith_flatten.py`).** Each program is one file with
no `#include`:
- a standalone LP64 prelude: `int printf(const char *, ...);` and the
  `<stdint.h>` typedefs (long = 64-bit, the same on x86-64 glibc and
  riscv64 newlib);
- Csmith's CRC-32 runtime;
- only the `safe_math.h` wrappers the program calls, preprocessed for LP64.

As a result, tcc-in-Sail doesn't lex newlib's stdio.h (about 3000 lines),
and the only external is printf. `--includes` uses the real headers instead.

**Filters, per seed:**
1. Size: 120 ≤ lines ≤ 500. Below 120 lines, Csmith output is trivial.
2. Host `gcc -O0` and `gcc -O2 -fsanitize=undefined,address`, both with
   `-funsigned-char`, exit 0 within 5 s / 20 s, with no sanitizer report and
   identical `checksum = %X` output.
3. `riscv-none-elf-gcc -march=rv64i -mabi=lp64` compiles it.
4. `scripts/run_ccomp_interp.sh` exits 0 with the same stdout. This step is
   pluggable: it is skipped if the script is missing. A 126 (stuck) result
   drops the program.
5. The xgcc_sail run finishes within 3M steps.

For seeds 1..336 the results were: 200 kept, 66 too large, 56 too small,
14 host timeouts (long loops). There were no sanitizer, ccomp or Sail
rejections.

**Size statistics (200 programs):**

| | min | p25 | median | p75 | p90 | max |
|---|---|---|---|---|---|---|
| lines | 123 | 232 | 325 | 409 | 465 | 497 |
| bytes | 3585 | 6925 | 9655 | 12616 | 14166 | 15371 |
| xgcc_sail steps | 41962 | 45115 | 47640 | 50816 | 55766 | 290433 |

Execution is cheap. About 40k of the steps are fixed cost: the CRC table and
printf. For tcc_sail, compile time will dominate, and it scales with
lines/bytes.

**Sail steps for the other corpora** (xgcc_sail, -O0):
- CompCert: median 52k, p90 3.6M, max 14.7M (`fftsp`: soft-float sinf/cosf), total 82M.
- tcc: median 10.6k, max 242k, total 2.1M.

## Harness

```
scripts/diff3.py [--oracles a,b] [--corpus csmith,compcert,tcc] [--progs GLOB] [--jobs N] [--no-cache] [--summary-only]
```

- Oracles are command templates in `scripts/oracles.toml`.
- Each run is cached at `results/diff3/.cache/<oracle>/<sha256(prog)>-<version>.json`. The version hashes the command, env, `version_cmd` output and `version_files`.
- Jobs run in parallel (default 8). Heavy oracles (Sail, Lean, ccomp) are limited to 8 at a time and wait while MemAvailable is under 25 GB.
- Output: `results/diff3/<corpus>/<prog>.json` (each oracle's outcome, stdout, exit, wall time, meta such as Sail `steps`, stderr tail), `results/diff3/summary.md` and `summary.tsv`.
- Classifications live in `results/diff3/classifications.tsv`, one row per (corpus, prog, pair or `*`). The category is exactly one of: tcc bug, Lean-port bug, CompCert-frontend issue, unspecified behaviour, harness bug, unclassified.

**Oracle interface:**
- The command is run with `{prog}` set to the absolute path of one `.c` file. cwd is the repo root and stdin is `/dev/null`.
- stdout is the program's stdout, byte for byte. stderr is saved but not compared.
- The exit code is the program's status, compared mod 256. 124 means timeout or step budget exhausted; 125 means the oracle can't run the program (n/a).
- A per-oracle `codes` table maps extra codes to `n/a`, `stuck` or `timeout`. For ccomp/Lean: 120 rejected → n/a, 126 stuck → stuck, 127 outside printf model → n/a, 137 OOM kill → n/a.
- The command may append `key=value` lines to `$ORACLE_META`.
- Two oracles agree when both are `ok` with the same stdout and exit code. Any `timeout`, `n/a`, `stuck` or `absent` makes the pair incomparable.

**Oracles:**
- **gcc_native** (`scripts/run_gcc_native.sh`): host gcc 15.2 `-O0 -std=gnu11 -fpermissive -funsigned-char -lm`, run with argv[0]="prog".
- **xgcc_sail** (`scripts/run_xgcc_sail.sh`): xPack riscv-none-elf-gcc 15.2 `-O0 -march=rv64i -mabi=lp64 -mcmodel=medany -nostartfiles -T c/harness/link.ld c/harness/crt0.S c/harness/htif.c prog.c -lm -lc -lgcc`.
  - It runs on ship-your-lua's Lean Sail emulator with `--trace-pcs <_exit+16> --max-steps 60M`.
  - Steps are the row of the last traced `sd` to tohost, plus 1 (as in ship-your-lua's difftest.sh). TRACE-FUEL-OUT maps to 124.
  - `c/harness` is adapted from ship-your-lua. It is console only, writes to fd 2 are dropped, and `main(1, {"prog"})`.
  - It uses the same newlib printf that tcc-on-Sail links.
- **tcc_sail** (`scripts/run_tcc_sail.sh`): placeholder, `requires` the script. See the plug-in notes below.
- **ccomp_interp** / **lean_clight**: the Certora lane's scripts, run under `systemd-run --scope -p MemoryMax=8G/4G` with a 600 s timeout. `CERTORA_DIR` and `CLIGHT_RUN` point at that lane's build, set in the `env` of `oracles.toml`.

**Emulator console encoding.** The Lean emulator keeps console bytes in a
Lean `String`, one code point per byte, and prints it UTF-8 encoded. Byte
0xfe comes out as C3 BE. `run_xgcc_sail.sh` reverses this (UTF-8 decode, then
Latin-1 encode). `run_tcc_sail.sh` must do the same. In trace mode the
emulator's stdout holds only console bytes; the `PC =`, `htif_tohost =` and
`SUCCESS` lines appear only in plain mode.

**Minimisation.** `scripts/minimise.sh prog.c ORACLE_A ORACLE_B [OUT.c]` runs
C-Vise 2.12 (`build/cvise-env`, from conda-forge via micromamba; there is no
PyPI package and no clang dev headers on this box). A candidate counts as
interesting when all of the following hold:
- no UB-signalling gcc warnings (C-Reduce's list plus -Wformat and -Wmain);
- clean under UBSan/ASan;
- `ccomp -interp` exits 0, when that script is present;
- A and B both finish and differ.

On a test program with a `%p` difference it reduced 17 lines to 3.
`scripts/oracle.py NAME prog.c` runs one oracle uncached.

## Results

### Two oracles (gcc_native vs xgcc_sail), run before the Certora oracles existed

The first run found four harness or corpus bugs. All are fixed:

1. **Harness bug:** no `-lm` in the xgcc link, so 7 benchmarks were n/a.
2. **Harness bug:** emulator console bytes ≥ 0x80 were UTF-8 encoded.
   - `mandelbrot`'s binary PBM output and `t_83_utf8_in_identifiers` disagreed.
   - Fixed in run_xgcc_sail.sh.
3. **Harness bug:** a shrink note containing `*/` broke the copied file's header comment, so `vmach` was n/a for both oracles.
4. **Corpus bug:** the `bisect` shrink introduced an out-of-bounds read (UB); see above.

After the fixes, gcc_native and xgcc_sail agree on all 345 programs: Csmith
200/200, CompCert 72/72, tcc 73/73.

### Four oracles

| corpus | pairs among gcc_native, xgcc_sail, ccomp_interp | pairs with lean_clight |
|---|---|---|
| csmith | 200 agree, 0 disagree | 200 agree, 0 disagree |
| compcert | 45 agree, 27 incomparable | 43 agree, **1 disagree** (`regr_int64`), 28 incomparable |
| tcc | 57 agree, 16 incomparable | 56 agree, **1 disagree** (`t_134_double_to_signed`), 16 incomparable |

Both disagreements are Lean-port bugs. Each is hand-reduced to a UBSan-clean
reproducer, and gcc_native, xgcc_sail and ccomp_interp agree on the correct
output:

- `results/repro/lean_long_to_float_rounding.c`: converting a 64-bit integer to float rounds twice (through double). `(float)0x52ce6b4000000063` gives `5ea59cd6`; the correct result is `5ea59cd7`. This is true for both signed and unsigned.
- `results/repro/lean_double_to_ulong.c`: converting double to unsigned long saturates as if to signed long. `(unsigned long)1e19` gives `9223372036854775807`; the correct result is `10000000000000000000`.

**Other findings for the Certora lane** (incomparable cases, not disagreements):
- `regr_floats-lit` produces a `clightgen -lean` file that doesn't elaborate (`Unknown identifier f_testdbl`, and a maxRecDepth error). lean_clight is n/a there while ccomp_interp is ok.
- `regr_funptr2` is correctly stuck in both: `f+2 == f` on function pointers is a GNU extension.
- `regr_casts3` and `mandelbrot` get stuck on newlib's `_impure_ptr` (`fflush(stdout)`, `putc(..., stdout)`).
