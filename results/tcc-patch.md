# tcc RV64I soft-float code generator (`TCC_RV64I_SOFT`)

The executable Sail model is RV64I only: it traps on `mul`, has no F/D, and we
assume no C and no A extension. `patches/0001-tcc-rv64i-soft.patch` makes
tinycc's riscv64 back end (pinned at 43c7708) emit only RV64I instructions
when it is built with `-DTCC_RV64I_SOFT`. Multiply, divide and every
floating-point operation become calls to libgcc. The ABI is lp64 soft-float,
the same one that `riscv-none-elf-gcc -march=rv64i -mabi=lp64` uses, so tcc
objects link and interoperate with the rv64i/lp64 newlib and libgcc that the
sister projects use.

## Results (2026-10-13)

| check | result |
|---|---|
| corpus `c/tests/tcc/*.c` (43 programs), tcc vs xpack gcc -O0 rv64i, both on Sail | **43/43 pass** (stdout and exit code identical) |
| same corpus, tcc on Sail vs host gcc -O0 -funsigned-char | **43/43 pass** |
| `check_isa.sh` on every tcc object (corpus and tests2) | **0 offending instructions** (146 objects: 43 corpus + 103 tests2) |
| `check_isa.sh` on every fully linked corpus ELF (newlib and libgcc included) | 43/43 RV64I-only |
| `check_isa.sh` on `riscv64-libtcc1.a` | OK (608 instructions) |
| tcc's `tests/tests2`, same harness, compared with `.expect` | 101/104 pass; the 3 failures are harness or header issues (below) |
| `check_patch_neutral.sh`: patched vs pristine tcc, both built without the macro | byte-identical objects for all 43 programs |

`results/tcc-patch-corpus.txt` has the full table with Sail step counts.
Reproduce with:

```
scripts/build_cross_tcc.sh        # build/cross/riscv64-tcc, riscv64-libtcc1.a
scripts/test_tcc_patch.sh         # corpus (about 6 min with JOBS=8; t36/t37 dominate)
scripts/test_tcc_tests2.sh        # tcc's tests2 (about 10 min)
scripts/check_patch_neutral.sh
```

Sail step counts for tcc range from 31k (`t14_bool`) to 18.0M (`t36_mandel`).
The median tcc/gcc ratio is about 1.01. The outliers are `t33_llong_edge`
(2.4x), `t38_hash` (1.8x) and `t39_matrix_int` (1.5x): gcc expands
multiplication by a constant into shifts and adds even at -O0, whereas tcc
calls `__muldi3`. tcc turns only powers of two into shifts.

## What the patch does

Every change is inside `#ifdef TCC_RV64I_SOFT` / `#ifndef TCC_RV64I_SOFT`:
riscv64-gen.c, plus 4 lines in tccelf.c. The patch is 335 lines in all
(+186/-9 in the source).

- **Registers.** `a0..a7` carry `RC_INT | RC_FLOAT`, so float and double
  values live in integer registers. `RC_FRET`/`REG_FRET` are `a0`. The eight
  f-register slots have class 0 and are never allocated. `load`/`store` then
  use `lw/ld/sw/sd` for floats automatically. The store assertion that
  requires an f-register is disabled, and a float constant in a register (a
  path that is in practice dead, because tccgen puts float constants in
  .rodata) is built as an integer bit pattern.
- **Calling convention (lp64).** `reg_pass` forces the "unnamed" path for
  every argument. All scalars and aggregates, including structs of floats,
  are passed by size in up to 2 integer registers, or on the stack, or by
  reference above 16 bytes. This is exactly the existing vararg path, so
  `gfunc_call`, `gfunc_prolog`, `gfunc_sret` and the vararg save area need
  no further change: the save area only ever stored `a0..a7`. Struct return
  is the same. Long double was already passed as an integer pair.
- **mul/div/rem** (`gen_opil`): `*` calls `__muldi3`, `/` and pointer
  differences call `__divdi3`, `%` calls `__moddi3`, unsigned `/` calls
  `__udivdi3`, and unsigned `%` calls `__umoddi3`. Both widths use the DI
  routines, as gcc -march=rv64i does (libgcc has no `__mulsi3` on rv64).
  For 32-bit unsigned ops, both operands are zero-extended first (tcc keeps
  32-bit values sign-extended). Every 32-bit result is `sext.w`'d, as gcc
  does after the call.
- **float/double arithmetic** (`gen_opf`): `__{add,sub,mul,div}{sf,df}3`.
- **Comparisons:** `__{eq,ne,lt,le,gt,ge}{sf,df}2`, with the int result
  compared against 0 using the same relation. This is gcc's lowering and
  gives IEEE semantics: with a NaN operand, `== < <= > >=` are false and
  `!=` is true. `!x`, `if (x)`, `&&`, `||`, `?:` and conversion to `_Bool`
  go through tccgen's `x != 0.0`, which calls `__nedf2`, as gcc does.
- **Conversions:** int to FP is `__float{si,unsi,di,undi}{sf,df}`, chosen by
  the source's signedness and width. char, short and _Bool are already
  extended in their register. FP to int is
  `__fix{,uns}{sf,df}{si,di}`: tccgen narrows to char or short through
  `int`, so (unsigned char)d uses `__fixdfsi`, where gcc uses `__fixunsdfsi`.
  The two agree wherever the result is defined. FP to FP is
  `__extendsfdf2`, `__truncdfsf2`, and for long double
  `__extend{sf,df}tf2` and `__trunctf{sf,df}2`. These now go through a
  plain `gfunc_call`, because the hard-float special case that loaded `fa0`
  is gone.
- **Long double** (128-bit): upstream already used `__{add,sub,mul,div}tf3`,
  `__{eq,ne,lt,le,gt,ge}tf2`, `__float{si,unsi,di,undi}tf` and
  `__fix{,uns}tf{si,di}` with integer register pairs. That code is
  unchanged.
- **Unary minus** stays tccgen's generic sign flip through memory (spill,
  `xor` the top byte with 0x80, reload): RV64I only, and exact for -0.0 and
  NaN. gcc does the same `xor` in registers. Neither calls `__negdf2`.
- **Predefined macros:** `__riscv`, `__riscv_xlen 64` and
  `__riscv_float_abi_soft`. `__riscv_flen`, `__riscv_mul`, `__riscv_div`,
  `__riscv_fdiv` and `__riscv_fsqrt` are no longer defined; this matches
  gcc -march=rv64i -mabi=lp64.
- **ELF:** `e_flags = EF_RISCV_FLOAT_ABI_SOFT` (0) with no RVC bit, so
  GNU ld accepts tcc objects alongside rv64i/lp64 objects. Upstream wrote
  DOUBLE.
- **Bounds-checking epilogue** (only with `CONFIG_TCC_BCHECK`, which we
  configure off): upstream emitted raw RVC words and `fsd/fld fa0`. The
  patch replaces them with full-width `sd/ld` of a0/a1. This path is
  untested.

Helpers are named with `tok_alloc_const()` from riscv64-gen.c, so tcctok.h
is not touched.

### libgcc routines tcc can emit under the macro

`__muldi3 __divdi3 __moddi3 __udivdi3 __umoddi3`
`__addsf3 __subsf3 __mulsf3 __divsf3 __adddf3 __subdf3 __muldf3 __divdf3`
`__eqsf2 __nesf2 __ltsf2 __lesf2 __gtsf2 __gesf2 __eqdf2 __nedf2 __ltdf2 __ledf2 __gtdf2 __gedf2`
`__floatsisf __floatunsisf __floatdisf __floatundisf __floatsidf __floatunsidf __floatdidf __floatundidf`
`__fixsfsi __fixunssfsi __fixsfdi __fixunssfdi __fixdfsi __fixunsdfsi __fixdfdi __fixunsdfdi`
`__extendsfdf2 __truncdfsf2`
`__addtf3 __subtf3 __multf3 __divtf3 __eqtf2 __netf2 __lttf2 __letf2 __gttf2 __getf2`
`__floatsitf __floatunsitf __floatditf __floatunditf __fixtfsi __fixunstfsi __fixtfdi __fixunstfdi`
`__extendsftf2 __extenddftf2 __trunctfsf2 __trunctfdf2`

The corpus objects reference all of these (checked with `nm -u`). All are
exported by the xpack rv64i/lp64 `libgcc.a`. `__unord*2` and `__neg*2` are
never needed: C has no unordered comparison operator, and negation is a
sign flip.

## libtcc1.a for rv64i/lp64

`scripts/build_cross_tcc.sh` builds `build/cross/riscv64-libtcc1.a` **with
the patched cross tcc**, as upstream does. The sources are written for tcc
(`__TINYC__` paths, tcc-asm `.S`, and the `__riscv64_clear_cache` builtin,
which gcc does not have). The result passes `check_isa.sh`. It contains:

- `builtin.o`: `__builtin_{ffs,clz,ctz,clrsb,popcount,parity}{,l,ll}`, which
  tcc lowers to calls. These call `__muldi3`.
- `alloca.o`: `alloca`.
- `armflush.o`: `__clear_cache` (fence + fence.i).
- `dsohandle.o`: `__dso_handle`.

Three upstream pieces are dropped:
- `lib-arm64.o`: its soft long-double routines duplicate libgcc's, and we
  want libgcc's so that the sister projects' proofs apply.
- `atomic.o`/`stdatomic.o`: `lr/sc/amo` (the A extension) and raw RVC
  words.
- `alloca-bt.o`/bcheck.

## Not supported

- **C11 atomics / `__atomic_*`:** there is no libtcc1 support, so they fail
  to link (tests2 `124`, `125`, `136`). A single-hart bare-metal version
  could be plain loads and stores; it is not written.
- **Inline asm that names FP registers or `f` constraints** would still try
  f-registers. Only user code can do this; nothing in tcc's own output
  does.
- **Bounds checking (`-b`) and backtraces** are configured off and untested.
- **Complex numbers:** tcc has none.
- **Constant folding:** tccgen folds FP constant expressions in the *host's*
  `long double`, then rounds. On the x86 cross compiler that is x87 80-bit;
  on the bare-metal tcc built by gcc rv64i it will be binary128 via libgcc.
  A folded `double` expression can therefore in principle double-round
  differently from run-time evaluation or from gcc, which folds exactly.
  This was not observed: `t25_fconst` compares folded and run-time `1/3`
  and `0.1+0.2` bit for bit. It is upstream behaviour, not part of the
  patch.
- **Long double printing** (`%Lf`) is not compared against the host, which
  uses 80-bit long double. Long double tests print through `(double)`.

## tests2 failures (all outside the code generator)

- `61_integers`: identical program output. The only difference is an extra
  compile warning, "UINT_MAX redefined", because the test redefines a macro
  that newlib's `<limits.h>` also defines.
- `83_utf8_in_identifiers`: correct bytes. The Sail emulator writes each
  console byte as a Latin-1 character re-encoded in UTF-8, so non-ASCII
  output is mangled. The gcc-built ELF prints the identical mangled bytes.
- `118_switch` (build failure): `#include <limits.h>` as the *first*
  include breaks newlib's headers under tcc. tcc does not predefine
  `__INT8_TYPE__`/`__UINT8_TYPE__`/... (gcc does), so newlib's
  `machine/_default_types.h` probes `<limits.h>` while it is half-included,
  and `__uint8_t` is never defined. Workaround, verified on a reproducer:
  predefine `__INT8_TYPE__=signed char`, `__UINT8_TYPE__=unsigned char`, and
  the same for 16, 32 and 64 bits, when tcc uses newlib's headers.
- Skipped: 38 tests that need `-dt`, `-b`, `-run`, argv, pthreads, TLS,
  shared libraries, two files or constructors, or that are x86/arm asm
  tests. Also skipped: `119` (needs `sys/mman.h`), `136` (atomics) and
  `122_vla_reuse` (correct, but 100000 iterations of 4 soft `%` take more
  than 200M Sail steps).

## Robustness of `check_isa.sh`

- **Disassembly:** `objdump -D -M no-aliases`, restricted to SHF_EXECINSTR
  sections. `-D` is needed because tcc emits address-taken C labels
  (`&&lab`) as OBJECT symbols in `.text`, and plain `-d` dumps those bytes
  as data. This happened in tests2 `90_struct-init`.
- **Allowed instructions:** a whitelist of RV64I mnemonics plus
  fence/fence.i. `ecall` and `ebreak` are reported but accepted.
- **Compressed instructions:** caught independently by encoding length
  (4-hex-digit parcels).
- **Data in code:** undecodable words, and zero words inside code, fail as
  "data-or-unknown". Zero words just before the next symbol are accepted as
  linker padding (one occurrence, in the linked ELFs).
- **ELF header:** the RVC flag and the float-ABI bits are checked.
- **tcc and data in `.text`:** tcc put no data in `.text` anywhere in the
  corpus or tests2.
- **Negative tests:**
  - a gcc rv64imafdc/lp64d object: 302 RVC, 53 F/D and 9 M offenders, plus
    the header flags;
  - a hand-written object with `mul` as `.word`, garbage, zero words,
    `lr.w` and an `fcsr` access: every one is flagged.

## Notes for the image lane

- **Source:** `scripts/tcc_src.sh <dir>` produces the patched tree. Build
  tcc itself with `-DTCC_TARGET_RISCV64 -DTCC_RV64I_SOFT`, and with
  bcheck/backtrace off.
- **Link order:** tcc-generated code needs `riscv64-libtcc1.a`, then
  newlib's `libc.a` (and `libm.a` if used), then **`libgcc.a`**, all
  rv64i/lp64. A single pass in the order `objs libtcc1.a libm.a libc.a
  libgcc.a` links the corpus with plain `ld`. libgcc must come after both
  libc and libtcc1, because both call its routines.
- **`tcc -run` does not add libgcc by itself.** `tcc_add_runtime` adds `c`,
  `TCC_LIBGCC` (only when not `static_link`, and as a DLL), libtcc1, then
  `c` again. The image's tcc must load libgcc.a after that, for example
  `tcc_add_library(s1, "gcc")` after the final `c`. Archives named on the
  command line *before* the source file pull nothing, because they are
  loaded before any symbol is undefined. Alternatively, the image can
  resolve these symbols against the copies already linked into tcc itself:
  tcc is compiled by gcc rv64i, so it already contains the same libgcc
  routines.
- **Newlib headers:** see the `__INT8_TYPE__` workaround under `118_switch`
  above.
- **Console:** use ASCII output (see `83_utf8_in_identifiers` above).

## Files

- `patches/0001-tcc-rv64i-soft.patch`: the patch.
- `scripts/tcc_src.sh`: copies vendor/tinycc and applies the patches
  (idempotent; stamped by a hash of the vendor files and the patches).
- `scripts/build_cross_tcc.sh`: builds `build/cross/riscv64-tcc` and
  `riscv64-libtcc1.a`.
- `scripts/check_isa.sh`, `scripts/check_patch_neutral.sh`.
- `scripts/test_tcc_patch.sh`, `scripts/test_tcc_tests2.sh`.
- `c/tests/tcc/t01..t43_*.c`: the corpus.
- `c/tests/tcc/harness/`: `crt0.S`, `link.ld` and `htif.c`, copied from
  ship-your-lua's `c/src`. Each carries a header saying so; `link.ld` drops
  the `.lua_chunk` region, and `htif.c` adds `TCC_HTIF` to its `#if`.
- vendor fix: `vendor/tinycc/conftest.c` and `win32/include/tcc/tcc_libm.h`
  are upstream files that the vendoring commit had dropped, because
  tinycc's own `.gitignore` matches them. `configure`/`make` need
  `conftest.c`.
