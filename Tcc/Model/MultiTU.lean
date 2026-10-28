import Tcc.Model.Main
import Tcc.Model.CompilerOpening

/-! Open whole-command duties. Finite bundles supply no universal theorem.
Supported/Fits range over the complete command, with no refusal alternative. -/
namespace Tcc.Model.Ghost

/-- Actual per-file input, in command order; no token or Cabs concatenation. -/
structure UnitInput where
  filename : ByteArray
  contents : ByteArray
  config : Tcc.CSem.Raw.Config

/-- F3/F6 must connect command dispatch and resets to the independent raw
frontend for every unit. Linking/CSem program meaning remains F4/F5 work. -/
structure UnitTransport (expected : UnitInput) (record : UnitRecord) where
  raw : Tcc.CSem.Raw.Result
  input : Input
  source : OpenedFile
  retainedSource : record.source = some source
  filename : record.filename = expected.filename
  openedAs : source.openedAs = expected.filename
  contents : source.contents = expected.contents
  configuredName : expected.config.pre.mainName = expected.filename.toList
  includePaths : expected.config.pre.includePaths = record.includePaths.toList.map ByteArray.toList
  systemPaths : expected.config.pre.sysIncludePaths = record.systemPaths.toList.map ByteArray.toList
  checked : Tcc.CSem.Raw.checked expected.config expected.contents.toList = .ok raw
  compiled : record.compilerResult = some 0
  retainedInput : record.input = some input
  tokens : input.spelling.toList = raw.preprocessing.tokens.filterMap (fun t =>
    if t.token.kind == .pragmaBegin || t.token.kind == .pragmaEnd then none else some t.token)
  positioned : input.language.positioned = raw.input.positioned
  origins : input.language.origins = raw.input.origins
  pragmas : input.language.pragmas = raw.input.pragmas
  openings : OpeningTransport expected.config (unitView record) raw
  lexical : LexicalAuthorityTransport (unitView record) input
  constructed : record.tree = .ok raw.cabs

/-- Supplied by F3/F6 whole-command induction: every ordered unit survives
through relocation/handoff. F4/F5 separately owe cross-unit linkage meaning. -/
structure CommandConstruction (initial : TccSt) (argv : Array ByteArray)
    (expected : Array UnitInput) where
  state : TccSt
  handoff : RunResult
  compiled : (tcc_main (Tie.tie initial.fuel) argv).run
    { initial with ghostEnabled := true } = .ok (.ran handoff) state
  count : state.ghostUnits.size = expected.size
  units : ∀ i (h : i < expected.size),
    Nonempty (UnitTransport expected[i] (state.ghostUnits[i]'(by omega)))
  ordered : ∀ i (h : i < state.ghostUnits.size), state.ghostUnits[i].ordinal = i

def MultiTUConstruction_Statement (inputs : TccSt → Array ByteArray → Array UnitInput)
    (supported fits : TccSt → Array ByteArray → Prop) : Prop :=
  ∀ initial argv, supported initial argv → fits initial argv →
    Nonempty (CommandConstruction initial argv (inputs initial argv))

/-- F3/F6 must prove equality of all erased compiler state and outcome,
including diagnostics; neither arena equality nor successful-only tests prove it. -/
structure CommandErasure (initial : TccSt) (argv : Array ByteArray) : Prop where
  outcomes : ∀ observed after,
    (tcc_main (Tie.tie initial.fuel) argv).run { initial with ghostEnabled := true } = .ok observed after →
    (tcc_main (Tie.tie initial.fuel) argv).run (erase initial) = .ok observed (erase after)
  errors : ∀ error after,
    (tcc_main (Tie.tie initial.fuel) argv).run { initial with ghostEnabled := true } = .error error after →
    (tcc_main (Tie.tie initial.fuel) argv).run (erase initial) = .error error (erase after)

def MultiTUErasure_Statement : Prop := ∀ initial argv, CommandErasure initial argv
end Tcc.Model.Ghost

namespace Tcc.Model.Ghost
/-- Local snapshot law for function-action replay. Scanner coverage and
complete construction are additionally required by MultiTUConstruction. -/
def DeferredFunctionErasure_Statement : Prop :=
  ∀ R sym saved st,
    match (deferredFunctionSyntax R sym saved).run st with
    | .ok _ after => erase after = erase st
    | .error _ _ => False
end Tcc.Model.Ghost

namespace Tcc.Model.Ghost
/-- Reuse the same snapshot erasure law as association replay, with no
parser-success or saved-word-coverage premise. -/
theorem deferredFunctionErasure : DeferredFunctionErasure_Statement := by
  intro R sym saved st
  exact isolatedGhostSyntax_erasure _ _ _ _ _ _
end Tcc.Model.Ghost
