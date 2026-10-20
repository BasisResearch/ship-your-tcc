import Tcc.Boot.Run
import Vsa.Sim.Generic.ExitStep
import Vsa.Sim.HtifStepObs
import Vsa.Sim.Muldi3Spec

/-!
# One-instruction rules between replay rows

The segment layer (`row_step`) covers straight-line bodies ending at a branch,
`j` or `jr`. These rules cover the other steps the program phase takes, each a
generic lemma proved once from the copied Sail step lemmas, used at concrete
instances whose side conditions are closed kernel `decide`s:

* `row_jalr` / `row_jal`: calls (`jalr rd, imm(rs1)`, `jal rd, imm`, `rd ≠ x0`);
* `row_putchar`: the HTIF console store to `tohost` (`stepObs_tohost_putchar`);
* `row_exit`: the HTIF exit store, which halts (`stepOnce_tohost_G`).

`InstrAt` bundles the fetch facts of one instruction at a row.
-/

namespace Tcc.Boot

open LeanRV64DExecutable LeanRV64DExecutable.Functions Sail ConcurrencyInterfaceV1
open Vsa Vsa.Machine Vsa.Sim

/-- The fetch facts of the word `w` at `pc` decoding to `ast`, over a row's
view `v` and log `log`: every field is closed (`decide`, or `decodeW`). -/
structure InstrAt (v : Nat → Option (BitVec 8)) (log : List WEntry) (pc : BitVec 64)
    (w : BitVec 32) (ast : instruction) : Prop where
  b0 : (firstByte log pc.toNat).or (v pc.toNat) = some (w.extractLsb' 0 8)
  b1 : (firstByte log (pc.toNat + 1)).or (v (pc.toNat + 1)) = some (w.extractLsb' 8 8)
  b2 : (firstByte log (pc.toNat + 2)).or (v (pc.toNat + 2)) = some (w.extractLsb' 16 8)
  b3 : (firstByte log (pc.toNat + 3)).or (v (pc.toNat + 3)) = some (w.extractLsb' 24 8)
  word : (((w.extractLsb' 24 8).append (w.extractLsb' 16 8)).append
    (w.extractLsb' 8 8)).append (w.extractLsb' 0 8) = w
  notrvc : Sail.BitVec.extractLsb w 1 0 = 0b11#2
  lo : 0x80000000 ≤ pc.toNat
  fetch : FetchOK pc.toNat
  align : pc.toNat % 4 = 0
  dec : ∀ s : SequentialState RegisterType trivialChoiceSource,
    s.regs.get? Register.misa = some ((Vsa.Sim.initMisa) : RegisterType Register.misa) →
    s.regs.get? Register.cur_privilege =
      some ((Privilege.Machine) : RegisterType Register.cur_privilege) →
    s.regs.get? Register.mseccfg = some ((0#64) : RegisterType Register.mseccfg) →
    (ext_decode w).run s = .ok ast s

theorem pin_log {m0 : Std.ExtHashMap Nat (BitVec 8)} {v : Nat → Option (BitVec 8)}
    (hv : PartialView m0 v) {log : List WEntry} {a : Nat} {b : BitVec 8}
    (h : (firstByte log a).or (v a) = some b) : (writeLog m0 log.reverse)[a]? = some b := by
  rw [writeLog_get, logByte_reverse]; exact pin_view hv h

/-- The code pins, the decode fact at `afterPrelude σ`, and the word/RVC facts
of an `InstrAt` at a machine state whose memory is the row's. -/
theorem InstrAt.facts {v : Nat → Option (BitVec 8)} {log : List WEntry} {pc : BitVec 64}
    {w : BitVec 32} {ast : instruction} (hi : InstrAt v log pc w ast) {σ : MState}
    (hG : GoodState σ) {m0 : Std.ExtHashMap Nat (BitVec 8)} (hv : PartialView m0 v)
    (hm : σ.mem = writeLog m0 log.reverse) :
    σ.mem[pc.toNat]? = some (w.extractLsb' 0 8) ∧
    σ.mem[pc.toNat + 1]? = some (w.extractLsb' 8 8) ∧
    σ.mem[pc.toNat + 2]? = some (w.extractLsb' 16 8) ∧
    σ.mem[pc.toNat + 3]? = some (w.extractLsb' 24 8) ∧
    (ext_decode w).run (afterPrelude σ) = .ok ast (afterPrelude σ) := by
  rw [hm]
  exact ⟨pin_log hv hi.b0, pin_log hv hi.b1, pin_log hv hi.b2, pin_log hv hi.b3,
    hi.dec (afterPrelude σ)
      (by rw [get?_afterPrelude σ _ (by decide)]; exact hG.misa)
      (by rw [get?_afterPrelude σ _ (by decide)]; exact hG.cur_privilege)
      (by rw [get?_afterPrelude σ _ (by decide)]; exact hG.mseccfg)⟩

theorem InstrAt.notrvc' {v : Nat → Option (BitVec 8)} {log : List WEntry} {pc : BitVec 64}
    {w : BitVec 32} {ast : instruction} (hi : InstrAt v log pc w ast) :
    Sail.BitVec.extractLsb ((((w.extractLsb' 24 8).append (w.extractLsb' 16 8)).append
      (w.extractLsb' 8 8)).append (w.extractLsb' 0 8)) 1 0 = 0b11#2 := by
  rw [hi.word]; exact hi.notrvc

/-! ## GPR reads and frames for symbolic indices -/

theorem gprGet_congr {σ σ' : MState} : ∀ n, 1 ≤ n → n ≤ 31 →
    σ'.regs.get? (gprReg n) = σ.regs.get? (gprReg n) → gprGet σ' n = gprGet σ n
  | 1, _, _, h | 2, _, _, h | 3, _, _, h | 4, _, _, h | 5, _, _, h | 6, _, _, h | 7, _, _, h
  | 8, _, _, h | 9, _, _, h | 10, _, _, h | 11, _, _, h | 12, _, _, h | 13, _, _, h
  | 14, _, _, h | 15, _, _, h | 16, _, _, h | 17, _, _, h | 18, _, _, h | 19, _, _, h
  | 20, _, _, h | 21, _, _, h | 22, _, _, h | 23, _, _, h | 24, _, _, h | 25, _, _, h
  | 26, _, _, h | 27, _, _, h | 28, _, _, h | 29, _, _, h | 30, _, _, h | 31, _, _, h => h
  | 0, h, _, _ => absurd h (by omega)
  | _ + 32, _, h, _ => absurd h (by omega)

theorem gprGet_of_get {σ : MState} {x : BitVec 64} : ∀ n, 1 ≤ n → n ≤ 31 →
    σ.regs.get? (gprReg n) = some (gprRT n x) → gprGet σ n = some x
  | 1, _, _, h | 2, _, _, h | 3, _, _, h | 4, _, _, h | 5, _, _, h | 6, _, _, h | 7, _, _, h
  | 8, _, _, h | 9, _, _, h | 10, _, _, h | 11, _, _, h | 12, _, _, h | 13, _, _, h
  | 14, _, _, h | 15, _, _, h | 16, _, _, h | 17, _, _, h | 18, _, _, h | 19, _, _, h
  | 20, _, _, h | 21, _, _, h | 22, _, _, h | 23, _, _, h | 24, _, _, h | 25, _, _, h
  | 26, _, _, h | 27, _, _, h | 28, _, _, h | 29, _, _, h | 30, _, _, h | 31, _, _, h => h
  | 0, h, _, _ => absurd h (by omega)
  | _ + 32, _, h, _ => absurd h (by omega)

/-- A register that is not a GPR differs from every `gprReg n`. -/
abbrev NotGpr (R : Register) : Prop := ∀ n, (R == gprReg n) = false

theorem notGpr_of {R : Register} (h : ∀ n, n < 32 → (R == gprReg n) = false) : NotGpr R := by
  intro n
  by_cases hn : n < 32
  · exact h n hn
  · have : gprReg n = Register.x1 := by
      unfold gprReg; split <;> first | rfl | omega
    rw [this]; exact h 1 (by decide)

theorem notGpr_minstret : NotGpr Register.minstret := notGpr_of (by decide)
theorem notGpr_PC : NotGpr Register.PC := notGpr_of (by decide)
theorem notGpr_nextPC : NotGpr Register.nextPC := notGpr_of (by decide)
theorem notGpr_minstret_increment : NotGpr Register.minstret_increment := notGpr_of (by decide)
theorem notGpr_mcycle : NotGpr Register.mcycle := notGpr_of (by decide)
theorem notGpr_mtime : NotGpr Register.mtime := notGpr_of (by decide)
theorem notGpr_mip : NotGpr Register.mip := notGpr_of (by decide)
theorem notGpr_cmd : NotGpr Register.htif_cmd_write := notGpr_of (by decide)
theorem notGpr_payload : NotGpr Register.htif_payload_writes := notGpr_of (by decide)
theorem notGpr_tohost : NotGpr Register.htif_tohost := notGpr_of (by decide)

/-- Pins of every GPR but `rd` survive a step that preserves those GPRs. -/
theorem gholds_erase_frame {σ σ' : MState} (rd : Nat)
    (h : ∀ n, 1 ≤ n → n ≤ 31 → n ≠ rd → σ'.regs.get? (gprReg n) = σ.regs.get? (gprReg n)) :
    ∀ (L : GRegs), KeysOK (keysG L) → GHolds σ L → GHolds σ' (eraseG rd L) := by
  intro L
  induction L with
  | nil => intro _ _; exact trivial
  | cons p L ih =>
    obtain ⟨n, x⟩ := p
    intro hk hL
    have hn := hk n (List.mem_cons_self ..)
    have hk' : KeysOK (keysG L) := fun m hm => hk m (List.mem_cons_of_mem _ hm)
    unfold eraseG
    split
    · exact ih hk' hL.2
    · rename_i hne
      exact ⟨(gprGet_congr n hn.1 hn.2 (h n hn.1 hn.2 hne)).trans hL.1, ih hk' hL.2⟩

/-- Pins of every GPR survive a step that preserves all GPRs. -/
theorem gholds_frame {σ σ' : MState}
    (h : ∀ n, 1 ≤ n → n ≤ 31 → σ'.regs.get? (gprReg n) = σ.regs.get? (gprReg n)) :
    ∀ (L : GRegs), KeysOK (keysG L) → GHolds σ L → GHolds σ' L := by
  intro L
  induction L with
  | nil => intro _ _; exact trivial
  | cons p L ih =>
    obtain ⟨n, x⟩ := p
    intro hk hL
    have hn := hk n (List.mem_cons_self ..)
    exact ⟨(gprGet_congr n hn.1 hn.2 (h n hn.1 hn.2)).trans hL.1,
      ih (fun m hm => hk m (List.mem_cons_of_mem _ hm)) hL.2⟩

/-! ## Calls: `jalr rd, imm(rs1)` -/

theorem get?_sigmaTick_jalr (σ : MState) (pc vminstret tgt : BitVec 64)
    (rd_reg : Register) (link : RegisterType rd_reg)
    (vmip vmtime vmtimecmp vmcycle : BitVec 64) (R : Register)
    (hmc : (Register.mcycle == R) = false) (hmt : (Register.mtime == R) = false)
    (hmi : (Register.mip == R) = false) :
    (sigmaTick_jalr σ pc vminstret tgt rd_reg link vmip vmtime vmtimecmp vmcycle).regs.get? R
      = (sigmaPost_jalr σ pc vminstret tgt rd_reg link).regs.get? R := by
  show (((((sigmaPost_jalr σ pc vminstret tgt rd_reg link).regs.insert Register.mcycle _).insert
      Register.mtime _).insert Register.mip _)).get? R = _
  rw [Std.ExtDHashMap.get?_insert]
  simp only [hmi, dif_neg, reduceCtorEq, not_false_eq_true]
  rw [Std.ExtDHashMap.get?_insert]
  simp only [hmt, dif_neg, reduceCtorEq, not_false_eq_true]
  rw [Std.ExtDHashMap.get?_insert]
  simp only [hmc, dif_neg, reduceCtorEq, not_false_eq_true]

/-- `jalr` with a link register, parity-absorbing (after `stepObs_jal`). -/
theorem stepObs_jalr
    (σ : MState) (i u : Nat) (pc : BitVec 64) (vminstret vrs1 : BitVec 64)
    (w : BitVec 32) (imm : BitVec 12) (rs1 rd : regidx) (rd_reg : Register)
    (link : RegisterType rd_reg) (b0 b1 b2 b3 : BitVec 8)
    (hG : GoodState σ) (hpc : σ.regs.get? Register.PC = some pc)
    (hminstret : σ.regs.get? Register.minstret = some vminstret)
    (hb0 : σ.mem[pc.toNat]? = some b0) (hb1 : σ.mem[pc.toNat + 1]? = some b1)
    (hb2 : σ.mem[pc.toNat + 2]? = some b2) (hb3 : σ.mem[pc.toNat + 3]? = some b3)
    (hlo : 0x80000000 ≤ pc.toNat) (hhi : FetchOK pc.toNat) (halign : pc.toNat % 4 = 0)
    (hnotrvc : Sail.BitVec.extractLsb (((b3.append b2).append b1).append b0) 1 0 = (0b11#2 : BitVec 2))
    (hword : (((b3.append b2).append b1).append b0) = w)
    (hdec : (ext_decode w).run (afterPrelude σ)
      = .ok (instruction.JALR (imm, rs1, rd)) (afterPrelude σ))
    (hrs1 : (rX_bits rs1).run (afterNextPC (afterPrelude σ) pc)
      = .ok vrs1 (afterNextPC (afterPrelude σ) pc))
    (htgt : (BitVec.update (vrs1 + sign_extend (m := 64) imm) 0 0#1).toNat % 4 = 0)
    (hrd_npc : (rd_reg == Register.nextPC) = false)
    (hrd_mi : (rd_reg == Register.minstret_increment) = false)
    (hrd_ms : (rd_reg == Register.minstret) = false)
    (hrd_hart : (rd_reg == Register.hart_state) = false)
    (hrd : NonPinned rd_reg)
    (hwr : (wX_bits rd (BitVec.addInt pc 4)).run
        {(afterNextPC (afterPrelude σ) pc) with
          regs := (afterNextPC (afterPrelude σ) pc).regs.insert Register.nextPC
            (BitVec.update (vrs1 + sign_extend (m := 64) imm) 0 0#1)}
        = .ok () (sigma3_jalr σ pc (BitVec.update (vrs1 + sign_extend (m := 64) imm) 0 0#1) rd_reg link))
    (hi : i < 2) :
    ∃ (σ' : MState) (i' : Nat),
      Vsa.Machine.Step ⟨σ, i, u⟩ ⟨σ', i', u + 1⟩ ∧ i' < 2 ∧ GoodState σ' ∧
      σ'.mem = σ.mem ∧
      ReadsLikePost σ' (sigmaPost_jalr σ pc vminstret
        (BitVec.update (vrs1 + sign_extend (m := 64) imm) 0 0#1) rd_reg link) := by
  by_cases htick : i + 1 = 2
  · have hGp := goodstate_sigmaPost_jalr σ pc vminstret
      (BitVec.update (vrs1 + sign_extend (m := 64) imm) 0 0#1) rd_reg hrd link hG
    obtain ⟨vmip, hmip⟩ := hGp.mip
    obtain ⟨vmtime, hmtime⟩ := hGp.mtime
    obtain ⟨vmtimecmp, hmtimecmp⟩ := hGp.mtimecmp
    obtain ⟨vmcycle, hmcycle⟩ := hGp.mcycle
    obtain ⟨hstep, hGt⟩ := step_jalr_tick σ i u pc vminstret vrs1 w imm rs1 rd rd_reg link
      b0 b1 b2 b3 vmip vmtime vmtimecmp vmcycle hG hpc hminstret hmip hmtime hmtimecmp hmcycle
      hb0 hb1 hb2 hb3 hlo hhi halign hnotrvc hword hdec hrs1 htgt
      hrd_npc hrd_mi hrd_ms hrd_hart hrd hwr htick
    refine ⟨_, 0, hstep, by decide, hGt, rfl, ?_, rfl⟩
    intro R hmc hmt hmi
    exact get?_sigmaTick_jalr σ pc vminstret _ rd_reg link vmip vmtime vmtimecmp vmcycle R hmc hmt hmi
  · obtain ⟨hstep, hGt⟩ := step_jalr_notick σ i u pc vminstret vrs1 w imm rs1 rd rd_reg link
      b0 b1 b2 b3 hG hpc hminstret hb0 hb1 hb2 hb3 hlo hhi halign hnotrvc hword hdec hrs1 htgt
      hrd_npc hrd_mi hrd_ms hrd_hart hrd hwr htick
    exact ⟨_, i + 1, hstep, by omega, hGt, rfl, ReadsLikePost.rfl _⟩

/-- A call through a register: `jalr rd, imm(rs1)` with `rd ≠ x0`. The next
row's pins `L'` are checked against `rd := pc + 4` over `L`. -/
theorem row_jalr {v : Nat → Option (BitVec 8)} {pc tgt : BitVec 64} {L L' : GRegs}
    {log : List WEntry} {out : String} {w : BitVec 32} {imm : BitVec 12} {rd rs1 : Nat}
    (hi : InstrAt v log pc w (instruction.JALR (imm, gprIdx rs1, gprIdx rd)))
    (hrd : 1 ≤ rd ∧ rd ≤ 31) (hrs1 : rs1 ≤ 31 ∧ (rs1 = 0 ∨ rs1 ∈ keysG L))
    (htgt : BitVec.update (srcVal rs1 L + sign_extend (m := 64) imm) 0 0#1 = tgt)
    (halign : tgt.toNat % 4 = 0)
    (hL' : ∀ p ∈ L', lookupG p.1 ((rd, BitVec.addInt pc 4) :: eraseG rd L) = some p.2)
    (hk' : KeysOK (keysG L')) :
    Leads (RunRow v pc L log out) (RunRow v tgt L' log out) := by
  rintro ⟨σ, i, u⟩ ⟨hG, hpc, hL, hk, ⟨m0, hv, hm⟩, hout, hpw, hti⟩
  obtain ⟨vm, hmi⟩ := hG.minstret
  obtain ⟨hb0, hb1, hb2, hb3, hdec⟩ := hi.facts hG hv hm
  have hrx1 := rX_src σ pc rs1 hrs1.1 _ (srcPin_srcVal σ L rs1 hrs1.2 hL)
  obtain ⟨hr1, hr2, hr3, hr4, hr5⟩ := gpr_rd_ok rd (by omega) hrd.1
  have hwx := wX_gpr {(afterNextPC (afterPrelude σ) pc) with
      regs := (afterNextPC (afterPrelude σ) pc).regs.insert Register.nextPC
        (BitVec.update (srcVal rs1 L + sign_extend (m := 64) imm) 0 0#1)}
    (BitVec.addInt pc 4) rd hrd.1 hrd.2
  obtain ⟨σ', i', hs, hi', hG', hmem', hobs⟩ :=
    stepObs_jalr σ i u pc vm (srcVal rs1 L) _ imm (gprIdx rs1) (gprIdx rd) (gprReg rd)
      (gprRT rd (BitVec.addInt pc 4)) _ _ _ _ hG hpc hmi hb0 hb1 hb2 hb3 hi.lo hi.fetch
      hi.align hi.notrvc' hi.word (hi.word ▸ hdec) hrx1 (htgt ▸ halign)
      hr1 hr2 hr3 hr4 hr5 hwx hti
  -- reads off the post state
  have hfr : ∀ R : Register, NotGpr R → (Register.mcycle == R) = false →
      (Register.mtime == R) = false → (Register.mip == R) = false →
      (Register.minstret == R) = false → (Register.PC == R) = false →
      (Register.nextPC == R) = false → (Register.minstret_increment == R) = false →
      σ'.regs.get? R = σ.regs.get? R := fun R hR h1 h2 h3 h4 h5 h6 h7 =>
    (hobs.1 R h1 h2 h3).trans (get?_sigmaPost_jalr σ pc vm _ _ _ R h4 h5
      (by rw [Bool.beq_comm]; exact hR rd) h6 h7)
  have hgpr : ∀ n, 1 ≤ n → n ≤ 31 → n ≠ rd →
      σ'.regs.get? (gprReg n) = σ.regs.get? (gprReg n) := fun n h1 h31 hne =>
    (hobs.1 _ (notGpr_mcycle n) (notGpr_mtime n) (notGpr_mip n)).trans
      (get?_sigmaPost_jalr σ pc vm _ _ _ _ (notGpr_minstret n) (notGpr_PC n)
        (gprReg_beq_false rd (by omega) n (by omega) hrd.1 h1 (Ne.symm hne))
        (notGpr_nextPC n) (notGpr_minstret_increment n))
  have hrdv : σ'.regs.get? (gprReg rd) = some (gprRT rd (BitVec.addInt pc 4)) := by
    rw [hobs.1 _ (notGpr_mcycle rd) (notGpr_mtime rd) (notGpr_mip rd)]
    show ((((sigma3_jalr σ pc _ (gprReg rd) _).regs.insert Register.PC _).insert
      Register.minstret _)).get? (gprReg rd) = _
    rw [Std.ExtDHashMap.get?_insert]
    simp only [notGpr_minstret rd, dif_neg, reduceCtorEq, not_false_eq_true]
    rw [Std.ExtDHashMap.get?_insert]
    simp only [notGpr_PC rd, dif_neg, reduceCtorEq, not_false_eq_true]
    show ((_ : Std.ExtDHashMap Register RegisterType).insert (gprReg rd) _).get? (gprReg rd) = _
    rw [Std.ExtDHashMap.get?_insert_self]
  have hGH : GHolds σ' ((rd, BitVec.addInt pc 4) :: eraseG rd L) :=
    ⟨gprGet_of_get rd hrd.1 hrd.2 hrdv, gholds_erase_frame rd hgpr L hk hL⟩
  refine ⟨⟨σ', i', u + 1⟩, Steps.head hs (Steps.refl _), hG', ?_, gholds_of_lookup hGH L' hL',
    hk', ⟨m0, hv, hmem'.trans hm⟩, ?_, ?_, hi'⟩
  · rw [hobs.1 _ (by decide) (by decide) (by decide)]
    show ((((sigma3_jalr σ pc _ (gprReg rd) _).regs.insert Register.PC _).insert
      Register.minstret _)).get? Register.PC = _
    rw [Std.ExtDHashMap.get?_insert]
    simp only [show (Register.minstret == Register.PC) = false from by decide, dif_neg,
      reduceCtorEq, not_false_eq_true]
    rw [Std.ExtDHashMap.get?_insert_self, htgt]
  · simp only [output] at hout ⊢; rw [hobs.2]; exact hout
  · rw [hfr _ notGpr_payload (by decide) (by decide) (by decide) (by decide) (by decide)
      (by decide) (by decide)]
    exact hpw

/-! ## One ALU instruction writing `rd ≠ x0` -/

/-- The core of the one-instruction ALU rules: an instruction whose execute
writes `rd := val` (`hexec`, proved per kind below). -/
theorem row_alu_core {v : Nat → Option (BitVec 8)} {pc : BitVec 64} {L L' : GRegs}
    {log : List WEntry} {out : String} {w : BitVec 32} {ast : instruction} {rd : Nat}
    {val : BitVec 64}
    (hi : InstrAt v log pc w ast) (hrd : 1 ≤ rd ∧ rd ≤ 31)
    (hexec : ∀ σ : MState, GoodState σ → GHolds σ L →
      (execute ast).run (afterNextPC (afterPrelude σ) pc)
        = .ok RETIRE_SUCCESS (sigma3_alu σ pc (gprReg rd) (gprRT rd val)))
    (hL' : ∀ p ∈ L', lookupG p.1 ((rd, val) :: eraseG rd L) = some p.2)
    (hk' : KeysOK (keysG L')) :
    Leads (RunRow v pc L log out) (RunRow v (BitVec.addInt pc 4) L' log out) := by
  rintro ⟨σ, i, u⟩ ⟨hG, hpc, hL, hk, ⟨m0, hv, hm⟩, hout, hpw, hti⟩
  obtain ⟨vm, hmi⟩ := hG.minstret
  obtain ⟨hb0, hb1, hb2, hb3, hdec⟩ := hi.facts hG hv hm
  obtain ⟨hr1, hr2, hr3, hr4, hr5⟩ := gpr_rd_ok rd (by omega) hrd.1
  obtain ⟨σ', i', hs, hi', hG', hmem', hobs⟩ :=
    stepObs_alu σ i u pc vm _ ast (gprReg rd) (gprRT rd val) _ _ _ _ hG hpc hmi hi.word
      hi.notrvc' (hi.word ▸ hdec) (hexec σ hG hL)
      hr1 hr2 hr3 hr4 hr5 hb0 hb1 hb2 hb3 hi.lo hi.fetch
      hi.align hti
  have hgpr : ∀ n, 1 ≤ n → n ≤ 31 → n ≠ rd →
      σ'.regs.get? (gprReg n) = σ.regs.get? (gprReg n) := fun n h1 h31 hne =>
    (hobs.1 _ (notGpr_mcycle n) (notGpr_mtime n) (notGpr_mip n)).trans
      (get?_sigmaPost_alu σ pc vm _ _ _ (notGpr_minstret n) (notGpr_PC n)
        (gprReg_beq_false rd (by omega) n (by omega) hrd.1 h1 (Ne.symm hne))
        (notGpr_nextPC n) (notGpr_minstret_increment n))
  have hrdv : σ'.regs.get? (gprReg rd) = some (gprRT rd val) := by
    rw [hobs.1 _ (notGpr_mcycle rd) (notGpr_mtime rd) (notGpr_mip rd)]
    show ((((sigma3_alu σ pc (gprReg rd) _).regs.insert Register.PC _).insert
      Register.minstret _)).get? (gprReg rd) = _
    rw [Std.ExtDHashMap.get?_insert]
    simp only [notGpr_minstret rd, dif_neg, reduceCtorEq, not_false_eq_true]
    rw [Std.ExtDHashMap.get?_insert]
    simp only [notGpr_PC rd, dif_neg, reduceCtorEq, not_false_eq_true]
    show ((_ : Std.ExtDHashMap Register RegisterType).insert (gprReg rd) _).get? (gprReg rd) = _
    rw [Std.ExtDHashMap.get?_insert_self]
  have hGH : GHolds σ' ((rd, val) :: eraseG rd L) :=
    ⟨gprGet_of_get rd hrd.1 hrd.2 hrdv, gholds_erase_frame rd hgpr L hk hL⟩
  refine ⟨⟨σ', i', u + 1⟩, Steps.head hs (Steps.refl _), hG', ?_, gholds_of_lookup hGH L' hL',
    hk', ⟨m0, hv, hmem'.trans hm⟩, ?_, ?_, hi'⟩
  · rw [hobs.1 _ (by decide) (by decide) (by decide)]
    show ((((sigma3_alu σ pc (gprReg rd) _).regs.insert Register.PC _).insert
      Register.minstret _)).get? Register.PC = _
    rw [Std.ExtDHashMap.get?_insert]
    simp only [show (Register.minstret == Register.PC) = false from by decide, dif_neg,
      reduceCtorEq, not_false_eq_true]
    rw [Std.ExtDHashMap.get?_insert_self]
  · simp only [output] at hout ⊢; rw [hobs.2]; exact hout
  · rw [hobs.1 _ (by decide) (by decide) (by decide),
      get?_sigmaPost_alu σ pc vm _ _ _ (by decide) (by decide)
        (by rw [Bool.beq_comm]; exact notGpr_payload rd) (by decide) (by decide)]
    exact hpw

/-- `sltu rd, rs1, rs2`. -/
theorem row_sltu {v : Nat → Option (BitVec 8)} {pc : BitVec 64} {L L' : GRegs}
    {log : List WEntry} {out : String} {w : BitVec 32} {rd rs1 rs2 : Nat}
    (hi : InstrAt v log pc w (instruction.RTYPE (gprIdx rs2, gprIdx rs1, gprIdx rd, rop.SLTU)))
    (hrd : 1 ≤ rd ∧ rd ≤ 31) (hrs1 : rs1 ≤ 31 ∧ (rs1 = 0 ∨ rs1 ∈ keysG L))
    (hrs2 : rs2 ≤ 31 ∧ (rs2 = 0 ∨ rs2 ∈ keysG L))
    (hL' : ∀ p ∈ L', lookupG p.1 ((rd, zero_extend (m := 64)
      (bool_to_bit (zopz0zI_u (srcVal rs1 L) (srcVal rs2 L)))) :: eraseG rd L) = some p.2)
    (hk' : KeysOK (keysG L')) :
    Leads (RunRow v pc L log out) (RunRow v (BitVec.addInt pc 4) L' log out) :=
  row_alu_core hi hrd (fun σ _ hL =>
    execute_rtype_sltu_char _ _ _ _ _ _ _
      (rX_src σ pc rs1 hrs1.1 _ (srcPin_srcVal σ L rs1 hrs1.2 hL))
      (rX_src σ pc rs2 hrs2.1 _ (srcPin_srcVal σ L rs2 hrs2.2 hL))
      (wX_gpr _ _ rd hrd.1 hrd.2)) hL' hk'

/-- `addi x0, x0, 0` (nop): seen as rewriting `x1` with its own value. -/
theorem row_nop {v : Nat → Option (BitVec 8)} {pc : BitVec 64} {L : GRegs}
    {log : List WEntry} {out : String} {w : BitVec 32}
    (hi : InstrAt v log pc w (instruction.ITYPE (0#12, gprIdx 0, gprIdx 0, iop.ADDI)))
    (h1 : 1 ∈ keysG L)
    (hL : ∀ p ∈ L, lookupG p.1 ((1, srcVal 1 L) :: eraseG 1 L) = some p.2)
    (hk : KeysOK (keysG L)) :
    Leads (RunRow v pc L log out) (RunRow v (BitVec.addInt pc 4) L log out) :=
  row_alu_core hi ⟨by decide, by decide⟩ (fun σ _ hLs => by
    have hx1 : σ.regs.get? Register.x1 = some (srcVal 1 L) :=
      srcPin_srcVal σ L 1 (Or.inr h1) hLs
    have hr : (afterNextPC (afterPrelude σ) pc).regs.insert Register.x1 (srcVal 1 L) =
        (afterNextPC (afterPrelude σ) pc).regs := by
      apply Std.ExtDHashMap.ext_get?
      intro k
      rw [Std.ExtDHashMap.get?_insert]
      split
      · rename_i hk
        have : Register.x1 = k := eq_of_beq hk
        subst this
        rw [get?_afterNextPC σ pc _ (by decide) (by decide), hx1]; rfl
      · rfl
    have hs : sigma3_alu σ pc (gprReg 1) (gprRT 1 (srcVal 1 L)) =
        afterNextPC (afterPrelude σ) pc := by
      show ({(afterNextPC (afterPrelude σ) pc) with
        regs := (afterNextPC (afterPrelude σ) pc).regs.insert Register.x1 (srcVal 1 L)} : MState) = _
      rw [hr]
    rw [hs]
    exact execute_itype_addi_char _ _ _ _ _ _ (rX_bits_zero _) (wX_bits_zero _ _)) hL hk

/-! ## Calls: `jal rd, imm` -/

/-- A direct call: `jal rd, imm` with `rd ≠ x0`. -/
theorem row_jal {v : Nat → Option (BitVec 8)} {pc tgt : BitVec 64} {L L' : GRegs}
    {log : List WEntry} {out : String} {w : BitVec 32} {imm : BitVec 21} {rd : Nat}
    (hi : InstrAt v log pc w (instruction.JAL (imm, gprIdx rd)))
    (hrd : 1 ≤ rd ∧ rd ≤ 31)
    (htgt : pc + sign_extend (m := 64) imm = tgt)
    (halign : tgt.toNat % 4 = 0)
    (hL' : ∀ p ∈ L', lookupG p.1 ((rd, BitVec.addInt pc 4) :: eraseG rd L) = some p.2)
    (hk' : KeysOK (keysG L')) :
    Leads (RunRow v pc L log out) (RunRow v tgt L' log out) := by
  rintro ⟨σ, i, u⟩ ⟨hG, hpc, hL, hk, ⟨m0, hv, hm⟩, hout, hpw, hti⟩
  obtain ⟨vm, hmi⟩ := hG.minstret
  obtain ⟨hb0, hb1, hb2, hb3, hdec⟩ := hi.facts hG hv hm
  obtain ⟨hr1, hr2, hr3, hr4, hr5⟩ := gpr_rd_ok rd (by omega) hrd.1
  have hwx := wX_gpr {(afterNextPC (afterPrelude σ) pc) with
      regs := (afterNextPC (afterPrelude σ) pc).regs.insert Register.nextPC
        (pc + sign_extend (m := 64) imm)}
    (BitVec.addInt pc 4) rd hrd.1 hrd.2
  obtain ⟨σ', i', hs, hi', hG', hmem', hobs⟩ :=
    stepObs_jal σ i u pc vm _ imm (gprIdx rd) (gprReg rd) (gprRT rd (BitVec.addInt pc 4))
      _ _ _ _ hG hpc hmi hb0 hb1 hb2 hb3 hi.lo hi.fetch hi.align hi.notrvc' hi.word
      (hi.word ▸ hdec) (htgt ▸ halign) hr1 hr2 hr3 hr4 hr5 hwx hti
  have hgpr : ∀ n, 1 ≤ n → n ≤ 31 → n ≠ rd →
      σ'.regs.get? (gprReg n) = σ.regs.get? (gprReg n) := fun n h1 h31 hne =>
    (hobs.1 _ (notGpr_mcycle n) (notGpr_mtime n) (notGpr_mip n)).trans
      (get?_sigmaPost_jal σ pc vm _ _ _ _ (notGpr_minstret n) (notGpr_PC n)
        (gprReg_beq_false rd (by omega) n (by omega) hrd.1 h1 (Ne.symm hne))
        (notGpr_nextPC n) (notGpr_minstret_increment n))
  have hrdv : σ'.regs.get? (gprReg rd) = some (gprRT rd (BitVec.addInt pc 4)) := by
    rw [hobs.1 _ (notGpr_mcycle rd) (notGpr_mtime rd) (notGpr_mip rd)]
    show ((((sigma3_jal σ pc imm (gprReg rd) _).regs.insert Register.PC _).insert
      Register.minstret _)).get? (gprReg rd) = _
    rw [Std.ExtDHashMap.get?_insert]
    simp only [notGpr_minstret rd, dif_neg, reduceCtorEq, not_false_eq_true]
    rw [Std.ExtDHashMap.get?_insert]
    simp only [notGpr_PC rd, dif_neg, reduceCtorEq, not_false_eq_true]
    show ((_ : Std.ExtDHashMap Register RegisterType).insert (gprReg rd) _).get? (gprReg rd) = _
    rw [Std.ExtDHashMap.get?_insert_self]
  have hGH : GHolds σ' ((rd, BitVec.addInt pc 4) :: eraseG rd L) :=
    ⟨gprGet_of_get rd hrd.1 hrd.2 hrdv, gholds_erase_frame rd hgpr L hk hL⟩
  refine ⟨⟨σ', i', u + 1⟩, Steps.head hs (Steps.refl _), hG', ?_, gholds_of_lookup hGH L' hL',
    hk', ⟨m0, hv, hmem'.trans hm⟩, ?_, ?_, hi'⟩
  · rw [hobs.1 _ (by decide) (by decide) (by decide)]
    show ((((sigma3_jal σ pc imm (gprReg rd) _).regs.insert Register.PC _).insert
      Register.minstret _)).get? Register.PC = _
    rw [Std.ExtDHashMap.get?_insert]
    simp only [show (Register.minstret == Register.PC) = false from by decide, dif_neg,
      reduceCtorEq, not_false_eq_true]
    rw [Std.ExtDHashMap.get?_insert_self, htgt]
  · simp only [output] at hout ⊢; rw [hobs.2]; exact hout
  · rw [hobs.1 _ (by decide) (by decide) (by decide),
      get?_sigmaPost_jal σ pc vm _ _ _ _ (by decide) (by decide)
        (by rw [Bool.beq_comm]; exact notGpr_payload rd) (by decide) (by decide)]
    exact hpw

/-! ## The HTIF console store -/

theorem output_push (σ σ' : MState) (s : String) (h : σ'.sailOutput = σ.sailOutput.push s) :
    output σ' = output σ ++ s := by
  simp only [output, h, Array.toList_push, String.join, List.foldl_append, List.foldl_cons,
    List.foldl_nil]

/-- `sd rs2, imm(rs1)` to `tohost` with a console-putchar command for the byte
`c`: one character more on the console, nothing else observable changes. -/
theorem row_putchar {v : Nat → Option (BitVec 8)} {pc : BitVec 64} {L : GRegs}
    {log : List WEntry} {out out' : String} {w : BitVec 32} {imm : BitVec 12} {rs1 rs2 : Nat}
    {c : BitVec 8}
    (hi : InstrAt v log pc w (instruction.STORE (imm, gprIdx rs2, gprIdx rs1, 8)))
    (hrs1 : rs1 ≤ 31 ∧ (rs1 = 0 ∨ rs1 ∈ keysG L)) (hrs2 : rs2 ≤ 31 ∧ (rs2 = 0 ∨ rs2 ∈ keysG L))
    (haddr : srcVal rs1 L + sign_extend (m := 64) imm = BitVec.ofNat 64 tohostAddr)
    (hputc : srcVal rs2 L = (0x0101000000000000#64) ||| BitVec.zeroExtend 64 c)
    (hout' : out ++ toString (Char.ofNat c.toNat) = out') :
    Leads (RunRow v pc L log out) (RunRow v (BitVec.addInt pc 4) L log out') := by
  rintro ⟨σ, i, u⟩ ⟨hG, hpc, hL, hk, ⟨m0, hv, hm⟩, hout, hpw, hti⟩
  obtain ⟨vm, hmi⟩ := hG.minstret
  obtain ⟨th, hth⟩ := hG.htif_tohost
  obtain ⟨hb0, hb1, hb2, hb3, hdec⟩ := hi.facts hG hv hm
  have hrx1 := rX_src σ pc rs1 hrs1.1 _ (srcPin_srcVal σ L rs1 hrs1.2 hL)
  have hrx2 := rX_src σ pc rs2 hrs2.1 _ (srcPin_srcVal σ L rs2 hrs2.2 hL)
  obtain ⟨σ', i', hs, hi', hG', hmem', hso, hpc', _, hpw', _, hfr⟩ :=
    stepObs_tohost_putchar σ i u pc vm _ imm (gprIdx rs2) (gprIdx rs1) _ _ _ c th _ _ _ _
      hG hpc hmi hi.word hi.notrvc' (hi.word ▸ hdec) hrx1 hrx2 haddr rfl hpw hth hputc
      hb0 hb1 hb2 hb3 hi.lo hi.fetch hi.align hti
  have hgpr : ∀ n, 1 ≤ n → n ≤ 31 → σ'.regs.get? (gprReg n) = σ.regs.get? (gprReg n) :=
    fun n _ _ => hfr _ (notGpr_PC n) (notGpr_minstret n) (notGpr_minstret_increment n)
      (notGpr_nextPC n) (notGpr_cmd n) (notGpr_payload n) (notGpr_tohost n) (notGpr_mip n)
      (notGpr_mtime n) (notGpr_mcycle n)
  refine ⟨⟨σ', i', u + 1⟩, Steps.head hs (Steps.refl _), hG', hpc', gholds_frame hgpr L hk hL,
    hk, ⟨m0, hv, hmem'.trans hm⟩, ?_, hpw', hi'⟩
  show output σ' = out'
  rw [output_push σ σ' _ hso, ← hout', ← hout]

/-! ## The HTIF exit store -/

/-- `sd rs2, imm(rs1)` to `tohost` with the exit command `(e <<< 1) ||| 1`: the
machine halts with code `e`, the console as it is. -/
theorem row_exit {v : Nat → Option (BitVec 8)} {pc : BitVec 64} {L : GRegs}
    {log : List WEntry} {out : String} {w : BitVec 32} {imm : BitVec 12} {rs1 rs2 : Nat}
    {e : BitVec 64}
    (hi : InstrAt v log pc w (instruction.STORE (imm, gprIdx rs2, gprIdx rs1, 8)))
    (hrs1 : rs1 ≤ 31 ∧ (rs1 = 0 ∨ rs1 ∈ keysG L)) (hrs2 : rs2 ≤ 31 ∧ (rs2 = 0 ∨ rs2 ∈ keysG L))
    (haddr : srcVal rs1 L + sign_extend (m := 64) imm = BitVec.ofNat 64 tohostAddr)
    (hdata : srcVal rs2 L = (e <<< 1) ||| 1#64) (hsmall : e.toNat < 2 ^ 47) :
    ∀ c, RunRow v pc L log out c → Halts c out e.toNat := by
  rintro ⟨σ, i, u⟩ ⟨hG, hpc, hL, hk, ⟨m0, hv, hm⟩, hout, hpw, hti⟩
  obtain ⟨vm, hmi⟩ := hG.minstret
  obtain ⟨th, hth⟩ := hG.htif_tohost
  obtain ⟨hb0, hb1, hb2, hb3, hdec⟩ := hi.facts hG hv hm
  have hrx1 := rX_src σ pc rs1 hrs1.1 _ (srcPin_srcVal σ L rs1 hrs1.2 hL)
  have hrx2 := rX_src σ pc rs2 hrs2.1 _ (srcPin_srcVal σ L rs2 hrs2.2 hL)
  have hstep := stepOnce_tohost_G σ i u pc vm _ imm (gprIdx rs2) (gprIdx rs1) _ _ _ e th
    _ _ _ _ hG hpc hmi hi.word hi.notrvc' (hi.word ▸ hdec) hrx1 hrx2 haddr rfl hpw hth hsmall
    hdata hb0 hb1 hb2 hb3 hi.lo hi.fetch hi.align
  exact ⟨⟨σ, i, u⟩, _, Steps.refl _, Halted.mk hstep, hout⟩

end Tcc.Boot
