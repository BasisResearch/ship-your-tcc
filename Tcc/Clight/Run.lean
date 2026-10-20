/-
  Running a printf-only Clight program, and what a run proves about the
  relational semantics `StepWith X` for every `X` satisfying the printf
  premise.

  `runPf p s fuel` iterates Certora's interpreter `doStep`; where `doStep`
  stops at `printf(...)` with its result discarded, it evaluates and converts
  the arguments, prints them with `printfOut`, and continues after the call.
  The state after such a call does not depend on the value printf returns
  (the result is discarded, memory is unchanged by `PrintfModel.shape`), so
  one run of `runPf` covers every printf result the world may choose.

  * `runPf_sound`: a run of `runPf` is a run of `StepWith X` printing the
    same bytes (needs `PrintfModel X`).
  * `runPf_unique`: every terminating `StepWith X` run from the same state
    prints those bytes and exits with that code (needs `PrintfModel X` and
    `ExtDeterm X`; through `doStepWith_complete`, patch 0002).
  * `runPfProgram_prints`: both, from the program's initial state, packaged
    as `Prints X p out r`. Per program, `runPfProgram p fuel = some (out, r)`
    is closed by `decide +kernel`.
-/
import Tcc.Clight.Model

namespace Tcc.Clight
open CC

/-- Clight's step relation for `p` under external-call semantics `X`, with
    parameters as temporaries (`function_entry2`: what `clightgen -normalize`
    output and Certora's interpreter `doStep` use). -/
abbrev PStep (X : ExtEnv) (p : Program) : State → Trace → State → Prop :=
  StepWith X p.globalenv (FunctionEntry2 p.globalenv)

/-- From `s`, a run with trace `t` reaches a final state with exit code `r`. -/
inductive RunsTo (X : ExtEnv) (p : Program) (s : State) (t : Trace) (r : Integers.Int) :
    Prop where
  | intro (sf : State) (run : Star (PStep X p) s t sf) (final : FinalState sf r)

/-- **`p` terminates under `X` with trace `t` and exit code `r`**: from its
    initial state (`Clight.initial_state`), a `StepWith X` run with trace `t`
    reaches a final state returning `r` from `main` (`Clight.final_state`). -/
inductive Terminates (X : ExtEnv) (p : Program) (t : Trace) (r : Integers.Int) : Prop where
  | intro (s0 : State) (init : InitialState p s0) (runs : RunsTo X p s0 t r)

/-- **What `p` does under `X`**: some terminating run prints `out` and exits
    with `r`, and every terminating run prints `out` and exits with `r`. -/
structure Prints (X : ExtEnv) (p : Program) (out : List UInt8) (r : Integers.Int) : Prop where
  terminates : ∃ t, Terminates X p t r ∧ traceOut p t = some out
  unique : ∀ t r', Terminates X p t r' → traceOut p t = some out ∧ r' = r

/-! ## Final states -/

def finalCode : State → Option Integers.Int
  | .Returnstate (.Vint r) .Kstop _ => some r
  | _ => none

theorem finalState_of_finalCode {s r} (h : finalCode s = some r) : FinalState s r := by
  unfold finalCode at h
  split at h
  · cases h; exact .intro _ _
  · cases h

theorem finalCode_of_finalState {s r} (h : FinalState s r) : finalCode s = some r := by
  cases h; rfl

theorem final_noStep {X ge fe s r t s'} (h : FinalState s r) :
    ¬ StepWith X ge fe s t s' := by
  cases h; intro hs; cases hs

/-- A silent step can only be matched by a silent step. -/
theorem matchTraces_E0 {ge t} (h : MatchTraces ge E0 t) : t = E0 := by
  cases h; rfl

/-! ## printf calls -/

/-- `printf(args)` with its result discarded, `args` evaluated and converted:
    the event arguments and the state after the call. -/
def printfCall (ge : CGenv) : State → Option (List EventVal × State)
  | .State f (.Sbuiltin none (.EF_external name _) tyargs al) k e le m =>
      if name = "printf" then
        (doEvalExprlist ge e le m al tyargs).bind fun vargs =>
          (vargs.mapM (toEventVal ge.genv_genv.toSenv)).map fun evs =>
            (evs, .State f .Sskip k e le m)
      else none
  | _ => none

/-- What `printfCall` found. -/
inductive IsPrintfCall (ge : CGenv) : State → List EventVal → State → Prop where
  | intro (f sg tyargs al k e le m vargs evs)
      (eval : doEvalExprlist ge e le m al tyargs = some vargs)
      (conv : vargs.mapM (toEventVal ge.genv_genv.toSenv) = some evs) :
      IsPrintfCall ge (.State f (.Sbuiltin none (.EF_external "printf" sg) tyargs al) k e le m)
        evs (.State f .Sskip k e le m)

theorem isPrintfCall_of_printfCall {ge s evs s'} (h : printfCall ge s = some (evs, s')) :
    IsPrintfCall ge s evs s' := by
  unfold printfCall at h
  split at h
  · rename_i f name sg tyargs al k e le m
    split at h
    · rename_i hname
      subst hname
      cases hv : doEvalExprlist ge e le m al tyargs with
      | none => simp [hv] at h
      | some vargs =>
        cases hc : vargs.mapM (toEventVal ge.genv_genv.toSenv) with
        | none => simp [hv, hc] at h
        | some evs' =>
          simp only [hv, hc, Option.bind_some, Option.map_some, Option.some.injEq,
            Prod.mk.injEq] at h
          obtain ⟨rfl, rfl⟩ := h
          exact .intro f sg tyargs al k e le m vargs evs' hv hc
    · cases h
  · cases h

/-- The steps out of a builtin statement (all `Step.builtin`). -/
inductive BuiltinStep (X : ExtEnv) (ge : CGenv) (f : Function) (optid : Option Ident)
    (ef : ExtFun) (tyargs : List Ty) (al : List Expr) (k : Cont) (e : Env) (le : TempEnv)
    (m : Mem) : Trace → State → Prop where
  | intro (vargs t vres m')
      (args : EvalExprlist ge e le m al tyargs vargs)
      (call : externalCallWith X ef ge.genv_genv.toSenv vargs m t vres m') :
      BuiltinStep X ge f optid ef tyargs al k e le m t
        (.State f .Sskip k e (setOpttemp optid vres le) m')

theorem builtinStep_of_step {X ge fe f optid ef tyargs al k e le m t s'}
    (h : StepWith X ge fe (.State f (.Sbuiltin optid ef tyargs al) k e le m) t s') :
    BuiltinStep X ge f optid ef tyargs al k e le m t s' := by
  cases h with
  | builtin _ _ _ _ _ _ _ _ _ vargs _ vres m' hel hec => exact .intro vargs _ vres m' hel hec
  | skip_or_continue_loop1 _ _ _ _ _ _ _ _ hx => rcases hx with hx | hx <;> cases hx
  | skip_break_switch _ _ _ _ _ _ hx => rcases hx with hx | hx <;> cases hx

/-! ## The runner -/

/-- Run `p` from `s` for at most `fuel` steps: interpreter steps must be
    silent, printf calls print. The bytes printed and the exit code. -/
def runPf (p : Program) : State → Nat → Option (List UInt8 × Integers.Int)
  | _, 0 => none
  | s, fuel + 1 =>
      match finalCode s with
      | some r => some ([], r)
      | none =>
        match doStep p.globalenv s with
        | some ([], s') => runPf p s' fuel
        | some (_ :: _, _) => none
        | none =>
          match printfCall p.globalenv s with
          | none => none
          | some (evs, s') =>
            (printfOut p evs).bind fun o => (runPf p s' fuel).map fun res => (o ++ res.1, res.2)

/-- The argument lists of the printf calls `runPf p s fuel` makes, in order. -/
def runPfCalls (p : Program) : State → Nat → List (List EventVal)
  | _, 0 => []
  | s, fuel + 1 =>
      match finalCode s with
      | some _ => []
      | none =>
        match doStep p.globalenv s with
        | some ([], s') => runPfCalls p s' fuel
        | some (_ :: _, _) => []
        | none =>
          match printfCall p.globalenv s with
          | none => []
          | some (evs, s') => evs :: runPfCalls p s' fuel

/-- `runPf_sound` for a printf that need only step on the calls the run
makes (`PrintfModelOn Q`, `Q` holding at each of `runPfCalls`). -/
theorem runPf_soundOn {Q : Senv → List EventVal → Prop} {X : ExtEnv} (hP : PrintfModelOn Q X)
    {p : Program} :
    ∀ fuel s out r, runPf p s fuel = some (out, r) →
      (∀ evs ∈ runPfCalls p s fuel, Q p.globalenv.genv_genv.toSenv evs) →
      ∃ t, RunsTo X p s t r ∧ traceOut p t = some out := by
  intro fuel
  induction fuel with
  | zero => intro s out r h; simp [runPf] at h
  | succ n ih =>
    intro s out r h hQ
    simp only [runPf] at h
    simp only [runPfCalls] at hQ
    cases hf : finalCode s with
    | some r0 =>
      simp only [hf, Option.some.injEq, Prod.mk.injEq] at h
      obtain ⟨rfl, rfl⟩ := h
      exact ⟨[], .intro s (.refl s) (finalState_of_finalCode hf), rfl⟩
    | none =>
      simp only [hf] at h hQ
      cases hd : doStep p.globalenv s with
      | some ts =>
        obtain ⟨t1, s1⟩ := ts
        cases t1 with
        | nil =>
          simp only [hd] at h hQ
          obtain ⟨t, ⟨sf, hrun, hfin⟩, hout⟩ := ih s1 out r h hQ
          exact ⟨t, .intro sf (.step _ _ _ _ _ _ (doStepWith_sound X _ _ _ _ hd) hrun rfl) hfin,
            hout⟩
        | cons => simp [hd] at h
      | none =>
        cases hp : printfCall p.globalenv s with
        | none => simp [hd, hp] at h
        | some es =>
          obtain ⟨evs, s1⟩ := es
          simp only [hd, hp] at h hQ
          cases ho : printfOut p evs with
          | none => simp [ho] at h
          | some o =>
            cases hr : runPf p s1 n with
            | none => simp [ho, hr] at h
            | some res =>
              obtain ⟨o', r'⟩ := res
              simp only [ho, hr, Option.bind_some, Option.map_some, Option.some.injEq,
                Prod.mk.injEq] at h
              obtain ⟨rfl, rfl⟩ := h
              obtain ⟨t, ⟨sf, hrun, hfin⟩, hout⟩ :=
                ih s1 o' r' hr fun e he => hQ e (List.mem_cons_of_mem _ he)
              cases isPrintfCall_of_printfCall hp with
              | intro f sg tyargs al k e le m vargs _ heval hconv =>
                obtain ⟨t1, vres, m', hX⟩ :=
                  hP.total sg _ vargs m evs hconv (hQ evs (List.mem_cons_self ..))
                cases hP.shape _ _ _ _ _ _ _ hX with
                | intro evs' n hconv' =>
                  rw [hconv] at hconv'
                  cases hconv'
                  have step : PStep X p
                      (.State f (.Sbuiltin none (.EF_external "printf" sg) tyargs al) k e le m)
                      [printfEvent evs n] (.State f .Sskip k e le m) :=
                    .builtin f none _ tyargs al k e le m vargs _ (.Vint n) m
                      (doEvalExprlist_sound _ _ _ _ _ _ _ heval) hX
                  refine ⟨printfEvent evs n :: t, .intro sf (.step _ _ _ _ _ _ step hrun rfl) hfin,
                    ?_⟩
                  rw [traceOut_printfEvent, ho, hout]
                  rfl

theorem runPf_sound {X : ExtEnv} (hP : PrintfModel X) {p : Program} :
    ∀ fuel s out r, runPf p s fuel = some (out, r) →
      ∃ t, RunsTo X p s t r ∧ traceOut p t = some out :=
  fun fuel s out r h => runPf_soundOn (hP.on fun _ _ => True) fuel s out r h fun _ _ => trivial

/-- Uniqueness needs only the shape of printf's steps and determinism. -/
theorem runPf_unique {X : ExtEnv} (hP : PrintfShape X) (hD : ExtDeterm X) {p : Program} :
    ∀ fuel s out r, runPf p s fuel = some (out, r) →
      ∀ t r', RunsTo X p s t r' → traceOut p t = some out ∧ r' = r := by
  intro fuel
  induction fuel with
  | zero => intro s out r h; simp [runPf] at h
  | succ n ih =>
    intro s out r h t r' hrt
    obtain ⟨sf, hrun, hfin⟩ := hrt
    simp only [runPf] at h
    cases hf : finalCode s with
    | some r0 =>
      simp only [hf, Option.some.injEq, Prod.mk.injEq] at h
      obtain ⟨rfl, rfl⟩ := h
      have hs := finalState_of_finalCode hf
      cases hrun with
      | refl => exact ⟨rfl, finalState_determ hfin hs⟩
      | step _ _ _ _ _ _ hstep => exact absurd hstep (final_noStep hs)
    | none =>
      simp only [hf] at h
      cases hd : doStep p.globalenv s with
      | some ts =>
        obtain ⟨t1, s1⟩ := ts
        cases t1 with
        | nil =>
          simp only [hd] at h
          cases hrun with
          | refl => simp [finalCode_of_finalState hfin] at hf
          | step _ t1 s2 t2 _ _ hstep hrest heq =>
            obtain ⟨hm, hs⟩ := doStepWith_complete hD p.globalenv
              (program_symbolsInjective p) hd hstep
            obtain rfl := matchTraces_E0 hm
            obtain rfl := hs rfl
            subst heq
            exact ih _ out r h _ r' (.intro sf hrest hfin)
        | cons => simp [hd] at h
      | none =>
        cases hp : printfCall p.globalenv s with
        | none => simp [hd, hp] at h
        | some es =>
          obtain ⟨evs, s1⟩ := es
          simp only [hd, hp] at h
          cases ho : printfOut p evs with
          | none => simp [ho] at h
          | some o =>
            cases hr : runPf p s1 n with
            | none => simp [ho, hr] at h
            | some res =>
              obtain ⟨o', r''⟩ := res
              simp only [ho, hr, Option.bind_some, Option.map_some, Option.some.injEq,
                Prod.mk.injEq] at h
              obtain ⟨rfl, rfl⟩ := h
              cases isPrintfCall_of_printfCall hp with
              | intro f sg tyargs al k e le m vargs _ heval hconv =>
                cases hrun with
                | refl => cases hfin
                | step _ t1 s2 t2 _ _ hstep hrest heq =>
                  cases builtinStep_of_step hstep with
                  | intro vargs2 _ vres m2 hel hX =>
                    have hv := evalExprlist_determ
                      (doEvalExprlist_sound _ _ _ _ _ _ _ heval) _ hel
                    subst hv
                    cases hP _ _ _ _ _ _ _ hX with
                    | intro evs' n hconv' =>
                      rw [hconv] at hconv'
                      cases hconv'
                      subst heq
                      obtain ⟨hout, hr'⟩ := ih _ o' r'' hr t2 r' (.intro sf hrest hfin)
                      refine ⟨?_, hr'⟩
                      show traceOut p (printfEvent evs n :: t2) = _
                      rw [traceOut_printfEvent, ho, hout]
                      rfl

/-! ## Whole programs -/

/-- `p`'s initial state (as `CC.runProgram` builds it). -/
def initPf (p : Program) : Option State :=
  match p.initMem with
  | none => none
  | some m0 =>
    match Genv.findSymbol p.globalenv.genv_genv p.prog_main with
    | none => none
    | some b =>
      match Genv.findFunctPtr p.globalenv.genv_genv b with
      | none => none
      | some f =>
        if typeOfFundef f = .Tfunction [] type_int32s cc_default
        then some (.Callstate f [] .Kstop m0)
        else none

theorem initPf_init {p : Program} {s0 : State} (h : initPf p = some s0) : InitialState p s0 := by
  unfold initPf at h
  split at h
  · cases h
  · rename_i m0 hm
    split at h
    · cases h
    · rename_i b hb
      split at h
      · cases h
      · rename_i f hfp
        split at h
        · rename_i hty
          cases h
          exact .intro b f m0 hm hb hfp hty
        · cases h

/-- `runPf` from `p`'s initial state. -/
def runPfProgram (p : Program) (fuel : Nat) : Option (List UInt8 × Integers.Int) :=
  (initPf p).bind fun s0 => runPf p s0 fuel

/-- The printf calls of that run. -/
def runPfProgramCalls (p : Program) (fuel : Nat) : List (List EventVal) :=
  match initPf p with
  | some s0 => runPfCalls p s0 fuel
  | none => []

theorem runPfProgram_init_calls {p : Program} {fuel res} (h : runPfProgram p fuel = some res) :
    ∃ s0, InitialState p s0 ∧ runPf p s0 fuel = some res ∧
      runPfCalls p s0 fuel = runPfProgramCalls p fuel := by
  unfold runPfProgram at h
  unfold runPfProgramCalls
  cases hi : initPf p with
  | none => simp [hi] at h
  | some s0 =>
    simp only [hi, Option.bind_some] at h
    exact ⟨s0, initPf_init hi, h, rfl⟩

theorem runPfProgram_init {p : Program} {fuel res} (h : runPfProgram p fuel = some res) :
    ∃ s0, InitialState p s0 ∧ runPf p s0 fuel = some res :=
  let ⟨s0, hi, hr, _⟩ := runPfProgram_init_calls h
  ⟨s0, hi, hr⟩

/-- A run of `runPfProgram` is a terminating run under every `X` whose printf
steps on the calls the run makes. -/
theorem runPfProgram_terminatesOn {Q : Senv → List EventVal → Prop} {X : ExtEnv}
    (hP : PrintfModelOn Q X) {p : Program} {fuel : Nat} {out : List UInt8} {r : Integers.Int}
    (h : runPfProgram p fuel = some (out, r))
    (hQ : ∀ evs ∈ runPfProgramCalls p fuel, Q p.globalenv.genv_genv.toSenv evs) :
    ∃ t, Terminates X p t r ∧ traceOut p t = some out := by
  obtain ⟨s0, hinit, hrun, hcalls⟩ := runPfProgram_init_calls h
  obtain ⟨t, hrt, hout⟩ := runPf_soundOn hP fuel s0 out r hrun (hcalls ▸ hQ)
  exact ⟨t, .intro s0 hinit hrt, hout⟩

/-- A run of `runPfProgram` is a terminating run under every `X` with the
    printf premise. -/
theorem runPfProgram_terminates {X : ExtEnv} (hP : PrintfModel X)
    {p : Program} {fuel : Nat} {out : List UInt8} {r : Integers.Int}
    (h : runPfProgram p fuel = some (out, r)) :
    ∃ t, Terminates X p t r ∧ traceOut p t = some out :=
  runPfProgram_terminatesOn (hP.on fun _ _ => True) h fun _ _ => trivial

/-- Under printf premise and determinism, every terminating run prints what
    `runPfProgram` printed and exits with its code. -/
theorem runPfProgram_unique {X : ExtEnv} (hP : PrintfShape X) (hD : ExtDeterm X)
    {p : Program} {fuel : Nat} {out : List UInt8} {r : Integers.Int}
    (h : runPfProgram p fuel = some (out, r)) {t : Trace} {r' : Integers.Int}
    (ht : Terminates X p t r') : traceOut p t = some out ∧ r' = r := by
  obtain ⟨s0, hinit, hrun⟩ := runPfProgram_init h
  obtain ⟨s0', hinit', hrt⟩ := ht
  have := initialState_determ hinit hinit'
  subst this
  exact runPf_unique hP hD fuel s0 out r hrun t r' hrt

/-- **A run of `runPfProgram` decides what `p` does under every `X`** that
    satisfies the printf premise and determinism. -/
theorem runPfProgram_prints {X : ExtEnv} (hP : PrintfModel X) (hD : ExtDeterm X)
    {p : Program} {fuel : Nat} {out : List UInt8} {r : Integers.Int}
    (h : runPfProgram p fuel = some (out, r)) : Prints X p out r :=
  ⟨runPfProgram_terminates hP h, fun _ _ ht => runPfProgram_unique hP.shape hD h ht⟩

/-- The same for a printf that steps only on the calls the run makes. -/
theorem runPfProgram_printsOn {Q : Senv → List EventVal → Prop} {X : ExtEnv}
    (hP : PrintfModelOn Q X) (hD : ExtDeterm X)
    {p : Program} {fuel : Nat} {out : List UInt8} {r : Integers.Int}
    (h : runPfProgram p fuel = some (out, r))
    (hQ : ∀ evs ∈ runPfProgramCalls p fuel, Q p.globalenv.genv_genv.toSenv evs) :
    Prints X p out r :=
  ⟨runPfProgram_terminatesOn hP h hQ, fun _ _ ht => runPfProgram_unique hP.shape hD h ht⟩

end Tcc.Clight
