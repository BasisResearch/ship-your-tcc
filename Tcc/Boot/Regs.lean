import Vsa.Sim.GoodState
import Vsa.Sim.BlockPilot
import Std.Data.ExtDHashMap.Lemmas

/-!
# Registers at a captured cut

The register side of a captured cut (after ship-your-interpreter's
`Vsa/Sim/Boot/{Config,Entry}.lean` and `Vsa/Sim/OutputAliasPhysical.lean`):

* `EntryRegs σ g pc`: what a cut pins of the registers: `GoodState` (the
  control registers the step lemmas consult), the PC, an idle HTIF mailbox,
  and every GPR `x1..x31` at the captured value `g i`. syi's version
  hard-codes its interpreter's entry PC `0x800043ec`; here the PC is a
  parameter.
* `cutState m g pc`: a concrete machine state with memory `m` satisfying
  `EntryRegs`. Its control registers are the post-`setupElf` values of
  `Vsa.Sim.InitValues` (with this image's `tohostAddr`), as in syi's
  `physicalAssignments`. It is the representative a witness exhibits; the
  real machine's counters (`mtime`, `minstret`, …) differ, and `EntryRegs`
  does not pin them.
-/

namespace Tcc.Boot

open LeanRV64DExecutable Sail ConcurrencyInterfaceV1 Vsa.Machine Vsa.Sim

/-- Control registers of a cut state: syi's `physicalAssignments` (the
values after `Vsa.setupElf`), with the PC a parameter. -/
def csrAssignments (pc : BitVec 64) : List ((r : Register) × RegisterType r) :=
  [⟨Register.cur_privilege, Privilege.Machine⟩,
   ⟨Register.misa, initMisa⟩,
   ⟨Register.mstatus, initMstatus⟩,
   ⟨Register.mie, 0#64⟩,
   ⟨Register.mseccfg, 0#64⟩,
   ⟨Register.satp, 0#64⟩,
   ⟨Register.mtvec, 0#64⟩,
   ⟨Register.mideleg, 0#64⟩,
   ⟨Register.medeleg, 0#64⟩,
   ⟨Register.hart_state, HartState.HART_ACTIVE ()⟩,
   ⟨Register.htif_done, false⟩,
   ⟨Register.htif_tohost, 0#64⟩,
   ⟨Register.htif_tohost_base, some (BitVec.ofNat 64 tohostAddr)⟩,
   ⟨Register.elp, 0#1⟩,
   ⟨Register.pmpcfg_n, initPmpcfg⟩,
   ⟨Register.pmpaddr_n, initPmpaddr⟩,
   ⟨Register.pma_regions, initPmaRegions⟩,
   ⟨Register.menvcfg, 0#64⟩,
   ⟨Register.mcountinhibit, 0#32⟩,
   ⟨Register.mcyclecfg, 0#64⟩,
   ⟨Register.minstretcfg, 0#64⟩,
   ⟨Register.mip, 0#64⟩,
   ⟨Register.sig_meip, 0#1⟩,
   ⟨Register.sig_seip, 0#1⟩,
   ⟨Register.mtime, 0#64⟩,
   ⟨Register.mtimecmp, 0#64⟩,
   ⟨Register.minstret, 0#64⟩,
   ⟨Register.minstret_increment, false⟩,
   ⟨Register.mcycle, 0#64⟩,
   ⟨Register.nextPC, pc⟩,
   ⟨Register.PC, pc⟩,
   ⟨Register.htif_payload_writes, 0#4⟩]

/-- The GPRs `x1..x31` at `g 1 .. g 31`. -/
def gprAssignments (g : Nat → BitVec 64) : List ((r : Register) × RegisterType r) :=
  (List.range 31).map fun i => ⟨gprReg (i + 1), gprRT (i + 1) (g (i + 1))⟩

def cutAssignments (g : Nat → BitVec 64) (pc : BitVec 64) :
    List ((r : Register) × RegisterType r) :=
  csrAssignments pc ++ gprAssignments g

/-- The keys do not depend on the values. -/
def cutKeys : List Register :=
  (csrAssignments 0).map Sigma.fst ++ (List.range 31).map fun i => gprReg (i + 1)

theorem cutAssignments_keys (g : Nat → BitVec 64) (pc : BitVec 64) :
    (cutAssignments g pc).map Sigma.fst = cutKeys := by
  simp [cutAssignments, gprAssignments, cutKeys, csrAssignments, Function.comp_def]

private theorem cutKeys_distinct : cutKeys.Pairwise (fun a b => (a == b) = false) := by
  decide

private theorem cutAssignments_distinct (g : Nat → BitVec 64) (pc : BitVec 64) :
    (cutAssignments g pc).Pairwise (fun a b => (a.1 == b.1) = false) := by
  have h := cutKeys_distinct
  rw [← cutAssignments_keys g pc, List.pairwise_map] at h
  exact h

def cutRegs (g : Nat → BitVec 64) (pc : BitVec 64) : Std.ExtDHashMap Register RegisterType :=
  Std.ExtDHashMap.ofList (cutAssignments g pc)

theorem cutRegs_get {g : Nat → BitVec 64} {pc : BitVec 64} {r : Register} {v : RegisterType r}
    (h : ⟨r, v⟩ ∈ cutAssignments g pc) : (cutRegs g pc).get? r = some v := by
  simpa [cutRegs] using Std.ExtDHashMap.get?_ofList_of_mem
    (k := r) (k' := r) (by simp) (cutAssignments_distinct g pc) h

/-- A machine state at a cut: registers `cutRegs g pc`, memory `m`, empty
console. -/
def cutState (m : Std.ExtHashMap Nat (BitVec 8)) (g : Nat → BitVec 64) (pc : BitVec 64) :
    MState :=
  ⟨cutRegs g pc, (), m, (), 0, #[]⟩

@[simp] theorem cutState_mem (m : Std.ExtHashMap Nat (BitVec 8)) (g : Nat → BitVec 64)
    (pc : BitVec 64) : (cutState m g pc).mem = m := rfl

theorem cutState_output (m : Std.ExtHashMap Nat (BitVec 8)) (g : Nat → BitVec 64)
    (pc : BitVec 64) : output (cutState m g pc) = "" := rfl

local macro "csr" : tactic =>
  `(tactic| (apply cutRegs_get; simp [cutAssignments, csrAssignments]))

theorem cutState_good (m : Std.ExtHashMap Nat (BitVec 8)) (g : Nat → BitVec 64)
    (pc : BitVec 64) : GoodState (cutState m g pc) where
  cur_privilege := by csr
  misa := by csr
  mstatus := by csr
  mie := by csr
  mseccfg := by csr
  satp := by csr
  mtvec := by csr
  mideleg := by csr
  medeleg := by csr
  hart_state := by csr
  htif_done := by csr
  htif_tohost := ⟨0#64, by csr⟩
  htif_tohost_base := by csr
  elp := by csr
  pmpcfg_n := by csr
  pmpaddr_n := by csr
  pma_regions := by csr
  menvcfg := by csr
  mcountinhibit := by csr
  mcyclecfg := by csr
  minstretcfg := by csr
  mip := ⟨0#64, by csr⟩
  sig_meip := ⟨0#1, by csr⟩
  sig_seip := ⟨0#1, by csr⟩
  mtime := ⟨0#64, by csr⟩
  mtimecmp := ⟨0#64, by csr⟩
  minstret := ⟨0#64, by csr⟩
  minstret_increment := ⟨false, by csr⟩
  mcycle := ⟨0#64, by csr⟩
  nextPC := ⟨pc, by csr⟩
  PC := ⟨pc, by csr⟩

/-- What a cut pins of the registers (syi's `EntryRegs`, PC a parameter). -/
structure EntryRegs (σ : MState) (g : Nat → BitVec 64) (pc : BitVec 64) : Prop where
  good : GoodState σ
  pc : σ.regs.get? Register.PC = some pc
  /-- The HTIF mailbox is idle: no partial payload pending. -/
  payload : σ.regs.get? Register.htif_payload_writes = some (0#4)
  gpr : ∀ i, i < 31 → σ.regs.get? (gprReg (i + 1)) = some (gprRT (i + 1) (g (i + 1)))

theorem cutState_entryRegs (m : Std.ExtHashMap Nat (BitVec 8)) (g : Nat → BitVec 64)
    (pc : BitVec 64) : EntryRegs (cutState m g pc) g pc where
  good := cutState_good m g pc
  pc := by csr
  payload := by csr
  gpr := fun i hi =>
    cutRegs_get (List.mem_append_right _ (List.mem_map.2 ⟨i, List.mem_range.2 hi, rfl⟩))

end Tcc.Boot
