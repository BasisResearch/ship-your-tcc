# Certora/clean oracles: `ccomp -interp` and the Lean Clight interpreter

Phase 2 step 1. Two of the three differential oracles, built from Certora/clean
(a CompCert 3.17 fork), configured for 64-bit RISC-V with the newlib headers of
the rv64i/lp64 multilib that tcc sees.

## Versions

| Item | Version |
|---|---|
| Certora/clean | `123f7f7767e215aceb4821f0d044336d78103ade` (2026-09-08, "docs(export): update LEAN.md"), cloned into `build/certora/src` (gitignored) |
| Local patch | `patches/certora/0001-printf-interp-lp64-and-clightgen-printf-builtin.patch` (driver code only, see below) |
| OCaml | 4.14.2, opam switch `certora-clean` (new, `--no-switch`; no other switch touched) |
| Coq/Rocq | 9.1.0 (`coq.9.1.0`, which pulls in `rocq-prover`/`rocq-core` 9.1.0) |
| Menhir | 20260209 |
| Lean | 4.32.1 (from `export/lean-toolchain`, installed through elan) |
| Preprocessor | xpack `riscv-none-elf-gcc` 15.2.0-1 with `-march=rv64i -mabi=lp64 -U__GNUC__ -E` |

## Configure and build

```
opam switch create certora-clean ocaml-base-compiler.4.14.2 --no-switch --repos=default,rocq-released
opam install --switch=certora-clean coq.9.1.0 menhir.20260209          # 358 s
scripts/build_certora_ccomp.sh     # clone, patch, configure, make -j8 all, make install
scripts/build_certora_lean.sh      # lake build CCLib Clightdefs   (85 s, 12G cap, 8 cores)
scripts/build_certora_lean.sh runner   # lake build clight_run      (54 s)
```

`build_certora_ccomp.sh` runs

```
./configure -prefix build/certora/install \
  -toolprefix ~/toolchains/xpack-riscv-none-elf-gcc-15.2.0-1/bin/riscv-none-elf- \
  -clightgen rv64-linux
```

and then edits `Makefile.config` so that `CPREPRO_OPTIONS=-march=rv64i -mabi=lp64 -U__GNUC__ -E`.
configure puts `-mabi=lp64d` everywhere by default; the assembler and linker keep
`-march=rv64gc -mabi=lp64d`, because CompCert's own RISC-V backend emits M/F/D
instructions and they are only used to build CompCert's runtime library. Neither oracle
assembles or links anything.

The first full build (Coq proofs, extraction, OCaml, runtime library) took about 8 minutes
at `-j8`. Later rebuilds after driver patches took 150 to 230 s, because rerunning configure
recompiles all the OCaml. Disk use: `build/certora` is 432 MB, `lean/ClightRun/.lake` is
222 MB and the opam switch is 1.5 GB.

### Checked ABI facts (both oracles, `c/smoke` and a header probe)

`sizeof`: char 1, short 2, int 4, long 8, long long 8, pointer 8, size_t 8,
wchar_t 4, float 4, double 8. `char` is unsigned (CHAR_MIN=0), wchar_t is signed,
little-endian. These match `riscv-none-elf-gcc -march=rv64i -mabi=lp64 -dM -E`
(`__CHAR_UNSIGNED__`, `__SIZEOF_LONG__ 8`, ...), with one exception: **long double**. gcc
and tcc use 16 bytes (IEEE quad). CompCert rejects `long double` unless you pass
`-flongdouble`, which turns it into an 8-byte double. We do not pass `-flongdouble`, so
programs that use long double are rejected (exit 120) instead of silently disagreeing.

The preprocessor gets `-std=c99 -D__COMPCERT__ -U__GNUC__ ... -I build/certora/install/lib/compcert/include`,
so `stdarg.h`, `stddef.h`, `float.h`, `stdbool.h`, ... are CompCert's own headers, and
`stdio.h`, `stdlib.h`, `stdint.h`, `string.h`, ... are newlib's from
`riscv-none-elf/include`.

The Lean `CCLib/Archi.lean` is the AArch64 one. The parameters it uses (ptr64, little
endian, `align_int64 = align_float64 = 8`, no splitlong) are the same as `riscV/Archi.v`
for rv64, so it is correct for this target. The NaN parameters differ in principle; see the
false-disagreement list.

## The patch (driver/front-end only; nothing proved changes)

1. **`clightgen -fprintf-builtin`** (new option, `Clflags.option_printf_builtin`).
   CompCert's Clight (Rocq, and the Lean port) **cannot execute a call to a variadic function
   that passes more arguments than the prototype has**. `eval_exprlist` needs exactly one
   parameter type per argument, and the call's function type is the prototype
   `Tfunction [tptr tuchar] tint {cc_vararg := some 1}`, so `printf("%d\n", x)` is stuck at
   `Scall`. `ccomp -interp` gets around this in `C2C.ml`: under `-interp`, every `printf(...)`
   call becomes `Ebuiltin(EF_external("printf", sig of the actual argument types), ...)`. The
   new flag makes clightgen do the same, so the Clight we export has
   `Sbuiltin none (EF_external "printf" sg) [actual types] args`. That is an external call
   CompCert compiles normally, and it is exactly what -interp executes.
2. **`ccomp -interp` printf** (`driver/Interp.ml`), so that it renders like C/newlib:
   - `%ld %lu %lx %zu %zd %td`: on a 64-bit target the argument is a `Vlong`, and stock
     -interp printed `<int argument expected>`;
   - `%hd`/`%hhd`/...: the value is truncated to short or char, as C does;
   - `*` width and precision: stock -interp printed the spec literally;
   - `%F` and `%A`: unrecognised in stock -interp;
   - width, `-` and precision for `%s`: stock -interp ignored them;
   - a printf whose arguments are not all "event values" (for example `%s` of a local
     buffer) no longer gets stuck *after* printing. The trace event just records no
     arguments.

## Oracle 1: `scripts/run_ccomp_interp.sh prog.c`

Runs `ccomp -interp -quiet $CERTORA_CFLAGS -I<dir> prog.c`, where
`CERTORA_CFLAGS=-fstruct-passing` (struct/union arguments and results by value). Other
language options: `-fvararg-calls` and `-funprototyped` are on by default. `-fbitfields` is
ignored (bit fields are native in 3.17). `-funstructured-switch`, `-fpacked-structs` and
`-finline-asm` stay off. stdout is exactly the program's output. The script strips the
interpreter's "Stuck state ..." diagnosis and sends it to stderr.

Exit codes: the program's status (low 8 bits of main's result), or **120** if ccomp rejected
the program, or **126** if it got stuck (undefined behaviour, or any external other than
printf).

What -interp supports. It executes **CompCert C** (`Cexec`, one level above Clight) with
`-random` off, so it uses the first evaluation order.
- **Externals:** only `printf`, through `Interp.do_printf`, which renders each conversion
  with OCaml's `caml_format_float`/`caml_int*_format`, i.e. the host glibc snprintf.
  `putchar`, `puts`, `exit`, `memcpy`, `strlen`, ... are stuck. `malloc`/`free` work (C2C
  maps them to the EF_malloc/EF_free builtins), and so do struct copies
  (`__builtin_memcpy_aligned`).
- **Volatile:** works on globals (as vload/vstore events against an initial "world").
- **Doubles and floats:** Flocq, exact IEEE with RISC-V NaN rules. **long long:** yes.
  **Bit fields:** yes, but see "uninitialised container" below.
  **Struct passing:** yes, with `-fstruct-passing`. **User-defined variadic functions:** stuck
  at the call (same reason as above).
- `int main(int, char**)` gets argc=0 and argv=0.

## Oracle 2: `scripts/run_lean_clight.sh prog.c`

1. `clightgen -lean -normalize -fprintf-builtin $CERTORA_CFLAGS -I<dir> -o Prog.lean prog.c`
   (the source is copied to `prog.c` so that the Lean namespace is always `Prog`);
2. `lean/ClightRun/.lake/build/bin/clight_run Prog.lean`.

Exit codes: the program's status, or **120** if clightgen rejected the program, **124** for
out of fuel (`--fuel N`; the default is unbounded), **125** for a harness error (the
generated file does not elaborate, ...), **126** for stuck (UB, or an unknown
external/builtin, volatile access, user varargs), **127** if the program is outside the
printf model (`%p`, `%n`, long double conversions). A one-line `[clight_run] ...` reason goes
to stderr. Options: `--stats` (steps, load_ms, run_ms), `--keep`, `--lean FILE` (save the
generated Lean), `-- extra clightgen flags`.

### How it runs a program, and why it is cheap

Their harness (`test/lean/diff_exec.sh` with `export/DiffRun.lean`) compiles every generated
file with `lean -o` and then runs `lean --run DiffRun.lean`, all in the IR interpreter. It
prints only what `main` returns: their 16 + 500 program corpus has **no printf at all**.

`clight_run` (in `lean/ClightRun`, a Lake project that depends on `export/` by path) is a
natively compiled executable with `supportInterpreter`. Per program it:
- imports the prebuilt `Clightdefs`/`CCLib` .oleans;
- elaborates the generated file in-process (`Lean.Elab.process`; no lake, no .olean written);
- evaluates `Prog.prog : CC.Program` with `evalConst`;
- runs the **natively compiled** `CC.doStep` in a tail-recursive loop.

Cost per program is about 0.5 to 0.8 s fixed (import plus elaboration, ~500 MB RSS) plus
about 1.5 to 2 million Clight steps per second. On `c/smoke/heavy.c` (26.7M steps)
clight_run takes 14 s at 513 MB, while `ccomp -interp` takes 236 s and peaks at **5.3 GB
RSS**. -interp is the slow, memory-hungry oracle: give the harness per-program timeouts and a
memory cap.

### External calls on the Lean side (printf)

CCLib leaves external functions uninterpreted, as CompCert does: the axioms
`externalFunctionsSem` and `inlineAssemblySem`, plus their `ec_determ` versions. `doStep`
executes only `EF_malloc`, `EF_free`, `EF_memcpy` and `EF_debug`, and returns `none` (stuck)
at every other external. The step relation emits trace events, but the interpreter never
produces any: there is no `Event_syscall` to convert.

So `clight_run` adds a small, explicit **external-call model**
(`lean/ClightRun/ClightRun/Run.lean`) on top of `doStep`, which it does not change. When the
run is stuck at
- `State f (Sbuiltin optid (EF_external name sg) tyargs al) k e le m` (printf from
  `-fprintf-builtin`), it evaluates the arguments with CCLib's own `doEvalExprlist`, performs
  the call and continues with `State f Sskip k e (setOpttemp optid vres le) m`. This is
  Clight's `step_builtin`;
- `Callstate (External (EF_external name sg) ..) vargs k m` (an ordinary external call), it
  continues with `Returnstate vres k m`. This is `step_external_function`.

Memory is unchanged in both cases. These steps are instances of the Clight rules **if** the
uninterpreted `externalFunctionsSem name sg` admits that behaviour. That premise is trusted
and is not covered by `doStep_sound`. For Phase 3 it is the natural place for the stated
premise linking printf's machine spec to Clight's external-call axiom.

Modelled externals: `printf`, `putchar`, `puts` (returns 0, like newlib) and `exit`. Of these,
`ccomp -interp` has only printf, so in the common subset only printf matters.

printf model: the format string and `%s` arguments are read from Clight memory byte by byte
(`Mem.load Mint8unsigned`; an undefined byte means stuck). Each conversion's flags, width
(including `*`), precision (including `*`) and length are parsed in Lean, the argument is
converted to the width the length modifier names, and that single conversion is rendered by
the host **glibc snprintf** through a 50-line C FFI (`lean/ClightRun/c/fmt.c`):
- integers: hh/h/none/l/ll/j/z/t map to 8/16/32/64 bits, signed for d/i, unsigned for
  u/o/x/X; a 32-bit Vint at width 64 is sign-extended, as lp64 registers are;
- `%c`; `%s` (read up to the precision); `%f %F %e %E %g %G %a %A` with a `Vfloat` argument.
Unsupported: `%p`, `%n`, `L`. The return value is the byte count. Since -interp also renders
through host snprintf, the two oracles agree on float formatting by construction.

## Results

### Smoke tests (`scripts/certora_smoke.sh`, full output in `results/certora_smoke.txt`)

| program | interp rc | lean rc | gcc rc | interp ms | lean ms | Clight steps | verdict |
|---|---|---|---|---|---|---|---|
| hello | 0 | 0 | 0 | 46 to 151 | 515 to 775 | 6 | agree |
| loops (int/unsigned/long long, %x %o %ld %lu %hhd ...) | 209 | 209 | 209 | 65 to 105 | 668 to 1136 | 14,105 | agree |
| structs (nested, copy, by-value args/results, unions) | 0 | 0 | 0 | 49 to 141 | 712 to 1061 | 126 | agree |
| doubles (%f %g %e %.17g, conversions) | 0 | 0 | 0 | 60 to 139 | 760 to 997 | 200 | agree |
| funptr (tables of function pointers, fib(20)) | 55 | 55 | 55 | 325 to 402 | 813 to 1461 | 221,233 | agree |
| printf_formats (~200 conversions: flags, `*`, %s width) | 0 | 0 | 0 | 59 to 88 | 1004 to 1401 | 259 | agree (3-way, after the -interp patch) |
| bitfields (global and initialised container) | 0 | 0 | 0 | 59 to 119 | 701 to 1099 | 60 | agree |
| malloc (linked list, malloc/free) | 0 | 0 | 0 | 113 | 1012 | 459 | agree |
| heavy (26.7M steps) | 0 | 0 | 0 | 197,840 to 238,264 | 11,021 to 21,711 | 26,686,077 | agree |
| ub (out-of-bounds read) | 126 | 126 | 0 | | | | both stuck after "before" |
| varargs (user variadic function) | 126 | 126 | 0 | | | | both stuck (outside both) |
| bitfield_uninit | 126 | 126 | 0 | | | | both stuck (CompCert semantics, see below) |
| putchar (putchar/puts/%s of a local buffer) | 126 | 3 | 3 | | | | -interp has no putchar; Lean = gcc |
| nan | 0 | 0 | 0 | | | | **Lean port differs**: `-NaN` |

Timings vary by up to 2x with load on the shared box.

### Validation against the Certora harness's own corpora (exit status only, no output)

- `test/lean/corpus/*.c` (their 16 hand-written programs): **16/16 agree** (and agree with gcc).
- `gen_random_c.py out 100 7` (their UB-free generator): **100/100 agree**. Mean 56 ms for
  -interp and 737 ms for the Lean oracle.

## Things that will produce false (or uninteresting) disagreements in the three-way differential

1. **Bit-field stores into an uninitialised container are stuck in CompCert's semantics.**
   `struct bf f = {5, -3, 200};` for a local is a read-modify-write of an undefined word, so
   both oracles exit 126 and tcc prints. Csmith generates exactly this often: run Csmith with
   `--no-bitfields`, or treat "both oracles stuck" as excluded.
2. **Variadic functions other than printf** (user-defined `va_arg` code, `fprintf`,
   `sprintf`, `snprintf`) are stuck in both CompCert oracles: Clight cannot pass extra
   arguments. Only printf is special-cased.
3. **libc beyond printf/malloc/free** (`memcpy`, `memset`, `strlen`, `strcmp`, `abs`, math
   functions, `exit`) is stuck in -interp. The Lean oracle also has putchar, puts and exit.
   Csmith's `main` tests `argc == 2 && strcmp(argv[1], "1")`, but argc=0 short-circuits it.
   Check that the Csmith runtime header the corpus uses needs nothing beyond printf.
4. **NaN sign.** Lean's `Float.toBits` canonicalises every NaN to `0x7ff8000000000000`
   (observed), and CCLib routes every float operation through it. For arithmetic results this
   happens to match CompCert's RISC-V rule (default NaN positive), but `-x` of a NaN gives
   `0x7ff8...` in the Lean port and `0xfff8...` in CompCert (`printf("%f")`: `nan` vs `-nan`).
   This is a real (small) Lean-port deviation, the first finding of the differential. tcc's
   NaN sign depends on libgcc soft-fp and how tcc negates. Exclude NaN printing, or normalise
   `-nan` to `nan` when comparing with tcc.
5. **printf rendering, newlib vs glibc.** Both CompCert oracles format with the *host glibc*.
   tcc on Sail uses newlib's printf. Expect differences in `%a` (hex-float normalisation),
   `%p`, possibly `%e`/`%g` of subnormals or huge `%f` values, depending on the newlib
   build's float printing (`_PRINTF_FLOAT`/nano), and the `-nan` spelling. Restrict corpora
   to `%d %i %u %x %X %o %c %s %ld/%lld` variants, plus `%f %e %g` with modest precision.
6. **`%p` and printed pointer values** cannot be compared (-interp prints `<blk+ofs>`, Lean
   refuses with 127).
7. **Evaluation order / unspecified behaviour:** -interp picks the first order allowed by
   CompCert C; Clight is already determinised by clightgen's SimplExpr. They agree with each
   other but may differ from tcc's order. That is unspecified behaviour, not a bug.
8. **Exit status** is compared mod 256; `ccomp -interp` exits with main's int result
   truncated by the OS, like a native process.
9. **Undefined behaviour that tcc executes happily** (signed overflow, out-of-bounds,
   uninitialised reads, pointer comparisons across blocks): both CompCert oracles get stuck
   (126). Use that as the corpus UB filter, as the brief says.
10. **long double**: rejected by both (120); tcc uses quad. **VLAs, `_Complex`, `_Atomic`,
    inline asm, unstructured switch (Duff's device), packed structs:** rejected or stuck.
11. **Volatile**: -interp executes volatile globals; the Lean interpreter is stuck (126).
    Csmith: `--no-volatiles`.
12. The Lean oracle's per-program cost is dominated by its ~0.6 s fixed load. -interp is fast
    on small programs and very slow/memory-hungry on long-running ones (5.3 GB at 27M Clight
    steps).

## Paths (for the other lanes)

Built under this worktree's `build/` (gitignored):
- `build/certora/install/bin/ccomp`, `build/certora/install/bin/clightgen` (with `install/share/compcert.ini`)
- `build/certora/src/export/.lake/build/lib/lean` (CCLib/Clightdefs .oleans)
- `lean/ClightRun/.lake/build/bin/clight_run`
- `scripts/certora_env.sh` exports `CCOMP CLIGHTGEN CLIGHT_RUN CERTORA_LEAN_PROJECT LEAN_SYSROOT CLIGHT_LEAN_PATH CERTORA_CFLAGS`.

From another worktree, either call this worktree's scripts directly
(`/data/home/kirancodes/Documents/code/ship-your-tcc/.claude/worktrees/agent-a39962f25417ac6c6/scripts/run_*.sh`),
or, after merging the scripts, export
`CERTORA_DIR=<this worktree>/build/certora` and `CLIGHT_RUN=<this worktree>/lean/ClightRun/.lake/build/bin/clight_run`
before calling your copy.
