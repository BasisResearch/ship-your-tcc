import Tcc.CSem.Statement
import CSemCParser.Phase7
import Tcc.CSem.PackLayout
import Spec.Pre.Tcc.Cfg
import Spec.Pre.Tcc.ImageFS

/-! Candidate raw-source frontend. This module supplies a concrete pure
`FrontEnd`; it does not replace the parameter of every existing F0 theorem.
The detailed result retains pack schedules and full physical provenance.
Cabs positions carry resolved identity separately from C-visible labels. Installing it as the semantic
frontend owes the explicitly named connection obligations below. -/
namespace Tcc.CSem.Raw

open CSem.Pre
open CSemCParser

/-- Supplied compiler environment. These are complete physical input bytes,
not filenames or hashes inferred from declarations. The image connection is
stated separately; the default pins come from the generated image table. -/
structure CompilerEnvironment where
  predefined : Bytes
  root : List Bytes
  header : List Bytes
  headerContents : Bytes
  deriving Repr

def tccEnvironment : CompilerEnvironment where
  predefined := bs _root_.CSem.Pre.Tcc.predefsRun
  root := FileSys.normComponents _root_.CSem.Pre.Tcc.cfgTcc.predefsName
  header := FileSys.normComponents (bs "/lib/tcc/include/tccdefs.h")
  headerContents := (FileSys.lookup _root_.CSem.Pre.Tcc.imageFS (bs "/lib/tcc/include/tccdefs.h")).getD []

structure Config where
  pre : PreCfg
  fs : FileSys
  parser : ParseCfg := {}
  compilerEnvironment : Option CompilerEnvironment := none
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
def libraryOrigin (cfg : Config) (site identitySite : SourceSite)
    (injected : Bool := false) : LibraryOrigin :=
  ⟨site, identitySite, cfg.libraryFiles.contains identitySite.physical.identity || injected,
    presumedLibrary site.presumed⟩

/-- Preserve each complete parsed signature, original binding carrier and
locations. This is environment data, never a replacement Cabs specification. -/
structure InjectedBinding where
  identity : compiler_binding
  declaration : cabs_declaration
  identifier : identifier
  carrier : attribute0

/-- Authenticate the actual initial source opening. A user include of the
same physical header has a different directive/root. Ambiguous configured
root/main identities have no authority. No #line field is consulted. -/
def compilerOpening (cfg : Config) (pp : PreprocessingResult) : Option Phase7.CompilerOpening := do
  let env ← cfg.compilerEnvironment
  if cfg.pre.predefs != env.predefined || FileSys.normComponents cfg.pre.predefsName != env.root ||
      FileSys.normComponents cfg.pre.mainName == env.root ||
      FileSys.normComponents cfg.pre.mainName == env.header then none else do
    let root ← pp.sources.head?
    if root.identity != env.root || root.contents != env.predefined || root.includedAt.isSome then none else do
      let header ← pp.sources.find? (fun f => f.identity == env.header &&
        f.contents == env.headerContents && f.includedAt.any (fun edge => edge.identity == env.root))
      let directive ← header.includedAt
      some ⟨env.root, env.header, directive⟩

def bindingIdentifier : direct_declarator → Option (attributes × identifier)
  | .DDecl_identifier attrs ident => some (attrs, ident)
  | .DDecl_declarator (.Declarator _ d) => bindingIdentifier d
  | .DDecl_array d _ | .DDecl_function d _ => bindingIdentifier d

def injectedBindings : translation_unit → List InjectedBinding
  | .TUnit declarations => declarations.flatMap fun ext => match ext with
    | .EDecl_decl declaration@(.Declaration_base _ _ initDecls) =>
        initDecls.filterMap fun (.InitDecl _ (.Declarator _ d) _) => do
          let (.Attrs attrs, ident) ← bindingIdentifier d
          let binding ← compiler_binding_for ident
          let carrier ← attrs.find? (fun a => match a.attr_id with
            | .Identifier loc name => name == "asm_label" && CerbLocation.isCompilerInjected loc)
          some ⟨binding, declaration, ident, carrier⟩
    | _ => []

structure Result where
  digest : String
  preprocessing : PreprocessingResult
  input : Phase7.Input
  cabs : translation_unit
  libraryOrigins : Array LibraryOrigin
  bindings : List InjectedBinding

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
  let input ← Phase7.convertWithIdentity (some ⟨cfg.libraryFiles⟩) cfg.parser fname pp (compilerOpening cfg pp)
  let tu ← match parsePositioned cfg.parser input.positioned with
    | .error e => throw s!"{showPos e.pos}: {e.msg}"
    | .ok tu => pure tu
  let libraries := input.origins.filterMap (fun o => do
    let site ← o.site
    let identity ← o.identitySite
    pure (libraryOrigin cfg site identity ((compilerOpening cfg pp).any (·.contains identity))))
  pure ⟨digest src, pp, input, tu, libraries, injectedBindings tu⟩

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

/-- F1/F6 must connect the supplied environment to the actual image. These
are full byte pins and an explicit resolved opening identity, not a membership
claim inferred from the header's presumed name. -/
structure CompilerEnvironmentImage_Statement (cfg : Config) (image : FileSys) : Prop where
  environment : cfg.compilerEnvironment = some tccEnvironment
  predefined : cfg.pre.predefs = tccEnvironment.predefined
  root : FileSys.normComponents cfg.pre.predefsName = tccEnvironment.root
  header : FileSys.lookup image (bs "/lib/tcc/include/tccdefs.h") = some tccEnvironment.headerContents

/-- OPEN F3: independently captured model openings must supply this new
compiler-environment authority. The earlier ghost driver compares parser
construction on model-produced positioned tokens; it does not call independent
`Raw.checked cfgTcc`. Add that independent raw comparison as well as this
opening connection. Full records include bytes and include edges, including
empty and repeated openings. -/
def CompilerOpeningConnection_Statement (cfg : Config)
    (modelOpenings : Bytes → Except String (List SourceFile)) : Prop :=
  ∀ src r, run cfg src = .ok r → modelOpenings src = .ok r.preprocessing.sources

/-- A Core external binding retains the C spellings, but all declarations of
one linker function must name exactly the same canonical symbols. Consequently
function-pointer creation, storage and equality use the existing single
`Mem.fun_ptrval` identity; there is no call-only alias rewrite. -/
structure BindingResolution (binding : InjectedBinding) (core : file Unit) where
  symbols : List sym
  nonempty : symbols ≠ []
  kind : linking_kind
  alias : fmapLookupBy (fun (a b : identifier) => Lem_Basic_classes.ordCompare a b) (.Identifier .unknown binding.identity.binding_name) core.extern = some (symbols, kind)
  target : ∀ targets k,
    fmapLookupBy (fun (a b : identifier) => Lem_Basic_classes.ordCompare a b) (.Identifier .unknown binding.identity.binding_linker_name) core.extern = some (targets, k) →
      targets = symbols

/-- OPEN F1/F3/F7 transport: actual raw elaboration must supply canonical
binding resolution. The checked executions are finite evidence; no generic
compiler, linking or function-address refinement proof is asserted here. -/
def BindingResolution_Statement (raw : Config) (base : Tcc.CSem.Config)
    (runtime : RuntimeIdentities) : Prop :=
  ∀ src r n, checked raw src = .ok r →
    let cfg := semanticConfig raw base runtime
    letI : CerbImplCfg.Impl := ⟨cfg.implementation⟩
    letI : LemFuel := ⟨n⟩
    letI : CerbGlobal.Switches := ⟨CerbGlobal.defaultSwitches⟩
    letI : CerberusFresh.Digest := ⟨r.digest⟩
    ∀ std core supply, stdlib cfg = .ok std → elaborate cfg std r.cabs = .ok (core, supply) →
      ∀ binding ∈ r.bindings, binding.identity.binding_kind = .Runtime_alias →
        Nonempty (BindingResolution binding core)

/-- A runtime alias needs an actual Core implementation under its target
name. Merely declaring the external function does not supply this record.
The full parsed C signature remains in `InjectedBinding.declaration`. -/
structure RuntimeBindingEntry (binding : compiler_binding)
    (names : Fmap String sym) (functions : fun_map Unit) where
  symbol : sym
  implementation : fun_map_decl Unit
  target : fmapLookupBy (fun (a b : String) => Lem_Basic_classes.ordCompare a b)
    binding.binding_linker_name names = some symbol
  body : fmapLookupBy (fun (a b : sym) => Lem_Basic_classes.ordCompare a b)
    symbol functions = some implementation

/-- OPEN F1/F7 runtime coverage: the current Core runtime has entries for
memcpy, memcmp, malloc, realloc and free. The other runtime aliases still need
their library bodies and image-pinned machine refinements. Function-lifetime
allocation is a separate intrinsic obligation, never a malloc synonym. -/
def RuntimeBindingCoverage_Statement (cfg : Tcc.CSem.Config) : Prop :=
  letI : CerbImplCfg.Impl := ⟨cfg.implementation⟩
  ∀ std, stdlib cfg = .ok std → ∀ binding ∈ compiler_bindings,
    binding.binding_kind = .Runtime_alias →
      Nonempty (RuntimeBindingEntry binding std.1.1 std.1.2)

/-- Executable inventory of the actual loaded Core table. `none` identifies
the separate intrinsic duty; `some false` is a missing runtime body. -/
structure RuntimeBindingStatus where
  identity : compiler_binding
  coreBody : Option Bool

def runtimeBindingInventory (cfg : Tcc.CSem.Config) : Except String (List RuntimeBindingStatus) :=
  letI : CerbImplCfg.Impl := ⟨cfg.implementation⟩
  do
    let std ← stdlib cfg
    pure (compiler_bindings.map fun binding =>
      match binding.binding_kind with
      | .Function_allocation_intrinsic => ⟨binding, none⟩
      | .Runtime_alias =>
          let symbol := fmapLookupBy (fun (a b : String) => Lem_Basic_classes.ordCompare a b)
            binding.binding_linker_name std.1.1
          ⟨binding, some (symbol.any fun s =>
            (fmapLookupBy (fun (a b : sym) => Lem_Basic_classes.ordCompare a b) s std.1.2).isSome)⟩)

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
structure TokenIdentityConnection (cfg : Config) (pp : PreprocessingResult) (origin : Origin) (p : PTok) where
  site : SourceSite
  originSite : origin.identitySite = some site
  identity : p.sp.sourceIdentity = some (Phase7.identify ⟨cfg.libraryFiles⟩ (compilerOpening cfg pp) site)
  endpoint : p.ep.sourceIdentity = p.sp.sourceIdentity

/-- Full proof of phase-7 identity transport remains open. This fixed relation
replaces the false premise that actual identities agree with filename suffixes.
Grammar/Ail/Core preservation beyond this boundary remains a separate proof duty. -/
def LibraryConnection_Statement (cfg : Config) : Prop :=
  ∀ src r, run cfg src = .ok r → ∀ (i : Nat) (origin : Origin) (p : PTok),
    r.input.origins[i]? = some origin → r.input.positioned.tokens[i]? = some p →
      Nonempty (TokenIdentityConnection cfg r.preprocessing origin p)

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
