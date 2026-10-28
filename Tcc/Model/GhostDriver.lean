import Tcc.Model.Tie
import Tcc.Model.Preprocess

/-! Focused shared-token driver. Phase-four spelling and provenance are the
only external syntax inputs. The compiler lexer/parser/codegen run on a
virtual token-per-line buffer. Cabs is constructed by their parsing actions,
never by CSemCParser.parsePositioned. This is an independent testing seam;
connecting the ordinary preprocessor's replay locations is an open duty. -/
namespace Tcc.Model

/-- One eraser per annotated compiler shape; compiler token words stay intact. -/
def TokPtr.eraseGhost (p : TokPtr) : TokPtr := { p with ghostOrigins := #[] }
def TokenString.eraseGhost (s : TokenString) : TokenString := { s with ghostOrigins := #[] }
def MacroFrame.eraseGhost (f : MacroFrame) : MacroFrame :=
  { f with str := f.str.eraseGhost, prev_ptr := f.prev_ptr.map TokPtr.eraseGhost }

def BufferedFile.eraseGhost (f : BufferedFile) : BufferedFile :=
  { f with ghostPhysical := #[], ghostBuffer := #[], ghostPending := #[] }

end Tcc.Model

namespace Tcc.Model.Ghost
open C

/-- The one-token-per-line transport never runs phases 1-2 again: synthetic
buffers use tcc_open_bf. Original spelling and provenance stay in Input. -/
def transport (input : Input) : ByteArray :=
  input.spelling.foldl (fun b t => b ++ ⟨t.spell.toArray⟩ ++ "\n".toUTF8) ByteArray.empty

/-- Erase every ghost field, retaining all parser/codegen/compiler state. -/
def erase (s : TccSt) : TccSt :=
  { s with
    ghostEnabled := false, ghostFixture := false, ghostParsing := false,
    ghostNextDepth := 0, ghostStringCapture := false, ghostLexOffset := 0,
    file := s.file.map BufferedFile.eraseGhost,
    ghostLexCharStart := none, ghostLexPhysicalStart := none, ghostLexSourceId := 0, ghostPriorOrigin := 0,
    ghostTokOrigin := 0, ghostExpansionOrigin := 0,
    ghostOriginTable := #[⟨.unavailable,none,false⟩],
    ghostSymD := {}, ghostSymE := {}, ghostArgIndex := {}, ghostLastOrigins := #[],
    macro_ptr := s.macro_ptr.map TokPtr.eraseGhost,
    macro_stack := s.macro_stack.map MacroFrame.eraseGhost,
    pragma_emitted := s.pragma_emitted.eraseGhost,
    tokstr_buf := s.tokstr_buf.eraseGhost,
    unget_buf := s.unget_buf.eraseGhost,
    ghostSaved := #[], ghostReplay := {}, ghostFdSources := {}, ghostDirective := none,
    ghostImplicitOffset := none, ghostCompilerRoot := none, ghostCompilerEvent := none, ghostLibraryFiles := [],
    ghostSources := #[], ghostClosedPhysical := {}, ghostOrigins := #[], ghostPragmas := #[],
    ghostInput := none, ghostCurrent := 0, ghostPrior := 0,
    ghost := #[], ghostAudit := #[], ghostNormalizations := #[], ghostDrops := #[], ghostTokenErrors := #[] }

/-- Updating syntax observations cannot change the erased compiler state.
This law covers any fragments, audits, normalizations, drops and token errors,
including partial observations returned by a failed isolated parser. -/
theorem erase_syntaxFields (st : TccSt) (fragments audit : Array Fragment)
    (normalizations : Array NormalizationUse) (drops : Array Drop)
    (tokenErrors : Array String) :
    erase { st with
      ghost := fragments, ghostAudit := audit,
      ghostNormalizations := normalizations, ghostDrops := drops,
      ghostTokenErrors := tokenErrors } = erase st := rfl

/-- Local replay noninterference, including diagnostics. Proved below from
snapshot restoration; saved-word coverage and
complete typed expression construction additionally require the scanner/parser
induction in RawConstruction_Statement and InjectedAuthorityTransport_Statement. -/
def AssociationReplayErasure_Statement : Prop :=
  ∀ R saved st,
    match (genericAssociationSyntax R saved).run st with
    | .ok _ after => erase after = erase st
    | .error _ _ => False

/-- The actual replay restores the snapshot's entire compiler state on
parser success, appended-source detection and parser diagnostics. The parser
result is arbitrary: no construction, coverage or parser-success premise is
needed for this local law. -/
theorem associationReplayErasure : AssociationReplayErasure_Statement := by
  intro R saved st
  simp only [genericAssociationSyntax, EStateM.run]
  split <;> rename_i _ _ _ hresult
  all_goals
    split at hresult
    · split at hresult <;> cases hresult <;> exact erase_syntaxFields _ _ _ _ _ _
    · cases hresult <;> exact erase_syntaxFields _ _ _ _ _ _

/-- Supplied by the F3/F6 induction over actual reads, trigraph replacement,
held-byte refill, ungets and successful splice branches. Finite comparisons
supply neither these universal laws nor RawErasure_Statement. -/
structure PhysicalTransformObligations : Prop where
  blockErasure : ∀ bytes origins, (trigraphBlock bytes origins).1 = tcc_trigraphs bytes
  blockShape : ∀ bytes origins, origins.size = bytes.size →
    (trigraphBlock bytes origins).2.size = (trigraphBlock bytes origins).1.size
  spliceErasure : ∀ st first last,
    match (ghostSplice first last).run st with
    | .ok _ after => erase after = erase st
    | .error _ _ => False
  refillErasure : ∀ R st,
    match (handle_eob R).run st, (handle_eob R).run (erase st) with
    | .ok a after, .ok b plain => a = b ∧ erase after = erase plain
    | .error a after, .error b plain => a = b ∧ erase after = erase plain
    | _, _ => False

/-- Open physical interval law for the actual trigraph transform. The F3/F6
read/refill invariant must supply contiguous input intervals, carry held bytes,
and transport this local law through successful LF/CRLF splices and ungets.
This states the transform's meaning without importing spec output into it. -/
def PhysicalOriginMap_Statement : Prop :=
  ∀ bytes,
    let raw := bytes.data.mapIdx (fun i c =>
      ({ byte := c, start := i, stop := i+1 } : PhysicalChar))
    let mapped := (trigraphBlock bytes raw).2
    mapped.toList.Pairwise (fun a b => a.stop ≤ b.start) ∧
    ∀ c ∈ mapped.toList,
      c.start < c.stop ∧ c.stop ≤ bytes.size ∧
      (trigraphBlock (bytes.extract c.start c.stop) #[]).1 = ⟨#[c.byte]⟩

/-- Open CRLF correspondence for the actual trigraph-block shadow, with no
restriction on CRLF, lone CR, CR at EOF or absent terminal newline. This is
an obligation about a view, not a construction from spec preprocessing. -/
def PhysicalReadView_Statement : Prop :=
  ∀ bytes,
    let raw := bytes.data.mapIdx (fun i c =>
      ({ byte := c, start := i, stop := i+1 } : PhysicalChar))
    let mapped := (trigraphBlock bytes raw).2
    (normalizePhysical mapped.toList).map (fun c => (c.byte,c.start,c.stop)) =
      (CSem.Pre.locatedPhase1 true bytes.toList ⟨0,1,1⟩ []).map
        (fun c => (c.c,(c.physical.getD default).1.offset,(c.physical.getD default).2.offset))

def action (R : Rec) (src : ByteArray) : M Unit := do
  tccpp_new R
  tccelf_new
  tcc_open_bf "/ghost-tokens.c".toUTF8 (some src)
  tccgen_init R
  let _ ← tccgen_compile R

/-- No reference AST, diagnostic digest, or reference code/data is an input. -/
def run (input : Input) (observe : Bool := true) : EStateM.Result Err TccSt Unit :=
  let src := transport input
  let fuel := 16*src.size+4096
  let initial : TccSt := {
    fuel := fuel, cversion := 201112, warn_none := 1,
    error_set_jmp_enabled := 1, output_type := TCC_OUTPUT_MEMORY,
    ghostFixture := observe, ghostParsing := observe,
    ghostInput := if observe then some input else none }
  (action (Tie.tie fuel) src).run initial

/-- The ordinary physical-source lexer, preprocessor and compiler parser.
Only model source bytes/files enter this computation; the spec parser is
absent. Pragma effects execute through the unchanged compilation path. -/
def rawAction (R : Rec) : M Unit := do
  if (← tcc_open "/prog.c".toUTF8) < 0 then throw (.internal "missing /prog.c")
  preprocess_start R AFF_TYPE_C
  tccelf_new
  tccgen_init R
  let _ ← tccgen_compile R

def runRaw (src : ByteArray) (fs : List (CSem.Pre.Bytes × CSem.Pre.Bytes))
    (observe : Bool := true) (libraries : List (List CSem.Pre.Bytes) := []) : EStateM.Result Err TccSt Unit :=
  let base := preprocessState src fs
  let initial := { base with
    ghostEnabled := observe, cversion := 201112, warn_none := 1,
    ghostLibraryFiles := if observe then libraries else [],
    error_set_jmp_enabled := 1, output_type := TCC_OUTPUT_MEMORY,
    tcc_lib_path := "/lib/tcc".toUTF8,
    sysinclude_paths := #["/lib/tcc/include".toUTF8, "/include".toUTF8] }
  (rawAction (Tie.tie base.fuel)).run initial

def output (s : TccSt) : Except Gap translation_unit := do
  if !s.ghostTokenErrors.isEmpty then
    throw ⟨.tu,⟨0,s.ghostCurrent⟩,"token/provenance transport disagreement"⟩
  match s.ghost.toList with
  | [f] => match ← f.value with
    | .translationUnit tu => pure tu
    | _ => throw ⟨.tu,f.range,"model did not construct a translation unit"⟩
  | _ => throw ⟨.tu,⟨0,s.ghostCurrent⟩,"model ghost stack has extra/missing productions"⟩

/-- Whole parser/codegen state erasure, including identical errors. F3/F6
must prove this by induction on the modeled actions. Finite byte comparisons
do not supply this proposition. -/
def Erasure_Statement : Prop :=
  ∀ input,
    match run input true, run input false with
    | .ok _ a, .ok _ b => erase a = erase b
    | .error e a, .error e' b => e = e' ∧ erase a = erase b
    | _, _ => False

/-- Exact typed agreement on a shared positioned token fixture. No arbitrary
normalizer is parameterized here. Whole-C coverage, fuel adequacy and the
ordinary raw-source connection remain required for DESIGN §6.2. -/
def ParserAgreement_Statement (cfg : CSemCParser.ParseCfg) : Prop :=
  ∀ input s tu,
    run input = .ok () s → output s = .ok tu →
    CSemCParser.parsePositioned cfg input.language.positioned = .ok tu

/-- Ordinary-source erasure is a separate unproved obligation from fixture
transport. It includes all compiler state and both success/error outcomes. -/
def RawErasure_Statement : Prop :=
  ∀ src fs,
    match runRaw src fs, runRaw src fs false with
    | .ok _ a, .ok _ b => erase a = erase b
    | .error e a, .error e' b => e = e' ∧ erase a = erase b
    | _, _ => False

/-- Named witness for required total ghost construction, without a compiler
refusal or a missing-construction disjunct. Supplied by F3/F6 for all
Supported/Fits programs, not by filtering differential successes. -/
structure RawConstruction (src : ByteArray) (fs : List (CSem.Pre.Bytes × CSem.Pre.Bytes)) where
  state : TccSt
  tree : translation_unit
  compiled : runRaw src fs = .ok () state
  constructed : output state = .ok tree

def RawConstruction_Statement (supported fits : ByteArray → Prop)
    (fs : List (CSem.Pre.Bytes × CSem.Pre.Bytes)) : Prop :=
  ∀ src, supported src → fits src → Nonempty (RawConstruction src fs)

/-- Raw source parser agreement with the model's own retained positioned
stream; source provenance/phase-four correctness and binary refinement remain
independent obligations. No arbitrary syntax normalization is allowed. -/
def RawParserAgreement_Statement (cfg : CSemCParser.ParseCfg) : Prop :=
  ∀ src fs s tu input,
    runRaw src fs = .ok () s → s.ghostInput = some input → output s = .ok tu →
    CSemCParser.parsePositioned cfg input.language.positioned = .ok tu

/-- Agreement for the fixed GNU-attribute, asm-binding and logical-chain
encodings recorded in ghostNormalizations. This is exact Cabs equality,
not a caller-selected relation, and remains unproved. F1 must provide the
independent grammar/meaning link; F3 must prove the action construction. -/
def NormalizationAgreement_Statement (cfg : CSemCParser.ParseCfg) : Prop :=
  RawParserAgreement_Statement cfg

end Tcc.Model.Ghost
