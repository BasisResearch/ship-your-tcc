import Tcc.CSem.Statement
import CSemCParser.Phase7
import Tcc.CSem.PackLayout

/-! Candidate raw-source frontend. This module supplies a concrete pure
`FrontEnd`; it does not replace the parameter of every existing F0 theorem.
The detailed result retains pack schedules and full physical provenance.
Cabs positions carry resolved identity separately from C-visible labels. Installing it as the semantic
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
  identitySite : SourceSite
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
def libraryOrigin (cfg : Config) (site identitySite : SourceSite) : LibraryOrigin :=
  ⟨site, identitySite, cfg.libraryFiles.contains identitySite.physical.identity,
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
  -- The supplied input occupies its resolved image path. Remove stale
  -- duplicates: ImageFiles.initial lets later entries replace earlier data.
  let mainIdentity := FileSys.normComponents cfg.pre.mainName
  let mainPath := [47] ++ List.intercalate [47] mainIdentity
  let fs : FileSys := ⟨(mainPath, src) :: cfg.fs.files.filter (fun (p, _) =>
    FileSys.normComponents p != mainIdentity)⟩
  let pp ← match phases14WithOrigins cfg.pre fs src with
    | .error e => throw s!"preprocessing line {e.line}: {e.msg}"
    | .ok p => pure p
  let fname := (String.fromUTF8? (Phase7.bytes cfg.pre.mainName)).getD
    (Lex.slice (Phase7.bytes cfg.pre.mainName) 0 cfg.pre.mainName.length)
  let input ← Phase7.convertWithIdentity (some ⟨cfg.libraryFiles⟩) cfg.parser fname pp
  let tu ← match parsePositioned cfg.parser input.positioned with
    | .error e => throw s!"{showPos e.pos}: {e.msg}"
    | .ok tu => pure tu
  let libraries := input.origins.filterMap (fun o => do
    let site ← o.site
    let identity ← o.identitySite
    pure (libraryOrigin cfg site identity))
  pure ⟨digest src, pp, input, tu, libraries⟩

/-- Errors of the checked semantic entry are named obligations, never
Supported exclusions. The audit/parse entry `run` still retains full evidence. -/
inductive FrontEndError where
  | parse (message : String)
  | unimplementedPackLayout (events : Array Phase7.Pragma)

def FrontEndError.code : FrontEndError → String
  | .parse _ => "parse"
  | .unimplementedPackLayout _ => "unimplemented-pack-layout"

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
  pure r

/-- F0-shaped projection of the checked entry. It is not installed globally:
its explicit diagnostic gaps must be closed for the whole compiler theorem. -/
def frontEnd (cfg : Config) : FrontEnd := fun src =>
  match checked cfg src with
  | .error _ => none
  | .ok r => some (r.digest, r.cabs)

/-- Install the identity-aware raw frontend and Core runtime loading together.
The caller still supplies the runtime bytes and their resolved image identities;
proving that these describe TccImage is an F1/F6 connection obligation. -/
def semanticConfig (cfg : Config) (base : Tcc.CSem.Config)
    (runtime : RuntimeIdentities) : Tcc.CSem.Config :=
  { base with
    frontEnd := frontEnd cfg
    identity := some ⟨⟨cfg.libraryFiles⟩, runtime⟩ }

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

/-- Named transport witness at the concrete phase-7 boundary. Positions retain
presumed labels while membership follows the token's identity derivation. -/
structure TokenIdentityConnection (cfg : Config) (origin : Origin) (p : PTok) where
  site : SourceSite
  originSite : origin.identitySite = some site
  identity : p.sp.sourceIdentity = some ((CerbLocation.LibraryConfig.mk cfg.libraryFiles).identify site.physical.identity)
  endpoint : p.ep.sourceIdentity = p.sp.sourceIdentity

/-- Full proof of phase-7 identity transport remains open. This fixed relation
replaces the false premise that actual identities agree with filename suffixes.
Grammar/Ail/Core preservation beyond this boundary remains a separate proof duty. -/
def LibraryConnection_Statement (cfg : Config) : Prop :=
  ∀ src r, run cfg src = .ok r → ∀ (i : Nat) (origin : Origin) (p : PTok),
    r.input.origins[i]? = some origin → r.input.positioned.tokens[i]? = some p →
      Nonempty (TokenIdentityConnection cfg origin p)

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
  library : LibraryConnection_Statement cfg
  pragmas : PragmaConnection_Statement cfg semanticLayout

end Tcc.CSem.Raw
