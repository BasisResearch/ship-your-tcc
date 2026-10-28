# Explicit multi-file corpus invocations

The historical standalone `104_inline.c` refusal omitted `104+_inline.c`;
the companion alone has no `main`. TinyCC's pristine tests2 Makefile runs
`104+_inline.c -run 104_inline.c`, preserving separate translation units.
A direct probe with the adopted compiler and model produces identical fast
arenas for that command. The old standalone refusals remain historical data.

The new opt-in `scripts/model_diff.py --invocations` accepts explicit manifests
such as `c/invocations/tinycc-104-inline.json`. Each manifest pins the primary
source, ordered companion files, and the upstream Makefile. Both engines see
the same independent source bytes and argument order with `-std=c11`. Companion
units are not concatenated or converted into headers. Their ordinary include
fixtures are embedded too. Conflicting virtual paths, stale source hashes,
duplicate companions and replacement of `/prog.c` are rejected. Standalone
corpus invocations remain unchanged. Reports retain the actual command and
all input hashes; successful arenas and refusals remain distinct.

The combined full gate passes with **3,583 unchanged source pins**. The final
frozen harness matches one independent fast arena and **all 4 MiB on native
Sail**, reaching handoff after **11,459,979 compiler steps**. Eight existing
comparison tests and four invocation input checks pass. Fresh standalone runs
of both files still produce matching refusals, counted as zero successes.
The 53 retained files include full native captures, both arena files, sources,
manifest, gate receipts/logs and historical special-invocation diagnostics.
Run `python3 results/model/corpus-invocations/verify.py`; `--sources` checks the
exact integration snapshot as well. This is observational evidence, not a
compiler proof or proof of NoUB.

The development probe and first native capture are retained separately. The
latter began before the manifest reader's byte-snapshot cleanup, so only the
second capture is adopted for the final harness. A temporary-path unit check
initially compared a `/Users` alias with its `/data/home` canonical path; the
corrected check resolves paths. Neither superseded result is silently promoted.

## Remaining invocation and provenance duties

This checkpoint does not establish complete multi-TU ghost provenance. The
current Ghost.runRaw interface accepts one translation unit; GhostExport does
not export complete per-unit opening logs for a multi-file command. Per-unit
construction, include ancestry, injected opening authority and cross-unit
linkage remain explicit F1/F3/F6 work. The compiler arena comparison alone
cannot prove these obligations or C-defined behavior.

The upstream Makefile also distinguishes these cases, which must not be
silently turned into ordinary standalone successes:

| Cases | Intended setup / remaining duty |
|---|---|
| `120_alias` + `120+_alias` | Two files and an executable-link step (`NORUN`), not the current `-run` harness. |
| `60_errors_and_warnings`, `96_nodata_wanted`, `125_atomic_misc` | `-dt` snippet driver and expected diagnostics; a missing `main` is not a compiler regression. |
| `106_versym` | pthread setup and executable linking; target pthread facilities remain work. |
| `113_btdll` | Builds two shared libraries and an executable with backtrace/linker configuration. |
| `124_atomic_counter` | pthread and atomic runtime support; historical diagnostics include missing atomic helper bodies. |
| `144_tls`, `146_tls_extern` | Platform TLS plus executable/object setup; historical `-run` TLS refusal and missing external TLS symbols are retained. |

These are harness/runtime classifications, not new Supported exclusions.
Only DESIGN-COMPILER.md section 5 defines semantic exclusions. The historical
1,815-input totals are unchanged, and sum's compile-cut chain remains frozen.
