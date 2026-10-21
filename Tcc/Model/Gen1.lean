import Tcc.Model.Riscv
import Tcc.Model.PpMacro

/-!
# TccModel: tccgen.c, part 1 — symbols, the value stack, registers, `gv`

tccgen.c lines 160–2100: code suppression (gsym/gind/gjmp_acs), the ELF
symbol glue (elfsym, update_storage, put_extern_sym2, greloca), the Sym
stacks (sym_push, global_identifier_push, sym_pop, labels), the value
stack (vsetc, vpush*, vswap, vrot*, vpop, VT_CMP/VT_JMP), external
symbols and attribute merging, register allocation and spilling
(save_reg_upstack, get_reg, get_temp_local_var), bit-field access, and
`gv`/`gv2`/`gv_dup`. Calls to later tccgen functions go through `R`.
-/

namespace Tcc.Model
open C

/-! ## small helpers -/

@[inline] def NODATA_WANTED : M Bool := do return (← get).nocode_wanted > 0
@[inline] def NOEVAL_WANTED : M Bool := do return (← get).nocode_wanted &&& NOEVAL_MASK != 0
@[inline] def CONST_WANTED : M Bool := do return (← get).nocode_wanted &&& CONST_WANTED_MASK != 0
def CODE_OFF : M Unit := modify fun s => if s.nocode_wanted == 0 then { s with nocode_wanted := s.nocode_wanted ||| CODE_OFF_BIT } else s
def CODE_ON : M Unit := modify fun s => { s with nocode_wanted := s.nocode_wanted &&& ~~~CODE_OFF_BIT }

@[inline] def vtr (k : Int := 0) : M Int32 := do return (← vt k).r.toInt32'
@[inline] def setVtR (k : Int) (r : Int32) : M Unit := modVt k fun v => { v with r := r.toUInt16' }
@[inline] def vtt (k : Int := 0) : M Int32 := do return (← vt k).type.t
@[inline] def setVtT (k : Int) (t : Int32) : M Unit := modVt k fun v => { v with type := { v.type with t := t } }
@[inline] def setVtType (k : Int) (t : CType) : M Unit := modVt k fun v => { v with type := t }
@[inline] def vtci (k : Int := 0) : M UInt64 := do return (← vt k).c.lo
@[inline] def setVtCi (k : Int) (i : UInt64) : M Unit := modVt k fun v => { v with c := v.c.setI i }
@[inline] def vtopDec : M Unit := modify fun s => { s with vtop := s.vtop - 1 }
@[inline] def vtopInc : M Unit := modify fun s => { s with vtop := s.vtop + 1 }
@[inline] def symC (s : SymId) : M Int32 := do return (← getSym s).c

/-- `is_integer_btype(bt)` -/
@[inline] def is_integer_btype (bt : Int32) : Bool :=
  bt == VT_BYTE || bt == VT_BOOL || bt == VT_SHORT || bt == VT_INT || bt == VT_LLONG

/-- `btype_size(bt)` -/
def btype_size (bt : Int32) : Int32 :=
  if bt == VT_BYTE || bt == VT_BOOL then 1 else if bt == VT_SHORT then 2
  else if bt == VT_INT then 4 else if bt == VT_LLONG then 8 else if bt == VT_PTR then 8 else 0

/-- `R_RET(t)` -/
def R_RET (t : Int32) : Int32 :=
  if !is_float t then REG_IRET else if t &&& VT_BTYPE == VT_LDOUBLE then REG_IRET else REG_FRET
/-- `R2_RET(t)` -/
def R2_RET (t : Int32) : Int32 := if t &&& VT_BTYPE == VT_LDOUBLE then REG_IRE2 else VT_CONST
@[inline] def USING_TWO_WORDS (t : Int32) : Bool := R2_RET t != VT_CONST
/-- `RC_RET(t)` -/
def RC_RET (t : Int32) : Int32 := reg_classes[(R_RET t).toNat']! &&& ~~~(RC_FLOAT ||| RC_INT)
/-- `RC_TYPE(t)` -/
def RC_TYPE (t : Int32) : Int32 :=
  if !is_float t then RC_INT else if t &&& VT_BTYPE == VT_LDOUBLE then RC_INT else RC_FLOAT
/-- `RC2_TYPE(t, rc)` -/
def RC2_TYPE (t rc : Int32) : Int32 :=
  if !USING_TWO_WORDS t then 0 else if rc == RC_IRET then RC_IRE2
  else if rc &&& RC_FLOAT != 0 then RC_FLOAT else RC_INT

/-- `ieee_finite(d)` on the bits of a double -/
def ieee_finite (dbits : UInt64) : Bool :=
  let hi := (dbits >>> 32).toUInt32
  ((hi ||| 0x800fffff) + 1) >>> 31 != 0

/-- `test_lvalue()` -/
def test_lvalue (R : Rec) : M Unit := do
  if (← vtr) &&& VT_LVAL == 0 then expect R "lvalue"

/-- `check_vstack()` -/
def check_vstack (R : Rec) : M Unit := do
  let s ← get
  if s.vtop != 0 then
    tcc_error R (cfmt "internal compiler error: vstack leak (%d)" [.i (s.vtop : Int)])

/-! ## code suppression -/

/-- `gsym(t)` (tccgen.c) -/
def gsym (R : Rec) (t : Int32) : M Unit := do
  if t != 0 then
    gsym_addr R t (← get).ind
    CODE_ON

/-- `gind()` -/
def gind : M Int32 := do
  let t := (← get).ind
  CODE_ON
  return t

/-- `gjmp_addr_acs(t)` (tccgen.c's gjmp_addr) -/
def gjmp_addr_acs (t : Int32) : M Unit := do
  gjmp_addr t
  CODE_OFF

/-- `gjmp_acs(t)` (tccgen.c's gjmp) -/
def gjmp_acs (t : Int32) : M Int32 := do
  let t ← gjmp t
  CODE_OFF
  return t

/-! ## ELF symbol glue -/

/-- `update_storage(sym)` -/
def update_storage (sym : SymId) : M Unit := do
  let s ← getSym sym
  if sym == 0 || s.c == 0 then return
  let st := (← get).symtab_section
  let i := s.c.toNat'
  let mut e ← getElfSym st i
  if s.a_visibility != 0 then
    e := { e with st_other := ((e.st_other.toInt32 &&& ~~~3) ||| s.a_visibility.toInt32').toUInt8 }
  let sym_bind := if s.type.t &&& (VT_STATIC ||| VT_INLINE) != 0 then STB_LOCAL
                  else if s.a_weak != 0 then STB_WEAK else STB_GLOBAL
  let old_sym_bind := ELF_ST_BIND e.st_info.toInt32
  if sym_bind != old_sym_bind then
    e := { e with st_info := (ELF_ST_INFO sym_bind (ELF_ST_TYPE e.st_info.toInt32)).toUInt8 }
  setElfSym st i e

/-- `put_extern_sym2(sym, sh_num, value, size, can_add_underscore)` -/
def put_extern_sym2 (sym : SymId) (sh_num : Int32) (value size : UInt64) (_can_add_underscore : Bool) : M Unit := do
  let s ← getSym sym
  if s.c == 0 then
    let mut name ← get_tok_str s.v none
    let t := s.type.t
    let sym_type :=
      if t &&& VT_BTYPE == VT_FUNC then STT_FUNC
      else if t &&& VT_BTYPE == VT_VOID then (if IS_ASM_FUNC t then STT_FUNC else STT_NOTYPE)
      else if t &&& VT_TLS != 0 then STT_TLS
      else STT_OBJECT
    let sym_bind := if t &&& (VT_STATIC ||| VT_INLINE) != 0 then STB_LOCAL else STB_GLOBAL
    if s.asm_label != 0 then
      name ← get_tok_str s.asm_label none
    let info := ELF_ST_INFO sym_bind sym_type
    let c ← put_elf_sym (← get).symtab_section value size info 0 sh_num (some name)
    modSym sym fun x => { x with c := Int32.ofNat c }
  else
    let st := (← get).symtab_section
    let e ← getElfSym st s.c.toNat'
    setElfSym st s.c.toNat' { e with st_value := value, st_size := size, st_shndx := sh_num.toUInt32.toUInt16 }
  update_storage sym

/-- `put_extern_sym(sym, s, value, size)` -/
def put_extern_sym (sym : SymId) (s : SecId) (value size : UInt64) : M Unit := do
  let st ← get
  if st.nocode_wanted != 0 && (st.nocode_wanted > 0 || (s != 0 && s == st.cur_text_section)) then return
  let shn ← if s != 0 then pure (← getSec s).sh_num else pure SHN_UNDEF
  put_extern_sym2 sym shn value size true

/-- `greloca(s, sym, offset, type, addend)` -/
def greloca (s : SecId) (sym : SymId) (offset : UInt64) (ty : Int32) (addend : Int64) : M Unit := do
  let st ← get
  if st.nocode_wanted != 0 && s == st.cur_text_section then return
  let mut c : Int32 := 0
  if sym != 0 then
    if (← getSym sym).c == 0 then
      put_extern_sym sym 0 0 0
      let x ← getSym sym
      if x.sym_scope != 0 && (x.type.t &&& (VT_STATIC ||| VT_EXTERN)) == (VT_STATIC ||| VT_EXTERN) then
        let mut s2 := sym
        let fuel ← fuelM
        for _ in [0:fuel] do
          let pt := (← getSym s2).prev_tok
          if pt == 0 then break
          s2 := pt
        let xc := x.c
        modSym s2 fun y => { y with c := xc }
    c := (← getSym sym).c
  put_elf_reloca (← get).symtab_section s offset ty c.toNat' addend

/-! ## the Sym stacks -/

/-- `sym_link(s, yes)` -/
def sym_link (s : SymId) (yes : Bool) : M Unit := do
  let x ← getSym s
  let v := (x.v &&& ~~~SYM_STRUCT)
  let ts ← tokSym v
  let isStruct := x.v &&& SYM_STRUCT != 0
  if yes then
    let top := if isStruct then ts.sym_struct else ts.sym_identifier
    let ls := (← get).local_scope
    modSym s fun y => { y with prev_tok := top, aux := ls.toUInt32 }
    modTokSym v fun t => if isStruct then { t with sym_struct := s } else { t with sym_identifier := s }
  else
    let pt := x.prev_tok
    modTokSym v fun t => if isStruct then { t with sym_struct := pt } else { t with sym_identifier := pt }

/-- `sym_scope_ex(s)` -/
def sym_scope_ex (s : SymId) : M Int32 := do
  let x ← getSym s
  if IS_ENUM_VAL x.type.t then return (← getSym x.type.ref).sym_scope
  return x.sym_scope

/-- `sym_push(v, type, r, c)` -/
def sym_push (R : Rec) (v : Int32) (type : CType) (r c : Int32) : M SymId := do
  let st ← get
  let useLocal := st.local_stack != 0
  let top := if useLocal then st.local_stack else st.global_stack
  let s ← sym_push2 top v type.t c
  if useLocal then modify fun st => { st with local_stack := s } else modify fun st => { st with global_stack := s }
  modSym s fun x => { x with type := { x.type with ref := type.ref }, r := r.toUInt16' }
  if (v &&& ~~~SYM_STRUCT) < SYM_FIRST_ANOM then
    sym_link s true
    let pt := (← getSym s).prev_tok
    if pt != 0 && (← sym_scope_ex pt) == (← get).local_scope then
      tcc_error R (cfmt "redeclaration of '%s'" [.s (← get_tok_str v none)])
  return s

/-- `global_identifier_push(v, t, c)` -/
def global_identifier_push (v t c : Int32) : M SymId := do
  let s ← sym_push2 (← get).global_stack v t c
  modify fun st => { st with global_stack := s }
  modSym s fun x => { x with r := (VT_CONST ||| VT_SYM).toUInt16' }
  if v < SYM_FIRST_ANOM then
    -- walk past local identifiers: ps = &table_ident[v]->sym_identifier, then &(*ps)->prev_tok
    let ts ← tokSym v
    let mut owner : Option SymId := none     -- none: the TokenSym's field
    let mut cur := ts.sym_identifier
    let fuel ← fuelM
    for _ in [0:fuel] do
      if cur == 0 then break
      let x ← getSym cur
      if x.sym_scope == 0 then break
      owner := some cur
      cur := x.prev_tok
    modSym s fun x => { x with prev_tok := cur }
    match owner with
    | none => modTokSym v fun t => { t with sym_identifier := s }
    | some o => modSym o fun x => { x with prev_tok := s }
  return s

/-- `sym_pop(ptop, b, keep)`: returns the new top -/
def sym_pop (top : SymId) (b : SymId) (keep : Bool) : M SymId := do
  let fuel ← fuelM
  let mut s := top
  for _ in [0:fuel] do
    if s == b then break
    let x ← getSym s
    let ss := x.prev
    if (x.v &&& ~~~SYM_STRUCT) < SYM_FIRST_ANOM then sym_link s false
    if !keep then sym_free s
    s := ss
  return if keep then top else b

/-- `label_push(ptop, v, flags)` (`isGlobal`: ptop == &global_label_stack) -/
def label_push (isGlobal : Bool) (v : Int32) (flags : Int32) : M SymId := do
  let st ← get
  let top := if isGlobal then st.global_label_stack else st.local_label_stack
  let s ← sym_push2 top v VT_STATIC 0
  if isGlobal then modify fun st => { st with global_label_stack := s }
  else modify fun st => { st with local_label_stack := s }
  modSym s fun x => { x with r := flags.toUInt16' }
  let ts ← tokSym v
  let mut owner : Option SymId := none
  let mut cur := ts.sym_label
  if isGlobal then
    let fuel ← fuelM
    for _ in [0:fuel] do
      if cur == 0 then break
      owner := some cur
      cur := (← getSym cur).prev_tok
  modSym s fun x => { x with prev_tok := cur }
  match owner with
  | none => modTokSym v fun t => { t with sym_label := s }
  | some o => modSym o fun x => { x with prev_tok := s }
  return s

/-- `label_pop(ptop, slast, keep)`: returns the new top -/
def label_pop (R : Rec) (top : SymId) (slast : SymId) (keep : Bool) : M SymId := do
  let fuel ← fuelM
  let mut s := top
  for _ in [0:fuel] do
    if s == slast then break
    let x ← getSym s
    let s1 := x.prev
    if x.r == LABEL_DECLARED then
      tcc_warning_c R WARNC_all (cfmt "label '%s' declared but not used" [.s (← get_tok_str x.v none)])
    else if x.r == LABEL_FORWARD then
      tcc_error R (cfmt "label '%s' used but not defined" [.s (← get_tok_str x.v none)])
    else
      if x.c != 0 then
        put_extern_sym s (← get).cur_text_section x.jind.toUInt64 1
    if x.r != LABEL_GONE then
      let pt := x.prev_tok
      modTokSym x.v fun t => { t with sym_label := pt }
    if !keep then sym_free s
    else modSym s fun y => { y with r := LABEL_GONE }
    s := s1
  return if keep then top else slast

/-! ## the value stack -/

/-- `vcheck_cmp()` -/
def vcheck_cmp (R : Rec) : M Unit := do
  if (← vtr) == VT_CMP && ((← get).nocode_wanted &&& ~~~CODE_OFF_BIT) == 0 then
    let _ ← R.gv RC_INT

/-- `vsetc(type, r, vc)` -/
def vsetc (R : Rec) (type : CType) (r : Int32) (vc : CValue) : M Unit := do
  if (← get).vtop ≥ VSTACK_SIZE then tcc_error_s R "memory full (vstack)"
  vcheck_cmp R
  vtopInc
  setVt 0 { type := type, r := r.toUInt16', r2 := VT_CONST.toUInt16', c := vc, u := 0 }

/-- `vswap()` -/
def vswap (R : Rec) : M Unit := do
  vcheck_cmp R
  let a ← vt 0
  let b ← vt (-1)
  setVt 0 b
  setVt (-1) a

/-- `vpop()` -/
def vpop (R : Rec) : M Unit := do
  let v ← vt
  if v.r.toInt32' &&& VT_VALMASK == VT_CMP then
    R.gsym v.jtrue
    R.gsym v.jfalse
  vtopDec

/-- `vpush64(ty, v)` -/
def vpush64 (R : Rec) (ty : Int32) (v : UInt64) : M Unit :=
  vsetc R { t := ty } VT_CONST (CValue.ofI v)

/-- `vpush(type)` -/
def vpush (R : Rec) (type : CType) : M Unit := vsetc R type VT_CONST (CValue.ofI 0)

/-- `vpushi(v)` -/
def vpushi (R : Rec) (v : Int32) : M Unit := vpush64 R VT_INT v.toInt64.toUInt64

/-- `vpushs(v)` -/
def vpushs (R : Rec) (v : UInt64) : M Unit := vpush64 R VT_SIZE_T v

/-- `vpushll(v)` -/
def vpushll (R : Rec) (v : Int64) : M Unit := vpush64 R VT_LLONG v.toUInt64

/-- `vset(type, r, v)` -/
def vset (R : Rec) (type : CType) (r v : Int32) : M Unit :=
  vsetc R type r (CValue.ofI v.toInt64.toUInt64)

/-- `vseti(r, v)` -/
def vseti (R : Rec) (r v : Int32) : M Unit := vset R { t := VT_INT } r v

/-- `vpushv(v)` -/
def vpushv (R : Rec) (v : SValue) : M Unit := do
  if (← get).vtop ≥ VSTACK_SIZE then tcc_error_s R "memory full (vstack)"
  vtopInc
  setVt 0 v

/-- `vdup()` -/
def vdup (R : Rec) : M Unit := do vpushv R (← vt)

/-- `vrotb(n)` -/
def vrotb (R : Rec) (n0 : Int32) : M Unit := do
  let n := n0 - 1
  if n < 1 then return
  vcheck_cmp R
  let tmp ← vt (-(n.toInt))
  for i in [0:n.toNat'] do
    -- vtop[-n + i] = vtop[-n + i + 1]
    let k : Int := -(n.toInt) + i
    setVt k (← vt (k + 1))
  setVt 0 tmp

/-- `vrott(n)` -/
def vrott (R : Rec) (n0 : Int32) : M Unit := do
  let n := n0 - 1
  if n < 1 then return
  vcheck_cmp R
  let tmp ← vt 0
  -- memmove(vtop - n + 1, vtop - n, n): from the top down
  for i in [0:n.toNat'] do
    let k : Int := -(i : Int)
    setVt k (← vt (k - 1))
  setVt (-(n.toInt)) tmp

/-- `vrev(n)` -/
def vrev (R : Rec) (n0 : Int32) : M Unit := do
  vcheck_cmp R
  let mut i : Int := 0
  let mut n : Int := -(n0.toInt)
  let fuel ← fuelM
  for _ in [0:fuel] do
    n := n + 1
    if !(i > n) then break
    let a ← vt i
    let b ← vt n
    setVt i b
    setVt n a
    i := i - 1

/-- `vset_VT_CMP(op)` -/
def vset_VT_CMP (op : Int32) : M Unit :=
  modVt 0 fun v =>
    let v := { v with r := VT_CMP.toUInt16' }
    let v := v.setCmp op v.cmp_r
    { v with c := (v.c.setJfalse 0).setJtrue 0 }

/-- `vset_VT_JMP()` -/
def vset_VT_JMP (R : Rec) : M Unit := do
  let v ← vt
  let op := v.cmp_op
  if v.jtrue != 0 || v.jfalse != 0 then
    let origt := v.type.t
    let inv := op &&& (if op < 2 then 1 else 0)
    let t ← R.gvtst inv 0
    vseti R (VT_JMP + inv) t
    modVt 0 fun v => { v with type := { v.type with t := v.type.t ||| (origt &&& (VT_UNSIGNED ||| VT_DEFSIGN)) } }
  else
    setVtCi 0 op.toInt64.toUInt64
    if op < 2 then setVtR 0 VT_CONST

/-- `gvtst_set(inv, t)` -/
def gvtst_set (R : Rec) (inv t : Int32) : M Unit := do
  if (← vtr) != VT_CMP then
    vpushi R 0
    R.gen_op TOK_NE
    if (← vtr) != VT_CMP then
      vset_VT_CMP (b2i ((← vtci) != 0))
  let v ← vt
  if inv != 0 then
    let j ← gjmp_append v.jfalse t
    modVt 0 fun v => { v with c := v.c.setJfalse j }
  else
    let j ← gjmp_append v.jtrue t
    modVt 0 fun v => { v with c := v.c.setJtrue j }

/-- `gvtst(inv, t)` -/
def gvtst (R : Rec) (inv t0 : Int32) : M Int32 := do
  gvtst_set R inv t0
  let v ← vt
  let mut t := v.jtrue
  let mut u := v.jfalse
  if inv != 0 then
    let x := u; u := t; t := x
  let op := v.cmp_op
  if op > 1 then t ← gjmp_cond (op ^^^ inv) t
  else if op != inv then t ← gjmp_acs t
  gsym R u
  vtopDec
  return t

/-- `gen_test_zero(op)` -/
def gen_test_zero (R : Rec) (op : Int32) : M Unit := do
  if (← vtr) == VT_CMP then
    if op == TOK_EQ then
      modVt 0 fun v =>
        let j := v.jfalse
        let v := { v with c := (v.c.setJfalse v.jtrue).setJtrue j }
        v.setCmp (v.cmp_op ^^^ 1) v.cmp_r
  else
    vpushi R 0
    R.gen_op op

/-- `vpushsym(type, sym)` -/
def vpushsym (R : Rec) (type : CType) (sym : SymId) : M Unit := do
  vsetc R type (VT_CONST ||| VT_SYM) (CValue.ofI 0)
  modVt 0 fun v => v.setSym sym

/-- `get_sym_ref(type, sec, offset, size)` -/
def get_sym_ref (R : Rec) (type : CType) (sec : SecId) (offset size : UInt64) : M SymId := do
  let v := (← get).anon_sym
  modify fun s => { s with anon_sym := s.anon_sym + 1 }
  let sym ← sym_push R v type (VT_CONST ||| VT_SYM) 0
  modSym sym fun x => { x with type := { x.type with t := x.type.t ||| VT_STATIC } }
  put_extern_sym sym sec offset size
  return sym

/-- `vpush_ref(type, sec, offset, size)` -/
def vpush_ref (R : Rec) (type : CType) (sec : SecId) (offset size : UInt64) : M Unit := do
  vpushsym R type (← get_sym_ref R type sec offset size)

/-- `IS_ASM_SYM(sym)` -/
def IS_ASM_SYM (x : Sym) : Bool :=
  (x.type.t &&& ((VT_BTYPE ||| VT_STRUCT_MASK) &&& ~~~((1 : Int32) <<< VT_STRUCT_SHIFT))) == VT_ASM

/-- `external_global_sym(v, type)` -/
def external_global_sym (v : Int32) (type : CType) : M SymId := do
  let mut s ← sym_find v
  if s == 0 then
    s ← global_identifier_push v (type.t ||| VT_EXTERN) 0
    let s' := s
    modSym s' fun x => { x with type := { x.type with ref := type.ref } }
  else
    let x ← getSym s
    if IS_ASM_SYM x then
      let s' := s
      modSym s' fun x => { x with type := { t := type.t ||| (x.type.t &&& VT_EXTERN), ref := type.ref } }
      update_storage s
  return s

/-- `external_helper_sym(v)` -/
def external_helper_sym (v : Int32) : M SymId := external_global_sym v { t := VT_ASM_FUNC }

/-- `vpush_helper_func(v)` -/
def vpush_helper_func (R : Rec) (v : Int32) : M Unit := do
  vpushsym R (← get).func_old_type (← external_helper_sym v)

/-! ## attributes -/

/-- `merge_symattr(sa, sa1)` -/
def merge_symattr (sa sa1 : UInt16) : UInt16 := Id.run do
  let get_ (x : UInt16) (pos w : UInt16) : UInt16 := (x >>> pos) &&& ((1 <<< w) - 1)
  let mut sa := sa
  if get_ sa1 0 5 != 0 && get_ sa 0 5 == 0 then sa := setBits16 sa 0 5 (get_ sa1 0 5)
  sa := sa ||| (sa1 &&& (0x2060 : UInt16))   -- packed, weak, nodebug
  if get_ sa1 7 2 != 0 then
    let mut vis := get_ sa 7 2
    if vis == 0 || vis > get_ sa1 7 2 then vis := get_ sa1 7 2
    sa := setBits16 sa 7 2 vis
  sa := sa ||| (sa1 &&& (0x0e00 : UInt16))  -- dllexport, nodecorate, dllimport
  return sa

/-- `merge_funcattr(fa, fa1)` -/
def merge_funcattr (fa fa1 : UInt32) : UInt32 := Id.run do
  let get_ (x : UInt32) (pos w : UInt32) : UInt32 := (x >>> pos) &&& ((1 <<< w) - 1)
  let mut fa := fa
  if get_ fa1 0 3 != 0 && get_ fa 0 3 == 0 then fa := setBits32 fa 0 3 (get_ fa1 0 3)
  if get_ fa1 3 2 != 0 && get_ fa 3 2 == 0 then fa := setBits32 fa 3 2 (get_ fa1 3 2)
  if get_ fa1 8 8 != 0 && get_ fa 8 8 == 0 then fa := setBits32 fa 8 8 (get_ fa1 8 8)
  if get_ fa1 5 1 != 0 then fa := setBits32 fa 5 1 1
  if get_ fa1 6 1 != 0 then fa := setBits32 fa 6 1 1
  if get_ fa1 7 1 != 0 then fa := setBits32 fa 7 1 1
  return fa

/-- `merge_attr(ad, ad1)` -/
def merge_attr (ad ad1 : AttributeDef) : AttributeDef :=
  let ad := { ad with a := merge_symattr ad.a ad1.a, f := merge_funcattr ad.f ad1.f }
  let ad := if ad1.sec != 0 then { ad with sec := ad1.sec } else ad
  let ad := if ad1.alias_target != 0 then { ad with alias_target := ad1.alias_target } else ad
  let ad := if ad1.asm_label != 0 then { ad with asm_label := ad1.asm_label } else ad
  if ad1.attr_mode != 0 then { ad with attr_mode := ad1.attr_mode } else ad

/-- `patch_type(sym, type)`; `type` may be updated through its ref -/
def patch_type (R : Rec) (sym : SymId) (type : CType) : M Unit := do
  let x ← getSym sym
  if type.t &&& VT_EXTERN == 0 || IS_ENUM_VAL x.type.t then
    if x.type.t &&& VT_EXTERN == 0 then
      tcc_error R (cfmt "redefinition of '%s'" [.s (← get_tok_str x.v none)])
    modSym sym fun y => { y with type := { y.type with t := y.type.t &&& ~~~VT_EXTERN } }
  let x ← getSym sym
  if IS_ASM_SYM x then
    modSym sym fun y =>
      let y := { y with type := { t := type.t &&& (y.type.t ||| ~~~VT_STATIC), ref := type.ref } }
      if type.t &&& VT_BTYPE != VT_FUNC && type.t &&& VT_ARRAY == 0 then { y with r := y.r ||| VT_LVAL.toUInt16' } else y
  let x ← getSym sym
  if !(← R.is_compatible_types x.type type) then
    tcc_error R (cfmt "incompatible types for redefinition of '%s'" [.s (← get_tok_str x.v none)])
  else if x.type.t &&& VT_BTYPE == VT_FUNC then
    let mut static_proto := x.type.t &&& VT_STATIC
    let xr ← getSym x.type.ref
    let ft1 := xr.f_func_type
    let ft2 := (← getSym type.ref).f_func_type
    if type.t &&& VT_STATIC != 0 && static_proto == 0 && ((type.t ||| x.type.t) &&& VT_INLINE) == 0 then
      tcc_warning R (cfmt "static storage ignored for redefinition of '%s'" [.s (← get_tok_str x.v none)])
    if (type.t ||| x.type.t) &&& VT_INLINE != 0 then
      if ((type.t ^^^ x.type.t) &&& VT_INLINE) == 0 || ((type.t ||| x.type.t) &&& VT_STATIC) != 0 then
        static_proto := static_proto ||| VT_INLINE
    if type.t &&& VT_EXTERN == 0 then
      let f := xr.aux
      let sp := static_proto
      modSym sym fun y => { y with type := { y.type with t := (type.t &&& ~~~(VT_STATIC ||| VT_INLINE)) ||| sp } }
      if ft1 != FUNC_OLD then
        modSym type.ref fun y => { y with aux := setBits32 y.aux 3 2 ft1 }
      modSym sym fun y => { y with type := { y.type with ref := type.ref } }
      modSym type.ref fun y => { y with aux := merge_funcattr y.aux f }
    else
      let sp := static_proto
      modSym sym fun y => { y with type := { y.type with t := y.type.t &&& (~~~VT_INLINE ||| sp) } }
      if ft1 == FUNC_OLD && ft2 != FUNC_OLD then
        modSym sym fun y => { y with type := { y.type with ref := type.ref } }
  else
    if x.type.t &&& VT_ARRAY != 0 && (← symC type.ref) ≥ 0 then
      let c ← symC type.ref
      modSym x.type.ref fun y => { y with c := c }
    if (type.t ^^^ x.type.t) &&& VT_STATIC != 0 then
      tcc_warning R (cfmt "storage mismatch for redefinition of '%s'" [.s (← get_tok_str x.v none)])

/-- `patch_storage(sym, ad, type)` -/
def patch_storage (R : Rec) (sym : SymId) (ad : AttributeDef) (type : Option CType) : M Unit := do
  match type with
  | some t => patch_type R sym t
  | none => pure ()
  modSym sym fun y => { y with a := merge_symattr y.a ad.a }
  if ad.asm_label != 0 then
    let l := ad.asm_label
    modSym sym fun y => { y with next := l.toNat' }
  update_storage sym

/-- `sym_copy(s0, ps)`: returns (new sym, new top) -/
def sym_copy (s0 : SymId) (top : SymId) : M SymId := do
  let s ← sym_malloc
  let x ← getSym s0
  modSym s fun _ => { x with prev := top }
  if (x.v &&& ~~~SYM_STRUCT) < SYM_FIRST_ANOM then sym_link s true
  return s

/-- `move_ref_to_global(s)` (recursive on the type chain; fuel-bounded) -/
def move_ref_to_global : Nat → SymId → M Unit
  | 0, _ => throw (.fuel "move_ref_to_global")
  | fuel+1, s0 => do
    let bt := (← getSym s0).type.t &&& VT_BTYPE
    let t0 := (← getSym s0).type.t
    if !(bt == VT_PTR || bt == VT_FUNC || bt == VT_STRUCT || IS_ENUM t0) then return
    let mut s := (← getSym s0).type.ref
    let mut n := 0
    let lfuel ← fuelM
    for _ in [0:lfuel] do
      if s == 0 then break
      -- for (lp = &local_stack; l = *lp; lp = &l->prev)
      let mut owner : Option SymId := none   -- none: local_stack itself
      let mut l := (← get).local_stack
      let mut found := false
      for _ in [0:lfuel] do
        if l == 0 then break
        if l == s then
          found := true
          break
        owner := some l
        l := (← getSym l).prev
      if found then
        let sp := (← getSym s).prev
        match owner with
        | none => modify fun st => { st with local_stack := sp }
        | some o => modSym o fun y => { y with prev := sp }
        let gs := (← get).global_stack
        modSym s fun y => { y with prev := gs }
        modify fun st => { st with global_stack := s }
        if n != 0 || bt == VT_PTR || bt == VT_FUNC then
          move_ref_to_global fuel s
        else
          let sv := (← getSym s).v
          if (sv &&& ~~~SYM_STRUCT) < SYM_FIRST_ANOM then
            modSym s fun y => { y with v := y.v ||| SYM_FIELD }
            -- l = sym_copy(s, lp): inserted where s was
            let top ← match owner with | none => pure (← get).local_stack | some o => pure (← getSym o).prev
            let l2 ← sym_copy s top
            match owner with
            | none => modify fun st => { st with local_stack := l2 }
            | some o => modSym o fun y => { y with prev := l2 }
            modSym l2 fun y => { y with v := y.v &&& ~~~SYM_FIELD }
        if bt != VT_PTR then n := 1
      if n == 0 then break
      s := (← getSym s).next

/-- `external_sym(v, type, r, ad)` -/
def external_sym (R : Rec) (v : Int32) (type : CType) (r : Int32) (ad : AttributeDef) : M SymId := do
  let mut s ← sym_find v
  let fuel ← fuelM
  for _ in [0:fuel] do
    if s == 0 then break
    if (← getSym s).sym_scope == 0 then break
    s := (← getSym s).prev_tok
  if s == 0 then
    s ← global_identifier_push v type.t 0
    let s' := s
    modSym s' fun x => { x with r := x.r ||| r.toUInt16', a := ad.a, next := ad.asm_label.toNat',
                                 type := { x.type with ref := type.ref } }
  else
    patch_storage R s ad (some type)
  if (← get).local_stack != 0 then
    move_ref_to_global fuel s
    let c ← sym_copy s (← get).local_stack
    modify fun st => { st with local_stack := c }
  return s

/-! ## registers -/

/-- `get_temp_local_var(size, align, &r2)`: (offset, r2) -/
def get_temp_local_var (size align : Int32) : M (Int32 × Int32) := do
  let s ← get
  let mut used : UInt32 := 0
  for p in [1:s.vtop + 1] do
    let sv := s.vstack[p]!
    let r := sv.r.toInt32' &&& VT_VALMASK
    if r == VT_LOCAL || r == VT_LLOCAL then
      let r := sv.r2.toInt32' - (VT_CONST + 1)
      if r ≥ 0 && r < 8 then used := used ||| ((1 : UInt32) <<< r.toUInt32)
  for i in [0:s.nb_temp_local_vars.toNat'] do
    let tv := s.arr_temp_local_vars[i]!
    if used &&& ((1 : UInt32) <<< i.toUInt32) == 0 && tv.size ≥ size && tv.align ≥ align then
      return (tv.location, (VT_CONST + 1) + Int32.ofNat i)
  let loc := (s.loc - size) &&& (-align)
  modify fun s => { s with loc := loc }
  let n := s.nb_temp_local_vars
  if n.toNat' < 8 then
    modify fun s => { s with arr_temp_local_vars := s.arr_temp_local_vars.set! n.toNat' { location := loc, size := size, align := align },
                             nb_temp_local_vars := n + 1 }
    return (loc, (VT_CONST + 1) + n)
  return (loc, VT_CONST)

/-- `save_reg_upstack(r, n)` -/
def save_reg_upstack (R : Rec) (r0 : Int32) (n : Int32) : M Unit := do
  let r := r0 &&& VT_VALMASK
  if r ≥ VT_CONST then return
  if (← get).nocode_wanted != 0 then return
  let mut l : Int32 := 0
  let mut r2 : Int32 := 0
  let top : Int := ((← get).vtop : Int) - n.toInt
  let mut p : Nat := 1
  let fuel ← fuelM
  for _ in [0:fuel] do
    if (p : Int) > top then break
    let sv := (← get).vstack[p]!
    if (sv.r.toInt32' &&& VT_VALMASK) == r || sv.r2.toInt32' == r then
      let mut skip := false
      if l == 0 then
        let mut bt := sv.type.t &&& VT_BTYPE
        if bt == VT_VOID then skip := true
        else
          if sv.r.toInt32' &&& VT_LVAL != 0 || bt == VT_FUNC then bt := VT_PTR
          let (size, align) ← R.type_size { t := bt }
          let (l', r2') ← get_temp_local_var size align
          l := l'; r2 := r2'
          let mut svs : SValue := { type := { t := bt }, r := (VT_LOCAL ||| VT_LVAL).toUInt16', c := CValue.ofI l.toInt64.toUInt64 }
          store R (sv.r.toInt32' &&& VT_VALMASK) svs
          if sv.r2.toInt32' < VT_CONST && USING_TWO_WORDS bt then
            svs := { svs with c := svs.c.setI (svs.c.i + 8) }
            store R sv.r2.toInt32' svs
      if !skip then
        let sv := (← get).vstack[p]!
        let sv :=
          if sv.r.toInt32' &&& VT_LVAL != 0 then
            { sv with r := ((sv.r.toInt32' &&& ~~~(VT_VALMASK ||| VT_BOUNDED)) ||| VT_LLOCAL).toUInt16' }
          else
            { sv with r := (VT_LVAL ||| VT_LOCAL).toUInt16', type := { sv.type with t := sv.type.t &&& ~~~VT_ARRAY } }
        let sv := (sv.setSym 0)
        let sv := { sv with r2 := r2.toUInt16', c := sv.c.setI l.toInt64.toUInt64 }
        modify fun s => { s with vstack := s.vstack.set! p sv }
    p := p + 1

/-- `save_reg(r)` -/
def save_reg (R : Rec) (r : Int32) : M Unit := save_reg_upstack R r 0

/-- `save_regs(n)` -/
def save_regs (R : Rec) (n : Int32) : M Unit := do
  let top : Int := ((← get).vtop : Int) - n.toInt
  let mut p : Nat := 1
  let fuel ← fuelM
  for _ in [0:fuel] do
    if (p : Int) > top then break
    save_reg R (← get).vstack[p]!.r.toInt32'
    p := p + 1

/-- `get_reg(rc)` -/
def get_reg (R : Rec) (rc : Int32) : M Int32 := do
  for r in [0:NB_REGS] do
    if reg_classes[r]! &&& rc != 0 then
      if (← get).nocode_wanted != 0 then return Int32.ofNat r
      let s ← get
      let mut used := false
      for p in [1:s.vtop + 1] do
        let sv := s.vstack[p]!
        if (sv.r.toInt32' &&& VT_VALMASK) == Int32.ofNat r || sv.r2.toInt32' == Int32.ofNat r then
          used := true; break
      if !used then return Int32.ofNat r
  -- no register left: free the first one on the stack
  let s ← get
  for p in [1:s.vtop + 1] do
    let sv := s.vstack[p]!
    let r := sv.r2.toInt32'
    if r < VT_CONST && reg_classes[r.toNat']! &&& rc != 0 then
      save_reg R r
      return r
    let r := sv.r.toInt32' &&& VT_VALMASK
    if r < VT_CONST && reg_classes[r.toNat']! &&& rc != 0 then
      save_reg R r
      return r
  return -1

/-- `move_reg(r, s, t)` -/
def move_reg (R : Rec) (r s t : Int32) : M Unit := do
  if r != s then
    save_reg R r
    load R r { type := { t := t }, r := s.toUInt16', c := CValue.ofI 0 }

/-- `gaddrof()` -/
def gaddrof : M Unit := do
  modVt 0 fun v =>
    let r := v.r.toInt32' &&& ~~~VT_LVAL
    let r := if r &&& VT_VALMASK == VT_LLOCAL then (r &&& ~~~VT_VALMASK) ||| VT_LOCAL ||| VT_LVAL else r
    { v with r := r.toUInt16' }

/-- `incr_offset(offset)` -/
def incr_offset (R : Rec) (offset : Int32) : M Unit := do
  let t ← vtt
  gaddrof
  setVtT 0 VT_PTRDIFF_T
  vpushs R offset.toInt64.toUInt64
  R.gen_op 43
  modVt 0 fun v => { v with r := v.r ||| VT_LVAL.toUInt16', type := { v.type with t := t } }

/-- `incr_bf_adr(o)` -/
def incr_bf_adr (R : Rec) (o : Int32) : M Unit := do
  setVtT 0 (VT_BYTE ||| VT_UNSIGNED)
  incr_offset R o

/-- `load_packed_bf(type, bit_pos, bit_size)` -/
def load_packed_bf (R : Rec) (type : CType) (bit_pos0 bit_size0 : Int32) : M Unit := do
  save_reg_upstack R (← vtr) 1
  vpush64 R (type.t &&& VT_BTYPE) 0
  let mut bits : Int32 := 0
  let mut o := bit_pos0 >>> 3
  let mut bit_pos := bit_pos0 &&& 7
  let mut bit_size := bit_size0
  let fuel ← fuelM
  for _ in [0:fuel] do
    vswap R
    incr_bf_adr R o
    vdup R
    let mut n := 8 - bit_pos
    if n > bit_size then n := bit_size
    if bit_pos != 0 then
      vpushi R bit_pos; R.gen_op TOK_SHR; bit_pos := 0
    if n < 8 then
      vpushi R (((1 : Int32) <<< n) - 1); R.gen_op 38
    R.gen_cast type
    if bits != 0 then
      vpushi R bits; R.gen_op TOK_SHL
    vrotb R 3
    R.gen_op 124
    bits := bits + n; bit_size := bit_size - n; o := 1
    if bit_size == 0 then break
  vswap R; vpop R
  if type.t &&& VT_UNSIGNED == 0 then
    let n := (if type.t &&& VT_BTYPE == VT_LLONG then 64 else 32) - bits
    vpushi R n; R.gen_op TOK_SHL
    vpushi R n; R.gen_op TOK_SAR

/-- `store_packed_bf(bit_pos, bit_size)` -/
def store_packed_bf (R : Rec) (bit_pos0 bit_size0 : Int32) : M Unit := do
  let c := ((← vtr) &&& (VT_VALMASK ||| VT_LVAL ||| VT_SYM)) == VT_CONST
  vswap R
  save_reg_upstack R (← vtr) 1
  let mut bits : Int32 := 0
  let mut o := bit_pos0 >>> 3
  let mut bit_pos := bit_pos0 &&& 7
  let mut bit_size := bit_size0
  let fuel ← fuelM
  for _ in [0:fuel] do
    incr_bf_adr R o
    vswap R
    if c then vdup R else R.gv_dup
    vrott R 3
    if bits != 0 then
      vpushi R bits; R.gen_op TOK_SHR
    if bit_pos != 0 then
      vpushi R bit_pos; R.gen_op TOK_SHL
    let mut n := 8 - bit_pos
    if n > bit_size then n := bit_size
    if n < 8 then
      let m := (((1 : Int32) <<< n) - 1) <<< bit_pos
      vpushi R m; R.gen_op 38
      vpushv R (← vt (-1))
      vpushi R (if m &&& 0x80 != 0 then ~~~m &&& 0x7f else ~~~m)
      R.gen_op 38
      R.gen_op 124
    vdup R
    setVt (-1) (← vt (-2))
    R.vstore; vpop R
    bits := bits + n; bit_size := bit_size - n; bit_pos := 0; o := 1
    if bit_size == 0 then break
  vpop R; vpop R

/-- `adjust_bf(sv, bit_pos, bit_size)` on vtop[k] -/
def adjust_bf (k : Int) : M Int32 := do
  let sv ← vt k
  if sv.type.ref == 0 then return 0
  let t := (← getSym sv.type.ref).auxtype
  if t != -1 && t != VT_STRUCT then
    modVt k fun v => { v with type := { v.type with t := (v.type.t &&& ~~~(VT_BTYPE ||| VT_LONG)) ||| t },
                               r := v.r ||| VT_LVAL.toUInt16' }
  return t

/-- `gv(rc)` -/
def gv (R : Rec) (rc0 : Int32) : M Int32 := do
  let mut rc := rc0
  let t0 ← vtt
  if t0 &&& VT_BITFIELD != 0 then
    let bit_pos := BIT_POS t0
    let bit_size := BIT_SIZE t0
    setVtT 0 (t0 &&& ~~~VT_STRUCT_MASK)
    let t1 ← vtt
    let mut type : CType := { t := t1 &&& VT_UNSIGNED }
    if t1 &&& VT_BTYPE == VT_BOOL then type := { type with t := type.t ||| VT_UNSIGNED }
    let r ← adjust_bf 0
    if (← vtt) &&& VT_BTYPE == VT_LLONG then type := { type with t := type.t ||| VT_LLONG }
    else type := { type with t := type.t ||| VT_INT }
    if r == VT_STRUCT then
      load_packed_bf R type bit_pos bit_size
    else
      let bits : Int32 := if type.t &&& VT_BTYPE == VT_LLONG then 64 else 32
      R.gen_cast type
      vpushi R (bits - (bit_pos + bit_size))
      R.gen_op TOK_SHL
      vpushi R (bits - bit_size)
      R.gen_op TOK_SAR
    R.gv rc
  else
    if is_float (← vtt) && ((← vtr) &&& (VT_VALMASK ||| VT_LVAL)) == VT_CONST then
      let sec := (← get).rodata_section
      let (size0, align0) ← R.type_size (← vt).type
      let (size, align) := if ← NODATA_WANTED then ((0 : Int32), (1 : Int32)) else (size0, align0)
      let offset ← section_add sec size.toInt64.toUInt64 align
      vpush_ref R (← vt).type sec offset.toUInt64 size.toInt64.toUInt64
      vswap R
      R.init_putv sec 0 0 (← vt).type offset.toUInt64
      modVt 0 fun v => { v with r := v.r ||| VT_LVAL.toUInt16' }
    let bt := (← vtt) &&& VT_BTYPE
    if bt == VT_VOID || bt == VT_STRUCT then return (← vtr)
    if bt == VT_LDOUBLE && rc == RC_FLOAT then rc := RC_INT
    let rc2 := RC2_TYPE bt rc
    let v ← vt
    let mut r := v.r.toInt32' &&& VT_VALMASK
    let r_ok := v.r.toInt32' &&& VT_LVAL == 0 && r < VT_CONST && (reg_classes[r.toNat']! &&& rc) != 0
    let r2_ok := rc2 == 0 || (v.r2.toInt32' < VT_CONST && (reg_classes[v.r2.toNat]! &&& rc2) != 0)
    if !r_ok || !r2_ok then
      if !r_ok then
        if r < VT_CONST && (reg_classes[r.toNat']! &&& rc) != 0 && rc2 == 0 then
          save_reg_upstack R r 1
        else
          r ← get_reg R rc
      if rc2 != 0 then
        let load_type := if bt == VT_QFLOAT then VT_DOUBLE else VT_PTRDIFF_T
        let original_type := (← vt).type.t
        let mut done := false
        let v ← vt
        if (v.r.toInt32' &&& (VT_VALMASK ||| VT_LVAL)) == VT_CONST then
          let ll := v.c.i
          setVtCi 0 ll
          load R r (← vt)
          setVtR 0 r
          vpushi R (ll >>> 32).toUInt32.toInt32
        else if v.r.toInt32' &&& VT_LVAL != 0 then
          save_reg_upstack R v.r.toInt32' 1
          setVtT 0 load_type
          load R r (← vt)
          vdup R
          setVtR (-1) r
          incr_offset R 8
        else
          if !r_ok then load R r (← vt)
          if r2_ok && (← vt).r2.toInt32' < VT_CONST then
            done := true
          else
            vdup R
            setVtR (-1) r
            let r2v := (← vt (-1)).r2.toInt32'
            setVtR 0 r2v
        if !done then
          let r2 ← get_reg R rc2
          load R r2 (← vt)
          vpop R
          modVt 0 fun v => { v with r2 := r2.toUInt16' }
        setVtT 0 original_type
      else
        if (← vtr) == VT_CMP then vset_VT_JMP R
        load R r (← vt)
    setVtR 0 r
    return r

/-- `gv2(rc1, rc2)` -/
def gv2 (R : Rec) (rc1 rc2 : Int32) : M Unit := do
  if (← vtr) != VT_CMP && rc1 ≤ rc2 then
    vswap R
    let _ ← gv R rc1
    vswap R
    let _ ← gv R rc2
    if ((← vtr (-1)) &&& VT_VALMASK) ≥ VT_CONST then
      vswap R
      let _ ← gv R rc1
      vswap R
  else
    let _ ← gv R rc2
    vswap R
    let _ ← gv R rc1
    vswap R
    if ((← vtr 0) &&& VT_VALMASK) ≥ VT_CONST then
      let _ ← gv R rc2

/-- `gv_dup()` -/
def gv_dup (R : Rec) : M Unit := do
  let t ← vtt
  let rc := RC_TYPE t
  let _ ← gv R rc
  let r ← get_reg R rc
  vdup R
  load R r (← vt)
  setVtR 0 r

end Tcc.Model
