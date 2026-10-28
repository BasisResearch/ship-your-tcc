# Compiler-action typed Cabs checkpoint

This is an incomplete F3 checkpoint on 8371087 / adopted main 2c8599a.
The ordinary whole-translation-unit target remains blocked. Its blockers are
retained below, rather than counted as parser agreements or Supported exclusions.
CSem stays C11; Supported/Fits and the sum compile-cut are unchanged.

The string-tagged GNode has been replaced by typed Cabs fragments. Named
production marks retain token ranges, typed children, and explicit construction
errors. Specifiers/declarators are captured before CType merging. Constants,
characters and adjacent strings retain their spelling/components. Initializers
retain brace/designator boundaries and require modeled expression actions for
values/indices. Labels attach to their following statements. Saved initializer
replay records word offsets and original source-token identities; unget replay
uses the concrete macro frame identity. Equal saved token bytes do not select
another source occurrence. Dry runs and preprocessing expressions have named
drop reasons and retain their fragments in the audit.

`tcc_model --ghost-out FILE` now enables ordinary model capture and exports
successful typed fragments alongside explicit gaps, tokens and provenance.
A failed whole unit remains `unimplemented`; exporting individual fragments is
not a successful translation unit. `Ghost.runRaw` executes the normal physical
lexer, directives, macros and compiler parser, without any reference AST or
spec parser in that computation. Directive and `_Pragma` operand lexing is
kept outside the language-token view while normal effects execute once.
Physical offsets, opened files, presumed labels, include/macro frame identities
and pragma-state changes are retained. Missing derivations diagnose; they are
never filled from the spec's provenance.

`Ghost.run` is the separate shared-positioned-token seam. It receives only
tokens/positions/provenance and runs the modeled lexer/parser/code generator on
a virtual token-per-line buffer. Spec Cabs is produced in a separate process
by `parsePositioned`. Focused fixtures explicitly use **no predefined text**;
this is not the actual image's complete ordinary-source configuration. Raw
modes retain the actual implicit header and its failures. No equality comparison
erases Cabs locations, declarators, qualifiers, initializers or arbitrary AST
fields.

Tcc's constant folding does not require folding the ghost: source expressions
are retained before their codegen values disappear. The named `buildChain`
construction restores a collapsed logical chain to source-order, left-associated
Cabs, with every operand and operator region. Its agreement is owed by the
parser-agreement Statements; it is not a proved normalization theorem. There
is no arbitrary AST normalizer or caller-chosen agreement relation.

## Required ordinary-source obstruction

The actual image's predefined text includes `tccdefs.h`. Even `int x;` therefore
starts with `struct __uint128__ { char x[16]; } __attribute((__aligned__(16)));`
and macro-produced builtin prototypes. The current independent spec parser
rejects this stream (`expected a declarator (at punctuator)`); the model's typed
adapter also reports its missing attribute construction. Macro-produced tokens
lack complete model-owned origin derivations. For the scalar-declaration probe,
the first unavailable origins are tokens 25–27 (`void`, `*`,
`__builtin_memcpy`) in an actual macro frame. All source/header tokens and gaps
remain in raw artifacts. The implicit header has not been stripped, replaced,
or normalized out of the ordinary model.

Whole-C remaining constructions also include typeof, generic associations,
choose_expr, static assertions, GNU attributes, K&R definitions and deferred
inline bodies. GNU local labels need Cabs representation. Deferred inline
bodies and consumed local-label declarations now force explicit gaps instead
of silently becoming prototypes or empty blocks. The spec rejects this GNU
omitted-middle conditional fixture; that is recorded separately from the
model's constructed Cabs. These are obligations, not a refusal disjunct.

## Open obligations and ledger suggestions

`Ghost.Erasure_Statement` and `RawErasure_Statement` require equality of the
whole parser/codegen state after erasing all named ghost fields, with identical
errors. `ParserAgreement_Statement` and `RawParserAgreement_Statement` require
exact Cabs equality on their positioned streams. `RawConstruction_Statement`
requires a named successful state/tree witness for every Supported/Fits source,
without an unimplemented/refusal escape. Finite comparisons prove none of
these Statements. Phase-four/provenance correctness, complete macro/include
and transformed-source mapping, fuel adequacy and F6 binary refinement remain
separate obligations. The gate audits the new definitions and Statements;
only propext, Classical.choice and Quot.sound occur.

Lead owns STATUS/PHASES; neither is edited. Suggested F3 ledger wording:
"typed production fragments and independent fixture comparison implemented;
ordinary full Cabs blocked on injected-header syntax and macro provenance;
state erasure, total construction, parser agreement and whole-C coverage open."
Lua's abstraction inventory and mandatory table were read before this work.
No library proof was copied; no byte-identity proof reuse is claimed.

## Validation and pins

The matching `ghost-gate-2` receipt passes `scripts/check.sh --csem`, with
all 2,076 frozen inputs verified. `ghost-13` explicitly rebuilt tcc_ghost,
tcc_model, tcc_pre_tokens, pre_spec and GhostAudit. The only subsequent build
source change tightened the strict comparator's minimum; gate 2 covers it.

| Check | Checkpoint result |
|---|---|
| Strict shared-token Cabs | 44 exact / 56; 12 explicit nonsuccesses; exit 0; all 44 required positives present |
| Strict ordinary raw-source controls | 0 exact / 3; exit 1, an acceptance blocker |
| Explicit raw diagnostic mode | nonsuccess retained; exit 0 only with --diagnostic |
| Strict minimum zero | rejected by argument parser, exit 2 |
| Independent fast compiler/model arenas | 7/7 equal; zero refusal/mismatch/model/harness errors |
| Ordinary ghost-on/off arenas and diagnostics | 7/7 equal; typed fragments exported; whole units remain unimplemented |
| Full repository/CSem gate | pass, ghost-gate-2; source hashes verified |

These arena checks use the image's independent fast RV64 compiler runner;
no new native Sail capture is claimed here. They do not prove state erasure.
The comparator checks relevant source hashes before and after the run, fixture
hashes, executable/driver hashes, and a passing frozen receipt. It compares
complete exported Cabs, including all locations; no field-erasing normalizer
is supplied. All omission fails the default strict gate.

[Evidence](ghost-cabs/pins.json) records executable and artifact hashes.
`fixture-comparison.json`, `raw-comparison.json`, `arena-comparison.json`,
`erasure-comparison.json` and `exit-statuses.json` retain classifications and
commands. `artifacts.tar.gz` holds complete fixtures/outputs/ordinary ghost
artifacts; `build-history.tar.gz` retains failed builds and logs. The gzipped
request/receipt pairs contain exact full source manifests, not just the dirty
parent revision. Read-only oracle reuse is pinned by content key
`db28f95fedcb38cac435c74ab6ff3d264b9d597ef2b6182505feb7a76943e953`;
no shared toolchain or semantic source was modified.

The independent unchanged text-parser diagnostic was run on a token-per-line
serialization of the **model-produced** raw stream. It fails at line 11,
column 1: zero-based model token 10, `__attribute`, before the opening `(`.
`header-obstruction.json` pins that binary/input and retains its exact error;
this locates the spec failure independently of the model's attribute gap.
GNU mode does not make pAttrSeqOpt accept GNU attributes: its existing entry
recognizes C [[...]] attributes. Independent F1 syntax/alignment support and
the model's own macro-origin repair remain distinct work.

Historical ghost-1 through ghost-8 and ghost-10 failures are not build evidence.
The first ghost-9 56/56 nonsuccess result was diagnostic only. ghost-11 had
43 exact, one for-clause location mismatch and 12 explicit nonsuccesses; the
final model includes that clause's semicolon and passes all 44 controls.
The overall task continues after this coherent checkpoint; ordinary complete
Cabs and macro provenance are not declared complete.

## Combined integration, 2026-10-28

The checkpoint is combined with adopted allocation, library identity and corpus
fixtures. The full CSem gate passes with all 2,302 captured source hashes
unchanged. All 44 required fixture Cabs comparisons still agree; 12 diagnostic
fixtures stay outside that count. All three ordinary-source strict controls
still fail on the documented provenance/header gaps. Seven fresh ghost-on/off
arena/console/diagnostic comparisons agree. No state-erasure theorem follows.

The complete 1,815-input fast-reference run retains 1,345 successful equal
arenas, 463 matching refusals and seven explicit assembly gaps, with no other
differences or errors. Its exit remains 1 for assembly. Fresh executions of
this ghost-instrumented model also match all three complete native Sail arenas
retained from the corpus-fixture review. This reuses unchanged native captures;
Sail was not rerun. Source/runtime/model pins, complete comparison artifacts
and both gate snapshots are in ghost-cabs-integration-2026-10-28/.

The first combined build exposed the positional Pos constructor's new identity
field. Named-field construction preserves the checkpoint's incomplete raw
origin information without inventing membership. A first erasure invocation
lacked generated build/image.syms; normal harness image/symbol preparation
followed by the same check passes. Both failed attempts remain diagnostics.
The ongoing Sol continuation owns macro origins/replay and typed header syntax;
independent F1 GNU attribute meanings remain queued. This adopts a useful
instrumentation checkpoint, not ordinary whole-C parser completion.

The final rebase adds the independently validated comparison-isolation change.
All 2,300 other gate inputs, including every semantic/compiler source, are
byte-identical; only check.sh/model_diff.py differ. The prior two scripts are
retained exactly beside their hashes so the gated source snapshot remains
reconstructible. The rebased static gate, including concurrency controls,
passes. Historical fixture/raw receipt pins refer to their original run.
