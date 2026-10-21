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

The default configuration follows the pinned Menhir lexer and grammar,
including its limitations. The `--gnu` switch currently enables keyword
aliases in `Lexer.gnuKeyword`; it does **not** establish a completed GNU
grammar. In particular ATTRIBUTE, EXTENSION, LABEL, and BUILTIN_VA_LIST are
recognized tokens without grammar productions. Do not use that switch as
validation of these forms, or silently consume them.

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

Remaining GNU work includes preserving GNU attributes at each declaration
position (including cleanup calls on every scope exit), `__extension__`, local
`__label__` scoping, builtin varargs types, GNU keyword aliases, and empty
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
