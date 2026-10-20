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
is pinned to 123f7f7, with the documented printf/export driver patch. The
Lean Clight oracle runs the vendored CCLib on Lean 4.34.0, the same one
Phase 3 uses, and prints through `Tcc.Clight.format`, the pure printf model
of the Phase 3 premise. That column was rerun with this formatter on
2026-10-20; no outcome, stdout or exit code changed (it previously used the
host `snprintf`). See [oracle construction](results/certora.md),
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
| native GCC vs tcc/Sail | 344 | 1 | 0 |

An agreement requires identical stdout bytes and exit status. Incomplete,
rejected, unsupported, or stuck executions are incomparable. Every current
pairwise disagreement is classified in
[classifications.tsv](results/diff3/classifications.tsv); the generated
[summary](results/diff3/summary.md) lists every outcome and pair.

Every number here comes from one complete rerun on 2026-10-20, on the
current harness and the corrected runtime (hash `9e1142dc96d8db10`). All
345 tcc/Sail records carry the same harness version, `12ca7335e2361f43`,
recorded in each JSON's `version`. The fingerprint covers the wrapper, the
patches and the runtime sources, so a change to any of them invalidates the
cache. The earlier mixed-version measurements gave the same three-way
counts.

### tcc/Sail cost

tcc/Sail completes all 345 programs (`ok`, no timeouts, no `n/a`), each
under a 100M-step budget.

| phase | median steps | max steps | total steps |
|---|---:|---:|---:|
| compile (`_start` to `tcc_baremetal_run`) | 8,636,467 | 21,436,675 | 3,307,962,085 |
| program (`tcc_baremetal_enter` to `_exit`) | 51,661 | 14,983,519 | 95,207,108 |

Median wall time per program is 339 s for Csmith, 528 s for CompCert tests
and 499 s for TinyCC tests. Summed over all programs, that is 40.9 hours of
emulator time. The rerun took 3 h 12 min of wall time (01:40–04:52 UTC)
in two runner instances, one for Csmith and one for the two test suites.
On the same programs, RV64I GCC on Sail needs about 1–3 s each: the cost is
tcc compiling on the model, not the generated code.

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

Nine programs need libm: `bench_almabench`, `bench_binarytrees`,
`bench_bisect`, `bench_fft`, `bench_fftsp`, `bench_nbody`, `bench_perlin`,
`bench_spectral` and `t_24_math_library`. In the earlier measurements their
tcc results were `n/a`, because `-lm` named before the source file pulls
nothing in. The wrapper now retries with `-lm` after `/prog.c` when tcc
first stops on an unresolved reference (metadata `libm=1`). All nine now
complete and match both GCC oracles byte for byte. For example,
`t_24_math_library` takes 13,822,698 compile steps and 311,447 program
steps, and `bench_almabench` takes 17,553,173 and 11,192,976. All nine are
outside the common interpreter subset.

`regr_varargs2` once had a record wrongly marked `ok`, with empty stdout and
exit 2 from a shell syntax error in the wrapper. That record is kept under
[results/review](results/review/). In the rerun it completes with exit 0 and
matches both GCC oracles, taking 12,980,867 compile steps and 234,364
program steps. It is still outside the three-way subset, because both
CompCert interpreters get stuck on its user-defined variadic calls.

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
takes 5–6 hours with eight jobs, because tcc's compilation itself
executes on Sail.

The user approved publication to `BasisResearch/ship-your-tcc` on 2026-10-20.
