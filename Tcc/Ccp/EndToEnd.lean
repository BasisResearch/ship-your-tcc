import Tcc.Ccp.SumCut
import Tcc.EndToEnd.Sum
import Tcc.Printf.Sum.Model

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

/-! ## With the printf read off the image

`Tcc.Printf.Sum.sumX` is the external-call semantics whose printf steps
exactly on the calls `sum.c`'s image makes, returning the image's `a0` and
printing what the image appended to `stdout` (`Tcc.Printf.machX`). P's one
call is the image's (`Tcc.Printf.Sum.call0`, from the replay), so the
theorem below has no premise at all. -/

/-- **End to end for P, no premise**: from reset, `tcc -run sum.c` on Sail
halts printing `55\n` with exit code 0, and Certora's Lean Clight of `sum.c`,
under the printf read off this image (`sumX`), prints exactly that and
returns 0 (and every terminating run does). -/
theorem tcc_run_sum_derived :
    ∀ elf, TccSumElf elf → ∀ c₀, ElfReset elf c₀ →
      Agree Tcc.Printf.Sum.sumX Tcc.Prog.Sum.prog c₀ "55\n" 0 0 := by
  intro elf helf c₀ hreset
  refine ⟨sum_halts_from_reset Tcc.Ccp.Sum.compileCut_proved Tcc.Prog.Sum.Machine.sum_halts
    elf helf c₀ hreset, ?_, rfl⟩
  rw [← Tcc.Prog.Sum.expectedOut_eq_utf8]
  exact Tcc.Printf.Sum.prints_sumX Tcc.Ccp.Sum.compileCut_proved elf helf c₀ hreset

/-- Every way the machine halts from reset is Clight's under `sumX`. -/
theorem tcc_run_sum_halts_derived :
    ∀ elf, TccSumElf elf → ∀ c₀, ElfReset elf c₀ → ∀ out e, Halts c₀ out e →
      Agree Tcc.Printf.Sum.sumX Tcc.Prog.Sum.prog c₀ out e 0 := by
  intro elf helf c₀ hreset out e h
  have hA := tcc_run_sum_derived elf helf c₀ hreset
  obtain ⟨rfl, rfl⟩ := Halts.deterministic h hA.machine
  exact hA

end Tcc.EndToEnd
