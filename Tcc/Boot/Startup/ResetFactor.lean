import Vsa.Sim.FactorSail
import Tcc.Boot.Reset

/-!
# Register initialization, factored (ship-your-ocaml's `Startup/Reset.lean`)

The program-independent part of ship-your-ocaml's
`OCaml/Vm/Boot/Startup/Reset.lean`: `initializeRegisters` factored by
`#factor_sail` (its deep bind chain kept opaque, the small metadata-dependent
prefix rewritten), so that register setup observes only the entry PC and the
`.tohost` metadata. `resetSeed`, `ElfReset` and `tohostMetadata` are
`Tcc.Boot`'s (the same definitions). The while_min-specific statements of the
source file are not copied.
-/

namespace Tcc.Boot.Startup
open Vsa.Machine LeanRV64DExecutable Sail.ConcurrencyInterfaceV1 Tcc.Boot

#factor_sail initializeRegisters as registersFactored

/-- Register initialization observes just the entry PC and HTIF metadata. -/
theorem registers_metadata {a b : ELF64File}
    (entry : a.file_header.e_entry = b.file_header.e_entry)
    (tohost : tohostMetadata a = tohostMetadata b) : initializeRegisters a = initializeRegisters b := by
  rw [registersFactored.eq]
  unfold registersFactored
  change (a.interpreted_sections.find? is_tohost).map (fun s => s.2.section_addr) =
    (b.interpreted_sections.find? is_tohost).map (fun s => s.2.section_addr) at tohost
  rw [entry, tohost]

/-- ELF parsing and memory construction are separate from register setup. -/
theorem setupElf_congr {a b : ELF64File}
    (entry : a.file_header.e_entry = b.file_header.e_entry)
    (tohost : tohostMetadata a = tohostMetadata b) : Vsa.setupElf a = Vsa.setupElf b := by
  unfold Vsa.setupElf
  rw [registers_metadata entry tohost, entry]

end Tcc.Boot.Startup
