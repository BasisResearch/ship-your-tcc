import Tcc.Model.Gen2

/-!
# TccModel: tccgen.c, part 3 — attributes, struct/union/enum layout,
declaration specifiers and declarators

tccgen.c lines 3923–5560: parse_attribute, find_field, check_fields,
struct_layout, struct_decl, parse_btype, post_type, type_decl, indir,
gfunc_param_typed, expr_type, parse_expr_type, parse_type,
parse_builtin_params, parse_atomic.

tcc's declarators work on `CType *`s that may point into a Sym (the
innermost pointed-to type); `CLoc` names such a location.
-/

namespace Tcc.Model
open C
open Tok

/-! ## attributes -/

def setA (ad : AttributeDef) (pos w : UInt16) (v : UInt16) : AttributeDef :=
  { ad with a := setBits16 ad.a pos w v }
def setF (ad : AttributeDef) (pos w : UInt32) (v : UInt32) : AttributeDef :=
  { ad with f := setBits32 ad.f pos w v }

/-- `parse_attribute(ad)` -/
def parse_attribute (R : Rec) (ad0 : AttributeDef) : M AttributeDef := do
  let fuel ← fuelM
  let mut ad := ad0
  for _ in [0:fuel] do   -- redo:
    let tk ← getTok
    if tk != TOK_ATTRIBUTE1 && tk != TOK_ATTRIBUTE2 then return ad
    R.next
    skip R 40
    skip R 40
    for _ in [0:fuel] do
      if (← getTok) == 41 then break
      if (← getTok) < C.TOK_IDENT then expect R "attribute name"
      let t ← getTok
      R.next
      let mut skipParam := false
      if t == TOK_CLEANUP1 || t == TOK_CLEANUP2 then
        skip R 40
        let tk ← getTok
        let mut s ← sym_find tk
        if s == 0 then
          let st ← get
          tcc_warning_c R WARNC_implicit_function_declaration
            (cfmt "implicit declaration of function '%s'" [.s (← get_tok_str st.tok (some st.tokc))])
          s ← external_global_sym tk (← get).func_old_type
        else if (← getSym s).type.t &&& VT_BTYPE != VT_FUNC then
          let st ← get
          tcc_error R (cfmt "'%s' is not declared as function" [.s (← get_tok_str st.tok (some st.tokc))])
        ad := { ad with cleanup_func := s }
        R.next
        skip R 41
      else if t == TOK_CONSTRUCTOR1 || t == TOK_CONSTRUCTOR2 then ad := setF ad 6 1 1
      else if t == TOK_DESTRUCTOR1 || t == TOK_DESTRUCTOR2 then ad := setF ad 7 1 1
      else if t == TOK_ALWAYS_INLINE1 || t == TOK_ALWAYS_INLINE2 then ad := setF ad 16 1 1
      else if t == TOK_SECTION1 || t == TOK_SECTION2 then
        skip R 40
        let astr ← parse_mult_str R "section name"
        let n := (← get).sections.size
        let sec ← find_section (Bytes.cstrAt astr 0)
        ad := { ad with sec := sec, new_section := b2i (n < (← get).sections.size) }
        skip R 41
      else if t == TOK_ALIAS1 || t == TOK_ALIAS2 then
        skip R 40
        let astr ← parse_mult_str R "alias(\"target\")"
        ad := { ad with alias_target := (← tok_alloc R (Bytes.cstrAt astr 0)).tok }
        skip R 41
      else if t == TOK_VISIBILITY1 || t == TOK_VISIBILITY2 then
        skip R 40
        let astr := Bytes.cstrAt (← parse_mult_str R "visibility(\"default|hidden|internal|protected\")") 0
        if astr == "default".toUTF8 then ad := setA ad 7 2 0
        else if astr == "hidden".toUTF8 then ad := setA ad 7 2 2
        else if astr == "internal".toUTF8 then ad := setA ad 7 2 1
        else if astr == "protected".toUTF8 then ad := setA ad 7 2 3
        else expect R "visibility(\"default|hidden|internal|protected\")"
        skip R 41
      else if t == TOK_ALIGNED1 || t == TOK_ALIGNED2 then
        let mut n : Int32 := 0
        if (← getTok) == 40 then
          R.next
          n ← R.expr_const
          if n ≤ 0 || (n &&& (n - 1)) != 0 then tcc_error_s R "alignment must be a positive power of two"
          skip R 41
        else
          n := MAX_ALIGN
        let al := exact_log2p1 n
        ad := setA ad 0 5 al.toUInt16'
        if n != ((1 : Int32) <<< ((ad.a &&& 0x1f).toInt32' - 1)) then
          tcc_error R (cfmt "alignment of %d is larger than implemented" [.i n.toInt])
      else if t == TOK_PACKED1 || t == TOK_PACKED2 then ad := setA ad 5 1 1
      else if t == TOK_WEAK1 || t == TOK_WEAK2 then ad := setA ad 6 1 1
      else if t == TOK_NODEBUG1 || t == TOK_NODEBUG2 then ad := setA ad 13 1 1
      else if t == TOK_USED1 || t == TOK_USED2 || t == TOK_UNUSED1 || t == TOK_UNUSED2 then pure ()
      else if t == TOK_CONST1 || t == TOK_CONST2 || t == TOK_CONST3 || t == TOK_PURE1 || t == TOK_PURE2 then pure ()
      else if t == TOK_NOINLINE then pure ()
      else if t == TOK_FORMAT1 || t == TOK_FORMAT2 then skipParam := true
      else if t == TOK_NORETURN1 || t == TOK_NORETURN2 then ad := setF ad 5 1 1
      else if t == TOK_CDECL1 || t == TOK_CDECL2 || t == TOK_CDECL3 then ad := setF ad 0 3 0
      else if t == TOK_STDCALL1 || t == TOK_STDCALL2 || t == TOK_STDCALL3 then ad := setF ad 0 3 1
      else if t == TOK_MODE then
        skip R 40
        let tk ← getTok
        if tk == TOK_MODE_DI then ad := { ad with attr_mode := VT_LLONG + 1 }
        else if tk == TOK_MODE_QI then ad := { ad with attr_mode := VT_BYTE + 1 }
        else if tk == TOK_MODE_HI then ad := { ad with attr_mode := VT_SHORT + 1 }
        else if tk == TOK_MODE_SI || tk == TOK_MODE_word then ad := { ad with attr_mode := VT_INT + 1 }
        else tcc_warning R (cfmt "__mode__(%s) not supported\n" [.s (← get_tok_str tk none)])
        R.next
        skip R 41
      else if t == TOK_DLLEXPORT then ad := setA ad 9 1 1
      else if t == TOK_NODECORATE then ad := setA ad 10 1 1
      else if t == TOK_DLLIMPORT then ad := setA ad 11 1 1
      else
        tcc_warning_c R WARNC_unsupported (cfmt "'%s' attribute ignored" [.s (← get_tok_str t none)])
        skipParam := true
      if skipParam then
        if (← getTok) == 40 then
          let mut parenthesis : Int32 := 0
          for _ in [0:fuel] do
            let tk ← getTok
            if tk == 40 then parenthesis := parenthesis + 1
            else if tk == 41 then parenthesis := parenthesis - 1
            R.next
            if !(parenthesis != 0 && (← getTok) != -1) then break
      if (← getTok) != 44 then break
      R.next
    skip R 41
    skip R 41
  throw (.fuel "parse_attribute")

/-! ## struct fields and layout -/

/-- Promoted field lookup retains qualifiers of all anonymous containers. -/
structure FieldLookup where
  field : SymId
  offset : Int32
  qualifiers : Int32

/-- `find_field_qualified(type, v, &cumofs, &qualifiers)`. The caller adds
the enclosing expression's qualifiers; the field itself retains its type. -/
def find_field_qualified : Nat → Rec → CType → Int32 → M FieldLookup
  | 0, _, _, _ => throw (.fuel "find_field")
  | fuel+1, R, type, v => do
    let mut s := type.ref
    let v1 := v ||| SYM_FIELD
    if v &&& SYM_FIELD == 0 then
      if type.t &&& VT_BTYPE != VT_STRUCT then expect R "struct or union"
      if v < C.TOK_UIDENT then expect R "field name"
      let x ← getSym s
      if x.c < 0 then
        tcc_error R (cfmt "dereferencing incomplete type '%s'" [.s (← get_tok_str (x.v &&& ~~~SYM_STRUCT) none)])
    let lfuel ← fuelM
    for _ in [0:lfuel] do
      s := (← getSym s).next
      if s == 0 then break
      let x ← getSym s
      if x.v == v1 then return ⟨s, x.c, 0⟩
      if x.type.t &&& VT_BTYPE == VT_STRUCT && x.v ≥ (SYM_FIRST_ANOM ||| SYM_FIELD) then
        let result ← find_field_qualified fuel R x.type v1
        if result.field != 0 then
          return { result with
            offset := result.offset + x.c
            qualifiers := result.qualifiers ||| (x.type.t &&& VT_QUAL) }
    if v &&& SYM_FIELD == 0 then
      tcc_error R (cfmt "field not found: %s" [.s (← get_tok_str v none)])
    return ⟨0, 0, 0⟩

/-- Initializer lookup uses the same physical path without expression qualifiers. -/
def find_field (fuel : Nat) (R : Rec) (type : CType) (v : Int32) : M (SymId × Int32) := do
  let result ← find_field_qualified fuel R type v
  return (result.field, result.offset)

/-- `check_fields(type, check)` -/
def check_fields : Nat → Rec → CType → Bool → M Unit
  | 0, _, _, _ => throw (.fuel "check_fields")
  | fuel+1, R, type, check => do
    let mut s := type.ref
    let lfuel ← fuelM
    for _ in [0:lfuel] do
      s := (← getSym s).next
      if s == 0 then break
      let x ← getSym s
      let v := x.v &&& ~~~SYM_FIELD
      if v < SYM_FIRST_ANOM then
        let ts ← tokSym v
        if check && (ts.tok &&& SYM_FIELD != 0) then
          tcc_error R (cfmt "duplicate member '%s'" [.s (← get_tok_str v none)])
        modTokSym v fun t => { t with tok := t.tok ^^^ SYM_FIELD }
      else if x.type.t &&& VT_BTYPE == VT_STRUCT then
        check_fields fuel R x.type check

/-- `struct_layout(type, ad)` -/
def struct_layout (R : Rec) (type : CType) (ad : AttributeDef) : M Unit := do
  let st ← get
  let pcc := st.ms_bitfields == 0
  let pragma_pack := st.pack_stack[st.pack_stack_ptr]!
  let mut maxalign : Int32 := 1
  let mut offset : Int32 := 0
  let mut c : Int32 := 0
  let mut bit_pos : Int32 := 0
  let mut prevbt : Int32 := VT_STRUCT
  let mut prev_bit_size : Int32 := 0
  let isUnion := (← getSym type.ref).type.t == VT_UNION
  let fuel ← fuelM
  let mut f := (← getSym type.ref).next
  for _ in [0:fuel] do
    if f == 0 then break
    let fx ← getSym f
    let bit_size : Int32 := if fx.type.t &&& VT_BITFIELD != 0 then BIT_SIZE fx.type.t else -1
    let (size0, align0) ← R.type_size fx.type
    let mut size := size0
    let mut align := align0
    let mut a : Int32 := if fx.a_aligned != 0 then (1 : Int32) <<< (fx.a_aligned.toInt32' - 1) else 0
    let mut packed := false
    if pcc && bit_size == 0 then pure ()
    else
      if pcc && (fx.a_packed != 0 || (ad.a >>> 5) &&& 1 != 0) then
        align := 1; packed := true
      if pragma_pack != 0 then
        packed := true
        if pragma_pack < align then align := pragma_pack
        if pcc && pragma_pack < a then a := 0
    if a != 0 then align := a
    if isUnion then
      if pcc && bit_size ≥ 0 then size := (bit_size + 7) >>> 3
      offset := 0
      if size > c then c := size
    else if bit_size < 0 then
      if pcc then c := c + ((bit_pos + 7) >>> 3)
      c := (c + align - 1) &&& (-align)
      offset := c
      if size > 0 then c := c + size
      bit_pos := 0
      prevbt := VT_STRUCT
      prev_bit_size := 0
    else
      if pcc then
        let mut newField := false
        if bit_size == 0 then newField := true
        else if fx.a_aligned != 0 then newField := true
        else if !packed then
          let a8 := align * 8
          let ofs := ((c * 8 + bit_pos) % a8 + bit_size + a8 - 1) / a8
          if ofs > size / align then newField := true
        if newField then
          c := (c + ((bit_pos + 7) >>> 3) + align - 1) &&& (-align)
          bit_pos := 0
        if size == 8 && bit_size ≤ 32 then
          modSym f fun y => { y with type := { y.type with t := (y.type.t &&& ~~~VT_BTYPE) ||| VT_INT } }
          size := 4
        for _ in [0:fuel] do
          if bit_pos < align * 8 then break
          c := c + align
          bit_pos := bit_pos - align * 8
        offset := c
        if fx.v &&& SYM_FIRST_ANOM != 0 then align := 1
      else
        let bt := fx.type.t &&& VT_BTYPE
        if (bit_pos + bit_size > size * 8) || ((bit_size > 0) == (bt != prevbt)) then
          c := (c + align - 1) &&& (-align)
          offset := c
          bit_pos := 0
          if bit_size != 0 || prev_bit_size != 0 then c := c + size
        if bit_size == 0 && prevbt != bt then align := 1
        prevbt := bt
        prev_bit_size := bit_size
      let bp := bit_pos
      modSym f fun y => { y with type := { y.type with t := (y.type.t &&& ~~~((0x3f : Int32) <<< VT_STRUCT_SHIFT)) ||| (bp <<< VT_STRUCT_SHIFT) } }
      bit_pos := bit_pos + bit_size
    if align > maxalign then maxalign := align
    let off := offset
    modSym f fun y => { y with c := off, r := 0 }
    f := fx.next
  if pcc then c := c + ((bit_pos + 7) >>> 3)
  let bt0 : Int32 := if ad.a &&& 0x1f != 0 then (1 : Int32) <<< ((ad.a &&& 0x1f).toInt32' - 1) else 1
  let mut a := bt0
  if a < maxalign then a := maxalign
  let a' := a
  modSym type.ref fun y => { y with r := a'.toUInt16' }
  if pragma_pack != 0 && pragma_pack < maxalign && !pcc then
    a := pragma_pack
    if a < bt0 then a := bt0
  c := (c + a - 1) &&& (-a)
  let c' := c
  modSym type.ref fun y => { y with c := c' }
  -- check whether we can access bitfields by their type
  f := (← getSym type.ref).next
  for _ in [0:fuel] do
    if f == 0 then break
    let fx ← getSym f
    if fx.type.t &&& VT_BITFIELD == 0 then
      f := fx.next; continue
    let f' := f
    modSym f fun y => { y with type := { y.type with ref := f' }, aux := (-1 : Int32).toUInt32 }
    let bit_size := BIT_SIZE fx.type.t
    if bit_size == 0 then
      f := fx.next; continue
    let mut bpos := BIT_POS fx.type.t
    let fx ← getSym f
    let (size, _) ← R.type_size fx.type
    if bpos + bit_size ≤ size * 8 && fx.c + size ≤ c then
      f := fx.next; continue
    let mut c0 : Int32 := -1
    let mut s : Int32 := 1
    let mut align : Int32 := 1
    let mut tt : CType := { t := VT_BYTE }
    let mut px : Int32 := 0
    let mut cx : Int32 := 0
    for _ in [0:fuel] do
      px := fx.c * 8 + bpos
      cx := (px >>> 3) &&& (-align)
      px := px - (cx <<< 3)
      if c0 == cx then break
      s := (px + bit_size + 7) >>> 3
      if s > 4 then tt := { t := VT_LLONG }
      else if s > 2 then tt := { t := VT_INT }
      else if s > 1 then tt := { t := VT_SHORT }
      else tt := { t := VT_BYTE }
      let (s', a') ← R.type_size tt
      s := s'; align := a'
      c0 := cx
    if px + bit_size ≤ s * 8 && cx + s ≤ c then
      bpos := px
      let bp := bpos
      let cx' := cx
      let aux := if s != size then tt.t else (-1 : Int32)
      modSym f fun y => { y with c := cx', type := { y.type with
                                 t := (y.type.t &&& ~~~((0x3f : Int32) <<< VT_STRUCT_SHIFT)) ||| (bp <<< VT_STRUCT_SHIFT) },
                                 aux := if s != size then aux.toUInt32 else y.aux }
    else
      modSym f fun y => { y with aux := VT_STRUCT.toUInt32 }
    f := fx.next

/-- `in_range(n, t)` -/
def in_range (n : Int64) (t : Int32) : Bool :=
  let m : UInt64 := ((1 : UInt64) <<< (btype_size (t &&& VT_BTYPE) * 8 - 1).toUInt64) - 1
  if t &&& VT_UNSIGNED != 0 then n.toUInt64 ≤ (m <<< 1) + 1
  else n ≥ -(m.toInt64) - 1 && n ≤ m.toInt64

/-! ## struct/union/enum declarations -/

/-- `struct_decl(type, u)` -/
def struct_decl (R : Rec) (u : Int32) : M CType := do
  let mut ad : AttributeDef := {}
  R.next
  ad ← parse_attribute R ad
  let mut v : Int32 := 0
  if (← getTok) ≥ C.TOK_IDENT then
    v ← getTok
    R.next
  let mut bt : Int32 := 0
  let mut ut : Int32 := 0
  if u == VT_ENUM then
    ut := VT_INT
    if (← getTok) == 58 then
      R.next
      let (found, btype, _) ← R.parse_btype {} 0
      if !found || !is_integer_btype (btype.t &&& VT_BTYPE) then expect R "enum type"
      bt := btype.t &&& (VT_BTYPE ||| VT_LONG ||| VT_UNSIGNED ||| VT_DEFSIGN)
      ut := bt
  let mut s : SymId := 0
  let mut doDecl := false
  if v != 0 then
    s ← struct_find v
    if s != 0 then
      let x ← getSym s
      let tk ← getTok
      if x.sym_scope == (← get).local_scope || (tk != 123 && tk != 59) then
        if u == x.type.t then doDecl := true
        else if u == VT_ENUM && IS_ENUM x.type.t then doDecl := true
        else tcc_error R (cfmt "redeclaration of '%s'" [.s (← get_tok_str v none)])
  else
    if (← getTok) != 123 then expect R "struct/union/enum name"
    v := (← get).anon_sym
    modify fun st => { st with anon_sym := st.anon_sym + 1 }
  if !doDecl then
    let type1 : CType := { t := u ||| ut }
    s ← sym_push R (v ||| SYM_STRUCT) type1 0 (if bt != 0 then 0 else -1)
    let s' := s
    modSym s' fun y => { y with r := 0 }
  -- do_decl:
  let mut type : CType := { t := (← getSym s).type.t, ref := s }
  if (← getTok) == 123 then
    R.next
    let x ← getSym s
    if x.c != -1 && !(u == VT_ENUM && x.c == 0) then tcc_error_s R "struct/union/enum already defined"
    modSym s fun y => { y with c := -2 }
    let mut last : SymId := 0     -- ps = &last->next (0: &s->next)
    let fuel ← fuelM
    if u == VT_ENUM then
      let mut ll : Int64 := 0
      let mut pl : Int64 := 0
      let mut nl : Int64 := 0
      let ls := (← get).local_scope
      modSym s fun y => { y with aux := ls.toUInt32 }
      let mut t : CType := { t := VT_INT ||| VT_STATIC ||| VT_ENUM_VAL, ref := s }
      if bt != 0 then t := { t with t := bt ||| VT_STATIC ||| VT_ENUM_VAL }
      for _ in [0:fuel] do
        let vv ← getTok
        if vv < C.TOK_UIDENT then expect R "identifier"
        R.next
        if (← getTok) == 61 then
          R.next
          ll ← R.expr_const64
        if bt != 0 && !in_range ll t.t then
          tcc_error R (cfmt "enumerator '%s' out of range of its type" [.s (← get_tok_str vv none)])
        let ss ← sym_push R vv t VT_CONST 0
        let ll' := ll
        modSym ss fun y => y.setEnumVal ll'
        if last == 0 then modSym s fun y => { y with next := ss } else modSym last fun y => { y with next := ss }
        last := ss
        if ll < nl then nl := ll
        if ll > pl then pl := ll
        if (← getTok) != 44 then break
        R.next
        ll := ll + 1
        if (← getTok) == 125 then break
      skip R 125
      if bt != 0 then
        t := { t with t := bt }
        modSym s fun y => { y with c := 2 }
      else
        t := { t with t := VT_INT }
        if nl ≥ 0 then
          if pl != pl.toUInt64.toUInt32.toUInt64.toInt64 then t := { t with t := VT_LLONG ||| VT_LONG }
          t := { t with t := t.t ||| VT_UNSIGNED }
        else if pl != pl.toInt32.toInt64 || nl != nl.toInt32.toInt64 then
          t := { t with t := VT_LLONG ||| VT_LONG }
        let mut ss := (← getSym s).next
        for _ in [0:fuel] do
          if ss == 0 then break
          let y ← getSym ss
          let lv := y.enum_val
          if lv == lv.toInt32.toInt64 then
            ss := y.next; continue
          if t.t &&& VT_UNSIGNED != 0 then
            modSym ss fun z => { z with type := { z.type with t := z.type.t ||| VT_UNSIGNED } }
            if lv == lv.toUInt32.toUInt64.toInt64 then
              ss := y.next; continue
          modSym ss fun z => { z with type := { z.type with t := (z.type.t &&& ~~~VT_BTYPE) ||| (VT_LLONG ||| VT_LONG) } }
          ss := y.next
        modSym s fun y => { y with c := 1 }
      -- enum_done:
      let tt := t.t ||| VT_ENUM
      modSym s fun y => { y with type := { y.type with t := tt } }
      type := { type with t := tt }
    else
      let mut c : Int32 := 0
      let mut flexible := false
      let mut ad1 : AttributeDef := {}
      for _ in [0:fuel] do
        if (← getTok) == 125 then break
        let (found, btype, ad1') ← R.parse_btype {} 0
        ad1 := ad1'
        if !found then
          if (← getTok) == TOK_STATIC_ASSERT then
            R.do_Static_assert
            continue
          skip R 59
          continue
        for _ in [0:fuel] do
          if flexible then
            tcc_error R (cfmt "flexible array member '%s' not at the end of struct" [.s (← get_tok_str v none)])
          let mut bit_size : Int32 := -1
          v := 0
          let mut type1 := btype
          if (← getTok) != 58 then
            if (← getTok) != 59 then
              let (t1, ad1'', v') ← R.type_decl type1 ad1 TYPE_DIRECT
              type1 := t1; ad1 := ad1''; v := v'
            if v == 0 then
              if type1.t &&& VT_BTYPE != VT_STRUCT then expect R "identifier"
              else
                let bv := (← getSym btype.ref).v
                if (bv &&& ~~~SYM_STRUCT) < SYM_FIRST_ANOM then
                  if (← get).ms_extensions == 0 then expect R "identifier"
            if (← R.type_size type1).1 < 0 then
              if u == VT_STRUCT && type1.t &&& VT_ARRAY != 0 && c != 0 then flexible := true
              else tcc_error R (cfmt "field '%s' has incomplete type" [.s (← get_tok_str v none)])
            if type1.t &&& VT_BTYPE == VT_FUNC || type1.t &&& VT_BTYPE == VT_VOID || type1.t &&& VT_STORAGE != 0 then
              tcc_error R (cfmt "invalid type for '%s'" [.s (← get_tok_str v none)])
          if (← getTok) == 58 then
            R.next
            bit_size ← R.expr_const
            if bit_size < 0 then
              tcc_error R (cfmt "negative width in bit-field '%s'" [.s (← get_tok_str v none)])
            if v != 0 && bit_size == 0 then
              tcc_error R (cfmt "zero width for bit-field '%s'" [.s (← get_tok_str v none)])
            ad1 ← parse_attribute R ad1
          let (size, _) ← R.type_size type1
          if bit_size ≥ 0 then
            let bt1 := type1.t &&& VT_BTYPE
            if bt1 != VT_INT && bt1 != VT_BYTE && bt1 != VT_SHORT && bt1 != VT_BOOL && bt1 != VT_LLONG then
              tcc_error_s R "bitfields must have scalar type"
            let bsize := size * 8
            let st ← get
            if bit_size > bsize then
              tcc_error R (cfmt "width of '%s' exceeds its type" [.s (← get_tok_str v none)])
            else if bit_size == bsize && st.pack_stack[st.pack_stack_ptr]! == 0 &&
                    (ad.a >>> 5) &&& 1 == 0 && (ad1.a >>> 5) &&& 1 == 0 then
              pure ()
            else if bit_size == 64 then
              tcc_error_s R "field width 64 not implemented"
            else
              type1 := { type1 with t := (type1.t &&& ~~~VT_STRUCT_MASK) ||| VT_BITFIELD |||
                                          (bit_size.toUInt32 <<< (VT_STRUCT_SHIFT.toUInt32 + 6)).toInt32 }
          if v != 0 || type1.t &&& VT_BTYPE == VT_STRUCT then c := 1
          if v == 0 && (type1.t &&& VT_BTYPE == VT_STRUCT || bit_size ≥ 0) then
            v := (← get).anon_sym
            modify fun st => { st with anon_sym := st.anon_sym + 1 }
          if v != 0 then
            let ss ← sym_push R (v ||| SYM_FIELD) type1 0 0
            let aa := ad1.a
            modSym ss fun y => { y with a := aa }
            if last == 0 then modSym s fun y => { y with next := ss } else modSym last fun y => { y with next := ss }
            last := ss
          let tk ← getTok
          if tk == 59 || tk == C.TOK_EOF then break
          skip R 44
        skip R 59
      skip R 125
      ad ← parse_attribute R ad
      if ad.cleanup_func != 0 then
        tcc_warning R "attribute '__cleanup__' ignored on type".toUTF8
      check_fields fuel R type true
      check_fields fuel R type false
      struct_layout R type ad
  return type

/-- `sym_to_attr(ad, s)` -/
def sym_to_attr (ad : AttributeDef) (s : Sym) : AttributeDef :=
  { ad with a := merge_symattr ad.a s.a, f := merge_funcattr ad.f s.aux }

/-! ## declaration specifiers -/

/-- `parse_btype(type, ad, ignore_label)`: (type_found, type, ad) -/
def parse_btype (R : Rec) (_ad0 : AttributeDef) (ignore_label : Int32) : M (Bool × CType × AttributeDef) := do
  let mut ad : AttributeDef := {}
  let mut type_found := false
  let mut typespec_found := false
  let mut t : Int32 := VT_INT
  let mut bt : Int32 := -1
  let mut st : Int32 := -1
  let mut type : CType := { t := 0, ref := 0 }
  let fuel ← fuelM
  -- basic_type1 with `u`: returns updated (t, bt, st)
  let basic1 (u t bt st : Int32) : M (Int32 × Int32 × Int32) := do
    let mut t := t
    let mut bt := bt
    let mut st := st
    if u == VT_SHORT || u == VT_LONG then
      if st != -1 || (bt != -1 && bt != VT_INT) then tcc_error_s R "too many basic types"
      st := u
    else
      if bt != -1 || (st != -1 && u != VT_INT) then tcc_error_s R "too many basic types"
      if (t &&& VT_DEFSIGN != 0) && (u == VT_VOID || u > VT_LLONG) then tcc_error_s R "too many basic types"
      bt := u
    if u != VT_INT then t := (t &&& ~~~(VT_BTYPE ||| VT_LONG)) ||| u
    return (t, bt, st)
  for _ in [0:fuel] do
    let tok ← getTok
    let mut theEnd := false
    let mut cont := false     -- `continue` (no type_found = 1)
    let mut u : Int32 := 0
    let mut basic := false    -- goto basic_type (next + basic_type1)
    let mut basic1' := false  -- goto basic_type1 (no next)
    if tok == TOK_EXTENSION then
      R.next; cont := true
    else if tok == TOK_CHAR then u := VT_BYTE; basic := true
    else if tok == TOK_VOID then u := VT_VOID; basic := true
    else if tok == TOK_SHORT then u := VT_SHORT; basic := true
    else if tok == TOK_INT then u := VT_INT; basic := true
    else if tok == TOK_ALIGNAS then
      R.next
      skip R 40
      let (f1, type1, ad1) ← R.parse_btype {} 0
      let mut n : Int32 := 0
      if f1 then
        let (type1', ad1', _) ← R.type_decl type1 ad1 TYPE_ABSTRACT
        if ad1'.a &&& 0x1f != 0 then n := (1 : Int32) <<< ((ad1'.a &&& 0x1f).toInt32' - 1)
        else n := (← R.type_size type1').2
      else
        n ← R.expr_const
        if n < 0 || (n &&& (n - 1)) != 0 then tcc_error_s R "alignment must be a positive power of two"
      skip R 41
      ad := setA ad 0 5 (exact_log2p1 n).toUInt16'
      cont := true
    else if tok == TOK_LONG then
      if t &&& VT_BTYPE == VT_DOUBLE then
        t := (t &&& ~~~(VT_BTYPE ||| VT_LONG)) ||| VT_LDOUBLE
        R.next
      else if (t &&& (VT_BTYPE ||| VT_LONG)) == VT_LONG then
        t := (t &&& ~~~(VT_BTYPE ||| VT_LONG)) ||| VT_LLONG
        R.next
      else
        u := VT_LONG; basic := true
    else if tok == TOK_BOOL then u := VT_BOOL; basic := true
    else if tok == TOK_COMPLEX then tcc_error_s R "_Complex is not yet supported"
    else if tok == TOK_FLOAT then u := VT_FLOAT; basic := true
    else if tok == TOK_DOUBLE then
      if (t &&& (VT_BTYPE ||| VT_LONG)) == VT_LONG then
        t := (t &&& ~~~(VT_BTYPE ||| VT_LONG)) ||| VT_LDOUBLE
        R.next
      else
        u := VT_DOUBLE; basic := true
    else if tok == TOK_ENUM || tok == TOK_STRUCT || tok == TOK_UNION then
      let type1 ← struct_decl R (if tok == TOK_ENUM then VT_ENUM else if tok == TOK_STRUCT then VT_STRUCT else VT_UNION)
      u := type1.t
      type := { type with ref := type1.ref }
      basic1' := true
    else if tok == TOK__Atomic then
      R.next
      type := { type with t := t }
      type ← parse_btype_qualify R type VT_ATOMIC
      t := type.t
      if (← getTok) == 40 then
        let mut type1 ← R.parse_expr_type
        type1 := { type1 with t := type1.t &&& ~~~(VT_STORAGE &&& ~~~VT_TYPEDEF) }
        if type1.ref != 0 then ad := sym_to_attr ad (← getSym type1.ref)
        u := type1.t
        type := { type with ref := type1.ref }
        basic1' := true
    else if tok == TOK_CONST1 || tok == TOK_CONST2 || tok == TOK_CONST3 then
      type := { type with t := t }
      type ← parse_btype_qualify R type VT_CONSTANT
      t := type.t
      R.next
    else if tok == TOK_VOLATILE1 || tok == TOK_VOLATILE2 || tok == TOK_VOLATILE3 then
      type := { type with t := t }
      type ← parse_btype_qualify R type VT_VOLATILE
      t := type.t
      R.next
    else if tok == TOK_SIGNED1 || tok == TOK_SIGNED2 || tok == TOK_SIGNED3 then
      if (t &&& (VT_DEFSIGN ||| VT_UNSIGNED)) == (VT_DEFSIGN ||| VT_UNSIGNED) then
        tcc_error_s R "signed and unsigned modifier"
      t := t ||| VT_DEFSIGN
      R.next
      typespec_found := true
    else if tok == TOK_RESTRICT1 || tok == TOK_RESTRICT2 || tok == TOK_RESTRICT3 then
      t := t ||| VT_RESTRICT
      R.next
    else if tok == TOK_REGISTER || tok == TOK_AUTO then
      R.next
    else if tok == TOK_UNSIGNED then
      if (t &&& (VT_DEFSIGN ||| VT_UNSIGNED)) == VT_DEFSIGN then
        tcc_error_s R "signed and unsigned modifier"
      t := t ||| VT_DEFSIGN ||| VT_UNSIGNED
      R.next
      typespec_found := true
    else if tok == TOK_EXTERN || tok == TOK_STATIC || tok == TOK_TYPEDEF then
      let g := if tok == TOK_EXTERN then VT_EXTERN else if tok == TOK_STATIC then VT_STATIC else VT_TYPEDEF
      if t &&& (VT_EXTERN ||| VT_STATIC ||| VT_TYPEDEF) &&& ~~~g != 0 then
        tcc_error_s R "multiple storage classes"
      t := t ||| g
      R.next
    else if tok == TOK_INLINE1 || tok == TOK_INLINE2 || tok == TOK_INLINE3 then
      t := t ||| VT_INLINE
      R.next
    else if tok == TOK_NORETURN3 then
      R.next
      ad := setF ad 5 1 1
    else if tok == TOK_ATTRIBUTE1 || tok == TOK_ATTRIBUTE2 then
      ad ← parse_attribute R ad
      if ad.attr_mode != 0 then
        let u' := ad.attr_mode - 1
        t := (t &&& ~~~(VT_BTYPE ||| VT_LONG)) ||| u'
      cont := true
    else if tok == TOK_TYPEOF1 || tok == TOK_TYPEOF2 || tok == TOK_TYPEOF3 then
      R.next
      let mut type1 ← R.parse_expr_type
      type1 := { type1 with t := type1.t &&& ~~~(VT_STORAGE &&& ~~~VT_TYPEDEF) }
      if type1.ref != 0 then
        ad := sym_to_attr ad (← getSym type1.ref)
        if type1.t &&& VT_ARRAY != 0 then type1 := { type1 with t := type1.t ||| VT_BT_ARRAY }
      u := type1.t
      type := { type with ref := type1.ref }
      basic1' := true
    else if tok == TOK_THREAD_LOCAL || tok == TOK___thread then
      if t &&& VT_TLS != 0 then tcc_error_s R "multiple thread-local storage specifiers"
      t := t ||| VT_TLS
      R.next
    else
      if typespec_found then theEnd := true
      else
        let s ← sym_find tok
        if s == 0 || (← getSym s).type.t &&& VT_TYPEDEF == 0 then theEnd := true
        else
          let n := tok
          R.next
          if (← getTok) == 58 && ignore_label != 0 then
            unget_tok n
            theEnd := true
          else
            let sx ← getSym s
            t := t &&& ~~~(VT_BTYPE ||| VT_LONG)
            let uu := t &&& ~~~VT_QUAL
            t := t ^^^ uu
            type := { t := (sx.type.t &&& ~~~VT_TYPEDEF) ||| uu, ref := sx.type.ref }
            if t != 0 then type ← parse_btype_qualify R type t
            t := type.t
            if t &&& VT_ARRAY != 0 then t := t ||| VT_BT_ARRAY
            ad := sym_to_attr ad sx
            typespec_found := true
            st := -2; bt := -2
    if theEnd then break
    if cont then continue
    if basic then
      R.next
      basic1' := true
    if basic1' then
      let (t', bt', st') ← basic1 u t bt st
      t := t'; bt := bt'; st := st'
      typespec_found := true
    type_found := true
  -- the_end:
  if (← get).char_is_unsigned != 0 then
    if (t &&& (VT_DEFSIGN ||| VT_BTYPE)) == VT_BYTE then t := t ||| VT_UNSIGNED
  let bt2 := t &&& (VT_BTYPE ||| VT_LONG)
  if bt2 == VT_LONG then t := t ||| VT_LLONG
  type := { type with t := t }
  check_restrict_type R type
  return (type_found, type, ad)

/-- `convert_parameter_type(pt)` -/
def convert_parameter_type (R : Rec) (pt0 : CType) : M CType := do
  let pt := { pt0 with t := pt0.t &&& ~~~(VT_ARRAY ||| VT_VLA) }
  if pt.t &&& VT_BTYPE == VT_FUNC then mk_pointer R pt else return pt

/-- `convert_expression_type(pt)` -/
def convert_expression_type (R : Rec) (pt : CType) : M CType :=
  convert_parameter_type R { pt with t := pt.t &&& ~~~VT_QUAL }

/-- `parse_asm_str()` -/
def parse_asm_str (R : Rec) : M ByteArray := do
  skip R 40
  parse_mult_str R "string constant"

/-- `asm_label_instr()` -/
def asm_label_instr (R : Rec) : M Int32 := do
  R.next
  let astr ← parse_asm_str R
  skip R 41
  return (← tok_alloc R (Bytes.cstrAt astr 0)).tok

/-! ## declarators -/

/-- where a declarator's `CType *` points: the caller's type, or the
type field of a Sym (an innermost pointed-to type) -/
inductive CLoc where
  | top
  | sym (s : SymId)
  deriving BEq, Inhabited

def locGet (top : CType) : CLoc → M CType
  | .top => pure top
  | .sym s => do return (← getSym s).type

def locSet (top : CType) (l : CLoc) (v : CType) : M CType :=
  match l with
  | .top => pure v
  | .sym s => do modSym s fun y => { y with type := v }; return top

/-- `post_type(type, ad, storage, td)`: (result, type, ad) -/
def post_type : Nat → Rec → CType → AttributeDef → Int32 → Int32 → M (Bool × CType × AttributeDef)
  | 0, _, _, _, _, _ => throw (.fuel "post_type")
  | fuel+1, R, type0, ad0, storage, td => do
    let mut type := type0
    let mut ad := ad0
    let lfuel ← fuelM
    if (← getTok) == 40 then
      R.next
      if TYPE_DIRECT == (td &&& (TYPE_DIRECT ||| TYPE_ABSTRACT)) then return (false, type, ad)
      let isLocal := (← get).local_stack != 0
      modify fun s => { s with local_scope := s.local_scope + 1 }
      let st0 ← get
      let top := if isLocal then st0.local_stack else st0.global_stack
      let sr ← sym_push2 top SYM_FIELD 0 0
      if isLocal then modify fun s => { s with local_stack := sr } else modify fun s => { s with global_stack := sr }
      let mut l : UInt32 := 0
      let mut pt : CType := {}
      let mut ad1 : AttributeDef := {}
      if (← getTok) == 41 then l := 0
      else
        let (f, pt', ad1') ← R.parse_btype {} 0
        pt := pt'; ad1 := ad1'
        if f then l := FUNC_NEW
        else if td &&& (TYPE_DIRECT ||| TYPE_ABSTRACT) != 0 then
          let srPrev := (← getSym sr).prev
          let st0 ← get
          let cur := if isLocal then st0.local_stack else st0.global_stack
          let nt ← sym_pop cur srPrev false
          if isLocal then modify fun s => { s with local_stack := nt } else modify fun s => { s with global_stack := nt }
          modify fun s => { s with local_scope := s.local_scope - 1 }
          ad := merge_attr ad ad1
          return (false, type, ad)
        else l := FUNC_OLD
      let mut first : SymId := 0
      let mut last : SymId := 0
      let mut arg_size : Int32 := 0
      if l != 0 then
        for _ in [0:lfuel] do
          let mut n : Int32 := 0
          if l != FUNC_OLD then
            if pt.t &&& VT_BTYPE == VT_VOID && (← getTok) == 41 then break
            let (pt', ad1', n') ← R.type_decl pt ad1 (TYPE_DIRECT ||| TYPE_ABSTRACT ||| TYPE_PARAM)
            pt := pt'; ad1 := ad1'; n := n'
            if pt.t &&& VT_BTYPE == VT_VOID then tcc_error_s R "parameter declared as void"
            if n == 0 || td &&& TYPE_PARAM != 0 then n := n ||| SYM_FIELD
          else
            n ← getTok
            pt := { t := VT_INT ||| VT_EXTERN, ref := 0 }
            R.next
          if n < C.TOK_UIDENT then expect R "identifier"
          pt ← convert_parameter_type R pt
          arg_size := arg_size + ((← R.type_size pt).1 + 8 - 1) / 8
          let s ← sym_push R n pt (VT_LOCAL ||| VT_LVAL) 0
          if last == 0 then first := s else modSym last fun y => { y with next := s }
          last := s
          if (← getTok) == 41 then break
          skip R 44
          if l == FUNC_NEW && (← getTok) == TOK_DOTS then
            l := FUNC_ELLIPSIS
            R.next
            break
          if l == FUNC_NEW then
            let (f, pt', ad1') ← R.parse_btype {} 0
            pt := pt'; ad1 := ad1'
            if !f then tcc_error_s R "invalid type"
      else
        l := FUNC_OLD
      skip R 41
      type := { type with t := type.t &&& ~~~VT_QUAL }
      if (← getTok) == 91 then
        R.next
        skip R 93
        type ← mk_pointer R type
      ad := setF ad 8 8 arg_size.toUInt32
      ad := setF ad 3 2 l
      let ty := type
      let aa := ad.a
      let ff := ad.f
      let fs := first
      modSym sr fun y => { y with type := ty, a := aa, aux := ff, next := fs }
      type := { t := VT_FUNC, ref := sr }
      let st0 ← get
      let cur := if isLocal then st0.local_stack else st0.global_stack
      let _ ← sym_pop cur sr true
      modify fun s => { s with local_scope := s.local_scope - 1 }
    else if (← getTok) == 91 then
      let saved_nocode_wanted := (← get).nocode_wanted
      let mut array_qualifiers : Int32 := 0
      R.next
      let mut n : Int32 := -1
      let mut t1 : Int32 := 0
      let mut vla_array_str : Option TokArr := none
      let mut doCheck := false
      if td &&& TYPE_PARAM != 0 then
        for _ in [0:lfuel] do
          let tk ← getTok
          if tk == TOK_CONST1 || tk == TOK_CONST2 || tk == TOK_CONST3 then
            array_qualifiers := array_qualifiers ||| VT_CONSTANT; R.next
          else if tk == TOK_VOLATILE1 || tk == TOK_VOLATILE2 || tk == TOK_VOLATILE3 then
            array_qualifiers := array_qualifiers ||| VT_VOLATILE; R.next
          else if tk == TOK_RESTRICT1 || tk == TOK_RESTRICT2 || tk == TOK_RESTRICT3 then
            array_qualifiers := array_qualifiers ||| VT_RESTRICT; R.next
          else if tk == TOK_STATIC || tk == 42 then R.next
          else break
        if (← getTok) != 93 then
          modify fun s => { s with nocode_wanted := 1 }
          let toks ← R.skip_or_save_block
          unget_tok 0
          let arr := toks.getD #[]
          vla_array_str := toks
          let _ ← begin_macro .heap arr 0 2
          R.next
          R.gexpr
          end_macro
          R.next
          doCheck := true
      else if (← getTok) != 93 then
        let st ← get
        if st.local_stack == 0 || storage &&& VT_STATIC != 0 then
          vpushi R (← R.expr_const)
        else
          modify fun s => { s with nocode_wanted := 0 }
          R.gexpr
        doCheck := true
      if doCheck then
        if ((← vtr) &&& (VT_VALMASK ||| VT_LVAL ||| VT_SYM)) == VT_CONST then
          n := (← vtci).toUInt32.toInt32
          if n < 0 then tcc_error_s R "invalid array size"
        else
          if !is_integer_btype ((← vtt) &&& VT_BTYPE) then
            tcc_error_s R "size of variable length array should be an integer"
          n := 0
          t1 := VT_VLA
      skip R 93
      let (_, type', ad') ← post_type fuel R type ad storage ((td &&& ~~~(TYPE_DIRECT ||| TYPE_ABSTRACT)) ||| TYPE_NEST)
      type := type'; ad := ad'
      if type.t &&& VT_BTYPE == VT_FUNC then tcc_error_s R "declaration of an array of functions"
      if type.t &&& VT_BTYPE == VT_VOID || (← R.type_size type).1 < 0 then
        tcc_error_s R "declaration of an array of incomplete type elements"
      t1 := t1 ||| (type.t &&& VT_VLA)
      if t1 &&& VT_VLA != 0 then
        if n < 0 then
          if td &&& TYPE_NEST != 0 then tcc_error_s R "need explicit inner array size in VLAs"
        else
          let (isz, ial) ← R.type_size (← get).int_type
          modify fun s => { s with loc := (s.loc - isz) &&& (-ial) }
          n := (← get).loc
          let _ ← R.vpush_type_size type
          R.gen_op 42
          vset R (← get).int_type (VT_LOCAL ||| VT_LVAL) n
          vswap R
          R.vstore
      if n != -1 then vpop R
      modify fun s => { s with nocode_wanted := saved_nocode_wanted }
      let s ← sym_push R SYM_FIELD type 0 n
      type := { t := (if t1 != 0 then VT_VLA else VT_ARRAY) ||| VT_PTR, ref := s }
      type := { type with t := type.t ||| array_qualifiers }
      match vla_array_str with
      | some vs =>
        if t1 &&& VT_VLA != 0 && td &&& TYPE_NEST != 0 then
          modSym s fun y => { y with vla_array_str := some vs }
      | none => pure ()
    return (true, type, ad)

/-- `type_decl(type, ad, &v, td)` with the returned `CType *` as a location:
(type, ad, v, ret) -/
def type_decl_loc : Nat → Rec → CType → AttributeDef → Int32 → M (CType × AttributeDef × Int32 × CLoc)
  | 0, _, _, _, _ => throw (.fuel "type_decl")
  | fuel+1, R, type0, ad0, td => do
    let mut type := type0
    let mut ad := ad0
    let storage := type.t &&& VT_STORAGE
    type := { type with t := type.t &&& ~~~VT_STORAGE }
    let mut post : CLoc := .top
    let mut ret : CLoc := .top
    let lfuel ← fuelM
    for _ in [0:lfuel] do
      if (← getTok) != 42 then break
      let mut qualifiers : Int32 := 0
      for _ in [0:lfuel] do    -- redo:
        R.next
        let tk ← getTok
        if tk == TOK__Atomic then qualifiers := qualifiers ||| VT_ATOMIC
        else if tk == TOK_CONST1 || tk == TOK_CONST2 || tk == TOK_CONST3 then qualifiers := qualifiers ||| VT_CONSTANT
        else if tk == TOK_VOLATILE1 || tk == TOK_VOLATILE2 || tk == TOK_VOLATILE3 then qualifiers := qualifiers ||| VT_VOLATILE
        else if tk == TOK_RESTRICT1 || tk == TOK_RESTRICT2 || tk == TOK_RESTRICT3 then qualifiers := qualifiers ||| VT_RESTRICT
        else
          if tk == TOK_ATTRIBUTE1 || tk == TOK_ATTRIBUTE2 then ad ← parse_attribute R ad
          break
      type ← mk_pointer R type
      type := { type with t := type.t ||| qualifiers }
      if ret == .top then ret := .sym type.ref
    let mut v : Int32 := 0
    let mut abstract_ := false
    if (← getTok) == 40 then
      let (r, type', ad') ← post_type lfuel R type ad 0 td
      type := type'; ad := ad'
      if !r then
        ad ← parse_attribute R ad
        let (type'', ad'', v', post') ← type_decl_loc fuel R type ad td
        type := type''; ad := ad''; v := v'; post := post'
        skip R 41
      else
        abstract_ := true
    else if (← getTok) ≥ C.TOK_IDENT && td &&& TYPE_DIRECT != 0 then
      v ← getTok
      R.next
    else
      abstract_ := true
    if abstract_ then
      if td &&& TYPE_ABSTRACT == 0 then expect R "identifier"
      v := 0
    let pv ← locGet type post
    let (_, pv', ad') ← post_type lfuel R pv ad (if post != ret then 0 else storage) (td &&& ~~~(TYPE_DIRECT ||| TYPE_ABSTRACT))
    ad := ad'
    type ← locSet type post pv'
    check_restrict_type R type
    ad ← parse_attribute R ad
    type := { type with t := type.t ||| storage }
    return (type, ad, v, ret)

/-- `type_decl(type, ad, &v, td)`: (type, ad, v) -/
def type_decl (R : Rec) (type : CType) (ad : AttributeDef) (td : Int32) : M (CType × AttributeDef × Int32) := do
  let (t, a, v, _) ← type_decl_loc (← fuelM) R type ad td
  return (t, a, v)

/-! ## indirection, arguments, expression types -/

/-- `indir()` -/
def indir (R : Rec) : M Unit := do
  if (← vtt) &&& VT_BTYPE != VT_PTR then
    if (← vtt) &&& VT_BTYPE == VT_FUNC then return
    expect R "pointer"
  if (← vtr) &&& VT_LVAL != 0 then
    let _ ← R.gv RC_INT
  setVtType 0 (← pointed_type (← vt).type)
  let t ← vtt
  if t &&& (VT_ARRAY ||| VT_VLA) == 0 && t &&& VT_BTYPE != VT_FUNC then
    modVt 0 fun v => { v with r := v.r ||| VT_LVAL.toUInt16' }

/-- `gfunc_param_typed(func, arg)` -/
def gfunc_param_typed (R : Rec) (func arg : SymId) : M Unit := do
  let func_type := (← getSym func).f_func_type
  if func_type == FUNC_OLD || (func_type == FUNC_ELLIPSIS && arg == 0) then
    if (← vtt) &&& VT_BTYPE == VT_FLOAT then
      R.gen_cast_s VT_DOUBLE
    else if (← vtt) &&& VT_BITFIELD != 0 then
      let v ← vt
      R.gen_cast { t := v.type.t &&& (VT_BTYPE ||| VT_UNSIGNED), ref := v.type.ref }
    else if (← vtr) &&& VT_MUSTCAST != 0 then
      force_charshort_cast R
  else if arg == 0 then
    tcc_error_s R "too many arguments to function"
  else
    let aty := (← getSym arg).type
    gen_assign_cast R { aty with t := aty.t &&& ~~~VT_CONSTANT }

/-- `expr_type(type, expr_fn)` -/
def expr_type (expr_fn : M Unit) : M CType := do
  modify fun s => { s with nocode_wanted := s.nocode_wanted + 1 }
  expr_fn
  let t := (← vt).type
  vtopDec  -- vpop() on a non-CMP value at nocode
  modify fun s => { s with nocode_wanted := s.nocode_wanted - 1 }
  return t

/-- `parse_expr_type(type)` -/
def parse_expr_type (R : Rec) : M CType := do
  skip R 40
  let (f, type, ad) ← R.parse_btype {} 0
  let mut ty := type
  if f then
    let (t', _, _) ← R.type_decl type ad TYPE_ABSTRACT
    ty := t'
  else
    modify fun s => { s with nocode_wanted := s.nocode_wanted + 1 }
    R.gexpr
    ty := (← vt).type
    vpop R
    modify fun s => { s with nocode_wanted := s.nocode_wanted - 1 }
  skip R 41
  return ty

/-- `parse_type(type)` -/
def parse_type (R : Rec) : M CType := do
  let (f, type, ad) ← R.parse_btype {} 0
  if !f then expect R "type"
  let (t', _, _) ← R.type_decl type ad TYPE_ABSTRACT
  return t'

/-- `parse_builtin_params(nc, args)` -/
def parse_builtin_params (R : Rec) (nc : Bool) (args : String) : M Unit := do
  if nc then modify fun s => { s with nocode_wanted := s.nocode_wanted + 1 }
  R.next
  let mut sep : Int32 := 40
  if args.isEmpty then skip R sep
  for c in args.toList do
    skip R sep
    sep := 44
    if c == 't' then
      let type ← parse_type R
      vpush R type
      continue
    R.expr_eq
    let mut type : CType := { t := 0, ref := 0 }
    if c == 'e' then continue
    else if c == 'V' || c == 'v' then
      type := { type with t := (if c == 'V' then VT_CONSTANT else 0) ||| VT_VOID }
      type ← mk_pointer R type
    else if c == 'S' || c == 's' then
      type := { type with t := (if c == 'S' then VT_CONSTANT else 0) ||| (← get).char_type.t }
      type ← mk_pointer R type
    else if c == 'i' then type := { type with t := VT_INT }
    else if c == 'l' then type := { type with t := VT_SIZE_T }
    gen_assign_cast R type
  skip R 41
  if nc then modify fun s => { s with nocode_wanted := s.nocode_wanted - 1 }

/-- `PUT_R_RET(sv, t)` on vtop -/
def PUT_R_RET_vtop (t : Int32) : M Unit :=
  modVt 0 fun v => { v with r := (R_RET t).toUInt16', r2 := (R2_RET t).toUInt16' }

/-- `parse_atomic(atok)` -/
def parse_atomic (R : Rec) (atok : Int32) : M Unit := do
  let templates : Array String := #["alm.?", "Asm.v", "alsm.v", "aplbmm.b", "avm.v", "avm.v", "avm.v", "avm.v",
    "avm.v", "avm.v", "avm.v", "avm.v", "avm.v", "avm.v", "avm.v", "avm.v"]
  let template := (templates[(atok - TOK___atomic_store).toNat']!).toList.toArray
  let mut atom : CType := {}
  let mut atom_ptr : CType := {}
  let mut size : Int32 := 0
  let mut save := false
  let mut store : SValue := {}
  R.next
  skip R 40
  let mut arg := 0
  let fuel ← fuelM
  for _ in [0:fuel] do
    R.expr_eq
    let tc := template[arg]!
    if tc == 'a' || tc == 'A' then
      atom_ptr := (← vt).type
      if atom_ptr.t &&& VT_BTYPE != VT_PTR then expect R "pointer"
      atom ← pointed_type atom_ptr
      size := (← R.type_size atom).1
      if size > 8 || (size &&& (size - 1)) != 0 ||
         (atok > TOK___atomic_compare_exchange && (btype_size (atom.t &&& VT_BTYPE) == 0 || atom.t &&& VT_BTYPE == VT_PTR)) then
        expect R "integral or integer-sized pointer target type"
    else if tc == 'p' then
      if (← vtt) &&& VT_BTYPE != VT_PTR || (← R.type_size (← pointed_type (← vt).type)).1 != size then
        tcc_error R (cfmt "pointer target type mismatch in argument %d" [.i (arg + 1 : Nat)])
      gen_assign_cast R atom_ptr
    else if tc == 'v' then gen_assign_cast R atom
    else if tc == 'l' then
      indir R
      gen_assign_cast R atom
    else if tc == 's' then
      save := true
      indir R
      store ← vt
      vpop R
    else if tc == 'm' then gen_assign_cast R (← get).int_type
    else if tc == 'b' then gen_assign_cast R { t := VT_BOOL }
    arg := arg + 1
    if template[arg]! == '.' then break
    skip R 44
  skip R 41
  let mut ct : CType := { t := VT_VOID }
  let rc := template[arg + 1]!
  if rc == 'b' then ct := { t := VT_BOOL }
  else if rc == 'v' then ct := atom
  let buf := cfmt "%s_%d" [.s (← get_tok_str atok none), .i size.toInt]
  vpush_helper_func R (← tok_alloc R buf).tok
  vrott R (Int32.ofNat arg - b2i save + 1)
  R.gfunc_call (Int32.ofNat arg - b2i save)
  vpush R ct
  PUT_R_RET_vtop ct.t
  let t := ct.t &&& VT_BTYPE
  if t == VT_BYTE || t == VT_SHORT || t == VT_BOOL then
    modVt 0 fun v => { v with r := (v.r.toInt32' ||| BFVAL_MUSTCAST 1).toUInt16' }
  R.gen_cast ct
  if save then
    vpush R ct
    setVt 0 store
    vswap R
    R.vstore

end Tcc.Model
