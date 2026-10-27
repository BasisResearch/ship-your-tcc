import Tcc.CSem.Statement
import CSemCParser.Phase7
import Tcc.CSem.PackLayout

/-! Candidate raw-source frontend. This module supplies a concrete pure
`FrontEnd`; it does not replace the parameter of every existing F0 theorem.
The detailed result retains information Cabs cannot currently express (pack
schedule and physical library identity). Installing it as the semantic
frontend owes the explicitly named connection obligations below. -/
namespace Tcc.CSem.Raw

open CSem.Pre
open CSemCParser

structure Config where
  pre : PreCfg
  fs : FileSys
  parser : ParseCfg := {}
  /-- Explicit resolved image identities admitted as runtime library files.
  Neither suffixes nor #line labels establish this membership. -/
  libraryFiles : List (List Bytes) := []

structure LibraryOrigin where
  site : SourceSite
  actualLibrary : Bool
  presumedLibrary : Bool
  deriving Repr

/-- Byte-list form of Unix Filename.dirname, without partial String slice
iterators or panic/native seams. Labels remain labels. -/
def labelDirectory (path : Bytes) : Bytes :=
  let trimmed := (path.reverse.dropWhile (· = 47)).reverse
  if trimmed.isEmpty then (if path.isEmpty then bs "." else bs "/")
  else if !(trimmed.contains 47) then bs "."
  else
    let dir := ((trimmed.reverse.dropWhile (· ≠ 47)).dropWhile (· = 47)).reverse
    if dir.isEmpty then bs "/" else dir

def presumedLibrary (path : Bytes) : Bool :=
  let dir := labelDirectory path
  [bs "runtime/libc/include", bs "runtime/libcore", bs "runtime/libcore/impls"].any
    (fun d => dir = d ∨ (bs "/" ++ d).isSuffixOf dir)

/-- Keep both answers visible even when they disagree. This is an audit, not
an extra Supported restriction or a successful classification by refusal. -/
def libraryOrigin (cfg : Config) (site : SourceSite) : LibraryOrigin :=
  ⟨site, cfg.libraryFiles.contains site.physical.identity,
    presumedLibrary site.presumed⟩

structure Result where
  digest : String
  preprocessing : PreprocessingResult
  input : Phase7.Input
  cabs : translation_unit
  libraryOrigins : Array LibraryOrigin

/-- Raw bytes, including non-UTF-8 bytes, are digested directly, as the OCaml
Digest.file oracle does. No conversion to String precedes MD5. -/
def digest (src : Bytes) : String :=
  CerberusFresh.MD5.hex (CerberusFresh.MD5.digestBytes (Phase7.bytes src))

def run (cfg : Config) (src : Bytes) : Except String Result := do
  let pp ← match phases14WithOrigins cfg.pre cfg.fs src with
    | .error e => throw s!"preprocessing line {e.line}: {e.msg}"
    | .ok p => pure p
  let fname := (String.fromUTF8? (Phase7.bytes cfg.pre.mainName)).getD
    (Lex.slice (Phase7.bytes cfg.pre.mainName) 0 cfg.pre.mainName.length)
  let input ← Phase7.convert cfg.parser fname pp
  let tu ← match parsePositioned cfg.parser input.positioned with
    | .error e => throw s!"{showPos e.pos}: {e.msg}"
    | .ok tu => pure tu
  let libraries := input.origins.filterMap (fun o => o.site.map (libraryOrigin cfg))
  pure ⟨digest src, pp, input, tu, libraries⟩

/-- Errors of the checked semantic entry are named obligations, never
Supported exclusions. The audit/parse entry `run` still retains full evidence. -/
inductive FrontEndError where
  | parse (message : String)
  | unimplementedPackLayout (events : Array Phase7.Pragma)
  | libraryIdentityMismatch (origins : Array LibraryOrigin)

def FrontEndError.code : FrontEndError → String
  | .parse _ => "parse"
  | .unimplementedPackLayout _ => "unimplemented-pack-layout"
  | .libraryIdentityMismatch _ => "library-identity-mismatch"

/-- Concrete pure checked frontend. Packed layout cannot be represented by
this Cabs pipeline yet; return a named diagnostic instead of successful Cabs
with silently ignored pack effects. This is an open whole-C obligation. -/
def checked (cfg : Config) (src : Bytes) : Except FrontEndError Result := do
  let r ← match run cfg src with
    | .error e => throw (.parse e)
    | .ok r => pure r
  let packs := r.input.pragmas.filter (fun e =>
    (e.payload.head?.map (fun p => p.token.spell)) = some (bs "pack"))
  if !packs.isEmpty then throw (.unimplementedPackLayout packs)
  let mismatches := r.libraryOrigins.filter (fun o => o.actualLibrary != o.presumedLibrary)
  if !mismatches.isEmpty then throw (.libraryIdentityMismatch mismatches)
  pure r

/-- F0-shaped projection of the checked entry. It is not installed globally:
its explicit diagnostic gaps must be closed for the whole compiler theorem. -/
def frontEnd (cfg : Config) : FrontEnd := fun src =>
  match checked cfg src with
  | .error _ => none
  | .ok r => some (r.digest, r.cabs)

/-- Phase-7 language-token agreement is checked on shared fully positioned
inputs, not on ASTs with their locations erased. A future proof of the
preprocessor and lexer, plus the grammar proof, supplies this premise. -/
def ParserConnection_Statement (cfg : Config)
    (languageTokens : PreprocessingResult → Except String PositionedTokens) : Prop :=
  ∀ src r, run cfg src = .ok r → languageTokens r.preprocessing = .ok r.input.positioned

/-- The byte-list audit must agree with the inherited semantic classifier.
This connection is separate from actual runtime identity; suffix agreement
alone cannot identify a library file. -/
def LibraryClassifier_Statement : Prop :=
  ∀ path : Bytes, presumedLibrary path = CerbLocation.isLibraryPathSuffix
    ((String.fromUTF8? (Phase7.bytes path)).getD (Lex.slice (Phase7.bytes path) 0 path.length))

/-- The current semantic library classifier reads presumed Cabs labels.
Its agreement with the actual image identities must be established, or that
classifier must be replaced. #line is not evidence of runtime membership. -/
def LibraryConnection_Statement (cfg : Config) : Prop :=
  ∀ src r, run cfg src = .ok r →
    ∀ o ∈ r.libraryOrigins.toList, o.actualLibrary = o.presumedLibrary

/-- A real semantic layout consumer must answer every field-layout query
with the fixed `layoutReference` offsets/size/alignment. Its reference cannot
be chosen by the caller or discharged by selecting identity on Cabs. Natural
field extraction and packed bitfield layout are additional whole-C duties. -/
def PragmaConnection_Statement (cfg : Config)
    (semanticLayout : translation_unit → LayoutQuery → Except String AggregateLayout) : Prop :=
  ∀ src r, run cfg src = .ok r → ∀ query,
    semanticLayout r.cabs query = layoutReference cfg.pre r.input query

/-- Named obligations required before installing the candidate in a semantic
configuration. No instance of this structure is asserted. A future Cabs/Core layout adapter
must consume the fixed pack policy before the checked diagnostic can close. -/
structure ConnectionObligations (cfg : Config)
    (languageTokens : PreprocessingResult → Except String PositionedTokens)
    (semanticLayout : translation_unit → LayoutQuery → Except String AggregateLayout) : Prop where
  parser : ParserConnection_Statement cfg languageTokens
  classifier : LibraryClassifier_Statement
  library : LibraryConnection_Statement cfg
  pragmas : PragmaConnection_Statement cfg semanticLayout

end Tcc.CSem.Raw
