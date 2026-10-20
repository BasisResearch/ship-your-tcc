import Vsa.Machine
import Tcc.Boot.View

/-!
# The machine's reset on an ELF

ship-your-ocaml's `OCaml/Vm/Boot/Startup/Reset.lean` (`resetSeed`,
`ElfReset`, `setupElf_pc`), without its `#factor_sail` register-metadata
factoring: the state the emulator builds from an ELF before the first
instruction, exactly as `lean_emulator`'s `runElf64` builds it
(`initializeMemory`, then `Vsa.setupElf`).
-/

namespace Tcc.Boot

open Vsa.Machine LeanRV64DExecutable Sail.ConcurrencyInterfaceV1

/-- State supplied to Sail's setup routine by the ELF runner. -/
def resetSeed (elf : ELF64File) : MState :=
  ⟨.emptyWithCapacity, (), initializeMemory .B64 elf, (), 0, #[]⟩

/-- Successful platform setup at architectural step zero. -/
structure ElfReset (elf : ELF64File) (c : Config) : Prop where
  setup : (Vsa.setupElf elf).run (resetSeed elf) = .ok () c.σ
  tick : c.tick = 0
  steps : c.steps = 0

/-- The `.tohost` address the register setup reads from the ELF. -/
def tohostMetadata (elf : ELF64File) : Option Nat :=
  (elf.interpreted_sections.find? is_tohost).map (fun s => s.2.section_addr)

private theorem bind_success {α β : Type} (f : SailM α) (g : α → SailM β)
    {s t : MState} {v : β} (h : (f >>= g).run s = .ok v t) :
    ∃ x u, f.run s = .ok x u ∧ (g x).run u = .ok v t := by
  simp only [EStateM.run, bind, EStateM.bind] at h
  cases hf : f s with
  | error e u => simp only [hf] at h; cases h
  | ok x u => exact ⟨x, u, hf, by simpa only [EStateM.run, hf] using h⟩

theorem setupElf_pc {elf : ELF64File} {s t : MState}
    (h : (Vsa.setupElf elf).run s = .ok () t) :
    t.regs.get? .PC = some elf.file_header.e_entry.toBitVec := by
  unfold Vsa.setupElf at h
  obtain ⟨_, _, _, h⟩ := bind_success _ _ h
  obtain ⟨_, _, _, h⟩ := bind_success _ _ h
  obtain ⟨_, _, _, h⟩ := bind_success _ _ h
  obtain ⟨_, _, _, h⟩ := bind_success _ _ h
  simp only [EStateM.run, writeReg, PreSail.writeReg, modify, modifyGet] at h
  cases h
  simp

/-- Successful reset finishes at the ELF entry PC before any instruction. -/
theorem ElfReset.pc {elf : ELF64File} {c : Config} (h : ElfReset elf c) :
    c.σ.regs.get? .PC = some elf.file_header.e_entry.toBitVec := setupElf_pc h.setup

/-- What pins an ELF for a run from reset (no ELF bytes are materialised):
the two metadata the register setup reads (the entry PC and the `.tohost`
address) and the loaded memory on a set of bytes, given as a view `img`
(present with these values) and ranges `absent` (not loaded). A run from
reset that reads only bytes of `img` and `absent` before writing them is the
same run for every ELF so pinned (the model reads an absent byte as 0). -/
structure ElfPin (img : Nat → Option (BitVec 8)) (absent : List (Nat × Nat)) (entry tohost : Nat)
    (elf : ELF64File) : Prop where
  entry : elf.file_header.e_entry.toNat = entry
  tohost : tohostMetadata elf = some tohost
  image : PartialView (initializeMemory .B64 elf) img
  absent : ∀ k, inRanges absent k = true → (initializeMemory .B64 elf)[k]? = none

end Tcc.Boot
