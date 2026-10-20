/-
  printf at the Clight level: the event a printf call emits, the bytes a trace
  prints (`traceOut`), and the premise `PrintfModel X` on an external-call
  semantics `X : CC.ExtEnv` (patch 0002 parametrises Certora's `Step` by `X`).

  A pointer argument becomes an event value through the global environment's
  `invert_symbol`, as CompCert's interpreter does (`Cexec.eventval_of_val`
  without the public-symbol check). The format string of `P` is
  `__stringlit_1`, which is not a public identifier: a premise phrased with
  `EventValMatch` (which requires `public_symbol`) would make every printf step
  impossible and the theorems about `P` vacuous. `modelX_printfModel` and
  `modelX_extDeterm` show the premises here are satisfiable.
-/
import Tcc.Clight.Printf

namespace Tcc.Clight
open CC

/-! ## Events -/

/-- The event value of a machine value; pointers through `invert_symbol`. -/
def toEventVal (ge : Senv) : Val → Option EventVal
  | .Vint n => some (.EVint n)
  | .Vlong n => some (.EVlong n)
  | .Vfloat f => some (.EVfloat f)
  | .Vsingle f => some (.EVsingle f)
  | .Vptr b ofs => (ge.invert_symbol b).map fun id => .EVptr_global id ofs
  | .Vundef => none

/-- The event of a printf call with converted arguments `evargs` returning `n`. -/
def printfEvent (evargs : List EventVal) (n : Integers.Int) : Event :=
  .Event_syscall "printf" evargs (.EVint n)

/-! ## Output -/

/-- A C string from a list of bytes: up to the first NUL, which must exist. -/
def cString : List UInt8 → Option (List UInt8)
  | [] => none
  | c :: cs => if c = 0 then some [] else (c :: ·) <$> cString cs

/-- The bytes of a global's initialiser, if it is made of bytes and zeros. -/
def initBytes : List InitData → Option (List UInt8)
  | [] => some []
  | .Init_int8 n :: l => ((n.toNat % 256).toUInt8 :: ·) <$> initBytes l
  | .Init_space k :: l => (List.replicate k.toNat 0 ++ ·) <$> initBytes l
  | _ :: _ => none

/-- The string at offset `ofs` of the read-only, non-volatile global `id` of
    `p`, read from its initialiser. -/
def globalString (p : Program) (id : Ident) (ofs : Integers.Ptrofs) : Option (List UInt8) :=
  let ge := p.globalenv.genv_genv
  match Genv.findSymbol ge id with
  | none => none
  | some b =>
      match Genv.findVarInfo ge b with
      | none => none
      | some v =>
          if v.gvar_readonly && !v.gvar_volatile then
            (initBytes v.gvar_init).bind fun bs => cString (bs.drop ofs.toNat)
          else none

/-- What `printf` prints given its converted arguments. -/
def printfOut (p : Program) : List EventVal → Option (List UInt8)
  | .EVptr_global id ofs :: args => (globalString p id ofs).bind fun fmt =>
      format (globalString p) fmt args
  | _ => none

/-- The bytes an event prints: printf events only. -/
def eventOut (p : Program) : Event → Option (List UInt8)
  | .Event_syscall name args _ => if name = "printf" then printfOut p args else none
  | _ => none

/-- The bytes a trace prints, or `none` if it holds anything but printf calls
    in the model. -/
def traceOut (p : Program) : Trace → Option (List UInt8)
  | [] => some []
  | ev :: t => (eventOut p ev).bind fun a => (traceOut p t).map (a ++ ·)

theorem traceOut_printfEvent (p : Program) (evargs : List EventVal) (n : Integers.Int)
    (t : Trace) :
    traceOut p (printfEvent evargs n :: t) =
      (printfOut p evargs).bind fun a => (traceOut p t).map (a ++ ·) := by
  simp [traceOut, eventOut, printfEvent]

/-! ## The premise on `X` -/

/-- What one printf step of an external-call semantics does: its arguments
    convert to event values, it emits exactly the printf event, returns the
    event's result and leaves memory unchanged. -/
inductive PrintfStep (ge : Senv) (vargs : List Val) (m : Mem) :
    Trace → Val → Mem → Prop where
  | intro (evargs : List EventVal) (n : Integers.Int)
      (args : vargs.mapM (toEventVal ge) = some evargs) :
      PrintfStep ge vargs m [printfEvent evargs n] (.Vint n) m

/-- **The printf premise.** `X`'s printf (any signature, any environment)
    steps only as `PrintfStep` says (`shape`), and steps for every argument
    list that converts (`total`). Supplied by the machine-level printf spec
    (BRIEF Phase 3b, PHASES `PrintfSpec_Statement`); satisfiable:
    `modelX_printfModel`. -/
structure PrintfModel (X : ExtEnv) : Prop where
  shape : ∀ sg ge vargs m t vres m', X.external "printf" sg ge vargs m t vres m' →
    PrintfStep ge vargs m t vres m'
  total : ∀ sg ge vargs m evargs, vargs.mapM (toEventVal ge) = some evargs →
    ∃ t vres m', X.external "printf" sg ge vargs m t vres m'

/-! ## Non-vacuity -/

/-- A printf that may return any `int` (the result is the world's choice, so
    determinism holds only up to `MatchTraces`). -/
inductive ModelPrintf : ExtcallSem where
  | intro (ge vargs m evargs n) :
      vargs.mapM (toEventVal ge) = some evargs →
      ModelPrintf ge vargs m [printfEvent evargs n] (.Vint n) m

/-- The empty relation (an external that never returns). -/
def noSem : ExtcallSem := fun _ _ _ _ _ _ => False

/-- printf as `ModelPrintf`; every other external and all inline assembly
    never return. -/
def modelX : ExtEnv where
  external name _ := if name = "printf" then ModelPrintf else noSem
  inlineAsm _ _ := noSem

theorem modelX_printfModel : PrintfModel modelX where
  shape := by
    intro sg ge vargs m t vres m' h
    simp only [modelX, ↓reduceIte] at h
    cases h with
    | intro _ _ _ evargs n hc => exact .intro evargs n hc
  total := by
    intro sg ge vargs m evargs hc
    exact ⟨_, _, _, by simp only [modelX, ↓reduceIte]; exact .intro ge vargs m evargs 0 hc⟩

theorem noSem_determ (ge : Senv) : ExtcallDeterm noSem ge :=
  ⟨fun _ _ _ _ _ _ _ _ h => h.elim, fun _ _ _ _ _ h => h.elim⟩

theorem modelPrintf_determ (ge : Senv) : ExtcallDeterm ModelPrintf ge := by
  refine ⟨?_, ?_⟩
  · intro vargs m t1 vres1 m1 t2 vres2 m2 h1 h2
    cases h1 with
    | intro _ _ _ ev1 n1 hc1 =>
      cases h2 with
      | intro _ _ _ ev2 n2 hc2 =>
        rw [hc1] at hc2
        cases hc2
        refine ⟨.syscall _ _ _ _ trivial trivial rfl, fun ht => ?_⟩
        simp only [printfEvent, List.cons.injEq, Event.Event_syscall.injEq,
          EventVal.EVint.injEq, and_true, true_and] at ht
        subst ht
        exact ⟨rfl, rfl⟩
  · intro vargs m t vres m' h
    cases h
    simp

theorem modelX_extDeterm : ExtDeterm modelX where
  external name sg ge := by
    by_cases h : name = "printf"
    · subst h; simpa [modelX] using modelPrintf_determ ge
    · simpa [modelX, h] using noSem_determ ge
  inlineAsm _ _ ge := noSem_determ ge

/-- The two premises of the per-program theorems hold together. -/
theorem premises_satisfiable : ∃ X, PrintfModel X ∧ ExtDeterm X :=
  ⟨modelX, modelX_printfModel, modelX_extDeterm⟩

end Tcc.Clight
