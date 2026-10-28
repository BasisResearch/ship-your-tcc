# Lean C grammar and remaining work

The endpoint is all of C11 and the GNU forms approved in DESIGN-COMPILER.md
§5. This document records the current implementation; it does not narrow
`Supported` or authorize compiler refusal.

`cparser/CSemCParser` is handwritten source, ported from the pinned
`parsers/c/c_parser.mly`, `c_lexer.mll`, and `lexer_feedback.ml`. `Token` derives
exact equality; every `accept`, `expectT`, `at?`, and direct `isTok` call uses
it. No keyword whitelist controls token equality. The semantic front end is
total and fuel bounded; `Main.firstDiff` is a diagnostic outside that entry
point. The files under `lean_frontend/generated` remain generator outputs.

The default text parser follows the pinned Menhir lexer/grammar. `--gnu`
additionally recognizes GNU declaration attributes and keyword aliases. The
raw CLI's `--tcc` selects these approved extension spellings too. This does
not enable C23 bracket attributes in raw C11: raw, strict GCC and patched tcc
all reject the retained `[[gnu::unused]]` dialect control.

GNU attribute introducer aliases, names (including keyword aliases such as
`__const__`), argument token spellings and all locations are retained in
Cabs. Attributes attach at declaration/specifier, typedef, aggregate/member,
pointer-qualifier, name and concrete declarator suffix positions. Abstract
or type-name positions without a representation produce diagnostics, rather
than discarding semantic attributes. `__builtin_va_list` in the injected
header is its real `typedef char *` declaration, using ordinary name feedback.
Declaration `__extension__` is a retained diagnostic-mode marker.

Shared Lem elaboration validates effects. Aggregates carry a named
`Ctype.aggregate_layout` independently of declared member alignment. Concrete
layout computes effective packing without changing physical member lists;
typedef alignment has an explicit Ctype annotation consumed by both layout
readers, and does not change `sizeof` of the underlying type. Function
`noreturn` feeds the existing Ail flag. No-op compiler hints remain attributes.
The current aligned-argument adapter accepts checked positive decimal powers
of two (and the target's default maximum); other integer constant-expression
forms remain a diagnostic and an open obligation, not a Supported exclusion.

GNU asm-label syntax is retained as a located attribute with its original
introducer and literal tokens. Elaboration reports its separate DESIGN §5
linkage obligation. This does not give user assembly a C meaning or adopt
compiler-injected aliases as library proxies. A typed injected-binding/origin
connection remains required before adopting the raw frontend into the
headline theorem; the legacy Supported visitor is unchanged.

Validation uses `scripts/csem_parse_cmp.py`. Both parsers receive the same
preprocessing environment. JSON comparison retains every Cabs field and
location; only the oracle document's source digest (metadata absent from the
Cabs datatype) is removed. Import/export round-trip equality is checked
first. Mismatches, including LEAN_ONLY, and unexpected process failures fail
the command. A diagnosed oracle refusal is classified separately from a
crash, timeout, malformed JSON, or inconsistent exit/status.

The oracle itself does not accept every approved form. Its refusals are
listed in `results/csem/f1a-oracle.tsv`, with full diagnostics retained in the
run artifacts named in `results/csem/f1a-parser.md`. They are gaps to fix and
validate, not evidence that those constructs are outside Supported.

Remaining GNU work includes unrepresented attribute/type positions and
nonliteral alignment arguments, cleanup calls on every scope exit, expression
`__extension__`, GNU expression-alignof, local
`__label__` scoping, intrinsic varargs operations, remaining aliases, and empty
initializers. K&R definitions with declaration lists also remain rejected by
the pinned Menhir action and `Parser.createFunctionDefinition`. The current
Cabs datatype has no local-label-declaration constructor; a correct grammar
extension must preserve label scope through an explicit representation or
validated desugaring. Cleanup attribute tokens alone do not supply cleanup
semantics: the Lem source has no cleanup lowering yet. Change that source
and regenerate when extending the datatype or elaboration.

Do not rewrite input, remove attributes/locations, normalize Cabs differences,
or map extensions to arbitrary identifiers just to improve agreement counts.
Menhir must be extended from its source where it lacks an approved construct;
tcc behavior is tested separately and does not redefine C11.
