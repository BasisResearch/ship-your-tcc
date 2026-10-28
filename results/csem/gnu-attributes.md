# GNU declaration attributes — 2026-10-28

The specification frontend now parses the actual injected tccdefs and the
probed runtime headers while retaining their GNU attributes and declaration
bindings. Alignment and packing have explicit Ail/Core data and concrete
layout meaning. This is a frontend/layout checkpoint, not an ordinary-raw
NoUB witness or a compiler correctness theorem. Compiler-injected asm/runtime
binding meaning and the other residuals below remain open.

## Representation and meaning

`CSemCParser` accepts both `__attribute` and `__attribute__`. The existing
Cabs attribute carrier retains the original introducer namespace, original
attribute-name alias, full locations and each argument's exact located token
sequence. Keyword aliases such as `__const__` retain their spelling rather
than becoming `const`. Declaration/specifier, typedef, aggregate/member,
pointer-qualifier, identifier and concrete declarator suffix positions are
represented. Positions without a semantic representation produce diagnostics.

The shared Lem implementation validates effects before elaboration:

- `Ctype.aggregate_layout` stores aggregate packing and minimum alignment
  independently of declared member alignment. Layout computes effective
  packing transiently; physical members and anonymous containers are kept.
- Typedef alignment is an explicit, unspellable internal Ctype annotation,
  consumed by the implementation and memory layout readers. It changes
  alignment without changing the underlying `sizeof`; typedef chains retain
  the annotation.
- Object/member attributes become explicit alignment overrides. Function
  `noreturn` uses the existing Ail function flag. Compiler hints remain
  attributes. Invalid alignment values/arity and unknown effects retain
  diagnostics; they are not silently parsed away.
- The actual `__builtin_va_list` declaration remains `typedef char *`, using
  normal typedef feedback. Declaration `__extension__` is a retained
  diagnostic-mode marker, without a change to C type/runtime behavior.

The new layout field is preserved through Ail and typed Core quotation.
The existing fuel-stability proof is adapted using its original potentials
and `lfoldl_congr`, with a small packing/potential lemma. No elaboration or
recursion budget was raised. No machine/library proof was reused; the Lua
inventory and mandatory table were read before the proof adaptation.

GNU asm-label declarations retain an attribute with the original asm
introducer namespace, `asm_label` identifier and exact located literal tokens.
Shared elaboration diagnoses the distinct DESIGN §5 linkage effect. It does
not infer library membership or substitute a library proxy from a label.
`Supported` and `Fits` source definitions are unchanged, but the new parser
acceptance exposes the old asm-label exclusion visitor assumption described
below; it is not a completed preservation argument. CSem's C11 mode,
allocation policy, library identities, IEEE semantics and sum cuts are unchanged.

## Actual injected input

The initial independent raw diagnostic used the unstripped, pristine source
header: SHA-256 `d1016b6170c45f1b8e7f8d83bf1f00f21e288eb2e446d9f141457f6f821acf7b`.
Its first rejection was zero-based semantic token 10, `__attribute`, at
`tccdefs.h:184:40`, in:

```c
struct __uint128__ { char x[16]; } __attribute((__aligned__(16)));
```

The subsequent image audit distinguishes source from image bytes. The image
embeds `c/src/strip_headers.py` output, SHA-256
`3bbcf6cc77af60bf2f56e1bce14a18ff100b75a0207129a31c9dde115862dc8a`.
The header harness verifies exact equality with both that generator's output
and `Spec/Pre/Tcc/ImageFS.lean`. The same token is at image line184 column36;
removing indentation/comments does not change this semantic token sequence.
The initial source-header diagnostic is retained separately, not relabeled
as an exact image-byte observation.

The final actual-image syntax probes use ordinary `csem_raw --tcc` without
removing any injected declaration:

| input | result | declarations |
|---|---|---:|
| ordinary source | parse pass, 494 phase-4 tokens | 43 |
| macro-produced declaration attributes | parse pass | 44 |
| actual newlib stdio | parse pass | 342 |
| actual newlib setjmp | parse pass | 46 |
| relabeled tccdefs | parse pass | 43 |
| relabeled runtime header | parse pass | 46 |

Every probe retains the injected aligned attribute and all 21 asm bindings.
Relabel controls preserve explicit resolved-membership metadata under their
stated library policy; they do not establish a new semantic library link.
The stdio probe independently exposed `__extension__ typedef long long
_off64_t` at `/include/sys/_types.h:96:1`; that declaration now parses with
its retained diagnostic-mode marker.

## Independent comparisons

Pristine pinned Menhir rejects the GNU attribute syntax. Its exact refusal
on the independently serialized header token stream is retained, not counted
as AST equality. Three explicit historical-Menhir attribute counterparts
(aggregate aligned, aggregate packed, typedef aligned) each have exact full
Cabs equality between Menhir and Lean, and each executes to `Specified(0)`
on the new concrete OCaml oracle and on Lean. These bracket-attribute files
are declared semantic counterparts only, not raw C11 acceptance tests.

The raw C11 control `[[gnu::unused]] int main(void){return 0;}` is rejected by
raw CSem, strict GCC and patched tcc. The historical text/JSON parser remains
a separately documented interface.

Final focused checks:

| check | result |
|---|---|
| complete GNU programs against independent host GCC | 23 pass |
| invalid arguments/unsupported effects | 8 retained diagnostics |
| valid forms still requiring meaning/representation | 3 separate residuals |
| full positioned raw/text Cabs transport | 34 exact historical-position projections |
| pristine Menhir full-Cabs parser corpus | 614 exact, 41 joint rejections, 9 preprocessing failures / 664 |
| independent documented-desugaring Cabs comparisons | 3 exact |
| anonymous-member suite | 92/92, including negative controls and Core parsing |
| raw/parser/raw-token protocol tests | 28 methods pass |
| library identity regression | 22 pass; 2 inherited CSEM-10 pointer residuals retained |

The raw/text transport checks compare every language field/location; only the
identity field absent from historical text is projected out. They are an
internal transport check, not a replacement for independent Menhir results.
No model-constructed Cabs is used as a specification result.

The focused programs cover default and explicit alignment, packed/aligned
aggregates, unions, explicit member alignment, nested/anonymous containers,
macro-produced tokens/lists, pointer placement, local declarations, typedef
chains, function declarations, diagnostic-mode declarations and keyword
attribute aliases. These are finite executions, not universal NoUB proofs.

## Actual GCC and tcc/Sail observations

The GCC observations are cross-compiled program executions on Sail. The tcc
observations execute the full compiler image with `tcc -run` on Sail; their
metadata records separate compile/program counts. The retained complete
21-case matrix executes all cases without
stopping at its first failure. xPack GCC/Sail prints mask0 and exits0.
Patched tcc/Sail takes 8,256,889 compile steps and 3,513 program steps,
prints mask28672 and exits1: precisely typedef-up,
typedef-down and typedef-chain. The older complete batch stops at case13;
its nonzero result is retained rather than counted as agreement. These native
observations precede the two additional declaration/alias controls in the
23-case spec suite; they are not pooled as 23 native successes.

The lead's separately retained minimization shows that physical typedef
layout honors the attributes, whereas `_Alignof(type)` loses them, including
through a typedef chain. The compiler repair is a separate active F3 task.
CSem is not weakened or reconfigured to match that failure.

The independent member-expression probe produces:

- xPack GCC/Sail: `16 1 16 1 16 16`
- patched tcc/Sail: `16 1 16 1 16 1`

This distinguishes aggregate/member alignment and also exposes a GNU
expression-alignof meaning difference. It is not classified as a C11
counterexample. The spec parser still supports type operands only, so this
is an unresolved GNU meaning connection, not a claimed successful comparison.

Each native observation pins its source, program ELF, trace, raw output and
runtime context. Compiler, image and semantic pins are retained; no sum cut
was regenerated and no compiler/image source was changed by this task.

## Generator, oracle and build receipts

Lean outputs were regenerated through `scripts/csem_regen.sh` with the
unchanged read-only Cerberus/Lem pins and Lean4.34. The datatype visitor was
regenerated through `IEEEVisitor` and installed by its generator: 645 model
constructors. Quotation preserves the new typed layout fields.

The concrete OCaml oracle's content key is
`3a9bc451ab1fd535523882e559223dab323f5c3a2dfd33cf15fc0fc564b13c16`.
Its five verbatim shared semantic sources are `ctype.lem`, `cabs_to_ail.lem`,
`cabs_to_ail_effect.lem`, `ail/genTyping.lem` and `desugaring_init.lem`.
Four pinned source patches supply the union-table correction, typedef
alignment readers, explicit aggregate readers/interfaces and arity-only
compatibility in other pristine consumers. Every patched path is checked in
the overlay manifest, including handwritten interfaces and parser sources.
The pristine switch/upstream/oracles are not mutated.

This oracle retains upstream's Core/IEEE scope. It is not claimed identical
to the Lean IEEE semantics: unrelated local Core files are not overlaid and
no floating ABI is stubbed. Non-concrete backend arity shims are not semantic
support for symbolic/CHERI layout. Existing Core text printers/parsers are a
historical projection; this task does not claim lossless GNU layout transport
through that debug format. Typed Core/quotation retain the metadata.

All Lean builds use the shared root `build/lake.lock`, the user systemd scope
with `MemoryMax=12G`, and a preceding memory check. Sources remain frozen
through matching receipts; hash manifests are verified by script.

| receipt | result |
|---|---|
| gnu-parser-1 | pass, syntax checkpoint |
| gnu-semantics-1 | failed pattern binding; retained |
| gnu-layout-2/3 | failed proof adaptation/closing step; retained |
| gnu-layout-4 | pass, 829 jobs, 2188 verified inputs |
| gnu-layout-5 | pass, 744 jobs, 2189 verified inputs |
| gnu-layout-6 | pass, 744 jobs, 2190 verified inputs |
| gnu-oracle-2 | failed: wholesale Core overlay required absent Mem.convert_fval |
| gnu-oracle-3 | failed: old handwritten pseudo-tag interface |
| gnu-oracle-4 | pass, 437 verified inputs; isolated concrete engine |
| gnu-full-1 | pass; 2,190 unchanged frozen inputs, verified again by lead |

## Ledger suggestions and remaining obligations

Record this as GNU declaration-attribute/header syntax and concrete layout
progress in F1, without closing whole-C coverage or any compiler theorem.
No STATUS/PHASES edit is made here.

- Nonliteral/suffixed/hexadecimal aligned arguments still need the complete
  integer-constant-expression interface. The retained `1<<4` probe is a valid
  diagnosed residual, not an invalid program or Supported exclusion.
- Empty aggregates, function placement alignment, unrepresented abstract/type
  positions, expression `__extension__`, cleanup and other effects remain
  diagnosed obligations. General typedef-array constraints require further
  coverage. Pragma pack schedules remain the separate F2 layout connection.
- The actual injected asm/runtime aliases require an explicit typed binding
  connection. It must carry resolved physical/header-content identity,
  predefs ancestry, declaration/target names and literal tokens, distinguishing
  compiler-injected bindings from user asm and intrinsic alloca from library
  functions. Presumed labels or paths alone do not supply that connection.
- The unchanged legacy Supported visitor relied on parser refusal for asm
  labels. Its eventual adoption must distinguish those user exclusions from
  the injected bindings; blindly excluding every retained binding would make
  every raw unit unsupported. No ordinary-raw NoUB witness is claimed here.
- Preserve the compiler `_Alignof(typedef)` discrepancy and the separate GNU
  member-expression observation. Neither is evidence of successful target
  agreement or a reason to narrow Supported/Fits.

Dead ends not to resend: first-member aggregate-alignment encoding; confusing
pristine source with stripped image header bytes; Lex.slice length/end-index
confusion; keyword aliases canonicalized by the generic attribute reader;
wholesale local Core overlays into the older floating oracle; and updating
only an implementation while leaving its typed pseudo-tag interface old.
Failed receipts and native disagreements remain in the evidence, with no
uncapped fallback or budget increase.

## Lead recovery and evidence package

The external Sol worker stopped with a model-capacity error after the full
gate completed. The lead verified all 2,190 final gate inputs unchanged and
packaged the retained observations. This checkpoint is ready for combined
integration review, not yet adopted by that fact alone. No executable source
was edited during this packaging. Earlier request manifests remain historical:
`gnu-layout-6` predates a change to scripts/check_gnu_attributes.py; only the
final gate manifest is a current-source claim.

[Artifact pins](gnu-attributes/pins.json) include the final gate, focused and
header reports, complete native program ELFs/traces and compressed captures.
Every archive member was rehashed against its loose input. Earlier failed
build/oracle logs and requests are retained. The old pointer failures in this
worker base are historical; main has already adopted the separate CSEM-10
repair and it must survive integration. The required next binding task also
updates the Supported visitor's stale reliance on parser rejection, admitting
genuine injected bindings by resolved identity while excluding user asm labels.

Evidence-label correction: the lead's initial recovery prose incorrectly
called the tcc observations cross-compiled program runs. The preserved
commands invoke scripts/run_tcc_sail.sh and the metadata records compiler
execution. The 21-case matrix has 8,256,889 compile / 3,513 program steps;
the member-expression probe has 13,420,116 / 4,561; the earlier stopping batch
has 4,394,200 / 459. The GCC counterparts are separate cross-program runs.
This corrects the label only; no capture, outcome, pin or success count changes.
