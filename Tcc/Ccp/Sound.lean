import Tcc.Ccp.Lemmas
import Tcc.Boot.Step

/-!
# Soundness of the evaluator against the Sail model (CompileCut pilot, P2)

`Sim c ab s cf` relates an evaluator state `s` to a machine configuration `cf`:
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

/-- The B-type immediate of a word. -/
def bimm (w : Nat) : BitVec 13 :=
  BitVec.ofNat 13 ((((w >>> 31) % 2) <<< 12) + (((w >>> 7) % 2) <<< 11) +
    (((w >>> 25) % 64) <<< 5) + (((w >>> 8) % 16) <<< 1))

/-- The J-type immediate of a word. -/
def jimm (w : Nat) : BitVec 21 :=
  BitVec.ofNat 21 ((((w >>> 31) % 2) <<< 20) + (((w >>> 12) % 256) <<< 12) +
    (((w >>> 20) % 2) <<< 11) + (((w >>> 21) % 1024) <<< 1))

/-- A conditional branch entry: sources, the target's meaning, the decode fact. -/
def BrOK (pc w : Nat) (op : bop) (a b tgt : Nat) : Prop :=
  a ≤ 31 ∧ b ≤ 31 ∧ tgt < 2 ^ 64 ∧ tgt % 4 = 0 ∧
    BitVec.ofNat 64 pc + sign_extend (m := 64) (bimm w) = BitVec.ofNat 64 tgt ∧
    DecodeFactT (mkT pc w (.br op true) a b (bimm w) 0 0)

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

/-- σ-generic decode fact for an arbitrary instruction. -/
abbrev ADec (w : Nat) (ast : instruction) : Prop :=
  ∀ s : SequentialState RegisterType trivialChoiceSource,
    s.regs.get? Register.misa = some ((Vsa.Sim.initMisa) : RegisterType Register.misa) →
    s.regs.get? Register.cur_privilege =
      some ((Privilege.Machine) : RegisterType Register.cur_privilege) →
    s.regs.get? Register.mseccfg = some ((0#64) : RegisterType Register.mseccfg) →
    (ext_decode (BitVec.ofNat 32 w)).run s = .ok ast s

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
  | .lw rd rs1 imm => (1 ≤ rd ∧ rd ≤ 31) ∧ rs1 ≤ 31 ∧ Imm12 imm ∧
      DecodeFactM (mkM pc w .lw rd rs1 0 imm)
  | .lwu rd rs1 imm => (1 ≤ rd ∧ rd ≤ 31) ∧ rs1 ≤ 31 ∧ Imm12 imm ∧
      DecodeFactM (mkM pc w .lwu rd rs1 0 imm)
  | .lh rd rs1 imm => (1 ≤ rd ∧ rd ≤ 31) ∧ rs1 ≤ 31 ∧ Imm12 imm ∧
      DecodeFactM (mkM pc w .lh rd rs1 0 imm)
  | .lhu rd rs1 imm => (1 ≤ rd ∧ rd ≤ 31) ∧ rs1 ≤ 31 ∧ Imm12 imm ∧
      DecodeFactM (mkM pc w .lhu rd rs1 0 imm)
  | .li rd v => (1 ≤ rd ∧ rd ≤ 31) ∧
      ((DecodeFactM (mkM pc w .lui rd 0 0 0) ∧ wvalM (mkM pc w .lui rd 0 0 0) [] [] = BitVec.ofNat 64 v) ∨
       (DecodeFactM (mkM pc w .auipc rd 0 0 0) ∧
         wvalM (mkM pc w .auipc rd 0 0 0) [] [] = BitVec.ofNat 64 v))
  | .sw rs1 rs2 imm => rs1 ≤ 31 ∧ rs2 ≤ 31 ∧ Imm12 imm ∧
      DecodeFactM (mkM pc w .sw 0 rs1 rs2 imm)
  | .sh rs1 rs2 imm => rs1 ≤ 31 ∧ rs2 ≤ 31 ∧ Imm12 imm ∧
      DecodeFactM (mkM pc w .sh 0 rs1 rs2 imm)
  | .sb rs1 rs2 imm => rs1 ≤ 31 ∧ rs2 ≤ 31 ∧ Imm12 imm ∧
      DecodeFactM (mkM pc w .sb 0 rs1 rs2 imm)
  | .sd rs1 rs2 imm => rs1 ≤ 31 ∧ rs2 ≤ 31 ∧ Imm12 imm ∧
      DecodeFactM (mkM pc w .sd 0 rs1 rs2 imm)
  | .jal rd tgt => rd ≤ 31 ∧ tgt < 2 ^ 64 ∧ tgt % 4 = 0 ∧
      BitVec.ofNat 64 pc + sign_extend (m := 64) (jimm w) = BitVec.ofNat 64 tgt ∧ JalDec w rd (jimm w)
  | .jalr rd rs1 imm => rd ≤ 31 ∧ rs1 ≤ 31 ∧ Imm12 imm ∧ JalrDec w rd rs1 imm
  | .beq a b tgt => BrOK pc w .BEQ a b tgt
  | .bne a b tgt => BrOK pc w .BNE a b tgt
  | .blt a b tgt => BrOK pc w .BLT a b tgt
  | .bge a b tgt => BrOK pc w .BGE a b tgt
  | .bltu a b tgt => BrOK pc w .BLTU a b tgt
  | .bgeu a b tgt => BrOK pc w .BGEU a b tgt
  | .sub rd rs1 rs2 => (1 ≤ rd ∧ rd ≤ 31) ∧ rs1 ≤ 31 ∧ rs2 ≤ 31 ∧
      DecodeFactM (mkM pc w .sub rd rs1 rs2 0)
  | .and rd rs1 rs2 => (1 ≤ rd ∧ rd ≤ 31) ∧ rs1 ≤ 31 ∧ rs2 ≤ 31 ∧
      DecodeFactM (mkM pc w .and rd rs1 rs2 0)
  | .or rd rs1 rs2 => (1 ≤ rd ∧ rd ≤ 31) ∧ rs1 ≤ 31 ∧ rs2 ≤ 31 ∧
      DecodeFactM (mkM pc w .or rd rs1 rs2 0)
  | .xor rd rs1 rs2 => (1 ≤ rd ∧ rd ≤ 31) ∧ rs1 ≤ 31 ∧ rs2 ≤ 31 ∧
      DecodeFactM (mkM pc w .xor rd rs1 rs2 0)
  | .andi rd rs1 imm => (1 ≤ rd ∧ rd ≤ 31) ∧ rs1 ≤ 31 ∧ Imm12 imm ∧ imm < 2 ^ 64 ∧
      DecodeFactM (mkM pc w .andi rd rs1 0 imm)
  | .ori rd rs1 imm => (1 ≤ rd ∧ rd ≤ 31) ∧ rs1 ≤ 31 ∧ Imm12 imm ∧ imm < 2 ^ 64 ∧
      DecodeFactM (mkM pc w .ori rd rs1 0 imm)
  | .xori rd rs1 imm => (1 ≤ rd ∧ rd ≤ 31) ∧ rs1 ≤ 31 ∧ Imm12 imm ∧ imm < 2 ^ 64 ∧
      DecodeFactM (mkM pc w .xori rd rs1 0 imm)
  | .slli rd rs1 k => (1 ≤ rd ∧ rd ≤ 31) ∧ rs1 ≤ 31 ∧ k < 64 ∧
      DecodeFactM (mkM pc w .slli rd rs1 0 k)
  | .srli rd rs1 k => (1 ≤ rd ∧ rd ≤ 31) ∧ rs1 ≤ 31 ∧ k < 64 ∧
      DecodeFactM (mkM pc w .srli rd rs1 0 k)
  | .addiw rd rs1 imm => (1 ≤ rd ∧ rd ≤ 31) ∧ rs1 ≤ 31 ∧ Imm12 imm ∧
      DecodeFactM (mkM pc w .addiw rd rs1 0 imm)
  | .addw rd rs1 rs2 => (1 ≤ rd ∧ rd ≤ 31) ∧ rs1 ≤ 31 ∧ rs2 ≤ 31 ∧
      DecodeFactM (mkM pc w .addw rd rs1 rs2 0)
  | .subw rd rs1 rs2 => (1 ≤ rd ∧ rd ≤ 31) ∧ rs1 ≤ 31 ∧ rs2 ≤ 31 ∧
      DecodeFactM (mkM pc w .subw rd rs1 rs2 0)
  | .slliw rd rs1 k => (1 ≤ rd ∧ rd ≤ 31) ∧ rs1 ≤ 31 ∧ k < 32 ∧
      (BitVec.ofNat 32 w).extractLsb' 20 5 = BitVec.ofNat 5 k ∧ DecodeFactM (mkM pc w .slliw rd rs1 0 k)
  | .srliw rd rs1 k => (1 ≤ rd ∧ rd ≤ 31) ∧ rs1 ≤ 31 ∧ k < 32 ∧
      (BitVec.ofNat 32 w).extractLsb' 20 5 = BitVec.ofNat 5 k ∧ DecodeFactM (mkM pc w .srliw rd rs1 0 k)
  | .slt rd rs1 rs2 => (1 ≤ rd ∧ rd ≤ 31) ∧ rs1 ≤ 31 ∧ rs2 ≤ 31 ∧
      DecodeFactM (mkM pc w .slt rd rs1 rs2 0)
  | .slti rd rs1 imm => (1 ≤ rd ∧ rd ≤ 31) ∧ rs1 ≤ 31 ∧ Imm12 imm ∧ imm < 2 ^ 64 ∧
      DecodeFactM (mkM pc w .slti rd rs1 0 imm)
  | .sll rd rs1 rs2 => (1 ≤ rd ∧ rd ≤ 31) ∧ rs1 ≤ 31 ∧ rs2 ≤ 31 ∧
      DecodeFactM (mkM pc w .sll rd rs1 rs2 0)
  | .srl rd rs1 rs2 => (1 ≤ rd ∧ rd ≤ 31) ∧ rs1 ≤ 31 ∧ rs2 ≤ 31 ∧
      DecodeFactM (mkM pc w .srl rd rs1 rs2 0)
  | .sllw rd rs1 rs2 => (1 ≤ rd ∧ rd ≤ 31) ∧ rs1 ≤ 31 ∧ rs2 ≤ 31 ∧
      DecodeFactM (mkM pc w .sllw rd rs1 rs2 0)
  | .srlw rd rs1 rs2 => (1 ≤ rd ∧ rd ≤ 31) ∧ rs1 ≤ 31 ∧ rs2 ≤ 31 ∧
      DecodeFactM (mkM pc w .srlw rd rs1 rs2 0)
  | .fence => ADec w (instruction.FENCE (BitVec.ofNat 4 (w >>> 28), 0xf#4, 0xf#4,
      gprIdx ((w >>> 15) % 32), gprIdx ((w >>> 7) % 32)))
  | .srai rd rs1 k => (1 ≤ rd ∧ rd ≤ 31) ∧ rs1 ≤ 31 ∧ k < 64 ∧
      DecodeFactM (mkM pc w .srai rd rs1 0 k)
  | .sraiw rd rs1 k => (1 ≤ rd ∧ rd ≤ 31) ∧ rs1 ≤ 31 ∧ k < 32 ∧
      (BitVec.ofNat 32 w).extractLsb' 20 5 = BitVec.ofNat 5 k ∧ DecodeFactM (mkM pc w .sraiw rd rs1 0 k)
  | .sra rd rs1 rs2 => (1 ≤ rd ∧ rd ≤ 31) ∧ rs1 ≤ 31 ∧ rs2 ≤ 31 ∧
      ADec w (instruction.RTYPE (gprIdx rs2, gprIdx rs1, gprIdx rd, rop.SRA))
  | .sltu rd rs1 rs2 => (1 ≤ rd ∧ rd ≤ 31) ∧ rs1 ≤ 31 ∧ rs2 ≤ 31 ∧
      ADec w (instruction.RTYPE (gprIdx rs2, gprIdx rs1, gprIdx rd, rop.SLTU))
  | .sltiu rd rs1 imm => (1 ≤ rd ∧ rd ≤ 31) ∧ rs1 ≤ 31 ∧ Imm12 imm ∧ imm < 2 ^ 64 ∧
      ADec w (instruction.ITYPE (BitVec.ofNat 12 imm, gprIdx rs1, gprIdx rd, iop.SLTIU))
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

/-- What the relation knows of a byte: present with value `b`, or, where `ab`
holds, only that the model's total read gives `b`. -/
def MemAt (ab : Nat → Bool) (m : Std.ExtHashMap Nat (BitVec 8)) (a b : Nat) : Prop :=
  if ab a then (m[a]?).getD 0 = BitVec.ofNat 8 b else m[a]? = some (BitVec.ofNat 8 b)

theorem MemAt.read {ab : Nat → Bool} {m : Std.ExtHashMap Nat (BitVec 8)} {a b : Nat}
    (h : MemAt ab m a b) : (m[a]?).getD 0 = BitVec.ofNat 8 b := by
  unfold MemAt at h; split at h
  · exact h
  · rw [h]; rfl

theorem MemAt.of_some {ab : Nat → Bool} {m : Std.ExtHashMap Nat (BitVec 8)} {a b : Nat}
    (h : m[a]? = some (BitVec.ofNat 8 b)) : MemAt ab m a b := by
  unfold MemAt; split
  · rw [h]; rfl
  · exact h

/-- The simulation relation. -/
structure Sim (c : Code) (ab : Nat → Bool) (s : St) (cf : Config) : Prop where
  good : GoodState cf.σ
  pc : cf.σ.regs.get? Register.PC = some (BitVec.ofNat 64 s.pc)
  regs : GHolds cf.σ (pinsOf s.regs)
  x0 : getR s.regs 0 = 0
  /-- A known byte is present with its value; on the bytes `ab` marks (read
  before written but not loaded by the ELF) only its total read is known. -/
  mem : ∀ a b, s.mem.byte a = some b → MemAt ab cf.σ.mem a b
  code : CodeIn c cf.σ.mem
  console : output cf.σ = ""
  payload : cf.σ.regs.get? Register.htif_payload_writes = some 0#4
  tick : cf.tick < 2

/-- From `cf` the machine runs to a configuration related to `t`. -/
inductive Reaches (c : Code) (ab : Nat → Bool) (cf : Config) (t : St) : Prop where
  | intro (cf' : Config) (steps : Steps cf cf') (sim : Sim c ab t cf')

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
theorem mstep {c : Code} {ab : Nat → Bool} {s : St} {cf : Config} (hS : Sim c ab s cf) {w : Nat} {a : MInstr}
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
theorem alu_sound {c : Code} {ab : Nat → Bool} {s : St} {cf : Config} (hS : Sim c ab s cf) {w : Nat}
    (hf : Fetchable c s.pc w) {k : MKind} {rd rs1 rs2 imm v : Nat}
    (hdec : DecodeFactM (mkM s.pc w k rd rs1 rs2 imm))
    (hk : KindOK (keysG (pinsOf s.regs)) k rd rs1 rs2)
    (hmf : MemFacts cf.σ.mem (pinsOf s.regs) [] (mkM s.pc w k rd rs1 rs2 imm))
    (hrd : 1 ≤ rd ∧ rd ≤ 31)
    (hgm : stepGM (mkM s.pc w k rd rs1 rs2 imm) (pinsOf s.regs) [] =
      (rd, BitVec.ofNat 64 v) :: eraseG rd (pinsOf s.regs))
    (hlog : wlogM [mkM s.pc w k rd rs1 rs2 imm] (pinsOf s.regs) [[]] = [])
    {i : Ins} (hget : c.table.getW s.pc = some (w, i)) :
    Reaches c ab cf ⟨(s.pc + 4) % W, setR s.regs rd v, s.mem⟩ := by
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

theorem sound_addi {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s : St} {cf : Config} (hS : Sim c ab s cf)
    {w rd rs1 imm : Nat} (hget : c.table.getW s.pc = some (w, .addi rd rs1 imm)) :
    Reaches c ab cf ⟨(s.pc + 4) % W, setR s.regs rd (fAdd (getR s.regs rs1) imm), s.mem⟩ := by
  obtain ⟨hf, hrd, hrs1, himm, hdec⟩ := hT _ _ _ hget
  refine alu_sound hS hf hdec ⟨hrd, srcOK_of hrs1⟩ trivial hrd ?_ rfl hget
  show (rd, srcVal rs1 (pinsOf s.regs) + sign_extend (m := 64) (BitVec.ofNat 12 imm)) :: _ = _
  rw [himm, srcVal_pinsOf hS.x0 hrs1, ofNat_add_mod]; rfl

theorem sound_add {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s : St} {cf : Config} (hS : Sim c ab s cf)
    {w rd rs1 rs2 : Nat} (hget : c.table.getW s.pc = some (w, .add rd rs1 rs2)) :
    Reaches c ab cf ⟨(s.pc + 4) % W, setR s.regs rd (fAdd (getR s.regs rs1) (getR s.regs rs2)), s.mem⟩ := by
  obtain ⟨hf, hrd, hrs1, hrs2, hdec⟩ := hT _ _ _ hget
  refine alu_sound hS hf hdec ⟨hrd, srcOK_of hrs1, srcOK_of hrs2⟩ trivial hrd ?_ rfl hget
  show (rd, srcVal rs1 (pinsOf s.regs) + srcVal rs2 (pinsOf s.regs)) :: _ = _
  rw [srcVal_pinsOf hS.x0 hrs1, srcVal_pinsOf hS.x0 hrs2, ofNat_add_mod]; rfl

/-! ## Terminators: branches, `j`, `jr` (`term_step_bt`) -/

/-- `term_step_bt` for a terminator of an entry, from the relation: the
registers and memory stay, the PC moves to `tgtPCT`. -/
theorem tstep {c : Code} {ab : Nat → Bool} {s : St} {cf : Config} (hS : Sim c ab s cf) {w : Nat}
    (hf : Fetchable c s.pc w) {i : Ins} (hget : c.table.getW s.pc = some (w, i))
    {k : TKind} {rs1 rs2 : Nat} {i13 : BitVec 13} {i21 : BitVec 21} {i12 : BitVec 12}
    (hdec : DecodeFactT (mkT s.pc w k rs1 rs2 i13 i21 i12))
    (hk : TermKindOK (keysG (pinsOf s.regs)) (BitVec.ofNat 64 s.pc) rs1 rs2 i13 i21 k)
    (htf : TermFactsT (pinsOf s.regs) (mkT s.pc w k rs1 rs2 i13 i21 i12))
    {pc' : Nat} (hpc' : tgtPCT (mkT s.pc w k rs1 rs2 i13 i21 i12) (pinsOf s.regs) = BitVec.ofNat 64 pc') :
    Reaches c ab cf ⟨pc', s.regs, s.mem⟩ := by
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
theorem branch_sound {c : Code} {ab : Nat → Bool} {s : St} {cf : Config} (hS : Sim c ab s cf) {w : Nat}
    (hf : Fetchable c s.pc w) {i : Ins} (hget : c.table.getW s.pc = some (w, i))
    {op : bop} {f : Nat → Nat → Bool}
    (hop : ∀ x y, x < 2 ^ 64 → y < 2 ^ 64 → guardB op (BitVec.ofNat 64 x) (BitVec.ofNat 64 y) = f x y)
    {rs1 rs2 tgt : Nat} {off : BitVec 13} (h1 : rs1 ≤ 31) (h2 : rs2 ≤ 31)
    (htgt : BitVec.ofNat 64 s.pc + sign_extend (m := 64) off = BitVec.ofNat 64 tgt)
    (hlt : tgt < 2 ^ 64) (hal : tgt % 4 = 0)
    (hdec : DecodeFactT (mkT s.pc w (.br op true) rs1 rs2 off 0 0)) :
    Reaches c ab cf (br s (f (getR s.regs rs1) (getR s.regs rs2)) tgt) := by
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

theorem sound_br {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s : St} {cf : Config} (hS : Sim c ab s cf)
    {w a b tgt : Nat} {i : Ins} (hget : c.table.getW s.pc = some (w, i)) {op : bop}
    (hbr : BrOK s.pc w op a b tgt) {f : Nat → Nat → Bool}
    (hop : ∀ x y, x < 2 ^ 64 → y < 2 ^ 64 → guardB op (BitVec.ofNat 64 x) (BitVec.ofNat 64 y) = f x y) :
    Reaches c ab cf (br s (f (getR s.regs a) (getR s.regs b)) tgt) := by
  obtain ⟨ha, hb, hlt, hal, htgt, hdec⟩ := hbr
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
theorem sound_ld {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s s' : St} {cf : Config} (hS : Sim c ab s cf)
    {w rd rs1 imm : Nat} (hget : c.table.getW s.pc = some (w, .ld rd rs1 imm))
    (hex : exec (.ld rd rs1 imm) c s = some s') :
    Reaches c ab cf s' := by
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
      show Reaches c ab cf ⟨(s.pc + 4) % W, setR s.regs rd v, s.mem⟩
      obtain ⟨hv, hbytes⟩ := load_bytes hl
      have hea := eaddr_mkM (k := .ld) (pc := s.pc) (w := w) (rd := rd) (rs2 := 0) hS.x0 hrs1 himm
      rw [ha] at hea
      have heat : (eaddrM (mkM s.pc w .ld rd rs1 0 imm) (pinsOf s.regs)).toNat = a := by
        rw [hea, ← ha, ea_toNat]
      have hpin : ∀ k, k < 8 → (cf.σ.mem[a + k]?).getD 0 = vbyte v k := fun k hk =>
        (hS.mem _ _ (hbytes k hk)).read
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
theorem mem_after_store {c : Code} {ab : Nat → Bool} {M M' : MT} {a wd v : Nat} {σm : Std.ExtHashMap Nat (BitVec 8)}
    {e : WEntry} (hst : store M a wd v = some M')
    (hrel : ∀ x b, M.byte x = some b → MemAt ab σm x b)
    (hin : ∀ x, a ≤ x → x < a + wd → entryByte e x = some (BitVec.ofNat 8 (fldAt (8 * (x - a)) 8 v)))
    (hout : ∀ x, ¬ (a ≤ x ∧ x < a + wd) → entryByte e x = none)
    (hcode : CodeIn c σm) (hoff : a + wd ≤ c.lo ∨ c.hi ≤ a) (hT : TableOK c) :
    (∀ x b, M'.byte x = some b → MemAt ab (applyW σm e) x b) ∧
      CodeIn c (applyW σm e) := by
  constructor
  · intro x b hb
    rw [store_bytes hst] at hb
    by_cases hx : a ≤ x ∧ x < a + wd
    · rw [if_pos hx] at hb; cases hb
      apply MemAt.of_some; rw [applyW_get, hin x hx.1 hx.2]; rfl
    · rw [if_neg hx] at hb
      have h := hrel x b hb
      unfold MemAt at h ⊢
      rw [applyW_get, hout x hx]; exact h
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
theorem sound_sd {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s s' : St} {cf : Config} (hS : Sim c ab s cf)
    {w rs1 rs2 imm : Nat} (hget : c.table.getW s.pc = some (w, .sd rs1 rs2 imm))
    (hex : exec (.sd rs1 rs2 imm) c s = some s') :
    Reaches c ab cf s' := by
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
theorem sound_lbu {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s s' : St} {cf : Config} (hS : Sim c ab s cf)
    {w rd rs1 imm : Nat} (hget : c.table.getW s.pc = some (w, .lbu rd rs1 imm))
    (hex : exec (.lbu rd rs1 imm) c s = some s') :
    Reaches c ab cf s' := by
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
      show Reaches c ab cf ⟨(s.pc + 4) % W, setR s.regs rd v, s.mem⟩
      obtain ⟨hv, hbytes⟩ := load_bytes hl
      have hea := eaddr_mkM (k := .lbu) (pc := s.pc) (w := w) (rd := rd) (rs2 := 0) hS.x0 hrs1 himm
      rw [ha] at hea
      have heat : (eaddrM (mkM s.pc w .lbu rd rs1 0 imm) (pinsOf s.regs)).toNat = a := by
        rw [hea, ← ha, ea_toNat]
      have hpin : (cf.σ.mem[a]?).getD 0 = vbyte v 0 := by
        have := (hS.mem _ _ (hbytes 0 (by omega))).read; simpa using this
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
theorem sound_li {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s : St} {cf : Config} (hS : Sim c ab s cf)
    {w rd v : Nat} (hget : c.table.getW s.pc = some (w, .li rd v)) :
    Reaches c ab cf ⟨(s.pc + 4) % W, setR s.regs rd v, s.mem⟩ := by
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
theorem sim_link {c : Code} {ab : Nat → Bool} {s : St} {σ σ' : MState} {t u t' u' : Nat} (hS : Sim c ab s ⟨σ, t, u⟩)
    {rd : Nat} (hrd : 1 ≤ rd ∧ rd ≤ 31) {link : BitVec 64} {tgt : Nat}
    (hG' : GoodState σ') (ht' : t' < 2) (hmem : σ'.mem = σ.mem)
    (hpc : σ'.regs.get? Register.PC = some (BitVec.ofNat 64 tgt))
    (hrdv : σ'.regs.get? (gprReg rd) = some (gprRT rd link))
    (hgpr : ∀ n, 1 ≤ n → n ≤ 31 → n ≠ rd → σ'.regs.get? (gprReg n) = σ.regs.get? (gprReg n))
    (hout : σ'.sailOutput = σ.sailOutput)
    (hpay : σ'.regs.get? Register.htif_payload_writes = σ.regs.get? Register.htif_payload_writes) :
    Sim c ab ⟨tgt, setR s.regs rd link.toNat, s.mem⟩ ⟨σ', t', u'⟩ := by
  have hGH : GHolds σ' ((rd, link) :: eraseG rd (pinsOf s.regs)) :=
    ⟨gprGet_of_get rd hrd.1 hrd.2 hrdv, gholds_erase_frame rd hgpr _ (keysOK_pinsOf _) hS.regs⟩
  refine ⟨hG', hpc, pins_setR hrd.1 hrd.2 hGH, getR_setR_zero _ hS.x0, ?_, ?_, ?_, ?_, ht'⟩
  · intro a b hb; simp only; rw [hmem]; exact hS.mem a b hb
  · simp only; rw [hmem]; exact hS.code
  · simp only [output] at hS ⊢; rw [hout]; exact hS.console
  · rw [hpay]; exact hS.payload

theorem sound_jal {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s : St} {cf : Config} (hS : Sim c ab s cf)
    {w rd tgt : Nat} (hget : c.table.getW s.pc = some (w, .jal rd tgt)) :
    Reaches c ab cf ⟨tgt, setR s.regs rd ((s.pc + 4) % W), s.mem⟩ := by
  obtain ⟨hf, hrd, hlt, hal, htgt, hdec⟩ := hT _ _ _ hget
  have hal' : (BitVec.ofNat 64 s.pc + sign_extend (m := 64) (jimm w)).toNat % 4 = 0 := by
    rw [htgt, BitVec.toNat_ofNat, Nat.mod_eq_of_lt hlt]; exact hal
  by_cases h0 : rd = 0
  · subst h0
    rw [setR_zero]
    exact tstep hS hf hget (k := .j) (rs1 := 0) (rs2 := 0) (i13 := 0) (i21 := jimm w) (i12 := 0)
      hdec hal' trivial (by simp only [tgtPCT, tgtPC0, mkT]; exact htgt)
  · obtain ⟨σ, t, u⟩ := cf
    have hrd' : 1 ≤ rd ∧ rd ≤ 31 := ⟨by omega, hrd⟩
    obtain ⟨vm, hmi⟩ := hS.good.minstret
    have hpcn := pc_toNat hf.fetch
    have hcode := hS.code _ _ _ hget
    obtain ⟨hr1, hr2, hr3, hr4, hr5⟩ := gpr_rd_ok rd (by omega) hrd'.1
    have hwx := wX_gpr {(afterNextPC (afterPrelude σ) (BitVec.ofNat 64 s.pc)) with
        regs := (afterNextPC (afterPrelude σ) (BitVec.ofNat 64 s.pc)).regs.insert Register.nextPC
          (BitVec.ofNat 64 s.pc + sign_extend (m := 64) (jimm w))}
      (BitVec.addInt (BitVec.ofNat 64 s.pc) 4) rd hrd'.1 hrd'.2
    obtain ⟨σ', i', hs, hi', hG', hmem', hobs⟩ :=
      stepObs_jal σ t u (BitVec.ofNat 64 s.pc) vm _ (jimm w) (gprIdx rd) (gprReg rd)
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
      show ((((sigma3_jal σ _ (jimm w) (gprReg rd) _).regs.insert Register.PC _).insert
        Register.minstret _)).get? (gprReg rd) = _
      rw [Std.ExtDHashMap.get?_insert]
      simp only [notGpr_minstret rd, dif_neg, reduceCtorEq, not_false_eq_true]
      rw [Std.ExtDHashMap.get?_insert]
      simp only [notGpr_PC rd, dif_neg, reduceCtorEq, not_false_eq_true]
      show ((_ : Std.ExtDHashMap Register RegisterType).insert (gprReg rd) _).get? (gprReg rd) = _
      rw [Std.ExtDHashMap.get?_insert_self]
    have hpc' : σ'.regs.get? Register.PC = some (BitVec.ofNat 64 tgt) := by
      rw [hobs.1 _ (by decide) (by decide) (by decide)]
      show ((((sigma3_jal σ _ (jimm w) (gprReg rd) _).regs.insert Register.PC _).insert
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

theorem sound_jalr {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s s' : St} {cf : Config} (hS : Sim c ab s cf)
    {w rd rs1 imm : Nat} (hget : c.table.getW s.pc = some (w, .jalr rd rs1 imm))
    (hex : exec (.jalr rd rs1 imm) c s = some s') :
    Reaches c ab cf s' := by
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

/-! ## Arithmetic bridges (Nat evaluator ↔ Sail BitVec) -/

theorem ofNat_sub_mod (x y : Nat) (hy : y < 2 ^ 64) :
    BitVec.ofNat 64 x - BitVec.ofNat 64 y = BitVec.ofNat 64 ((x + (W - y)) % W) := by
  apply BitVec.eq_of_toNat_eq
  simp only [BitVec.toNat_sub, BitVec.toNat_ofNat, W]; omega

theorem ofNat_and (x y : Nat) (hx : x < 2 ^ 64) (hy : y < 2 ^ 64) :
    BitVec.ofNat 64 x &&& BitVec.ofNat 64 y = BitVec.ofNat 64 (x &&& y) := by
  apply BitVec.eq_of_toNat_eq
  simp only [BitVec.toNat_and, BitVec.toNat_ofNat, Nat.mod_eq_of_lt hx, Nat.mod_eq_of_lt hy,
    Nat.mod_eq_of_lt (Nat.and_lt_two_pow x hy)]

theorem ofNat_or (x y : Nat) (hx : x < 2 ^ 64) (hy : y < 2 ^ 64) :
    BitVec.ofNat 64 x ||| BitVec.ofNat 64 y = BitVec.ofNat 64 (x ||| y) := by
  apply BitVec.eq_of_toNat_eq
  simp only [BitVec.toNat_or, BitVec.toNat_ofNat, Nat.mod_eq_of_lt hx, Nat.mod_eq_of_lt hy,
    Nat.mod_eq_of_lt (Nat.or_lt_two_pow hx hy)]

theorem ofNat_xor (x y : Nat) (hx : x < 2 ^ 64) (hy : y < 2 ^ 64) :
    BitVec.ofNat 64 x ^^^ BitVec.ofNat 64 y = BitVec.ofNat 64 (x ^^^ y) := by
  apply BitVec.eq_of_toNat_eq
  simp only [BitVec.toNat_xor, BitVec.toNat_ofNat, Nat.mod_eq_of_lt hx, Nat.mod_eq_of_lt hy,
    Nat.mod_eq_of_lt (Nat.xor_lt_two_pow hx hy)]

theorem shl_ofNat (x k : Nat) (hk : k < 64) :
    shift_bits_left (BitVec.ofNat 64 x) (Sail.BitVec.extractLsb ((BitVec.ofNat 12 k).extractLsb' 0 6) 5 0)
      = BitVec.ofNat 64 ((x <<< k) % W) := by
  apply BitVec.eq_of_toNat_eq
  simp [shift_bits_left, Sail.BitVec.extractLsb, BitVec.toNat_shiftLeft, W]
  rw [Nat.mod_eq_of_lt hk, Nat.shiftLeft_eq, Nat.shiftLeft_eq, Nat.mul_mod, Nat.mod_mod, ← Nat.mul_mod]

theorem guard_bltu (x y : Nat) (hx : x < 2 ^ 64) (hy : y < 2 ^ 64) :
    guardB .BLTU (BitVec.ofNat 64 x) (BitVec.ofNat 64 y) = bLtu x y := by
  simp [guardB, bLtu, zopz0zI_u, BitVec.toNatInt, Nat.mod_eq_of_lt hx, Nat.mod_eq_of_lt hy]

theorem srl_ofNat (x k : Nat) (hx : x < 2 ^ 64) (hk : k < 64) :
    shift_bits_right (BitVec.ofNat 64 x) (Sail.BitVec.extractLsb ((BitVec.ofNat 12 k).extractLsb' 0 6) 5 0)
      = BitVec.ofNat 64 (x >>> k) := by
  apply BitVec.eq_of_toNat_eq
  simp [shift_bits_right, Sail.BitVec.extractLsb, BitVec.toNat_ushiftRight]
  rw [Nat.mod_eq_of_lt hx, Nat.mod_eq_of_lt hk,
    Nat.mod_eq_of_lt (Nat.lt_of_le_of_lt (Nat.shiftRight_le x k) hx)]

theorem xorH (x : Nat) (hx : x < 2 ^ 64) : x ^^^ H = if x < H then x + H else x - H := by
  have hH : H = 2 ^ 63 * 1 + 0 := rfl
  have hx' : x = 2 ^ 63 * (x / 2 ^ 63) + x % 2 ^ 63 := (Nat.div_add_mod x _).symm
  have hq : x / 2 ^ 63 ≤ 1 := by
    have : x / 2 ^ 63 < 2 := Nat.div_lt_of_lt_mul (by simpa using hx)
    omega
  have hr : x % 2 ^ 63 < 2 ^ 63 := Nat.mod_lt _ (Nat.two_pow_pos 63)
  apply Nat.eq_of_testBit_eq
  intro j
  rw [Nat.testBit_xor]
  conv => lhs; rw [hx', hH]
  rw [Nat.testBit_two_pow_mul_add _ hr, Nat.testBit_two_pow_mul_add _ (Nat.two_pow_pos 63)]
  by_cases hlt : x < H
  · have hq0 : x / 2 ^ 63 = 0 := by simp only [H] at hlt; omega
    rw [if_pos hlt, show x + H = 2 ^ 63 * 1 + x % 2 ^ 63 by simp only [H]; omega,
      Nat.testBit_two_pow_mul_add _ hr, hq0]
    by_cases hj : j < 63
    · simp [hj]
    · simp [hj]
  · have hq1 : x / 2 ^ 63 = 1 := by simp only [H] at hlt; omega
    rw [if_neg hlt, show x - H = 2 ^ 63 * 0 + x % 2 ^ 63 by simp only [H]; omega,
      Nat.testBit_two_pow_mul_add _ hr, hq1]
    by_cases hj : j < 63
    · simp [hj]
    · by_cases h63 : j - 63 = 0
      · simp [hj, h63]
      · simp [hj, Nat.testBit_one_eq_true_iff_self_eq_zero, h63]

theorem guard_blt (x y : Nat) (hx : x < 2 ^ 64) (hy : y < 2 ^ 64) :
    guardB .BLT (BitVec.ofNat 64 x) (BitVec.ofNat 64 y) = bLt x y := by
  simp [guardB, bLt, slt64, zopz0zI_s, BitVec.toInt, Nat.mod_eq_of_lt hx, Nat.mod_eq_of_lt hy, xorH x hx, xorH y hy]
  simp only [H]
  by_cases h1 : x < 9223372036854775808 <;> by_cases h2 : y < 9223372036854775808 <;>
    simp only [h1, h2, if_true, if_false, show (2 * x < 18446744073709551616) = (x < 9223372036854775808) by
      apply propext; omega, show (2 * y < 18446744073709551616) = (y < 9223372036854775808) by
      apply propext; omega] <;> omega

theorem guard_bgeu (x y : Nat) (hx : x < 2 ^ 64) (hy : y < 2 ^ 64) :
    guardB .BGEU (BitVec.ofNat 64 x) (BitVec.ofNat 64 y) = bGeu x y := by
  simp [guardB, bGeu, zopz0zKzJ_u, BitVec.toNatInt, Nat.mod_eq_of_lt hx, Nat.mod_eq_of_lt hy]
  by_cases h : x < y
  · simp [h, Nat.not_le.2 h]
  · simp [h, Nat.not_lt.1 h]

theorem guard_bge (x y : Nat) (hx : x < 2 ^ 64) (hy : y < 2 ^ 64) :
    guardB .BGE (BitVec.ofNat 64 x) (BitVec.ofNat 64 y) = bGe x y := by
  have h := guard_blt x y hx hy
  simp only [guardB, bLt] at h
  simp only [guardB, bGe, ← h, zopz0zKzJ_s, zopz0zI_s]
  by_cases hc : (BitVec.ofNat 64 x).toInt < (BitVec.ofNat 64 y).toInt
  · simp [hc, Int.not_le.2 hc]
  · simp [hc, Int.not_lt.1 hc]

/-! ## More ALU kinds -/

/-- An ALU entry `rd := op(rs1, rs2)` whose Sail value the evaluator computes as `f`. -/
theorem sound_rr {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s : St} {cf : Config} (hS : Sim c ab s cf)
    {w rd rs1 rs2 : Nat} {i : Ins} (hget : c.table.getW s.pc = some (w, i)) {k : MKind}
    (hk : (1 ≤ rd ∧ rd ≤ 31) ∧ rs1 ≤ 31 ∧ rs2 ≤ 31 ∧ DecodeFactM (mkM s.pc w k rd rs1 rs2 0))
    (hkind : KindOK (keysG (pinsOf s.regs)) k rd rs1 rs2)
    (hmf : MemFacts cf.σ.mem (pinsOf s.regs) [] (mkM s.pc w k rd rs1 rs2 0))
    {f : Nat → Nat → Nat}
    (hval : stepGM (mkM s.pc w k rd rs1 rs2 0) (pinsOf s.regs) [] =
      (rd, BitVec.ofNat 64 (f (getR s.regs rs1) (getR s.regs rs2))) :: eraseG rd (pinsOf s.regs))
    (hlog : wlogM [mkM s.pc w k rd rs1 rs2 0] (pinsOf s.regs) [[]] = []) :
    Reaches c ab cf ⟨(s.pc + 4) % W, setR s.regs rd (f (getR s.regs rs1) (getR s.regs rs2)), s.mem⟩ :=
  alu_sound hS (hT _ _ _ hget).fetch hk.2.2.2 hkind hmf hk.1 hval hlog hget

theorem sound_sub {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s : St} {cf : Config} (hS : Sim c ab s cf)
    {w rd rs1 rs2 : Nat} (hget : c.table.getW s.pc = some (w, .sub rd rs1 rs2)) :
    Reaches c ab cf ⟨(s.pc + 4) % W, setR s.regs rd (fSub (getR s.regs rs1) (getR s.regs rs2)), s.mem⟩ := by
  have hk := (hT _ _ _ hget).dec
  refine sound_rr hT hS hget hk ⟨hk.1, srcOK_of hk.2.1, srcOK_of hk.2.2.1⟩ trivial ?_ rfl
  show (rd, srcVal rs1 (pinsOf s.regs) - srcVal rs2 (pinsOf s.regs)) :: _ = _
  rw [srcVal_pinsOf hS.x0 hk.2.1, srcVal_pinsOf hS.x0 hk.2.2.1, ofNat_sub_mod _ _ (getR_lt _ _)]; rfl

theorem sound_and {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s : St} {cf : Config} (hS : Sim c ab s cf)
    {w rd rs1 rs2 : Nat} (hget : c.table.getW s.pc = some (w, .and rd rs1 rs2)) :
    Reaches c ab cf ⟨(s.pc + 4) % W, setR s.regs rd (fAnd (getR s.regs rs1) (getR s.regs rs2)), s.mem⟩ := by
  have hk := (hT _ _ _ hget).dec
  refine sound_rr hT hS hget hk ⟨hk.1, srcOK_of hk.2.1, srcOK_of hk.2.2.1⟩ trivial ?_ rfl
  show (rd, srcVal rs1 (pinsOf s.regs) &&& srcVal rs2 (pinsOf s.regs)) :: _ = _
  rw [srcVal_pinsOf hS.x0 hk.2.1, srcVal_pinsOf hS.x0 hk.2.2.1,
    ofNat_and _ _ (getR_lt _ _) (getR_lt _ _)]; rfl

theorem sound_or {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s : St} {cf : Config} (hS : Sim c ab s cf)
    {w rd rs1 rs2 : Nat} (hget : c.table.getW s.pc = some (w, .or rd rs1 rs2)) :
    Reaches c ab cf ⟨(s.pc + 4) % W, setR s.regs rd (fOr (getR s.regs rs1) (getR s.regs rs2)), s.mem⟩ := by
  have hk := (hT _ _ _ hget).dec
  refine sound_rr hT hS hget hk ⟨hk.1, srcOK_of hk.2.1, srcOK_of hk.2.2.1⟩ trivial ?_ rfl
  show (rd, srcVal rs1 (pinsOf s.regs) ||| srcVal rs2 (pinsOf s.regs)) :: _ = _
  rw [srcVal_pinsOf hS.x0 hk.2.1, srcVal_pinsOf hS.x0 hk.2.2.1,
    ofNat_or _ _ (getR_lt _ _) (getR_lt _ _)]; rfl

theorem sound_xor {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s : St} {cf : Config} (hS : Sim c ab s cf)
    {w rd rs1 rs2 : Nat} (hget : c.table.getW s.pc = some (w, .xor rd rs1 rs2)) :
    Reaches c ab cf ⟨(s.pc + 4) % W, setR s.regs rd (fXor (getR s.regs rs1) (getR s.regs rs2)), s.mem⟩ := by
  have hk := (hT _ _ _ hget).dec
  refine sound_rr hT hS hget hk ⟨hk.1, srcOK_of hk.2.1, srcOK_of hk.2.2.1⟩ trivial ?_ rfl
  show (rd, srcVal rs1 (pinsOf s.regs) ^^^ srcVal rs2 (pinsOf s.regs)) :: _ = _
  rw [srcVal_pinsOf hS.x0 hk.2.1, srcVal_pinsOf hS.x0 hk.2.2.1,
    ofNat_xor _ _ (getR_lt _ _) (getR_lt _ _)]; rfl

/-- An ALU entry `rd := op(rs1, imm)` whose Sail value the evaluator computes as `f`. -/
theorem sound_ri {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s : St} {cf : Config} (hS : Sim c ab s cf)
    {w rd rs1 imm : Nat} {i : Ins} (hget : c.table.getW s.pc = some (w, i)) {k : MKind}
    (hdec : DecodeFactM (mkM s.pc w k rd rs1 0 imm)) (hrd : 1 ≤ rd ∧ rd ≤ 31)
    (hkind : KindOK (keysG (pinsOf s.regs)) k rd rs1 0)
    (hmf : MemFacts cf.σ.mem (pinsOf s.regs) [] (mkM s.pc w k rd rs1 0 imm))
    {f : Nat → Nat → Nat}
    (hval : stepGM (mkM s.pc w k rd rs1 0 imm) (pinsOf s.regs) [] =
      (rd, BitVec.ofNat 64 (f (getR s.regs rs1) imm)) :: eraseG rd (pinsOf s.regs))
    (hlog : wlogM [mkM s.pc w k rd rs1 0 imm] (pinsOf s.regs) [[]] = []) :
    Reaches c ab cf ⟨(s.pc + 4) % W, setR s.regs rd (f (getR s.regs rs1) imm), s.mem⟩ :=
  alu_sound hS (hT _ _ _ hget).fetch hdec hkind hmf hrd hval hlog hget

theorem sound_andi {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s : St} {cf : Config} (hS : Sim c ab s cf)
    {w rd rs1 imm : Nat} (hget : c.table.getW s.pc = some (w, .andi rd rs1 imm)) :
    Reaches c ab cf ⟨(s.pc + 4) % W, setR s.regs rd (fAnd (getR s.regs rs1) imm), s.mem⟩ := by
  obtain ⟨hrd, hrs1, himm, hlt, hdec⟩ := (hT _ _ _ hget).dec
  refine sound_ri hT hS hget hdec hrd ⟨hrd, srcOK_of hrs1⟩ trivial ?_ rfl
  show (rd, srcVal rs1 (pinsOf s.regs) &&& sign_extend (m := 64) (BitVec.ofNat 12 imm)) :: _ = _
  rw [himm, srcVal_pinsOf hS.x0 hrs1, ofNat_and _ _ (getR_lt _ _) hlt]; rfl

theorem sound_ori {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s : St} {cf : Config} (hS : Sim c ab s cf)
    {w rd rs1 imm : Nat} (hget : c.table.getW s.pc = some (w, .ori rd rs1 imm)) :
    Reaches c ab cf ⟨(s.pc + 4) % W, setR s.regs rd (fOr (getR s.regs rs1) imm), s.mem⟩ := by
  obtain ⟨hrd, hrs1, himm, hlt, hdec⟩ := (hT _ _ _ hget).dec
  refine sound_ri hT hS hget hdec hrd ⟨hrd, srcOK_of hrs1⟩ trivial ?_ rfl
  show (rd, srcVal rs1 (pinsOf s.regs) ||| sign_extend (m := 64) (BitVec.ofNat 12 imm)) :: _ = _
  rw [himm, srcVal_pinsOf hS.x0 hrs1, ofNat_or _ _ (getR_lt _ _) hlt]; rfl

theorem sound_xori {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s : St} {cf : Config} (hS : Sim c ab s cf)
    {w rd rs1 imm : Nat} (hget : c.table.getW s.pc = some (w, .xori rd rs1 imm)) :
    Reaches c ab cf ⟨(s.pc + 4) % W, setR s.regs rd (fXor (getR s.regs rs1) imm), s.mem⟩ := by
  obtain ⟨hrd, hrs1, himm, hlt, hdec⟩ := (hT _ _ _ hget).dec
  refine sound_ri hT hS hget hdec hrd ⟨hrd, srcOK_of hrs1⟩ trivial ?_ rfl
  show (rd, srcVal rs1 (pinsOf s.regs) ^^^ sign_extend (m := 64) (BitVec.ofNat 12 imm)) :: _ = _
  rw [himm, srcVal_pinsOf hS.x0 hrs1, ofNat_xor _ _ (getR_lt _ _) hlt]; rfl

theorem sound_slli {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s : St} {cf : Config} (hS : Sim c ab s cf)
    {w rd rs1 k : Nat} (hget : c.table.getW s.pc = some (w, .slli rd rs1 k)) :
    Reaches c ab cf ⟨(s.pc + 4) % W, setR s.regs rd (fSlli (getR s.regs rs1) k), s.mem⟩ := by
  obtain ⟨hrd, hrs1, hk, hdec⟩ := (hT _ _ _ hget).dec
  refine sound_ri hT hS hget hdec hrd ⟨hrd, srcOK_of hrs1⟩ trivial ?_ rfl
  show (rd, shift_bits_left (srcVal rs1 (pinsOf s.regs))
    (Sail.BitVec.extractLsb ((BitVec.ofNat 12 k).extractLsb' 0 6) 5 0)) :: _ = _
  rw [srcVal_pinsOf hS.x0 hrs1, shl_ofNat _ _ hk]; rfl

theorem sound_srli {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s : St} {cf : Config} (hS : Sim c ab s cf)
    {w rd rs1 k : Nat} (hget : c.table.getW s.pc = some (w, .srli rd rs1 k)) :
    Reaches c ab cf ⟨(s.pc + 4) % W, setR s.regs rd (fSrli (getR s.regs rs1) k), s.mem⟩ := by
  obtain ⟨hrd, hrs1, hk, hdec⟩ := (hT _ _ _ hget).dec
  refine sound_ri hT hS hget hdec hrd ⟨hrd, srcOK_of hrs1⟩ trivial ?_ rfl
  show (rd, shift_bits_right (srcVal rs1 (pinsOf s.regs))
    (Sail.BitVec.extractLsb ((BitVec.ofNat 12 k).extractLsb' 0 6) 5 0)) :: _ = _
  rw [srcVal_pinsOf hS.x0 hrs1, srl_ofNat _ _ (getR_lt _ _) hk]; rfl

/-! ## 32-bit, comparison and register-shift ALU kinds -/

theorem sext32_bv (v : Nat) (hv : v < 2 ^ 32) :
    sign_extend (m := 64) (BitVec.ofNat 32 v) = BitVec.ofNat 64 (sext32 v) := by
  apply BitVec.eq_of_toNat_eq
  simp [sign_extend, Sail.BitVec.signExtend, BitVec.toNat_signExtend, sext32, BitVec.msb_eq_decide]
  by_cases h : v < 2147483648
  · simp [h, show ¬ 2147483648 ≤ v % 4294967296 by omega]; omega
  · simp [h, show 2147483648 ≤ v % 4294967296 by omega, W, W32]; omega

theorem lo32 (z : Nat) : Sail.BitVec.extractLsb (BitVec.ofNat 64 z) 31 0 = BitVec.ofNat 32 (z % W32) := by
  apply BitVec.eq_of_toNat_eq
  simp [Sail.BitVec.extractLsb, W32]

theorem mod32_lt (z : Nat) : z % W32 < 2 ^ 32 := Nat.mod_lt _ (by decide)

theorem shamt_reg (y : Nat) (hy : y < 2 ^ 64) :
    (Sail.BitVec.extractLsb (BitVec.ofNat 64 y) 5 0).toNat = y % 64 := by
  simp [Sail.BitVec.extractLsb]

theorem shamt_reg32 (y : Nat) :
    (Sail.BitVec.extractLsb (Sail.BitVec.extractLsb (BitVec.ofNat 64 y) 31 0) 4 0).toNat = y % 32 := by
  simp [Sail.BitVec.extractLsb]

theorem shl_mod (a b k M : Nat) (h : a % M = b % M) : (a <<< k) % M = (b <<< k) % M := by
  rw [Nat.shiftLeft_eq, Nat.shiftLeft_eq, Nat.mul_mod, h, ← Nat.mul_mod]

/-- The 32-bit kinds' computed values. -/
theorem wv_addw (x y : Nat) :
    sign_extend (m := 64) (Sail.BitVec.extractLsb (BitVec.ofNat 64 x) 31 0 +
      Sail.BitVec.extractLsb (BitVec.ofNat 64 y) 31 0) = BitVec.ofNat 64 (fAddw x y) := by
  rw [lo32, lo32, BitVec.ofNat_add_ofNat]
  rw [show BitVec.ofNat 32 (x % W32 + y % W32) = BitVec.ofNat 32 ((x + y) % W32) by
    apply BitVec.eq_of_toNat_eq; simp [W32]]
  exact sext32_bv _ (mod32_lt _)

theorem wv_subw (x y : Nat) :
    sign_extend (m := 64) (Sail.BitVec.extractLsb (BitVec.ofNat 64 x) 31 0 -
      Sail.BitVec.extractLsb (BitVec.ofNat 64 y) 31 0) = BitVec.ofNat 64 (fSubw x y) := by
  rw [lo32, lo32]
  rw [show BitVec.ofNat 32 (x % W32) - BitVec.ofNat 32 (y % W32) =
      BitVec.ofNat 32 ((x + (W32 - y % W32)) % W32) by
    apply BitVec.eq_of_toNat_eq; simp [BitVec.toNat_sub, W32]; omega]
  exact sext32_bv _ (mod32_lt _)

theorem wv_addiw (x imm : Nat) (himm : Imm12 imm) :
    sign_extend (m := 64) (Sail.BitVec.extractLsb (BitVec.ofNat 64 x +
      sign_extend (m := 64) (BitVec.ofNat 12 imm)) 31 0) = BitVec.ofNat 64 (sext32 ((x + imm) % W32)) := by
  rw [himm, ofNat_add_mod, lo32, show (x + imm) % W % W32 = (x + imm) % W32 by simp [W, W32]]
  exact sext32_bv _ (mod32_lt _)

theorem wv_slliw (x k : Nat) (hk : k < 32) :
    sign_extend (m := 64) (shift_bits_left (Sail.BitVec.extractLsb (BitVec.ofNat 64 x) 31 0)
      (BitVec.ofNat 5 k)) = BitVec.ofNat 64 (fSlliw x k) := by
  rw [lo32]
  rw [show shift_bits_left (BitVec.ofNat 32 (x % W32)) (BitVec.ofNat 5 k) =
      BitVec.ofNat 32 ((x <<< k) % W32) by
    apply BitVec.eq_of_toNat_eq
    simp only [shift_bits_left, BitVec.shiftLeft_eq', BitVec.toNat_shiftLeft, BitVec.toNat_ofNat]
    rw [Nat.mod_eq_of_lt (show k < 2 ^ 5 by omega)]
    simp only [W32]
    rw [Nat.mod_eq_of_lt (show x % 4294967296 < 2 ^ 32 by omega),
      Nat.mod_mod_of_dvd _ (by decide : 2 ^ 32 ∣ 4294967296)]
    exact shl_mod _ _ _ _ (by simp)]
  exact sext32_bv _ (mod32_lt _)

theorem wv_srliw (x k : Nat) (hk : k < 32) :
    sign_extend (m := 64) (shift_bits_right (Sail.BitVec.extractLsb (BitVec.ofNat 64 x) 31 0)
      (BitVec.ofNat 5 k)) = BitVec.ofNat 64 (fSrliw x k) := by
  rw [lo32]
  rw [show shift_bits_right (BitVec.ofNat 32 (x % W32)) (BitVec.ofNat 5 k) =
      BitVec.ofNat 32 ((x % W32) >>> k) by
    apply BitVec.eq_of_toNat_eq
    simp only [shift_bits_right, BitVec.ushiftRight_eq', BitVec.toNat_ushiftRight, BitVec.toNat_ofNat]
    rw [Nat.mod_eq_of_lt (show k < 2 ^ 5 by omega), Nat.mod_eq_of_lt (mod32_lt x),
      Nat.mod_eq_of_lt (Nat.lt_of_le_of_lt (Nat.shiftRight_le _ k) (mod32_lt x))]]
  exact sext32_bv _ (Nat.lt_of_le_of_lt (Nat.shiftRight_le _ k) (mod32_lt x))

theorem zext_bit (b : Bool) :
    zero_extend (m := 64) (bool_to_bit b) = BitVec.ofNat 64 (if b then 1 else 0) := by
  cases b <;> rfl

theorem slt_bv (x y : Nat) (hx : x < 2 ^ 64) (hy : y < 2 ^ 64) :
    zopz0zI_s (BitVec.ofNat 64 x) (BitVec.ofNat 64 y) = slt64 x y := by
  have := guard_blt x y hx hy; simpa [guardB, bLt] using this

theorem sll_bv (x y : Nat) (hy : y < 2 ^ 64) :
    shift_bits_left (BitVec.ofNat 64 x) (Sail.BitVec.extractLsb (BitVec.ofNat 64 y) 5 0) =
      BitVec.ofNat 64 (fSll x y) := by
  apply BitVec.eq_of_toNat_eq
  simp only [shift_bits_left, BitVec.shiftLeft_eq', BitVec.toNat_shiftLeft, BitVec.toNat_ofNat,
    shamt_reg y hy, fSll, W]
  rw [show (18446744073709551616 : Nat) = 2 ^ 64 from rfl, Nat.mod_mod]
  exact shl_mod _ _ _ _ (Nat.mod_mod _ _)

theorem srl_bv (x y : Nat) (hx : x < 2 ^ 64) (hy : y < 2 ^ 64) :
    shift_bits_right (BitVec.ofNat 64 x) (Sail.BitVec.extractLsb (BitVec.ofNat 64 y) 5 0) =
      BitVec.ofNat 64 (fSrl x y) := by
  apply BitVec.eq_of_toNat_eq
  simp only [shift_bits_right, BitVec.ushiftRight_eq', BitVec.toNat_ushiftRight, BitVec.toNat_ofNat,
    shamt_reg y hy, fSrl, Nat.mod_eq_of_lt hx]
  rw [Nat.mod_eq_of_lt (Nat.lt_of_le_of_lt (Nat.shiftRight_le _ _) hx)]

theorem sound_addiw {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s : St} {cf : Config} (hS : Sim c ab s cf)
    {w rd rs1 imm : Nat} (hget : c.table.getW s.pc = some (w, .addiw rd rs1 imm)) :
    Reaches c ab cf ⟨(s.pc + 4) % W, setR s.regs rd (fAddw (getR s.regs rs1) imm), s.mem⟩ := by
  obtain ⟨hrd, hrs1, himm, hdec⟩ := (hT _ _ _ hget).dec
  refine sound_ri hT hS hget hdec hrd ⟨hrd, srcOK_of hrs1⟩ trivial ?_ rfl
  show (rd, sign_extend (m := 64) (Sail.BitVec.extractLsb (srcVal rs1 (pinsOf s.regs) +
    sign_extend (m := 64) (BitVec.ofNat 12 imm)) 31 0)) :: _ = _
  rw [srcVal_pinsOf hS.x0 hrs1, wv_addiw _ _ himm]; rfl

theorem sound_addw {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s : St} {cf : Config} (hS : Sim c ab s cf)
    {w rd rs1 rs2 : Nat} (hget : c.table.getW s.pc = some (w, .addw rd rs1 rs2)) :
    Reaches c ab cf ⟨(s.pc + 4) % W, setR s.regs rd (fAddw (getR s.regs rs1) (getR s.regs rs2)), s.mem⟩ := by
  have hk := (hT _ _ _ hget).dec
  refine sound_rr hT hS hget hk ⟨hk.1, srcOK_of hk.2.1, srcOK_of hk.2.2.1⟩ trivial ?_ rfl
  show (rd, sign_extend (m := 64) (Sail.BitVec.extractLsb (srcVal rs1 (pinsOf s.regs)) 31 0 +
    Sail.BitVec.extractLsb (srcVal rs2 (pinsOf s.regs)) 31 0)) :: _ = _
  rw [srcVal_pinsOf hS.x0 hk.2.1, srcVal_pinsOf hS.x0 hk.2.2.1, wv_addw]; rfl

theorem sound_subw {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s : St} {cf : Config} (hS : Sim c ab s cf)
    {w rd rs1 rs2 : Nat} (hget : c.table.getW s.pc = some (w, .subw rd rs1 rs2)) :
    Reaches c ab cf ⟨(s.pc + 4) % W, setR s.regs rd (fSubw (getR s.regs rs1) (getR s.regs rs2)), s.mem⟩ := by
  have hk := (hT _ _ _ hget).dec
  refine sound_rr hT hS hget hk ⟨hk.1, srcOK_of hk.2.1, srcOK_of hk.2.2.1⟩ trivial ?_ rfl
  show (rd, sign_extend (m := 64) (Sail.BitVec.extractLsb (srcVal rs1 (pinsOf s.regs)) 31 0 -
    Sail.BitVec.extractLsb (srcVal rs2 (pinsOf s.regs)) 31 0)) :: _ = _
  rw [srcVal_pinsOf hS.x0 hk.2.1, srcVal_pinsOf hS.x0 hk.2.2.1, wv_subw]; rfl

theorem sound_slliw {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s : St} {cf : Config} (hS : Sim c ab s cf)
    {w rd rs1 k : Nat} (hget : c.table.getW s.pc = some (w, .slliw rd rs1 k)) :
    Reaches c ab cf ⟨(s.pc + 4) % W, setR s.regs rd (fSlliw (getR s.regs rs1) k), s.mem⟩ := by
  obtain ⟨hrd, hrs1, hk, hsh, hdec⟩ := (hT _ _ _ hget).dec
  refine sound_ri hT hS hget hdec hrd ⟨hrd, srcOK_of hrs1⟩ trivial ?_ rfl
  show (rd, sign_extend (m := 64) (shift_bits_left (Sail.BitVec.extractLsb (srcVal rs1 (pinsOf s.regs)) 31 0)
    ((BitVec.ofNat 32 w).extractLsb' 20 5))) :: _ = _
  rw [hsh, srcVal_pinsOf hS.x0 hrs1, wv_slliw _ _ hk]; rfl

theorem sound_srliw {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s : St} {cf : Config} (hS : Sim c ab s cf)
    {w rd rs1 k : Nat} (hget : c.table.getW s.pc = some (w, .srliw rd rs1 k)) :
    Reaches c ab cf ⟨(s.pc + 4) % W, setR s.regs rd (fSrliw (getR s.regs rs1) k), s.mem⟩ := by
  obtain ⟨hrd, hrs1, hk, hsh, hdec⟩ := (hT _ _ _ hget).dec
  refine sound_ri hT hS hget hdec hrd ⟨hrd, srcOK_of hrs1⟩ trivial ?_ rfl
  show (rd, sign_extend (m := 64) (shift_bits_right (Sail.BitVec.extractLsb (srcVal rs1 (pinsOf s.regs)) 31 0)
    ((BitVec.ofNat 32 w).extractLsb' 20 5))) :: _ = _
  rw [hsh, srcVal_pinsOf hS.x0 hrs1, wv_srliw _ _ hk]; rfl

theorem sound_slt {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s : St} {cf : Config} (hS : Sim c ab s cf)
    {w rd rs1 rs2 : Nat} (hget : c.table.getW s.pc = some (w, .slt rd rs1 rs2)) :
    Reaches c ab cf ⟨(s.pc + 4) % W, setR s.regs rd (fSlt (getR s.regs rs1) (getR s.regs rs2)), s.mem⟩ := by
  have hk := (hT _ _ _ hget).dec
  refine sound_rr hT hS hget hk ⟨hk.1, srcOK_of hk.2.1, srcOK_of hk.2.2.1⟩ trivial ?_ rfl
  show (rd, zero_extend (m := 64) (bool_to_bit (zopz0zI_s (srcVal rs1 (pinsOf s.regs))
    (srcVal rs2 (pinsOf s.regs))))) :: _ = _
  rw [srcVal_pinsOf hS.x0 hk.2.1, srcVal_pinsOf hS.x0 hk.2.2.1,
    slt_bv _ _ (getR_lt _ _) (getR_lt _ _), zext_bit]; rfl

theorem sound_slti {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s : St} {cf : Config} (hS : Sim c ab s cf)
    {w rd rs1 imm : Nat} (hget : c.table.getW s.pc = some (w, .slti rd rs1 imm)) :
    Reaches c ab cf ⟨(s.pc + 4) % W, setR s.regs rd (fSlt (getR s.regs rs1) imm), s.mem⟩ := by
  obtain ⟨hrd, hrs1, himm, hlt, hdec⟩ := (hT _ _ _ hget).dec
  refine sound_ri hT hS hget hdec hrd ⟨hrd, srcOK_of hrs1⟩ trivial ?_ rfl
  show (rd, zero_extend (m := 64) (bool_to_bit (zopz0zI_s (srcVal rs1 (pinsOf s.regs))
    (sign_extend (m := 64) (BitVec.ofNat 12 imm))))) :: _ = _
  rw [himm, srcVal_pinsOf hS.x0 hrs1, slt_bv _ _ (getR_lt _ _) hlt, zext_bit]; rfl

theorem sound_sll {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s : St} {cf : Config} (hS : Sim c ab s cf)
    {w rd rs1 rs2 : Nat} (hget : c.table.getW s.pc = some (w, .sll rd rs1 rs2)) :
    Reaches c ab cf ⟨(s.pc + 4) % W, setR s.regs rd (fSll (getR s.regs rs1) (getR s.regs rs2)), s.mem⟩ := by
  have hk := (hT _ _ _ hget).dec
  refine sound_rr hT hS hget hk ⟨hk.1, srcOK_of hk.2.1, srcOK_of hk.2.2.1⟩ trivial ?_ rfl
  show (rd, shift_bits_left (srcVal rs1 (pinsOf s.regs))
    (Sail.BitVec.extractLsb (srcVal rs2 (pinsOf s.regs)) 5 0)) :: _ = _
  rw [srcVal_pinsOf hS.x0 hk.2.1, srcVal_pinsOf hS.x0 hk.2.2.1, sll_bv _ _ (getR_lt _ _)]; rfl

theorem sound_srl {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s : St} {cf : Config} (hS : Sim c ab s cf)
    {w rd rs1 rs2 : Nat} (hget : c.table.getW s.pc = some (w, .srl rd rs1 rs2)) :
    Reaches c ab cf ⟨(s.pc + 4) % W, setR s.regs rd (fSrl (getR s.regs rs1) (getR s.regs rs2)), s.mem⟩ := by
  have hk := (hT _ _ _ hget).dec
  refine sound_rr hT hS hget hk ⟨hk.1, srcOK_of hk.2.1, srcOK_of hk.2.2.1⟩ trivial ?_ rfl
  show (rd, shift_bits_right (srcVal rs1 (pinsOf s.regs))
    (Sail.BitVec.extractLsb (srcVal rs2 (pinsOf s.regs)) 5 0)) :: _ = _
  rw [srcVal_pinsOf hS.x0 hk.2.1, srcVal_pinsOf hS.x0 hk.2.2.1,
    srl_bv _ _ (getR_lt _ _) (getR_lt _ _)]; rfl

/-! ## Loads and stores of every width (generic) -/

theorem bytes4 (v : Nat) :
    (((vbyte v 3).append (vbyte v 2)).append (vbyte v 1)).append (vbyte v 0) = BitVec.ofNat 32 v := by
  apply BitVec.eq_of_getElem_eq
  intro i hi
  simp only [BitVec.append_eq, BitVec.getElem_append]
  simp only [BitVec.getElem_eq_testBit_toNat, BitVec.toNat_ofNat, Nat.testBit_mod_two_pow,
    testBit_fldAt]
  have hi' : i < 32 := hi
  repeat' split
  all_goals first
    | (simp only [show i - 8 - 8 - 8 < 8 by omega, decide_true, Bool.true_and, hi'];
       congr 1; omega)
    | (simp only [decide_eq_true_eq, hi', decide_true, Bool.true_and, *]; congr 1; omega)
    | omega

theorem bytes2 (v : Nat) : (vbyte v 1).append (vbyte v 0) = BitVec.ofNat 16 v := by
  apply BitVec.eq_of_getElem_eq
  intro i hi
  simp only [BitVec.append_eq, BitVec.getElem_append]
  simp only [BitVec.getElem_eq_testBit_toNat, BitVec.toNat_ofNat, Nat.testBit_mod_two_pow,
    testBit_fldAt]
  have hi' : i < 16 := hi
  repeat' split
  all_goals first
    | (simp only [show i - 8 < 8 by omega, decide_true, Bool.true_and, hi'];
       congr 1; omega)
    | (simp only [decide_eq_true_eq, hi', decide_true, Bool.true_and, *]; congr 1; omega)
    | omega

theorem sext16_bv (v : Nat) (hv : v < 2 ^ 16) :
    sign_extend (m := 64) (BitVec.ofNat 16 v) = BitVec.ofNat 64 (sextH v) := by
  apply BitVec.eq_of_toNat_eq
  simp [sign_extend, Sail.BitVec.signExtend, BitVec.toNat_signExtend, sextH, BitVec.msb_eq_decide]
  by_cases h : v < 32768
  · simp [h, show ¬ 32768 ≤ v % 65536 by omega]; omega
  · simp [h, show 32768 ≤ v % 65536 by omega, W]; omega

theorem zext_bv {n : Nat} (v : Nat) (hv : v < 2 ^ n) (hn : n ≤ 64) :
    zero_extend (m := 64) (BitVec.ofNat n v) = BitVec.ofNat 64 v := by
  apply BitVec.eq_of_toNat_eq
  simp only [zero_extend, Sail.BitVec.zeroExtend, BitVec.toNat_setWidth, BitVec.toNat_ofNat]
  rw [Nat.mod_eq_of_lt hv, Nat.mod_eq_of_lt (Nat.lt_of_lt_of_le hv (Nat.pow_le_pow_right (by decide) hn))]

/-- A load entry: the evaluator's `ldI` at width `wd` with extension `ext`,
the Sail value of the loaded bytes, and the kind's `MemFacts` from the
relation's byte pins. -/
theorem load_sound {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s s' : St} {cf : Config} (hS : Sim c ab s cf)
    {w rd rs1 imm : Nat} {i : Ins} (hget : c.table.getW s.pc = some (w, i)) {k : MKind}
    {wd : Nat} {ext : Nat → Nat}
    (hk : (1 ≤ rd ∧ rd ≤ 31) ∧ rs1 ≤ 31 ∧ Imm12 imm ∧ DecodeFactM (mkM s.pc w k rd rs1 0 imm))
    (hkind : KindOK (keysG (pinsOf s.regs)) k rd rs1 0)
    (hex : ldI s rd rs1 imm wd ext = some s')
    (bsOf : Nat → List (BitVec 8))
    (hmf : ∀ a v, loadOK a wd = true → (∀ j, j < wd → (cf.σ.mem[a + j]?).getD 0 = vbyte v j) →
      (eaddrM (mkM s.pc w k rd rs1 0 imm) (pinsOf s.regs)).toNat = a →
      MemFacts cf.σ.mem (pinsOf s.regs) (bsOf v) (mkM s.pc w k rd rs1 0 imm))
    (hval : ∀ v, v < 2 ^ (8 * wd) →
      wvalM (mkM s.pc w k rd rs1 0 imm) (pinsOf s.regs) (bsOf v) = BitVec.ofNat 64 (ext v))
    (hgm : ∀ bs, runGM [mkM s.pc w k rd rs1 0 imm] (pinsOf s.regs) [bs] =
      (rd, wvalM (mkM s.pc w k rd rs1 0 imm) (pinsOf s.regs) bs) :: eraseG rd (pinsOf s.regs))
    (hlog : ∀ bs, wlogM [mkM s.pc w k rd rs1 0 imm] (pinsOf s.regs) [bs] = []) :
    Reaches c ab cf s' := by
  obtain ⟨hrd, hrs1, himm, hdec⟩ := hk
  have hf := (hT _ _ _ hget).fetch
  unfold ldI at hex
  generalize ha : (getR s.regs rs1 + imm) % W = a at hex
  cases hok : loadOK a wd with
  | false => simp [hok] at hex
  | true =>
    cases hl : load s.mem a wd with
    | none => simp [hok, hl] at hex
    | some v =>
      simp only [hok, hl, Option.some.injEq] at hex
      subst hex
      show Reaches c ab cf ⟨(s.pc + 4) % W, setR s.regs rd (ext v), s.mem⟩
      obtain ⟨hv, hbytes⟩ := load_bytes hl
      have hea := eaddr_mkM (k := k) (pc := s.pc) (w := w) (rd := rd) (rs2 := 0) hS.x0 hrs1 himm
      rw [ha] at hea
      have heat : (eaddrM (mkM s.pc w k rd rs1 0 imm) (pinsOf s.regs)).toNat = a := by
        rw [hea, ← ha, ea_toNat]
      obtain ⟨σ', t', hs, ht', hG', hmem', hout, hpc', hregs, hpay⟩ :=
        mstep hS hf rfl rfl ⟨rfl, rfl, rfl, rfl⟩ (hS.code _ _ _ hget) hdec hkind
          (hmf a v hok (fun j hj => (hS.mem _ _ (hbytes j hj)).read) heat)
      have hregs' : GHolds σ' ((rd, BitVec.ofNat 64 (ext v)) :: eraseG rd (pinsOf s.regs)) := by
        rw [hgm, hval v hv] at hregs; exact hregs
      have hm : σ'.mem = cf.σ.mem := by rw [hmem', hlog]; rfl
      refine ⟨_, hs, ⟨hG', ?_, ?_, getR_setR_zero _ hS.x0, ?_, ?_, hout, hpay, ht'⟩⟩
      · rw [hpc']; simp only [mkM]; rw [addInt_ofNat]
      · have := pins_setR hrd.1 hrd.2 hregs'
        rwa [BitVec.toNat_ofNat, setR_mod] at this
      · intro x b hb; rw [hm]; exact hS.mem x b hb
      · rw [hm]; exact hS.code

/-- The 4-byte pins of `MemFacts` from the relation's. -/
theorem lpins4_of {m : Std.ExtHashMap Nat (BitVec 8)} {a v : Nat}
    (h : ∀ j, j < 4 → (m[a + j]?).getD 0 = vbyte v j) :
    LPins4 m a [vbyte v 0, vbyte v 1, vbyte v 2, vbyte v 3] :=
  ⟨by simpa using h 0 (by omega), h 1 (by omega), h 2 (by omega), h 3 (by omega)⟩

theorem sound_lw {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s s' : St} {cf : Config} (hS : Sim c ab s cf)
    {w rd rs1 imm : Nat} (hget : c.table.getW s.pc = some (w, .lw rd rs1 imm))
    (hex : exec (.lw rd rs1 imm) c s = some s') : Reaches c ab cf s' := by
  have hk := (hT _ _ _ hget).dec
  refine load_sound hT hS hget (k := .lw) hk ⟨hk.1, srcOK_of hk.2.1⟩ hex
    (fun v => [vbyte v 0, vbyte v 1, vbyte v 2, vbyte v 3])
    (fun a v hok hpin heat => by
      show (_ ∧ _ ∧ _) ∧ LPins4 _ _ _
      rw [heat]; exact ⟨loadOK_iff hok, lpins4_of hpin⟩)
    (fun v hv => by
      show sign_extend (m := 64) _ = _
      simp only [List.getD, List.getElem?_cons_succ, List.getElem?_cons_zero, Option.getD_some]
      rw [bytes4]; exact sext32_bv v hv) (fun _ => rfl) (fun _ => rfl)

theorem sound_lwu {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s s' : St} {cf : Config} (hS : Sim c ab s cf)
    {w rd rs1 imm : Nat} (hget : c.table.getW s.pc = some (w, .lwu rd rs1 imm))
    (hex : exec (.lwu rd rs1 imm) c s = some s') : Reaches c ab cf s' := by
  have hk := (hT _ _ _ hget).dec
  refine load_sound hT hS hget (k := .lwu) hk ⟨hk.1, srcOK_of hk.2.1⟩ hex
    (fun v => [vbyte v 0, vbyte v 1, vbyte v 2, vbyte v 3])
    (fun a v hok hpin heat => by
      show (_ ∧ _ ∧ _) ∧ LPins4 _ _ _
      rw [heat]; exact ⟨loadOK_iff hok, lpins4_of hpin⟩)
    (fun v hv => by
      simp only [wvalM, bytesVal, mkM, List.getD, List.getElem?_cons_succ, List.getElem?_cons_zero,
        Option.getD_some]
      rw [bytes4]; exact zext_bv v hv (by omega)) (fun _ => rfl) (fun _ => rfl)

theorem sound_lh {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s s' : St} {cf : Config} (hS : Sim c ab s cf)
    {w rd rs1 imm : Nat} (hget : c.table.getW s.pc = some (w, .lh rd rs1 imm))
    (hex : exec (.lh rd rs1 imm) c s = some s') : Reaches c ab cf s' := by
  have hk := (hT _ _ _ hget).dec
  refine load_sound hT hS hget (k := .lh) hk ⟨hk.1, srcOK_of hk.2.1⟩ hex
    (fun v => [vbyte v 0, vbyte v 1])
    (fun a v hok hpin heat => by
      show (_ ∧ _ ∧ _) ∧ _ ∧ _
      rw [heat]
      exact ⟨loadOK_iff hok, by simpa using hpin 0 (by omega), hpin 1 (by omega)⟩)
    (fun v hv => by
      show sign_extend (m := 64) _ = _
      simp only [List.getD, List.getElem?_cons_succ, List.getElem?_cons_zero, Option.getD_some]
      rw [bytes2]; exact sext16_bv v hv) (fun _ => rfl) (fun _ => rfl)

theorem sound_lhu {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s s' : St} {cf : Config} (hS : Sim c ab s cf)
    {w rd rs1 imm : Nat} (hget : c.table.getW s.pc = some (w, .lhu rd rs1 imm))
    (hex : exec (.lhu rd rs1 imm) c s = some s') : Reaches c ab cf s' := by
  have hk := (hT _ _ _ hget).dec
  refine load_sound hT hS hget (k := .lhu) hk ⟨hk.1, srcOK_of hk.2.1⟩ hex
    (fun v => [vbyte v 0, vbyte v 1])
    (fun a v hok hpin heat => by
      show (_ ∧ _ ∧ _) ∧ _ ∧ _
      rw [heat]
      exact ⟨loadOK_iff hok, by simpa using hpin 0 (by omega), hpin 1 (by omega)⟩)
    (fun v hv => by
      simp only [wvalM, bytesVal, mkM, List.getD, List.getElem?_cons_succ, List.getElem?_cons_zero,
        Option.getD_some]
      rw [bytes2]; exact zext_bv v hv (by omega)) (fun _ => rfl) (fun _ => rfl)

theorem low_byte (v j hi : Nat) (hj : 8 * j + 8 ≤ hi + 1) (hhi : hi < 64) :
    (Sail.BitVec.extractLsb (BitVec.ofNat 64 v) hi 0).extractLsb' (8 * j) 8 =
      BitVec.ofNat 8 (fldAt (8 * j) 8 v) := by
  apply BitVec.eq_of_toNat_eq
  simp only [BitVec.extractLsb'_toNat, Sail.BitVec.extractLsb, BitVec.extractLsb_toNat,
    BitVec.toNat_ofNat, Nat.shiftRight_zero, Nat.sub_zero]
  show _ = fldAt (8 * j) 8 v % 2 ^ 8
  rw [Nat.mod_eq_of_lt (fldAt_lt _ _ _)]
  have e1 : v % 2 ^ 64 % 2 ^ (hi + 1) = fldAt 0 (hi + 1) v := by
    simp only [fldAt, Nat.shiftRight_zero]
    exact Nat.mod_mod_of_dvd _ (Nat.pow_dvd_pow 2 (by omega))
  rw [e1]
  show fldAt (8 * j) 8 (fldAt 0 (hi + 1) v) = _
  rw [fldAt_fldAt v (by omega), Nat.zero_add]

/-- A store entry: the evaluator's `stI` at width `wd`, the kind's `MemFacts`
from the store window, and the write log's bytes. -/
theorem store_sound {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s s' : St} {cf : Config} (hS : Sim c ab s cf)
    {w rs1 rs2 imm : Nat} {i : Ins} (hget : c.table.getW s.pc = some (w, i)) {k : MKind}
    {wd : Nat}
    (hk : rs1 ≤ 31 ∧ rs2 ≤ 31 ∧ Imm12 imm ∧ DecodeFactM (mkM s.pc w k 0 rs1 rs2 imm))
    (hkind : KindOK (keysG (pinsOf s.regs)) k 0 rs1 rs2)
    (hex : stI c s rs1 rs2 imm wd = some s')
    (hmf : ∀ a, storeOK c a wd = true →
      (eaddrM (mkM s.pc w k 0 rs1 rs2 imm) (pinsOf s.regs)).toNat = a →
      MemFacts cf.σ.mem (pinsOf s.regs) [] (mkM s.pc w k 0 rs1 rs2 imm))
    (hlog : wlogM [mkM s.pc w k 0 rs1 rs2 imm] (pinsOf s.regs) [[]] =
      [((eaddrM (mkM s.pc w k 0 rs1 rs2 imm) (pinsOf s.regs)).toNat, wd, srcVal rs2 (pinsOf s.regs))])
    (hgm : runGM [mkM s.pc w k 0 rs1 rs2 imm] (pinsOf s.regs) [[]] = pinsOf s.regs)
    (hin : ∀ a x v, a ≤ x → x < a + wd →
      entryByte (a, wd, BitVec.ofNat 64 v) x = some (BitVec.ofNat 8 (fldAt (8 * (x - a)) 8 v)))
    (hout : ∀ a x d, ¬ (a ≤ x ∧ x < a + wd) → entryByte (a, wd, d) x = none) :
    Reaches c ab cf s' := by
  obtain ⟨hrs1, hrs2, himm, hdec⟩ := hk
  have hf := (hT _ _ _ hget).fetch
  unfold stI at hex
  generalize ha : (getR s.regs rs1 + imm) % W = a at hex
  cases hok : storeOK c a wd with
  | false => simp [hok] at hex
  | true =>
    cases hst : store s.mem a wd (getR s.regs rs2) with
    | none => simp [hok, hst] at hex
    | some M' =>
      simp only [hok, hst, Option.some.injEq] at hex
      subst hex
      obtain ⟨_, _, _, _, hoff⟩ := storeOK_iff hok
      have hea := eaddr_mkM (k := k) (pc := s.pc) (w := w) (rd := 0) (rs2 := rs2) hS.x0 hrs1 himm
      rw [ha] at hea
      have heat : (eaddrM (mkM s.pc w k 0 rs1 rs2 imm) (pinsOf s.regs)).toNat = a := by
        rw [hea, ← ha, ea_toNat]
      obtain ⟨σ', t', hs, ht', hG', hmem', hout', hpc', hregs, hpay⟩ :=
        mstep hS hf rfl rfl ⟨rfl, rfl, rfl, rfl⟩ (hS.code _ _ _ hget) hdec hkind (hmf a hok heat)
      have hm : σ'.mem = applyW cf.σ.mem (a, wd, BitVec.ofNat 64 (getR s.regs rs2)) := by
        rw [hmem', hlog, heat, srcVal_pinsOf hS.x0 hrs2]; rfl
      obtain ⟨hmem2, hcode2⟩ := mem_after_store (c := c)
        (e := (a, wd, BitVec.ofNat 64 (getR s.regs rs2))) hst hS.mem
        (fun x h1 h2 => hin a x _ h1 h2) (fun x hx => hout a x _ hx) hS.code hoff hT
      rw [hgm] at hregs
      refine ⟨_, hs, ⟨hG', ?_, hregs, hS.x0, ?_, ?_, hout', hpay, ht'⟩⟩
      · rw [hpc']; simp only [mkM]; rw [addInt_ofNat]
      · rw [hm]; exact hmem2
      · rw [hm]; exact hcode2

theorem sound_sw {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s s' : St} {cf : Config} (hS : Sim c ab s cf)
    {w rs1 rs2 imm : Nat} (hget : c.table.getW s.pc = some (w, .sw rs1 rs2 imm))
    (hex : exec (.sw rs1 rs2 imm) c s = some s') : Reaches c ab cf s' := by
  have hk := (hT _ _ _ hget).dec
  refine store_sound hT hS hget (k := .sw) hk ⟨srcOK_of hk.1, srcOK_of hk.2.1⟩ hex
    (fun a hok heat => by
      obtain ⟨h1, h2, h3, h4, _⟩ := storeOK_iff hok
      show _ ∧ _ ∧ _ ∧ _
      rw [heat]; exact ⟨h1, h2, h3, h4⟩) rfl rfl
    (fun a x v h1 h2 => by
      show (if a ≤ x ∧ x < a + 4 then some _ else none) = _
      rw [if_pos ⟨h1, h2⟩]
      exact congrArg some (low_byte v (x - a) _ (by simp; omega) (by simp)))
    (fun a x d hx => by
      show (if a ≤ x ∧ x < a + 4 then some _ else none) = _
      rw [if_neg hx])

theorem sound_sh {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s s' : St} {cf : Config} (hS : Sim c ab s cf)
    {w rs1 rs2 imm : Nat} (hget : c.table.getW s.pc = some (w, .sh rs1 rs2 imm))
    (hex : exec (.sh rs1 rs2 imm) c s = some s') : Reaches c ab cf s' := by
  have hk := (hT _ _ _ hget).dec
  refine store_sound hT hS hget (k := .sh) hk ⟨srcOK_of hk.1, srcOK_of hk.2.1⟩ hex
    (fun a hok heat => by
      obtain ⟨h1, h2, h3, h4, _⟩ := storeOK_iff hok
      show _ ∧ _ ∧ _ ∧ _
      rw [heat]; exact ⟨h1, h2, h3, h4⟩) rfl rfl
    (fun a x v h1 h2 => by
      show (if x = a then some _ else if x = a + 1 then some _ else none) = _
      by_cases hx : x = a
      · subst hx; rw [if_pos rfl]
        exact congrArg some (by simpa using low_byte v 0 15 (by omega) (by omega))
      · rw [if_neg hx, if_pos (by omega)]
        exact congrArg some (by
          have := low_byte v 1 15 (by omega) (by omega)
          rw [show x - a = 1 by omega]; simpa using this))
    (fun a x d hx => by
      show (if x = a then some _ else if x = a + 1 then some _ else none) = _
      rw [if_neg (by omega), if_neg (by omega)])

theorem sound_sb {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s s' : St} {cf : Config} (hS : Sim c ab s cf)
    {w rs1 rs2 imm : Nat} (hget : c.table.getW s.pc = some (w, .sb rs1 rs2 imm))
    (hex : exec (.sb rs1 rs2 imm) c s = some s') : Reaches c ab cf s' := by
  have hk := (hT _ _ _ hget).dec
  refine store_sound hT hS hget (k := .sb) hk ⟨srcOK_of hk.1, srcOK_of hk.2.1⟩ hex
    (fun a hok heat => by
      obtain ⟨h1, h2, h3, _, _⟩ := storeOK_iff hok
      show _ ∧ _ ∧ _
      rw [heat]; exact ⟨h1, h2, h3⟩) rfl rfl
    (fun a x v h1 h2 => by
      show (if x = a then some _ else none) = _
      have hx : x = a := by omega
      subst hx; rw [if_pos rfl]
      have := low_byte v 0 7 (by omega) (by omega)
      simp only [Nat.sub_self]
      apply congrArg some
      rw [← this]
      apply BitVec.eq_of_toNat_eq
      simp [sbData, Sail.BitVec.extractLsb])
    (fun a x d hx => by
      show (if x = a then some _ else none) = _
      rw [if_neg (by omega)])

/-! ## ALU kinds without a block kind (`stepObs_alu` with an execute characterization) -/

/-- An ALU step whose execute writes `x_rd := v` (`hexec`, from an
`ExecuteAlu` characterization), from the relation. After `row_alu_core`. -/
theorem core_sound {c : Code} {ab : Nat → Bool} {s : St} {cf : Config} (hS : Sim c ab s cf) {w : Nat}
    (hf : Fetchable c s.pc w) {i : Ins} (hget : c.table.getW s.pc = some (w, i))
    {ast : instruction} {rd v : Nat}
    (hdec : ∀ s : SequentialState RegisterType trivialChoiceSource,
      s.regs.get? Register.misa = some ((Vsa.Sim.initMisa) : RegisterType Register.misa) →
      s.regs.get? Register.cur_privilege =
        some ((Privilege.Machine) : RegisterType Register.cur_privilege) →
      s.regs.get? Register.mseccfg = some ((0#64) : RegisterType Register.mseccfg) →
      (ext_decode (BitVec.ofNat 32 w)).run s = .ok ast s)
    (hrd : 1 ≤ rd ∧ rd ≤ 31)
    (hexec : ∀ σ : MState, GoodState σ → GHolds σ (pinsOf s.regs) →
      (execute ast).run (afterNextPC (afterPrelude σ) (BitVec.ofNat 64 s.pc))
        = .ok RETIRE_SUCCESS (sigma3_alu σ (BitVec.ofNat 64 s.pc) (gprReg rd) (gprRT rd (BitVec.ofNat 64 v)))) :
    Reaches c ab cf ⟨(s.pc + 4) % W, setR s.regs rd v, s.mem⟩ := by
  obtain ⟨σ, t, u⟩ := cf
  obtain ⟨vm, hmi⟩ := hS.good.minstret
  have hpcn := pc_toNat hf.fetch
  have hcode := hS.code _ _ _ hget
  obtain ⟨hr1, hr2, hr3, hr4, hr5⟩ := gpr_rd_ok rd (by omega) hrd.1
  obtain ⟨σ', i', hs, hi', hG', hmem', hobs⟩ :=
    stepObs_alu σ t u (BitVec.ofNat 64 s.pc) vm _ ast (gprReg rd) (gprRT rd (BitVec.ofNat 64 v))
      _ _ _ _ hS.good hS.pc hmi hf.word (by rw [hf.word]; exact hf.rvc) (dec_at hS.good hdec)
      (hexec σ hS.good hS.regs) hr1 hr2 hr3 hr4 hr5
      (by rw [hpcn]; exact hcode.1) (by rw [hpcn]; exact hcode.2.1)
      (by rw [hpcn]; exact hcode.2.2.1) (by rw [hpcn]; exact hcode.2.2.2)
      (by rw [hpcn]; exact hf.base) (by rw [hpcn]; exact hf.fetch) (by rw [hpcn]; exact hf.align)
      hS.tick
  have hgpr : ∀ n, 1 ≤ n → n ≤ 31 → n ≠ rd →
      σ'.regs.get? (gprReg n) = σ.regs.get? (gprReg n) := fun n h1 h31 hne =>
    (hobs.1 _ (notGpr_mcycle n) (notGpr_mtime n) (notGpr_mip n)).trans
      (get?_sigmaPost_alu σ _ vm _ _ _ (notGpr_minstret n) (notGpr_PC n)
        (gprReg_beq_false rd (by omega) n (by omega) hrd.1 h1 (Ne.symm hne))
        (notGpr_nextPC n) (notGpr_minstret_increment n))
  have hrdv : σ'.regs.get? (gprReg rd) = some (gprRT rd (BitVec.ofNat 64 v)) := by
    rw [hobs.1 _ (notGpr_mcycle rd) (notGpr_mtime rd) (notGpr_mip rd)]
    show ((((sigma3_alu σ _ (gprReg rd) _).regs.insert Register.PC _).insert
      Register.minstret _)).get? (gprReg rd) = _
    rw [Std.ExtDHashMap.get?_insert]
    simp only [notGpr_minstret rd, dif_neg, reduceCtorEq, not_false_eq_true]
    rw [Std.ExtDHashMap.get?_insert]
    simp only [notGpr_PC rd, dif_neg, reduceCtorEq, not_false_eq_true]
    show ((_ : Std.ExtDHashMap Register RegisterType).insert (gprReg rd) _).get? (gprReg rd) = _
    rw [Std.ExtDHashMap.get?_insert_self]
  have hpc' : σ'.regs.get? Register.PC = some (BitVec.ofNat 64 ((s.pc + 4) % W)) := by
    rw [hobs.1 _ (by decide) (by decide) (by decide)]
    show ((((sigma3_alu σ _ (gprReg rd) _).regs.insert Register.PC _).insert
      Register.minstret _)).get? Register.PC = _
    rw [Std.ExtDHashMap.get?_insert]
    simp only [show (Register.minstret == Register.PC) = false from by decide, dif_neg,
      reduceCtorEq, not_false_eq_true]
    rw [Std.ExtDHashMap.get?_insert_self, addInt_ofNat]
  have hpay : σ'.regs.get? Register.htif_payload_writes =
      σ.regs.get? Register.htif_payload_writes := by
    rw [hobs.1 _ (by decide) (by decide) (by decide),
      get?_sigmaPost_alu σ _ vm _ _ _ (by decide) (by decide)
        (by rw [Bool.beq_comm]; exact notGpr_payload rd) (by decide) (by decide)]
  have := sim_link hS hrd hG' hi' hmem' hpc' hrdv hgpr hobs.2 hpay (u' := u + 1)
  rw [BitVec.toNat_ofNat, setR_mod] at this
  exact ⟨_, Steps.head hs (Steps.refl _), this⟩

theorem sltu_bv (x y : Nat) (hx : x < 2 ^ 64) (hy : y < 2 ^ 64) :
    zero_extend (m := 64) (bool_to_bit (zopz0zI_u (BitVec.ofNat 64 x) (BitVec.ofNat 64 y))) =
      BitVec.ofNat 64 (fSltu x y) := by
  have := guard_bltu x y hx hy
  simp only [guardB, bLtu] at this
  rw [this, zext_bit]; simp [fSltu]

theorem srcPin_pins {σ : MState} {R n : Nat} (hn : n ≤ 31) (h : GHolds σ (pinsOf R)) :
    srcPin σ n (srcVal n (pinsOf R)) :=
  srcPin_srcVal σ (pinsOf R) n (by
    by_cases h0 : n = 0
    · exact Or.inl h0
    · exact Or.inr (mem_keys_pinsOf.2 ⟨by omega, hn⟩)) h

theorem sound_sltu {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s : St} {cf : Config} (hS : Sim c ab s cf)
    {w rd rs1 rs2 : Nat} (hget : c.table.getW s.pc = some (w, .sltu rd rs1 rs2)) :
    Reaches c ab cf ⟨(s.pc + 4) % W, setR s.regs rd (fSltu (getR s.regs rs1) (getR s.regs rs2)), s.mem⟩ := by
  obtain ⟨hrd, hrs1, hrs2, hdec⟩ := (hT _ _ _ hget).dec
  refine core_sound hS (hT _ _ _ hget).fetch hget hdec hrd (fun σ _ hL => ?_)
  have hv := sltu_bv _ _ (getR_lt s.regs rs1) (getR_lt s.regs rs2)
  rw [← srcVal_pinsOf hS.x0 hrs1, ← srcVal_pinsOf hS.x0 hrs2] at hv
  rw [← hv]
  exact execute_rtype_sltu_char _ _ _ _ _ _ _
    (rX_src σ _ rs1 hrs1 _ (srcPin_pins hrs1 hL)) (rX_src σ _ rs2 hrs2 _ (srcPin_pins hrs2 hL))
    (wX_gpr _ _ rd hrd.1 hrd.2)

theorem sound_sltiu {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s : St} {cf : Config} (hS : Sim c ab s cf)
    {w rd rs1 imm : Nat} (hget : c.table.getW s.pc = some (w, .sltiu rd rs1 imm)) :
    Reaches c ab cf ⟨(s.pc + 4) % W, setR s.regs rd (fSltu (getR s.regs rs1) imm), s.mem⟩ := by
  obtain ⟨hrd, hrs1, himm, hlt, hdec⟩ := (hT _ _ _ hget).dec
  refine core_sound hS (hT _ _ _ hget).fetch hget hdec hrd (fun σ _ hL => ?_)
  have hv := sltu_bv _ _ (getR_lt s.regs rs1) hlt
  rw [← srcVal_pinsOf hS.x0 hrs1, ← himm] at hv
  rw [← hv]
  exact execute_itype_sltiu_char _ _ _ _ _ _
    (rX_src σ _ rs1 hrs1 _ (srcPin_pins hrs1 hL)) (wX_gpr _ _ rd hrd.1 hrd.2)

theorem wv_sllw (x y : Nat) :
    sign_extend (m := 64) (shift_bits_left (Sail.BitVec.extractLsb (BitVec.ofNat 64 x) 31 0)
      (Sail.BitVec.extractLsb (Sail.BitVec.extractLsb (BitVec.ofNat 64 y) 31 0) 4 0)) =
      BitVec.ofNat 64 (fSllw x y) := by
  have hk : Sail.BitVec.extractLsb (Sail.BitVec.extractLsb (BitVec.ofNat 64 y) 31 0) 4 0 =
      BitVec.ofNat 5 (y % 32) := by
    apply BitVec.eq_of_toNat_eq; rw [shamt_reg32]; simp
  rw [hk]
  exact wv_slliw x (y % 32) (Nat.mod_lt _ (by decide))

theorem wv_srlw (x y : Nat) :
    sign_extend (m := 64) (shift_bits_right (Sail.BitVec.extractLsb (BitVec.ofNat 64 x) 31 0)
      (Sail.BitVec.extractLsb (Sail.BitVec.extractLsb (BitVec.ofNat 64 y) 31 0) 4 0)) =
      BitVec.ofNat 64 (fSrlw x y) := by
  have hk : Sail.BitVec.extractLsb (Sail.BitVec.extractLsb (BitVec.ofNat 64 y) 31 0) 4 0 =
      BitVec.ofNat 5 (y % 32) := by
    apply BitVec.eq_of_toNat_eq; rw [shamt_reg32]; simp
  rw [hk]
  exact wv_srliw x (y % 32) (Nat.mod_lt _ (by decide))

theorem sound_sllw {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s : St} {cf : Config} (hS : Sim c ab s cf)
    {w rd rs1 rs2 : Nat} (hget : c.table.getW s.pc = some (w, .sllw rd rs1 rs2)) :
    Reaches c ab cf ⟨(s.pc + 4) % W, setR s.regs rd (fSllw (getR s.regs rs1) (getR s.regs rs2)), s.mem⟩ := by
  have hk := (hT _ _ _ hget).dec
  refine sound_rr hT hS hget hk ⟨hk.1, srcOK_of hk.2.1, srcOK_of hk.2.2.1⟩ trivial ?_ rfl
  show (rd, sign_extend (m := 64) (shift_bits_left (Sail.BitVec.extractLsb (srcVal rs1 (pinsOf s.regs)) 31 0)
    (Sail.BitVec.extractLsb (Sail.BitVec.extractLsb (srcVal rs2 (pinsOf s.regs)) 31 0) 4 0))) :: _ = _
  rw [srcVal_pinsOf hS.x0 hk.2.1, srcVal_pinsOf hS.x0 hk.2.2.1, wv_sllw]; rfl

theorem sound_srlw {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s : St} {cf : Config} (hS : Sim c ab s cf)
    {w rd rs1 rs2 : Nat} (hget : c.table.getW s.pc = some (w, .srlw rd rs1 rs2)) :
    Reaches c ab cf ⟨(s.pc + 4) % W, setR s.regs rd (fSrlw (getR s.regs rs1) (getR s.regs rs2)), s.mem⟩ := by
  have hk := (hT _ _ _ hget).dec
  refine sound_rr hT hS hget hk ⟨hk.1, srcOK_of hk.2.1, srcOK_of hk.2.2.1⟩ trivial ?_ rfl
  show (rd, sign_extend (m := 64) (shift_bits_right (Sail.BitVec.extractLsb (srcVal rs1 (pinsOf s.regs)) 31 0)
    (Sail.BitVec.extractLsb (Sail.BitVec.extractLsb (srcVal rs2 (pinsOf s.regs)) 31 0) 4 0))) :: _ = _
  rw [srcVal_pinsOf hS.x0 hk.2.1, srcVal_pinsOf hS.x0 hk.2.2.1, wv_srlw]; rfl

/-! ## Arithmetic right shifts -/

/-- Complementing, shifting and complementing again is the shift with ones
shifted in (the arithmetic shift of a negative value). -/
theorem ashr_compl (x s n : Nat) (hs : s ≤ n) (hx : x < 2 ^ n) :
    2 ^ n - 1 - (2 ^ n - 1 - x) / 2 ^ s = x / 2 ^ s + (2 ^ n - 2 ^ n / 2 ^ s) := by
  have hP : 0 < 2 ^ s := Nat.two_pow_pos s
  have hPQ : 2 ^ s * 2 ^ (n - s) = 2 ^ n := by rw [← Nat.pow_add]; congr 1; omega
  have hQ : 2 ^ n / 2 ^ s = 2 ^ (n - s) := by
    rw [← hPQ, Nat.mul_div_cancel_left _ hP]
  rw [hQ]
  generalize hq : x / 2 ^ s = q
  generalize hr : x % 2 ^ s = r
  have hx' : x = 2 ^ s * q + r := by rw [← hq, ← hr]; exact (Nat.div_add_mod x _).symm
  have hrP : r < 2 ^ s := by rw [← hr]; exact Nat.mod_lt _ hP
  have hqQ : q < 2 ^ (n - s) := by
    rw [← hq]; exact Nat.div_lt_of_lt_mul (by rw [hPQ]; exact hx)
  have hc : 2 ^ n - 1 - x = 2 ^ s * (2 ^ (n - s) - 1 - q) + (2 ^ s - 1 - r) := by
    have h1 : 2 ^ s * (2 ^ (n - s) - 1 - q) = 2 ^ s * 2 ^ (n - s) - 2 ^ s - 2 ^ s * q := by
      rw [Nat.mul_sub, Nat.mul_sub, Nat.mul_one]
    rw [h1, hPQ, hx']
    have h2 : 2 ^ s * q + 2 ^ s ≤ 2 ^ n := by
      rw [← hPQ, ← Nat.mul_succ]; exact Nat.mul_le_mul_left _ hqQ
    omega
  rw [hc, Nat.mul_add_div hP, Nat.div_eq_of_lt (by omega)]
  have h3 : 2 ^ (n - s) ≤ 2 ^ n := by rw [← hPQ]; exact Nat.le_mul_of_pos_left _ hP
  omega

theorem sra_toNat (x s : Nat) (hx : x < 2 ^ 64) (hs : s < 64) :
    ((BitVec.ofNat 64 x).sshiftRight s).toNat = sra64 x s := by
  simp only [BitVec.toNat_sshiftRight, BitVec.toNat_ofNat, Nat.mod_eq_of_lt hx, BitVec.msb_eq_decide, sra64]
  by_cases h : x < H
  · have : ¬ 2 ^ (64 - 1) ≤ x := by simp only [H] at h; omega
    simp [h, this]
  · have : 2 ^ (64 - 1) ≤ x := by simp only [H] at h; omega
    simp only [this, decide_true, if_true, h, if_false]
    rw [Nat.shiftRight_eq_div_pow, Nat.shiftRight_eq_div_pow, Nat.shiftRight_eq_div_pow,
      ashr_compl x s 64 (by omega) hx]

theorem sra32_toNat (x s : Nat) (hx : x < 2 ^ 32) (hs : s < 32) :
    ((BitVec.ofNat 32 x).sshiftRight s).toNat = sra32 x s := by
  simp only [BitVec.toNat_sshiftRight, BitVec.toNat_ofNat, Nat.mod_eq_of_lt hx, BitVec.msb_eq_decide, sra32]
  by_cases h : x < 2147483648
  · have : ¬ 2 ^ (32 - 1) ≤ x := by omega
    simp [h, this]
  · have : 2 ^ (32 - 1) ≤ x := by omega
    simp only [this, decide_true, if_true, h, if_false]
    rw [Nat.shiftRight_eq_div_pow, Nat.shiftRight_eq_div_pow, Nat.shiftRight_eq_div_pow,
      ashr_compl x s 32 (by omega) hx]

theorem sra_bv (x k : Nat) (hx : x < 2 ^ 64) (hk : k < 64) :
    shift_bits_right_arith (BitVec.ofNat 64 x) (BitVec.ofNat 6 k) = BitVec.ofNat 64 (sra64 x k) := by
  have e : shift_bits_right_arith (BitVec.ofNat 64 x) (BitVec.ofNat 6 k) =
      (BitVec.ofNat 64 x).sshiftRight k := by
    simp only [shift_bits_right_arith, BitVec.toNatInt, BitVec.toNat_ofNat]
    rw [Nat.mod_eq_of_lt (show k < 2 ^ 6 by omega)]; rfl
  rw [e]
  apply BitVec.eq_of_toNat_eq
  rw [sra_toNat x k hx hk, BitVec.toNat_ofNat, Nat.mod_eq_of_lt]
  rw [← sra_toNat x k hx hk]; exact BitVec.isLt _

theorem shamt6 (k : Nat) (hk : k < 64) :
    Sail.BitVec.extractLsb ((BitVec.ofNat 12 k).extractLsb' 0 6) 5 0 = BitVec.ofNat 6 k := by
  apply BitVec.eq_of_toNat_eq; simp [Sail.BitVec.extractLsb]

theorem shamt_reg6 (y : Nat) (hy : y < 2 ^ 64) :
    Sail.BitVec.extractLsb (BitVec.ofNat 64 y) 5 0 = BitVec.ofNat 6 (y % 64) := by
  apply BitVec.eq_of_toNat_eq; rw [shamt_reg y hy]; simp

theorem wv_sraiw (x k : Nat) (hk : k < 32) :
    sign_extend (m := 64) (shift_bits_right_arith (Sail.BitVec.extractLsb (BitVec.ofNat 64 x) 31 0)
      (BitVec.ofNat 5 k)) = BitVec.ofNat 64 (fSraiw x k) := by
  rw [lo32]
  have e : shift_bits_right_arith (BitVec.ofNat 32 (x % W32)) (BitVec.ofNat 5 k) =
      BitVec.ofNat 32 (sra32 (x % W32) k) := by
    have e1 : shift_bits_right_arith (BitVec.ofNat 32 (x % W32)) (BitVec.ofNat 5 k) =
        (BitVec.ofNat 32 (x % W32)).sshiftRight k := by
      simp only [shift_bits_right_arith, BitVec.toNatInt, BitVec.toNat_ofNat]
      rw [Nat.mod_eq_of_lt (show k < 2 ^ 5 by omega)]; rfl
    have hlt : sra32 (x % W32) k < 2 ^ 32 := by
      rw [← sra32_toNat _ k (mod32_lt x) hk]; exact BitVec.isLt _
    rw [e1]
    apply BitVec.eq_of_toNat_eq
    rw [sra32_toNat _ k (mod32_lt x) hk, BitVec.toNat_ofNat, Nat.mod_eq_of_lt hlt]
  rw [e]
  have hlt : sra32 (x % W32) k < 2 ^ 32 := by
    rw [← sra32_toNat _ k (mod32_lt x) hk]; exact BitVec.isLt _
  exact sext32_bv _ hlt

theorem sound_srai {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s : St} {cf : Config} (hS : Sim c ab s cf)
    {w rd rs1 k : Nat} (hget : c.table.getW s.pc = some (w, .srai rd rs1 k)) :
    Reaches c ab cf ⟨(s.pc + 4) % W, setR s.regs rd (fSrai (getR s.regs rs1) k), s.mem⟩ := by
  obtain ⟨hrd, hrs1, hk, hdec⟩ := (hT _ _ _ hget).dec
  refine sound_ri hT hS hget hdec hrd ⟨hrd, srcOK_of hrs1⟩ trivial ?_ rfl
  show (rd, shift_bits_right_arith (srcVal rs1 (pinsOf s.regs))
    (Sail.BitVec.extractLsb ((BitVec.ofNat 12 k).extractLsb' 0 6) 5 0)) :: _ = _
  rw [srcVal_pinsOf hS.x0 hrs1, shamt6 k hk, sra_bv _ _ (getR_lt _ _) hk]; rfl

theorem sound_sraiw {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s : St} {cf : Config} (hS : Sim c ab s cf)
    {w rd rs1 k : Nat} (hget : c.table.getW s.pc = some (w, .sraiw rd rs1 k)) :
    Reaches c ab cf ⟨(s.pc + 4) % W, setR s.regs rd (fSraiw (getR s.regs rs1) k), s.mem⟩ := by
  obtain ⟨hrd, hrs1, hk, hsh, hdec⟩ := (hT _ _ _ hget).dec
  refine sound_ri hT hS hget hdec hrd ⟨hrd, srcOK_of hrs1⟩ trivial ?_ rfl
  show (rd, sign_extend (m := 64) (shift_bits_right_arith
    (Sail.BitVec.extractLsb (srcVal rs1 (pinsOf s.regs)) 31 0) ((BitVec.ofNat 32 w).extractLsb' 20 5))) :: _ = _
  rw [hsh, srcVal_pinsOf hS.x0 hrs1, wv_sraiw _ _ hk]; rfl

theorem sound_sra {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s : St} {cf : Config} (hS : Sim c ab s cf)
    {w rd rs1 rs2 : Nat} (hget : c.table.getW s.pc = some (w, .sra rd rs1 rs2)) :
    Reaches c ab cf ⟨(s.pc + 4) % W, setR s.regs rd (fSra (getR s.regs rs1) (getR s.regs rs2)), s.mem⟩ := by
  obtain ⟨hrd, hrs1, hrs2, hdec⟩ := (hT _ _ _ hget).dec
  refine core_sound hS (hT _ _ _ hget).fetch hget hdec hrd (fun σ _ hL => ?_)
  have hv : shift_bits_right_arith (srcVal rs1 (pinsOf s.regs))
      (Sail.BitVec.extractLsb (srcVal rs2 (pinsOf s.regs)) 5 0) =
      BitVec.ofNat 64 (fSra (getR s.regs rs1) (getR s.regs rs2)) := by
    rw [srcVal_pinsOf hS.x0 hrs1, srcVal_pinsOf hS.x0 hrs2, shamt_reg6 _ (getR_lt _ _),
      sra_bv _ _ (getR_lt _ _) (Nat.mod_lt _ (by decide))]; rfl
  rw [← hv]
  exact execute_rtype_sra_char _ _ _ _ _ _ _
    (rX_src σ _ rs1 hrs1 _ (srcPin_pins hrs1 hL)) (rX_src σ _ rs2 hrs2 _ (srcPin_pins hrs2 hL))
    (wX_gpr _ _ rd hrd.1 hrd.2)

/-! ## `fence` (Sail: a barrier, no architectural effect in machine mode) -/

theorem updAt_fldAt (sh n R : Nat) : updAt sh n R (fldAt sh n R) = R := by
  apply Nat.eq_of_testBit_eq
  intro j
  rw [testBit_updAt]
  split
  · rw [testBit_fldAt]; simp only [show j - sh < n by omega, decide_true, Bool.true_and]
    congr 1; omega
  · rfl

theorem setR_getR (R i : Nat) : setR R i (getR R i) = R := by
  by_cases h : i = 0
  · subst h; rfl
  · rw [setR_ne _ h]; exact updAt_fldAt _ _ _

/-- `fence iorw, iorw` in machine mode: a barrier, no architectural effect. -/
theorem execute_fence (σ : MState) (fm : BitVec 4) (rs rd : regidx)
    (h : σ.regs.get? Register.cur_privilege = some Privilege.Machine) :
    (execute (instruction.FENCE (fm, 0xf#4, 0xf#4, rs, rd))).run σ = .ok RETIRE_SUCCESS σ := by
  simp only [execute, execute_FENCE, is_fiom_active, EStateM.run, bind, EStateM.bind, readReg,
    PreSail.readReg, get, getThe, MonadStateOf.get, EStateM.get, h, pure, EStateM.pure]
  rfl

theorem sound_fence {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s : St} {cf : Config} (hS : Sim c ab s cf)
    {w : Nat} (hget : c.table.getW s.pc = some (w, .fence)) :
    Reaches c ab cf ⟨(s.pc + 4) % W, s.regs, s.mem⟩ := by
  have hdec := (hT _ _ _ hget).dec
  have := core_sound (rd := 1) (v := getR s.regs 1) hS (hT _ _ _ hget).fetch hget hdec ⟨by decide, by decide⟩
    (fun σ hG hL => by
      have hx1 : σ.regs.get? Register.x1 = some (BitVec.ofNat 64 (getR s.regs 1)) := by
        have := gholds_lookup (pinsOf s.regs) hL (lookupG_pinsOf (R := s.regs) (n := 1) (by decide) (by decide))
        exact this
      have hr : (afterNextPC (afterPrelude σ) (BitVec.ofNat 64 s.pc)).regs.insert Register.x1
          (BitVec.ofNat 64 (getR s.regs 1)) = (afterNextPC (afterPrelude σ) (BitVec.ofNat 64 s.pc)).regs := by
        apply Std.ExtDHashMap.ext_get?
        intro k
        rw [Std.ExtDHashMap.get?_insert]
        split
        · rename_i hk
          have : Register.x1 = k := eq_of_beq hk
          subst this
          rw [get?_afterNextPC σ _ _ (by decide) (by decide), hx1]; rfl
        · rfl
      have hs : sigma3_alu σ (BitVec.ofNat 64 s.pc) (gprReg 1) (gprRT 1 (BitVec.ofNat 64 (getR s.regs 1))) =
          afterNextPC (afterPrelude σ) (BitVec.ofNat 64 s.pc) := by
        show ({(afterNextPC (afterPrelude σ) (BitVec.ofNat 64 s.pc)) with
          regs := (afterNextPC (afterPrelude σ) (BitVec.ofNat 64 s.pc)).regs.insert Register.x1
            (BitVec.ofNat 64 (getR s.regs 1))} : MState) = _
        rw [hr]
      rw [hs]
      exact execute_fence _ _ _ _ (by
        rw [get?_afterNextPC σ _ _ (by decide) (by decide)]
        exact hG.cur_privilege))
  rwa [setR_getR] at this

/-! ## The generic theorem -/

/-- **One evaluator step is a Sail step.** Every instruction kind `DecOK`
covers is handled; for the others `DecOK` is `False`, so a table that
`TableOK` holds of contains only covered kinds. -/
theorem exec_sound {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s s' : St} {cf : Config} (hS : Sim c ab s cf)
    {w : Nat} {i : Ins} (hget : c.table.getW s.pc = some (w, i)) (hex : exec i c s = some s') :
    Reaches c ab cf s' := by
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
  case sub rd rs1 rs2 =>
    simp only [exec, alu, wr, next, Option.some.injEq] at hex; subst hex; exact sound_sub hT hS hget
  case and rd rs1 rs2 =>
    simp only [exec, alu, wr, next, Option.some.injEq] at hex; subst hex; exact sound_and hT hS hget
  case or rd rs1 rs2 =>
    simp only [exec, alu, wr, next, Option.some.injEq] at hex; subst hex; exact sound_or hT hS hget
  case xor rd rs1 rs2 =>
    simp only [exec, alu, wr, next, Option.some.injEq] at hex; subst hex; exact sound_xor hT hS hget
  case andi rd rs1 imm =>
    simp only [exec, alui, wr, next, Option.some.injEq] at hex; subst hex; exact sound_andi hT hS hget
  case ori rd rs1 imm =>
    simp only [exec, alui, wr, next, Option.some.injEq] at hex; subst hex; exact sound_ori hT hS hget
  case xori rd rs1 imm =>
    simp only [exec, alui, wr, next, Option.some.injEq] at hex; subst hex; exact sound_xori hT hS hget
  case slli rd rs1 k =>
    simp only [exec, alui, wr, next, Option.some.injEq] at hex; subst hex; exact sound_slli hT hS hget
  case srli rd rs1 k =>
    simp only [exec, alui, wr, next, Option.some.injEq] at hex; subst hex; exact sound_srli hT hS hget
  case blt a b tgt =>
    simp only [exec, bra, Option.some.injEq] at hex; subst hex; exact sound_br hT hS hget hok guard_blt
  case bge a b tgt =>
    simp only [exec, bra, Option.some.injEq] at hex; subst hex; exact sound_br hT hS hget hok guard_bge
  case bltu a b tgt =>
    simp only [exec, bra, Option.some.injEq] at hex; subst hex; exact sound_br hT hS hget hok guard_bltu
  case bgeu a b tgt =>
    simp only [exec, bra, Option.some.injEq] at hex; subst hex; exact sound_br hT hS hget hok guard_bgeu
  case addiw rd rs1 imm =>
    simp only [exec, alui, wr, next, Option.some.injEq] at hex; subst hex; exact sound_addiw hT hS hget
  case addw rd rs1 rs2 =>
    simp only [exec, alu, wr, next, Option.some.injEq] at hex; subst hex; exact sound_addw hT hS hget
  case subw rd rs1 rs2 =>
    simp only [exec, alu, wr, next, Option.some.injEq] at hex; subst hex; exact sound_subw hT hS hget
  case slliw rd rs1 k =>
    simp only [exec, alui, wr, next, Option.some.injEq] at hex; subst hex; exact sound_slliw hT hS hget
  case srliw rd rs1 k =>
    simp only [exec, alui, wr, next, Option.some.injEq] at hex; subst hex; exact sound_srliw hT hS hget
  case slt rd rs1 rs2 =>
    simp only [exec, alu, wr, next, Option.some.injEq] at hex; subst hex; exact sound_slt hT hS hget
  case slti rd rs1 imm =>
    simp only [exec, alui, wr, next, Option.some.injEq] at hex; subst hex; exact sound_slti hT hS hget
  case sll rd rs1 rs2 =>
    simp only [exec, alu, wr, next, Option.some.injEq] at hex; subst hex; exact sound_sll hT hS hget
  case srl rd rs1 rs2 =>
    simp only [exec, alu, wr, next, Option.some.injEq] at hex; subst hex; exact sound_srl hT hS hget
  case lw rd rs1 imm => exact sound_lw hT hS hget hex
  case lwu rd rs1 imm => exact sound_lwu hT hS hget hex
  case lh rd rs1 imm => exact sound_lh hT hS hget hex
  case lhu rd rs1 imm => exact sound_lhu hT hS hget hex
  case sw rs1 rs2 imm => exact sound_sw hT hS hget hex
  case sh rs1 rs2 imm => exact sound_sh hT hS hget hex
  case sb rs1 rs2 imm => exact sound_sb hT hS hget hex
  case sltu rd rs1 rs2 =>
    simp only [exec, alu, wr, next, Option.some.injEq] at hex; subst hex; exact sound_sltu hT hS hget
  case sltiu rd rs1 imm =>
    simp only [exec, alui, wr, next, Option.some.injEq] at hex; subst hex; exact sound_sltiu hT hS hget
  case sllw rd rs1 rs2 =>
    simp only [exec, alu, wr, next, Option.some.injEq] at hex; subst hex; exact sound_sllw hT hS hget
  case srlw rd rs1 rs2 =>
    simp only [exec, alu, wr, next, Option.some.injEq] at hex; subst hex; exact sound_srlw hT hS hget
  case srai rd rs1 k =>
    simp only [exec, alui, wr, next, Option.some.injEq] at hex; subst hex; exact sound_srai hT hS hget
  case sraiw rd rs1 k =>
    simp only [exec, alui, wr, next, Option.some.injEq] at hex; subst hex; exact sound_sraiw hT hS hget
  case sra rd rs1 rs2 =>
    simp only [exec, alu, wr, next, Option.some.injEq] at hex; subst hex; exact sound_sra hT hS hget
  case fence =>
    simp only [exec, opFence, next, Option.some.injEq] at hex; subst hex; exact sound_fence hT hS hget
  case addi rd rs1 imm =>
    simp only [exec, alui, wr, next, Option.some.injEq] at hex; subst hex; exact sound_addi hT hS hget
  case add rd rs1 rs2 =>
    simp only [exec, alu, wr, next, Option.some.injEq] at hex; subst hex; exact sound_add hT hS hget
  all_goals exact (hok : False).elim

theorem step_sound {c : Code} {ab : Nat → Bool} (hT : TableOK c) {s s' : St} {cf : Config} (hS : Sim c ab s cf)
    (h : step c s = some s') : Reaches c ab cf s' := by
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
theorem run_sound {c : Code} {ab : Nat → Bool} (hT : TableOK c) : ∀ (n : Nat) {s t : St} {cf : Config},
    Sim c ab s cf → run c n s = some t → Reaches c ab cf t
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


/-! ## The table, entry by entry -/

/-- Every leaf of a code tree satisfies `P` (a generated proof follows the tree). -/
def CT.All (P : Nat → Nat → Ins → Prop) : CT → Prop
  | .nil => True
  | .leaf pc w i => P pc w i
  | .node _ l r => CT.All P l ∧ CT.All P r

theorem CT.All.getW {P : Nat → Nat → Ins → Prop} : ∀ {t : CT}, CT.All P t →
    ∀ {pc w : Nat} {i : Ins}, t.getW pc = some (w, i) → P pc w i
  | .nil, _, _, _, _, h => nomatch h
  | .leaf p w0 i0, ha, pc, w, i, h => by
    simp only [CT.getW] at h
    cases hb : Nat.beq p pc with
    | false => simp [hb] at h
    | true =>
      simp only [hb, Option.some.injEq, Prod.mk.injEq] at h
      obtain ⟨rfl, rfl⟩ := h
      have := Nat.eq_of_beq_eq_true hb; subst this; exact ha
  | .node s l r, ha, pc, w, i, h => by
    simp only [CT.getW] at h
    cases hb : Nat.ble s pc with
    | false => simp only [hb] at h; exact CT.All.getW ha.1 h
    | true => simp only [hb] at h; exact CT.All.getW ha.2 h

theorem tableOK_of_all {c : Code} (h : CT.All (EntryOK c) c.table) : TableOK c :=
  fun _ _ _ hg => CT.All.getW h hg

/-- Close a generated entry obligation: closed facts by `decide`, decode facts
by `decodeW`, conjunctions and structures by `constructor`, the `li`
disjunction (`lui` or `auipc`) by trying each side. -/
syntax "ccp_entry" : tactic
macro_rules
  | `(tactic| ccp_entry) => `(tactic| first
    | exact True.intro
    | decide
    | (intro s h1 h2 h3; exact Vsa.Sim.decodeW s h1 h2 h3)
    | (apply Or.inl; ccp_entry; done)
    | (apply Or.inr; ccp_entry; done)
    | (constructor <;> ccp_entry))

end Tcc.Ccp
