# Imported runtime zero-count controls, 2026-10-28

Two exact function bodies from `csem/runtime/libc/src/string.c` disagree with
host libc on defined zero-count inputs. The full source is byte-identical to
the adopted upstream checkout at 704c4e697845127bced66c93fa0763eb51527ca4.
`subject.h` contains exact excerpts with only preprocessor name separation.

| Case | Imported body exit | Host libc exit |
|---|---:|---:|
| strncpy to a writable sentinel buffer, empty source, n=0 | 1 (first byte changed) | 0 |
| strncmp of distinct valid strings, n=0 | 1 (nonzero comparison) | 0 |
| strncpy, n=1, neighboring byte preserved | 0 | 0 |
| strncmp, n=1, expected negative sign | 0 | 0 |

The probes compile with GCC C11, no builtins and unsigned char (the target
choice). `check.py`, inputs, eight actual binaries, compiler commands/results,
source and hashes are retained. Execution is native host GCC of the exact C
bodies, **not** a Lean/Core run, full compiler Sail run or rebuilt upstream
executable. These observations concern Cerberus's imported C runtime source;
they do not establish a bug introduced by the Lean port. The checked raw
configuration currently has no loaded body for either function, so loading
those bodies must accompany repair and independent semantic validation.
No runtime source is changed by this review. Kiran owns upstream reporting.
