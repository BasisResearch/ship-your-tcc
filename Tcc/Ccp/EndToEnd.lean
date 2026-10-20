import Tcc.Ccp.SumCut
import Tcc.EndToEnd.Sum

/-!
# `tcc -run sum.c` agrees with Certora's Lean Clight, no machine premise left

`Tcc.EndToEnd.tcc_run_sum` with its compile-phase premise discharged by
`Tcc.Ccp.Sum.compileCut_proved` (the kernel-checked evaluator run of tcc's
compile phase, the generic evaluator soundness, the generated table and pin
proofs, and the ported reset chain). The only premises left are the Clight
side's `PrintfModel X` and `ExtDeterm X` (external calls are a parameter of
Certora's semantics; `premises_satisfiable` shows they can be met).

In library `TccCompileCut` (not a default target: it builds the evaluator run,
about 2.3 CPU-hours; `scripts/check.sh --compile-cut`).
-/

namespace Tcc.EndToEnd

open Vsa.Machine Tcc.Boot Tcc.Capture.Sum Tcc.Clight CC

/-- **End to end for P**: from reset on the tcc image with `sum.c` embedded,
`tcc -run sum.c` on Sail halts printing `55\n` with exit code 0, exactly what
Certora's Lean Clight of `sum.c` prints and returns. -/
theorem tcc_run_sum_closed (X : ExtEnv) (hP : PrintfModel X) (hD : ExtDeterm X) :
    ∀ elf, TccSumElf elf → ∀ c₀, ElfReset elf c₀ →
      Agree X Tcc.Prog.Sum.prog c₀ "55\n" 0 0 :=
  tcc_run_sum X hP hD Tcc.Ccp.Sum.compileCut_proved

/-- Every way the machine halts from reset is Clight's. -/
theorem tcc_run_sum_halts_closed (X : ExtEnv) (hP : PrintfModel X) (hD : ExtDeterm X) :
    ∀ elf, TccSumElf elf → ∀ c₀, ElfReset elf c₀ → ∀ out e, Halts c₀ out e →
      Agree X Tcc.Prog.Sum.prog c₀ out e 0 :=
  tcc_run_sum_halts X hP hD Tcc.Ccp.Sum.compileCut_proved

end Tcc.EndToEnd
