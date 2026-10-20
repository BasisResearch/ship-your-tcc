/-
  Phase 3's program P (c/tests/p3/sum.c) at the Clight level: under every
  external-call semantics `X` that satisfies the printf premise and
  determinism, `clightgen(P)` terminates, prints "55\n" and returns 0, and
  every terminating run does the same.

  The whole run (about 150 Clight steps, one printf) is one kernel evaluation
  of `runPfProgram`; `runPfProgram_prints` (Tcc.Clight.Run) turns it into the
  statement about `StepWith X`.
-/
import Tcc.Clight.Run
import Tcc.Prog.Sum.Clight

namespace Tcc.Prog.Sum
open CC Tcc.Clight

/-- The bytes P prints. -/
def expectedOut : List UInt8 := [0x35, 0x35, 0x0a]

theorem expectedOut_eq_utf8 : expectedOut = "55\n".toUTF8.toList := by decide +kernel

/-- The run, evaluated by the kernel. -/
theorem runPfProgram_eq : runPfProgram prog 1000 = some (expectedOut, 0) := by
  decide +kernel

/-- **P at the Clight level.** For every `X` with `PrintfModel X` and
    `ExtDeterm X`: P terminates under `StepWith X` with a trace printing
    "55\n" and exit code 0, and every terminating run prints "55\n" and
    exits with 0. -/
theorem prints (X : ExtEnv) (hP : PrintfModel X) (hD : ExtDeterm X) :
    Prints X prog expectedOut 0 :=
  runPfProgram_prints hP hD runPfProgram_eq

/-- The same, unfolded. -/
theorem terminates (X : ExtEnv) (hP : PrintfModel X) :
    ∃ t, Terminates X prog t 0 ∧ traceOut prog t = some expectedOut :=
  runPfProgram_terminates hP runPfProgram_eq

theorem unique (X : ExtEnv) (hP : PrintfModel X) (hD : ExtDeterm X) (t : Trace)
    (r : Integers.Int) (h : Terminates X prog t r) :
    traceOut prog t = some expectedOut ∧ r = 0 :=
  (prints X hP hD).unique t r h

/-- Non-vacuity: the premises hold for `modelX`, so `prints` says something. -/
theorem prints_modelX : Prints modelX prog expectedOut 0 :=
  prints modelX modelX_printfModel modelX_extDeterm

end Tcc.Prog.Sum
