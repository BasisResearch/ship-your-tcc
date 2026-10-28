import CSemCParser.Grammar

/-! # The front end: preprocessed bytes → Cabs

`frontEnd cfg fname pp` is the Lean replacement of Cerberus's
`C_parser_driver.parse_from_string ~filename pp` (minus CN): lex (`Lexer.lean`),
map lexing positions to Cerberus positions through the line map
(`C_lexer.LineMap.position`, `Cerb_position.LineMap.lookup`), parse
(`Grammar.lean`). `fname` is the name the driver hands the lexer (the C file's
path as given on the command line); it is every position's `lexingFile`, and
the source file of lines before the first line marker.

All functions here are total. -/

namespace CSemCParser

open CerbLocation (Pos)

/-- `Cerb_position.LineMap.lookup` with `j` = the number of marks whose key is
`< lnum` (the marks are in increasing key order), then `LineMap.position`. -/
def mkPos (fname : String) (marks : Array LineMark) (j : Nat) (p : LPos) : Pos :=
  let (f, n) := if j = 0 then (fname, p.lnum)
    else let m := marks.getD (j - 1) default; (m.file, m.line + (p.lnum - m.key - 1))
  { file := f, line := n, col := 1 + p.cnum - p.bol, lexingFile := fname, lexingLine := p.lnum }

/-- Advance the mark pointer `j` past every mark with key `< lnum`. -/
def advanceMarks (marks : Array LineMark) (lnum : Nat) : Nat → Nat → Nat
  | 0, j => j
  | f+1, j => if j < marks.size && (marks.getD j default).key < lnum then advanceMarks marks lnum f (j+1) else j

/-- Every token with its positions mapped. -/
def mapPositions (fname : String) (toks : Array RawTok) (marks : Array LineMark) : Array PTok :=
  (toks.foldl (fun (acc : Array PTok × Nat) t =>
      let j := advanceMarks marks t.sp.lnum (marks.size + 1) acc.2
      let je := advanceMarks marks t.ep.lnum (marks.size + 1) j
      (acc.1.push ⟨t.tok, mkPos fname marks j t.sp, mkPos fname marks je t.ep, t.spelling⟩, j))
    (#[], 0)).1

/-- Parse a token array. The fuel is 64 per token: each recursion level of the
grammar either shifts a token or is one of a bounded number (< 16) of levels
above one that does. -/
def parseTokens (cfg : ParseCfg) (fname : String) (toks : Array RawTok) (marks : Array LineMark) :
    Except PErr translation_unit :=
  let ptoks := mapPositions fname toks marks
  let start := mkPos fname marks 0 ⟨1, 0, 0⟩
  let st : PState := { toks := ptoks, idx := 0, prevEnd := start, ctx := initialCtx, cfg }
  match (pTranslationUnit (64 * (ptoks.size + 2))).run st with
  | .ok tu _ => .ok tu
  | .error e _ => .error e

def showPos (p : Pos) : String := s!"{p.file}:{p.line}:{p.col}"

/-- The front end on preprocessed text: Cabs, or the error (lexer or parser)
with its source position. -/
def frontEnd (cfg : ParseCfg) (fname : String) (pp : ByteArray) : Except String translation_unit :=
  match lex { gnu := cfg.gnu } pp with
  | .error e => .error s!"{fname}: lexing line {e.pos.lnum}, column {1 + e.pos.cnum - e.pos.bol}: {e.msg}"
  | .ok (toks, marks) =>
    match parseTokens cfg fname toks marks with
    | .ok tu => .ok tu
    | .error e => .error s!"{showPos e.pos}: error: {e.msg}"

end CSemCParser
