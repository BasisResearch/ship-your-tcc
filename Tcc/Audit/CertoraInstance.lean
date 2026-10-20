/-
  Audit of patch 0002 (patches/certora/0002-cclib-extsem-param.patch): the
  parametrised semantics at `X := certoraExtEnv` (Certora's axioms
  `externalFunctionsSem`, `inlineAssemblySem`) is Certora's original one.

  * `step_eq` / `externalCall_eq`: the patched `Step`/`externalCall` are the
    `certoraExtEnv` instances, by `rfl`.
  * `externalCallOrig_eq`: the pristine external-call definition (kept by the
    patch as `externalCallOrig`) equals the patched one, case by case.
  * `stepOrig_iff`: the pristine `Step` inductive (restated verbatim, renamed,
    in the generated Tcc/Audit/StepOrig.lean) relates exactly the same states
    as the patched `Step`.
  * `stepWith_determ_certora`: the parametrised determinism at Certora's
    axioms gives back the original statement.

  This is the only file whose `#print axioms` may list Certora's four axioms
  (scripts/axioms_certora.lean); everything else is checked against the
  standard three (scripts/axioms.lean).
-/
import Tcc.Audit.StepOrig

namespace Tcc.Audit
open CC

theorem step_eq : @Step = fun ge fe => StepWith certoraExtEnv ge fe := rfl

theorem externalCall_eq : externalCall = externalCallWith certoraExtEnv := rfl

theorem externalCallOrig_eq (ef : ExtFun) : externalCallOrig ef = externalCall ef := by
  cases ef <;> rfl

theorem stepOrig_iff {ge : CGenv}
    {fe : Function → List Val → Mem → Env → TempEnv → Mem → Prop} {s t s'} :
    StepOrig ge fe s t s' ↔ Step ge fe s t s' := by
  constructor
  · intro h
    cases h <;> (try rw [externalCallOrig_eq] at *) <;> (constructor <;> assumption)
  · intro h
    cases h <;> (try rw [← externalCall_eq, ← externalCallOrig_eq] at *) <;>
      (constructor <;> assumption)

theorem stepWith_determ_certora {ge : CGenv}
    {fe : Function → List Val → Mem → Env → TempEnv → Mem → Prop}
    (hfe : EntryDeterm fe) (hinj : (Genv.toSenv ge.genv_genv).SymbolsInjective)
    {s t1 s1 t2 s2} (h1 : Step ge fe s t1 s1) (h2 : Step ge fe s t2 s2) :
    MatchTraces (Genv.toSenv ge.genv_genv) t1 t2 ∧ (t1 = t2 → s1 = s2) :=
  stepWith_determ certoraExtDeterm hfe hinj h1 h2

end Tcc.Audit
