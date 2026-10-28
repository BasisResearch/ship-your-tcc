import CSemCParser.Front
import Spec.Pre.Main
import CSemCParser.Identifier

/-! Direct phase-four token connection. No text printing, re-preprocessing,
JSON import, native numeric conversion, or reference AST is on this path.
Cabs stores literal escape fragments and numeric spellings: execution-character
conversion and numeric value interpretation belong to its existing elaborator.
Adjacent strings retain component regions; `pStringLiteral` merges prefixes
and rejects inconsistent combinations (the same code as the text frontend).

`PositionedTokens` is also an independent testing seam: the parser consumes
full positions supplied by a caller without rewriting them. -/
namespace CSemCParser

structure PositionedTokens where
  tokens : Array PTok
  start : CerbLocation.Pos

def parsePositioned (cfg : ParseCfg) (input : PositionedTokens) : Except PErr translation_unit :=
  let st : PState := {
    toks := input.tokens, idx := 0, prevEnd := input.start,
    ctx := initialCtx, cfg }
  match (pTranslationUnit (64 * (input.tokens.size + 2))).run st with
  | .ok tu _ => .ok tu
  | .error e _ => .error e

namespace Phase7

open CSem.Pre

def bytes (s : Bytes) : ByteArray := ⟨s.toArray⟩

/-- Convert one *whole* preprocessing token. The old text lexer deliberately
can split 08/1abc into tokens; phase 7 must diagnose them instead. -/
def token (cfg : ParseCfg) (p : PPToken) : Except String Tok := do
  let b := bytes p.spell
  let whole (r : Option (Tok × Nat)) : Except String Tok :=
    match r with
    | some (t, n) => if n = b.size then .ok t else .error "invalid phase-7 number"
    | none => .error "invalid phase-7 number"
  match p.kind with
  | .ppNumber =>
    let t ← whole (Lex.numberToken b 0)
    if t == .CN_CONSTANT then throw "non-C numeric suffix" else pure t
  | .ident =>
    let canonical ← canonicalIdentifier cfg.gnu p.spell
    let id := Lex.slice (bytes canonical) 0 canonical.length
    pure ((Lex.keyword cfg.gnu id).getD (.NAME id))
  | .strLit | .charConst | .punct =>
    if !validUniversalEscapes p.spell then throw "invalid literal universal character name"
    let st : Lex.St := ⟨0, 1, 0, #[], #[]⟩
    match Lex.step { gnu := cfg.gnu } b st with
    | .error e => throw e.msg
    | .ok st =>
      if st.i ≠ b.size ∨ st.toks.size ≠ 1 then throw "invalid phase-7 token"
      let t := (st.toks.getD 0 default).tok
      let valid := match p.kind, t with
        | .strLit, .STRING_LITERAL .. => true
        | .charConst, .CONSTANT (.CabsCharacter_const _) => true
        | .punct, _ => true
        | _, _ => false
      if valid then pure t else throw "phase-7 token kind changed"
  | _ => throw "non-language preprocessing token in phase 7"

/-- A retained phase-four pragma at its exact stream boundary. The parser has
no pack-state input; this schedule must be consumed by a future layout adapter,
not silently interpreted as Cabs support for packed layout. -/
structure Pragma where
  beforeToken : Nat
  payload : List LocatedPPToken
  origin : Origin

structure Input where
  positioned : PositionedTokens
  /-- Origins remain parallel to semantic tokens (excluding EOF). -/
  origins : Array Origin
  pragmas : Array Pragma

/-- A declared virtual lexing stream: each semantic token has its own line,
preceded by a line marker. Its column is its physical anchor's column. This
supplies Cerberus's *lexing* coordinates without pretending that raw file lines
are the lines of a preprocessed stream. Source ranges remain in `origins`.
The independent oracle harness can serialize this layout; conversion never
reads that serialization. -/
def position (fname : String) (i : Nat) (site : SourceSite) (width : Nat) :
    CerbLocation.Pos × CerbLocation.Pos :=
  let sp : CerbLocation.Pos := {
    file := (String.fromUTF8? (bytes site.presumed)).getD (Lex.slice (bytes site.presumed) 0 site.presumed.length),
    line := site.line, col := site.physical.start.column,
    lexingFile := fname, lexingLine := 2 * i + 2 }
  (sp, { sp with col := sp.col + width })

def startPos (fname : String) : CerbLocation.Pos :=
  { file := fname, line := 1, col := 1, lexingFile := fname, lexingLine := 1 }

/-- Authenticated opening pair supplied by the raw adapter after checking full
predefined/header bytes. The include edge identifies the compiler opening,
not a user include of a same-named file or a presumed #line label. -/
structure CompilerOpening where
  root : List Bytes
  header : List Bytes
  directive : SourceSpan

def CompilerOpening.contains (opening : CompilerOpening) (site : SourceSite) : Bool :=
  site.physical.identity == opening.header && site.includes == [opening.directive] &&
    opening.directive.identity == opening.root

/-- One fixed identity computation shared by the converter and its transport
obligation. Injection authority also marks these locations as supplied runtime
context for the five existing library-location consumers. -/
def identify (policy : CerbLocation.LibraryConfig) (injected : Option CompilerOpening)
    (site : SourceSite) : CerbLocation.SourceIdentity :=
  let supplied := injected.any (·.contains site)
  let identity := policy.identify site.physical.identity
  { identity with
    compilerInjected := supplied
    runtimeLibrary := identity.runtimeLibrary || supplied }

/-- Conversion preserves boundaries and spelling. Pragma payloads are opaque
and zero width, including when they separate adjacent string literals. -/
def convertWithIdentity (identity : Option CerbLocation.LibraryConfig)
    (cfg : ParseCfg) (fname : String) (r : PreprocessingResult)
    (injected : Option CompilerOpening := none) : Except String Input := do
  let mut toks : Array PTok := #[]
  let mut origins : Array Origin := #[]
  let mut pragmas : Array Pragma := #[]
  let mut pending : Option (Origin × List LocatedPPToken) := none
  for p in r.tokens do
    match pending with
    | some (o, body) =>
      if p.token.kind = .pragmaEnd then
        pragmas := pragmas.push ⟨toks.size, body.reverse, o⟩
        pending := none
      else if p.token.kind = .pragmaBegin then throw "nested phase-four pragma boundary"
      else pending := some (o, p :: body)
    | none =>
      if p.token.kind = .pragmaBegin then pending := some (p.origin, [])
      else if p.token.kind = .pragmaEnd then throw "unmatched phase-four pragma end"
      else
        let site ← match p.origin.site with
          | none => throw "phase-four token has no source provenance"
          | some s => pure s
        let t ← token cfg p.token
        let spelling ← parserSpelling cfg p.token
        let (sp, ep) := position fname toks.size site spelling.length
        let source ← match identity with
          | none => pure none
          | some policy =>
            match p.origin.identitySite with
            | none => throw "phase-four token has no source identity"
            | some physical => pure (some (identify policy injected physical))
        let sp := { sp with sourceIdentity := source }
        let ep := { ep with sourceIdentity := source }
        toks := toks.push ⟨t, sp, ep, Lex.slice (bytes p.token.spell) 0 p.token.spell.length⟩
        origins := origins.push p.origin
  if pending.isSome then throw "unterminated phase-four pragma boundary"
  let endPos := match toks.back? with
    | none => startPos fname
    | some t => { t.ep with
        line := t.ep.line + 1, col := 1,
        lexingLine := 2 * toks.size + 1 }
  toks := toks.push ⟨.EOF, endPos, endPos, ""⟩
  return ⟨⟨toks, startPos fname⟩, origins, pragmas⟩

/-- Historical positioned comparison view: labels/positions alone, as the
pinned text/JSON oracle can represent them. It carries no identity claim. -/
def convert (cfg : ParseCfg) (fname : String) (r : PreprocessingResult) : Except String Input :=
  convertWithIdentity none cfg fname r

end Phase7
end CSemCParser
