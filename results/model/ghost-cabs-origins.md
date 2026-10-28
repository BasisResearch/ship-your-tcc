# Model-owned origins and typed header continuation

Continuation after 31d18ba; historical evidence under ghost-cabs/ is unchanged.
Final validation and exact pins are recorded below after the frozen build.
Whole-C construction and the proof Statements remain open.

The origin layer uses shadow word-offset maps on TokenString/TokPtr and
parallel maps for macro definitions, arguments and cached expansions. No
metadata word is inserted in compiler token arrays. Macro actions construct
replacement, indexed argument, stringification, paste and builtin derivations.
Replay uses the actual saved slot/frame identity, never equal token bytes.
The model captures physical cursors before lexing, so refill cannot substitute
a different buffer's cursor. Folded string initializer replay retains all
original components and its exact lookahead identity. Nested ghost annotation
erasers are factored by compiler shape and clear every new field too.

Successful modeled Host.openRd events retain the requested name, resolved
image identity and bytes before descriptors can be reused. Compiler-opening
IDs bind to those descriptor records; even empty and macro-only files remain
in the opening graph. #line labels never supply physical identity. Include
processing captures the directive introducer's physical span before its buffer
is consumed, and gives each included opening its actual innermost-first span
chain. Repeated openings remain distinct. An unmapped directive marks the
include metadata incomplete; byte equality alone cannot certify ancestry.
Generated model buffers retain their exact bytes/opening IDs and are explicitly
marked generated, with empty image identity. In particular, the compiler's
command-line header include has a real generated-buffer span but makes no
runtime-image-membership claim. Internal synthetic lexical origins remain
explicitly unavailable; incomplete physical mappings propagate diagnostics.
Trigraph/splice transformations still need the actual refiller's shadow map.

Model parser actions now retain typed GNU attribute, asm-binding, declaration
name and typedef-name fragments. The latter decisions come from the modeled
parser before CType/name folding, including the actual __builtin_va_list
binder. Full GNU attribute tokens and argument pieces survive merged flags.
The independent F1 contract is constructed from model actions/tokens:
attr_ns retains the exact located introducer, attr_id the exact alias;
arguments retain located raw token pieces and a declared space-separated
rendering of those same pieces. For asm bindings, attr_ns is the located exact
asm keyword and attr_id is the located synthetic asm_label. The typed string
fragment also remains in the audit. Nothing strips injected declarations or
changes emitted bytes. These are named carrier encodings, not semantic support
for arbitrary user asm labels (outside DESIGN §5). The Supported predicate is
about user source; compiler-generated header declarations remain real data.

The only normalizations are named fixed constructions: logical-chain source
association, GNU-attribute carrier and asm-binding carrier. Their exact
NormalizationAgreement_Statement remains unproved, as do Erasure_Statement,
RawErasure_Statement, ParserAgreement_Statement, RawParserAgreement_Statement
and RawConstruction_Statement. There is no arbitrary AST normalizer, caller
relation, reference AST input, digest input or spec-parser call in model
computation. Pos constructors use named fields for current-main compatibility;
no arbitrary sourceIdentity/runtime-membership premise is introduced. The
pending independent F1 PTok.spelling field will need the already-retained
model token spelling when integrated; this branch does not alter spec grammar.

The source-origin comparator independently processes the same source/files
without predefined text. It locates the user-source suffix by genuine model
opening IDs, then compares every token, full recursive Origin and actual user
opening graph (including contents, physical identities and include sites).
The actual injected header remains present but explicitly uncompared by this
component check. Exact component origins do not establish whole-TU Cabs
agreement or state erasure. Missing/unimplemented/rejected cases remain outside
Cabs success counts and do not restrict whole-C Supported/Fits obligations.

Ledger suggestions for the lead (STATUS/PHASES are untouched): retain whole-C
construction, raw parser agreement, state erasure, normalization meaning,
source transformation mapping, replay coverage/fuel and F6 binary refinement
as obligations. GNU typeof, generic associations, choose_expr, static asserts,
K&R definitions, deferred inline bodies and local labels remain explicit
construction/representation gaps. Independent F1 header grammar and semantic
alignment/linkage connections remain separate, including the newly reproduced
typedef alignment compiler discrepancy; ghost carriers do not fix that bug.

## Frozen validation

The explicit `ghost-origins-12` receipt rebuilds tcc_ghost, tcc_model,
tcc_pre_tokens, pre_spec and GhostAudit. `ghost-origins-gate-1` passes the full
`scripts/check.sh --csem` gate. Both full source manifests (2,077 inputs)
verify against the frozen tree. The full gate preserves the compared ghost
executable. Fresh independent arena and ghost-on/off regressions were repeated
**after** that gate and pass. No new native Sail execution is claimed.

| Check | Result |
|---|---|
| Shared-token Cabs | 44 exact / 56; all 44 required controls; 12 explicit nonsuccesses |
| Ordinary model-only Cabs | 7/7 constructed; zero token diagnostics; pragma fixture retains four effects |
| Strict ordinary model/spec Cabs | 0/3 exact; model ok, spec rejects; expected nonzero exit 1 retained |
| User-source token/origin/opening graph | 16 required exact controls; three separate explicit gaps |
| Independent compiler/model arena regression after gate | 7/7 equal; zero errors, refusals or assembly gaps |
| Ordinary ghost-on/off regression after gate | 7/7 equal arenas, console, stdout and stderr |
| Full repository/CSem gate | pass; exact frozen source hashes verified |

The origin positives include object/function macros, repeated arguments,
stringification, pasting, builtins/#line, raw UCN/UTF-8 spelling, adjacent
strings, the 8 KiB refill boundary, incomplete initializer replay, GNU varargs,
nested headers, repeated declaration-only headers, #line inside a header and
a noncanonical include path. Full recursive Origin data and source graphs are
compared, not only diagnostic anchors. The three failures stay visible:
splicing and trigraphs have incomplete physical maps; a header that repeats an
identical macro definition retains the second definition's origin in the model
but the first in the independent spec. The two actual include-directive spans
are distinct and preserved. No origin is replaced to obtain equality. The
initial failing discovery is separately archived. Required positives still
fail strict acceptance on omission; a run containing only diagnostic cases
cannot pass.

Raw Cabs strict controls retain the actual compiler-injected header. Model
construction now succeeds, including its 22 explicit carrier records (one GNU
attribute and 21 asm bindings); the local independent spec rejects the header.
Its separately developed F1 grammar is not adopted or silently used here. Exact
ordinary parser agreement, generated-buffer/source-identity conventions and
header meaning therefore remain open. The user-origin component check excludes
that header **only in its stated comparison scope**, while model computation
and byte comparisons retain it.

[Evidence and pins](ghost-cabs-origins/pins.json) retain complete outputs,
fixtures, commands, current and historical failed receipts, executable hashes
and exact source manifests. Run
`python3 results/model/ghost-cabs-origins/verify.py --sources` against this exact
snapshot to check artifact integrity and frozen inputs; omit --sources to check
the portable artifact package after integration. The reproducible comparators
are scripts/ghost_cabs_cmp.py and scripts/ghost_origins_cmp.py. Original
checkpoint evidence is unchanged. STATUS/PHASES remain untouched; lead review
must retain the open obligations above.

Final lead review independently assigns the identical-redefinition provenance
correction to Spec.Pre.Phase4.doDefine on main. Its sameDef currently keeps the
old definition; the modeled compiler installs the new one. This branch keeps
its actual origin and the 16-exact/three-gap checkpoint unchanged until combined
validation. The full final lead note is pinned with the evidence. The
independent typedef/compiler alias follow-ups also remain outside this patch.

## Comparison-scope follow-up

Final review tightened the origin harness after checkpoint 291bbb2: its model
source selection now comes entirely from actual model opening records. It
excludes only the identified initial tccdefs opening and rejects any other
prefix anchor. All remaining physical openings, including non-emitting and
unmatched ones, must agree with the independent source graph. Spec source names
never select the model records. Three perturbations of actual captures verify
that extra non-emitting openings, extra model-only prefix tokens and missing
opening graphs fail comparison; these are harness checks, not success counts.
The source-fixture result remains 16 required exact and three explicit gaps.

The follow-up evidence and current-source verifier are in
[ghost-cabs-origin-scope](ghost-cabs-origin-scope/pins.json). The older origin
package and verifier refer to the exact 291bbb2 snapshot; do not use its
--sources option to claim that a later source tree is unchanged. Both packages
retain their exact manifests and failures. No model computation or spec grammar
changes in this follow-up.

The tightened harness is rebuilt under `ghost-origins-13` and validated by
full `ghost-origins-gate-2` (2,077 frozen inputs verified). All three omission
checks pass, the 44 required Cabs controls remain exact, and strict ordinary
model/spec acceptance still returns 1 for its three spec rejections. Both
seven-case arena/on-off regressions pass again after gate 2. The gate preserves
the compared ghost executable. Verify this final snapshot with
`python3 results/model/ghost-cabs-origin-scope/verify.py --sources`.
