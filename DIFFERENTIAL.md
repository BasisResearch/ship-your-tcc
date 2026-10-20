# Three-way differential: TinyCC, Lean Clight and CompCert

The measured corpus contains 345 programs. All five oracles agree on all
200 Csmith programs. Across the full corpus, 301 programs complete on all
three primary oracles: 298 agree and three disagree. The three discrepancies
have small reproducers: two in Certora's Lean numeric conversions and one
in TinyCC's union initializer handling.

These are execution results, not a compiler-correctness theorem or a proof
that Certora's Lean semantics equals Rocq's semantics.

## Scope and measurements

The primary oracles are bare-metal `tcc -run` on the executable Sail RV64I
model, Certora's Lean Clight interpreter, and `ccomp -interp`. Native GCC
and RV64I GCC on Sail supply two sanity checks. TinyCC is pinned to 43c7708
with this repository's RV64I soft-float and bare-metal patches. Certora/clean
is pinned to 123f7f7, with the documented printf/export driver patch and
Lean 4.32.1. See [oracle construction](results/certora.md),
[corpus selection](results/corpus.md), and [configuration](scripts/oracles.toml).

| corpus | attempted | all three complete | three-way agreement | discrepant programs |
|---|---:|---:|---:|---:|
| Csmith | 200 | 200 | 200 | 0 |
| CompCert tests | 72 | 44 | 42 | 2 |
| TinyCC tests | 73 | 57 | 56 | 1 |
| total | 345 | 301 | 298 | 3 |

| pair | agree | disagree | incomparable |
|---|---:|---:|---:|
| tcc/Sail vs ccomp | 301 | 1 | 43 |
| tcc/Sail vs Lean Clight | 298 | 3 | 44 |
| ccomp vs Lean Clight | 299 | 2 | 44 |
| native GCC vs RV64I GCC/Sail | 345 | 0 | 0 |
| native GCC vs tcc/Sail | 335 | 1 | 9 |

An agreement requires identical stdout bytes and exit status. Incomplete,
rejected, unsupported, or stuck executions are incomparable. Every current
pairwise disagreement is classified in
[classifications.tsv](results/diff3/classifications.tsv); the generated
[summary](results/diff3/summary.md) lists every outcome and pair.

The retained measurements span multiple harness revisions, as recorded in
each JSON's `version`. They have not all been rerun after the errno and
completion-check fixes. One invalid wrapper result was replaced by the
explicitly labelled fresh rerun described below. The version identifiers
must be preserved when reproducing these numbers; current cache keys
invalidate earlier harness results.

## Reduced discrepancies

| program | category | observation | reproducer |
|---|---|---|---|
| `regr_int64` | Lean-port bug | signed and unsigned 64-bit integer to float conversion rounds through double; float bits `5ea59cd6` instead of `5ea59cd7` | [integer-to-float](results/repro/lean_long_to_float_rounding.c) |
| `t_134_double_to_signed` | Lean-port bug | `(unsigned long)1e19` produces `9223372036854775807` instead of `10000000000000000000` | [double-to-unsigned](results/repro/lean_double_to_ulong.c) |
| `regr_initializers2` | tcc bug | `.a = 42, .c[2] = 1` in a union retains bytes from the overridden member: `42 0 1 0` instead of `0 0 1 0` | [union designators](results/repro/tcc_union_designator_override.c) |

For the two numeric cases, tcc/Sail, both GCC oracles and ccomp agree. For
the initializer case, both GCC oracles, ccomp and Lean Clight agree; the
reproducer also fails on pristine native x86-64 TinyCC 43c7708. This locates
the initializer discrepancy outside the new RV64I backend. The reduced
files document their inputs and expected outputs. The numeric results
depend on the documented target and floating-point choices.

On 2026-10-20 all three reduced cases were rerun with the current GCC,
ccomp and Lean binaries; pristine native TinyCC was also rerun for the
union case. The same outputs reproduced. Raw outputs are retained in
[results/review/repro](results/review/repro/).

## Exclusions and harness findings

The remaining 44 programs do not complete on all three primary oracles.
Both CompCert-based interpreters get stuck on 32 programs, including
unsupported library calls, user-defined variadic calls, and reads they
treat as undefined. CompCert rejects another 11; Lean additionally cannot
elaborate `regr_floats-lit`. These are exclusions, not evidence of output
agreement or a compiler bug. Individual diagnostics remain in the JSONs.

Nine retained tcc results are `n/a` because libm symbols were unresolved:
`bench_almabench`, `bench_binarytrees`, `bench_bisect`, `bench_fft`,
`bench_fftsp`, `bench_nbody`, `bench_perlin`, `bench_spectral`, and
`t_24_math_library`. All nine are also outside the common interpreter
subset. The wrapper now retries with `-lm` after the source file; the
reported nine results still describe the earlier invocation.
An explicit `-lm` rerun of `t_24_math_library` on 2026-10-20 matches both
GCC oracles: 13,822,698 compile steps and 311,447 program steps. Its output
and metadata are supplemental artifacts under `results/review/`; they do
not replace the retained corpus record or alter the tables above.

`regr_varargs2` previously had an empty stdout and exit 2 caused by a shell
syntax error. That record was incorrectly marked `ok`. A fresh Sail run
on 2026-10-19 completed with exit 0 and matched both GCC outputs byte for
byte: 13,003,889 compile steps, 234,368 program steps, 13,238,282 total.
The invalid record and rerun output/metadata are retained under
[results/review](results/review/). This case is still outside the primary
three-way subset because both CompCert interpreters get stuck on its
user-defined variadic calls.

The harness now requires traced guest-exit metadata, preserves program
text that resembles emulator diagnostics, kills its own process group on
timeout, includes patches and runtime sources in cache fingerprints, and
requires successful oracle outcomes during reduction. Fast regression
checks cover these failure paths. The runtime errno regression separately
exposed incorrect propagation of syscall failures into the generated
program's newlib; the raw syscall errno slot is now shared.

## Reproduction

Rebuild the local summary without executing programs:

```sh
python3 scripts/diff3.py --summary-only
python3 scripts/tests/test_diff3.py
python3 scripts/tests/test_gates.py
python3 c/tests/test_run_harness.py
```

Run fresh oracles with `scripts/diff3.py --no-cache --jobs 8`; build them
first using the scripts and resource caps described in README.md and
results/certora.md. The runner waits for 25 GB available memory before
heavy work and permits at most eight heavy jobs. A complete fresh rerun
will take hours because tcc's compilation itself executes on Sail.

The user approved publication to `BasisResearch/ship-your-tcc` on 2026-10-20.
