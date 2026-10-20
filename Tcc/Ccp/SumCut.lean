import Tcc.Ccp.Sum.Seg.Join
import Tcc.Ccp.Sum.Table.All
import Tcc.Ccp.Sound
import Tcc.Capture.Sum.Cut

/-!
# `CompileCut_Statement` from the evaluator run (CompileCut pilot)

The compile phase of `tcc -run sum.c` as one kernel-checked evaluator run
(`Tcc.Ccp.Sum.run_all`, 266 generated segments), the generated proof that the
code table decodes as the evaluator runs it (`Tcc.Ccp.Sum.tableOK`), and the
generic soundness theorem (`Tcc.Ccp.run_sound`) give `CompileCut_Statement`
from one remaining premise, `ResetSim_Statement`: the machine's reset
configuration is related to the evaluator's reset state `st0`.

`ResetSim_Statement` is what the reset port supplies (ship-your-ocaml's
`ElfReset.ready`: `GoodState`, zero GPRs, the loader's memory, empty console,
idle mailbox) together with the pin correspondence: every byte `mem0` knows
is the ELF's (`TccSumElf`), the `elfAbsent` ones absent, and the code table's
bytes present. See results/p3/compile-cut-pilot.md.
-/

namespace Tcc.Ccp.Sum

open Tcc.Ccp Tcc.Boot Tcc.Capture.Sum Vsa.Machine

/-- The bytes the run reads before writing them that the ELF does not load. -/
def absentB (a : Nat) : Bool := inRanges Elf.elfAbsent a

/-- Open: the reset configuration is related to the evaluator's reset state. -/
def ResetSim_Statement : Prop :=
  ∀ elf, TccSumElf elf → ∀ c₀, ElfReset elf c₀ → Sim code absentB st0 c₀

/-- The evaluator's state at the cut is the captured one: PC, all GPRs, and
every byte of the cut's view known, present and equal. -/
theorem st_cut_pc : BitVec.ofNat 64 st266.pc = enterPc := by decide +kernel

theorem st_cut_regs : ∀ i, i < 31 → regs (i + 1) = BitVec.ofNat 64 (getR st266.regs (i + 1)) := by
  decide +kernel

theorem st_cut_view : ∀ ch ∈ viewChunks, viewOK absentB st266.mem ch = true := by
  decide +kernel

/-- **PHASES 3a from the reset premise**: tcc's compile phase reaches the
captured cut. -/
theorem compileCut (hreset : ResetSim_Statement) : CompileCut_Statement := by
  intro elf helf c₀ hr
  obtain ⟨cf, hs, hS⟩ := run_sound tableOK _ (hreset elf helf c₀ hr) run_all
  exact ⟨cf, hs, sim_cut hS st_cut_pc st_cut_regs st_cut_view⟩

end Tcc.Ccp.Sum
