/-
What `tccpp_correct` (DESIGN-COMPILER.md §1.1) compares, so that the model
of tcc's preprocessor (F3, `Tcc/Model/`) and this specification meet on
like terms.

**The stream.** tcc's parser takes its tokens from tccpp.c `next()`. With
`PARSE_FLAG_TOK_NUM` and `PARSE_FLAG_TOK_STR` clear, `next()` delivers each
token before phase-7 conversion: `TOK_PPNUM` (a pp-number, with its
spelling), `TOK_PPSTR` (a character constant or string literal, with its
spelling, prefix and quotes included), identifiers and keywords (a
`TokenSym`, spelled by `get_tok_str`) and punctuators (a token code, spelled
by `get_tok_str`). Setting the two flags only adds the phase-7 conversion
(`parse_number`, `parse_string`), which is the business of the phase-7
pipeline, not of phases 1–4. The model's `preprocess` is that unconverted
stream, each token projected to a `PPToken`:
* `TOK_PPNUM` ↦ `ppNumber`; `TOK_PPSTR` ↦ `charConst` if its spelling
  starts with `'` or `L'`, else `strLit`; a `TokenSym` ↦ `ident` (keywords
  are identifiers in phases 1–4); a punctuator ↦ `punct`; the spelling is
  `get_tok_str`'s;
* a pragma that tcc executes after phase 4 (`#pragma pack`,
  `#pragma comment`) or ignores ↦ `pragmaBegin`, its tokens, `pragmaEnd`,
  at the point of the stream where tcc's preprocessor meets it (`pack`
  changes `s1->pack_stack`, which tccgen reads at the next struct
  declaration, so the position is meaningful);
* `#pragma once`, `push_macro`, `pop_macro` are phase 4's own and leave no
  token (as in the specification, `Ext.pragmaOnce`/`pushPopMacro`).
White space and line information are not part of either side.

**The relation.** Where the specification gives a token sequence, the model
must give the same one. Where the specification stops with an error (a
violated constraint, undefined behaviour it rejects, `#error`, a missing
header, fuel), there is no obligation: such a source has no CSem meaning,
so `tcc_run_correct` says nothing about it either.
-/
import Spec.Pre.Main
import Spec.Pre.Tcc.Cfg
import Spec.Pre.Tcc.ImageFS

namespace CSem.Pre

/-- The model agrees with the specification on one source. -/
def PreAgree {ε : Type} (spec : Except PreError (List PPToken)) (model : Except ε (List PPToken)) : Prop :=
  ∀ ts, spec = .ok ts → model = .ok ts

/-- `CSem.Pre.phases14` for tcc's image: the configuration and file system
of `tcc -run /prog.c` (`Spec/Pre/Tcc/`). -/
def phases14Tcc (src : Bytes) : Except PreError (List PPToken) :=
  phases14 Tcc.cfgTcc Tcc.imageFS src

/-- `tccpp_correct`, for a model `preprocess` of tccpp (F3) and the
resource premise `fits` (the static half of `Fits`: include and macro
nesting within tcc's limits, the spec's fuel not exhausted). To be proved
in F2's second half; listed in PHASES.md. -/
def TccppCorrect_Statement {ε : Type} (preprocess : Bytes → Except ε (List PPToken))
    (fits : Bytes → Prop) : Prop :=
  ∀ src, fits src → PreAgree (phases14Tcc src) (preprocess src)

end CSem.Pre
