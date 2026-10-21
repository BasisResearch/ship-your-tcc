# F1 digest seam recovery — 2026-10-21

Recovered the uncommitted work in `agent-a249bbaa5b1df80ec`, reviewed the
MD5 compression/padding and digest threading, and completed integration.

`CerberusFresh.md5Hex` is now a total Lean definition over UTF-8 bytes.
The mutable native translation-unit digest becomes a `Digest` instance
parameter. `frontend/model/lean_digest.lem` declares the reader; the pinned
Lem generator propagates it through symbol creation, desugaring and
translation. The driver and `Tcc.CSem.verdicts` supply the particular TU's
digest explicitly. The native MD5 object, global reader, forcing barrier
and `CSemNative` Lake package are removed.

Review fixed stale generated copies and propagated the new parameter into
the existing translation fuel lemmas and unit tests, without changing their
proof bodies or budgets. Driver instance scope encloses the entire IO
computation. The fork's opaque and native-seam population pins shrink;
reintroducing a removed site fails the census.

## Validation

- `scripts/csem_regen.sh` followed by `scripts/csem_gates.sh`: pass on
  Lean 4.34.0, with the shared lock and 12 GB systemd scope. The final
  incremental build used 3,451 MiB peak, 181 CPU seconds, 289 wall seconds
  including lock contention; this is not a from-scratch measurement.
- All 19 fork unit executables pass. `fresh-int-test` checks explicit
  cross-TU symbol identity and 18 MD5 vectors (RFC vectors, UTF-8 and
  padding boundaries). Empty-string and `abc` vectors are also computed
  by `decide +kernel`; fresh symbols carry the supplied digest by `rfl`.
- Purity plants: 17; totality plants: 8 plus 2 clean controls. Axiom census,
  sorry-token census and the remaining two-opaque population pin pass.
- `scripts/csem_regen.sh --check`: all 227 generated files reproduced.
- OCaml differential: 90 MATCH, 18 UB_MATCH, 5 CERB_SKIP; all 113 results
  retain the fork's baseline.
- Capped `lake env lean scripts/csem_cone.lean` succeeds;
  `Tcc.CSem.verdicts` depends only on `propext`, `Classical.choice`,
  `Quot.sound`.

## Remaining seams

This closes the digest seam only. The diagnostic dependency walk reports:

| cone | partial | other opaque | extern opaque | implemented_by | unsafe | axioms |
|---|---:|---:|---:|---:|---:|---:|
| kernel dependencies | 43 | 11 | 5 | 13 | 0 | 3 |
| also enter partial bodies | 100 | 15 | 9 | 14 | 0 | 3 |

Counts include Lean primitives. The walk does not enter `implemented_by`
bodies, so it is an inventory, not a runtime-purity certificate. Native
floating point, `lemSeq`/failure implementations and partial frontend/Core
parsers remain F1 work. No compiler correctness theorem is claimed here.
