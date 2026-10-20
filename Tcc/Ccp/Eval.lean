/-!
# A first-order, kernel-reducible RV64I evaluator (CompileCut pilot)

The candidate discharge of `CompileCut_Statement` runs tcc's compile phase
(6,632,342 steps) inside the kernel, in chunks between generated checkpoints,
and relates the evaluator to Sail once, generically (`Tcc.Ccp.Sound`).

Everything here is first order and uses only `Nat` arithmetic the kernel
evaluates with GMP (`+ - * / % <<< >>> &&& ||| ^^^`, `==`, `<`), with no
`BitVec` wrapping, no `partial` and no well-founded recursion:

* `St`: the PC, the 31 GPRs packed in ONE `Nat` (`x_i` at bits `64 i ..
  64 i + 63`, so a read is a shift and a `%`, a write four GMP operations),
  and the memory, a binary search tree of fixed shape over the pages the run
  touches (`MT`). A page holds its bytes packed little-endian in `data` and,
  in `mask`, one bit per byte the evaluator knows. Reading an unknown byte is
  a fault: the evaluator's memory is a partial view of Sail's.
* `CT`: the code, pre-decoded (`Ins`), in a binary search tree keyed by PC.
  The run never stores into the code range (`codeLo`, `codeHi`), so the table
  stays valid.
* `step` returns `none` on anything the soundness theorem does not cover
  (unknown PC, unknown byte, a misaligned or out-of-window access, a
  page-straddling access); `run` threads it.
-/

namespace Tcc.Ccp

/-- `2^64`. -/
abbrev W : Nat := 18446744073709551616
/-- `2^63`. -/
abbrev H : Nat := 9223372036854775808
/-- `2^32`. -/
abbrev W32 : Nat := 4294967296

/-- The HTIF mailbox (`Vsa.Sim.tohostAddr`). -/
abbrev tohost : Nat := 0x8007cec0

-- Pages are 256 bytes (written as literals so `omega` sees them).

/-! ## Packed fields

Registers, page bytes and known-byte masks are all fields of one `Nat`:
`fldAt sh n R` reads the `n` bits at `sh`, `updAt sh n R v` replaces them
by `v mod 2^n` (bitwise, so one generic `testBit` lemma serves every use). -/

/-- The `n` bits of `R` from bit `sh`. -/
@[inline] def fldAt (sh n R : Nat) : Nat := (R >>> sh) % 2 ^ n

/-- `R` with the `n` bits from bit `sh` replaced by `v mod 2^n`. -/
@[inline] def updAt (sh n R v : Nat) : Nat :=
  (R ^^^ (R &&& ((2 ^ n - 1) <<< sh))) ||| ((v % 2 ^ n) <<< sh)

/-! ## Registers -/

/-- `x_i` of the packed register file (`x0` reads 0: slot 0 is kept 0). -/
@[inline] def getR (R : Nat) (i : Nat) : Nat := fldAt (64 * i) 64 R

/-- Write `x_i := v mod 2^64`; `x0` is not written. -/
@[inline] def setR (R : Nat) (i : Nat) (v : Nat) : Nat :=
  match Nat.beq i 0 with
  | true => R
  | false => updAt (64 * i) 64 R v

/-! ## Memory -/

/-- A page tree: leaves carry their page number, packed bytes and known-byte
mask; nodes split on the page number. -/
inductive MT where
  | nil : MT
  | leaf (pg : Nat) (data : Nat) (mask : Nat) : MT
  | node (split : Nat) (l r : MT) : MT
  deriving Inhabited, DecidableEq, Repr

/-- The page `pg` as `(data, mask)`. -/
def MT.get : MT → Nat → Option (Nat × Nat)
  | .nil, _ => none
  | .leaf p d m, pg => match Nat.beq p pg with | true => some (d, m) | false => none
  | .node s l r, pg => match Nat.ble s pg with | true => r.get pg | false => l.get pg

/-- Replace page `pg` (which must exist; else the tree is unchanged). -/
def MT.set : MT → Nat → Nat → Nat → MT
  | .nil, _, _, _ => .nil
  | .leaf p d m, pg, d', m' => match Nat.beq p pg with | true => .leaf p d' m' | false => .leaf p d m
  | .node s l r, pg, d', m' => match Nat.ble s pg with
    | true => .node s l (r.set pg d' m')
    | false => .node s (l.set pg d' m') r

/-- A memory delta of the same shape as a page tree: `keep` leaves a subtree
as it is, `leaf` replaces a page's contents. A checkpoint's memory is its
anchor's `overlay`ed with a delta, a structural recursion of the tree's depth
(no chain of updates for the kernel to unwind). -/
inductive DT where
  | keep : DT
  | leaf (data : Nat) (mask : Nat) : DT
  | node (l r : DT) : DT
  deriving Inhabited, Repr

def MT.overlay : MT → DT → MT
  | t, .keep => t
  | .leaf p _ _, .leaf d m => .leaf p d m
  | .node s l r, .node dl dr => .node s (l.overlay dl) (r.overlay dr)
  | t, _ => t

/-- Little-endian load of `w` bytes at `a` (all in one page, all known). -/
def load (M : MT) (a w : Nat) : Option Nat :=
  let off := a % 256
  match Nat.ble (off + w) 256 with
  | true =>
    match M.get (a >>> 8) with
    | some (d, m) =>
      match Nat.beq (fldAt off w m) (2 ^ w - 1) with
      | true => some (fldAt (8 * off) (8 * w) d)
      | false => none
    | none => none
  | false => none

/-- Store the low `w` bytes of `v` at `a` (one page, which must exist). -/
def store (M : MT) (a w v : Nat) : Option MT :=
  let off := a % 256
  match Nat.ble (off + w) 256 with
  | true =>
    let pg := a >>> 8
    match M.get pg with
    | some (d, m) =>
      let d' := updAt (8 * off) (8 * w) d v
      let m' := updAt off w m (2 ^ w - 1)
      -- always `true`; forces `d'` and `m'` so no chain of unevaluated page
      -- updates builds up in the kernel between loads
      match Nat.beq ((d' >>> 2048) + (m' >>> 256)) 0 with
      | true => some (M.set pg d' m')
      | false => none
    | none => none
  | false => none

/-! ## Instructions -/

/-- Pre-decoded RV64I. Immediates are the sign-extended value mod `2^64`;
`li` is `lui` and `auipc` (whose value is fixed by the PC); `jal` carries its
target. -/
inductive Ins where
  | li (rd v : Nat)
  | jal (rd tgt : Nat)
  | jalr (rd rs1 imm : Nat)
  | beq (rs1 rs2 tgt : Nat) | bne (rs1 rs2 tgt : Nat)
  | blt (rs1 rs2 tgt : Nat) | bge (rs1 rs2 tgt : Nat)
  | bltu (rs1 rs2 tgt : Nat) | bgeu (rs1 rs2 tgt : Nat)
  | lb (rd rs1 imm : Nat) | lh (rd rs1 imm : Nat) | lw (rd rs1 imm : Nat) | ld (rd rs1 imm : Nat)
  | lbu (rd rs1 imm : Nat) | lhu (rd rs1 imm : Nat) | lwu (rd rs1 imm : Nat)
  | sb (rs1 rs2 imm : Nat) | sh (rs1 rs2 imm : Nat) | sw (rs1 rs2 imm : Nat) | sd (rs1 rs2 imm : Nat)
  | addi (rd rs1 imm : Nat) | slti (rd rs1 imm : Nat) | sltiu (rd rs1 imm : Nat)
  | xori (rd rs1 imm : Nat) | ori (rd rs1 imm : Nat) | andi (rd rs1 imm : Nat)
  | slli (rd rs1 sh : Nat) | srli (rd rs1 sh : Nat) | srai (rd rs1 sh : Nat)
  | add (rd rs1 rs2 : Nat) | sub (rd rs1 rs2 : Nat) | sll (rd rs1 rs2 : Nat)
  | slt (rd rs1 rs2 : Nat) | sltu (rd rs1 rs2 : Nat) | xor (rd rs1 rs2 : Nat)
  | srl (rd rs1 rs2 : Nat) | sra (rd rs1 rs2 : Nat) | or (rd rs1 rs2 : Nat) | and (rd rs1 rs2 : Nat)
  | addiw (rd rs1 imm : Nat) | slliw (rd rs1 sh : Nat) | srliw (rd rs1 sh : Nat) | sraiw (rd rs1 sh : Nat)
  | addw (rd rs1 rs2 : Nat) | subw (rd rs1 rs2 : Nat) | sllw (rd rs1 rs2 : Nat)
  | srlw (rd rs1 rs2 : Nat) | sraw (rd rs1 rs2 : Nat)
  | fence
  deriving Inhabited, DecidableEq, Repr

/-- The code table: leaves carry their PC. -/
inductive CT where
  | nil : CT
  | leaf (pc : Nat) (word : Nat) (i : Ins) : CT
  | node (split : Nat) (l r : CT) : CT
  deriving Inhabited, Repr

def CT.get : CT → Nat → Option Ins
  | .nil, _ => none
  | .leaf p _ i, pc => match Nat.beq p pc with | true => some i | false => none
  | .node s l r, pc => match Nat.ble s pc with | true => r.get pc | false => l.get pc

/-! ## Arithmetic on `Nat` mod `2^64` -/

/-- Signed order as unsigned order on the sign-flipped value. -/
@[inline] def slt64 (x y : Nat) : Bool := (x ^^^ H) < (y ^^^ H)
@[inline] def sext32 (v : Nat) : Nat := if v < 2147483648 then v else v + (W - W32)
@[inline] def sra64 (x s : Nat) : Nat := if x < H then x >>> s else (x >>> s) + (W - (W >>> s))
@[inline] def sra32 (x s : Nat) : Nat :=
  if x < 2147483648 then x >>> s else (x >>> s) + (W32 - (W32 >>> s))
@[inline] def sextB (v : Nat) : Nat := if v < 128 then v else v + (W - 256)
@[inline] def sextH (v : Nat) : Nat := if v < 32768 then v else v + (W - 65536)

/-! ## The machine -/

structure St where
  pc : Nat
  regs : Nat
  mem : MT
  deriving Inhabited, DecidableEq, Repr

/-- The code range: never stored into. -/
structure Code where
  table : CT
  lo : Nat
  hi : Nat

/-- A load's address window (`MemFacts` of `Vsa.Sim.BlockMem`): RAM below
`2^32`, off the HTIF mailbox. -/
@[inline] def loadOK (a w : Nat) : Bool :=
  0x80000000 ≤ a && a + w ≤ 0x100000000 && (a + w ≤ tohost || tohost + 8 ≤ a)

/-- A store's window: as a load's, aligned, off the HTIF mailbox (`StoreWin`)
and off the code. -/
@[inline] def storeOK (c : Code) (a w : Nat) : Bool :=
  0x80000000 ≤ a && a + w ≤ 0x100000000 && (a + 8 ≤ tohost || tohost + 16 ≤ a) &&
    a % w == 0 && (a + w ≤ c.lo || c.hi ≤ a)

@[inline] def next (s : St) (R : Nat) : St := ⟨(s.pc + 4) % W, R, s.mem⟩
@[inline] def wr (s : St) (rd v : Nat) : St := next s (setR s.regs rd v)
@[inline] def br (s : St) (b : Bool) (tgt : Nat) : St :=
  if b then ⟨tgt, s.regs, s.mem⟩ else next s s.regs

@[inline] def ldI (s : St) (rd rs1 imm w : Nat) (ext : Nat → Nat) : Option St :=
  let a := (getR s.regs rs1 + imm) % W
  match loadOK a w with
  | true =>
    match load s.mem a w with
    | some v => some (wr s rd (ext v))
    | none => none
  | false => none

@[inline] def stI (c : Code) (s : St) (rs1 rs2 imm w : Nat) : Option St :=
  let a := (getR s.regs rs1 + imm) % W
  match storeOK c a w with
  | true =>
    match store s.mem a w (getR s.regs rs2) with
    | some m => some ⟨(s.pc + 4) % W, s.regs, m⟩
    | none => none
  | false => none

/-! Each instruction is a top-level operation of the decoded fields, the
code and the state: `exec` dispatches on the instruction alone, so unfolding
it per step instantiates only the dispatch spine (its alternatives are closed
terms), never 50 alternatives over the state. -/

@[inline] def alu (f : Nat → Nat → Nat) (rd a b : Nat) (_ : Code) (s : St) : Option St :=
  some (wr s rd (f (getR s.regs a) (getR s.regs b)))
@[inline] def alui (f : Nat → Nat → Nat) (rd a imm : Nat) (_ : Code) (s : St) : Option St :=
  some (wr s rd (f (getR s.regs a) imm))
@[inline] def bra (f : Nat → Nat → Bool) (a b t : Nat) (_ : Code) (s : St) : Option St :=
  some (br s (f (getR s.regs a) (getR s.regs b)) t)
@[inline] def opLi (rd v : Nat) (_ : Code) (s : St) : Option St := some (wr s rd v)
@[inline] def opJal (rd tgt : Nat) (_ : Code) (s : St) : Option St :=
  some ⟨tgt, setR s.regs rd ((s.pc + 4) % W), s.mem⟩
@[inline] def opJalr (rd rs1 imm : Nat) (_ : Code) (s : St) : Option St :=
  let t := (getR s.regs rs1 + imm) % W
  let t := t - t % 2
  if t % 4 == 0 then some ⟨t, setR s.regs rd ((s.pc + 4) % W), s.mem⟩ else none
@[inline] def opLd (w : Nat) (ext : Nat → Nat) (rd rs1 imm : Nat) (_ : Code) (s : St) :
    Option St := ldI s rd rs1 imm w ext
@[inline] def opSt (w : Nat) (rs1 rs2 imm : Nat) (c : Code) (s : St) : Option St :=
  stI c s rs1 rs2 imm w
@[inline] def opFence (_ : Code) (s : St) : Option St := some (next s s.regs)

def fAdd (x y : Nat) : Nat := (x + y) % W
def fSub (x y : Nat) : Nat := (x + (W - y)) % W
def fSlt (x y : Nat) : Nat := if slt64 x y then 1 else 0
def fSltu (x y : Nat) : Nat := if x < y then 1 else 0
def fXor (x y : Nat) : Nat := x ^^^ y
def fOr (x y : Nat) : Nat := x ||| y
def fAnd (x y : Nat) : Nat := x &&& y
def fSll (x y : Nat) : Nat := (x <<< (y % 64)) % W
def fSrl (x y : Nat) : Nat := x >>> (y % 64)
def fSra (x y : Nat) : Nat := sra64 x (y % 64)
def fAddw (x y : Nat) : Nat := sext32 ((x + y) % W32)
def fSubw (x y : Nat) : Nat := sext32 ((x + (W32 - y % W32)) % W32)
def fSllw (x y : Nat) : Nat := sext32 ((x <<< (y % 32)) % W32)
def fSrlw (x y : Nat) : Nat := sext32 ((x % W32) >>> (y % 32))
def fSraw (x y : Nat) : Nat := sext32 (sra32 (x % W32) (y % 32))
def fSlli (x k : Nat) : Nat := (x <<< k) % W
def fSrli (x k : Nat) : Nat := x >>> k
def fSrai (x k : Nat) : Nat := sra64 x k
def fSlliw (x k : Nat) : Nat := sext32 ((x <<< k) % W32)
def fSrliw (x k : Nat) : Nat := sext32 ((x % W32) >>> k)
def fSraiw (x k : Nat) : Nat := sext32 (sra32 (x % W32) k)
def bEq (x y : Nat) : Bool := x == y
def bNe (x y : Nat) : Bool := x != y
def bLt (x y : Nat) : Bool := slt64 x y
def bGe (x y : Nat) : Bool := !slt64 x y
def bLtu (x y : Nat) : Bool := x < y
def bGeu (x y : Nat) : Bool := !(x < y)

/-- One instruction. -/
def exec : Ins → Code → St → Option St
  | .li rd v => opLi rd v
  | .jal rd tgt => opJal rd tgt
  | .jalr rd rs1 imm => opJalr rd rs1 imm
  | .beq a b t => bra bEq a b t
  | .bne a b t => bra bNe a b t
  | .blt a b t => bra bLt a b t
  | .bge a b t => bra bGe a b t
  | .bltu a b t => bra bLtu a b t
  | .bgeu a b t => bra bGeu a b t
  | .lb _ _ _ => fun _ _ => none
  | .lh rd rs1 imm => opLd 2 sextH rd rs1 imm
  | .lw rd rs1 imm => opLd 4 sext32 rd rs1 imm
  | .ld rd rs1 imm => opLd 8 id rd rs1 imm
  | .lbu rd rs1 imm => opLd 1 id rd rs1 imm
  | .lhu rd rs1 imm => opLd 2 id rd rs1 imm
  | .lwu rd rs1 imm => opLd 4 id rd rs1 imm
  | .sb rs1 rs2 imm => opSt 1 rs1 rs2 imm
  | .sh rs1 rs2 imm => opSt 2 rs1 rs2 imm
  | .sw rs1 rs2 imm => opSt 4 rs1 rs2 imm
  | .sd rs1 rs2 imm => opSt 8 rs1 rs2 imm
  | .addi rd a imm => alui fAdd rd a imm
  | .slti rd a imm => alui fSlt rd a imm
  | .sltiu rd a imm => alui fSltu rd a imm
  | .xori rd a imm => alui fXor rd a imm
  | .ori rd a imm => alui fOr rd a imm
  | .andi rd a imm => alui fAnd rd a imm
  | .slli rd a k => alui fSlli rd a k
  | .srli rd a k => alui fSrli rd a k
  | .srai rd a k => alui fSrai rd a k
  | .add rd a b => alu fAdd rd a b
  | .sub rd a b => alu fSub rd a b
  | .sll rd a b => alu fSll rd a b
  | .slt rd a b => alu fSlt rd a b
  | .sltu rd a b => alu fSltu rd a b
  | .xor rd a b => alu fXor rd a b
  | .srl rd a b => alu fSrl rd a b
  | .sra rd a b => alu fSra rd a b
  | .or rd a b => alu fOr rd a b
  | .and rd a b => alu fAnd rd a b
  | .addiw rd a imm => alui fAddw rd a imm
  | .slliw rd a k => alui fSlliw rd a k
  | .srliw rd a k => alui fSrliw rd a k
  | .sraiw rd a k => alui fSraiw rd a k
  | .addw rd a b => alu fAddw rd a b
  | .subw rd a b => alu fSubw rd a b
  | .sllw rd a b => alu fSllw rd a b
  | .srlw rd a b => alu fSrlw rd a b
  | .sraw rd a b => alu fSraw rd a b
  | .fence => opFence

/-- One step: fetch from the table, execute. -/
def step (c : Code) (s : St) : Option St :=
  match c.table.get s.pc with
  | some i => exec i c s
  | none => none

/-- `n` steps, `none` at the first fault. -/
def run (c : Code) : Nat → St → Option St
  | 0, s => some s
  | n + 1, s =>
    match step c s with
    | some s' => run c n s'
    | none => none

/-- Boolean equality of page trees (`Nat.beq` on the literals). -/
def MT.beq : MT → MT → Bool
  | .nil, .nil => true
  | .leaf p d m, .leaf p' d' m' => p == p' && d == d' && m == m'
  | .node s l r, .node s' l' r' => s == s' && l.beq l' && r.beq r'
  | _, _ => false

def St.beq (s t : St) : Bool := s.pc == t.pc && s.regs == t.regs && s.mem.beq t.mem

/-- The kernel's check of one chunk. -/
def chunkOK (c : Code) (n : Nat) (s t : St) : Bool :=
  match run c n s with
  | some u => u.beq t
  | none => false

end Tcc.Ccp
