import Tcc.Prog.Sum.Machine.Replay
import Tcc.Prog.Sum.ClightFacts

/-!
# `tcc -run sum.c` on Sail agrees with Certora's Lean Clight of `sum.c`

The end-to-end theorem for Phase 3's program P = `c/tests/p3/sum.c`, modulo
one open premise:

* machine side: from reset on the tcc image with P embedded (`TccSumElf`,
  `ElfReset`), tcc's compile phase reaches the captured cut
  (`CompileCut_Statement`, **open**), and from the cut the program phase halts
  printing `55\n` with exit code 0 (`Tcc.Prog.Sum.Machine.sum_halts`, proved by
  the generated replay of all 2,470 program-phase steps);
* Clight side: for every external-call semantics `X` with the printf premise
  `PrintfModel X` and `ExtDeterm X`, `clightgen(P)` terminates printing those
  bytes with return code 0, and every terminating run does
  (`Tcc.Prog.Sum.prints`).

The console is a `String` on the machine (`Vsa.Machine.output`, one character
per HTIF byte) and a byte list in Clight (`traceOut`); they are related by
`String.toUTF8` (the bytes here are ASCII). The machine's exit code is a `Nat`
(the HTIF exit word's payload), Clight's an `Integers.Int` (`BitVec 32`); they
are related by `BitVec.toNat`.
-/

namespace Tcc.EndToEnd

open Vsa.Machine Tcc.Boot Tcc.Capture.Sum Tcc.Clight CC

/-- What the machine and Clight agree on for one program: the machine halts
with console `out` and code `e`, Clight's P prints `out`'s bytes and returns
`r`, and the codes are the same number. -/
structure Agree (X : ExtEnv) (p : Program) (c₀ : Config) (out : String) (e : Nat)
    (r : Integers.Int) : Prop where
  machine : Halts c₀ out e
  clight : Prints X p out.toUTF8.toList r
  code : e = r.toNat

/-- **End to end for P**, given the compile-phase statement: from reset on
the image, `tcc -run sum.c` on Sail and Certora's Lean Clight of `sum.c` agree
(`"55\n"`, 0). -/
theorem tcc_run_sum (X : ExtEnv) (hP : PrintfModel X) (hD : ExtDeterm X)
    (hcomp : CompileCut_Statement) :
    ∀ elf, TccSumElf elf → ∀ c₀, ElfReset elf c₀ →
      Agree X Tcc.Prog.Sum.prog c₀ "55\n" 0 0 := by
  intro elf helf c₀ hreset
  refine ⟨sum_halts_from_reset hcomp Tcc.Prog.Sum.Machine.sum_halts elf helf c₀ hreset, ?_, rfl⟩
  rw [← Tcc.Prog.Sum.expectedOut_eq_utf8]
  exact Tcc.Prog.Sum.prints X hP hD

/-- Every way the machine halts from reset is Clight's: the console's bytes
are what P prints in Clight and the exit code is P's return code. -/
theorem tcc_run_sum_halts (X : ExtEnv) (hP : PrintfModel X) (hD : ExtDeterm X)
    (hcomp : CompileCut_Statement) :
    ∀ elf, TccSumElf elf → ∀ c₀, ElfReset elf c₀ → ∀ out e, Halts c₀ out e →
      Agree X Tcc.Prog.Sum.prog c₀ out e 0 := by
  intro elf helf c₀ hreset out e h
  have hA := tcc_run_sum X hP hD hcomp elf helf c₀ hreset
  obtain ⟨rfl, rfl⟩ := Halts.deterministic h hA.machine
  exact hA

end Tcc.EndToEnd
