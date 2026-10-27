import Tcc.Model.Gen6

/-!
# TccModel: tccgen.c, part 7 — initializers, declarations, functions,
and the compile driver

tccgen.c lines 7600–9045 and 380–460: parse_init_elem, init_putz,
decl_designator, init_putv (with write_ldouble's rv64 memcpy),
decl_initializer, decl_initializer_alloc, VLA parameters, gen_function,
inline functions, `_Static_assert`, `decl`; tccgen_init/compile/finish;
libtcc.c's tcc_compile, tccdbg.c's tcc_debug_start (the STT_FILE symbol).
-/

namespace Tcc.Model
open C
open Tok

/-- `init_params` -/
structure InitP where
  sec : SecId := 0
  local_offset : Int32 := 0
  flex_array_ref : SymId := 0
  deriving Inhabited

def EXPR_CONST : Int32 := 1
def EXPR_ANY : Int32 := 2
def DIF_FIRST : Int32 := 1
def DIF_SIZE_ONLY : Int32 := 2
def DIF_HAVE_ELEM : Int32 := 4
def DIF_CLEAR : Int32 := 8
def R_DATA_PTR : Int32 := 2   -- R_RISCV_64

/-- a scratch Sym (a C stack local `Sym x = {0}`), outside the free list -/
def scratchSym : M SymId :=
  modifyGet fun st =>
    let a := st.syms
    let st := { st with syms := #[] }
    (a.size, { st with syms := a.push default })

/-- `parse_init_elem(expr_type)` -/
def parse_init_elem (R : Rec) (expr_type : Int32) : M Unit := do
  if expr_type == EXPR_CONST then
    let saved := (← get).global_expr
    modify fun s => { s with global_expr := 1 }
    expr_const1 R
    modify fun s => { s with global_expr := saved }
    let v ← vt
    let r := v.r.toInt32'
    if (r &&& (VT_VALMASK ||| VT_LVAL)) != VT_CONST &&
       ((r &&& (VT_SYM ||| VT_LVAL)) != (VT_SYM ||| VT_LVAL) || (← getSym v.sym).v < SYM_FIRST_ANOM) then
      tcc_error_s R "initializer element is not constant"
  else
    R.expr_eq

/-- `init_assert(p, offset)` -/
def init_assert (R : Rec) (p : InitP) (offset : Int32) : M Unit := do
  let bad ←
    if p.sec != 0 then do
      let nd ← NODATA_WANTED
      pure (!nd && offset.toNat' > (← getSec p.sec).data_offset && offset ≥ 0)
    else
      pure ((← get).nocode_wanted == 0 && offset > p.local_offset)
  if bad then
    tcc_error R (cfmt "internal compiler error in %s:%d: %s" [.s "init_assert".toUTF8, .i 7652, .s "initializer overflow".toUTF8])

/-- `init_putz(p, c, size)` -/
def init_putz (R : Rec) (p : InitP) (c : UInt64) (size : Int32) : M Unit := do
  init_assert R p (c.toUInt32.toInt32 + size)
  if p.sec != 0 then
    if !(← NODATA_WANTED) then
      modSecData p.sec fun d => Bytes.copy d c.toNat (Bytes.zeros size.toNat') 0 size.toNat'
  else
    vpush_helper_func R TOK_memset
    vseti R VT_LOCAL c.toUInt32.toInt32
    vpushi R 0
    vpushs R size.toInt64.toUInt64
    R.gfunc_call 3

/-- `decl_design_delrels(sec, c, size)` -/
def decl_design_delrels (sec : SecId) (c size : Int32) : M Unit := do
  if sec == 0 then return
  let sr := (← getSec sec).reloc
  if sr == 0 then return
  let srs ← getSec sr
  let n := srs.data_offset / 24
  let mut w := 0
  let mut d := srs.data
  let mut off := srs.data_offset
  for k in [0:n] do
    let r := readRela d (24 * k)
    if r.r_offset ≥ c.toInt64.toUInt64 && r.r_offset < (c + size).toInt64.toUInt64 then
      off := off - 24
    else
      if w != k then d := writeRela d (24 * w) r
      w := w + 1
  modSec sr fun x => { x with data := d, data_offset := off }

/-- A union switch invalidates descendant records by type membership,
never an enclosing record at the same address. -/
def init_union_nested : Nat → SymId → SymId → M Bool
  | 0, _, _ => throw (.fuel "init_union_nested")
  | fuel+1, outer, inner => do
    let mut f := (← getSym outer).next
    let bound ← fuelM
    for _ in [0:bound] do
      if f == 0 then break
      let field ← getSym f
      let mut t := field.type
      for _ in [0:bound] do
        if t.t &&& VT_ARRAY == 0 then break
        t ← pointed_type t
      if t.t &&& VT_BTYPE == VT_STRUCT then
        if t.ref == inner then return true
        if ← init_union_nested fuel t.ref inner then return true
      f := field.next
    return false

/-- `init_union_select(p, type, member, c, flags)` -/
def init_union_select (R : Rec) (p : InitP) (type : CType) (member : SymId)
    (c : UInt64) (flags : Int32) : M Unit := do
  if flags &&& DIF_SIZE_ONLY != 0 || (← getSym type.ref).type.t != VT_UNION then return
  let us := (← get).init_unions
  let old := us.find? fun u => u.type == type.ref && u.addr == c
  if let some u := old then
    if u.member != member then
      let size := (← getSym type.ref).c
      let fuel ← fuelM
      let mut kept : Array InitUnion := #[]
      for v in us do
        if v.type == type.ref && v.addr == c then
          kept := kept.push v
        else if v.addr - c < size.toInt64.toUInt64 then
          if !(← init_union_nested fuel type.ref v.type) then kept := kept.push v
        else kept := kept.push v
      modify fun s => { s with init_unions := kept }
      decl_design_delrels p.sec c.toUInt32.toInt32 size
      init_putz R p c size
  else
    modify fun s => { s with init_unions := s.init_unions.push { type := type.ref, addr := c } }
  modify fun s => { s with init_unions := s.init_unions.map fun u =>
    if u.type == type.ref && u.addr == c then { u with member := member } else u }

/-- `init_field_path`: select the direct member along every anonymous path.
The size-only probe has no effects, matching patched tccgen.c. -/
def init_field_path : Nat → Rec → InitP → CType → SymId → UInt64 → Int32 → M Bool
  | 0, _, _, _, _, _, _ => throw (.fuel "init_field_path")
  | fuel+1, R, p, type, field, c, flags => do
    let mut s := (← getSym type.ref).next
    let bound ← fuelM
    for _ in [0:bound] do
      if s == 0 then break
      let sx ← getSym s
      let mut found := s == field
      if !found && sx.type.t &&& VT_BTYPE == VT_STRUCT && sx.v >= (SYM_FIRST_ANOM ||| SYM_FIELD) then
        found ← init_field_path fuel R p sx.type field (c + sx.c.toInt64.toUInt64) (flags ||| DIF_SIZE_ONLY)
      if found then
        init_union_select R p type s c flags
        if s != field then
          let _ ← init_field_path fuel R p sx.type field (c + sx.c.toInt64.toUInt64) flags
        return true
      s := sx.next
    return false

/-- `decl_design_flex(p, ref, index)` -/
def decl_design_flex (R : Rec) (p : InitP) (ref : SymId) (index : Int32) : M Unit := do
  let rc ← symC ref
  if ref == p.flex_array_ref then
    if index ≥ rc then modSym ref fun y => { y with c := index + 1 }
  else if rc < 0 && index ≥ 0 then
    tcc_error_s R "flexible array has zero size in this context"

/-- One containing aggregate and its selected child, as in patched tccgen.c. -/
structure InitCursor where
  type : CType
  addr : UInt64
  field : SymId := 0
  index : Int32 := 0
  deriving Inhabited

/-- The last frame selects the next object; each explicit brace list owns a path. -/
structure InitPath where
  frames : Array InitCursor := #[]
  deriving Inhabited

def init_path_push (path : InitPath) (type : CType) (c : UInt64) : M InitPath := do
  let field ← if type.t &&& VT_BTYPE == VT_STRUCT then pure (← getSym type.ref).next else pure 0
  return { frames := path.frames.push { type := type, addr := c, field := field } }

def init_cursor_type (q : InitCursor) : M CType := do
  if q.type.t &&& VT_ARRAY != 0 then pointed_type q.type else pure (← getSym q.field).type

def init_cursor_addr (R : Rec) (q : InitCursor) : M UInt64 := do
  if q.type.t &&& VT_ARRAY != 0 then
    let size := (← R.type_size (← pointed_type q.type)).1
    return q.addr + (q.index * size).toInt64.toUInt64
  else return q.addr + (← getSym q.field).c.toInt64.toUInt64

/-- Layout-only integer bitfields are never initializer positions. -/
def init_skip_padding (f0 : SymId) : M SymId := do
  let mut f := f0
  let fuel ← fuelM
  for _ in [0:fuel] do
    if f == 0 then break
    let fx ← getSym f
    if !(fx.v &&& SYM_FIRST_ANOM != 0 && is_integer_btype (fx.type.t &&& VT_BTYPE)) then break
    f := fx.next
  return f

def init_path_next (p : InitP) (path0 : InitPath) : M InitPath := do
  let mut path := path0
  for _ in [0:path0.frames.size] do
    if path.frames.isEmpty then break
    let mut q := path.frames.back!
    let mut more := false
    if q.type.t &&& VT_ARRAY != 0 then
      q := { q with index := q.index + 1 }
      more := q.type.ref == p.flex_array_ref || q.index < (← symC q.type.ref)
    else
      let field ← if (← getSym q.type.ref).type.t == VT_UNION then pure 0 else init_skip_padding (← getSym q.field).next
      q := { q with field := field }
      more := field != 0
    if more then return { frames := path.frames.set! (path.frames.size - 1) q }
    path := { frames := path.frames.pop }
  return path

/-- Retain every anonymous container hidden by find_field's flattened result. -/
def init_path_field : Nat → Rec → InitP → InitPath → SymId → Int32 → M InitPath
  | 0, _, _, _, _, _ => throw (.fuel "init_path_field")
  | fuel+1, R, p, path0, field, flags => do
    let mut path := path0
    let q := path.frames.back!
    let mut s := (← getSym q.type.ref).next
    let bound ← fuelM
    for _ in [0:bound] do
      if s == 0 then break
      let sx ← getSym s
      let mut found := s == field
      if !found && sx.type.t &&& VT_BTYPE == VT_STRUCT && sx.v >= (SYM_FIRST_ANOM ||| SYM_FIELD) then
        found ← init_field_path fuel R p sx.type field (q.addr + sx.c.toInt64.toUInt64) (flags ||| DIF_SIZE_ONLY)
      if found then
        path := { frames := path.frames.set! (path.frames.size - 1) { q with field := s } }
        init_union_select R p q.type s q.addr flags
        if s != field then
          path ← init_path_push path sx.type (q.addr + sx.c.toInt64.toUInt64)
          path ← init_path_field fuel R p path field flags
        return path
      s := sx.next
    return path

/-- `decl_designator`: initialize the selected object and advance its path. -/
def decl_designator (R : Rec) (p : InitP) (root : CType) (corig : UInt64) (path0 : InitPath)
    (flags0 : Int32) (al0 : Int32) : M (Int32 × InitPath) := do
  let mut path := path0
  let mut flags := flags0
  let mut al := al0
  let mut nb_elems : Int32 := 1
  let mut designated := false
  let fuel ← fuelM
  if flags &&& DIF_HAVE_ELEM == 0 then
    let mut structField : Option Int32 := none
    if (← get).gnu_ext != 0 && (← getTok) ≥ C.TOK_UIDENT then
      let l ← getTok
      R.next
      if (← getTok) == 58 then structField := some l
      else unget_tok l
    for _ in [0:fuel] do
      let tk ← getTok
      if !(structField.isSome || (nb_elems == 1 && (tk == 91 || tk == 46))) then break
      if !designated then
        path ← init_path_push {} root corig
        designated := true
      else
        let q := path.frames.back!
        let t ← init_cursor_type q
        let c ← init_cursor_addr R q
        path ← init_path_push path t c
      let q := path.frames.back!
      if structField.isNone && tk == 91 then
        if q.type.t &&& VT_ARRAY == 0 then expect R "array type"
        R.next
        let index ← R.expr_const
        let mut index_last := index
        if (← getTok) == TOK_DOTS && (← get).gnu_ext != 0 then
          R.next
          index_last ← R.expr_const
        skip R 93
        let s := q.type.ref
        decl_design_flex R p s index_last
        if index < 0 || index_last ≥ (← symC s) || index_last < index then
          tcc_error_s R "index exceeds array bounds or range is empty"
        path := { frames := path.frames.set! (path.frames.size - 1) { q with index := index } }
        nb_elems := index_last - index + 1
      else
        let l ← match structField with
          | some l => pure l
          | none => do R.next; getTok
        structField := none
        R.next
        let (f, _) ← find_field fuel R q.type l
        path ← init_path_field fuel R p path f flags
    if designated then
      if (← getTok) == 61 then R.next
      else if (← get).gnu_ext == 0 then expect R "="
  if path.frames.isEmpty then tcc_error_s R "too many initializers"
  let mut q := path.frames.back!
  if q.type.t &&& VT_ARRAY != 0 then
    decl_design_flex R p q.type.ref q.index
    if q.index ≥ (← symC q.type.ref) then tcc_error_s R "too many initializers"
  else
    let f ← init_skip_padding q.field
    q := { q with field := f }
    if f == 0 then tcc_error_s R "too many initializers"
    init_union_select R p q.type f q.addr flags
  let type ← init_cursor_type q
  let mut c ← init_cursor_addr R q
  q := { q with index := q.index + nb_elems - 1 }
  path := { frames := path.frames.set! (path.frames.size - 1) q }
  let elem_size := (← R.type_size type).1
  if flags &&& DIF_SIZE_ONLY == 0 && (c - corig).toUInt32.toInt32 < al then
    decl_design_delrels p.sec c.toUInt32.toInt32 (elem_size * nb_elems)
    flags := flags &&& ~~~DIF_CLEAR
  R.decl_initializer p.sec p.local_offset p.flex_array_ref type c (flags &&& ~~~DIF_FIRST)
  if flags &&& DIF_SIZE_ONLY == 0 && nb_elems > 1 then
    let mut t1 := type
    if p.sec != 0 || type.t &&& VT_ARRAY != 0 then
      let aref ← scratchSym
      modSym aref fun y => { y with c := elem_size }
      t1 := { t := VT_STRUCT, ref := aref }
    if p.sec != 0 then vpush_ref R t1 p.sec c elem_size.toInt64.toUInt64
    else vset R t1 (VT_LOCAL ||| VT_LVAL) c.toUInt32.toInt32
    for i in [1:nb_elems.toNat'] do
      vdup R
      R.init_putv p.sec p.local_offset p.flex_array_ref t1 (c + (elem_size * Int32.ofNat i).toInt64.toUInt64)
    vpop R
  c := c + (nb_elems * elem_size).toInt64.toUInt64
  if (c - corig).toUInt32.toInt32 > al then al := (c - corig).toUInt32.toInt32
  path ← init_path_next p path
  return (al, path)

/-- `init_putv(p, type, c)` -/
def init_putv (R : Rec) (p : InitP) (type : CType) (c : UInt64) : M Unit := do
  let dtype := { type with t := type.t &&& ~~~VT_CONSTANT }
  let mut size := (← R.type_size type).1
  if type.t &&& VT_BITFIELD != 0 then size := (BIT_POS type.t + BIT_SIZE type.t + 7) / 8
  init_assert R p (c.toUInt32.toInt32 + size)
  let sec := p.sec
  if sec != 0 then
    gen_assign_cast R dtype
    let bt := type.t &&& VT_BTYPE
    let v ← vt
    let r := v.r.toInt32'
    if r &&& VT_SYM != 0 && bt != VT_PTR && (bt != VT_LLONG || type.t &&& VT_BITFIELD != 0) &&
       !(r &&& VT_CONST != 0 && (← getSym v.sym).v ≥ SYM_FIRST_ANOM) then
      tcc_error_s R "initializer element is not computable at load time"
    if ← NODATA_WANTED then
      vtopDec
      return
    let ptr := c.toNat
    let v ← vt
    let val := v.c.i
    let r := v.r.toInt32'
    if (r &&& (VT_SYM ||| VT_CONST)) == (VT_SYM ||| VT_CONST) && (← getSym v.sym).v ≥ SYM_FIRST_ANOM &&
       (r &&& VT_LVAL != 0 || bt == VT_STRUCT) then
      let esym ← getElfSym (← get).symtab_section (← getSym v.sym).c.toNat'
      let st ← get
      let ssec := st.sections[esym.st_shndx.toNat]!
      let srcOff := (esym.st_value.toInt64 + v.c.i.toUInt32.toInt32.toInt64).toUInt64.toNat
      let sdata := (← getSec ssec).data
      let chunk := sdata.extract srcOff (srcOff + size.toNat')
      modSecData sec fun d => Bytes.copy d ptr chunk 0 size.toNat'
      let srl := (← getSec ssec).reloc
      if srl != 0 then
        let mut relofs := (← getSec srl).data_offset
        let fuel ← fuelM
        for _ in [0:fuel] do
          if relofs < 24 then break
          relofs := relofs - 24
          let rel := readRela (← getSec srl).data relofs
          if rel.r_offset ≥ esym.st_value + size.toInt64.toUInt64 then continue
          if rel.r_offset < esym.st_value then break
          put_elf_reloca (← get).symtab_section sec (c + rel.r_offset - esym.st_value) (R_TYPE rel.r_info)
            (R_SYM rel.r_info) rel.r_addend
    else
      if type.t &&& VT_BITFIELD != 0 then
        let mut bit_pos := BIT_POS v.type.t
        let mut bit_size := BIT_SIZE v.type.t
        let mut pp := ptr + (bit_pos >>> 3).toNat'
        bit_pos := bit_pos &&& 7
        let mut bits : Int32 := 0
        let fuel ← fuelM
        for _ in [0:fuel] do
          if bit_size == 0 then break
          let mut n := 8 - bit_pos
          if n > bit_size then n := bit_size
          let vv : UInt8 := ((val >>> bits.toUInt64) <<< bit_pos.toUInt64).toUInt8
          let m : UInt8 := ((((1 : UInt32) <<< n.toUInt32) - 1) <<< bit_pos.toUInt32).toUInt8
          let pp' := pp
          modSecData sec fun d => d.set! pp' ((d.get! pp' &&& ~~~m) ||| (vv &&& m))
          bits := bits + n; bit_size := bit_size - n; bit_pos := 0; pp := pp + 1
      else if bt == VT_BOOL then modSecData sec fun d => d.set! ptr (if val != 0 then 1 else 0)
      else if bt == VT_BYTE then modSecData sec fun d => d.set! ptr val.toUInt8
      else if bt == VT_SHORT then modSecData sec fun d => Bytes.w16 d ptr val.toUInt16
      else if bt == VT_FLOAT then modSecData sec fun d => Bytes.w32 d ptr val.toUInt32
      else if bt == VT_DOUBLE then modSecData sec fun d => Bytes.w64 d ptr val
      else if bt == VT_LDOUBLE then
        let hi := v.c.hi
        modSecData sec fun d => Bytes.w64 (Bytes.w64 d ptr val) (ptr + 8) hi
      else if bt == VT_LLONG || bt == VT_PTR then
        if r &&& VT_SYM != 0 then greloca sec v.sym c R_DATA_PTR val.toInt64
        else modSecData sec fun d => Bytes.w64 d ptr val
      else if bt == VT_INT then modSecData sec fun d => Bytes.w32 d ptr val.toUInt32
    vtopDec
  else
    vset R dtype (VT_LOCAL ||| VT_LVAL) c.toUInt32.toInt32
    vswap R
    R.vstore
    vpop R

/-- `decl_initializer(p, type, c, flags)` -/
def decl_initializer (R : Rec) (p : InitP) (type : CType) (c : UInt64) (flags0 : Int32) : M Unit := do
  let mut flags := flags0
  let fuel ← fuelM
  let tk ← getTok
  if flags &&& DIF_HAVE_ELEM == 0 && tk != 123 && !TOK_IS_STR tk &&
     (flags &&& DIF_SIZE_ONLY == 0 || type.t &&& VT_BTYPE == VT_STRUCT) then
    let ncw_prev := (← get).nocode_wanted
    if flags &&& DIF_SIZE_ONLY != 0 && p.sec == 0 then
      modify fun s => { s with nocode_wanted := s.nocode_wanted + 1 }
    parse_init_elem R (if p.sec == 0 then EXPR_ANY else EXPR_CONST)
    modify fun s => { s with nocode_wanted := ncw_prev }
    flags := flags ||| DIF_HAVE_ELEM
  -- the init-list loop (do_init_list), shared by arrays and structs
  let initList (s : SymId) (n size1 : Int32) (no_oblock : Bool) (flags0 : Int32) : M Unit := do
    let mut flags := flags0
    if flags &&& (DIF_CLEAR ||| DIF_SIZE_ONLY) == 0 then
      init_putz R p c (n * size1)
      flags := flags ||| DIF_CLEAR
    let mut len : Int32 := 0
    decl_design_flex R p s (len - 1)
    let mut path ← init_path_push {} type c
    for _ in [0:fuel] do
      if !((← getTok) != 125 || flags &&& DIF_HAVE_ELEM != 0) then break
      let (len', path') ← decl_designator R p type c path flags len
      len := len'
      path := path'
      flags := flags &&& ~~~DIF_HAVE_ELEM
      if no_oblock && path.frames.isEmpty then break
      if (← getTok) == 125 then break
      skip R 44
      if no_oblock && ((← getTok) == 91 || (← getTok) == 46) then
        unget_tok 44
        break
  if type.t &&& VT_ARRAY != 0 then
    let mut no_oblock := true
    let tk ← getTok
    if (flags &&& DIF_FIRST != 0 && !TOK_IS_STR tk) || tk == 123 then
      skip R 123
      no_oblock := false
    let s := type.ref
    let n ← symC s
    let t1 ← pointed_type type
    let size1 := (← R.type_size t1).1
    let tk ← getTok
    let mut doArray := true
    if (tk == TOK_LSTR && t1.t &&& VT_BTYPE == VT_INT) ||
       (tk == TOK_U16STR && t1.t &&& VT_BTYPE == VT_SHORT) ||
       (tk == TOK_U32STR && t1.t &&& VT_BTYPE == VT_INT) || (tk == TOK_STR && t1.t &&& VT_BTYPE == VT_BYTE) then
      doArray := false
      let str_tok := tk
      let mut len : Int32 := 0
      let mut initstr := ByteArray.empty
      if size1 != TOK_STR_SIZE tk then tcc_error_s R "unhandled string literal merging"
      for _ in [0:fuel] do
        let tk ← getTok
        if !(TOK_IS_STR tk) then break
        if size1 != TOK_STR_SIZE tk then tcc_error_s R "incompatible string literal initializer"
        if initstr.size > 0 then initstr := initstr.extract 0 (initstr.size - size1.toNat')
        let tc := (← get).tokc
        len := len + tc.strSize / size1
        len := len - 1
        initstr := initstr ++ tc.sdata.extract 0 tc.strSize.toNat'
        R.next
      let tk ← getTok
      if tk != 41 && tk != 125 && tk != 44 && tk != 59 && tk != C.TOK_EOF then
        unget_tok str_tok
        modify fun s => { s with tokc := s.tokc.setStr initstr (Int32.ofNat initstr.size), initstr := initstr }
        doArray := true
      else
        modify fun s => { s with initstr := initstr }
        decl_design_flex R p s len
        if flags &&& DIF_SIZE_ONLY == 0 then
          let mut nb := n
          if len < nb then nb := len
          if len > nb then tcc_warning R "initializer-string for array is too long".toUTF8
          if p.sec != 0 && size1 == 1 then
            init_assert R p (c.toUInt32.toInt32 + nb)
            if !(← NODATA_WANTED) then
              modSecData p.sec fun d => Bytes.copy d c.toNat initstr 0 nb.toNat'
          else
            for i in [0:n.toNat'] do
              let ii := Int32.ofNat i
              let mut ch : Int32 := 0
              if ii ≥ nb then
                if flags &&& DIF_CLEAR != 0 then break
                if n - ii ≥ 4 then
                  init_putz R p (c + (ii * size1).toInt64.toUInt64) ((n - ii) * size1)
                  break
                ch := 0
              else if size1 == 1 then ch := (initstr.get! i).toUInt32.toInt32
              else if size1 == 2 then ch := (Bytes.r16 initstr (2 * i)).toUInt32.toInt32
              else ch := (Bytes.r32 initstr (4 * i)).toInt32
              vpushi R ch
              R.init_putv p.sec p.local_offset p.flex_array_ref t1 (c + (ii * size1).toInt64.toUInt64)
        if (← getTok) == 44 && !no_oblock then R.next
    if doArray then
      initList s n size1 no_oblock flags
    if !no_oblock then skip R 125
  else if flags &&& DIF_HAVE_ELEM != 0 && (← R.is_compatible_unqualified_types type (← vt).type) then
    -- one_elem
    if flags &&& DIF_SIZE_ONLY != 0 then
      vpop R
    else
      let v ← vt
      if p.sec == 0 && flags &&& DIF_CLEAR != 0 &&
         (v.r.toInt32' &&& (VT_VALMASK ||| VT_LVAL ||| VT_SYM)) == VT_CONST && v.c.i == 0 &&
         btype_size (type.t &&& VT_BTYPE) != 0 then
        vpop R
      else
        R.init_putv p.sec p.local_offset p.flex_array_ref type c
  else if type.t &&& VT_BTYPE == VT_STRUCT then
    let mut no_oblock := true
    if flags &&& DIF_FIRST != 0 || (← getTok) == 123 then
      skip R 123
      no_oblock := false
    let s := type.ref
    let n ← symC s
    initList s n 1 no_oblock flags
    if !no_oblock then skip R 125
  else if (← getTok) == 123 then
    if flags &&& DIF_HAVE_ELEM != 0 then skip R 59
    R.next
    R.decl_initializer p.sec p.local_offset p.flex_array_ref type c (flags &&& ~~~DIF_HAVE_ELEM)
    skip R 125
  else if flags &&& DIF_SIZE_ONLY != 0 then
    if flags &&& DIF_HAVE_ELEM != 0 then vpop R
    else let _ ← skip_or_save_block R false
  else
    if flags &&& DIF_HAVE_ELEM == 0 then
      let tk ← getTok
      if !TOK_IS_STR tk then expect R "string constant"
      parse_init_elem R (if p.sec == 0 then EXPR_ANY else EXPR_CONST)
    let v ← vt
    if p.sec == 0 && flags &&& DIF_CLEAR != 0 &&
       (v.r.toInt32' &&& (VT_VALMASK ||| VT_LVAL ||| VT_SYM)) == VT_CONST && v.c.i == 0 &&
       btype_size (type.t &&& VT_BTYPE) != 0 then
      vpop R
    else
      R.init_putv p.sec p.local_offset p.flex_array_ref type c

end Tcc.Model
