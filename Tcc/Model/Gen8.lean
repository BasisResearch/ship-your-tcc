import Tcc.Model.Gen7

/-!
# TccModel: tccgen.c, part 8 — allocation, functions, `decl`, the driver
-/

namespace Tcc.Model
open C
open Tok

/-- the current value of decl_initializer_alloc's `type` (the global symbol's type, or the caller's) -/
def curT (tloc : Option SymId) (tval : CType) : M CType :=
  match tloc with
  | some s => do return (← getSym s).type
  | none => pure tval

/-- `decl_initializer_alloc(type, ad, r, has_init, v, scope)` -/
def decl_initializer_alloc (R : Rec) (type0 : CType) (ad : AttributeDef) (r0 : Int32) (has_init : Int32)
    (v : Int32) (scope : Int32) : M Unit := do
  let mut r := r0
  let saved_nocode_wanted := (← get).nocode_wanted
  let mut p : InitP := {}
  -- `type` is the caller's CType, or the existing global symbol's type
  let mut tloc : Option SymId := none
  let mut tval := type0
  let mut sym : SymId := 0
  let fuel ← fuelM
  if scope == VT_CONST then
    sym ← sym_find v
    if sym != 0 then
      patch_storage R sym ad (some tval)
      let sx ← getSym sym
      if has_init == 0 && sx.c != 0 && (← getElfSym (← get).symtab_section sx.c.toNat').st_shndx.toUInt32.toInt32 != SHN_UNDEF then
        return
      tloc := some sym
      tval := sx.type
  if v != 0 && (r &&& VT_VALMASK) == VT_CONST then
    modify fun s => { s with nocode_wanted := s.nocode_wanted ||| DATA_ONLY_WANTED }
  let mut flexible_array : SymId := 0
  let (size0, align0) ← R.type_size (← curT tloc tval)
  let mut size := size0
  let mut align := align0
  let mut init_str : Option TokArr := none
  if size < 0 then
    let t ← curT tloc tval
    if t.t &&& VT_ARRAY == 0 then tcc_error_s R "initialization of incomplete type"
    if IS_BT_ARRAY t.t then
      let rx ← getSym t.ref
      let nr ← sym_push R SYM_FIELD rx.type 0 rx.c
      match tloc with
      | some s => modSym s fun y => { y with type := { y.type with ref := nr } }
      | none => tval := { tval with ref := nr }
    p := { p with flex_array_ref := (← curT tloc tval).ref }
  else if has_init != 0 && (← curT tloc tval).t &&& VT_BTYPE == VT_STRUCT then
    let mut field := (← getSym (← curT tloc tval).ref).next
    if field != 0 then
      for _ in [0:fuel] do
        let nx := (← getSym field).next
        if nx == 0 then break
        field := nx
      let fx ← getSym field
      if fx.type.t &&& VT_ARRAY != 0 && (← symC fx.type.ref) < 0 then
        flexible_array := field
        p := { p with flex_array_ref := fx.type.ref }
        size := -1
  if size < 0 then
    if has_init == 0 then tcc_error_s R "unknown type size"
    if has_init == 2 then
      let mut str : TokenString := {}
      for _ in [0:fuel] do
        let tk ← getTok
        if !(TOK_IS_STR tk) then break
        str ← tok_str_add_tok str
        R.next
      str := tok_str_add str C.TOK_EOF
      init_str := some (str.str.extract 0 str.len.toNat')
    else
      init_str ← R.skip_or_save_block
    unget_tok 0
    let toks := init_str.getD #[]
    let _ ← begin_macro .heap toks 0 1
    R.next
    let gmd ← gmark
    R.decl_initializer p.sec p.local_offset p.flex_array_ref (← curT tloc tval) 0 (DIF_FIRST ||| DIF_SIZE_ONLY)
    gdrop gmd    -- the dry run's syntax is parsed again below
    -- prepare second initializer parsing
    modify fun s => { s with macro_ptr := some { arr := toks, pos := 0 } }
    R.next
    let (sz, al) ← R.type_size (← curT tloc tval)
    size := sz; align := al
    if size < 0 then tcc_error_s R "unknown type size"
    if flexible_array != 0 then
      let fa ← getSym flexible_array
      let fac ← symC fa.type.ref
      if fac > 0 then
        size := size + fac * (← pointed_size R fa.type)
  if ad.a &&& 0x1f != 0 then
    let speca : Int32 := (1 : Int32) <<< ((ad.a &&& 0x1f).toInt32' - 1)
    if speca > align then align := speca
  else if (ad.a >>> 5) &&& 1 != 0 then
    align := 1
  if v == 0 && (← NODATA_WANTED) then
    size := 0; align := 1
  let mut sec : SecId := 0
  let mut addr : Int32 := 0
  if (r &&& VT_VALMASK) == VT_LOCAL then
    sec := 0
    modify fun s => { s with loc := (s.loc - size) &&& (-align) }
    addr := (← get).loc
    p := { p with local_offset := addr + size }
    if v != 0 then
      sym ← sym_push R v (← curT tloc tval) r addr
      if ad.cleanup_func != 0 then
        let cs := (← get).cur_scope
        modScope cs fun x => { x with cl_n := x.cl_n + 1 }
        let n := (← getScope cs).cl_n
        let cls ← sym_push2 (← get).all_cleanups (SYM_FIELD ||| n) 0 0
        modify fun s => { s with all_cleanups := cls }
        let sym' := sym
        let ncl := (← getScope cs).cl_s
        modSym cls fun y => { y with prev_tok := sym', cleanup_func := ad.cleanup_func, next := ncl }
        modScope cs fun x => { x with cl_s := cls }
      let aa := ad.a
      let sym' := sym
      modSym sym' fun y => { y with a := aa }
    else
      vset R (← curT tloc tval) r addr
  else
    let mut tp ← curT tloc tval
    for _ in [0:fuel] do
      if (tp.t &&& (VT_BTYPE ||| VT_ARRAY)) != (VT_PTR ||| VT_ARRAY) then break
      tp := (← getSym tp.ref).type
    let is_const := tp.t &&& VT_CONSTANT != 0
    sec := ad.sec
    let t ← curT tloc tval
    if sec == 0 then
      if t.t &&& VT_TLS != 0 then
        sec ← find_section (if has_init != 0 then ".tdata" else ".tbss").toUTF8
        let s' := sec
        modSec s' fun x => { x with sh_flags := SHF_ALLOC ||| SHF_WRITE ||| SHF_TLS,
                                    sh_type := if has_init != 0 then SHT_PROGBITS else SHT_NOBITS }
      else if is_const then sec := (← get).rodata_section
      else if has_init != 0 then sec := (← get).data_section
      else if (← get).nocommon != 0 then sec := (← get).bss_section
    else if ad.new_section != 0 then
      if !is_const then
        let s' := sec
        modSec s' fun x => { x with sh_flags := x.sh_flags ||| SHF_WRITE,
                                    sh_type := if has_init == 0 then SHT_NOBITS else x.sh_type }
    if sec != 0 then
      addr := Int32.ofNat (← section_add sec size.toInt64.toUInt64 align)
    else
      addr := align
      sec := (← get).common_section
    if v != 0 then
      if sym == 0 then
        sym ← sym_push R v (← curT tloc tval) (r ||| VT_SYM) 0
        patch_storage R sym ad none
      put_extern_sym sym sec addr.toInt64.toUInt64 size.toInt64.toUInt64
    else
      vpush_ref R (← curT tloc tval) sec addr.toInt64.toUInt64 size.toInt64.toUInt64
      sym := (← vt).sym
      modVt 0 fun x => { x with r := x.r ||| r.toUInt16' }
  let t ← curT tloc tval
  if t.t &&& VT_VLA != 0 then
    if has_init != 0 then tcc_error_s R "variable length array cannot be initialized"
    if !(← NODATA_WANTED) then
      let cs := (← get).cur_scope
      let csx ← getScope cs
      if csx.vla_num == 0 then
        if csx.prev != 0 && (← getScope csx.prev).vla_num != 0 then
          let pl := (← getScope csx.prev).vla_loc
          modScope cs fun x => { x with vla_locorig := pl }
        else
          modify fun s => { s with loc := s.loc - 8 }
          gen_vla_sp_save (← get).loc
          let l := (← get).loc
          modScope cs fun x => { x with vla_locorig := l }
      let a ← R.vpush_type_size t
      gen_vla_alloc R t a
      gen_vla_sp_save addr
      let addr' := addr
      modScope cs fun x => { x with vla_loc := addr', vla_num := x.vla_num + 1 }
  else if has_init != 0 then
    p := { p with sec := sec }
    let saved_unions := (← get).init_unions
    modify fun s => { s with init_unions := #[] }
    R.decl_initializer p.sec p.local_offset p.flex_array_ref t addr.toInt64.toUInt64 DIF_FIRST
    modify fun s => { s with init_unions := saved_unions }
    if flexible_array != 0 then
      let fr := (← getSym flexible_array).type.ref
      modSym fr fun y => { y with c := -1 }
  -- no_alloc:
  if init_str.isSome then
    end_macro
    R.next
  modify fun s => { s with nocode_wanted := saved_nocode_wanted }

/-- `func_vla_arg_code(arg)` -/
def func_vla_arg_code : Nat → Rec → SymId → M Unit
  | 0, _, _ => throw (.fuel "func_vla_arg_code")
  | fuel+1, R, arg => do
    let ax ← getSym arg
    if ax.type.ref != 0 then func_vla_arg_code fuel R ax.type.ref
    let ax ← getSym arg
    if ax.type.t &&& VT_VLA != 0 then
      let rx ← getSym ax.type.ref
      match rx.vla_array_str with
      | some vs =>
        let (isz, ial) ← R.type_size (← get).int_type
        modify fun s => { s with loc := (s.loc - isz) &&& (-ial) }
        let l := (← get).loc
        modSym ax.type.ref fun y => { y with c := l }
        unget_tok 0
        let _ ← begin_macro .heap vs 0 1
        R.next
        R.gexpr
        end_macro
        R.next
        let _ ← R.vpush_type_size rx.type
        R.gen_op 42
        vset R (← get).int_type (VT_LOCAL ||| VT_LVAL) l
        vswap R
        R.vstore
        vpop R
      | none => pure ()

/-- `func_vla_arg(sym)` -/
def func_vla_arg (R : Rec) (sym : SymId) : M Unit := do
  let mut arg := (← getSym (← getSym sym).type.ref).next
  let fuel ← fuelM
  for _ in [0:fuel] do
    if arg == 0 then break
    let ax ← getSym arg
    if ax.type.t &&& VT_BTYPE == VT_PTR && (← getSym ax.type.ref).type.t &&& VT_VLA != 0 then
      func_vla_arg_code fuel R ax.type.ref
    arg := ax.next

/-- `gfunc_set_param(s, c, byref)` -/
def gfunc_set_param (s0 : SymId) (c byref : Int32) : M SymId := do
  let s ← sym_find (← getSym s0).v
  if s == 0 then return 0
  modSym s fun y =>
    let y := { y with c := c }
    if byref != 0 then { y with r := (VT_LLOCAL ||| VT_LVAL).toUInt16' } else y
  return s

/-- `sym_push_params(ref)` -/
def sym_push_params (ref : SymId) : M Unit := do
  let mut s := ref
  let fuel ← fuelM
  for _ in [0:fuel] do
    let nx := (← getSym s).next
    if nx == 0 then break
    s := nx
  for _ in [0:fuel] do
    if s == ref then break
    let sx ← getSym s
    if (sx.v &&& ~~~SYM_STRUCT) < SYM_FIRST_ANOM then
      let c ← sym_copy s (← get).local_stack
      modify fun st => { st with local_stack := c }
    s := sx.prev

/-- `add_array(s1, sec, c)` -/
def add_array (sec : String) (c : Int32) : M Unit := do
  let s ← find_section sec.toUTF8
  modSec s fun x => { x with sh_flags := SHF_ALLOC,
                             sh_type := if sec.get ⟨1⟩ == 'i' then SHT_INIT_ARRAY else SHT_FINI_ARRAY }
  put_elf_reloca (← get).symtab_section s (← getSec s).data_offset.toUInt64 R_DATA_PTR c.toNat' 0
  let _ ← section_ptr_add s 8

/-- `gen_function(sym)` -/
def gen_function (R : Rec) (sym : SymId) : M Unit := do
  let gm ← gmark
  let f ← newScopeObj
  modify fun s => { s with cur_scope := f, root_scope := f, nocode_wanted := 0 }
  let cts := (← get).cur_text_section
  modify fun s => { s with ind := 0 }
  let ind0 := Int32.ofNat (← getSec cts).data_offset
  modify fun s => { s with ind := ind0 }
  let sx ← getSym sym
  if sx.a_aligned != 0 then
    let newoff ← section_add cts 0 ((1 : Int32) <<< (sx.a_aligned.toInt32' - 1))
    gen_fill_nops R (Int32.ofNat newoff - ind0)
  let fname ← get_tok_str sx.v none
  let rx ← getSym sx.type.ref
  modify fun s => { s with funcname := fname, func_ind := s.ind, func_vt := rx.type,
                           func_var := b2i (rx.f_func_type == FUNC_ELLIPSIS),
                           func_old := b2i (rx.f_func_type == FUNC_OLD) }
  put_extern_sym sym cts (← get).ind.toInt64.toUInt64 0
  if rx.f_func_ctor != 0 then add_array ".init_array" (← getSym sym).c
  if rx.f_func_dtor != 0 then add_array ".fini_array" (← getSym sym).c
  let ls ← sym_push2 (← get).local_stack SYM_FIELD 0 0
  modify fun s => { s with local_stack := ls, local_scope := 1 }
  sym_push_params sx.type.ref
  modify fun s => { s with local_scope := 0, rsym := 0, nb_temp_local_vars := 0 }
  gfunc_prolog R sym
  func_vla_arg R sym
  R.block 0
  gsym R (← get).rsym
  modify fun s => { s with nocode_wanted := 0 }
  gfunc_epilog
  -- tcc_debug_funcend: the .eh_frame FDE
  let st ← get
  tcc_debug_frame_end (st.ind - st.func_ind)
  let st ← get
  let symtab := st.symtab_section
  let c := (← getSym sym).c.toNat'
  let e ← getElfSym symtab c
  setElfSym symtab c { e with st_size := (st.ind - st.func_ind).toInt64.toUInt64 }
  let indn := st.ind.toNat'
  modSec st.cur_text_section fun x => { x with data_offset := indn }
  let ns ← sym_pop (← get).local_stack 0 false
  modify fun s => { s with local_stack := ns }
  let ng ← label_pop R (← get).global_label_stack 0 false
  modify fun s => { s with global_label_stack := ng }
  let nc ← sym_pop (← get).all_cleanups 0 false
  modify fun s => { s with
    all_cleanups := nc, local_scope := 0, cur_text_section := 0,
    funcname := ByteArray.empty, func_vt := { s.func_vt with
    t := VT_VOID }, func_var := 0, ind := 0,
    func_ind := -1, nocode_wanted := DATA_ONLY_WANTED }
  check_vstack R
  gwrap gm "fundef" #[(← getSym sym).v]
  R.next

/-- `gen_inline_functions(s)` -/
def gen_inline_functions (R : Rec) : M Unit := do
  tcc_open_bf ":inline:".toUTF8 none
  let fuel ← fuelM
  for _ in [0:fuel] do
    let mut inline_generated := false
    let n := (← get).inline_fns.size
    for i in [0:n] do
      let fn := (← get).inline_fns[i]!
      let sym := fn.sym
      if sym != 0 then
        let sx ← getSym sym
        if sx.c != 0 || sx.type.t &&& VT_INLINE == 0 then
          modify fun s => { s with inline_fns := s.inline_fns.modify i fun x => { x with sym := 0 } }
          tccpp_putfile fn.filename
          let _ ← begin_macro .heap (fn.func_str.getD #[]) 0 1
          R.next
          modify fun s => { s with cur_text_section := s.text_section }
          gen_function R sym
          end_macro
          inline_generated := true
    if !inline_generated then break
  tcc_close

/-- `do_Static_assert()` -/
def do_Static_assert (R : Rec) : M Unit := do
  R.next
  skip R 40
  let c ← R.expr_const
  let mut msg := "_Static_assert fail".toUTF8
  if (← getTok) == 44 then
    R.next
    msg := Bytes.cstrAt (← parse_mult_str R "string constant") 0
  skip R 41
  if c == 0 then tcc_error R (cfmt "%s" [.s msg])
  skip R 59

/-- one declarator of `decl(l)`; returns `some ret` for decl's `return ret`,
`none` to continue with the next declarator/declaration (`brk`: break the
inner loop) -/
def decl_one_body (R : Rec) (l : Int32) (btype : CType) (adbase : AttributeDef) (oldint : Bool) :
    M (Option Int32 × Bool) := do
  let mut ad := adbase
  let (type1, ad1, v) ← R.type_decl btype ad (if l == VT_CMP then TYPE_DIRECT ||| TYPE_PARAM else TYPE_DIRECT)
  let mut type := type1
  ad := ad1
  if type.t &&& VT_BTYPE == VT_FUNC then
    if type.t &&& VT_STATIC != 0 && l != VT_CONST then
      tcc_error_s R "function without file scope cannot be static"
    let sym := type.ref
    let sx ← getSym sym
    if sx.f_func_type == FUNC_OLD && l == VT_CONST then
      modify fun s => { s with func_vt := type, local_scope := s.local_scope + 1 }
      let _ ← R.decl VT_CMP
      modify fun s => { s with local_scope := s.local_scope - 1 }
    if (type.t &&& (VT_EXTERN ||| VT_INLINE)) == (VT_EXTERN ||| VT_INLINE) then
      if (← get).gnu89_inline != 0 || (← getSym sym).f_func_alwinl != 0 then
        type := { type with t := (type.t &&& ~~~VT_EXTERN) ||| VT_STATIC }
      else
        type := { type with t := type.t &&& ~~~VT_INLINE }
  else if oldint then
    tcc_warning R "type defaults to int".toUTF8
  let tk ← getTok
  if (← get).gnu_ext != 0 && (tk == TOK_ASM1 || tk == TOK_ASM2 || tk == TOK_ASM3) then
    ad := { ad with asm_label := ← asm_label_instr R }
    ad ← parse_attribute R ad
  if (← getTok) == 123 then
    if l != VT_CONST then tcc_error_s R "cannot use local functions"
    if type.t &&& VT_BTYPE != VT_FUNC then expect R "function definition"
    let adf := ad.f
    modSym type.ref fun y => { y with aux := merge_funcattr y.aux adf }
    type := { type with t := type.t &&& ~~~VT_EXTERN }
    let sym ← external_sym R v type 0 ad
    let srx := (← getSym sym).type.ref
    let mut sa := srx
    let fuel ← fuelM
    for _ in [0:fuel] do
      sa := (← getSym sa).next
      if sa == 0 then break
      let sax ← getSym sa
      if sax.v &&& ~~~SYM_FIELD == 0 then expect R "identifier"
      if sax.type.t == VT_FLOAT && (← getSym srx).f_func_type == FUNC_OLD then
        modSym sa fun y => { y with type := { y.type with t := VT_DOUBLE } }
    if (← getSym sym).type.t &&& VT_INLINE != 0 then
      let fname := (← curFile).filename
      let i := (← get).inline_fns.size
      modify fun s => { s with inline_fns := s.inline_fns.push { sym := sym, filename := fname } }
      let toks ← R.skip_or_save_block
      modify fun s => { s with inline_fns := s.inline_fns.modify i fun x => { x with func_str := toks } }
    else
      modify fun s => { s with cur_text_section := s.text_section }
      if ad.sec != 0 then
        modify fun s => { s with cur_text_section := ad.sec }
        if ad.new_section != 0 then
          let tf := (← getSec (← get).text_section).sh_flags
          modSec ad.sec fun x => { x with sh_flags := tf }
      gen_function R sym
    return (none, true)
  let mut has_init : Int32 := 0
  if l == VT_CMP then
    let mut sym := (← getSym (← get).func_vt.ref).next
    let fuel ← fuelM
    let mut found := false
    for _ in [0:fuel] do
      if sym == 0 then break
      if ((← getSym sym).v &&& ~~~SYM_FIELD) == v then found := true; break
      sym := (← getSym sym).next
    if !found then
      tcc_error R (cfmt "declaration for parameter '%s' but no such parameter" [.s (← get_tok_str v none)])
    if type.t &&& VT_STORAGE != 0 then
      tcc_error R (cfmt "storage class specified for '%s'" [.s (← get_tok_str v none)])
    if (← getSym sym).type.t &&& VT_EXTERN == 0 then
      tcc_error R (cfmt "redefinition of parameter '%s'" [.s (← get_tok_str v none)])
    let pt ← convert_parameter_type R type
    let sym' := sym
    modSym sym' fun y => { y with type := pt }
  else if type.t &&& VT_TYPEDEF != 0 then
    let mut sym ← sym_find v
    if sym != 0 && (← getSym sym).sym_scope == (← get).local_scope then
      let sx ← getSym sym
      if !(← R.is_compatible_types sx.type type) || sx.type.t &&& VT_TYPEDEF == 0 then
        tcc_error R (cfmt "incompatible redefinition of '%s'" [.s (← get_tok_str v none)])
      let sym' := sym
      modSym sym' fun y => { y with type := type }
    else
      sym ← sym_push R v type 0 0
    let aa := ad.a
    let sym' := sym
    modSym sym' fun y => { y with a := aa }
    if type.t &&& VT_BTYPE == VT_FUNC then
      let adf := ad.f
      modSym (← getSym sym).type.ref fun y => { y with aux := merge_funcattr y.aux adf }
  else if type.t &&& VT_BTYPE == VT_VOID && type.t &&& VT_EXTERN == 0 then
    tcc_error_s R "declaration of void object"
  else
    let mut r : Int32 := 0
    if type.t &&& VT_BTYPE == VT_FUNC then
      let adf := ad.f
      modSym type.ref fun y => { y with aux := merge_funcattr y.aux adf }
    else if type.t &&& VT_ARRAY == 0 then
      r := r ||| VT_LVAL
    if (← getTok) == 61 then has_init := 1
    if (type.t &&& VT_EXTERN != 0 && (has_init == 0 || l != VT_CONST)) ||
       type.t &&& VT_BTYPE == VT_FUNC ||
       (type.t &&& VT_ARRAY != 0 && has_init == 0 && l == VT_CONST && (← symC type.ref) < 0) then
      type := { type with t := type.t ||| VT_EXTERN }
      let _ ← external_sym R v type r ad
    else
      if l == VT_CONST || type.t &&& VT_STATIC != 0 then r := r ||| VT_CONST
      else r := r ||| VT_LOCAL
      type := { type with t := type.t &&& ~~~VT_EXTERN }
      if has_init != 0 then R.next
      else if l == VT_CONST then type := { type with t := type.t ||| VT_EXTERN }
      R.decl_initializer_alloc type ad r has_init v l
    if ad.alias_target != 0 && l == VT_CONST then
      let atsym ← sym_find ad.alias_target
      let atx ← getSym atsym
      if atsym == 0 || atx.c == 0 then tcc_error_s R "unsupported forward __alias__ attribute"
      let esym ← getElfSym (← get).symtab_section atx.c.toNat'
      put_extern_sym2 (← sym_find v) esym.st_shndx.toUInt32.toInt32 esym.st_value esym.st_size true
  if (← getTok) != 44 then
    if l == VT_JMP then return (some (if has_init != 0 then v else 1), true)
    skip R 59
    return (none, true)
  R.next
  return (none, false)

/-- one declarator, with its ghost node -/
def decl_one (R : Rec) (l : Int32) (btype : CType) (adbase : AttributeDef) (oldint : Bool) :
    M (Option Int32 × Bool) := do
  let gm ← gmark
  let r ← decl_one_body R l btype adbase oldint
  gwrap gm "decl" #[btype.t]
  return r

/-- `decl(l)` -/
def decl (R : Rec) (l : Int32) : M Int32 := do
  let fuel ← fuelM
  for _ in [0:fuel] do
    let mut oldint := false
    let (found, btype0, adbase) ← R.parse_btype {} (b2i (l == VT_LOCAL))
    let mut btype := btype0
    if !found then
      if l == VT_JMP then return 0
      let tk ← getTok
      if tk == 59 && l != VT_CMP then
        R.next; continue
      if tk == TOK_STATIC_ASSERT then
        R.do_Static_assert; continue
      if l != VT_CONST then break
      if tk == TOK_ASM1 || tk == TOK_ASM2 || tk == TOK_ASM3 then
        R.asm_global_instr
        continue
      if tk ≥ C.TOK_UIDENT then
        btype := { btype with t := VT_INT }
        oldint := true
      else
        if tk != C.TOK_EOF then expect R "declaration"
        break
    if (← getTok) == 59 then
      if btype.t &&& VT_BTYPE == VT_STRUCT && ((← getSym btype.ref).v &&& ~~~SYM_STRUCT) < SYM_FIRST_ANOM then
        pure ()
      else if IS_ENUM btype.t then pure ()
      else tcc_warning R "useless type defines no instances".toUTF8
      if l == VT_JMP then return 1
      R.next
      continue
    for _ in [0:fuel] do
      let (ret, brk) ← decl_one R l btype adbase oldint
      match ret with
      | some r => return r
      | none => pure ()
      if brk then break
  return 0

/-! ## tccgen_init / tccgen_compile / tccgen_finish -/

/-- `tccgen_init(s1)` -/
def tccgen_init (R : Rec) : M Unit := do
  modify fun s => { s with vtop := 0, vstack := s.vstack.set! 0 default }
  let cu := (← get).char_is_unsigned
  let ct : CType := { t := VT_BYTE ||| (if cu != 0 then VT_UNSIGNED else 0) }
  modify fun s => { s with int_type := { t := VT_INT }, char_type := ct }
  let cpt ← mk_pointer R ct
  modify fun s => { s with char_pointer_type := cpt }
  let fo ← sym_push R SYM_FIELD { t := VT_INT } 0 0
  modSym fo fun y => { y with aux := setBits32 (setBits32 y.aux 0 3 FUNC_CDECL) 3 2 FUNC_OLD }
  modify fun s => { s with func_old_type := { t := VT_FUNC, ref := fo } }
  let mut prec : Array UInt8 := Array.replicate 256 0
  for i in [0:256] do prec := prec.set! i (precedence (Int32.ofNat i)).toUInt8
  modify fun s => { s with prec := prec, initstr := ByteArray.empty }

/-- `tcc_debug_start(s1)` with debug off: the STT_FILE symbol -/
def tcc_debug_start : M Unit := do
  let s ← get
  let filename := match s.file with
    | _ :: p :: _ => p.filename
    | f :: [] => f.filename
    | [] => ByteArray.empty
  let _ ← put_elf_sym s.symtab_section 0 0 (ELF_ST_INFO STB_LOCAL STT_FILE) 0 SHN_ABS (some filename)

/-- `tccgen_compile(s1)` -/
def tccgen_compile (R : Rec) : M Int32 := do
  modify fun s => { s with
    funcname := ByteArray.empty, func_ind := -1, anon_sym := SYM_FIRST_ANOM,
    nocode_wanted := DATA_ONLY_WANTED, debug_modes := 0, global_expr := 0 }
  tcc_debug_start
  modify fun s => { s with parse_flags := PARSE_FLAG_PREPROCESS ||| PARSE_FLAG_TOK_NUM ||| PARSE_FLAG_TOK_STR }
  let gm ← gmark
  R.next
  let _ ← R.decl VT_CONST
  R.gen_inline_functions
  gwrap gm "tu"
  check_vstack R
  tcc_eh_frame_end
  return 0

/-- `tccgen_finish(s1)` -/
def tccgen_finish (R : Rec) : M Unit := do
  modify fun s => { s with inline_fns := #[] }
  let g ← sym_pop (← get).global_stack 0 false
  modify fun s => { s with global_stack := g }
  let l ← sym_pop (← get).local_stack 0 false
  modify fun s => { s with local_stack := l }
  free_defines 0
  let fuel ← fuelM
  for _ in [0:fuel] do
    if (← get).cur_switch == 0 then break
    end_switch
  modify fun s => { s with
    local_scope := 0, loop_scope := 0, all_cleanups := 0, pending_gotos := 0,
    nb_temp_local_vars := 0, global_label_stack := 0, local_label_stack := 0, cur_text_section := 0,
    sym_free_first := 0 }
  let _ := R

/-- libtcc.c's `tcc_compile(s1, filetype, str, fd)` for a file -/
def tcc_compile (R : Rec) (filetype : Int32) (filename : ByteArray) (fd : Int32) : M Int32 := do
  modify fun s => { s with error_set_jmp_enabled := 1 }
  let body : M Unit := do
    tcc_open_bf filename none
    modFile fun f => { f with fd := fd }
    preprocess_start R filetype
    tccgen_init R
    tccelf_begin_file
    let _ ← tccgen_compile R
    tccelf_end_file R
  try body catch e =>
    match e with
    | .longjmp => pure ()
    | e => throw e
  tccgen_finish R
  preprocess_end
  modify fun s => { s with error_set_jmp_enabled := 0 }
  return if (← get).nb_errors != 0 then -1 else 0

end Tcc.Model
