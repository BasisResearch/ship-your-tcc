import Tcc.Model.Gen1

/-!
# TccModel: tccgen.c, part 2 — operators, types, casts, stores

tccgen.c lines 2265–3905: constant folding (gen_opic, gen_opif with the
image's binary128 `long double`), type printing and compatibility,
`combine_types`, `gen_op`, `gen_cast`, `type_size`, `mk_pointer`,
assignment checks, `vstore`, `inc`.
-/

namespace Tcc.Model
open C

/-! ## float views of CValue (union semantics) -/

/-- the binary128 value of a float constant of basic type `bt` -/
def cvLd (bt : Int32) (c : CValue) : Nat :=
  if bt == VT_FLOAT then SF.cvt SF.F32 SF.F128 c.fbits.toNat
  else if bt == VT_DOUBLE then SF.cvt SF.F64 SF.F128 c.lo.toNat
  else c.lo.toNat + c.hi.toNat * 2 ^ 64

/-- store a binary128 value back as `bt` (only the member's bytes) -/
def cvSetLd (bt : Int32) (c : CValue) (f : Nat) : CValue :=
  if bt == VT_FLOAT then c.setFbits (UInt32.ofNat (SF.cvt SF.F128 SF.F32 f))
  else if bt == VT_DOUBLE then c.setI (UInt64.ofNat (SF.cvt SF.F128 SF.F64 f))
  else { c with lo := UInt64.ofNat (f % 2 ^ 64), hi := UInt64.ofNat (f / 2 ^ 64) }

/-- `c.ld = x` (all 16 bytes) -/
def cvLdSet (c : CValue) (f : Nat) : CValue :=
  { c with lo := UInt64.ofNat (f % 2 ^ 64), hi := UInt64.ofNat (f / 2 ^ 64) }
def cvLdGet (c : CValue) : Nat := c.lo.toNat + c.hi.toNat * 2 ^ 64

/-! ## integer constant folding -/

/-- `value64(l1, t)` -/
def value64 (l1 : UInt64) (t : Int32) : UInt64 :=
  if t &&& VT_BTYPE == VT_LLONG || t &&& VT_BTYPE == VT_PTR then l1
  else if t &&& VT_UNSIGNED != 0 then l1.toUInt32.toUInt64
  else l1.toUInt32.toUInt64 ||| (0 - (l1 &&& 0x80000000))

def gen_opic_sdiv (a b : UInt64) : UInt64 :=
  let x := (if a >>> 63 != 0 then 0 - a else a) / (if b >>> 63 != 0 then 0 - b else b)
  if (a ^^^ b) >>> 63 != 0 then 0 - x else x

def gen_opic_lt (a b : UInt64) : Bool :=
  (a ^^^ ((1 : UInt64) <<< 63)) < (b ^^^ ((1 : UInt64) <<< 63))

/-- `gen_opic(op)` -/
def gen_opic (R : Rec) (op0 : Int32) : M Unit := do
  let mut op := op0
  let v1 ← vt (-1)
  let v2 ← vt 0
  let t1 := v1.type.t &&& VT_BTYPE
  let t2 := v2.type.t &&& VT_BTYPE
  let c1 := (v1.r.toInt32' &&& (VT_VALMASK ||| VT_LVAL ||| VT_SYM)) == VT_CONST
  let mut c2 := (v2.r.toInt32' &&& (VT_VALMASK ||| VT_LVAL ||| VT_SYM)) == VT_CONST
  let mut l1 := if c1 then value64 v1.c.i v1.type.t else 0
  let mut l2 := if c2 then value64 v2.c.i v2.type.t else 0
  let shm : UInt64 := if t1 == VT_LLONG then 63 else 31
  let mut general := false
  if c1 && c2 then
    let mut folded := true
    if op == 43 then l1 := l1 + l2
    else if op == 45 then l1 := l1 - l2
    else if op == 38 then l1 := l1 &&& l2
    else if op == 94 then l1 := l1 ^^^ l2
    else if op == 124 then l1 := l1 ||| l2
    else if op == 42 then l1 := l1 * l2
    else if op == TOK_PDIV || op == 47 || op == 37 || op == TOK_UDIV || op == TOK_UMOD then
      if l2 == 0 then
        if (← CONST_WANTED) && !(← NOEVAL_WANTED) then
          tcc_error_s R "division by zero in constant"
        folded := false
      else
        if op == 37 then l1 := l1 - l2 * gen_opic_sdiv l1 l2
        else if op == TOK_UDIV then l1 := l1 / l2
        else if op == TOK_UMOD then l1 := l1 % l2
        else l1 := gen_opic_sdiv l1 l2
    else if op == TOK_SHL then l1 := l1 <<< (l2 &&& shm)
    else if op == TOK_SHR then l1 := l1 >>> (l2 &&& shm)
    else if op == TOK_SAR then
      l1 := if l1 >>> 63 != 0 then ~~~((~~~l1) >>> (l2 &&& shm)) else l1 >>> (l2 &&& shm)
    else if op == TOK_ULT then l1 := if l1 < l2 then 1 else 0
    else if op == TOK_UGE then l1 := if l1 ≥ l2 then 1 else 0
    else if op == TOK_EQ then l1 := if l1 == l2 then 1 else 0
    else if op == TOK_NE then l1 := if l1 != l2 then 1 else 0
    else if op == TOK_ULE then l1 := if l1 ≤ l2 then 1 else 0
    else if op == TOK_UGT then l1 := if l1 > l2 then 1 else 0
    else if op == TOK_LT then l1 := if gen_opic_lt l1 l2 then 1 else 0
    else if op == TOK_GE then l1 := if !gen_opic_lt l1 l2 then 1 else 0
    else if op == TOK_LE then l1 := if !gen_opic_lt l2 l1 then 1 else 0
    else if op == TOK_GT then l1 := if gen_opic_lt l2 l1 then 1 else 0
    else if op == TOK_LAND then l1 := if l1 != 0 && l2 != 0 then 1 else 0
    else if op == TOK_LOR then l1 := if l1 != 0 || l2 != 0 then 1 else 0
    else folded := false
    if folded then
      let nc := v2.r.toInt32' &&& VT_NONCONST
      modVt (-1) fun v => { v with c := v.c.setI (value64 l1 v.type.t), r := v.r ||| nc.toUInt16' }
      vtopDec
      return
    general := true
  if !general then
    -- if commutative ops, put c2 as constant
    if c1 && (op == 43 || op == 38 || op == 94 || op == 124 || op == 42 || op == TOK_EQ || op == TOK_NE) then
      vswap R
      c2 := c1
      l2 := l1
    if c1 && ((l1 == 0 && (op == TOK_SHL || op == TOK_SHR || op == TOK_SAR)) ||
              (l1 == 0xffffffffffffffff && op == TOK_SAR)) then
      vpop R
    else if c2 && ((l2 == 0 && (op == 38 || op == 42)) ||
                   (op == 124 && (l2 == 0xffffffffffffffff || (l2 == 0xFFFFFFFF && t2 != VT_LLONG))) ||
                   (l2 == 1 && (op == 37 || op == TOK_UMOD))) then
      if l2 == 1 then setVtCi 0 0
      vswap R
      vtopDec
    else if c2 && (((op == 42 || op == 47 || op == TOK_UDIV || op == TOK_PDIV) && l2 == 1) ||
                   ((op == 43 || op == 45 || op == 124 || op == 94 || op == TOK_SHL || op == TOK_SHR || op == TOK_SAR) && l2 == 0) ||
                   (op == 38 && (l2 == 0xffffffffffffffff || (l2 == 0xFFFFFFFF && t2 != VT_LLONG)))) then
      vtopDec
    else if c2 && (op == 42 || op == TOK_PDIV || op == TOK_UDIV || op == TOK_UMOD) then
      if l2 > 0 && (l2 &&& (l2 - 1)) == 0 then
        if op == TOK_UMOD then
          setVtCi 0 (l2 - 1)
          op := 38
        else
          let mut n : Int64 := -1
          let mut l := l2
          for _ in [0:64] do
            if l == 0 then break
            l := l >>> 1
            n := n + 1
          setVtCi 0 n.toUInt64
          if op == 42 then op := TOK_SHL
          else if op == TOK_PDIV then op := TOK_SAR
          else op := TOK_SHR
      general := true
    else if c2 && (op == 43 || op == 45) &&
        (let r := (← vtr (-1)) &&& (VT_VALMASK ||| VT_LVAL ||| VT_SYM); r == (VT_CONST ||| VT_SYM) || r == VT_LOCAL) then
      if op == 45 then l2 := 0 - l2
      l2 := l2 + (← vtci (-1))
      if (l2.toUInt32.toInt32.toInt64.toUInt64) != l2 then
        general := true
      else
        vtopDec
        setVtCi 0 l2
    else
      general := true
  if general then
    if t1 == VT_LLONG || t2 == VT_LLONG || t1 == VT_PTR || t2 == VT_PTR then
      gen_opl R op
    else
      gen_opi R op
  if (← vtr) == VT_CONST then
    setVtR 0 (VT_CONST ||| VT_NONCONST)

/-- `gen_negf(op)` -/
def gen_negf (R : Rec) (_op : Int32) : M Unit := do
  let (size, _) ← R.type_size (← vt).type
  let bt := (← vtt) &&& VT_BTYPE
  if (← get).nocode_wanted == 0 then
    save_reg R (← R.gv (RC_TYPE bt))
    vdup R
    incr_bf_adr R (size - 1)
    vdup R
    vpushi R 0x80
    R.gen_op 94
    R.vstore
    vpop R
  let _ ← R.gv (RC_TYPE bt)

/-- `gen_opif(op)` -/
def gen_opif (R : Rec) (op : Int32) : M Unit := do
  let k1 : Int := if op == TOK_NEG then 0 else -1
  let v1 ← vt k1
  let v2 ← vt 0
  let bt := v1.type.t &&& VT_BTYPE
  let c1 := (v1.r.toInt32' &&& (VT_VALMASK ||| VT_LVAL ||| VT_SYM)) == VT_CONST
  let c2 := (v2.r.toInt32' &&& (VT_VALMASK ||| VT_LVAL ||| VT_SYM)) == VT_CONST
  let f := SF.F128
  let general : M Unit := do
    if op == TOK_NEG then gen_negf R op else gen_opf R op
  if c1 && c2 then
    let mut f1 := cvLd bt v1.c
    let f2 := cvLd bt v2.c
    let fin (x : Nat) : Bool := ieee_finite (UInt64.ofNat (SF.cvt f SF.F64 x))
    if !(fin f1 || !fin f2) && !(← CONST_WANTED) then return ← general
    let cmpRes (p : Int → Bool) (unordered : Bool) : Int32 :=
      match SF.cmp f f1 f2 with
      | some c => if p c then 1 else 0
      | none => if unordered then 1 else 0
    if op == 43 then f1 := SF.add f f1 f2
    else if op == 45 then f1 := SF.sub f f1 f2
    else if op == 42 then f1 := SF.mul f f1 f2
    else if op == 47 then
      if SF.cmp f f2 0 == some 0 then
        if !(← CONST_WANTED) then return ← general
        let x1 := SF.cvt f SF.F32 f1
        let x2 := SF.cvt f SF.F32 f2
        let mut y : Nat := if SF.cmp f f1 0 == some 0 then 0x7fc00000 else 0x7f800000
        y := y ||| ((x1 ^^^ x2) &&& 0x80000000)
        f1 := SF.cvt SF.F32 f y
      else
        f1 := SF.div f f1 f2
    else if op == TOK_NEG then
      f1 := SF.neg f f1
      modVt k1 fun v => { v with c := cvSetLd bt v.c f1 }
      return
    else if op == TOK_EQ || op == TOK_NE || op == TOK_LT || op == TOK_GE || op == TOK_LE || op == TOK_GT then
      let i : Int32 :=
        if op == TOK_EQ then cmpRes (· == 0) false
        else if op == TOK_NE then cmpRes (· != 0) true
        else if op == TOK_LT then cmpRes (· < 0) false
        else if op == TOK_GE then cmpRes (· ≥ 0) false
        else if op == TOK_LE then cmpRes (· ≤ 0) false
        else cmpRes (· > 0) false
      modify fun s => { s with vtop := s.vtop - 2 }
      vpushi R i
      return
    else return ← general
    vtopDec
    modVt 0 fun v => { v with c := cvSetLd bt v.c f1 }
  else
    general

/-! ## printing types -/

/-- `type_to_str(buf, buf_size, type, varstr)` -/
def type_to_str : Nat → Nat → CType → Option ByteArray → M ByteArray
  | 0, _, _, _ => throw (.fuel "type_to_str")
  | fuel+1, buf_size0, type, varstr => do
    let t := type.t
    let bt := t &&& VT_BTYPE
    let mut buf := ByteArray.empty
    let cat (b : ByteArray) (s : String) : ByteArray := pstrcat b buf_size0 s.toUTF8
    if t &&& VT_EXTERN != 0 then buf := cat buf "extern "
    if t &&& VT_STATIC != 0 then buf := cat buf "static "
    if t &&& VT_TYPEDEF != 0 then buf := cat buf "typedef "
    if t &&& VT_INLINE != 0 then buf := cat buf "inline "
    if bt != VT_PTR then
      if t &&& VT_RESTRICT != 0 then buf := cat buf "restrict "
      if t &&& VT_VOLATILE != 0 then buf := cat buf "volatile "
      if t &&& VT_CONSTANT != 0 then buf := cat buf "const "
    if (t &&& VT_DEFSIGN != 0 && bt == VT_BYTE) ||
       (t &&& VT_UNSIGNED != 0 && (bt == VT_SHORT || bt == VT_INT || bt == VT_LLONG) && !IS_ENUM t) then
      buf := cat buf (if t &&& VT_UNSIGNED != 0 then "unsigned " else "signed ")
    let pre := buf
    let buf_size := buf_size0 - buf.size
    let mut rest := ByteArray.empty
    let rcat (b : ByteArray) (s : ByteArray) : ByteArray := pstrcat b buf_size s
    let mut noVar := false
    let tstruct (tstr : String) : M ByteArray := do
      let mut r := pstrcat ByteArray.empty buf_size tstr.toUTF8
      let v := (← getSym type.ref).v &&& ~~~SYM_STRUCT
      if v ≥ SYM_FIRST_ANOM then r := pstrcat r buf_size "<anonymous>".toUTF8
      else r := pstrcat r buf_size (← get_tok_str v none)
      return r
    if bt == VT_VOID then rest := rcat rest "void".toUTF8
    else if bt == VT_BOOL then rest := rcat rest "_Bool".toUTF8
    else if bt == VT_BYTE then rest := rcat rest "char".toUTF8
    else if bt == VT_SHORT then rest := rcat rest "short".toUTF8
    else if bt == VT_INT || bt == VT_LLONG then
      let tstr := if t &&& VT_LONG != 0 then "long" else if bt == VT_INT then "int" else "long long"
      if !IS_ENUM t then rest := rcat rest tstr.toUTF8
      else rest ← tstruct "enum "
    else if bt == VT_FLOAT then rest := rcat rest "float".toUTF8
    else if bt == VT_DOUBLE then
      rest := rcat rest (if t &&& VT_LONG == 0 then "double" else "long double").toUTF8
    else if bt == VT_LDOUBLE then rest := rcat rest "long double".toUTF8
    else if bt == VT_STRUCT then
      rest ← tstruct (if IS_UNION t then "union " else "struct ")
    else if bt == VT_FUNC then
      let s ← getSym type.ref
      let mut buf1 := ByteArray.empty
      match varstr with
      | some vs => if vs.size > 0 && vs.get! 0 == 42 then
                     buf1 := pstrcat (pstrcat (pstrcat buf1 256 "(".toUTF8) 256 vs) 256 ")".toUTF8
      | none => pure ()
      buf1 := pstrcat buf1 256 "(".toUTF8
      let mut sa := s.next
      let lfuel ← fuelM
      for _ in [0:lfuel] do
        if sa == 0 then break
        let sax ← getSym sa
        let buf2 ← type_to_str fuel 256 sax.type none
        buf1 := pstrcat buf1 256 buf2
        sa := sax.next
        if sa != 0 then buf1 := pstrcat buf1 256 ", ".toUTF8
      if s.f_func_type == FUNC_ELLIPSIS then buf1 := pstrcat buf1 256 ", ...".toUTF8
      buf1 := pstrcat buf1 256 ")".toUTF8
      rest ← type_to_str fuel buf_size s.type (some buf1)
      noVar := true
    else if bt == VT_PTR then
      let s ← getSym type.ref
      if t &&& (VT_ARRAY ||| VT_VLA) != 0 then
        let buf1 := match varstr with
          | some vs => if vs.size > 0 && vs.get! 0 == 42 then pstrcpy 256 (cfmt "(%s)[%d]" [.s vs, .i s.c.toInt])
                       else pstrcpy 256 (cfmt "%s[%d]" [.s vs, .i s.c.toInt])
          | none => pstrcpy 256 (cfmt "%s[%d]" [.s ByteArray.empty, .i s.c.toInt])
        rest ← type_to_str fuel buf_size s.type (some buf1)
      else
        let mut buf1 := pstrcpy 256 "*".toUTF8
        if t &&& VT_RESTRICT != 0 then buf1 := pstrcat buf1 256 "restrict ".toUTF8
        if t &&& VT_CONSTANT != 0 then buf1 := pstrcat buf1 256 "const ".toUTF8
        if t &&& VT_VOLATILE != 0 then buf1 := pstrcat buf1 256 "volatile ".toUTF8
        match varstr with
        | some vs => buf1 := pstrcat buf1 256 vs
        | none => pure ()
        rest ← type_to_str fuel buf_size s.type (some buf1)
      noVar := true
    if !noVar then
      match varstr with
      | some vs =>
        rest := rcat rest " ".toUTF8
        rest := rcat rest vs
      | none => pure ()
    return pre ++ rest

def type_incompatibility_error (R : Rec) (st dt : CType) (fmt : String) : M α := do
  let fuel ← fuelM
  let b1 ← type_to_str fuel 256 st none
  let b2 ← type_to_str fuel 256 dt none
  tcc_error R (cfmt fmt [.s b1, .s b2])

def type_incompatibility_warning (R : Rec) (st dt : CType) (fmt : String) : M Unit := do
  let fuel ← fuelM
  let b1 ← type_to_str fuel 256 st none
  let b2 ← type_to_str fuel 256 dt none
  tcc_warning R (cfmt fmt [.s b1, .s b2])

/-! ## types -/

/-- `pointed_type(type)` -/
@[inline] def pointed_type (type : CType) : M CType := do return (← getSym type.ref).type

/-- `type_size(type, &a)`: (size, align) -/
def type_size : Nat → CType → M (Int32 × Int32)
  | 0, _ => throw (.fuel "type_size")
  | fuel+1, type => do
    let bt := type.t &&& VT_BTYPE
    if bt == VT_STRUCT then
      let s ← getSym type.ref
      return (s.c, s.r.toInt32')
    else if bt == VT_PTR then
      if type.t &&& VT_ARRAY != 0 then
        let s ← getSym type.ref
        let (ts, a) ← type_size fuel s.type
        if s.c < 0 then return (s.c, a)
        return (ts * s.c, a)
      else return (8, 8)
    else if IS_ENUM type.t && (← symC type.ref) < 0 then return (-1, 0)
    else if bt == VT_LDOUBLE then return (LDOUBLE_SIZE, LDOUBLE_ALIGN)
    else if bt == VT_DOUBLE || bt == VT_LLONG then return (8, 8)
    else if bt == VT_INT || bt == VT_FLOAT then return (4, 4)
    else if bt == VT_SHORT then return (2, 2)
    else if bt == VT_QLONG || bt == VT_QFLOAT then return (16, 8)
    else return (1, 1)

/-- `pointed_size(type)` -/
def pointed_size (R : Rec) (type : CType) : M Int32 := do
  return (← R.type_size (← pointed_type type)).1

/-- `is_null_pointer(p)` -/
def is_null_pointer (p : SValue) : M Bool := do
  if (p.r.toInt32' &&& (VT_VALMASK ||| VT_LVAL ||| VT_SYM ||| VT_NONCONST)) != VT_CONST then return false
  let bt := p.type.t &&& VT_BTYPE
  if bt == VT_INT && p.c.i.toUInt32 == 0 then return true
  if bt == VT_LLONG && p.c.i == 0 then return true
  if bt == VT_PTR && p.c.i == 0 then
    let pt ← pointed_type p.type
    return pt.t &&& VT_BTYPE == VT_VOID && pt.t &&& VT_QUAL == 0
  return false

/-- `is_compatible_func(type1, type2)` -/
def is_compatible_func (R : Rec) (type1 type2 : CType) : M Bool := do
  let mut s1 := type1.ref
  let mut s2 := type2.ref
  let x1 ← getSym s1
  let x2 ← getSym s2
  if x1.f_func_call != x2.f_func_call then return false
  if x1.f_func_type != x2.f_func_type && x1.f_func_type != FUNC_OLD && x2.f_func_type != FUNC_OLD then return false
  let fuel ← fuelM
  for _ in [0:fuel] do
    let a ← getSym s1
    let b ← getSym s2
    if !(← R.is_compatible_unqualified_types a.type b.type) then return false
    if a.f_func_type == FUNC_OLD || b.f_func_type == FUNC_OLD then return true
    s1 := a.next
    s2 := b.next
    if s1 == 0 then return s2 == 0
    if s2 == 0 then return false
  return false

/-- `compare_types(type1, type2, unqualified)` -/
def compare_types (R : Rec) (type10 type20 : CType) (unqualified : Bool) : M Bool := do
  let mut type1 := type10
  let mut type2 := type20
  if IS_ENUM type1.t then
    if IS_ENUM type2.t then return type1.ref == type2.ref
    type1 := (← getSym type1.ref).type
  else if IS_ENUM type2.t then
    type2 := (← getSym type2.ref).type
  let mut t1 := type1.t &&& VT_TYPE
  let mut t2 := type2.t &&& VT_TYPE
  if unqualified then
    t1 := t1 &&& ~~~VT_QUAL
    t2 := t2 &&& ~~~VT_QUAL
  if t1 &&& VT_BTYPE != VT_BYTE then
    t1 := t1 &&& ~~~VT_DEFSIGN
    t2 := t2 &&& ~~~VT_DEFSIGN
  if t1 != t2 then return false
  if t1 &&& VT_ARRAY != 0 then
    let c1 ← symC type1.ref
    let c2 ← symC type2.ref
    if !(c1 < 0 || c2 < 0 || c1 == c2) then return false
  let bt1 := t1 &&& VT_BTYPE
  if bt1 == VT_PTR then
    let p1 ← pointed_type type1
    let p2 ← pointed_type type2
    if unqualified && t1 &&& VT_ARRAY != 0 then return ← R.is_compatible_unqualified_types p1 p2
    return ← R.is_compatible_types p1 p2
  else if bt1 == VT_STRUCT then return type1.ref == type2.ref
  else if bt1 == VT_FUNC then return ← is_compatible_func R type1 type2
  else return true

def is_compatible_types (R : Rec) (a b : CType) : M Bool := compare_types R a b false
def is_compatible_unqualified_types (R : Rec) (a b : CType) : M Bool := compare_types R a b true

/-- `type_qualifiers(type)` -/
def type_qualifiers (type0 : CType) : M Int32 := do
  let mut type := type0
  let fuel ← fuelM
  for _ in [0:fuel] do
    if type.t &&& VT_ARRAY == 0 then break
    type ← pointed_type type
  return type.t &&& VT_QUAL

/-- `mk_pointer(type)` -/
def mk_pointer (R : Rec) (type : CType) : M CType := do
  let s ← sym_push R SYM_FIELD type 0 (-1)
  return { t := VT_PTR ||| (type.t &&& VT_STORAGE), ref := s }

/-- `check_restrict_type(type)` -/
def check_restrict_type (R : Rec) (type0 : CType) : M Unit := do
  let mut type := type0
  let fuel ← fuelM
  for _ in [0:fuel] do
    if type.t &&& VT_BTYPE != VT_PTR then break
    let pt ← pointed_type type
    if type.t &&& VT_RESTRICT != 0 && pt.t &&& VT_BTYPE == VT_FUNC then
      tcc_error_s R "restrict-qualified type must be a pointer to object or incomplete type"
    type := pt
  if type.t &&& VT_RESTRICT != 0 then
    tcc_error_s R "restrict-qualified type must be a pointer to object or incomplete type"

/-- `parse_btype_qualify(type, qualifiers)`: array levels are copied (the
type could be a typedef) and the element type is qualified -/
def parse_btype_qualify (R : Rec) (type0 : CType) (qualifiers : Int32) : M CType := do
  if type0.t &&& VT_ARRAY == 0 then
    if qualifiers &&& VT_RESTRICT != 0 then check_restrict_type R type0
    return { type0 with t := type0.t ||| qualifiers }
  let x ← getSym type0.ref
  let ns ← sym_push R SYM_FIELD x.type 0 x.c
  let top := { type0 with ref := ns }
  let mut prev := ns
  let fuel ← fuelM
  for _ in [0:fuel] do
    let pt := (← getSym prev).type
    if pt.t &&& VT_ARRAY == 0 then
      if qualifiers &&& VT_RESTRICT != 0 then check_restrict_type R pt
      modSym prev fun y => { y with type := { y.type with t := y.type.t ||| qualifiers } }
      break
    let y ← getSym pt.ref
    let ns2 ← sym_push R SYM_FIELD y.type 0 y.c
    modSym prev fun z => { z with type := { z.type with ref := ns2 } }
    prev := ns2
  return top

def CMP_OP : Int32 := 67   -- 'C'
def SHIFT_OP : Int32 := 83 -- 'S'

/-- `combine_types(dest, op1, op2, op)`: (ok, combined type) -/
def combine_types (R : Rec) (op1v op2v0 : SValue) (op : Int32) : M (Bool × CType) := do
  let op2v := if op == SHIFT_OP then op1v else op2v0
  let type1 := op1v.type
  let type2 := op2v.type
  let t1 := type1.t
  let t2 := type2.t
  let bt1 := t1 &&& VT_BTYPE
  let bt2 := t2 &&& VT_BTYPE
  let mut type : CType := { t := VT_VOID }
  let mut ret := true
  if bt1 == VT_VOID || bt2 == VT_VOID then
    if op != 63 then tcc_error_s R "operation on void value"
    type := { t := VT_VOID }
  else if bt1 == VT_PTR || bt2 == VT_PTR then
    if op == 43 then
      if !is_integer_btype (if bt1 == VT_PTR then bt2 else bt1) then ret := false
    else if ← is_null_pointer op2v then type := type1
    else if ← is_null_pointer op1v then type := type2
    else if bt1 != bt2 then
      if (op == 63 || op == CMP_OP) && (is_integer_btype bt1 || is_integer_btype bt2) then
        tcc_warning R (cfmt "pointer/integer mismatch in %s"
          [.s (if op == 63 then "conditional expression" else "comparison").toUTF8])
      else if op != 45 || !is_integer_btype bt2 then ret := false
      type := if bt1 == VT_PTR then type1 else type2
    else
      let pt1 ← pointed_type type1
      let pt2 ← pointed_type type2
      let pbt1 := pt1.t &&& VT_BTYPE
      let pbt2 := pt2.t &&& VT_BTYPE
      let mut copied := false
      if pbt1 != VT_VOID && pbt2 != VT_VOID && !(← compare_types R pt1 pt2 true) then
        if op != 63 && op != CMP_OP then ret := false
        else
          type_incompatibility_warning R type1 type2
            (if op == 63 then "pointer type mismatch in conditional expression ('%s' and '%s')"
             else "pointer type mismatch in comparison('%s' and '%s')")
      if op == 63 then
        type := if pbt1 == VT_VOID then type1 else type2
        let newquals := (← type_qualifiers pt1) ||| (← type_qualifiers pt2)
        if (~~~(← type_qualifiers (← pointed_type type))) &&& newquals != 0 then
          let rx ← getSym type.ref
          let nr ← sym_push R SYM_FIELD rx.type 0 rx.c
          type := { type with ref := nr }
          copied := true
          let pq ← parse_btype_qualify R (← pointed_type type) newquals
          modSym nr fun y => { y with type := pq }
        let ptt ← pointed_type type
        if pt1.t &&& VT_ARRAY != 0 && pt2.t &&& VT_ARRAY != 0 && (← symC ptt.ref) < 0 &&
           ((← symC pt1.ref) > 0 || (← symC pt2.ref) > 0) then
          if !copied then
            let rx ← getSym type.ref
            let nr ← sym_push R SYM_FIELD rx.type 0 rx.c
            type := { type with ref := nr }
          let ptt ← pointed_type type
          let px ← getSym ptt.ref
          let nr2 ← sym_push R SYM_FIELD px.type 0 px.c
          modSym type.ref fun y => { y with type := { y.type with ref := nr2 } }
          let c1 ← symC pt1.ref
          let c2 ← symC pt2.ref
          modSym nr2 fun y => { y with c := if 0 < c1 then c1 else c2 }
    if op == CMP_OP then type := { type with t := VT_SIZE_T }
  else if bt1 == VT_STRUCT || bt2 == VT_STRUCT then
    if op != 63 || !(← compare_types R type1 type2 true) then ret := false
    type := type1
  else if is_float bt1 || is_float bt2 then
    if bt1 == VT_LDOUBLE || bt2 == VT_LDOUBLE then type := { t := VT_LDOUBLE }
    else if bt1 == VT_DOUBLE || bt2 == VT_DOUBLE then type := { t := VT_DOUBLE }
    else type := { t := VT_FLOAT }
  else if bt1 == VT_LLONG || bt2 == VT_LLONG then
    let mut tt := VT_LLONG ||| VT_LONG
    if bt1 == VT_LLONG then tt := tt &&& t1
    if bt2 == VT_LLONG then tt := tt &&& t2
    if (t1 &&& (VT_BTYPE ||| VT_UNSIGNED)) == (VT_LLONG ||| VT_UNSIGNED) ||
       (t2 &&& (VT_BTYPE ||| VT_UNSIGNED)) == (VT_LLONG ||| VT_UNSIGNED) then
      tt := tt ||| VT_UNSIGNED
    type := { t := tt }
  else
    let mut tt := VT_INT ||| (VT_LONG &&& (t1 ||| t2))
    if ((t1 &&& (VT_BTYPE ||| VT_UNSIGNED)) == (VT_INT ||| VT_UNSIGNED) &&
          (t1 &&& VT_BITFIELD == 0 || BIT_SIZE t1 == 32)) ||
       ((t2 &&& (VT_BTYPE ||| VT_UNSIGNED)) == (VT_INT ||| VT_UNSIGNED) &&
          (t2 &&& VT_BITFIELD == 0 || BIT_SIZE t2 == 32)) then
      tt := tt ||| VT_UNSIGNED
    type := { t := tt }
  if op == CMP_OP then type := { type with aligned := 0 }
  else if type1.aligned == type2.aligned &&
      type.t &&& (VT_BTYPE ||| VT_UNSIGNED ||| VT_LONG) == t1 &&& (VT_BTYPE ||| VT_UNSIGNED ||| VT_LONG) &&
      type.t &&& (VT_BTYPE ||| VT_UNSIGNED ||| VT_LONG) == t2 &&& (VT_BTYPE ||| VT_UNSIGNED ||| VT_LONG) then
    type := { type with aligned := type1.aligned }
  return (ret, type)

/-! ## gen_op -/

/-- `vpush_type_size(type, &a)`: returns a -/
def vpush_type_size (R : Rec) (type : CType) : M Int32 := do
  if type.t &&& VT_VLA != 0 then
    let x ← getSym type.ref
    let (_, a) ← R.type_size x.type
    vset R (← get).int_type (VT_LOCAL ||| VT_LVAL) x.c
    return a
  else
    let (size, a) ← R.type_size type
    if size < 0 then tcc_error_s R "unknown type size"
    vpushs R size.toInt64.toUInt64
    return a

/-- `gen_op(op)` -/
def gen_op (R : Rec) (op0 : Int32) : M Unit := do
  let mut op := op0
  let op_class := if op == TOK_SHR || op == TOK_SAR || op == TOK_SHL then SHIFT_OP
                  else if TOK_ISCOND op then CMP_OP else op
  let fuel ← fuelM
  for _ in [0:fuel] do   -- redo:
    let t1 ← vtt (-1)
    let t2 ← vtt 0
    let bt1 := t1 &&& VT_BTYPE
    let bt2 := t2 &&& VT_BTYPE
    if bt1 == VT_FUNC || bt2 == VT_FUNC then
      if bt2 == VT_FUNC then
        setVtType 0 (← mk_pointer R (← vt).type)
        gaddrof
      if bt1 == VT_FUNC then
        vswap R
        setVtType 0 (← mk_pointer R (← vt).type)
        gaddrof
        vswap R
      continue
    let (ok, combtype) ← combine_types R (← vt (-1)) (← vt 0) op_class
    if !ok then tcc_error_s R "invalid operand types for binary operation"
    let mut stdOp := false
    if bt1 == VT_PTR || bt2 == VT_PTR then
      if op_class == CMP_OP then stdOp := true
      else if bt1 == VT_PTR && bt2 == VT_PTR then
        if op != 45 then tcc_error_s R "invalid operand types for binary operation"
        let _ ← vpush_type_size R (← pointed_type (← vt (-1)).type)
        modVt 0 fun v => { v with type := { v.type with t := v.type.t &&& ~~~VT_UNSIGNED } }
        vrott R 3
        gen_opic R op
        setVtT 0 VT_PTRDIFF_T
        vswap R
        R.gen_op TOK_PDIV
      else
        if op != 45 && op != 43 then tcc_error_s R "invalid operand types for binary operation"
        if bt2 == VT_PTR then
          vswap R
        let type1 := (← vt (-1)).type
        let _ ← vpush_type_size R (← pointed_type type1)
        modVt 0 fun v => { v with type := { v.type with t := v.type.t &&& ~~~VT_UNSIGNED } }
        R.gen_op 42
        gen_opic R op
        let type1 := { (array_decay_alignment type1) with t := type1.t &&& ~~~(VT_ARRAY ||| VT_VLA) }
        setVtType 0 type1
    else
      if is_float combtype.t && op != 43 && op != 45 && op != 42 && op != 47 && op_class != CMP_OP then
        tcc_error_s R "invalid operand types for binary operation"
      stdOp := true
    if stdOp then
      let t := combtype.t
      let t2' := if op_class == SHIFT_OP then VT_INT else t
      if t &&& VT_UNSIGNED != 0 then
        if op == TOK_SAR then op := TOK_SHR
        else if op == 47 then op := TOK_UDIV
        else if op == 37 then op := TOK_UMOD
        else if op == TOK_LT then op := TOK_ULT
        else if op == TOK_GT then op := TOK_UGT
        else if op == TOK_LE then op := TOK_ULE
        else if op == TOK_GE then op := TOK_UGE
      vswap R
      R.gen_cast_s t
      vswap R
      R.gen_cast_s t2'
      if is_float t then gen_opif R op else gen_opic R op
      if op_class == CMP_OP then
        setVtT 0 VT_INT
        modVt 0 fun v => { v with type := { v.type with aligned := 0 } }
      else
        setVtT 0 t
        modVt 0 fun v => { v with type := { v.type with aligned := combtype.aligned } }
    modVt 0 fun v => { v with query_aligned := 0 }
    if (← vtr) &&& VT_LVAL != 0 then
      let _ ← R.gv (RC_TYPE (← vtt))
    return

/-! ## casts -/

/-- `force_charshort_cast()` -/
def force_charshort_cast (R : Rec) : M Unit := do
  let r ← vtr
  let sbt := if ((r &&& VT_MUSTCAST) >>> 10) == 2 then VT_LLONG else VT_INT
  let dbt ← vtt
  setVtR 0 (r &&& ~~~VT_MUSTCAST)
  setVtT 0 sbt
  R.gen_cast_s (if dbt == VT_BOOL then VT_BYTE ||| VT_UNSIGNED else dbt)
  setVtT 0 dbt

/-- `gen_cast_s(t)` -/
def gen_cast_s (R : Rec) (t : Int32) : M Unit := R.gen_cast { t := t }

/-- `cast_error(st, dt)` -/
def cast_error (R : Rec) (st dt : CType) : M α :=
  type_incompatibility_error R st dt "cannot convert '%s' to '%s'"

/-- `gen_cast(type)` -/
def gen_cast (R : Rec) (type : CType) : M Unit := do
  modVt 0 fun v => { v with query_aligned := 0 }
  if (← vtr) &&& VT_MUSTCAST != 0 then force_charshort_cast R
  if (← vtt) &&& VT_BITFIELD != 0 then
    let _ ← R.gv RC_INT
  if IS_ENUM type.t && (← symC type.ref) < 0 then tcc_error_s R "cast to incomplete type"
  let mut dbt := type.t &&& (VT_BTYPE ||| VT_UNSIGNED)
  let mut sbt := (← vtt) &&& (VT_BTYPE ||| VT_UNSIGNED)
  if sbt == VT_FUNC then sbt := VT_PTR
  let fuel ← fuelM
  for _ in [0:fuel] do   -- again:
    if sbt == dbt then break
    let sf := is_float sbt
    let df := is_float dbt
    let dbt_bt := dbt &&& VT_BTYPE
    let sbt_bt := sbt &&& VT_BTYPE
    if dbt_bt == VT_VOID then
      vpop R; vpushi R 0
      break
    if sbt_bt == VT_VOID then cast_error R (← vt).type type
    let c := ((← vtr) &&& (VT_VALMASK ||| VT_LVAL ||| VT_SYM)) == VT_CONST
    if c then
      let f := SF.F128
      let mut cv := (← vt).c
      if sbt == VT_FLOAT then cv := cvLdSet cv (SF.cvt SF.F32 f cv.fbits.toNat)
      else if sbt == VT_DOUBLE then cv := cvLdSet cv (SF.cvt SF.F64 f cv.lo.toNat)
      if df then
        if sbt_bt == VT_LLONG then
          if sbt &&& VT_UNSIGNED != 0 || cv.i >>> 63 == 0 then cv := cvLdSet cv (SF.ofU64 f cv.i.toNat)
          else cv := cvLdSet cv (SF.neg f (SF.ofU64 f ((0 : UInt64) - cv.i).toNat))
        else if !sf then
          if sbt &&& VT_UNSIGNED != 0 || cv.i >>> 31 == 0 then cv := cvLdSet cv (SF.ofU64 f cv.i.toUInt32.toNat)
          else cv := cvLdSet cv (SF.neg f (SF.ofU64 f ((0 : UInt32) - cv.i.toUInt32).toNat))
        if dbt == VT_FLOAT then cv := cv.setFbits (UInt32.ofNat (SF.cvt f SF.F32 (cvLdGet cv)))
        else if dbt == VT_DOUBLE then cv := cv.setI (UInt64.ofNat (SF.cvt f SF.F64 (cvLdGet cv)))
      else if sf && dbt == VT_BOOL then
        cv := cv.setI (if SF.isNonzero f (cvLdGet cv) then 1 else 0)
      else
        if sf then
          if dbt &&& VT_UNSIGNED != 0 then cv := cv.setI (UInt64.ofNat (SF.fixU f (cvLdGet cv) 64))
          else cv := cv.setI (UInt64.ofNat (SF.fixS f (cvLdGet cv) 64))
        else if sbt_bt == VT_LLONG || sbt == VT_PTR then pure ()
        else if sbt &&& VT_UNSIGNED != 0 then cv := cv.setI cv.i.toUInt32.toUInt64
        else cv := cv.setI (cv.i.toUInt32.toUInt64 ||| ((0 : UInt64) - (cv.i &&& 0x80000000)))
        if dbt_bt == VT_LLONG || dbt == VT_PTR then pure ()
        else if dbt == VT_BOOL then cv := cv.setI (if cv.i != 0 then 1 else 0)
        else
          let m : UInt64 := if dbt_bt == VT_BYTE then 0xff else if dbt_bt == VT_SHORT then 0xffff else 0xffffffff
          cv := cv.setI (cv.i &&& m)
          if dbt &&& VT_UNSIGNED == 0 then
            cv := cv.setI (cv.i ||| ((0 : UInt64) - (cv.i &&& ((m >>> 1) + 1))))
      modVt 0 fun v => { v with c := cv }
      break
    else if dbt == VT_BOOL && ((← vtr) &&& (VT_VALMASK ||| VT_LVAL ||| VT_SYM)) == (VT_CONST ||| VT_SYM) then
      setVtR 0 VT_CONST
      setVtCi 0 1
      break
    if (← get).nocode_wanted &&& DATA_ONLY_WANTED != 0 then
      if df then setVtR 0 (← get_reg R RC_FLOAT)
      break
    if dbt == VT_BOOL then
      gen_test_zero R TOK_NE
      break
    if sf || df then
      if sf && df then
        gen_cvt_ftof R dbt
      else if df then
        gen_cvt_itof R dbt
      else
        sbt := dbt
        if dbt_bt != VT_LLONG && dbt_bt != VT_INT then sbt := VT_INT
        gen_cvt_ftoi R sbt
        continue   -- goto again
      break
    let mut ds := btype_size dbt_bt
    let mut ss := btype_size sbt_bt
    if ds == 0 || ss == 0 then cast_error R (← vt).type type
    if ds == ss && ds ≥ 4 then break
    if dbt_bt == VT_PTR || sbt_bt == VT_PTR then
      tcc_warning R "cast between pointer and integer of different size".toUTF8
      if sbt_bt == VT_PTR then setVtT 0 VT_LLONG
    if (← vtr) &&& VT_LVAL != 0 then
      if ds ≤ ss then break
      if ds ≤ 4 && !(dbt == (VT_SHORT ||| VT_UNSIGNED) && sbt == VT_BYTE) then
        let _ ← R.gv RC_INT
        break
    let _ ← R.gv RC_INT
    let mut trunc : Int32 := 0
    let mut dbt' := dbt
    if ds == 8 then
      if sbt &&& VT_UNSIGNED != 0 then
        trunc := 32
      else
        gen_cvt_sxtw R
        break
      ss := ds; ds := 4; dbt' := sbt
    else if ss == 8 then
      pure ()
    else
      ss := 4
    if ds ≥ ss then break
    if ss == 4 then
      gen_cvt_csti R dbt'
      break
    let bits := (ss - ds) * 8
    setVtT 0 ((if ss == 8 then VT_LLONG else VT_INT) ||| (dbt' &&& VT_UNSIGNED))
    vpushi R bits
    R.gen_op TOK_SHL
    vpushi R (bits - trunc)
    R.gen_op TOK_SAR
    vpushi R trunc
    R.gen_op TOK_SHR
    break
  -- done:
  setVtType 0 { (array_decay_alignment type) with t := type.t &&& ~~~(VT_QUAL ||| VT_ARRAY ||| VT_TLS) }

/-- `verify_assign_cast(dt)` -/
def verify_assign_cast (R : Rec) (dt : CType) : M Unit := do
  let st := (← vt).type
  let dbt := dt.t &&& VT_BTYPE
  let sbt := st.t &&& VT_BTYPE
  if dt.t &&& VT_CONSTANT != 0 then tcc_warning R "assignment of read-only location".toUTF8
  if dbt == VT_VOID then
    if sbt != dbt then tcc_error_s R "assignment to void expression"
  else if dbt == VT_PTR then
    if ← is_null_pointer (← vt) then return
    if is_integer_btype sbt then
      tcc_warning R "assignment makes pointer from integer without a cast".toUTF8
      return
    let mut type1 ← pointed_type dt
    let mut type2 ← if sbt == VT_PTR then pointed_type st
                    else if sbt == VT_FUNC then pure st
                    else cast_error R st dt
    if ← R.is_compatible_types type1 type2 then return
    let mut qualwarn := false
    let mut lvl := 0
    let mut d := 0
    let mut s := 0
    let fuel ← fuelM
    for _ in [0:fuel] do
      if (type2.t &&& ~~~type1.t) &&& VT_QUAL != 0 then qualwarn := true
      d := type1.t &&& (VT_BTYPE ||| VT_LONG)
      s := type2.t &&& (VT_BTYPE ||| VT_LONG)
      if d != VT_PTR || s != VT_PTR then break
      type1 ← pointed_type type1
      type2 ← pointed_type type2
      lvl := lvl + 1
    if !(← R.is_compatible_unqualified_types type1 type2) then
      if (d == VT_VOID || s == VT_VOID) && lvl == 0 then pure ()
      else if d == s && is_integer_btype (s &&& VT_BTYPE) &&
              (b2i (IS_ENUM type1.t) + b2i (IS_ENUM type2.t) + b2i ((type1.t ^^^ type2.t) &&& VT_UNSIGNED != 0)) < 2 then
        pure ()
      else
        tcc_warning R "assignment from incompatible pointer type".toUTF8
        return
    if qualwarn then
      tcc_warning_c R WARNC_discarded_qualifiers "assignment discards qualifiers from pointer target type".toUTF8
  else if dbt == VT_BYTE || dbt == VT_SHORT || dbt == VT_INT || dbt == VT_LLONG then
    if sbt == VT_PTR || sbt == VT_FUNC then
      tcc_warning R "assignment makes integer from pointer without a cast".toUTF8
    else if sbt == VT_STRUCT then
      if !(← R.is_compatible_unqualified_types dt st) then cast_error R st dt
  else if dbt == VT_STRUCT then
    if !(← R.is_compatible_unqualified_types dt st) then cast_error R st dt

/-- `gen_assign_cast(dt)` -/
def gen_assign_cast (R : Rec) (dt : CType) : M Unit := do
  verify_assign_cast R dt
  R.gen_cast dt

/-- `BFVAL(VT_MUSTCAST, n)` -/
def BFVAL_MUSTCAST (n : Int32) : Int32 := 0x400 * n

/-- `vstore()` -/
def vstore (R : Rec) : M Unit := do
  let ft ← vtt (-1)
  let sbt := (← vtt) &&& VT_BTYPE
  let mut dbt := ft &&& VT_BTYPE
  verify_assign_cast R (← vt (-1)).type
  if sbt == VT_STRUCT then
    let (size, _) ← R.type_size (← vt).type
    vpushv R (← vt (-1))
    setVtT 0 VT_PTR
    gaddrof
    vswap R
    setVtT 0 VT_PTR
    gaddrof
    vpushi R size
    vpush_helper_func R Tok.TOK_memmove
    vrott R 4
    R.gfunc_call 3
  else if ft &&& VT_BITFIELD != 0 then
    vdup R
    setVt (-1) (← vt (-2))
    let bit_pos := BIT_POS ft
    let bit_size := BIT_SIZE ft
    setVtT (-1) (ft &&& ~~~VT_STRUCT_MASK)
    if dbt == VT_BOOL then
      R.gen_cast (← vt (-1)).type
      modVt (-1) fun v => { v with type := { v.type with t := (v.type.t &&& ~~~VT_BTYPE) ||| (VT_BYTE ||| VT_UNSIGNED) } }
    let r ← adjust_bf (-1)
    if dbt != VT_BOOL then
      R.gen_cast (← vt (-1)).type
      dbt := (← vtt (-1)) &&& VT_BTYPE
    if r == VT_STRUCT then
      store_packed_bf R bit_pos bit_size
    else
      let mask : UInt64 := ((1 : UInt64) <<< bit_size.toUInt64) - 1
      if dbt != VT_BOOL then
        if dbt == VT_LLONG then vpushll R mask.toInt64
        else vpushi R mask.toUInt32.toInt32
        R.gen_op 38
      vpushi R bit_pos
      R.gen_op TOK_SHL
      vswap R
      vdup R
      vrott R 3
      if dbt == VT_LLONG then vpushll R (~~~(mask <<< bit_pos.toUInt64)).toInt64
      else vpushi R (~~~(mask.toUInt32 <<< bit_pos.toUInt32)).toInt32
      R.gen_op 38
      R.gen_op 124
      R.vstore
      vpop R
  else if dbt == VT_VOID then
    vtopDec
  else
    let mut delayed_cast := false
    if (dbt == VT_BYTE || dbt == VT_SHORT) && is_integer_btype sbt then
      if (← vtr) &&& VT_MUSTCAST != 0 && btype_size dbt > btype_size sbt then
        force_charshort_cast R
      delayed_cast := true
    else
      R.gen_cast (← vt (-1)).type
    let _ ← R.gv (RC_TYPE dbt)
    if delayed_cast then
      let bf := BFVAL_MUSTCAST (b2i (sbt == VT_LLONG) + 1)
      modVt 0 fun v => { v with r := (v.r.toInt32' ||| bf).toUInt16', type := { v.type with t := ft &&& VT_TYPE } }
    if ((← vtr (-1)) &&& VT_VALMASK) == VT_LLOCAL then
      let r ← get_reg R RC_INT
      let sv : SValue := { type := { t := VT_PTRDIFF_T }, r := (VT_LOCAL ||| VT_LVAL).toUInt16',
                           c := CValue.ofI (← vtci (-1)) }
      load R r sv
      setVtR (-1) (r ||| VT_LVAL)
    let r := (← vtr) &&& VT_VALMASK
    if USING_TWO_WORDS dbt then
      let load_type := if dbt == VT_QFLOAT then VT_DOUBLE else VT_PTRDIFF_T
      setVtT (-1) load_type
      store R r (← vt (-1))
      vswap R
      incr_offset R 8
      vswap R
      store R (← vt).r2.toInt32' (← vt (-1))
    else
      store R r (← vt (-1))
    vswap R
    vtopDec

/-- `inc(post, c)` -/
def inc (R : Rec) (post : Bool) (c : Int32) : M Unit := do
  test_lvalue R
  vdup R
  if post then
    R.gv_dup
    vrotb R 3
    vrotb R 3
  vpushi R (c - TOK_MID)
  R.gen_op 43
  R.vstore
  if post then vpop R

/-- `parse_mult_str(msg)` -/
def parse_mult_str (R : Rec) (msg : String) : M ByteArray := do
  if (← getTok) != TOK_STR then expect R msg
  let mut s := ByteArray.empty
  let fuel ← fuelM
  for _ in [0:fuel] do
    if (← getTok) != TOK_STR then break
    let tc := (← get).tokc
    s := s ++ Bytes.cstrAt tc.sdata 0
    R.next
  s := s.push 0
  modify fun st => { st with initstr := s }
  return s

/-- Assertion-only C11 string consumer. Phase 6 checks encoding compatibility.
Render basic diagnostic characters from 1/2/4-byte code units; a nonbasic or
embedded zero unit is '?' so later basic text is still included. The ghost
scanner retains the original literal spellings independently of this buffer. -/
def parse_assert_str (R : Rec) : M ByteArray := do
  if !TOK_IS_STR (← getTok) then expect R "string constant"
  let mut s := ByteArray.empty
  let fuel ← fuelM
  for _ in [0:fuel] do
    let tk ← getTok
    if !TOK_IS_STR tk then break
    let tc := (← get).tokc
    let width := (TOK_STR_SIZE tk).toNat'
    for i in [0:tc.strSize.toNat' / width - 1] do
      let ch := if width == 1 then (tc.sdata.get! i).toUInt32
        else if width == 2 then (Bytes.r16 tc.sdata (2 * i)).toUInt32
        else Bytes.r32 tc.sdata (4 * i)
      s := s.push (if ch != 0 && ch < 128 then ch.toUInt8 else 63)
    R.next
  s := s.push 0
  modify fun st => { st with initstr := s }
  return s

/-- `exact_log2p1(i)` -/
def exact_log2p1 (i0 : Int32) : Int32 := Id.run do
  if i0 == 0 then return 0
  let mut i := i0
  let mut ret : Int32 := 1
  for _ in [0:4] do
    if i < 256 then break
    i := i >>> 8
    ret := ret + 8
  if i ≥ 16 then ret := ret + 4; i := i >>> 4
  if i ≥ 4 then ret := ret + 2; i := i >>> 2
  if i ≥ 2 then ret := ret + 1
  return ret

end Tcc.Model
