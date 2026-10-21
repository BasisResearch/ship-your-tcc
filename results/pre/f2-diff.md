# F2 (spec half): `CSem.Pre.phases14` against `tcc -E` and `gcc -E`

Phase F2's first exit criterion (DESIGN-COMPILER.md §3.2, §9): the
preprocessing specification agrees with tcc token for token on every corpus
file and on Phase 1's header program, and the standard's own examples hold
as unit tests. Raw per-file results: `results/pre/f2-diff.tsv`
(`scripts/pre_diff.py --corpus-dir <checkout>/corpus --out …`).

## What was compared

* **Spec:** `.lake/build/bin/pre_spec --cfg E` = `CSem.Pre.phases14 Tcc.cfgTccE`
  with the image's header tree (c/build/fs.list), the file as `/prog.c`.
  `cfgTccE` is generated (scripts/gen_pre_cfg.py) and differs from the
  image's `cfgTcc` only in the predefined text: `__TCC_PP__` for
  `__TCC_RUN__`, no TCC_BAREMETAL `<stdint.h>` type macros (the cross tcc is
  not built with TCC_BAREMETAL). Its `#define` lines are checked equal, in
  order, to the cross tcc's `-dM -E` listing.
* **tcc:** the cross tcc (scripts/build_cross_tcc.sh, same sources and
  patches as the image) `-E -nostdinc -I<root>/lib/tcc/include -I<root>/include
  <root>/prog.c`, where `<root>` mirrors the image's file system (symlinks to
  c/build/fs) — the same search order as the image's
  `{B}/include:/include`.
* **gcc:** xpack `riscv-none-elf-gcc -E -undef -nostdinc -std=c99`, same
  `-I`, `-include predefs.h` (cfgTccE's predefined text without the three
  `__STDC*__` macros gcc defines itself).

**Normalisation rule** (scripts/pre_diff.py): line markers dropped;
a `#pragma` line becomes a pragma marker around its tokens; the text is
decomposed into preprocessing tokens by the spec's own phase 3
(`pre_spec --lex`) and white space dropped; `<root>` deleted inside string
literals (host path → image path); `__DATE__`/`__TIME__` taken from the host
tcc. A difference that is only the -E printer gluing two tokens would count
as "spacing": it never occurred. Both sides failing counts as agreement.

## Results (581 files, 2.95 M spec tokens)

| corpus | files | = tcc | = gcc | both fail | spec fails | differ |
|---|---|---|---|---|---|---|
| corpus/compcert | 72 | 72 | 72 | | | |
| corpus/csmith | 200 | 200 | 200 | | | |
| corpus/tcc (tests2 subset, Phase 2) | 73 | 73 | 72 | | | gcc 1 (b) |
| tinycc tests/tests2 (all) | 142 | 137 | 133 | 5 (a) | | gcc 4 (b, c) |
| tinycc tests/pp | 21 | 19 | 19 | | 2 (d) | |
| c/tests (run incl. headers.c, tcc, p3, …) | 57 | 57 | 57 | | | |
| c/tests/pre (torture tests, this phase) | 16 | 10 | 13 | | | tcc 6, gcc 3 (below) |

Phase 1's 18-header program `c/tests/run/headers.c` (12,356 tokens) agrees
with both. The C11 §6.10 examples (§6.10.3.5 EXAMPLES 1–7, §6.10.3.3p4,
§6.10.3.4p4, §6.10.9p2) are `#guard` tests in `Spec/Pre/Test.lean`, run under
both `cfgIso` (plain ISO C11) and `cfgTcc`; as a file
(`c/tests/pre/std_6_10_3_5.c`) they agree with tcc and gcc too. For
§6.10.3.4p4's unspecified `f(2)(9)` all three give `2*9*g`.

(a) both fail, a harness limit, not a disagreement: `#include` of files the
image does not have (`<signal.h>`-style host headers in 114, 145; `SELF`,
`../tests2/…` set up by tinycc's Makefile in 95, 18; 119).

(b) gcc's -E printer writes UTF-8 identifiers as UCNs (`\U0000043f…`):
normalisation, gcc only (83_utf8_in_identifiers and its corpus copy).

(c) gcc predefine: gcc's built-in `__has_extension(c_atomic)` is 1, so
newlib's `<sys/cdefs.h>` does not define `_Atomic(T)` as a struct
(124/125/136 atomic tests). Not a spec matter.

(d) the spec rejects undefined behaviour tcc accepts — no obligation for
`tccpp_correct` (Spec/Pre/Compare.lean: a spec error carries none):
* pp/21.c `#if 1 << 63 < 2` and `defined DDD << 63`: signed overflow in
  intmax_t (§6.5.7p4); tcc wraps. With those two lines removed the file
  agrees with tcc and gcc.
* pp/22.c: `#if` inside a macro's argument list (§6.10.3p11, undefined);
  tcc and gcc process it, the spec stops (the invocation is unterminated at
  the directive).

## The torture tests (c/tests/pre/) and every mismatch

| test | vs tcc | vs gcc | classification |
|---|---|---|---|
| std_6_10_3_5, hideset, placemarker, include_forms, gnu_ext, misc, pushpop, prefixes, local_include, tccdefs_run | = | = | — |
| digraphs | differ | = | **tcc deviation** 1 |
| pragma_op | differ | differ | **tcc deviation** 2 (`_Pragma`); vs gcc: gcc prints `#pragma pack(N)` unexpanded, tcc and the spec expand it (implementation-defined, §6.10.6p1) |
| ucn_ident | tcc error | differ | **tcc deviation** 3; vs gcc: gcc prints the UCN identifier in UTF-8 (normalisation) |
| line | differ | differ | **tcc deviation** 4 (`#line` file name); vs gcc: `__LINE__` inside a macro invocation spanning lines — gcc gives the line of the `__LINE__` token, tcc and the spec the line of the token that completes the invocation (unspecified, §6.10.8.1) |
| stringize | differ | = | **tcc deviation** 5 |
| ifexpr | differ | = | harness: `#if '\377' < 0` — the cross tcc computes a character constant with the *host's* `char` (x86: signed); on the image (RV64 host, unsigned `char`, tccpp.c `parse_string`) it is 255, as in the spec |

`prefixes.c` agrees only because tcc runs as C99 (`__STDC_VERSION__`
199901L): C99 has no `u8`/`u`/`U` prefixes, so `u8"a"` is an identifier and a
literal there. The spec has this as `PreCfg.c11Prefixes` (false for tcc,
true in `cfgIso`). CSem (Cerberus) is C11, so this is a scope question for the
headline theorem rather than a preprocessing bug: a C11 program using those
prefixes is not a C99 program, and tcc compiles C99 by default.

## tcc deviations from the standard (counterexamples to `tccpp_correct`)

Each has a runnable reproducer in `results/repro/` (expected output = gcc
`-std=c11`); none was filed anywhere. All five are in pristine tcc 43c7708
(tccpp.c is untouched by our patches except for the predefined type macros).

1. **Digraphs** are not recognised (C99/C11 §6.4.6p3): `<:` lexes as `<` `:`,
   and `%:define` is not a directive. `tcc_pp_digraphs.c`: expected `2 3`;
   tcc on the Sail image: `error: declaration expected`.
2. **`_Pragma`** is not implemented (C99/C11 §6.10.9): it reaches the parser as
   an identifier. `tcc_pp_pragma_operator.c`: expected `ok`.
3. **UCNs in identifiers** are rejected, "stray '\' in program" (C99/C11
   §6.4.2.1, §6.4.3). `tcc_pp_ucn_identifier.c`: expected `3`.
4. **`#line N "name"`** with a relative name sets `__FILE__` to the real file's
   directory + name (`tccpp_putfile`), not "name" (§6.10.4p4, §6.10.8.1).
   `tcc_pp_line_relative_name.c`: expected `foo.c 10`, tcc `/foo.c 10`.
5. **`#` stringizing** writes every byte outside 32..126 as an octal escape
   (`add_char`); §6.10.3.2p2 escapes only `"` and `\`. The string value of one
   stringization is the same; stringizing again shows the spelling.
   `tcc_pp_stringize_escapes.c`: expected `9`, tcc `15`.

`scripts/run_pre_repros.sh` runs them under `tcc -run` on the Sail image;
results in the next section.

## Image runs of the reproducers

`scripts/run_pre_repros.sh` (`tcc -run /prog.c` on the Sail RV64I image,
2026-10-21); expected = gcc `-std=c11` natively:

| reproducer | expected | tcc on the image |
|---|---|---|
| tcc_pp_digraphs.c | `2 3` | rejected: `/prog.c:9: error: declaration expected` |
| tcc_pp_pragma_operator.c | `ok` | rejected: `/prog.c:9: error: identifier expected` |
| tcc_pp_ucn_identifier.c | `3` | rejected: `/prog.c:8: error: stray '\' in program` |
| tcc_pp_line_relative_name.c | `foo.c 10` | `/foo.c 10` |
| tcc_pp_stringize_escapes.c | `9` | `15` |

The `ifexpr` classification was checked the same way: `#if '\377' < 0` in
a program run by the image's tcc takes the unsigned branch, as the spec
does (the cross tcc's signed answer is its x86 host's `char`).

## Known limits of the spec (choices recorded, not hidden)

* A directive inside a macro's argument list is rejected (undefined,
  §6.10.3p11); tcc and gcc execute it.
* A `_Pragma` whose pragma is phase 4's own (`once`, `push_macro`,
  `pop_macro`) takes effect at the end of the text run containing it (the
  standard has none of these; implementation-defined).
* `#include_next` from a file not found through the search list (current
  directory, absolute path, the main file) searches the whole list; an
  absolute `#include` name is tried as given only (tcc also tries it after
  each search directory, which only matters for a missing file).
* `__LINE__` in a multi-line invocation is the line of the token completing
  it (tcc's choice; unspecified).
* Universal character names are accepted in identifiers without checking
  the Annex D ranges.
* The fuel bound (`fuelBound`, 64·n² for n input bytes) is a resource
  premise: exhausting it is `ErrKind.fuel`, which `Fits` must exclude.
