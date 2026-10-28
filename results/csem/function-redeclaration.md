# Function identity across valid redeclarations (CSEM-10)

The local Lean entry seam now reserves symbol number zero for NULL. Both the
raw CSem elaborator and historical driver begin their C symbol stream at the
shared `CerberusFresh.initialSymbolSupply = 1` and continue threading the same
supply across translation and execution. Declaration resolution, linkage,
Supported/Fits, proxy filtering, allocation policies, physical anonymous members
and function-pointer encoding are unchanged. Generated handwritten copies came
from the pinned Lem recipe; no generated output was edited directly.

## Exact cause and first divergence

The retained neutral static `mine` case in
[library-identity/pointer-adopted/neutral.c](library-identity/pointer-adopted/neutral.c)
is sufficient. The old Core quotation contains one canonical `mine` symbol,
number **0**, in the declaration and every function pointer. Compatible
redeclarations reuse it through `register_ordinary_identifier` and
`register_function_declaration`; `translate_function_designator` preserves it.
There is no loss or replacement of the canonical symbol at redeclaration.

Pinned independent OCaml debug execution registers, defines and looks up the
same `mine` symbol **483** on the identical positioned input. Its runtime loading has
already drawn fresh symbols. Ordinary preprocessing additionally injects builtin
declarations and gives `mine` number 502; both debug traces are retained.
Lean's handwritten Core runtime parser instead uses separately hashed symbols,
but its C translation entry previously seeded the supply at zero. This is the
first relevant difference in the symbol/address assignment. In a definition
with a named parameter, the parameter may consume zero first, explaining why
some declaration orders previously passed. Prototype-first translation exposes
zero as the function symbol; an extra trailing declaration is unnecessary.

`CerbMem.memValueToBytes_lemFuel` and its equivalent append implementation
serialize `PVfunction (Symbol digest n name)` as the unsigned bytes of **n** and
register n in `funptrmap`. `reconstructValueAbst_lemFuel` recognizes all-zero
pointer bytes as `PVnull` *before* function-map lookup. Thus canonical symbol 0
is stored correctly but reloads as NULL: this reload is the first C-observable
semantic divergence from OCaml. The driver reports exactly
``ill-formed program: `null function pointer'``. The failure also affects
pointer equality and null tests without a call. Altering name resolution,
proxy selection or null decoding would address the wrong boundary.

The repaired quotation retains canonical symbol **1**. Its byte representation
is nonzero, lookup reconstructs the original digest/id/name and the indirect
call reaches the definition. [trace.json](function-redeclaration/trace.json)
records the quotation hashes, extracted symbol numbers and old executable pins;
the complete independent OCaml debug trace and pre-repair failure remain in
[before/](function-redeclaration/before/). The prior neutral and proxy-named
inherited-gap observations under library-identity remain untouched.

## Comparisons

The maintained checker `scripts/check_function_redeclaration.py` generates
32 probes. Independent pinned Menhir/OCaml consumes the raw frontend's
positioned preprocessing view through the existing cpp hook; full Cabs and
source MD5 agree for **32/32**. This keeps runtime symbol numbering independent
while preventing injected builtin declarations from hiding the zero seed.
Every command, source, stdout/stderr stream and executable hash is retained.

- **27 defined programs** return zero with raw Lean, historical Lean, pristine
  pinned OCaml and strict GCC (`-std=c11 -pedantic-errors -fno-builtin -O0`).
  They cover internal/external linkage; prototype/definition/redeclaration
  orders before and after main; direct/indirect calls; nested same-name extern
  declarations and an object shadow; distinct functions; address, equality and
  null tests; automatic/static/global/array/struct function-pointer objects;
  assignment and copying their object representations through unsigned char.
- **One defined internal-linkage proxy-name program** returns zero in raw Lean
  and GCC. Historical Lean and pinned OCaml retain their independently known
  `printf_proxy` driver failure. It is explicitly `HISTORICAL_PROXY_GAP`, not a
  four-engine agreement and not the CSEM-10 defect.
- **Three invalid constraint controls** (incompatible parameters, incompatible
  return type and duplicate definitions) are rejected by all three frontend
  engines and strict GCC. A fourth invalid function/object name conflict is
  incorrectly accepted by both CSem paths and pristine OCaml; GCC and patched
  tcc reject it. It is `INHERITED_CONSTRAINT_GAP`, outside the success count.

The [after matrix](function-redeclaration/after/results.json) has 30 successful
common rows (27 defined plus three constraints) and those two distinct retained
gaps. Its raw path returns zero for all **28 defined programs**. The maintained
library-identity matrix now requires **24/24 PASS**, including both formerly
retained pointer residuals; all **5,580** consumer-location checks still pass.
The original 22+2 report remains historical evidence.

Fresh patched cross-tcc and xPack GCC compile and execute all **28 defined
programs on Sail**, with matching empty stdout and zero exit; all object and
linked-image ISA checks pass. Patched tcc rejects all four invalid controls.
[sail/results.json](function-redeclaration/sail/results.json) pins sources,
objects, linked images, compiler, patches and emulator; complete traces remain.
A fresh **full compiler** `tcc -std=c11 -run /prog.c` execution of the original
neutral case also exits zero, with the three trace cuts at 3,382,570 / 3,382,595 /
3,382,698. Its fresh embed links unchanged worktree runtime objects, without
rebuilding the base image or any sum cut. [native/result.json](function-redeclaration/native/result.json)
retains image/source/runtime hashes, exact execution command, ISA result and
streams. No old runtime observation is relabeled as fresh.

## Validation and limits

The first frozen capped build passed in receipt
`redecl-initial-f5a54900b60ec6153907`; all 2,746 source hashes were verified
before edits resumed. The seed positivity theorem and general positive-supply
draw theorem elaborate at normal budgets and depend on no axioms. The root
axiom audit includes the seed theorem. Five fresh-symbol/MD5 test groups and
all **11 raw frontend tests** pass. The full `scripts/check.sh --csem` gate passed in receipt
`redecl-gate-43da13328e6a43ac3576`, with all 2,746 frozen source hashes verified.
It reran allocation controls/default equivalence, anonymous members, library
identities, focused redeclarations, 235-file pinned regeneration, historical
differential, purity and axiom audits together. All probes stayed within normal
budgets and the three standard axioms; both new supply facts use no axioms.
The earlier gate receipt stopped only at the missing local oracle cache and
is retained separately.

This is a finite CSEM-10 regression repair, not a proof of C11 frontend
correctness, runtime-image refinement or compiler correctness. Generic supply
preservation through elaboration and eventual 64-bit representability of all
symbol-based function addresses remain whole-pipeline/resource obligations.
The existing function-address numbering deviation (N1), cross-TU address model,
partial frontend/Core seams, historical proxies, CSEM-11 function/object
constraints, bitfields, aggregate-expression initializers and the other
documented gaps remain. Allocation resolutions/default semantics and image identity connections
are preserved; no new ConnectionObligations instance or NoUB witness is claimed.
Lua inventory/mandatory table was consulted; no machine-library proof was reused
or claimed instruction-identical. Toolchain and oracle caches remained read-only.

Suggested ledger text (STATUS/PHASES were not edited): CSEM-10 is repaired at
both local Lean C-translation entries by reserving NULL symbol number zero.
Valid redeclarations retain canonical function identity through local storage,
equality and indirect calls. Focused independent comparisons, library-identity
recheck and fresh Sail observations pass with explicitly separated historical
proxy and inherited constraint gaps. General frontend/supply/address and
whole-compiler proofs remain open.

## Dead ends retained

- Redeclaration translation is not the failing operation: both Core values and
  function definitions retain the same canonical symbol. No runtime-library
  identity change, Supported restriction or special function-name rule helps.
- Ordinary oracle preprocessing injects builtin declarations and consumes C
  symbol ids, masking the zero seed in historical Lean. The initial ordinary
  preprocessing matrix is retained in `before/matrix/`; only its raw semantic
  failures establish the regression. Its historical successes do not refute it.
- Bare `--cpp=cat` receives automatic `-I` flags and fails before parsing. Its
  `before/exact-matrix/` captures are harness failures, not semantic outcomes.
  The final checker uses the existing independent positioned-input hook and
  verifies full Cabs and MD5 instead.
- Do not count the historical printf proxy error or invalid function/object
  acceptance as identity success. Both remain exact retained classifications.
- The local systemd user bus denies the capped build. Matching frozen requests
  and lead receipts supply capped builds; no uncapped fallback was attempted.

The first full-gate attempt (`redecl-gate-85f7a69462a72436de48`) stopped at
missing matching-oracle cache, after the ordinary build/audits and unit checks.
Its frozen hashes verified. The existing reviewed integration oracle at content
key `9e775864dfd595f06f61463e4ff16d6664b5aa7dea800e471cd821462c73602f`
is reused through a worktree-local link after checking artifact and overlay
manifests. The cache itself remains read-only. Lead review requested and received
a separate CSEM-11 entry with the invalid source and exact observations; no gap
was relabeled as identity success.

The complete large streams and GCC executables in `before/matrix`,
`before/exact-matrix`, `after` and `library-recheck` are compressed as
`streams.tar.xz`, each with an original-member SHA-256/size manifest. Every
member was independently rehashed before loose copies were removed. Sources,
commands, result summaries, minimal driver failures and both Core quotations
remain directly readable. `tar -xJf streams.tar.xz -C DIR` restores streams at
their recorded paths. Complete successful/failed build logs are compressed in
`checks/build-logs.tar.xz`; concise check logs remain alongside it.

Final verification: `python3 results/csem/verify_function_redeclaration.py`
checks the matching completed receipt, frozen sources, current executable and
image pins, archive members, exact matrix classifications and final evidence
hashes. No original library-identity evidence, STATUS or PHASES was edited.
A simultaneous lead/worker cache-link setup briefly created a redundant nested
symlink; it was removed, and the six oracle artifact/overlay hashes reverified.
No cached source or binary bytes changed. Lead-requested CSEM-11 documentation
and separate cross-program/full-compiler Sail labels are complete.

## Adopted combined integration, 2026-10-28

The repair is combined with the adopted typed ghost checkpoint, shared corpus
fixtures and comparison isolation. The full CSem gate passes with all 2,304
tracked input hashes unchanged. The maintained declaration matrix again has
30 common passing rows (27 defined programs and three rejected constraints),
one historical proxy gap and one inherited CSEM-11 constraint gap. All 28
defined raw programs succeed; library identity is 24/24 with 5,580 consumer
checks. Allocation/anonymous regressions, regeneration, purity and axiom audits
pass in the same gate. Exact combined binaries, the complete fresh matrix and
gate/source pins are in function-redeclaration-integration-2026-10-28/.
The earlier native observations are retained rather than relabeled as reruns.

CSEM-11 now has a queued general declaration-constraint task. Slot 1 has moved
to independent GNU attribute syntax/meaning; slot 2 continues model-owned
macro provenance. Function address representation and whole-pipeline supply
invariants remain explicit obligations, alongside the remaining compiler work.
