import Tcc.Model.Elf

/-!
# TccModel: riscv64-gen.c (the code generator)

riscv64-gen.c as built into the image: TCC_RV64I_SOFT (patches/0001: no
M/F/D instructions, libgcc helpers for multiply/divide and all floating
point, lp64 soft-float calling convention) and TCC_BAREMETAL (patches/0002:
`gen_clear_cache` emits no `fence.i`). CONFIG_TCC_BCHECK is off, so the
bounds-checking code is absent.

Every call into tccgen.c goes through `R` (riscv64-gen.c comes before
tccgen.c in the module order). The image is built with assertions on, so
the C `assert`s are kept: a failing one is `Err.internal` (tcc would abort
with newlib's assertion message).
-/

namespace Tcc.Model
open C

/-! ## helpers -/

/-- a C `assert` of riscv64-gen.c -/
private def tassert (b : Bool) (what : String) : M Unit :=
  if b then pure () else internal ("riscv64-gen.c assertion failed: " ++ what)

@[inline] private def rI (v : SValue) : Int32 := v.r.toInt32'
@[inline] private def r2I (v : SValue) : Int32 := v.r2.toInt32'
/-- `(int) sv->c.i` -/
@[inline] private def ciI (v : SValue) : Int32 := v.c.lo.toUInt32.toInt32
/-- `fc == sv->c.i` (the int sign-extended against the 64-bit value) -/
@[inline] private def fitsI (fc : Int32) (v : SValue) : Bool := fc.toInt64.toUInt64 == v.c.lo

/-- `vtop += k` -/
@[inline] private def vtopAdd (k : Int) : M Unit :=
  modify fun s => { s with vtop := ((s.vtop : Int) + k).toNat }

@[inline] private def setR (k : Int) (r : Int32) : M Unit := modVt k fun v => { v with r := r.toUInt16' }
@[inline] private def setR2 (k : Int) (r : Int32) : M Unit := modVt k fun v => { v with r2 := r.toUInt16' }
@[inline] private def setTy (k : Int) (t : Int32) : M Unit :=
  modVt k fun v => { v with type := { v.type with t := t } }
@[inline] private def setType (k : Int) (ty : CType) : M Unit := modVt k fun v => { v with type := ty }
/-- `vtop->cmp_r = x` (cmp_op kept) -/
@[inline] private def setCmpR (x : Int32) : M Unit := modVt 0 fun v => v.setCmp v.cmp_op x

/-- `is_float(t)` (tccgen.c's, also used here) -/
@[inline] def is_float (t : Int32) : Bool :=
  let bt := t &&& VT_BTYPE
  bt == VT_LDOUBLE || bt == VT_DOUBLE || bt == VT_FLOAT || bt == VT_QFLOAT

def XLEN : Int32 := 8

/-- `UPPER(x)` -/
@[inline] def UPPER (x : Int32) : UInt32 := (x.toUInt32 + 0x800) &&& 0xfffff000
/-- `LOW_OVERFLOW(x)` -/
@[inline] def LOW_OVERFLOW (x : Int32) : Bool := UPPER x != 0
/-- `SIGN7(x)` -/
@[inline] def SIGN7 (x : Int32) : Int32 := ((x &&& 0xff) ^^^ 0x80) - 0x80
/-- `SIGN11(x)` -/
@[inline] def SIGN11 (x : Int32) : Int32 := ((x &&& 0xfff) ^^^ 0x800) - 0x800

def R_RISCV_GOT_HI20 : Int32 := 20
def R_RISCV_PCREL_HI20 : Int32 := 23
def R_RISCV_PCREL_LO12_I : Int32 := 24
def R_RISCV_PCREL_LO12_S : Int32 := 25
def R_RISCV_TPREL_HI20 : Int32 := 29
def R_RISCV_TPREL_LO12_I : Int32 := 30
def R_RISCV_CALL_PLT : Int32 := 19

/-! ## registers -/

/-- `ireg(r)` -/
def ireg (r : Int32) : M Int32 := do
  if r == TREG_RA then return 1
  if r == TREG_SP then return 2
  tassert (r ≥ 0 && r < 8) "r >= 0 && r < 8"
  return r + 10

/-- `is_ireg(r)` -/
@[inline] def is_ireg (r : Int32) : Bool := r.toUInt32 < 8 || r == TREG_RA || r == TREG_SP

/-- `freg(r)` -/
def freg (r : Int32) : M Int32 := do
  tassert (r ≥ 8 && r < 16) "r >= 8 && r < 16"
  return r - 8 + 10

/-- `is_freg(r)` -/
@[inline] def is_freg (r : Int32) : Bool := r ≥ 8 && r < 16

/-! ## instruction emission -/

/-- `o(c)` -/
def o (c : UInt32) : M Unit := do
  let s ← get
  let ind1 := s.ind + 4
  if s.nocode_wanted != 0 then return
  let sec := s.cur_text_section
  if ind1.toNat' > (← getSec sec).data.size then section_realloc sec ind1.toNat'
  modSecData sec fun d => Bytes.w32 d s.ind.toNat' c
  modify fun s => { s with ind := ind1 }

/-- `EIu(opcode, func3, rd, rs1, imm)` -/
def EIu (opcode func3 rd rs1 imm : Int32) : M Unit :=
  o (opcode.toUInt32 ||| (func3.toUInt32 <<< 12) ||| (rd.toUInt32 <<< 7) ||| (rs1.toUInt32 <<< 15) |||
     (imm.toUInt32 <<< 20))

/-- `ER(opcode, func3, rd, rs1, rs2, func7)` -/
def ER (opcode func3 rd rs1 rs2 func7 : Int32) : M Unit :=
  o (opcode.toUInt32 ||| (func3.toUInt32 <<< 12) ||| (rd.toUInt32 <<< 7) ||| (rs1.toUInt32 <<< 15) |||
     (rs2.toUInt32 <<< 20) ||| (func7.toUInt32 <<< 25))

/-- `EI(opcode, func3, rd, rs1, imm)` -/
def EI (opcode func3 rd rs1 imm : Int32) : M Unit := do
  tassert (!LOW_OVERFLOW imm) "! LOW_OVERFLOW(imm)"
  EIu opcode func3 rd rs1 imm

/-- `ES(opcode, func3, rs1, rs2, imm)` -/
def ES (opcode func3 rs1 rs2 imm : Int32) : M Unit := do
  tassert (!LOW_OVERFLOW imm) "! LOW_OVERFLOW(imm)"
  let i := imm.toUInt32
  o (opcode.toUInt32 ||| (func3.toUInt32 <<< 12) ||| ((i &&& 0x1f) <<< 7) ||| (rs1.toUInt32 <<< 15) |||
     (rs2.toUInt32 <<< 20) ||| ((i >>> 5) <<< 25))

/-- `gsym_addr(t, a)`: patch the branch chain `t` to jump to `a` -/
def gsym_addr (R : Rec) (t_ a_ : Int32) : M Unit := do
  let mut t := t_.toUInt32
  let a := a_.toUInt32
  let fuel ← fuelM
  for _ in [0:fuel] do
    if t == 0 then break
    let sec := (← get).cur_text_section
    let next := Bytes.r32 (← getSec sec).data t.toNat
    let r := a - t
    if (r + ((1 : UInt32) <<< 21)) &&& ~~~(((1 : UInt32) <<< 22) - 2) != 0 then
      tcc_error_s R "out-of-range branch chain"
    let imm := (((r >>> 12) &&& 0xff) <<< 12) ||| (((r >>> 11) &&& 1) <<< 20) |||
               (((r >>> 1) &&& 0x3ff) <<< 21) ||| (((r >>> 20) &&& 1) <<< 31)
    let w : UInt32 := if r == 4 then 0x33 else 0x6f ||| imm
    modSecData sec fun d => Bytes.w32 d t.toNat w
    t := next

/-- a zeroed Sym outside the free list (C's `Sym label = {0}` on the stack) -/
private def scratchSym : M SymId :=
  modifyGet fun s =>
    let a := s.syms
    let s := { s with syms := #[] }
    (a.size, { s with syms := a.push default })

/-- `load_symofs(r, sv, forstore, &new_fc)`: the base register and the
updated `fc` (initially `(int) sv->c.i`) -/
def load_symofs (R : Rec) (r : Int32) (sv : SValue) (forstore : Bool) (fc0 : Int32) : M (Int32 × Int32) := do
  let mut fc := fc0
  let fcI := ciI sv
  let v := rI sv &&& VT_VALMASK
  if rI sv &&& VT_SYM != 0 then
    tassert (v == VT_CONST) "v == VT_CONST"
    let sym ← getSym sv.sym
    let cur := (← get).cur_text_section
    if sym.type.t &&& VT_TLS != 0 then
      let rr ← if is_ireg r then ireg r else pure 5
      R.greloca cur sv.sym (← get).ind.toUInt64' R_RISCV_TPREL_HI20 sv.c.lo.toInt64
      o ((0x37 : UInt32) ||| (rr.toUInt32 <<< 7))
      R.greloca cur sv.sym (← get).ind.toUInt64' R_RISCV_TPREL_LO12_I sv.c.lo.toInt64
      EI 0x13 0 rr rr 0
      ER 0x33 0 rr rr 4 0
      return (rr, 0)
    let mut doload := false
    let mut large_addend := false
    if sym.type.t &&& VT_STATIC != 0 then
      R.greloca cur sv.sym (← get).ind.toUInt64' R_RISCV_PCREL_HI20 sv.c.lo.toInt64
      fc := 0
    else
      if LOW_OVERFLOW fcI then large_addend := true
      R.greloca cur sv.sym (← get).ind.toUInt64' R_RISCV_GOT_HI20 0
      doload := true
    let label ← scratchSym
    modSym label fun x => { x with type := { t := VT_VOID ||| VT_STATIC } }
    if (← get).nocode_wanted == 0 then
      R.put_extern_sym label cur (← get).ind.toUInt64' 0
    let rr ← if is_ireg r then ireg r else pure 5
    o ((0x17 : UInt32) ||| (rr.toUInt32 <<< 7))
    R.greloca cur label (← get).ind.toUInt64'
      (if doload || !forstore then R_RISCV_PCREL_LO12_I else R_RISCV_PCREL_LO12_S) 0
    if doload then
      EI 0x03 3 rr rr 0
      if large_addend then
        o ((0x37 : UInt32) ||| ((6 : UInt32) <<< 7) ||| UPPER fcI)
        ER 0x33 0 rr rr 6 0
        fc := SIGN11 fcI
    return (rr, fc)
  else if v == VT_LOCAL || v == VT_LLOCAL then
    let mut rr : Int32 := 8
    if !fitsI fcI sv then
      tcc_error_s R "unimp: store(giant local off) (0x%lx)" [.u sv.c.lo.toNat]
    if LOW_OVERFLOW fcI then
      rr ← if is_ireg r then ireg r else pure 5
      o ((0x37 : UInt32) ||| (rr.toUInt32 <<< 7) ||| UPPER fcI)
      ER 0x33 0 rr rr 8 0
      fc := SIGN11 fcI
    return (rr, fc)
  else
    tcc_error_s R "uhh"

/-- `load_large_constant(rr, fc, pi)` -/
def load_large_constant (rr fc : Int32) (pi0 : UInt32) : M Unit := do
  let mut pi := pi0
  if fc < 0 then pi := pi + 1
  o ((0x37 : UInt32) ||| (rr.toUInt32 <<< 7) ||| UPPER pi.toInt32)
  EI 0x13 0 rr rr (SIGN11 pi.toInt32)
  EI 0x13 1 rr rr 12
  EI 0x13 0 rr rr (SIGN11 ((fc.toUInt32 + ((1 : UInt32) <<< 19)) >>> 20).toInt32)
  EI 0x13 1 rr rr 12
  EI 0x13 0 rr rr (SIGN11 ((fc.toUInt32 + ((1 : UInt32) <<< 7)) >>> 8).toInt32)
  EI 0x13 1 rr rr 8

/-- `gjmp(t)` -/
def gjmp (t : Int32) : M Int32 := do
  if (← get).nocode_wanted != 0 then return t
  o t.toUInt32
  return (← get).ind - 4

/-- `gjmp_addr(a)` -/
def gjmp_addr (a : Int32) : M Unit := do
  let r := a.toUInt32 - (← get).ind.toUInt32
  if (r + ((1 : UInt32) <<< 21)) &&& ~~~(((1 : UInt32) <<< 22) - 2) != 0 then
    o ((0x17 : UInt32) ||| ((5 : UInt32) <<< 7) ||| UPPER r.toInt32)
    let r' := SIGN11 r.toInt32
    EI 0x67 0 0 5 r'
  else
    let imm := (((r >>> 12) &&& 0xff) <<< 12) ||| (((r >>> 11) &&& 1) <<< 20) |||
               (((r >>> 1) &&& 0x3ff) <<< 21) ||| (((r >>> 20) &&& 1) <<< 31)
    o ((0x6f : UInt32) ||| imm)

/-- `load(r, sv)` (`fuel` bounds its one recursive call, the
TCC_RV64I_SOFT float constant) -/
def loadF (R : Rec) : Nat → Int32 → SValue → M Unit
  | 0, _, _ => throw (.fuel "load")
  | n+1, r, sv => do
  let fr := rI sv
  let v := fr &&& VT_VALMASK
  let rr ← if is_ireg r then ireg r else freg r
  let mut fc := ciI sv
  let bt := sv.type.t &&& VT_BTYPE
  if fr &&& VT_LVAL != 0 then
    let opcode : Int32 := if is_freg r then 0x07 else 0x03
    let (size0, _) ← R.type_size sv.type
    tassert (!is_freg r || bt == VT_FLOAT || bt == VT_DOUBLE) "!is_freg(r) || bt == VT_FLOAT || bt == VT_DOUBLE"
    let size := if bt == VT_PTR || bt == VT_FUNC then PTR_SIZE else size0
    let mut func3 : Int32 := if size == 1 then 0 else if size == 2 then 1 else if size == 4 then 2 else 3
    if size < 4 && !is_float sv.type.t && (sv.type.t &&& VT_UNSIGNED != 0) then func3 := func3 ||| 4
    let mut br : Int32 := 0
    if v == VT_LOCAL || (fr &&& VT_SYM != 0) then
      let (b, f) ← load_symofs R r sv false fc
      br := b; fc := f
    else if v < VT_CONST then
      br ← ireg v
      fc := 0
    else if v == VT_LLOCAL then
      let (b, f) ← load_symofs R r sv false fc
      EI 0x03 3 rr b f
      br := rr
      fc := 0
    else if v == VT_CONST then
      let si := sv.c.lo.toInt64 >>> 32
      if si != 0 then
        load_large_constant rr fc si.toUInt64.toUInt32
        fc := SIGN7 fc
      else
        o ((0x37 : UInt32) ||| (rr.toUInt32 <<< 7) ||| UPPER fc)
        fc := SIGN11 fc
      br := rr
    else
      tcc_error_s R "unimp: load(non-local lval)"
    EI opcode func3 rr br fc
  else if v == VT_CONST then
    let mut rb : Int32 := 0
    let mut do32bit : Int32 := 8
    let mut zext := false
    if is_float sv.type.t && bt != VT_LDOUBLE then
      -- TCC_RV64I_SOFT: load the bit pattern as an integer constant
      let mut v' := sv
      v' := { v' with type := { v'.type with t := if bt == VT_DOUBLE then VT_LLONG else VT_INT } }
      if bt != VT_DOUBLE then
        v' := { v' with c := v'.c.setI sv.c.lo.toUInt32.toInt32.toInt64.toUInt64 }
      loadF R n r v'
      return
    tassert (is_ireg r || bt == VT_LDOUBLE) "is_ireg(r) || bt == VT_LDOUBLE"
    if fr &&& VT_SYM != 0 then
      let (b, f) ← load_symofs R r sv false fc
      rb := b; fc := f
      do32bit := 0
    if do32bit != 0 && !fitsI fc sv then
      let si := sv.c.lo.toInt64 >>> 32
      if si != 0 then
        load_large_constant rr fc si.toUInt64.toUInt32
        fc := SIGN7 fc
        rb := rr
        do32bit := 0
      else if bt == VT_LLONG then
        zext := true
    if LOW_OVERFLOW fc then
      o ((0x37 : UInt32) ||| (rr.toUInt32 <<< 7) ||| UPPER fc)
      rb := rr
    if fc != 0 || rr != rb || do32bit != 0 || (fr &&& VT_SYM != 0) then
      EI (0x13 ||| do32bit) 0 rr rb (SIGN11 fc)
    if zext then
      EI 0x13 1 rr rr 32
      EI 0x13 5 rr rr 32
  else if v == VT_LOCAL then
    let (br, f) ← load_symofs R r sv false fc
    tassert (is_ireg r) "is_ireg(r)"
    EI 0x13 0 rr br f
  else if v < VT_CONST then
    if is_freg r && is_freg v then
      let fv ← freg v
      ER 0x53 0 rr fv fv (if bt == VT_DOUBLE then 0x11 else 0x10)
    else if is_ireg r && is_ireg v then
      EI 0x13 0 rr (← ireg v) 0
    else
      let mut func7 : Int32 := if is_ireg r then 0x70 else 0x78
      let (size, _) ← R.type_size sv.type
      if size == 8 then func7 := func7 ||| 1
      tassert (size == 4 || size == 8) "size == 4 || size == 8"
      let vr ← if is_freg v then freg v else ireg v
      o ((0x53 : UInt32) ||| (rr.toUInt32 <<< 7) ||| (vr.toUInt32 <<< 15) ||| (func7.toUInt32 <<< 25))
  else if v == VT_CMP then
    let top ← vt 0
    let mut op := top.cmp_op
    let mut a := top.cmp_r &&& 0xff
    let mut b := (top.cmp_r >>> 8) &&& 0xff
    let mut inv := false
    if op == TOK_ULT || op == TOK_UGE || op == TOK_ULE || op == TOK_UGT ||
       op == TOK_LT || op == TOK_GE || op == TOK_LE || op == TOK_GT then
      if op &&& 1 != 0 then
        inv := true
        op := op - 1
      if op &&& 7 == 6 then
        let t := a
        a := b; b := t
        inv := !inv
      ER 0x33 (if op > TOK_UGT then 2 else 3) rr a b 0
      if inv then EI 0x13 4 rr rr 1
    else if op == TOK_NE || op == TOK_EQ then
      if rr != a || b != 0 then ER 0x33 0 rr a b 0x20
      if op == TOK_NE then ER 0x33 3 rr 0 rr 0
      else EI 0x13 3 rr rr 1
  else if (v &&& ~~~1) == VT_JMP then
    let t := v &&& 1
    tassert (is_ireg r) "is_ireg(r)"
    EI 0x13 0 rr 0 t
    gjmp_addr ((← get).ind + 8)
    R.gsym fc
    EI 0x13 0 rr 0 (t ^^^ 1)
  else
    tcc_error_s R "unimp: load(non-const)"

/-- `load(r, sv)` -/
def load (R : Rec) (r : Int32) (sv : SValue) : M Unit := loadF R 2 r sv

/-- `store(r, sv)` -/
def store (R : Rec) (r : Int32) (sv : SValue) : M Unit := do
  let fr := rI sv &&& VT_VALMASK
  let rr ← if is_ireg r then ireg r else freg r
  let mut fc := ciI sv
  let bt := sv.type.t &&& VT_BTYPE
  let (size0, _) ← R.type_size sv.type
  let size := if bt == VT_LDOUBLE then 8 else size0
  if bt == VT_STRUCT then tcc_error_s R "unimp: store(struct)"
  if size > 8 then tcc_error_s R "unimp: large sized store"
  tassert (rI sv &&& VT_LVAL != 0) "sv->r & VT_LVAL"
  let mut ptrreg : Int32 := 0
  if fr == VT_LOCAL || (rI sv &&& VT_SYM != 0) then
    let (p, f) ← load_symofs R (-1) sv true fc
    ptrreg := p; fc := f
  else if fr < VT_CONST then
    ptrreg ← ireg fr
    fc := 0
  else if fr == VT_CONST then
    ptrreg := 8
    let si := sv.c.lo.toInt64 >>> 32
    if si != 0 then
      load_large_constant ptrreg fc si.toUInt64.toUInt32
      fc := SIGN7 fc
    else
      o ((0x37 : UInt32) ||| (ptrreg.toUInt32 <<< 7) ||| UPPER fc)
      fc := SIGN11 fc
  else
    tcc_error_s R "implement me: %s(!local)" [.s "store".toUTF8]
  ES (if is_freg r then 0x27 else 0x23)
     (if size == 1 then 0 else if size == 2 then 1 else if size == 4 then 2 else 3)
     ptrreg rr fc

/-- `gcall_or_jmp(docall)` -/
def gcall_or_jmp (R : Rec) (docall : Bool) : M Unit := do
  let tr : Int32 := if docall then 1 else 5
  let top ← vt 0
  if (rI top &&& (VT_VALMASK ||| VT_LVAL)) == VT_CONST &&
     ((rI top &&& VT_SYM != 0) && fitsI (ciI top) top) then
    R.greloca (← get).cur_text_section top.sym (← get).ind.toUInt64' R_RISCV_CALL_PLT (ciI top).toInt64
    o ((0x17 : UInt32) ||| (tr.toUInt32 <<< 7))
    EI 0x67 0 tr tr 0
  else if rI top < VT_CONST then
    let r ← ireg (rI top)
    EI 0x67 0 tr r 0
  else
    load R TREG_RA top
    let r ← ireg TREG_RA
    EI 0x67 0 tr r 0

/-! ## the calling convention -/

/-- `reg_pass_rec(type, rc, fieldofs, ofs)` (fuel bounds the nesting) -/
def reg_pass_recF (R : Rec) : Nat → CType → Array Int32 → Array Int32 → Int32 → M (Array Int32 × Array Int32)
  | 0, _, _, _, _ => throw (.fuel "reg_pass_rec")
  | n+1, type, rc0, fo0, ofs => do
  let mut rc := rc0
  let mut fo := fo0
  let bt := type.t &&& VT_BTYPE
  if bt == VT_STRUCT then
    let ref ← getSym type.ref
    if ref.type.t == VT_UNION then
      rc := rc.set! 0 (-1)
    else
      let mut f := ref.next
      let fuel ← fuelM
      for _ in [0:fuel] do
        if f == 0 then break
        let fs ← getSym f
        let (rc', fo') ← reg_pass_recF R n fs.type rc fo (ofs + fs.c)
        rc := rc'; fo := fo'
        f := fs.next
  else if type.t &&& VT_ARRAY != 0 then
    let ref ← getSym type.ref
    if ref.c < 0 || ref.c > 2 then
      rc := rc.set! 0 (-1)
    else
      let (sz, _) ← R.type_size ref.type
      let (rc', fo') ← reg_pass_recF R n ref.type rc fo ofs
      rc := rc'; fo := fo'
      if rc[0]! > 2 || (rc[0]! == 2 && ref.c > 1) then
        rc := rc.set! 0 (-1)
      else if ref.c == 2 && rc[0]! != 0 && rc[1]! == RC_FLOAT then
        let k := rc[0]! + 1
        rc := rc.set! 0 k
        rc := rc.set! k.toNat' RC_FLOAT
        fo := fo.set! k.toNat' (((ofs + sz) <<< 4) ||| (ref.type.t &&& VT_BTYPE))
      else if ref.c == 2 then
        rc := rc.set! 0 (-1)
  else if rc[0]! == 2 || rc[0]! < 0 || bt == VT_LDOUBLE then
    rc := rc.set! 0 (-1)
  else if rc[0]! == 0 || rc[1]! == RC_FLOAT || is_float type.t then
    let k := rc[0]! + 1
    rc := rc.set! 0 k
    rc := rc.set! k.toNat' (if is_float type.t then RC_FLOAT else RC_INT)
    fo := fo.set! k.toNat' ((ofs <<< 4) ||| (if bt == VT_PTR then VT_LLONG else type.t &&& VT_BTYPE))
  else
    rc := rc.set! 0 (-1)
  return (rc, fo)

/-- `reg_pass(type, prc, fieldofs, named)`: (prc, fieldofs) -/
def reg_pass (R : Rec) (type : CType) (named0 : Bool) : M (Array Int32 × Array Int32) := do
  let prc0 : Array Int32 := #[0, 0, 0]
  let (prc1, fo1) ← reg_pass_recF R (← fuelM) type prc0 #[0, 0, 0] 0
  let mut prc := prc1
  let mut fo := fo1
  -- TCC_RV64I_SOFT: lp64, no FP calling convention
  let named := named0 && false
  if prc[0]! ≤ 0 || !named then
    let (size, _) ← R.type_size type
    prc := #[(size + 7) >>> 3, RC_INT, RC_INT]
    fo := fo.set! 1 (((0 : Int32) <<< 4) ||| (if size ≤ 1 then VT_BYTE else if size ≤ 2 then VT_SHORT
                                   else if size ≤ 4 then VT_INT else VT_LLONG))
    fo := fo.set! 2 (((8 : Int32) <<< 4) ||| (if size ≤ 9 then VT_BYTE else if size ≤ 10 then VT_SHORT
                                   else if size ≤ 12 then VT_INT else VT_LLONG))
  return (prc, fo)

/-- gfunc_call's first loop: the per-argument `info` words and the
stack/temp space -/
private def gfunc_call_classify (R : Rec) (nb_args : Int32) :
    M (Array Int32 × Int32 × Int32) := do
  let fty := (← vt (-(nb_args.toInt))).type
  let fref ← getSym fty.ref
  let old := fref.f_func_type == FUNC_OLD
  let mut areg : Array Int32 := #[0, 8]
  let mut sa := fref.next
  let mut info : Array Int32 := Array.replicate (nb_args.toNat' + 1) 0
  let mut stack_adj : Int32 := 0
  let mut tempspace : Int32 := 0
  for i in [0:nb_args.toNat'] do
    let k : Int := 1 + (i : Int) - nb_args.toInt
    modVt k fun v => { v with type := { v.type with t := v.type.t &&& ~~~VT_ARRAY } }
    let sv ← vt k
    let (size0, align0) ← R.type_size sv.type
    let mut size := size0
    let mut align := align0
    let mut byref : Int32 := 0
    if size > 16 then
      if align < XLEN then align := XLEN
      tempspace := (tempspace + align - 1) &&& -align
      let tempofs := tempspace
      tempspace := tempspace + size
      size := 8; align := 8
      byref := (64 : Int32) ||| (tempofs <<< 7)
    let (prc, fieldofs) ← reg_pass R sv.type (old || sa != 0)
    if !old && sa == 0 && align == 2 * XLEN && size ≤ 2 * XLEN then
      areg := areg.set! 0 ((areg[0]! + 1) &&& ~~~1)
    let nregs := prc[0]!
    let mut inf : Int32 := 0
    if size == 0 then
      inf := 0
    else if (prc[1]! == RC_INT && areg[0]! ≥ 8) || (prc[1]! == RC_FLOAT && areg[1]! ≥ 16) ||
            (nregs == 2 && prc[1]! == RC_FLOAT && prc[2]! == RC_FLOAT && areg[1]! ≥ 15) ||
            (nregs == 2 && prc[1]! != prc[2]! && (areg[1]! ≥ 16 || areg[0]! ≥ 8)) then
      inf := 32
      if align < XLEN then align := XLEN
      stack_adj := stack_adj + ((size + align - 1) &&& -align)
      if !old && sa == 0 then
        areg := #[8, 16]
    else
      let ai := (prc[1]! - 1).toNat'
      inf := areg[ai]!
      areg := areg.set! ai (areg[ai]! + 1)
      if byref == 0 then inf := inf ||| ((fieldofs[1]! &&& VT_BTYPE) <<< 12)
      tassert (fieldofs[1]! >>> 4 == 0) "!(fieldofs[1] >> 4)"
      if nregs == 2 then
        if prc[2]! == RC_FLOAT || areg[0]! < 8 then
          let aj := (prc[2]! - 1).toNat'
          inf := inf ||| (((1 : Int32) + areg[aj]!) <<< 7)
          areg := areg.set! aj (areg[aj]! + 1)
        else
          inf := inf ||| 16
          stack_adj := stack_adj + 8
        if byref == 0 then
          tassert ((fieldofs[2]! >>> 4) < 2048) "(fieldofs[2] >> 4) < 2048"
          inf := inf ||| (fieldofs[2]! <<< (12 + 4))
    inf := inf ||| byref
    info := info.set! i inf
    if sa != 0 then sa := (← getSym sa).next
  return (info, stack_adj, tempspace)

/-- gfunc_call's stores of the stack-passed and by-reference arguments:
the split-argument stack offset -/
private def gfunc_call_stack (R : Rec) (nb_args : Int32) (info : Array Int32) (stack_adj : Int32) : M Int32 := do
  let mut ofs : Int32 := 0
  let mut splitofs : Int32 := 0
  for i in [0:nb_args.toNat'] do
    let inf := info[i]!
    if inf &&& (64 ||| 32) != 0 then
      R.vrotb (nb_args - Int32.ofNat i)
      let (size0, align0) ← R.type_size (← vt 0).type
      let mut size := size0
      let mut align := align0
      let cpt := (← get).char_pointer_type
      if inf &&& 64 != 0 then
        R.vset cpt TREG_SP 0
        R.vpushi (stack_adj + (inf >>> 7))
        R.gen_op 43
        R.vpushv (← vt 0)
        R.vrott 3
        R.indir
        setType 0 (← vt (-1)).type
        R.vswap
        R.vstore
        R.vpop
        size := 8; align := 8
      if inf &&& 32 != 0 then
        if align < XLEN then align := XLEN
        R.vset cpt TREG_SP 0
        ofs := (ofs + align - 1) &&& -align
        R.vpushi ofs
        R.gen_op 43
        R.indir
        setType 0 (← vt (-1)).type
        R.vswap
        R.vstore
        setR 0 VT_CONST
        setR2 0 VT_CONST
        ofs := ofs + size
      R.vrott (nb_args - Int32.ofNat i)
    else if inf &&& 16 != 0 then
      tassert (splitofs == 0) "!splitofs"
      splitofs := ofs
      ofs := ofs + 8
  return splitofs

/-- gfunc_call's loads of the register-passed arguments -/
private def gfunc_call_regs (R : Rec) (nb_args : Int32) (info : Array Int32) (splitofs : Int32) : M Unit := do
  for i in [0:nb_args.toNat'] do
    let ii := info[(nb_args - 1 - Int32.ofNat i).toNat']!
    let mut r := ii
    let mut r2 := r
    if r &&& 32 == 0 then
      r := r &&& 15
      r2 := if r2 &&& 64 != 0 then 0 else (r2 >>> 7) &&& 31
      tassert (r2 ≤ 16) "r2 <= 16"
      R.vrotb (Int32.ofNat i + 1)
      let origtype := (← vt 0).type
      let (size, _) ← R.type_size origtype
      if size != 0 then
        let mut loadt := (← vt 0).type.t &&& VT_BTYPE
        if loadt == VT_STRUCT then loadt := (ii >>> 12) &&& VT_BTYPE
        if ii &&& 16 != 0 then
          tassert (r2 == 0) "!r2"
          r2 := 1 + TREG_RA
        if loadt == VT_LDOUBLE then
          tassert (r2 != 0) "r2"
          r2 := r2 - 1
        else if r2 != 0 then
          R.test_lvalue
          R.vpushv (← vt 0)
        setTy 0 (loadt ||| ((← vt 0).type.t &&& VT_UNSIGNED))
        let _ ← R.gv (if r < 8 then RC_R r else RC_F (r - 8))
        setType 0 origtype
        if r2 != 0 && loadt != VT_LDOUBLE then
          r2 := r2 - 1
          tassert (r2 < 16 || r2 == TREG_RA) "r2 < 16 || r2 == TREG_RA"
          R.vswap
          R.gaddrof
          setType 0 (← get).char_pointer_type
          R.vpushi (ii >>> 20)
          R.gen_op 43
          R.indir
          setType 0 origtype
          loadt := (← vt 0).type.t &&& VT_BTYPE
          if loadt == VT_STRUCT then loadt := (ii >>> 16) &&& VT_BTYPE
          R.save_reg_upstack r2 1
          setTy 0 (loadt ||| ((← vt 0).type.t &&& VT_UNSIGNED))
          load R r2 (← vt 0)
          tassert (r2 < VT_CONST) "r2 < VT_CONST"
          vtopAdd (-1)
          setR2 0 r2
        if ii &&& 16 != 0 then
          ES 0x23 3 2 (← ireg (r2I (← vt 0))) splitofs
          setR2 0 VT_CONST
        else if loadt == VT_LDOUBLE && r2I (← vt 0) != r2 then
          tassert (r2I (← vt 0) ≤ 7 && r2 ≤ 7) "vtop->r2 <= 7 && r2 <= 7"
          EI 0x13 0 (← ireg r2) (← ireg (r2I (← vt 0))) 0
          setR2 0 r2
      R.vrott (Int32.ofNat i + 1)

/-- `gfunc_call(nb_args)` -/
def gfunc_call (R : Rec) (nb_args : Int32) : M Unit := do
  let (info, stack_adj0, tempspace0) ← gfunc_call_classify R nb_args
  let stack_adj := (stack_adj0 + 15) &&& -16
  let tempspace := (tempspace0 + 15) &&& -16
  let stack_add := stack_adj + tempspace
  let mut splitofs : Int32 := 0
  if stack_add != 0 then
    if stack_add ≥ 0x800 then
      o ((0x37 : UInt32) ||| ((5 : UInt32) <<< 7) ||| UPPER (-stack_add))
      EI 0x13 0 5 5 (SIGN11 (-stack_add))
      ER 0x33 0 2 2 5 0
    else
      EI 0x13 0 2 2 (-stack_add)
    splitofs ← gfunc_call_stack R nb_args info stack_adj
  gfunc_call_regs R nb_args info splitofs
  R.vrotb (nb_args + 1)
  R.save_regs (nb_args + 1)
  gcall_or_jmp R true
  vtopAdd (-(nb_args.toInt + 1))
  if stack_add != 0 then
    if stack_add ≥ 0x800 then
      o ((0x37 : UInt32) ||| ((5 : UInt32) <<< 7) ||| UPPER stack_add)
      EI 0x13 0 5 5 (SIGN11 stack_add)
      ER 0x33 0 2 2 5 0
    else
      EI 0x13 0 2 2 stack_add

/-- `gfunc_prolog(func_sym)` -/
def gfunc_prolog (R : Rec) (func_sym : SymId) : M Unit := do
  let fty := (← getSym func_sym).type
  let mut sym := fty.ref
  modify fun s => { s with loc := -16, func_sub_sp_offset := s.ind, ind := s.ind + 5 * 4 }
  let mut areg : Array Int32 := #[0, 0]
  let mut addr : Int32 := 0
  let mut param_addr : Int32 := 0
  let (size, _) ← R.type_size (← get).func_vt
  if size > 2 * XLEN then
    modify fun s => { s with loc := s.loc - 8 }
    modify fun s => { s with func_vc := s.loc }
    ES 0x23 3 8 ((10 : Int32) + areg[0]!) (← get).loc
    areg := areg.set! 0 (areg[0]! + 1)
  let fuel ← fuelM
  for _ in [0:fuel] do
    sym := (← getSym sym).next
    if sym == 0 then break
    let mut byref : Int32 := 0
    let mut type := (← getSym sym).type
    let (size0, align0) ← R.type_size type
    let mut size := size0
    let mut align := align0
    if size > 2 * XLEN then
      type := (← get).char_pointer_type
      size := 8; align := 8; byref := 8
    let (prc, fieldofs) ← reg_pass R type true
    let regcount := prc[0]!
    if areg[(prc[1]! - 1).toNat']! ≥ 8 ||
       (regcount == 2 && ((prc[1]! == RC_FLOAT && prc[2]! == RC_FLOAT && areg[1]! ≥ 7) ||
                          (prc[1]! != prc[2]! && (areg[1]! ≥ 8 || areg[0]! ≥ 8)))) then
      if align < XLEN then align := XLEN
      addr := (addr + align - 1) &&& -align
      param_addr := addr
      addr := addr + size
    else
      modify fun s => { s with loc := s.loc - regcount * 8 }
      let loc := (← get).loc
      param_addr := loc
      for i in [0:regcount.toNat'] do
        let pc := prc[1 + i]!
        if areg[(pc - 1).toNat']! ≥ 8 then
          tassert (i == 1 && regcount == 2 && (addr &&& 7) == 0) "i == 1 && regcount == 2 && !(addr & 7)"
          EI 0x03 3 5 8 addr
          addr := addr + 8
          ES 0x23 3 8 5 (loc + Int32.ofNat i * 8)
        else if pc == RC_FLOAT then
          ES 0x27 (if size / regcount == 4 then 2 else 3) 8 ((10 : Int32) + areg[1]!) (loc + (fieldofs[i + 1]! >>> 4))
          areg := areg.set! 1 (areg[1]! + 1)
        else
          ES 0x23 3 8 ((10 : Int32) + areg[0]!) (loc + Int32.ofNat i * 8)
          areg := areg.set! 0 (areg[0]! + 1)
    let _ ← R.gfunc_set_param sym param_addr byref
  modify fun s => { s with func_va_list_ofs := addr, num_va_regs := 0 }
  if (← get).func_var != 0 then
    for _ in [0:8] do
      if areg[0]! ≥ 8 then break
      modify fun s => { s with num_va_regs := s.num_va_regs + 1 }
      ES 0x23 3 8 ((10 : Int32) + areg[0]!) ((-8 : Int32) + (← get).num_va_regs * 8)
      areg := areg.set! 0 (areg[0]! + 1)

/-- `gfunc_sret(vt, variadic, ret, &ret_align, &regsize)`: (result, ret,
ret_align, regsize) -/
def gfunc_sret (R : Rec) (vt_ : CType) (_variadic : Int32) (ret : CType) : M (Int32 × CType × Int32 × Int32) := do
  let (size, _) ← R.type_size vt_
  let ret_align : Int32 := 1
  let mut regsize : Int32 := 8
  if size > 16 then return (0, ret, ret_align, regsize)
  let (prc, fieldofs) ← reg_pass R vt_ true
  let nregs := prc[0]!
  if nregs == 2 && prc[1]! != prc[2]! then return (-1, ret, ret_align, regsize)
  if prc[1]! == RC_FLOAT then regsize := size / nregs
  return (nregs, { t := fieldofs[1]! &&& VT_BTYPE, ref := 0 }, ret_align, regsize)

/-- `arch_transfer_ret_regs(aftercall)` -/
def arch_transfer_ret_regs (R : Rec) (aftercall : Bool) : M Unit := do
  let (prc, fieldofs) ← reg_pass R (← vt 0).type true
  tassert (prc[0]! == 2 && prc[1]! != prc[2]! && (fieldofs[1]! >>> 4) == 0) "prc[0] == 2 && prc[1] != prc[2] && !(fieldofs[1] >> 4)"
  tassert (rI (← vt 0) == (VT_LOCAL ||| VT_LVAL)) "vtop->r == (VT_LOCAL | VT_LVAL)"
  R.vpushv (← vt 0)
  setTy 0 (fieldofs[1]! &&& VT_BTYPE)
  let r1 := if prc[1]! == RC_INT then REG_IRET else REG_FRET
  if aftercall then store R r1 (← vt 0) else load R r1 (← vt 0)
  modVt 0 fun v => { v with c := v.c.setI (v.c.lo + (fieldofs[2]! >>> 4).toInt64.toUInt64) }
  setTy 0 (fieldofs[2]! &&& VT_BTYPE)
  let r2 := if prc[2]! == RC_INT then REG_IRET else REG_FRET
  if aftercall then store R r2 (← vt 0) else load R r2 (← vt 0)
  vtopAdd (-1)

/-- `gfunc_epilog()` -/
def gfunc_epilog : M Unit := do
  let nva := (← get).num_va_regs
  modify fun s => { s with loc := s.loc - nva * 8 }
  let v := (-(← get).loc + 15) &&& -16
  let mut d := v
  EI 0x13 0 2 8 (nva * 8)
  EI 0x03 3 1 8 (-8)
  EI 0x03 3 8 8 (-16)
  EI 0x67 0 0 1 0
  let large_ofs_ind := (← get).ind
  let fsso := (← get).func_sub_sp_offset
  if v ≥ ((1 : Int32) <<< 11) then
    d := 16
    EI 0x13 0 8 2 (d - nva * 8)
    o ((0x37 : UInt32) ||| ((5 : UInt32) <<< 7) ||| UPPER (v - 16))
    EI 0x13 0 5 5 (SIGN11 (v - 16))
    ER 0x33 0 2 2 5 0x20
    gjmp_addr (fsso + 5 * 4)
  let saved_ind := (← get).ind
  modify fun s => { s with ind := fsso }
  EI 0x13 0 2 2 (-d)
  ES 0x23 3 2 1 (d - 8 - nva * 8)
  ES 0x23 3 2 8 (d - 16 - nva * 8)
  if v < ((1 : Int32) <<< 11) then
    EI 0x13 0 8 2 (d - nva * 8)
  else
    gjmp_addr large_ofs_ind
  if (← get).ind - fsso != 5 * 4 then
    EI 0x13 0 0 0 0
  modify fun s => { s with ind := saved_ind }

/-- `gen_va_start()` -/
def gen_va_start (R : Rec) : M Unit := do
  vtopAdd (-1)
  let s ← get
  R.vset s.char_pointer_type VT_LOCAL s.func_va_list_ofs

/-- `gen_fill_nops(bytes)` -/
def gen_fill_nops (R : Rec) (bytes0 : Int32) : M Unit := do
  if bytes0 &&& 3 != 0 then tcc_error_s R "alignment of code section not multiple of 4"
  let mut bytes := bytes0
  let fuel ← fuelM
  for _ in [0:fuel] do
    if bytes ≤ 0 then break
    EI 0x13 0 0 0 0
    bytes := bytes - 4

/-- `gjmp_cond(op, t)` -/
def gjmp_cond (op0 t : Int32) : M Int32 := do
  let top ← vt 0
  let mut a := top.cmp_r &&& 0xff
  let mut b := (top.cmp_r >>> 8) &&& 0xff
  let mut op := op0
  if op0 == TOK_ULT then op := 6
  else if op0 == TOK_UGE then op := 7
  else if op0 == TOK_ULE then op := 7; let tmp := a; a := b; b := tmp
  else if op0 == TOK_UGT then op := 6; let tmp := a; a := b; b := tmp
  else if op0 == TOK_LT then op := 4
  else if op0 == TOK_GE then op := 5
  else if op0 == TOK_LE then op := 5; let tmp := a; a := b; b := tmp
  else if op0 == TOK_GT then op := 4; let tmp := a; a := b; b := tmp
  else if op0 == TOK_NE then op := 1
  else if op0 == TOK_EQ then op := 0
  o ((0x63 : UInt32) ||| ((op ^^^ 1).toUInt32 <<< 12) ||| (a.toUInt32 <<< 15) ||| (b.toUInt32 <<< 20) ||| ((8 : UInt32) <<< 7))
  gjmp t

/-- `gjmp_append(n, t)` -/
def gjmp_append (n t : Int32) : M Int32 := do
  if n == 0 then return t
  let sec := (← get).cur_text_section
  let mut n1 := n.toUInt32
  let fuel ← fuelM
  for _ in [0:fuel] do
    let n2 := Bytes.r32 (← getSec sec).data n1.toNat
    if n2 == 0 then break
    n1 := n2
  modSecData sec fun d => Bytes.w32 d n1.toNat t.toUInt32
  return n

/-! ## arithmetic (TCC_RV64I_SOFT: libgcc helpers) -/

/-- `soft_call(name, nargs, t)` -/
def soft_call (R : Rec) (name : ByteArray) (nargs t : Int32) : M Unit := do
  R.vpush_helper_func (← tok_alloc R name).tok
  R.vrott (nargs + 1)
  gfunc_call R nargs
  R.vpushi 0
  setTy 0 t
  setR 0 REG_IRET
  setR2 0 (if t &&& VT_BTYPE == VT_LDOUBLE then 1 else VT_CONST)

/-- `gen_opil_soft(op, ll)` -/
def gen_opil_soft (R : Rec) (op : Int32) (ll : Bool) : M Bool := do
  let name : String ←
    if op == 42 then pure "__muldi3"
    else if op == 47 || op == TOK_PDIV then pure "__divdi3"
    else if op == 37 then pure "__moddi3"
    else if op == TOK_UDIV then pure "__udivdi3"
    else if op == TOK_UMOD then pure "__umoddi3"
    else return false
  R.gv2 RC_INT RC_INT
  for k in [0:2] do
    let i : Int := (k : Int) - 1
    if !ll && (op == TOK_UDIV || op == TOK_UMOD) then
      let r ← ireg (rI (← vt i))
      EI 0x13 1 r r 32
      EI 0x13 5 r r 32
    setTy i VT_LLONG
  soft_call R name.toUTF8 2 VT_LLONG
  if !ll then EI 0x1b 0 10 10 0
  return true

/-- `gen_opil(op, ll)` (`fuel` bounds its recursive calls, at most one deep) -/
def gen_opilF (R : Rec) : Nat → Int32 → Bool → M Unit
  | 0, _, _ => throw (.fuel "gen_opil")
  | n+1, op, ll0 => do
  if ← gen_opil_soft R op ll0 then return
  let ll : Int32 := if ll0 then 0 else 8
  let top ← vt 0
  if (rI top &&& (VT_VALMASK ||| VT_LVAL ||| VT_SYM)) == VT_CONST then
    let fc0 := ciI top
    if fitsI fc0 top && !LOW_OVERFLOW fc0 then
      let mut fc := fc0
      let mut cll : Int32 := 0
      let m : Int32 := if ll != 0 then 31 else 63
      R.vswap
      let _ ← R.gv RC_INT
      let mut a ← ireg (rI (← vt 0))
      vtopAdd (-1)
      let d ← R.get_reg RC_INT
      vtopAdd 1
      R.vswap
      -- the switch: `none` breaks to the general code, `some f3` is do_cop
      let mut cop : Option Int32 := none
      if op == 45 && fc > -((1 : Int32) <<< 11) then
        fc := -fc; cop := some 0; cll := ll
      else if op == 43 then
        cop := some 0; cll := ll
      else if op == TOK_LE && fc < ((1 : Int32) <<< 11) - 1 then
        fc := fc + 1; cop := some 2
      else if op == TOK_LT then cop := some 2
      else if op == TOK_ULE && !(fc ≥ ((1 : Int32) <<< 11) - 1 || fc == -1) then
        fc := fc + 1; cop := some 3
      else if op == TOK_ULT then cop := some 3
      else if op == 94 then cop := some 4
      else if op == 124 then cop := some 6
      else if op == 38 then cop := some 7
      else if op == TOK_SHL then cop := some 1; cll := ll; fc := fc &&& m
      else if op == TOK_SHR then cop := some 5; cll := ll; fc := fc &&& m
      else if op == TOK_SAR then cop := some 5; cll := ll; fc := 1024 ||| (fc &&& m)
      else if op == TOK_UGE || op == TOK_UGT || op == TOK_GE || op == TOK_GT then
        gen_opilF R n (op - 1) (ll == 0)
        modVt 0 fun v => v.setCmp (v.cmp_op ^^^ 1) v.cmp_r
        return
      else if op == TOK_NE || op == TOK_EQ then
        if fc != 0 then
          gen_opilF R n 45 (ll == 0)
          a ← ireg (rI (← vt 0))
          vtopAdd 1
        vtopAdd (-1)
        R.vset_VT_CMP op
        setCmpR (a)
        return
      match cop with
      | some func3 =>
        let id ← ireg d
        EI (0x13 ||| cll) func3 id a fc
        vtopAdd (-1)
        if op ≥ TOK_ULT && op ≤ TOK_GT then
          R.vset_VT_CMP TOK_NE
          setCmpR (id)
        else
          setR 0 d
        return
      | none => pure ()
  R.gv2 RC_INT RC_INT
  let a ← ireg (rI (← vt (-1)))
  let b ← ireg (rI (← vt 0))
  vtopAdd (-2)
  let d0 ← R.get_reg RC_INT
  vtopAdd 1
  setR 0 d0
  let d ← ireg d0
  if op == 43 then ER (0x33 ||| ll) 0 d a b 0
  else if op == 45 then ER (0x33 ||| ll) 0 d a b 0x20
  else if op == TOK_SAR then ER (0x33 ||| ll ||| ll) 5 d a b 0x20
  else if op == TOK_SHR then ER (0x33 ||| ll ||| ll) 5 d a b 0
  else if op == TOK_SHL then ER (0x33 ||| ll) 1 d a b 0
  else if op == 42 then ER (0x33 ||| ll) 0 d a b 1
  else if op == 47 || op == TOK_PDIV then ER (0x33 ||| ll) 4 d a b 1
  else if op == 38 then ER 0x33 7 d a b 0
  else if op == 94 then ER 0x33 4 d a b 0
  else if op == 124 then ER 0x33 6 d a b 0
  else if op == 37 then ER (if ll != 0 then 0x3b else 0x33) 6 d a b 1
  else if op == TOK_UMOD then ER (0x33 ||| ll) 7 d a b 1
  else if op == TOK_UDIV then ER (0x33 ||| ll) 5 d a b 1
  else if op ≥ TOK_ULT && op ≤ TOK_GT then
    R.vset_VT_CMP op
    setCmpR (a ||| (b <<< 8))
  else
    tcc_error_s R "implement me: %s(%s)" [.s "gen_opil".toUTF8, .s (← get_tok_str op none)]

/-- `gen_opil(op, ll)` -/
def gen_opil (R : Rec) (op : Int32) (ll : Bool) : M Unit := gen_opilF R 3 op ll

/-- `gen_opi(op)` -/
def gen_opi (R : Rec) (op : Int32) : M Unit := gen_opil R op false

/-- `gen_opl(op)` -/
def gen_opl (R : Rec) (op : Int32) : M Unit := gen_opil R op true

/-- `gen_opf(op)` -/
def gen_opf (R : Rec) (op : Int32) : M Unit := do
  let top ← vt 0
  if top.type.t == VT_LDOUBLE then
    let type := top.type
    let (func, cond) : Int32 × Int32 ←
      if op == 42 then pure (Tok.TOK___multf3, -1)
      else if op == 43 then pure (Tok.TOK___addtf3, -1)
      else if op == 45 then pure (Tok.TOK___subtf3, -1)
      else if op == 47 then pure (Tok.TOK___divtf3, -1)
      else if op == TOK_EQ then pure (Tok.TOK___eqtf2, 1)
      else if op == TOK_NE then pure (Tok.TOK___netf2, 0)
      else if op == TOK_LT then pure (Tok.TOK___lttf2, 10)
      else if op == TOK_GE then pure (Tok.TOK___getf2, 11)
      else if op == TOK_LE then pure (Tok.TOK___letf2, 12)
      else if op == TOK_GT then pure (Tok.TOK___gttf2, 13)
      else do tassert false "0"; pure (0, -1)
    R.vpush_helper_func func
    R.vrott 3
    gfunc_call R 2
    R.vpushi 0
    setR 0 REG_IRET
    setR2 0 (if cond < 0 then 1 else VT_CONST)
    if cond < 0 then
      setType 0 type
    else
      R.vpushi 0
      gen_opil R op true
    return
  -- TCC_RV64I_SOFT
  let mut ar := true
  let n : String ←
    if op == 43 then pure "add"
    else if op == 45 then pure "sub"
    else if op == 42 then pure "mul"
    else if op == 47 then pure "div"
    else if op == TOK_EQ then pure "eq"
    else if op == TOK_NE then pure "ne"
    else if op == TOK_LT then pure "lt"
    else if op == TOK_LE then pure "le"
    else if op == TOK_GT then pure "gt"
    else if op == TOK_GE then pure "ge"
    else tcc_error_s R "implement me: %s(%s)" [.s "gen_opf".toUTF8, .s (← get_tok_str op none)]
  if op == TOK_EQ || op == TOK_NE || op == TOK_LT || op == TOK_LE || op == TOK_GT || op == TOK_GE then
    ar := false
  let bt := top.type.t &&& VT_BTYPE
  let name := pstrcpy 16 (cfmt "__%s%cf%c" [.s n.toUTF8, .i (if bt == VT_DOUBLE then 100 else 115),
                                          .i (if ar then 51 else 50)])
  if ar then
    soft_call R name 2 bt
  else
    soft_call R name 2 VT_LLONG
    R.vpushi 0
    gen_opil R op true

/-! ## conversions -/

/-- `gen_cvt_csti(t)` -/
def gen_cvt_csti (R : Rec) (t : Int32) : M Unit := do
  let r ← ireg (← R.gv RC_INT)
  if t &&& VT_BTYPE == VT_SHORT then
    if t &&& VT_UNSIGNED != 0 then
      EI 0x13 1 r r 48
      EI 0x13 5 r r 48
    else
      EI 0x13 1 r r 48
      EIu 0x13 5 r r (0x400 ||| 48)
  else
    if t &&& VT_UNSIGNED != 0 then
      EI 0x13 7 r r 0xff
    else
      EI 0x13 1 r r 56
      EIu 0x13 5 r r (0x400 ||| 56)

/-- `gen_cvt_sxtw()` -/
def gen_cvt_sxtw (R : Rec) : M Unit := do
  let r ← ireg (← R.gv RC_INT)
  EI 0x1b 0 r r 0

/-- `gen_cvt_itof(t)` -/
def gen_cvt_itof (R : Rec) (t : Int32) : M Unit := do
  let _rr ← ireg (← R.gv RC_INT)
  let vty := (← vt 0).type.t
  let u := vty &&& VT_UNSIGNED != 0
  let l := vty &&& VT_BTYPE == VT_LLONG
  if t == VT_LDOUBLE then
    let func := if l then (if u then Tok.TOK___floatunditf else Tok.TOK___floatditf)
                else (if u then Tok.TOK___floatunsitf else Tok.TOK___floatsitf)
    R.vpush_helper_func func
    R.vrott 2
    gfunc_call R 1
    R.vpushi 0
    setTy 0 t
    setR 0 REG_IRET
    setR2 0 1
  else
    -- TCC_RV64I_SOFT: __float{,un}{si,di}{sf,df}
    let name := pstrcpy 24 (cfmt "__float%s%s%s" [.s (if u then "un" else "").toUTF8,
      .s (if l then "di" else "si").toUTF8, .s (if t == VT_DOUBLE then "df" else "sf").toUTF8])
    setTy 0 (if l then VT_LLONG else VT_INT)
    soft_call R name 1 t

/-- `gen_cvt_ftoi(t)` -/
def gen_cvt_ftoi (R : Rec) (t : Int32) : M Unit := do
  let ft := (← vt 0).type.t &&& VT_BTYPE
  let l := t &&& VT_BTYPE == VT_LLONG
  let u := t &&& VT_UNSIGNED != 0
  if ft == VT_LDOUBLE then
    let func := if l then (if u then Tok.TOK___fixunstfdi else Tok.TOK___fixtfdi)
                else (if u then Tok.TOK___fixunstfsi else Tok.TOK___fixtfsi)
    R.vpush_helper_func func
    R.vrott 2
    gfunc_call R 1
    R.vpushi 0
    setTy 0 t
    setR 0 REG_IRET
  else
    -- TCC_RV64I_SOFT: __fix{,uns}{sf,df}{si,di}
    let name := pstrcpy 24 (cfmt "__fix%s%s%s" [.s (if u then "uns" else "").toUTF8,
      .s (if ft == VT_DOUBLE then "df" else "sf").toUTF8, .s (if l then "di" else "si").toUTF8])
    soft_call R name 1 t

/-- `gen_cvt_ftof(dt)` -/
def gen_cvt_ftof (R : Rec) (dt0 : Int32) : M Unit := do
  let st := (← vt 0).type.t &&& VT_BTYPE
  let dt := dt0 &&& VT_BTYPE
  if st == dt then return
  -- TCC_RV64I_SOFT: __extend{sf,df}{df,tf}2 / __trunc{df,tf}{sf,df}2
  let fk : Array Int := #[115, 100, 116]   -- "sdt"
  let si : Nat := if st == VT_FLOAT then 0 else if st == VT_DOUBLE then 1 else 2
  let di : Nat := if dt == VT_FLOAT then 0 else if dt == VT_DOUBLE then 1 else 2
  let name := pstrcpy 24 (cfmt "__%s%cf%cf2" [.s (if di > si then "extend" else "trunc").toUTF8,
    .i fk[si]!, .i fk[di]!])
  soft_call R name 1 dt

/-- `gen_increment_tcov(sv)`: test coverage (-ftest-coverage) is off in
`tcc -run` without options -/
def gen_increment_tcov (_R : Rec) (_sv : SValue) : M Unit :=
  unsupported "gen_increment_tcov (test coverage)"

/-- `ggoto()` -/
def ggoto (R : Rec) : M Unit := do
  gcall_or_jmp R false
  vtopAdd (-1)

/-- `gen_vla_sp_save(addr)` -/
def gen_vla_sp_save (addr : Int32) : M Unit := do
  if LOW_OVERFLOW addr then
    o ((0x37 : UInt32) ||| ((5 : UInt32) <<< 7) ||| UPPER addr)
    ER 0x33 0 5 5 8 0
    ES 0x23 3 5 2 (SIGN11 addr)
  else
    ES 0x23 3 8 2 addr

/-- `gen_vla_sp_restore(addr)` -/
def gen_vla_sp_restore (addr : Int32) : M Unit := do
  if LOW_OVERFLOW addr then
    o ((0x37 : UInt32) ||| ((5 : UInt32) <<< 7) ||| UPPER addr)
    ER 0x33 0 5 5 8 0
    EI 0x03 3 2 5 (SIGN11 addr)
  else
    EI 0x03 3 2 8 addr

/-- `gen_vla_alloc(type, align)` -/
def gen_vla_alloc (R : Rec) (_type : CType) (_align : Int32) : M Unit := do
  let rr ← ireg (← R.gv RC_INT)
  EI 0x13 0 rr rr 15
  EI 0x13 7 rr rr (-16)
  ER 0x33 0 2 2 rr 0x20
  R.vpop

/-- `gen_clear_cache()` (TCC_BAREMETAL: no fence.i) -/
def gen_clear_cache : M Unit :=
  o 0x0ff0000f

end Tcc.Model
