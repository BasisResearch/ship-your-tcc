import Tcc.Model.GhostDriver
import Tcc.CSem.RawFrontEnd

/-! Open F1/F3/F6 provenance obligations. No test result inhabits these types.
Runtime implementation coverage remains Raw.RuntimeBindingCoverage_Statement
(F1/F7); opening authority does not supply it. -/
namespace Tcc.Model.Ghost
open CSem.Pre CSemCParser

/-- Model-owned opening projection. The actual command-line opening is created
later than main in the compiler; the spec processes predefines before main.
Name predefine/header roles by control IDs, retaining every other opening in model order.
No spec source set selects or deletes a model opening. -/
def sourceFile (s : ModelSource) : SourceFile where
  openedAs := s.openedAs.toList
  identity := s.identity
  contents := s.contents.toList
  includedAt := s.includedAt

def openingFiles (s : TccSt) : List SourceFile :=
  let root := s.ghostCompilerRoot.map (·.source.id)
  let roots := s.ghostSources.toList.filter (fun f => root == some f.id)
  let header := s.ghostCompilerEvent.map (·.header.id)
  let headers := s.ghostSources.toList.filter (fun f => header == some f.id)
  let others := s.ghostSources.toList.filter (fun f =>
    !f.generated && root != some f.id && header != some f.id)
  (roots ++ headers ++ others).map sourceFile

/-- F3 connects the actual opening event and all source bytes/edges to the
independent raw computation. F1/F6 separately pin the compiler environment to
TccImage; no current field proves that a supplied FS is a valid image. -/
structure OpeningTransport (cfg : Tcc.CSem.Raw.Config) (s : TccSt)
    (r : Tcc.CSem.Raw.Result) : Prop where
  sources : openingFiles s = r.preprocessing.sources
  authority : s.ghostCompilerEvent.bind CompilerEvent.authority =
    Tcc.CSem.Raw.compilerOpening cfg r.preprocessing

/-- Named per-token field transport through the shared Phase7.identify carrier.
Macro definition identity and invocation diagnostics have separate fields. -/
structure LexicalAuthorityTransport (s : TccSt) (input : Input) : Prop where
  tokens : ∀ (i : Nat) (p : PTok) (o : Origin), input.language.positioned.tokens[i]? = some p →
    input.language.origins[i]? = some o →
    p.sp.sourceIdentity = o.identitySite.map (Phase7.identify
      { files := s.ghostLibraryFiles } (s.ghostCompilerEvent.bind CompilerEvent.authority))
  endpoints : ∀ p ∈ input.language.positioned.tokens.toList,
    p.ep.sourceIdentity = p.sp.sourceIdentity

/-- Complete successful run required for every Supported/Fits source. -/
structure AuthorityRun (cfg : Tcc.CSem.Raw.Config) (src : ByteArray) where
  state : TccSt
  raw : Tcc.CSem.Raw.Result
  input : Input
  compiled : runRaw src cfg.fs.files true cfg.libraryFiles = .ok () state
  checked : Tcc.CSem.Raw.checked cfg src.toList = .ok raw
  retained : state.ghostInput = some input
  openings : OpeningTransport cfg state raw
  lexical : LexicalAuthorityTransport state input
  constructed : output state = .ok raw.cabs

/-- OPEN F3/F6: whole-source transport with no refusal alternative or filter on
successful construction. F1 supplies the raw grammar/meaning and environment
connections; F6 supplies actual image reachability and ghost noninterference. -/
def InjectedAuthorityTransport_Statement (cfg : Tcc.CSem.Raw.Config)
    (supported fits : ByteArray → Prop) : Prop :=
  ∀ src, supported src → fits src → Nonempty (AuthorityRun cfg src)

/-- OPEN F3/F6: capture and transport erase from the complete compiler state,
including failures, independently of F1/F7 alias meaning and runtime bodies. -/
def InjectedAuthorityErasure_Statement : Prop :=
  ∀ src fs libraries,
    match runRaw src fs true libraries, runRaw src fs false libraries with
    | .ok _ a, .ok _ b => erase a = erase b
    | .error e a, .error e' b => e = e' ∧ erase a = erase b
    | _, _ => False
end Tcc.Model.Ghost
