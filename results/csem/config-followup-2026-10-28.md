# Configuration follow-up: IEEE and stdin

The three earlier C11 configuration failures now agree exactly on fresh
patched tcc/Sail and CSem executions. These observations use the adopted
IEEE and filesystem implementations, after the anonymous-member and
preprocessing integration gate. They supersede these three classifications
in the dated `f1-config.md` measurement; the earlier raw failures are retained.

| probe | observation | compile steps | program steps | total steps |
|---|---|---:|---:|---:|
| p04_ldouble | size/alignment 16, mantissa 113, `ne=1`, exit 113 | 14,600,231 | 128,078 | 14,728,334 |
| p09_stdin | `eof=1 feof=1`, exit 0 | 13,457,977 | 3,713 | 13,461,715 |
| p10_float | exact sizes, rounding and conversion stdout, exit 1 | 14,108,757 | 19,371 | 14,128,153 |

`config-followup-2026-10-28/` retains full output, commands, timestamps,
source/image/toolchain hashes, ELF hashes and traced exit metadata. The
existing `cfg_probes.measure` helper hashes each ELF before removing its
temporary image; the ELF files themselves are not retained for these probes.
All image inputs were reverified after execution. The semantic binary and
2,284 source hashes still match the completed combined gate. The exact
content-addressed libc Core/Cabs inputs, runtime files and OCaml oracle are
also pinned, including stdin's `--libc` invocation.

These are the existing driver/text-JSON frontend observations, not a proof
of the raw frontend's semantic connections or a recertification of all fifteen
configuration probes. Bitfields remain required work; the out-of-int enum
probe remains a separately classified GNU extension. Binary128 library
formatting/sqrt discrepancies, frontend completeness, purity connections and
whole-compiler correctness remain open. No sum cut was regenerated.
