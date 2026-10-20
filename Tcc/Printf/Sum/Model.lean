import Tcc.Printf.Derived
import Tcc.Printf.Sum.Witness
import Tcc.Prog.Sum.ClightFacts

/-!
# P under the printf read off its own image

`sumX` is `machX` for `sum.c`'s image: its printf steps exactly on the calls
the image (from a reset of an ELF `TccSumElf` pins) makes, with the image's
return value and output. For P's one call `printf("%d\n", 55)` the image's
call is `Witness.call0`, reached from reset by the compile phase
(`CompileCut_Statement`) and the replay (`printf_reach0`). So, given the
compile phase, `clightgen(P)` under `sumX` prints `"55\n"` and returns 0, and
every terminating run does (`prints_sumX`), with no printf premise: the printf
step's result is the machine's `a0 = 3`.
-/

namespace Tcc.Printf.Sum

open Vsa.Machine Tcc.Boot Tcc.Capture.Sum Tcc.Clight CC

/-- Reset configurations of the image `TccSumElf` pins. -/
def SumReset (c₀ : Config) : Prop := ∃ elf, TccSumElf elf ∧ ElfReset elf c₀

/-- **The external-call semantics read off `sum.c`'s image.** -/
def sumX : ExtEnv := machX Tcc.Prog.Sum.prog layout SumReset sym

/-- P's printf call as Clight converts it. -/
def callArgs0 : List EventVal :=
  [.EVptr_global Tcc.Prog.Sum.___stringlit_1 0, .EVint (Integers.Int.repr 55)]

theorem runPfProgramCalls_eq : runPfProgramCalls Tcc.Prog.Sum.prog 1000 = [callArgs0] := by
  decide +kernel

/-- The image makes P's call, given the compile phase. -/
theorem imgMakes0 (hcomp : CompileCut_Statement) (elf : ELF64File) (helf : TccSumElf elf)
    (c₀ : Config) (hreset : ElfReset elf c₀) (ge : Senv) :
    ImgMakes Tcc.Prog.Sum.prog layout SumReset sym ge callArgs0 := by
  obtain ⟨c, hs, hcut⟩ := hcomp elf helf c₀ hreset
  obtain ⟨c1, hs1, h1⟩ := Tcc.Prog.Sum.Machine.printf_reach0 c hcut
  obtain ⟨c2, hcall⟩ := call0 c1 h1
  refine ⟨Tcc.Prog.Sum.___stringlit_1, 0, fmt0, out0, Integers.Int.repr 3, rfl, by decide +kernel,
    by decide +kernel, ?_⟩
  exact .intro _ 0 _ out0 _ 0x86410b68 args0 c₀ c1 c2 (by decide +kernel) (by decide +kernel)
    ⟨elf, helf, hreset⟩ (Steps.trans hs hs1) hcall

/-- **P under `sumX`**, given the compile phase: P prints `"55\n"` and
returns 0, and every terminating run does. -/
theorem prints_sumX (hcomp : CompileCut_Statement) (elf : ELF64File) (helf : TccSumElf elf)
    (c₀ : Config) (hreset : ElfReset elf c₀) :
    Prints sumX Tcc.Prog.Sum.prog Tcc.Prog.Sum.expectedOut 0 :=
  runPfProgram_printsOn (machX_model _ _ _ _) (machX_extDeterm _ _ _ _)
    Tcc.Prog.Sum.runPfProgram_eq (by
      rw [runPfProgramCalls_eq]
      intro evs hevs
      rw [List.mem_singleton.mp hevs]
      exact imgMakes0 hcomp elf helf c₀ hreset _)

end Tcc.Printf.Sum
