# Resolved runtime-library identity

The raw semantic path now consumes explicit resolved source identities through
Cabs, Ail/Core locations and Core runtime loading. Filename suffixes and
`#line` labels no longer decide library membership on this path. The full
`scripts/check.sh --csem` gate passes in frozen receipt
`library-gate-c221002d3db6a280cd0e`. No Supported/Fits restriction, source repin,
raised budget or sum-cut regeneration was used.

`CerbLocation.LibraryConfig` contains admitted resolved image identities.
`SourceIdentity` carries the resolved components and the membership computed
from that configuration. Positions retain every C-visible filename/line/column
and lexing coordinate separately. `Raw.semanticConfig` installs the same policy
for raw Cabs and the explicitly identified Core runtime sources; the test
runner deliberately gives those Core files neutral, non-library-looking
labels. The adapter's configuration-to-image/runtime-byte connection remains
a proof obligation, not something filename spelling supplies.

Macro replacement identity follows its definition, substituted argument identity
follows the operand, and constructed paste/stringify/builtin identity follows
the invocation. `Origin.site` remains the diagnostic anchor;
`Origin.identitySite` is separate. Complete origins, physical spans, include
ancestry and opened-source manifests remain in the result. The library audit
retains both answers even when presumed labels and actual identities disagree.
The checked entry accepts these disagreements; pack-layout consumption remains
a named diagnostic and an open whole-C duty.

The main-file overlay is a separate source-opening repair: self-includes read
the actual input bytes. Stale entries with that resolved identity are removed
before inserting the input, because the image initializer lets later duplicate
entries replace earlier contents. This is distinct from the opened-path
pragma-once cache and from presumed labels.

The historical text/JSON interface remains explicit. Its metadata-free positions
use `historicalLibraryLocation` and the inherited suffix classifier/import guard.
Raw JSON carries an additional `source_identity` field; the historical importer
diagnoses that enriched format instead of silently downgrading it. Independent
Cabs comparisons project only that extra field. They retain every source and
lexing coordinate, all regions/cursors, literal spelling and the raw-byte MD5.
The complete enriched outputs are retained alongside the historical projection.

The five semantic calls to `isLibraryLocation` are:

| Shared model | Behavior |
|---|---|
| core_eval.lem:602 | Attribute library-origin PEundef to the enclosing C location |
| core_run.lem:478 | Attribute library-origin kill requests to current C location |
| core_run.lem:783 | Keep current C location across library expression annotations |
| core_reduction.lem:1199 | Keep current C location across library expression annotations |
| core_reduction.lem:1209 | Attribute library action requests to current C location |

The four direct generated-consumer probes exercise those five sites, including
the alternate Core_run engine. Parser `rg`/`rgc` copy complete token positions;
Cabs-to-Ail retains locations; translation passes them to `add_loc`, actions
and lifetime operations. Cursor/region/bounding-box helpers select existing
positions without rebuilding filename-only positions. Unknown/internal generated
locations contain no label that can become a suffix plant. Core runtime stamping
attaches explicit identity alongside preserved labels and action/UB coordinates.
The historical driver import guard and runtime-resolution comparison remain
other callers of the suffix classifier; neither establishes raw membership.

Translation also has two name-based proxy lookups (translation.lem:243, :4364).
The identity-aware elaborator removes actual TU function definitions from the
proxy-name table. User internal-linkage implementations retain their bodies,
including definitions/redeclarations before or after calls and function pointers.
Unresolved external library declarations retain proxies, including compatible
C11 7.1.4 declarations without a header. The historical driver remains unchanged.

Validation, with exact commands, streams and pins under [library-identity/](library-identity/):

| Check | Result |
|---|---|
| Focused complete C pipeline | 14 defined positive programs and 8 expected-UB attribution controls pass; 2 local-fork gaps retained separately |
| Generated semantic consumers | 5,580 exact location checks pass, covering the five semantic call sites |
| Full positioned Cabs / MD5 | 24/24 agree with pinned Menhir and the unchanged text parser on the explicit historical projection |
| Existing raw frontend tests | 11/11 pass after the final gate; original positions/provenance checks retained |
| Core quotation | Full gate controls pass; datatype visitor reproduces exactly (644 model constructors, 82 heads); serializer retains optional identities |
| Historical fork lane | Unchanged 90 MATCH / 18 UB_MATCH / 5 CERB_SKIP across 113 files |
| Regeneration / axioms / purity | Pinned 235-file regeneration check passes; standard three axioms only; no native floating operations in either cone |
| Full maintained gate | Passes, including the focused matrix with fresh build/ output |

The focused inputs include genuine runtime stdio/stdbool headers with nested
headers, compatible manual printf declarations, user files with library-looking
paths, a #line plant, relabeled runtime code, include aliases, mixed replacement/
argument/paste/stringify provenance, source __FILE__/__LINE__, internal-linkage
shadowing and a stale-main self-include. Genuine runtime header bytes are pinned;
the small fault/macro header is an explicitly configured synthetic runtime fixture.
GCC separately accepts all eight initial defined controls and the four pointer
baseline controls with `-std=c11 -pedantic-errors -fno-builtin`.

The historical semantic observations remain limitations, not agreements:
plant/user-looking inputs are refused by its path guard; relabeled library
code attributes UB to the relabeled header rather than its caller; static
proxy-name definitions reach printf_proxy/close_proxy errors. Basic and manual
printf controls execute normally. The enriched JSON rejection is retained too.
No historical refusal counts as successful semantic comparison.

The two residuals are valid static-function prototype/definition/redeclaration
programs followed by function-pointer initialization. The candidate reports
exactly `ill-formed program: null function pointer` (the retained streams include
its punctuation). The same neutral source fails on the independently pinned
adopted anonymous-member integration Lean binary: its hash matches that
integration's existing focused/continuation records. Pristine pinned OCaml and
strict GCC return zero. Direct redeclared calls and pointers without the extra
redeclarations pass. This is a pre-existing local Lean-fork defect, not an
upstream OCaml defect or a successful result. CSEM-10 in CSEM-UPSTREAM.md and
[pointer-adopted/neutral.c](library-identity/pointer-adopted/neutral.c) retain the
minimal source and exact independent observations.

[pins.json](library-identity/pins.json) retains source hashes, executable hashes,
Lean 4.34.0 and Cerberus 704c4e69 / Lem e6d8db56 pins. The successful build/gate
receipts are distinct from failed attempts. The final freezer checks all 2,737
frozen source hashes, compared binary hashes and the evidence manifest without
printing manifests. Shared toolchain/oracle caches were read-only. Lua's
inventory/mandatory table was consulted; no machine library summary was reused
or claimed instruction-identical. The raw frontend census remains PARTIAL 0 /
UNSAFE 0 with its one recorded Lean-core string opaque; the whole semantic
cone retains its separately recorded partial/parser seams.

Remaining obligations are the fixed TokenIdentityConnection proof, grammar/
Ail/Core transport proofs, image/configuration membership and runtime-byte
connections, preprocessing/parser correctness, pack-layout consumption,
whole-C coverage and compiler/library refinement. No ConnectionObligations
instance, NoUB witness or universal correctness theorem follows from these
finite comparisons. The old suffix-audit Statement is historical only and is
no longer a semantic connection premise. Allocation was adopted independently
on main; lead integration must retain verdictsWithAllocation, allocation-resolved
outcomes/NoUB and these Config/stdlib/elaborate additions together.

Suggested ledger text: F1 raw semantics consume resolved source identity through
positioned Cabs and explicit Core runtime loading, preserving C observables and
full provenance. Focused semantics/consumer checks, historical-position Cabs
comparisons and the full gate pass. Token/grammar/elaboration/image proofs,
pack consumption and whole-compiler duties remain open. CSEM-10 is a separate
local Lean-fork redeclaration/pointer defect. STATUS/PHASES were not edited.

Dead ends retained for the lead:

- Extending Pos exposed the exhaustive quotation match. Fix its handwritten
  serializer and regenerate the visitor; do not patch generated copies.
- Treat UInt8 as a visitor scalar. Recursing through its dependent representation
  creates unbounded instantiated types and aborts. The full failed log remains
  in build/ with a retained hash and concise excerpt.
- Misaligned record fields, a polymorphic array index and diagnostic-runner
  pattern/printer errors caused failed receipts. Explicit matches/types and the
  existing generated UB/driver renderers fix them without larger budgets.
- The first stdio fixtures omitted the include search directory and nested
  posix headers. Those refusals were harness defects; preserve their failed
  observations separately.
- A same-candidate neutral/shadow pair proves name-independence only. The
  independent adopted-binary and pristine-OCaml observations are required to
  attribute the pointer residual; do not resend it as a suffix repair.
- Fourteen attempted preprocessing integration methods skipped for absent
  independent observer binaries and are not validation. The actual final
  --csem gate and the eleven raw methods above are the claimed checks.

## Adopted combined integration, 2026-10-28

The library identity changes are combined with the adopted allocation policy,
including verdictsWithAllocation, allocation-resolved outcomes/NoUB and the
new identity-aware Config/stdlib/elaborate path. The full CSem gate passes on
this combination, with all 2,291 pinned tracked inputs unchanged. Eleven raw
frontend methods pass after the gate. The maintained matrix again yields
22 passes, two separately retained pointer gaps and 5,580 consumer checks;
the 20 maintained allocation controls and six default/historical comparisons
also pass. The earlier independent native placement replay remains in its
allocation report; this integration does not rerun that capture.
Both semantic cones retain zero native floating operations; regenerated files
and theorem axioms pass their gates. Exact combined binary/source pins and
fresh outputs are in library-identity-integration-2026-10-28/; the original
worker observations above remain historical and unchanged.

CSEM-10 now has a separate worker task, tasks/f1-function-redeclaration.md.
GNU declaration attributes needed by the actual injected header are queued in
tasks/f1-gnu-attributes.md, alongside the existing bitfield task. Parser,
semantic transport, runtime membership and whole-compiler proofs remain open.
