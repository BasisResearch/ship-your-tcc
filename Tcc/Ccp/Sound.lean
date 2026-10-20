import Tcc.Ccp.Lemmas
import Tcc.Boot.Step

/-!
# Soundness of the evaluator against the Sail model (CompileCut pilot, P2)

`Sim c s cf` relates an evaluator state `s` to a machine configuration `cf`:
the PC, the 31 GPRs (`pinsOf`), and every byte the evaluator knows as the
model's total read of that byte; the code table's bytes are present in memory
(`CodeIn`); the console is empty and the HTIF mailbox idle. `TableOK c` is the
code table's static correctness: every entry's word decodes, under Sail, to
the instruction the evaluator executes (`EntryOK`).

`exec_sound` is the generic one-step theorem, proved once per instruction
kind from the copied per-instruction machinery (`block_mem_sound` for ALU,
load and store kinds; `term_step_bt` for branches, `j`, `jr`; `stepObs_jal` /
`stepObs_jalr` for linked jumps). Currently covered: the fragment `InFrag`.
-/

namespace Tcc.Ccp

open LeanRV64DExecutable LeanRV64DExecutable.Functions Sail ConcurrencyInterfaceV1
open Vsa Vsa.Machine Vsa.Sim Tcc.Boot

/-! ## Registers as pins -/

/-- The GPRs `x1 .. x_k` of a packed register file as a pin list. -/
def pinsN (R : Nat) : Nat → GRegs
  | 0 => []
  | k + 1 => (k + 1, BitVec.ofNat 64 (getR R (k + 1))) :: pinsN R k

/-- The 31 GPRs of a packed register file as a pin list. -/
abbrev pinsOf (R : Nat) : GRegs := pinsN R 31

theorem mem_keys_pinsN {R n : Nat} : ∀ k, n ∈ keysG (pinsN R k) ↔ 1 ≤ n ∧ n ≤ k
  | 0 => by simp [pinsN, keysG]; omega
  | k + 1 => by
    simp only [pinsN, keysG, List.mem_cons, mem_keys_pinsN k]
    omega

theorem mem_keys_pinsOf {R n : Nat} : n ∈ keysG (pinsOf R) ↔ 1 ≤ n ∧ n ≤ 31 := mem_keys_pinsN 31

theorem keysOK_pinsOf (R : Nat) : KeysOK (keysG (pinsOf R)) := fun _ h => mem_keys_pinsOf.1 h

theorem lookupG_pinsN {R n : Nat} (h1 : 1 ≤ n) : ∀ k, n ≤ k →
    lookupG n (pinsN R k) = some (BitVec.ofNat 64 (getR R n))
  | 0, h => by omega
  | k + 1, h => by
    simp only [pinsN, lookupG]
    by_cases he : k + 1 = n
    · subst he; simp
    · rw [if_neg he]; exact lookupG_pinsN h1 k (by omega)

theorem lookupG_pinsOf {R n : Nat} (h1 : 1 ≤ n) (h2 : n ≤ 31) :
    lookupG n (pinsOf R) = some (BitVec.ofNat 64 (getR R n)) := lookupG_pinsN h1 31 h2

theorem pinsN_mem {R : Nat} {p : Nat × BitVec 64} : ∀ k, p ∈ pinsN R k →
    1 ≤ p.1 ∧ p.1 ≤ k ∧ p.2 = BitVec.ofNat 64 (getR R p.1)
  | 0, h => nomatch h
  | k + 1, h => by
    rcases List.mem_cons.1 h with h | h
    · subst h; simp
    · obtain ⟨a, b, c⟩ := pinsN_mem k h; exact ⟨a, by omega, c⟩

theorem srcVal_pinsOf {R n : Nat} (hx0 : getR R 0 = 0) (hn : n ≤ 31) :
    srcVal n (pinsOf R) = BitVec.ofNat 64 (getR R n) := by
  cases n with
  | zero => simp [srcVal, hx0]
  | succ m => simp [srcVal, lookupG_pinsOf (R := R) (by omega) hn]

theorem lookupG_eraseG_ne {x n : Nat} (h : n ≠ x) : ∀ L : GRegs, lookupG x (eraseG n L) = lookupG x L
  | [] => rfl
  | (m, v) :: L => by
    unfold eraseG
    by_cases hm : m = n
    · subst hm
      rw [if_pos rfl, lookupG_eraseG_ne h L]
      simp [lookupG, h]
    · rw [if_neg hm]
      simp only [lookupG]
      split
      · rfl
      · exact lookupG_eraseG_ne h L

/-- After `x_rd := v`, the computed pin list `(rd, v) :: eraseG rd (pinsOf R)`
holds every pin of `pinsOf (setR R rd v)`. -/
theorem pins_setR {σ : MState} {R rd : Nat} {v : BitVec 64} (hrd1 : 1 ≤ rd) (hrd2 : rd ≤ 31)
    (h : GHolds σ ((rd, v) :: eraseG rd (pinsOf R))) :
    GHolds σ (pinsOf (setR R rd v.toNat)) := by
  apply gholds_of_lookup h
  intro p hp
  obtain ⟨h1, h2, h3⟩ := pinsN_mem 31 hp
  obtain ⟨n, x⟩ := p
  simp only at h1 h2 h3
  subst h3
  simp only [lookupG]
  by_cases he : rd = n
  · subst he
    rw [if_pos rfl, getR_setR, if_pos ⟨rfl, by omega⟩, Nat.mod_eq_of_lt v.isLt]; simp
  · rw [if_neg he, lookupG_eraseG_ne he, lookupG_pinsOf h1 h2, getR_setR, if_neg (by omega)]

theorem getR_setR_zero {R rd : Nat} (v : Nat) (h : getR R 0 = 0) : getR (setR R rd v) 0 = 0 := by
  rw [getR_setR]; split
  · omega
  · exact h

/-! ## The code table and the simulation relation -/

/-- A table entry with its instruction word. -/
def CT.getW : CT → Nat → Option (Nat × Ins)
  | .nil, _ => none
  | .leaf p w i, pc => match Nat.beq p pc with | true => some (w, i) | false => none
  | .node s l r, pc => match Nat.ble s pc with | true => r.getW pc | false => l.getW pc

theorem CT.get_getW : ∀ (t : CT) (pc : Nat), t.get pc = (t.getW pc).map Prod.snd
  | .nil, _ => rfl
  | .leaf p w i, pc => by
    simp only [CT.get, CT.getW]; cases Nat.beq p pc <;> rfl
  | .node s l r, pc => by
    simp only [CT.get, CT.getW]; cases Nat.ble s pc <;> simp only [CT.get_getW]

/-- Byte `k` of a word, as the decoder's fetch assembles it. -/
abbrev wbyte (w k : Nat) : BitVec 8 := (BitVec.ofNat 32 w).extractLsb' (8 * k) 8

/-- The straight-line instruction an entry denotes, for the copied block lemmas. -/
def mkM (pc w : Nat) (k : MKind) (rd rs1 rs2 imm : Nat) : MInstr :=
  { pc := BitVec.ofNat 64 pc, word := BitVec.ofNat 32 w,
    b0 := wbyte w 0, b1 := wbyte w 1, b2 := wbyte w 2, b3 := wbyte w 3,
    kind := k, rd := rd, rs1 := rs1, rs2 := rs2, imm := BitVec.ofNat 12 imm }

/-- The terminator an entry denotes. -/
def mkT (pc w : Nat) (k : TKind) (rs1 rs2 : Nat) (i13 : BitVec 13) (i21 : BitVec 21)
    (i12 : BitVec 12) : TInstr :=
  { pc := BitVec.ofNat 64 pc, word := BitVec.ofNat 32 w,
    b0 := wbyte w 0, b1 := wbyte w 1, b2 := wbyte w 2, b3 := wbyte w 3,
    kind := k, rs1 := rs1, rs2 := rs2, imm13 := i13, imm21 := i21, imm12 := i12 }

/-- What fetching an entry needs, whatever the instruction: in the code range,
in RAM, aligned, a 32-bit non-compressed word. -/
structure Fetchable (c : Code) (pc w : Nat) : Prop where
  lo : c.lo ≤ pc
  hi : pc + 4 ≤ c.hi
  base : 0x80000000 ≤ pc
  fetch : FetchOK pc
  align : pc % 4 = 0
  word : (((wbyte w 3).append (wbyte w 2)).append (wbyte w 1)).append (wbyte w 0) = BitVec.ofNat 32 w
  rvc : Sail.BitVec.extractLsb (BitVec.ofNat 32 w) 1 0 = 0b11#2

/-- The 12-bit immediate field `imm` denotes the 64-bit value `imm`. -/
abbrev Imm12 (imm : Nat) : Prop := sign_extend (m := 64) (BitVec.ofNat 12 imm) = BitVec.ofNat 64 imm

/-- A conditional branch entry: sources, the target's meaning, the decode fact. -/
def BrOK (pc w : Nat) (op : bop) (a b tgt : Nat) : Prop :=
  a ≤ 31 ∧ b ≤ 31 ∧ tgt < 2 ^ 64 ∧ tgt % 4 = 0 ∧ ∃ off : BitVec 13,
    BitVec.ofNat 64 pc + sign_extend (m := 64) off = BitVec.ofNat 64 tgt ∧
    DecodeFactT (mkT pc w (.br op true) a b off 0 0)

/-- The decode fact of a `jal` entry. -/
abbrev JalDec (w rd : Nat) (off : BitVec 21) : Prop :=
  ∀ s : SequentialState RegisterType trivialChoiceSource,
    s.regs.get? Register.misa = some ((Vsa.Sim.initMisa) : RegisterType Register.misa) →
    s.regs.get? Register.cur_privilege =
      some ((Privilege.Machine) : RegisterType Register.cur_privilege) →
    s.regs.get? Register.mseccfg = some ((0#64) : RegisterType Register.mseccfg) →
    (ext_decode (BitVec.ofNat 32 w)).run s = .ok (instruction.JAL (off, gprIdx rd)) s

/-- The decode fact of a `jalr` entry. -/
abbrev JalrDec (w rd rs1 imm : Nat) : Prop :=
  ∀ s : SequentialState RegisterType trivialChoiceSource,
    s.regs.get? Register.misa = some ((Vsa.Sim.initMisa) : RegisterType Register.misa) →
    s.regs.get? Register.cur_privilege =
      some ((Privilege.Machine) : RegisterType Register.cur_privilege) →
    s.regs.get? Register.mseccfg = some ((0#64) : RegisterType Register.mseccfg) →
    (ext_decode (BitVec.ofNat 32 w)).run s =
      .ok (instruction.JALR (BitVec.ofNat 12 imm, gprIdx rs1, gprIdx rd)) s

/-- Sail decodes the entry's word to the instruction the evaluator runs (per
kind: register ranges, the immediate's meaning, the decode fact). Kinds
outside the proved fragment are `False` for now. -/
def DecOK (pc w : Nat) : Ins → Prop
  | .addi rd rs1 imm => (1 ≤ rd ∧ rd ≤ 31) ∧ rs1 ≤ 31 ∧ Imm12 imm ∧
      DecodeFactM (mkM pc w .addi rd rs1 0 imm)
  | .add rd rs1 rs2 => (1 ≤ rd ∧ rd ≤ 31) ∧ rs1 ≤ 31 ∧ rs2 ≤ 31 ∧
      DecodeFactM (mkM pc w .add rd rs1 rs2 0)
  | .ld rd rs1 imm => (1 ≤ rd ∧ rd ≤ 31) ∧ rs1 ≤ 31 ∧ Imm12 imm ∧
      DecodeFactM (mkM pc w .ld rd rs1 0 imm)
  | .lbu rd rs1 imm => (1 ≤ rd ∧ rd ≤ 31) ∧ rs1 ≤ 31 ∧ Imm12 imm ∧
      DecodeFactM (mkM pc w .lbu rd rs1 0 imm)
  | .li rd v => (1 ≤ rd ∧ rd ≤ 31) ∧
      ((DecodeFactM (mkM pc w .lui rd 0 0 0) ∧ wvalM (mkM pc w .lui rd 0 0 0) [] [] = BitVec.ofNat 64 v) ∨
       (DecodeFactM (mkM pc w .auipc rd 0 0 0) ∧
         wvalM (mkM pc w .auipc rd 0 0 0) [] [] = BitVec.ofNat 64 v))
  | .sd rs1 rs2 imm => rs1 ≤ 31 ∧ rs2 ≤ 31 ∧ Imm12 imm ∧
      DecodeFactM (mkM pc w .sd 0 rs1 rs2 imm)
  | .jal rd tgt => rd ≤ 31 ∧ tgt < 2 ^ 64 ∧ tgt % 4 = 0 ∧ ∃ off : BitVec 21,
      BitVec.ofNat 64 pc + sign_extend (m := 64) off = BitVec.ofNat 64 tgt ∧ JalDec w rd off
  | .jalr rd rs1 imm => rd ≤ 31 ∧ rs1 ≤ 31 ∧ Imm12 imm ∧ JalrDec w rd rs1 imm
  | .beq a b tgt => BrOK pc w .BEQ a b tgt
  | .bne a b tgt => BrOK pc w .BNE a b tgt
  | _ => False

structure EntryOK (c : Code) (pc w : Nat) (i : Ins) : Prop where
  fetch : Fetchable c pc w
  dec : DecOK pc w i

/-- The static correctness of the code table. -/
def TableOK (c : Code) : Prop := ∀ pc w i, c.table.getW pc = some (w, i) → EntryOK c pc w i

/-- The table's bytes are in memory. -/
def CodeIn (c : Code) (m : Std.ExtHashMap Nat (BitVec 8)) : Prop :=
  ∀ pc w i, c.table.getW pc = some (w, i) →
    m[pc]? = some (wbyte w 0) ∧ m[pc + 1]? = some (wbyte w 1) ∧
    m[pc + 2]? = some (wbyte w 2) ∧ m[pc + 3]? = some (wbyte w 3)

/-- The simulation relation. -/
structure Sim (c : Code) (s : St) (cf : Config) : Prop where
  good : GoodState cf.σ
  pc : cf.σ.regs.get? Register.PC = some (BitVec.ofNat 64 s.pc)
  regs : GHolds cf.σ (pinsOf s.regs)
  x0 : getR s.regs 0 = 0
  mem : ∀ a b, s.mem.byte a = some b → (cf.σ.mem[a]?).getD 0 = BitVec.ofNat 8 b
  code : CodeIn c cf.σ.mem
  console : output cf.σ = ""
  payload : cf.σ.regs.get? Register.htif_payload_writes = some 0#4
  tick : cf.tick < 2

/-- From `cf` the machine runs to a configuration related to `t`. -/
inductive Reaches (c : Code) (cf : Config) (t : St) : Prop where
  | intro (cf' : Config) (steps : Steps cf cf') (sim : Sim c t cf')

/-- What one straight-line instruction's step gives (`block_mem_sound`). -/
inductive MPost (cf : Config) (a : MInstr) (L : GRegs) (bs : List (BitVec 8)) : Prop where
  | intro (σ' : MState) (t' : Nat) (steps : Steps cf ⟨σ', t', cf.steps + 1⟩) (tick : t' < 2)
      (good : GoodState σ') (mem : σ'.mem = writeLog cf.σ.mem (wlogM [a] L [bs]))
      (console : output σ' = "") (pc : σ'.regs.get? Register.PC = some (BitVec.addInt a.pc 4))
      (regs : GHolds σ' (runGM [a] L [bs]))
      (payload : σ'.regs.get? Register.htif_payload_writes = some 0#4)

/-! ## One straight-line instruction (`block_mem_sound` on a singleton block) -/

theorem pc_toNat {pc : Nat} (h : FetchOK pc) : (BitVec.ofNat 64 pc).toNat = pc := by
  simp only [BitVec.toNat_ofNat]
  exact Nat.mod_eq_of_lt (by unfold FetchOK at h; omega)

theorem payload_ne_gpr (n : Nat) : (gprReg n == Register.htif_payload_writes) = false :=
  gprReg_ne_payload n

theorem payload_not_noise : ∀ rr ∈ noiseRegs, (rr == Register.htif_payload_writes) = false := by
  decide

/-- `block_mem_sound` for one instruction of an entry, from the relation. -/
theorem mstep {c : Code} {s : St} {cf : Config} (hS : Sim c s cf) {w : Nat} {a : MInstr}
    (hf : Fetchable c s.pc w) (hapc : a.pc = BitVec.ofNat 64 s.pc) (hw : a.word = BitVec.ofNat 32 w)
    (hb : a.b0 = wbyte w 0 ∧ a.b1 = wbyte w 1 ∧ a.b2 = wbyte w 2 ∧ a.b3 = wbyte w 3)
    (hcode : cf.σ.mem[s.pc]? = some (wbyte w 0) ∧ cf.σ.mem[s.pc + 1]? = some (wbyte w 1) ∧
      cf.σ.mem[s.pc + 2]? = some (wbyte w 2) ∧ cf.σ.mem[s.pc + 3]? = some (wbyte w 3))
    (hdec : DecodeFactM a) (hk : KindOK (keysG (pinsOf s.regs)) a.kind a.rd a.rs1 a.rs2)
    {bs : List (BitVec 8)} (hmem : MemFacts cf.σ.mem (pinsOf s.regs) bs a) :
    MPost cf a (pinsOf s.regs) bs := by
  obtain ⟨σ, t, u⟩ := cf
  obtain ⟨hb0, hb1, hb2, hb3⟩ := hb
  obtain ⟨vm, hmi⟩ := hS.good.minstret
  have hpcn := pc_toNat hf.fetch
  obtain ⟨σ', t', hs, ht', hG', hmem', hout, hpc', _, hregs, hframe⟩ :=
    block_mem_sound [a] σ t u (BitVec.ofNat 64 s.pc) vm (pinsOf s.regs) [bs] hS.good hS.pc hmi
      hS.regs (keysOK_pinsOf _)
      ⟨⟨by simpa only [hapc, hpcn, hb0] using hcode.1,
         by simpa only [hapc, hpcn, hb1] using hcode.2.1,
         by simpa only [hapc, hpcn, hb2] using hcode.2.2.1,
         by simpa only [hapc, hpcn, hb3] using hcode.2.2.2⟩,
       hdec, hmem, trivial⟩
      ⟨⟨by rw [hapc], by rw [hb0, hb1, hb2, hb3, hf.word, hw], by rw [hb0, hb1, hb2, hb3, hf.word, hf.rvc],
        by rw [hapc, hpcn]; exact hf.base, by rw [hapc, hpcn]; exact hf.fetch,
        by rw [hapc, hpcn]; exact hf.align, hk⟩, trivial⟩ hS.tick
  refine ⟨σ', t', hs, ht', hG', hmem', ?_, by simpa [endPCM, hapc] using hpc', hregs, ?_⟩
  · simp only [output] at hS ⊢; rw [hout]; exact hS.console
  · rw [hframe _ payload_not_noise (fun n _ => payload_ne_gpr n)]; exact hS.payload

/-! ## ALU instructions -/

theorem addInt_ofNat (pc : Nat) : BitVec.addInt (BitVec.ofNat 64 pc) 4 = BitVec.ofNat 64 ((pc + 4) % W) := by
  apply BitVec.eq_of_toNat_eq
  simp only [BitVec.addInt, BitVec.toNat_add, BitVec.toNat_ofNat, BitVec.toNat_ofInt]
  have : ((4 : Int) % ((2 ^ 64 : Nat) : Int)).toNat = 4 := by decide
  rw [this]; simp only [W]; omega

theorem setR_mod (R rd v : Nat) : setR R rd (v % 2 ^ 64) = setR R rd v := by
  unfold setR; split
  · rfl
  · simp [updAt, Nat.mod_mod]

/-- An instruction writing `x_rd := v` and nothing else (no memory). -/
theorem alu_sound {c : Code} {s : St} {cf : Config} (hS : Sim c s cf) {w : Nat}
    (hf : Fetchable c s.pc w) {k : MKind} {rd rs1 rs2 imm v : Nat}
    (hdec : DecodeFactM (mkM s.pc w k rd rs1 rs2 imm))
    (hk : KindOK (keysG (pinsOf s.regs)) k rd rs1 rs2)
    (hmf : MemFacts cf.σ.mem (pinsOf s.regs) [] (mkM s.pc w k rd rs1 rs2 imm))
    (hrd : 1 ≤ rd ∧ rd ≤ 31)
    (hgm : stepGM (mkM s.pc w k rd rs1 rs2 imm) (pinsOf s.regs) [] =
      (rd, BitVec.ofNat 64 v) :: eraseG rd (pinsOf s.regs))
    (hlog : wlogM [mkM s.pc w k rd rs1 rs2 imm] (pinsOf s.regs) [[]] = [])
    {i : Ins} (hget : c.table.getW s.pc = some (w, i)) :
    Reaches c cf ⟨(s.pc + 4) % W, setR s.regs rd v, s.mem⟩ := by
  obtain ⟨σ', t', hs, ht', hG', hmem', hout, hpc', hregs, hpay⟩ :=
    mstep hS hf rfl rfl ⟨rfl, rfl, rfl, rfl⟩ (hS.code _ _ _ hget) hdec hk hmf
  have hregs' : GHolds σ' ((rd, BitVec.ofNat 64 v) :: eraseG rd (pinsOf s.regs)) := by
    have : runGM [mkM s.pc w k rd rs1 rs2 imm] (pinsOf s.regs) [[]] =
        stepGM (mkM s.pc w k rd rs1 rs2 imm) (pinsOf s.regs) [] := rfl
    rw [this, hgm] at hregs; exact hregs
  have hm : σ'.mem = cf.σ.mem := by rw [hmem', hlog]; rfl
  refine ⟨_, hs, ⟨hG', ?_, ?_, getR_setR_zero _ hS.x0, ?_, ?_, hout, hpay, ht'⟩⟩
  · rw [hpc']; simp only [mkM]; rw [addInt_ofNat]
  · have := pins_setR hrd.1 hrd.2 hregs'
    rwa [BitVec.toNat_ofNat, setR_mod] at this
  · intro a b hb; rw [hm]; exact hS.mem a b hb
  · rw [hm]; exact hS.code

theorem srcOK_of {R n : Nat} (h : n ≤ 31) : SrcOK n (keysG (pinsOf R)) := by
  refine ⟨h, ?_⟩
  by_cases h0 : n = 0
  · exact Or.inl h0
  · exact Or.inr (mem_keys_pinsOf.2 ⟨by omega, h⟩)

theorem ofNat_add_mod (x y : Nat) :
    BitVec.ofNat 64 x + BitVec.ofNat 64 y = BitVec.ofNat 64 ((x + y) % W) := by
  apply BitVec.eq_of_toNat_eq
  simp only [BitVec.toNat_add, BitVec.toNat_ofNat, W]; omega

theorem sound_addi {c : Code} (hT : TableOK c) {s : St} {cf : Config} (hS : Sim c s cf)
    {w rd rs1 imm : Nat} (hget : c.table.getW s.pc = some (w, .addi rd rs1 imm)) :
    Reaches c cf ⟨(s.pc + 4) % W, setR s.regs rd (fAdd (getR s.regs rs1) imm), s.mem⟩ := by
  obtain ⟨hf, hrd, hrs1, himm, hdec⟩ := hT _ _ _ hget
  refine alu_sound hS hf hdec ⟨hrd, srcOK_of hrs1⟩ trivial hrd ?_ rfl hget
  show (rd, srcVal rs1 (pinsOf s.regs) + sign_extend (m := 64) (BitVec.ofNat 12 imm)) :: _ = _
  rw [himm, srcVal_pinsOf hS.x0 hrs1, ofNat_add_mod]; rfl

theorem sound_add {c : Code} (hT : TableOK c) {s : St} {cf : Config} (hS : Sim c s cf)
    {w rd rs1 rs2 : Nat} (hget : c.table.getW s.pc = some (w, .add rd rs1 rs2)) :
    Reaches c cf ⟨(s.pc + 4) % W, setR s.regs rd (fAdd (getR s.regs rs1) (getR s.regs rs2)), s.mem⟩ := by
  obtain ⟨hf, hrd, hrs1, hrs2, hdec⟩ := hT _ _ _ hget
  refine alu_sound hS hf hdec ⟨hrd, srcOK_of hrs1, srcOK_of hrs2⟩ trivial hrd ?_ rfl hget
  show (rd, srcVal rs1 (pinsOf s.regs) + srcVal rs2 (pinsOf s.regs)) :: _ = _
  rw [srcVal_pinsOf hS.x0 hrs1, srcVal_pinsOf hS.x0 hrs2, ofNat_add_mod]; rfl

/-! ## Terminators: branches, `j`, `jr` (`term_step_bt`) -/

/-- `term_step_bt` for a terminator of an entry, from the relation: the
registers and memory stay, the PC moves to `tgtPCT`. -/
theorem tstep {c : Code} {s : St} {cf : Config} (hS : Sim c s cf) {w : Nat}
    (hf : Fetchable c s.pc w) {i : Ins} (hget : c.table.getW s.pc = some (w, i))
    {k : TKind} {rs1 rs2 : Nat} {i13 : BitVec 13} {i21 : BitVec 21} {i12 : BitVec 12}
    (hdec : DecodeFactT (mkT s.pc w k rs1 rs2 i13 i21 i12))
    (hk : TermKindOK (keysG (pinsOf s.regs)) (BitVec.ofNat 64 s.pc) rs1 rs2 i13 i21 k)
    (htf : TermFactsT (pinsOf s.regs) (mkT s.pc w k rs1 rs2 i13 i21 i12))
    {pc' : Nat} (hpc' : tgtPCT (mkT s.pc w k rs1 rs2 i13 i21 i12) (pinsOf s.regs) = BitVec.ofNat 64 pc') :
    Reaches c cf ⟨pc', s.regs, s.mem⟩ := by
  obtain ⟨σ, t, u⟩ := cf
  obtain ⟨vm, hmi⟩ := hS.good.minstret
  have hpcn := pc_toNat hf.fetch
  have hcode := hS.code _ _ _ hget
  obtain ⟨σ', t', hs, ht', hG', hmem', hout, hpc, _, hregs, hframe⟩ :=
    term_step_bt (mkT s.pc w k rs1 rs2 i13 i21 i12) σ t u vm (pinsOf s.regs) (keysG (pinsOf s.regs))
      hS.good hS.pc hmi hS.regs (keysOK_pinsOf _) (fun _ h => h)
      (by simpa only [mkT, hpcn] using hcode.1) (by simpa only [mkT, hpcn] using hcode.2.1)
      (by simpa only [mkT, hpcn] using hcode.2.2.1) (by simpa only [mkT, hpcn] using hcode.2.2.2)
      hdec
      ⟨by simp only [mkT]; rw [hf.word], by simp only [mkT]; rw [hf.word, hf.rvc],
        by simp only [mkT, hpcn]; exact hf.base, by simp only [mkT, hpcn]; exact hf.fetch,
        by simp only [mkT, hpcn]; exact hf.align, hk⟩
      htf hS.tick
  refine ⟨_, Steps.head hs (Steps.refl _), ⟨hG', by rw [hpc, hpc'], hregs, hS.x0, ?_, ?_,
    ?_, ?_, ht'⟩⟩
  · intro a b hb; rw [hmem']; exact hS.mem a b hb
  · rw [hmem']; exact hS.code
  · simp only [output] at hS ⊢; rw [hout]; exact hS.console
  · rw [hframe _ payload_not_noise]; exact hS.payload

theorem ofNat_inj64 {x y : Nat} (hx : x < 2 ^ 64) (hy : y < 2 ^ 64) :
    (BitVec.ofNat 64 x == BitVec.ofNat 64 y) = (x == y) := by
  by_cases h : x = y
  · subst h; simp
  · have : BitVec.ofNat 64 x ≠ BitVec.ofNat 64 y := by
      intro he
      have := congrArg BitVec.toNat he
      simp only [BitVec.toNat_ofNat, Nat.mod_eq_of_lt hx, Nat.mod_eq_of_lt hy] at this
      exact h this
    rw [beq_eq_false_iff_ne.2 this, beq_eq_false_iff_ne.2 h]

/-- A conditional branch whose guard the evaluator computes as `f`. -/
theorem branch_sound {c : Code} {s : St} {cf : Config} (hS : Sim c s cf) {w : Nat}
    (hf : Fetchable c s.pc w) {i : Ins} (hget : c.table.getW s.pc = some (w, i))
    {op : bop} {f : Nat → Nat → Bool}
    (hop : ∀ x y, x < 2 ^ 64 → y < 2 ^ 64 → guardB op (BitVec.ofNat 64 x) (BitVec.ofNat 64 y) = f x y)
    {rs1 rs2 tgt : Nat} {off : BitVec 13} (h1 : rs1 ≤ 31) (h2 : rs2 ≤ 31)
    (htgt : BitVec.ofNat 64 s.pc + sign_extend (m := 64) off = BitVec.ofNat 64 tgt)
    (hlt : tgt < 2 ^ 64) (hal : tgt % 4 = 0)
    (hdec : DecodeFactT (mkT s.pc w (.br op true) rs1 rs2 off 0 0)) :
    Reaches c cf (br s (f (getR s.regs rs1) (getR s.regs rs2)) tgt) := by
  have hg := hop _ _ (getR_lt s.regs rs1) (getR_lt s.regs rs2)
  rw [← srcVal_pinsOf hS.x0 h1, ← srcVal_pinsOf hS.x0 h2] at hg
  have hal' : (BitVec.ofNat 64 s.pc + sign_extend (m := 64) off).toNat % 4 = 0 := by
    rw [htgt, BitVec.toNat_ofNat, Nat.mod_eq_of_lt hlt]; exact hal
  unfold br
  cases hft : f (getR s.regs rs1) (getR s.regs rs2) with
  | true =>
    simp only [if_true]
    exact tstep hS hf hget (k := .br op true) (i13 := off) (i21 := 0) (i12 := 0) hdec
      ⟨srcOK_of h1, srcOK_of h2, hal'⟩ (by simp only [TermFactsT, mkT]; rw [hg, hft])
      (by simp only [tgtPCT, tgtPC0, mkT]; exact htgt)
  | false =>
    simp only [Bool.false_eq_true, if_false, next]
    exact tstep hS hf hget (k := .br op false) (i13 := off) (i21 := 0) (i12 := 0) hdec
      ⟨srcOK_of h1, srcOK_of h2, hal'⟩ (by simp only [TermFactsT, mkT]; rw [hg, hft])
      (by simp only [tgtPCT, tgtPC0, mkT]; exact addInt_ofNat s.pc)

theorem sound_br {c : Code} (hT : TableOK c) {s : St} {cf : Config} (hS : Sim c s cf)
    {w a b tgt : Nat} {i : Ins} (hget : c.table.getW s.pc = some (w, i)) {op : bop}
    (hbr : BrOK s.pc w op a b tgt) {f : Nat → Nat → Bool}
    (hop : ∀ x y, x < 2 ^ 64 → y < 2 ^ 64 → guardB op (BitVec.ofNat 64 x) (BitVec.ofNat 64 y) = f x y) :
    Reaches c cf (br s (f (getR s.regs a) (getR s.regs b)) tgt) := by
  obtain ⟨ha, hb, hlt, hal, off, htgt, hdec⟩ := hbr
  exact branch_sound hS (hT _ _ _ hget).fetch hget hop ha hb htgt hlt hal hdec

theorem guard_beq (x y : Nat) (hx : x < 2 ^ 64) (hy : y < 2 ^ 64) :
    guardB .BEQ (BitVec.ofNat 64 x) (BitVec.ofNat 64 y) = bEq x y := by
  simp only [guardB, bEq]; exact ofNat_inj64 hx hy

theorem guard_bne (x y : Nat) (hx : x < 2 ^ 64) (hy : y < 2 ^ 64) :
    guardB .BNE (BitVec.ofNat 64 x) (BitVec.ofNat 64 y) = bNe x y := by
  simp only [guardB, bNe, bne, ofNat_inj64 hx hy]

/-! ## Loads and stores -/

/-- Byte `k` of a value. -/
abbrev vbyte (v k : Nat) : BitVec 8 := BitVec.ofNat 8 (fldAt (8 * k) 8 v)

theorem bytes8 (v : Nat) :
    (((((((vbyte v 7).append (vbyte v 6)).append (vbyte v 5)).append (vbyte v 4)).append
      (vbyte v 3)).append (vbyte v 2)).append (vbyte v 1)).append (vbyte v 0) = BitVec.ofNat 64 v := by
  apply BitVec.eq_of_getElem_eq
  intro i hi
  simp only [BitVec.append_eq, BitVec.getElem_append]
  simp only [BitVec.getElem_eq_testBit_toNat, BitVec.toNat_ofNat, Nat.testBit_mod_two_pow,
    testBit_fldAt]
  have hi' : i < 64 := hi
  repeat' split
  all_goals first
    | (simp only [show i - 8 - 8 - 8 - 8 - 8 - 8 - 8 < 8 by omega, decide_true, Bool.true_and, hi'];
       congr 1; omega)
    | (simp only [decide_eq_true_eq, hi', decide_true, Bool.true_and, *]; congr 1; omega)
    | omega

/-- The effective address of a load/store entry. -/
theorem eaddr_mkM {R : Nat} (hx0 : getR R 0 = 0) {pc w : Nat} {k : MKind} {rd rs1 rs2 imm : Nat}
    (h1 : rs1 ≤ 31) (himm : Imm12 imm) :
    eaddrM (mkM pc w k rd rs1 rs2 imm) (pinsOf R) = BitVec.ofNat 64 ((getR R rs1 + imm) % W) := by
  simp only [eaddrM, mkM]; rw [himm, srcVal_pinsOf hx0 h1, ofNat_add_mod]

theorem ea_toNat (a : Nat) : (BitVec.ofNat 64 (a % W)).toNat = a % W := by
  simp only [BitVec.toNat_ofNat, W]; omega

theorem loadOK_iff {a w : Nat} (h : loadOK a w = true) :
    0x80000000 ≤ a ∧ a + w ≤ 0x100000000 ∧ (a + w ≤ tohostAddr ∨ tohostAddr + 8 ≤ a) := by
  simp only [loadOK, Bool.and_eq_true, Bool.or_eq_true, decide_eq_true_eq] at h
  exact ⟨h.1.1, h.1.2, h.2⟩

theorem storeOK_iff {c : Code} {a w : Nat} (h : storeOK c a w = true) :
    0x80000000 ≤ a ∧ a + w ≤ 0x100000000 ∧ StoreWin a ∧ a % w = 0 ∧ (a + w ≤ c.lo ∨ c.hi ≤ a) := by
  simp only [storeOK, Bool.and_eq_true, Bool.or_eq_true, decide_eq_true_eq, beq_iff_eq] at h
  exact ⟨h.1.1.1.1, h.1.1.1.2, h.1.1.2, h.1.2, h.2⟩

/-- `ld rd, imm(rs1)`. -/
theorem sound_ld {c : Code} (hT : TableOK c) {s s' : St} {cf : Config} (hS : Sim c s cf)
    {w rd rs1 imm : Nat} (hget : c.table.getW s.pc = some (w, .ld rd rs1 imm))
    (hex : exec (.ld rd rs1 imm) c s = some s') :
    Reaches c cf s' := by
  obtain ⟨hf, hrd, hrs1, himm, hdec⟩ := hT _ _ _ hget
  simp only [exec, opLd, ldI] at hex
  generalize ha : (getR s.regs rs1 + imm) % W = a at hex
  cases hok : loadOK a 8 with
  | false => simp [hok] at hex
  | true =>
    cases hl : load s.mem a 8 with
    | none => simp [hok, hl] at hex
    | some v =>
      simp only [hok, hl, Option.some.injEq, id] at hex
      subst hex
      show Reaches c cf ⟨(s.pc + 4) % W, setR s.regs rd v, s.mem⟩
      obtain ⟨hv, hbytes⟩ := load_bytes hl
      have hea := eaddr_mkM (k := .ld) (pc := s.pc) (w := w) (rd := rd) (rs2 := 0) hS.x0 hrs1 himm
      rw [ha] at hea
      have heat : (eaddrM (mkM s.pc w .ld rd rs1 0 imm) (pinsOf s.regs)).toNat = a := by
        rw [hea, ← ha, ea_toNat]
      have hpin : ∀ k, k < 8 → (cf.σ.mem[a + k]?).getD 0 = vbyte v k := fun k hk =>
        hS.mem _ _ (hbytes k hk)
      let bs : List (BitVec 8) := [vbyte v 0, vbyte v 1, vbyte v 2, vbyte v 3, vbyte v 4,
        vbyte v 5, vbyte v 6, vbyte v 7]
      have hmf : MemFacts cf.σ.mem (pinsOf s.regs) bs (mkM s.pc w .ld rd rs1 0 imm) := by
        show (_ ∧ _ ∧ _) ∧ LPins8 _ _ _
        rw [heat]
        refine ⟨loadOK_iff hok, ?_, hpin 1 (by omega), hpin 2 (by omega), hpin 3 (by omega),
          hpin 4 (by omega), hpin 5 (by omega), hpin 6 (by omega), hpin 7 (by omega)⟩
        exact (by simpa using hpin 0 (by omega) : (cf.σ.mem[a]?).getD 0 = vbyte v 0)
      obtain ⟨σ', t', hs, ht', hG', hmem', hout, hpc', hregs, hpay⟩ :=
        mstep hS hf rfl rfl ⟨rfl, rfl, rfl, rfl⟩ (hS.code _ _ _ hget) hdec ⟨hrd, srcOK_of hrs1⟩ hmf
      have hval : wvalM (mkM s.pc w .ld rd rs1 0 imm) (pinsOf s.regs) bs = BitVec.ofNat 64 v := by
        show sign_extend (m := 64) _ = _
        simp only [List.getD, bs, List.getElem?_cons_succ, List.getElem?_cons_zero, Option.getD_some]
        rw [bytes8]; simp [sign_extend, Sail.BitVec.signExtend]
      have hregs' : GHolds σ' ((rd, BitVec.ofNat 64 v) :: eraseG rd (pinsOf s.regs)) := by
        have : runGM [mkM s.pc w .ld rd rs1 0 imm] (pinsOf s.regs) [bs] =
            (rd, wvalM (mkM s.pc w .ld rd rs1 0 imm) (pinsOf s.regs) bs) :: eraseG rd (pinsOf s.regs) := rfl
        rw [this, hval] at hregs; exact hregs
      have hm : σ'.mem = cf.σ.mem := by rw [hmem']; rfl
      refine ⟨_, hs, ⟨hG', ?_, ?_, getR_setR_zero _ hS.x0, ?_, ?_, hout, hpay, ht'⟩⟩
      · rw [hpc']; simp only [mkM]; rw [addInt_ofNat]
      · have := pins_setR hrd.1 hrd.2 hregs'
        rwa [BitVec.toNat_ofNat, setR_mod] at this
      · intro x b hb; rw [hm]; exact hS.mem x b hb
      · rw [hm]; exact hS.code

theorem sd_byte (v j : Nat) (hj : j < 8) :
    (sdData_val (BitVec.ofNat 64 v)).extractLsb' (8 * j) 8 = BitVec.ofNat 8 (fldAt (8 * j) 8 v) := by
  apply BitVec.eq_of_toNat_eq
  simp only [BitVec.extractLsb'_toNat, sdData_val, Sail.BitVec.extractLsb, BitVec.extractLsb_toNat,
    BitVec.toNat_ofNat]
  show _ = fldAt (8 * j) 8 v % 2 ^ 8
  have h88 : ((8 * 8 - 1 : Int)).toNat = 63 := by decide
  simp only [h88]
  have e1 : (v % 2 ^ 64) >>> 0 % 2 ^ (63 - 0 + 1) = fldAt 0 64 v := by
    simp [fldAt, Nat.mod_mod]
  rw [e1, Nat.mod_eq_of_lt (fldAt_lt _ _ _)]
  show fldAt (8 * j) 8 (fldAt 0 64 v) = _
  rw [fldAt_fldAt v (by omega), Nat.zero_add]

/-- The relation's memory part and the code survive a store the evaluator
mirrors: `applyW` writes exactly the evaluator's new bytes, off the code. -/
theorem mem_after_store {c : Code} {M M' : MT} {a wd v : Nat} {σm : Std.ExtHashMap Nat (BitVec 8)}
    {e : WEntry} (hst : store M a wd v = some M')
    (hrel : ∀ x b, M.byte x = some b → (σm[x]?).getD 0 = BitVec.ofNat 8 b)
    (hin : ∀ x, a ≤ x → x < a + wd → entryByte e x = some (BitVec.ofNat 8 (fldAt (8 * (x - a)) 8 v)))
    (hout : ∀ x, ¬ (a ≤ x ∧ x < a + wd) → entryByte e x = none)
    (hcode : CodeIn c σm) (hoff : a + wd ≤ c.lo ∨ c.hi ≤ a) (hT : TableOK c) :
    (∀ x b, M'.byte x = some b → ((applyW σm e)[x]?).getD 0 = BitVec.ofNat 8 b) ∧
      CodeIn c (applyW σm e) := by
  constructor
  · intro x b hb
    rw [store_bytes hst] at hb
    rw [applyW_get]
    by_cases hx : a ≤ x ∧ x < a + wd
    · rw [if_pos hx] at hb; rw [hin x hx.1 hx.2]; cases hb; rfl
    · rw [if_neg hx] at hb; rw [hout x hx]; exact hrel x b hb
  · intro pc w i hg
    have hf := (hT pc w i hg).fetch
    have hc := hcode pc w i hg
    have hn : ∀ k, k < 4 → entryByte e (pc + k) = none := fun k hk =>
      hout _ (by have := hf.lo; have := hf.hi; omega)
    simp only [applyW_get]
    refine ⟨?_, ?_, ?_, ?_⟩
    · have := hn 0 (by omega); simp only [Nat.add_zero] at this; rw [this]; exact hc.1
    · rw [hn 1 (by omega)]; exact hc.2.1
    · rw [hn 2 (by omega)]; exact hc.2.2.1
    · rw [hn 3 (by omega)]; exact hc.2.2.2

/-- `sd rs2, imm(rs1)`. -/
theorem sound_sd {c : Code} (hT : TableOK c) {s s' : St} {cf : Config} (hS : Sim c s cf)
    {w rs1 rs2 imm : Nat} (hget : c.table.getW s.pc = some (w, .sd rs1 rs2 imm))
    (hex : exec (.sd rs1 rs2 imm) c s = some s') :
    Reaches c cf s' := by
  obtain ⟨hf, hrs1, hrs2, himm, hdec⟩ := hT _ _ _ hget
  simp only [exec, opSt, stI] at hex
  generalize ha : (getR s.regs rs1 + imm) % W = a at hex
  cases hok : storeOK c a 8 with
  | false => simp [hok] at hex
  | true =>
    cases hst : store s.mem a 8 (getR s.regs rs2) with
    | none => simp [hok, hst] at hex
    | some M' =>
      simp only [hok, hst, Option.some.injEq] at hex
      subst hex
      obtain ⟨hlo, hhi, hwin, hal, hoff⟩ := storeOK_iff hok
      have hea := eaddr_mkM (k := .sd) (pc := s.pc) (w := w) (rd := 0) (rs2 := rs2) hS.x0 hrs1 himm
      rw [ha] at hea
      have heat : (eaddrM (mkM s.pc w .sd 0 rs1 rs2 imm) (pinsOf s.regs)).toNat = a := by
        rw [hea, ← ha, ea_toNat]
      have hmf : MemFacts cf.σ.mem (pinsOf s.regs) [] (mkM s.pc w .sd 0 rs1 rs2 imm) := by
        show _ ∧ _ ∧ _ ∧ _
        rw [heat]; exact ⟨hlo, hhi, hwin, hal⟩
      obtain ⟨σ', t', hs, ht', hG', hmem', hout, hpc', hregs, hpay⟩ :=
        mstep hS hf rfl rfl ⟨rfl, rfl, rfl, rfl⟩ (hS.code _ _ _ hget) hdec
          ⟨srcOK_of hrs1, srcOK_of hrs2⟩ hmf
      have hm : σ'.mem = applyW cf.σ.mem (a, 8, BitVec.ofNat 64 (getR s.regs rs2)) := by
        rw [hmem']
        show applyW cf.σ.mem ((eaddrM (mkM s.pc w .sd 0 rs1 rs2 imm) (pinsOf s.regs)).toNat, 8,
          srcVal rs2 (pinsOf s.regs)) = _
        rw [heat, srcVal_pinsOf hS.x0 hrs2]
      obtain ⟨hmem2, hcode2⟩ := mem_after_store (c := c) (e := (a, 8, BitVec.ofNat 64 (getR s.regs rs2))) hst hS.mem
        (fun x h1 h2 => by
          show (if a ≤ x ∧ x < a + 8 then some _ else none) = _
          rw [if_pos ⟨h1, h2⟩, sd_byte _ _ (by omega)])
        (fun x hx => by
          show (if a ≤ x ∧ x < a + 8 then some _ else none) = _
          rw [if_neg hx])
        hS.code hoff hT
      refine ⟨_, hs, ⟨hG', ?_, hregs, hS.x0, ?_, ?_, hout, hpay, ht'⟩⟩
      · rw [hpc']; simp only [mkM]; rw [addInt_ofNat]
      · rw [hm]; exact hmem2
      · rw [hm]; exact hcode2

theorem zext_byte {v : Nat} (hv : v < 2 ^ (8 * 1)) :
    zero_extend (m := 64) ((vbyte v 0 : BitVec 8) : BitVec (8 * 1)) = BitVec.ofNat 64 v := by
  apply BitVec.eq_of_toNat_eq
  simp only [zero_extend, Sail.BitVec.zeroExtend, BitVec.toNat_setWidth, BitVec.toNat_ofNat, fldAt]
  omega

/-- `lbu rd, imm(rs1)`. -/
theorem sound_lbu {c : Code} (hT : TableOK c) {s s' : St} {cf : Config} (hS : Sim c s cf)
    {w rd rs1 imm : Nat} (hget : c.table.getW s.pc = some (w, .lbu rd rs1 imm))
    (hex : exec (.lbu rd rs1 imm) c s = some s') :
    Reaches c cf s' := by
  obtain ⟨hf, hrd, hrs1, himm, hdec⟩ := hT _ _ _ hget
  simp only [exec, opLd, ldI] at hex
  generalize ha : (getR s.regs rs1 + imm) % W = a at hex
  cases hok : loadOK a 1 with
  | false => simp [hok] at hex
  | true =>
    cases hl : load s.mem a 1 with
    | none => simp [hok, hl] at hex
    | some v =>
      simp only [hok, hl, Option.some.injEq, id] at hex
      subst hex
      show Reaches c cf ⟨(s.pc + 4) % W, setR s.regs rd v, s.mem⟩
      obtain ⟨hv, hbytes⟩ := load_bytes hl
      have hea := eaddr_mkM (k := .lbu) (pc := s.pc) (w := w) (rd := rd) (rs2 := 0) hS.x0 hrs1 himm
      rw [ha] at hea
      have heat : (eaddrM (mkM s.pc w .lbu rd rs1 0 imm) (pinsOf s.regs)).toNat = a := by
        rw [hea, ← ha, ea_toNat]
      have hpin : (cf.σ.mem[a]?).getD 0 = vbyte v 0 := by
        have := hS.mem _ _ (hbytes 0 (by omega)); simpa using this
      have hmf : MemFacts cf.σ.mem (pinsOf s.regs) [vbyte v 0] (mkM s.pc w .lbu rd rs1 0 imm) := by
        show (_ ∧ _ ∧ _) ∧ _
        rw [heat]
        exact ⟨loadOK_iff hok, hpin⟩
      obtain ⟨σ', t', hs, ht', hG', hmem', hout, hpc', hregs, hpay⟩ :=
        mstep hS hf rfl rfl ⟨rfl, rfl, rfl, rfl⟩ (hS.code _ _ _ hget) hdec ⟨hrd, srcOK_of hrs1⟩ hmf
      have hval : wvalM (mkM s.pc w .lbu rd rs1 0 imm) (pinsOf s.regs) [vbyte v 0] =
          BitVec.ofNat 64 v := zext_byte hv
      have hregs' : GHolds σ' ((rd, BitVec.ofNat 64 v) :: eraseG rd (pinsOf s.regs)) := by
        have : runGM [mkM s.pc w .lbu rd rs1 0 imm] (pinsOf s.regs) [[vbyte v 0]] =
            (rd, wvalM (mkM s.pc w .lbu rd rs1 0 imm) (pinsOf s.regs) [vbyte v 0]) ::
              eraseG rd (pinsOf s.regs) := rfl
        rw [this, hval] at hregs; exact hregs
      have hm : σ'.mem = cf.σ.mem := by rw [hmem']; rfl
      refine ⟨_, hs, ⟨hG', ?_, ?_, getR_setR_zero _ hS.x0, ?_, ?_, hout, hpay, ht'⟩⟩
      · rw [hpc']; simp only [mkM]; rw [addInt_ofNat]
      · have := pins_setR hrd.1 hrd.2 hregs'
        rwa [BitVec.toNat_ofNat, setR_mod] at this
      · intro x b hb; rw [hm]; exact hS.mem x b hb
      · rw [hm]; exact hS.code

/-- `lui rd, imm` / `auipc rd, imm` (the evaluator's `li`, whose value the table fixes). -/
theorem sound_li {c : Code} (hT : TableOK c) {s : St} {cf : Config} (hS : Sim c s cf)
    {w rd v : Nat} (hget : c.table.getW s.pc = some (w, .li rd v)) :
    Reaches c cf ⟨(s.pc + 4) % W, setR s.regs rd v, s.mem⟩ := by
  obtain ⟨hf, hrd, hk⟩ := hT _ _ _ hget
  rcases hk with ⟨hdec, hval⟩ | ⟨hdec, hval⟩
  · exact alu_sound hS hf hdec hrd trivial hrd (by show (rd, _) :: _ = _; rw [← hval]; rfl) rfl hget
  · exact alu_sound hS hf hdec hrd trivial hrd (by show (rd, _) :: _ = _; rw [← hval]; rfl) rfl hget

/-! ## Jumps: `jal` (`j` for `rd = 0`, `stepObs_jal` otherwise) -/

/-- A decode fact at the state the step lemmas decode in, from the σ-generic form. -/
theorem dec_at {σ : MState} (hG : GoodState σ) {w : BitVec 32} {ast : instruction}
    (h : ∀ s : SequentialState RegisterType trivialChoiceSource,
      s.regs.get? Register.misa = some ((Vsa.Sim.initMisa) : RegisterType Register.misa) →
      s.regs.get? Register.cur_privilege =
        some ((Privilege.Machine) : RegisterType Register.cur_privilege) →
      s.regs.get? Register.mseccfg = some ((0#64) : RegisterType Register.mseccfg) →
      (ext_decode w).run s = .ok ast s) :
    (ext_decode w).run (afterPrelude σ) = .ok ast (afterPrelude σ) :=
  h (afterPrelude σ)
    (by rw [get?_afterPrelude σ _ (by decide)]; exact hG.misa)
    (by rw [get?_afterPrelude σ _ (by decide)]; exact hG.cur_privilege)
    (by rw [get?_afterPrelude σ _ (by decide)]; exact hG.mseccfg)

/-- After a step that wrote `x_rd := link` and moved the PC (and the frame
otherwise), the relation holds at the evaluator's linked state. -/
theorem sim_link {c : Code} {s : St} {σ σ' : MState} {t u t' u' : Nat} (hS : Sim c s ⟨σ, t, u⟩)
    {rd : Nat} (hrd : 1 ≤ rd ∧ rd ≤ 31) {link : BitVec 64} {tgt : Nat}
    (hG' : GoodState σ') (ht' : t' < 2) (hmem : σ'.mem = σ.mem)
    (hpc : σ'.regs.get? Register.PC = some (BitVec.ofNat 64 tgt))
    (hrdv : σ'.regs.get? (gprReg rd) = some (gprRT rd link))
    (hgpr : ∀ n, 1 ≤ n → n ≤ 31 → n ≠ rd → σ'.regs.get? (gprReg n) = σ.regs.get? (gprReg n))
    (hout : σ'.sailOutput = σ.sailOutput)
    (hpay : σ'.regs.get? Register.htif_payload_writes = σ.regs.get? Register.htif_payload_writes) :
    Sim c ⟨tgt, setR s.regs rd link.toNat, s.mem⟩ ⟨σ', t', u'⟩ := by
  have hGH : GHolds σ' ((rd, link) :: eraseG rd (pinsOf s.regs)) :=
    ⟨gprGet_of_get rd hrd.1 hrd.2 hrdv, gholds_erase_frame rd hgpr _ (keysOK_pinsOf _) hS.regs⟩
  refine ⟨hG', hpc, pins_setR hrd.1 hrd.2 hGH, getR_setR_zero _ hS.x0, ?_, ?_, ?_, ?_, ht'⟩
  · intro a b hb; simp only; rw [hmem]; exact hS.mem a b hb
  · simp only; rw [hmem]; exact hS.code
  · simp only [output] at hS ⊢; rw [hout]; exact hS.console
  · rw [hpay]; exact hS.payload

theorem sound_jal {c : Code} (hT : TableOK c) {s : St} {cf : Config} (hS : Sim c s cf)
    {w rd tgt : Nat} (hget : c.table.getW s.pc = some (w, .jal rd tgt)) :
    Reaches c cf ⟨tgt, setR s.regs rd ((s.pc + 4) % W), s.mem⟩ := by
  obtain ⟨hf, hrd, hlt, hal, off, htgt, hdec⟩ := hT _ _ _ hget
  have hal' : (BitVec.ofNat 64 s.pc + sign_extend (m := 64) off).toNat % 4 = 0 := by
    rw [htgt, BitVec.toNat_ofNat, Nat.mod_eq_of_lt hlt]; exact hal
  by_cases h0 : rd = 0
  · subst h0
    rw [setR_zero]
    exact tstep hS hf hget (k := .j) (rs1 := 0) (rs2 := 0) (i13 := 0) (i21 := off) (i12 := 0)
      hdec hal' trivial (by simp only [tgtPCT, tgtPC0, mkT]; exact htgt)
  · obtain ⟨σ, t, u⟩ := cf
    have hrd' : 1 ≤ rd ∧ rd ≤ 31 := ⟨by omega, hrd⟩
    obtain ⟨vm, hmi⟩ := hS.good.minstret
    have hpcn := pc_toNat hf.fetch
    have hcode := hS.code _ _ _ hget
    obtain ⟨hr1, hr2, hr3, hr4, hr5⟩ := gpr_rd_ok rd (by omega) hrd'.1
    have hwx := wX_gpr {(afterNextPC (afterPrelude σ) (BitVec.ofNat 64 s.pc)) with
        regs := (afterNextPC (afterPrelude σ) (BitVec.ofNat 64 s.pc)).regs.insert Register.nextPC
          (BitVec.ofNat 64 s.pc + sign_extend (m := 64) off)}
      (BitVec.addInt (BitVec.ofNat 64 s.pc) 4) rd hrd'.1 hrd'.2
    obtain ⟨σ', i', hs, hi', hG', hmem', hobs⟩ :=
      stepObs_jal σ t u (BitVec.ofNat 64 s.pc) vm _ off (gprIdx rd) (gprReg rd)
        (gprRT rd (BitVec.addInt (BitVec.ofNat 64 s.pc) 4)) _ _ _ _ hS.good hS.pc hmi
        (by rw [hpcn]; exact hcode.1) (by rw [hpcn]; exact hcode.2.1)
        (by rw [hpcn]; exact hcode.2.2.1) (by rw [hpcn]; exact hcode.2.2.2)
        (by rw [hpcn]; exact hf.base) (by rw [hpcn]; exact hf.fetch) (by rw [hpcn]; exact hf.align)
        (by rw [hf.word]; exact hf.rvc) hf.word (dec_at hS.good hdec) hal'
        hr1 hr2 hr3 hr4 hr5 hwx hS.tick
    have hgpr : ∀ n, 1 ≤ n → n ≤ 31 → n ≠ rd →
        σ'.regs.get? (gprReg n) = σ.regs.get? (gprReg n) := fun n h1 h31 hne =>
      (hobs.1 _ (notGpr_mcycle n) (notGpr_mtime n) (notGpr_mip n)).trans
        (get?_sigmaPost_jal σ _ vm _ _ _ _ (notGpr_minstret n) (notGpr_PC n)
          (gprReg_beq_false rd (by omega) n (by omega) hrd'.1 h1 (Ne.symm hne))
          (notGpr_nextPC n) (notGpr_minstret_increment n))
    have hrdv : σ'.regs.get? (gprReg rd) =
        some (gprRT rd (BitVec.addInt (BitVec.ofNat 64 s.pc) 4)) := by
      rw [hobs.1 _ (notGpr_mcycle rd) (notGpr_mtime rd) (notGpr_mip rd)]
      show ((((sigma3_jal σ _ off (gprReg rd) _).regs.insert Register.PC _).insert
        Register.minstret _)).get? (gprReg rd) = _
      rw [Std.ExtDHashMap.get?_insert]
      simp only [notGpr_minstret rd, dif_neg, reduceCtorEq, not_false_eq_true]
      rw [Std.ExtDHashMap.get?_insert]
      simp only [notGpr_PC rd, dif_neg, reduceCtorEq, not_false_eq_true]
      show ((_ : Std.ExtDHashMap Register RegisterType).insert (gprReg rd) _).get? (gprReg rd) = _
      rw [Std.ExtDHashMap.get?_insert_self]
    have hpc' : σ'.regs.get? Register.PC = some (BitVec.ofNat 64 tgt) := by
      rw [hobs.1 _ (by decide) (by decide) (by decide)]
      show ((((sigma3_jal σ _ off (gprReg rd) _).regs.insert Register.PC _).insert
        Register.minstret _)).get? Register.PC = _
      rw [Std.ExtDHashMap.get?_insert]
      simp only [show (Register.minstret == Register.PC) = false from by decide, dif_neg,
        reduceCtorEq, not_false_eq_true]
      rw [Std.ExtDHashMap.get?_insert_self, htgt]
    have hpay : σ'.regs.get? Register.htif_payload_writes =
        σ.regs.get? Register.htif_payload_writes := by
      rw [hobs.1 _ (by decide) (by decide) (by decide),
        get?_sigmaPost_jal σ _ vm _ _ _ _ (by decide) (by decide)
          (by rw [Bool.beq_comm]; exact notGpr_payload rd) (by decide) (by decide)]
    have := sim_link hS hrd' hG' hi' hmem' hpc' hrdv hgpr hobs.2 hpay (u' := u + 1)
    rw [addInt_ofNat, BitVec.toNat_ofNat, show (s.pc + 4) % W % 2 ^ 64 = (s.pc + 4) % W by
      simp [W]] at this
    exact ⟨_, Steps.head hs (Steps.refl _), this⟩

/-! ## `jalr` (`jr` for `rd = 0`, `stepObs_jalr` otherwise) -/

theorem clr0 (y : Nat) : y - y % 2 = (y >>> 1) <<< 1 := by
  rw [Nat.shiftLeft_eq, Nat.shiftRight_eq_div_pow]; omega

theorem update0 (x : Nat) :
    (BitVec.update (BitVec.ofNat 64 x) 0 0#1) = BitVec.ofNat 64 (x % W - x % W % 2) := by
  apply BitVec.eq_of_toNat_eq
  simp [BitVec.update, BitVec.updateSubrange']
  apply Nat.eq_of_testBit_eq
  intro i
  have h2 : (18446744073709551614 : Nat) = (2 ^ 63 - 1) <<< 1 := by decide
  simp only [W]
  rw [h2, clr0, show (18446744073709551616 : Nat) = 2 ^ 64 from rfl, Nat.testBit_and,
    Nat.testBit_mod_two_pow, Nat.testBit_mod_two_pow, Nat.testBit_shiftLeft,
    Nat.testBit_shiftLeft, Nat.testBit_two_pow_sub_one, Nat.testBit_shiftRight,
    Nat.testBit_mod_two_pow]
  by_cases h0 : 1 ≤ i
  · by_cases h64 : i < 64
    · simp [h0, h64, show i - 1 < 63 by omega, show 1 + (i - 1) = i by omega]
    · simp [h64, show ¬ i - 1 < 63 by omega]
  · simp [h0]

/-- The `jalr` target the model computes is the evaluator's. -/
theorem jalr_tgt {R : Nat} (hx0 : getR R 0 = 0) {rs1 imm : Nat} (h1 : rs1 ≤ 31) (himm : Imm12 imm) :
    BitVec.update (srcVal rs1 (pinsOf R) + sign_extend (m := 64) (BitVec.ofNat 12 imm)) 0 0#1 =
      BitVec.ofNat 64 ((getR R rs1 + imm) % W - (getR R rs1 + imm) % W % 2) := by
  rw [himm, srcVal_pinsOf hx0 h1, ofNat_add_mod, update0, Nat.mod_mod]

theorem sound_jalr {c : Code} (hT : TableOK c) {s s' : St} {cf : Config} (hS : Sim c s cf)
    {w rd rs1 imm : Nat} (hget : c.table.getW s.pc = some (w, .jalr rd rs1 imm))
    (hex : exec (.jalr rd rs1 imm) c s = some s') :
    Reaches c cf s' := by
  obtain ⟨hf, hrd, hrs1, himm, hdec⟩ := hT _ _ _ hget
  simp only [exec, opJalr] at hex
  generalize ht : (getR s.regs rs1 + imm) % W - (getR s.regs rs1 + imm) % W % 2 = tg at hex
  cases hal : tg % 4 == 0 with
  | false => simp [hal] at hex
  | true =>
    simp only [hal, if_true, Option.some.injEq] at hex
    subst hex
    have hal' : tg % 4 = 0 := by simpa using hal
    have htg := jalr_tgt hS.x0 hrs1 himm (imm := imm)
    rw [ht] at htg
    have htglt : tg < 2 ^ 64 := by
      have := Nat.mod_lt (getR s.regs rs1 + imm) (show 0 < W by decide)
      simp only [W] at this ht; omega
    have hal'' : (BitVec.ofNat 64 tg).toNat % 4 = 0 := by
      rw [BitVec.toNat_ofNat, Nat.mod_eq_of_lt htglt]; exact hal'
    by_cases h0 : rd = 0
    · subst h0
      rw [setR_zero]
      exact tstep hS hf hget (k := .jr) (rs1 := rs1) (rs2 := 0) (i13 := 0) (i21 := 0)
        (i12 := BitVec.ofNat 12 imm) hdec (srcOK_of hrs1)
        (by show (BitVec.update _ 0 0#1).toNat % 4 = 0; simp only [mkT]; rw [htg]; exact hal'')
        (by simp only [tgtPCT, mkT]; exact htg)
    · obtain ⟨σ, t, u⟩ := cf
      have hrd' : 1 ≤ rd ∧ rd ≤ 31 := ⟨by omega, hrd⟩
      obtain ⟨vm, hmi⟩ := hS.good.minstret
      have hpcn := pc_toNat hf.fetch
      have hcode := hS.code _ _ _ hget
      have hrx1 := rX_src σ (BitVec.ofNat 64 s.pc) rs1 hrs1 _
        (srcPin_srcVal σ (pinsOf s.regs) rs1 (by
          by_cases h : rs1 = 0
          · exact Or.inl h
          · exact Or.inr (mem_keys_pinsOf.2 ⟨by omega, hrs1⟩)) hS.regs)
      obtain ⟨hr1, hr2, hr3, hr4, hr5⟩ := gpr_rd_ok rd (by omega) hrd'.1
      have hwx := wX_gpr {(afterNextPC (afterPrelude σ) (BitVec.ofNat 64 s.pc)) with
          regs := (afterNextPC (afterPrelude σ) (BitVec.ofNat 64 s.pc)).regs.insert Register.nextPC
            (BitVec.update (srcVal rs1 (pinsOf s.regs) + sign_extend (m := 64) (BitVec.ofNat 12 imm)) 0 0#1)}
        (BitVec.addInt (BitVec.ofNat 64 s.pc) 4) rd hrd'.1 hrd'.2
      obtain ⟨σ', i', hs, hi', hG', hmem', hobs⟩ :=
        stepObs_jalr σ t u (BitVec.ofNat 64 s.pc) vm (srcVal rs1 (pinsOf s.regs)) _
          (BitVec.ofNat 12 imm) (gprIdx rs1) (gprIdx rd) (gprReg rd)
          (gprRT rd (BitVec.addInt (BitVec.ofNat 64 s.pc) 4)) _ _ _ _ hS.good hS.pc hmi
          (by rw [hpcn]; exact hcode.1) (by rw [hpcn]; exact hcode.2.1)
          (by rw [hpcn]; exact hcode.2.2.1) (by rw [hpcn]; exact hcode.2.2.2)
          (by rw [hpcn]; exact hf.base) (by rw [hpcn]; exact hf.fetch) (by rw [hpcn]; exact hf.align)
          (by rw [hf.word]; exact hf.rvc) hf.word (dec_at hS.good hdec) hrx1 (by rw [htg]; exact hal'')
          hr1 hr2 hr3 hr4 hr5 hwx hS.tick
      have hgpr : ∀ n, 1 ≤ n → n ≤ 31 → n ≠ rd →
          σ'.regs.get? (gprReg n) = σ.regs.get? (gprReg n) := fun n h1 h31 hne =>
        (hobs.1 _ (notGpr_mcycle n) (notGpr_mtime n) (notGpr_mip n)).trans
          (get?_sigmaPost_jalr σ _ vm _ _ _ _ (notGpr_minstret n) (notGpr_PC n)
            (gprReg_beq_false rd (by omega) n (by omega) hrd'.1 h1 (Ne.symm hne))
            (notGpr_nextPC n) (notGpr_minstret_increment n))
      have hrdv : σ'.regs.get? (gprReg rd) =
          some (gprRT rd (BitVec.addInt (BitVec.ofNat 64 s.pc) 4)) := by
        rw [hobs.1 _ (notGpr_mcycle rd) (notGpr_mtime rd) (notGpr_mip rd)]
        show ((((sigma3_jalr σ _ _ (gprReg rd) _).regs.insert Register.PC _).insert
          Register.minstret _)).get? (gprReg rd) = _
        rw [Std.ExtDHashMap.get?_insert]
        simp only [notGpr_minstret rd, dif_neg, reduceCtorEq, not_false_eq_true]
        rw [Std.ExtDHashMap.get?_insert]
        simp only [notGpr_PC rd, dif_neg, reduceCtorEq, not_false_eq_true]
        show ((_ : Std.ExtDHashMap Register RegisterType).insert (gprReg rd) _).get? (gprReg rd) = _
        rw [Std.ExtDHashMap.get?_insert_self]
      have hpc' : σ'.regs.get? Register.PC = some (BitVec.ofNat 64 tg) := by
        rw [hobs.1 _ (by decide) (by decide) (by decide)]
        show ((((sigma3_jalr σ _ _ (gprReg rd) _).regs.insert Register.PC _).insert
          Register.minstret _)).get? Register.PC = _
        rw [Std.ExtDHashMap.get?_insert]
        simp only [show (Register.minstret == Register.PC) = false from by decide, dif_neg,
          reduceCtorEq, not_false_eq_true]
        rw [Std.ExtDHashMap.get?_insert_self, htg]
      have hpay : σ'.regs.get? Register.htif_payload_writes =
          σ.regs.get? Register.htif_payload_writes := by
        rw [hobs.1 _ (by decide) (by decide) (by decide),
          get?_sigmaPost_jalr σ _ vm _ _ _ _ (by decide) (by decide)
            (by rw [Bool.beq_comm]; exact notGpr_payload rd) (by decide) (by decide)]
      have := sim_link hS hrd' hG' hi' hmem' hpc' hrdv hgpr hobs.2 hpay (u' := u + 1)
      rw [addInt_ofNat, BitVec.toNat_ofNat, show (s.pc + 4) % W % 2 ^ 64 = (s.pc + 4) % W by
        simp [W]] at this
      exact ⟨_, Steps.head hs (Steps.refl _), this⟩

/-! ## The generic theorem -/

/-- **One evaluator step is a Sail step.** Every instruction kind `DecOK`
covers is handled; for the others `DecOK` is `False`, so a table that
`TableOK` holds of contains only covered kinds. -/
theorem exec_sound {c : Code} (hT : TableOK c) {s s' : St} {cf : Config} (hS : Sim c s cf)
    {w : Nat} {i : Ins} (hget : c.table.getW s.pc = some (w, i)) (hex : exec i c s = some s') :
    Reaches c cf s' := by
  have hok := (hT _ _ _ hget).dec
  cases i
  case li rd v =>
    simp only [exec, opLi, Option.some.injEq] at hex; subst hex; exact sound_li hT hS hget
  case jal rd tgt =>
    simp only [exec, opJal, Option.some.injEq] at hex; subst hex; exact sound_jal hT hS hget
  case jalr rd rs1 imm => exact sound_jalr hT hS hget hex
  case beq a b tgt =>
    simp only [exec, bra, Option.some.injEq] at hex; subst hex; exact sound_br hT hS hget hok guard_beq
  case bne a b tgt =>
    simp only [exec, bra, Option.some.injEq] at hex; subst hex; exact sound_br hT hS hget hok guard_bne
  case ld rd rs1 imm => exact sound_ld hT hS hget hex
  case lbu rd rs1 imm => exact sound_lbu hT hS hget hex
  case sd rs1 rs2 imm => exact sound_sd hT hS hget hex
  case addi rd rs1 imm =>
    simp only [exec, alui, wr, next, Option.some.injEq] at hex; subst hex; exact sound_addi hT hS hget
  case add rd rs1 rs2 =>
    simp only [exec, alu, wr, next, Option.some.injEq] at hex; subst hex; exact sound_add hT hS hget
  all_goals exact (hok : False).elim

theorem step_sound {c : Code} (hT : TableOK c) {s s' : St} {cf : Config} (hS : Sim c s cf)
    (h : step c s = some s') : Reaches c cf s' := by
  unfold step at h
  rw [CT.get_getW] at h
  cases hg : c.table.getW s.pc with
  | none => simp [hg] at h
  | some wi =>
    obtain ⟨w, i⟩ := wi
    simp only [hg, Option.map_some] at h
    exact exec_sound hT hS hg h

/-- **A run of the evaluator is a run of the Sail model**, between configurations
the simulation relation ties to its start and end states. -/
theorem run_sound {c : Code} (hT : TableOK c) : ∀ (n : Nat) {s t : St} {cf : Config},
    Sim c s cf → run c n s = some t → Reaches c cf t
  | 0, s, t, cf, hS, h => by
    simp only [run, Option.some.injEq] at h; subst h; exact ⟨cf, Steps.refl _, hS⟩
  | n + 1, s, t, cf, hS, h => by
    simp only [run] at h
    cases hs : step c s with
    | none => simp [hs] at h
    | some s1 =>
      simp only [hs] at h
      obtain ⟨cf1, h1, hS1⟩ := step_sound hT hS hs
      obtain ⟨cf2, h2, hS2⟩ := run_sound hT n hS1 h
      exact ⟨cf2, Steps.trans h1 h2, hS2⟩

end Tcc.Ccp
