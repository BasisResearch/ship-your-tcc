import LeanRiscv

/-!
# Post-`setupElf` values of the GoodState control registers

The concrete values every hot-path control register holds after
`Vsa.setupElf` (`sail_model_init` → `initializeRegisters` → `init_model ""`
→ `cycle_count` → `writeReg PC e_entry`) — the values `GoodState` pins for
the whole run (`PLAN-InterpSim.md` §Layer 0 item 1).

Provenance: read off the init code (`experiments/M1-init-footprint.md`,
with file:line for every write) and cross-checked by evaluation
(`experiments/M1_init_values_eval.lean`). The obligation that `setupElf`
actually produces these remains the one-time `init_good` lemma. A theorem
quantified over arbitrary ELFs must assume that the ELF's `.tohost` section is
at `tohostAddr`; `initializeRegisters` copies that ELF-dependent address into
the two HTIF registers pinned by `GoodState`.

Note the CSR types (`Misa`, `Seccfg`, `Minterrupts`, …) are `abbrev`s of
`BitVec`, so plain literals are the canonical form; field accessors on them
close by `decide`.

`misa = 0x800000000034112f`: MXL=RV64 (bits 63-62 = 10) and extensions
A,B,C,D,F,I(bit 8),M,S,U,V — this is the post-`reset_misa` value, NOT the
`sail_model_init` seed used in experiment E1i (which predates `reset()`).
`mstatus = 0xa00000000`: SXL=UXL=RV64 (0b10 at 35-34, 33-32), MIE=0,
MPRV=0. `pmpcfg_n`/`pmpaddr_n`: all 64 entries zero (A=OFF, L=0 — no PMP
entry matches, so M-mode accesses take the default-allow path).
-/

namespace Vsa.Sim

open LeanRV64DExecutable

/-- misa after init: RV64 + A,B,C,D,F,I,M,S,U,V. -/
def initMisa : BitVec 64 := 0x800000000034112f#64

/-- mstatus after init: SXL=UXL=RV64, everything else (incl. MIE, MPRV) 0. -/
def initMstatus : BitVec 64 := 0x0000000a00000000#64

/-- mseccfg after init (MML=MMWP=RLB=0, no landing pads enabled). -/
def initMseccfg : BitVec 64 := 0#64

/-- mie after init: all interrupts disabled — the crux of `dispatch_none`. -/
def initMie : BitVec 64 := 0#64

/-- mideleg/medeleg/mip/satp/mtvec after init. -/
def initZero64 : BitVec 64 := 0#64

/-- pmpcfg_n after init: every entry OFF and unlocked. -/
def initPmpcfg : Vector Pmpcfg_ent 64 := Vector.replicate 64 (0#8)

/-- pmpaddr_n after init. -/
def initPmpaddr : Vector (BitVec 64) 64 := Vector.replicate 64 (0#64)

/-- `.tohost` HTIF mailbox address of ship-your-tcc's image
`c/tcc-riscv-htif.elf` (symbol `tohost`; also `htif_tohost` post-init). The
image's runtime is the same bytes for every embedded program, so this does not
depend on the program (`scripts/gen_tcc_cut.py` checks it against the ELF's
symbol table; `Tcc.Capture.Sum.tohostAddr_eq`). ship-your-lua's value was the Lua ELF's `0x8005c6c0`,
ship-your-interpreter's the WHILE ELF's `0x8001ad00`. -/
def tohostAddr : Nat := 0x8007cec0

open MemoryRegionType AtomicSupport Reservability misaligned_exception in
/-- `pma_regions` after init (`sail_model_init`,
`LeanRV64DExecutable.lean:252-312`, verbatim with hex literals): the SIG
region at 0x1000, the CLINT/IO region at 0x2000000, and executable main
memory `[0x80000000, 0x100000000)`. -/
def initPmaRegions : List PMA_Region :=
  [{ base := 0x1000#64
     size := 0x1000#64
     attributes := { mem_type := IOMemory
                     cacheable := true
                     coherent := false
                     executable := false
                     readable := true
                     writable := false
                     read_idempotent := true
                     write_idempotent := true
                     misaligned_exceptions := { load_store := none
                                                vector := none
                                                amo := AccessFault }
                     atomic_support := AMONone
                     reservability := RsrvNone
                     supports_cbo_zero := false
                     supports_pte_read := false
                     supports_pte_write := false
                     misaligned_atomicity_granule_size_exp := 0
                     vector_misaligned_atomicity_granule_size_exp := 0 }
     include_in_device_tree := false },
   { base := 0x2000000#64
     size := 0x10000000#64
     attributes := { mem_type := IOMemory
                     cacheable := false
                     coherent := true
                     executable := false
                     readable := true
                     writable := true
                     read_idempotent := false
                     write_idempotent := false
                     misaligned_exceptions := { load_store := none
                                                vector := none
                                                amo := AccessFault }
                     atomic_support := AMONone
                     reservability := RsrvNone
                     supports_cbo_zero := false
                     supports_pte_read := false
                     supports_pte_write := false
                     misaligned_atomicity_granule_size_exp := 0
                     vector_misaligned_atomicity_granule_size_exp := 0 }
     include_in_device_tree := false },
   { base := 0x80000000#64
     size := 0x80000000#64
     attributes := { mem_type := MainMemory
                     cacheable := true
                     coherent := true
                     executable := true
                     readable := true
                     writable := true
                     read_idempotent := true
                     write_idempotent := true
                     misaligned_exceptions := { load_store := none
                                                vector := none
                                                amo := AccessFault }
                     atomic_support := AMOCASQ
                     reservability := RsrvEventual
                     supports_cbo_zero := true
                     supports_pte_read := true
                     supports_pte_write := true
                     misaligned_atomicity_granule_size_exp := 4
                     vector_misaligned_atomicity_granule_size_exp := 4 }
     include_in_device_tree := true }]

/-! ## Code and store windows (ship-your-tcc generalisation)

ship-your-interpreter's layer placed every fetched instruction below the HTIF
mailbox (`pc + 4 ≤ tohostAddr`) and every store above it (`tohostAddr + 16 ≤
a`). `tcc -run` executes from an arena above `.tohost` and stores below it, so
the layer takes these two off-window facts instead; the original bounds are
the special cases `fetchOK_of_le` and `storeWin_of_le`. -/

/-- A fetch address: four bytes of RAM below `2^32`, off the HTIF mailbox. -/
def FetchOK (a : Nat) : Prop :=
  a + 4 ≤ 0x100000000 ∧ (a + 4 ≤ tohostAddr ∨ tohostAddr + 16 ≤ a)

instance (a : Nat) : Decidable (FetchOK a) := inferInstanceAs (Decidable (_ ∧ _))

theorem fetchOK_of_le {a : Nat} (h : a + 4 ≤ tohostAddr) : FetchOK a :=
  ⟨by unfold tohostAddr at h; omega, Or.inl h⟩

/-- A store address (any width up to 8) off the HTIF mailbox. -/
def StoreWin (a : Nat) : Prop := a + 8 ≤ tohostAddr ∨ tohostAddr + 16 ≤ a

instance (a : Nat) : Decidable (StoreWin a) := inferInstanceAs (Decidable (_ ∨ _))

theorem storeWin_of_le {a : Nat} (h : tohostAddr + 16 ≤ a) : StoreWin a := Or.inr h

end Vsa.Sim
