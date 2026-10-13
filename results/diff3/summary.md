# diff3 summary

Programs: 345 (csmith: 200, compcert: 72, tcc: 73). Oracles and interface: scripts/oracles.toml.

## Oracle outcomes

| corpus | oracle | ok | timeout | n/a | stuck | absent | median wall s | median steps |
|---|---|---|---|---|---|---|---|---|
| csmith | gcc_native | 200 | 0 | 0 | 0 | 0 | 0.239 | - |
| csmith | xgcc_sail | 200 | 0 | 0 | 0 | 0 | 2.44 | 47640 |
| csmith | tcc_sail | 0 | 0 | 0 | 0 | 200 | - | - |
| csmith | ccomp_interp | 200 | 0 | 0 | 0 | 0 | 0.131 | - |
| csmith | lean_clight | 200 | 0 | 0 | 0 | 0 | 2.913 | - |
| compcert | gcc_native | 72 | 0 | 0 | 0 | 0 | 0.217 | - |
| compcert | xgcc_sail | 72 | 0 | 0 | 0 | 0 | 2.692 | 52213 |
| compcert | tcc_sail | 0 | 0 | 0 | 0 | 72 | - | - |
| compcert | ccomp_interp | 45 | 0 | 1 | 26 | 0 | 0.153 | - |
| compcert | lean_clight | 44 | 0 | 2 | 26 | 0 | 1.919 | - |
| tcc | gcc_native | 73 | 0 | 0 | 0 | 0 | 0.172 | - |
| tcc | xgcc_sail | 73 | 0 | 0 | 0 | 0 | 0.938 | 10635 |
| tcc | tcc_sail | 0 | 0 | 0 | 0 | 73 | - | - |
| tcc | ccomp_interp | 57 | 0 | 10 | 6 | 0 | 0.208 | - |
| tcc | lean_clight | 57 | 0 | 10 | 6 | 0 | 1.485 | - |

## Pairwise agreement

| corpus | pair | agree | disagree | incomparable |
|---|---|---|---|---|
| csmith | gcc_native vs xgcc_sail | 200 | 0 | 0 |
| csmith | gcc_native vs ccomp_interp | 200 | 0 | 0 |
| csmith | gcc_native vs lean_clight | 200 | 0 | 0 |
| csmith | xgcc_sail vs ccomp_interp | 200 | 0 | 0 |
| csmith | xgcc_sail vs lean_clight | 200 | 0 | 0 |
| csmith | ccomp_interp vs lean_clight | 200 | 0 | 0 |
| compcert | gcc_native vs xgcc_sail | 72 | 0 | 0 |
| compcert | gcc_native vs ccomp_interp | 45 | 0 | 27 |
| compcert | gcc_native vs lean_clight | 43 | 1 | 28 |
| compcert | xgcc_sail vs ccomp_interp | 45 | 0 | 27 |
| compcert | xgcc_sail vs lean_clight | 43 | 1 | 28 |
| compcert | ccomp_interp vs lean_clight | 43 | 1 | 28 |
| tcc | gcc_native vs xgcc_sail | 73 | 0 | 0 |
| tcc | gcc_native vs ccomp_interp | 57 | 0 | 16 |
| tcc | gcc_native vs lean_clight | 56 | 1 | 16 |
| tcc | xgcc_sail vs ccomp_interp | 57 | 0 | 16 |
| tcc | xgcc_sail vs lean_clight | 56 | 1 | 16 |
| tcc | ccomp_interp vs lean_clight | 56 | 1 | 16 |

## Disagreements

| corpus | program | pair | classification | note | left | right |
|---|---|---|---|---|---|---|
| compcert | regr_int64 | gcc_native/lean_clight | Lean-port bug | int64->float32 conversion double-rounds (via double): 0x52ce6b4000000063 gives 5ea59cd6, correct 5ea59cd7; repro results/repro/lean_long_to_float_rounding.c | exit 0, 88055 B, sha e9976dbb, "x = 0" | exit 0, 88055 B, sha ba7e9237, "x = 0" |
| compcert | regr_int64 | xgcc_sail/lean_clight | Lean-port bug | int64->float32 conversion double-rounds (via double): 0x52ce6b4000000063 gives 5ea59cd6, correct 5ea59cd7; repro results/repro/lean_long_to_float_rounding.c | exit 0, 88055 B, sha e9976dbb, "x = 0" | exit 0, 88055 B, sha ba7e9237, "x = 0" |
| compcert | regr_int64 | ccomp_interp/lean_clight | Lean-port bug | int64->float32 conversion double-rounds (via double): 0x52ce6b4000000063 gives 5ea59cd6, correct 5ea59cd7; repro results/repro/lean_long_to_float_rounding.c | exit 0, 88055 B, sha e9976dbb, "x = 0" | exit 0, 88055 B, sha ba7e9237, "x = 0" |
| tcc | t_134_double_to_signed | gcc_native/lean_clight | Lean-port bug | double->unsigned long saturates at LONG_MAX: (unsigned long)1e19 gives 9223372036854775807; repro results/repro/lean_double_to_ulong.c | exit 0, 51 B, sha b5bf839c, "-1" | exit 0, 50 B, sha 7611b951, "-1" |
| tcc | t_134_double_to_signed | xgcc_sail/lean_clight | Lean-port bug | double->unsigned long saturates at LONG_MAX: (unsigned long)1e19 gives 9223372036854775807; repro results/repro/lean_double_to_ulong.c | exit 0, 51 B, sha b5bf839c, "-1" | exit 0, 50 B, sha 7611b951, "-1" |
| tcc | t_134_double_to_signed | ccomp_interp/lean_clight | Lean-port bug | double->unsigned long saturates at LONG_MAX: (unsigned long)1e19 gives 9223372036854775807; repro results/repro/lean_double_to_ulong.c | exit 0, 51 B, sha b5bf839c, "-1" | exit 0, 50 B, sha 7611b951, "-1" |

## Not ok

| corpus | program | oracle | outcome | detail |
|---|---|---|---|---|
| compcert | bench_aes | ccomp_interp | stuck | [run_ccomp_interp] STUCK (undefined behaviour or unsupported external): Stuck state: calling memcmp( |
| compcert | bench_aes | lean_clight | stuck | [clight_run] STUCK (undefined behaviour or outside the interpreter): call to unsupported external fu |
| compcert | bench_almabench | ccomp_interp | stuck | [run_ccomp_interp] STUCK (undefined behaviour or unsupported external): Stuck state: calling fmod(1. |
| compcert | bench_almabench | lean_clight | stuck | [clight_run] STUCK (undefined behaviour or outside the interpreter): call to unsupported external fu |
| compcert | bench_binarytrees | ccomp_interp | stuck | [run_ccomp_interp] STUCK (undefined behaviour or unsupported external): Stuck state: calling pow(2., |
| compcert | bench_binarytrees | lean_clight | stuck | [clight_run] STUCK (undefined behaviour or outside the interpreter): call to unsupported external fu |
| compcert | bench_bisect | ccomp_interp | stuck | [run_ccomp_interp] STUCK (undefined behaviour or unsupported external): Stuck state: calling memset( |
| compcert | bench_bisect | lean_clight | stuck | [clight_run] STUCK (undefined behaviour or outside the interpreter): call to unsupported external fu |
| compcert | bench_fannkuch | ccomp_interp | stuck | [run_ccomp_interp] STUCK (undefined behaviour or unsupported external): Stuck state: calling calloc( |
| compcert | bench_fannkuch | lean_clight | stuck | [clight_run] STUCK (undefined behaviour or outside the interpreter): call to unsupported external fu |
| compcert | bench_fft | ccomp_interp | stuck | [run_ccomp_interp] STUCK (undefined behaviour or unsupported external): Stuck state: calling calloc( |
| compcert | bench_fft | lean_clight | stuck | [clight_run] STUCK (undefined behaviour or outside the interpreter): call to unsupported external fu |
| compcert | bench_fftsp | ccomp_interp | stuck | [run_ccomp_interp] STUCK (undefined behaviour or unsupported external): Stuck state: calling calloc( |
| compcert | bench_fftsp | lean_clight | stuck | [clight_run] STUCK (undefined behaviour or outside the interpreter): call to unsupported external fu |
| compcert | bench_mandelbrot | ccomp_interp | stuck | [run_ccomp_interp] STUCK (undefined behaviour or unsupported external): Stuck state: in function mai |
| compcert | bench_mandelbrot | lean_clight | stuck | [clight_run] STUCK (undefined behaviour or outside the interpreter): stuck at statement Sset |
| compcert | bench_nbody | ccomp_interp | stuck | [run_ccomp_interp] STUCK (undefined behaviour or unsupported external): Stuck state: calling sqrt(24 |
| compcert | bench_nbody | lean_clight | stuck | [clight_run] STUCK (undefined behaviour or outside the interpreter): call to unsupported external fu |
| compcert | bench_nsieve | ccomp_interp | stuck | [run_ccomp_interp] STUCK (undefined behaviour or unsupported external): Stuck state: calling memset( |
| compcert | bench_nsieve | lean_clight | stuck | [clight_run] STUCK (undefined behaviour or outside the interpreter): call to unsupported external fu |
| compcert | bench_nsievebits | ccomp_interp | stuck | [run_ccomp_interp] STUCK (undefined behaviour or unsupported external): Stuck state: calling memset( |
| compcert | bench_nsievebits | lean_clight | stuck | [clight_run] STUCK (undefined behaviour or outside the interpreter): call to unsupported external fu |
| compcert | bench_perlin | ccomp_interp | stuck | [run_ccomp_interp] STUCK (undefined behaviour or unsupported external): Stuck state: calling floor(- |
| compcert | bench_perlin | lean_clight | stuck | [clight_run] STUCK (undefined behaviour or outside the interpreter): call to unsupported external fu |
| compcert | bench_qsort | ccomp_interp | stuck | [run_ccomp_interp] STUCK (undefined behaviour or unsupported external): Stuck state: calling rand()  |
| compcert | bench_qsort | lean_clight | stuck | [clight_run] STUCK (undefined behaviour or outside the interpreter): call to unsupported external fu |
| compcert | bench_sha1 | ccomp_interp | stuck | [run_ccomp_interp] STUCK (undefined behaviour or unsupported external): Stuck state: calling strlen( |
| compcert | bench_sha1 | lean_clight | stuck | [clight_run] STUCK (undefined behaviour or outside the interpreter): call to unsupported external fu |
| compcert | bench_sha3 | ccomp_interp | stuck | [run_ccomp_interp] STUCK (undefined behaviour or unsupported external): Stuck state: calling strlen( |
| compcert | bench_sha3 | lean_clight | stuck | [clight_run] STUCK (undefined behaviour or outside the interpreter): call to unsupported external fu |
| compcert | bench_siphash24 | ccomp_interp | stuck | [run_ccomp_interp] STUCK (undefined behaviour or unsupported external): Stuck state: calling memcmp( |
| compcert | bench_siphash24 | lean_clight | stuck | [clight_run] STUCK (undefined behaviour or outside the interpreter): call to unsupported external fu |
| compcert | bench_spectral | ccomp_interp | stuck | [run_ccomp_interp] STUCK (undefined behaviour or unsupported external): Stuck state: calling sqrt(1. |
| compcert | bench_spectral | lean_clight | stuck | [clight_run] STUCK (undefined behaviour or outside the interpreter): call to unsupported external fu |
| compcert | regr_bitfields1 | ccomp_interp | stuck | [run_ccomp_interp] STUCK (undefined behaviour or unsupported external): Stuck state: in function mai |
| compcert | regr_bitfields1 | lean_clight | stuck | [clight_run] STUCK (undefined behaviour or outside the interpreter): stuck at statement Sassign |
| compcert | regr_bitfields10 | ccomp_interp | stuck | [run_ccomp_interp] STUCK (undefined behaviour or unsupported external): Stuck state: in function f,  |
| compcert | regr_bitfields10 | lean_clight | stuck | [clight_run] STUCK (undefined behaviour or outside the interpreter): stuck at statement Sassign |
| compcert | regr_bitfields4 | ccomp_interp | stuck | [run_ccomp_interp] STUCK (undefined behaviour or unsupported external): Stuck state: in function mai |
| compcert | regr_bitfields4 | lean_clight | stuck | [clight_run] STUCK (undefined behaviour or outside the interpreter): stuck at statement Sassign |
| compcert | regr_bitfields5 | ccomp_interp | stuck | [run_ccomp_interp] STUCK (undefined behaviour or unsupported external): Stuck state: in function mai |
| compcert | regr_bitfields5 | lean_clight | stuck | [clight_run] STUCK (undefined behaviour or outside the interpreter): stuck at statement Sassign |
| compcert | regr_bitfields6 | ccomp_interp | stuck | [run_ccomp_interp] STUCK (undefined behaviour or unsupported external): Stuck state: in function mai |
| compcert | regr_bitfields6 | lean_clight | stuck | [clight_run] STUCK (undefined behaviour or outside the interpreter): stuck at statement Sassign |
| compcert | regr_casts3 | ccomp_interp | stuck | [run_ccomp_interp] STUCK (undefined behaviour or unsupported external): Stuck state: in function mai |
| compcert | regr_casts3 | lean_clight | stuck | [clight_run] STUCK (undefined behaviour or outside the interpreter): stuck at statement Sset |
| compcert | regr_floats-lit | lean_clight | n/a | /Users/kirancodes/tmp/leanclight.skOc0Q/Prog.lean:6153:42: error(lean.unknownIdentifier): Unknown id |
| compcert | regr_funptr2 | ccomp_interp | stuck | [run_ccomp_interp] STUCK (undefined behaviour or unsupported external): Stuck state: in function mai |
| compcert | regr_funptr2 | lean_clight | stuck | [clight_run] STUCK (undefined behaviour or outside the interpreter): cannot evaluate the arguments o |
| compcert | regr_switch3 | ccomp_interp | n/a | [run_ccomp_interp] ccomp rejected the program: /data/home/kirancodes/Documents/code/ship-your-tcc/.c |
| compcert | regr_switch3 | lean_clight | n/a | [run_lean_clight] clightgen rejected the program: /Users/kirancodes/tmp/leanclight.rxksrC/prog.c:63: |
| compcert | regr_varargs1 | ccomp_interp | stuck | [run_ccomp_interp] STUCK (undefined behaviour or unsupported external): Stuck state: in function mai |
| compcert | regr_varargs1 | lean_clight | stuck | [clight_run] STUCK (undefined behaviour or outside the interpreter): stuck at statement Scall |
| compcert | regr_varargs2 | ccomp_interp | stuck | [run_ccomp_interp] STUCK (undefined behaviour or unsupported external): Stuck state: in function mai |
| compcert | regr_varargs2 | lean_clight | stuck | [clight_run] STUCK (undefined behaviour or outside the interpreter): stuck at statement Scall |
| tcc | t_102_alignas | ccomp_interp | n/a | [run_ccomp_interp] ccomp rejected the program: /data/home/kirancodes/Documents/code/ship-your-tcc/.c |
| tcc | t_102_alignas | lean_clight | n/a | [run_lean_clight] clightgen rejected the program: /Users/kirancodes/tmp/leanclight.fqPdE6/prog.c:5:1 |
| tcc | t_118_switch | ccomp_interp | n/a | [run_ccomp_interp] ccomp rejected the program: /data/home/kirancodes/Documents/code/ship-your-tcc/.c |
| tcc | t_118_switch | lean_clight | n/a | [run_lean_clight] clightgen rejected the program: /Users/kirancodes/tmp/leanclight.NsM2Vb/prog.c:9:1 |
| tcc | t_133_old_func | ccomp_interp | n/a | [run_ccomp_interp] ccomp rejected the program: /data/home/kirancodes/Documents/code/ship-your-tcc/.c |
| tcc | t_133_old_func | lean_clight | n/a | [run_lean_clight] clightgen rejected the program: /Users/kirancodes/tmp/leanclight.DWy05J/prog.c:38: |
| tcc | t_13_integer_literals | ccomp_interp | n/a | [run_ccomp_interp] ccomp rejected the program: /data/home/kirancodes/Documents/code/ship-your-tcc/.c |
| tcc | t_13_integer_literals | lean_clight | n/a | [run_lean_clight] clightgen rejected the program: /Users/kirancodes/tmp/leanclight.S5t0NA/prog.c:10: |
| tcc | t_17_enum | ccomp_interp | n/a | [run_ccomp_interp] ccomp rejected the program: /data/home/kirancodes/Documents/code/ship-your-tcc/.c |
| tcc | t_17_enum | lean_clight | n/a | [run_lean_clight] clightgen rejected the program: /Users/kirancodes/tmp/leanclight.DIAAgb/prog.c:23: |
| tcc | t_24_math_library | ccomp_interp | stuck | [run_ccomp_interp] STUCK (undefined behaviour or unsupported external): Stuck state: calling sin(0.1 |
| tcc | t_24_math_library | lean_clight | stuck | [clight_run] STUCK (undefined behaviour or outside the interpreter): call to unsupported external fu |
| tcc | t_28_strings | ccomp_interp | stuck | [run_ccomp_interp] STUCK (undefined behaviour or unsupported external): Stuck state: calling strcpy( |
| tcc | t_28_strings | lean_clight | stuck | [clight_run] STUCK (undefined behaviour or outside the interpreter): call to unsupported external fu |
| tcc | t_29_array_address | ccomp_interp | stuck | [run_ccomp_interp] STUCK (undefined behaviour or unsupported external): Stuck state: calling strcpy( |
| tcc | t_29_array_address | lean_clight | stuck | [clight_run] STUCK (undefined behaviour or outside the interpreter): call to unsupported external fu |
| tcc | t_35_sizeof | ccomp_interp | n/a | [run_ccomp_interp] ccomp rejected the program: /data/home/kirancodes/Documents/code/ship-your-tcc/.c |
| tcc | t_35_sizeof | lean_clight | n/a | [run_lean_clight] clightgen rejected the program: /Users/kirancodes/tmp/leanclight.L4b2hx/prog.c:12: |
| tcc | t_37_sprintf | ccomp_interp | stuck | [run_ccomp_interp] STUCK (undefined behaviour or unsupported external): Stuck state: in function mai |
| tcc | t_37_sprintf | lean_clight | stuck | [clight_run] STUCK (undefined behaviour or outside the interpreter): stuck at statement Scall |
| tcc | t_80_flexarray | ccomp_interp | n/a | [run_ccomp_interp] ccomp rejected the program: /data/home/kirancodes/Documents/code/ship-your-tcc/.c |
| tcc | t_80_flexarray | lean_clight | n/a | [run_lean_clight] clightgen rejected the program: /Users/kirancodes/tmp/leanclight.dT7AqS/prog.c:19: |
| tcc | t_81_types | ccomp_interp | n/a | [run_ccomp_interp] ccomp rejected the program: /data/home/kirancodes/Documents/code/ship-your-tcc/.c |
| tcc | t_81_types | lean_clight | n/a | [run_lean_clight] clightgen rejected the program: /Users/kirancodes/tmp/leanclight.KyKO7m/prog.c:4:  |
| tcc | t_83_utf8_in_identifiers | ccomp_interp | n/a | [run_ccomp_interp] ccomp rejected the program: /data/home/kirancodes/Documents/code/ship-your-tcc/.c |
| tcc | t_83_utf8_in_identifiers | lean_clight | n/a | [run_lean_clight] clightgen rejected the program: /Users/kirancodes/tmp/leanclight.2Bs40g/prog.c:3:  |
| tcc | t_92_enum_bitfield | ccomp_interp | stuck | [run_ccomp_interp] STUCK (undefined behaviour or unsupported external): Stuck state: in function mai |
| tcc | t_92_enum_bitfield | lean_clight | stuck | [clight_run] STUCK (undefined behaviour or outside the interpreter): stuck at statement Sassign |
| tcc | t_93_integer_promotion | ccomp_interp | n/a | [run_ccomp_interp] ccomp rejected the program: /data/home/kirancodes/Documents/code/ship-your-tcc/.c |
| tcc | t_93_integer_promotion | lean_clight | n/a | [run_lean_clight] clightgen rejected the program: /Users/kirancodes/tmp/leanclight.D3dhO4/prog.c:13: |
| tcc | t_97_utf8_string_literal | ccomp_interp | stuck | [run_ccomp_interp] STUCK (undefined behaviour or unsupported external): Stuck state: calling strcmp( |
| tcc | t_97_utf8_string_literal | lean_clight | stuck | [clight_run] STUCK (undefined behaviour or outside the interpreter): call to unsupported external fu |
