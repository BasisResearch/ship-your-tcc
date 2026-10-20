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

open Tcc.Ccp Tcc.Boot Tcc.Capture.Sum Vsa.Machine Vsa.Sim

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

/-- What the machine's reset gives (ship-your-ocaml's `ElfResetReady`, for an
ELF whose `.tohost` is this image's). -/
structure ResetFacts (elf : ELF64File) (c : Config) : Prop where
  good : GoodState c.σ
  memory : c.σ.mem = initializeMemory .B64 elf
  output : c.σ.sailOutput = #[]
  idle : c.σ.regs.get? .htif_payload_writes = some 0#4
  gprs : ∀ n, 1 ≤ n → n < 32 → gprGet c.σ n = some 0#64

/-- Open: the reset facts (port of ship-your-ocaml's `ElfReset.ready` chain,
OCaml/Vm/Boot/Startup/{SetupElf,InitModel,...}). -/
def ResetFacts_Statement : Prop :=
  ∀ elf, TccSumElf elf → ∀ c₀, ElfReset elf c₀ → ResetFacts elf c₀

/-- Open: every byte the evaluator's reset memory knows is the pinned ELF's
(present), or one of the absent ones (zero). A brute-force kernel `decide`
does not fit (two 64-page blocks exceeded 26 GB); see
results/p3/compile-cut-pilot.md for the planned builder proof. -/
def MemPin_Statement : Prop := MT.allKnown (pinB absentB Elf.elfImage) mem0 = true

/-- Open: every code-table entry's word is the pinned ELF's bytes at its PC. -/
def CodePin_Statement : Prop := CT.allW (codeB absentB Elf.elfImage) code.table = true

theorem uint64_toBitVec (x : UInt64) : x.toBitVec = BitVec.ofNat 64 x.toNat := by
  apply BitVec.eq_of_toNat_eq; simp

theorem resetSim_of (h1 : ResetFacts_Statement) (h2 : MemPin_Statement) (h3 : CodePin_Statement) :
    ResetSim_Statement := by
  intro elf helf c₀ hr
  have hf := h1 elf helf c₀ hr
  refine sim_reset (img := Elf.elfImage) hf.good ?_ hf.gprs rfl ?_ ?_ h2 h3 ?_ hf.idle
    (by rw [hr.tick]; decide)
  · rw [hr.pc, uint64_toBitVec, helf.entry]; rfl
  · rw [hf.memory]; exact helf.image
  · intro a ha; rw [hf.memory]; exact helf.absent a ha
  · simp [output, hf.output]

/-- **PHASES 3a from the reset premise**: tcc's compile phase reaches the
captured cut. -/
theorem compileCut (hreset : ResetSim_Statement) : CompileCut_Statement := by
  intro elf helf c₀ hr
  obtain ⟨cf, hs, hS⟩ := run_sound tableOK _ (hreset elf helf c₀ hr) run_all
  exact ⟨cf, hs, sim_cut hS st_cut_pc st_cut_regs st_cut_view⟩

/-- **PHASES 3a** from the three open premises (reset facts, the two pins). -/
theorem compileCut_of (h1 : ResetFacts_Statement) (h2 : MemPin_Statement) (h3 : CodePin_Statement) :
    CompileCut_Statement :=
  compileCut (resetSim_of h1 h2 h3)

end Tcc.Ccp.Sum
