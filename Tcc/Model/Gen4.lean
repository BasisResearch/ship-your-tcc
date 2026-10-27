import Tcc.Model.Gen3

/-!
# TccModel: tccgen.c, part 4 — expressions

tccgen.c lines 5650–6800: `unary` (primary expressions, builtins, postfix
operators, calls), the precedence parser (`expr_infix`, `expr_landor`),
`expr_cond`, `expr_eq`, `gexpr`, constant expressions.
-/

namespace Tcc.Model
open C
open Tok

/-- `skip_or_save_block(str)` (`save`: str != NULL): the saved tokens -/
def skip_or_save_block (R : Rec) (save : Bool) : M (Option TokArr) := do
  let braces := (← getTok) == 123
  let mut level : Int32 := 0
  let mut str : TokenString := {}
  let fuel ← fuelM
  for _ in [0:fuel] do
    let t ← getTok
    if level == 0 && (t == 44 || t == 59 || t == 125 || t == 41 || t == 93) then break
    if t == C.TOK_EOF then
      if save || level > 0 then tcc_error_s R "unexpected end of file"
      else break
    if save then str ← tok_str_add_tok str
    R.next
    if t == 123 || t == 40 || t == 91 then
      level := level + 1
    else if t == 125 || t == 41 || t == 93 then
      level := level - 1
      if level == 0 && braces && t == 125 then break
  if save then
    str := tok_str_add str C.TOK_EOF
    return some (str.str.extract 0 str.len.toNat')
  return none

/-- `unary()` call part: the function call at vtop (tok == '(') -/
def unary_call (R : Rec) : M Unit := do
  let gm ← gmark
  if (← vtt) &&& VT_BTYPE != VT_FUNC then
    if (← vtt) &&& (VT_BTYPE ||| VT_ARRAY) == VT_PTR then
      setVtType 0 (← pointed_type (← vt).type)
      if (← vtt) &&& VT_BTYPE != VT_FUNC then expect R "function pointer"
    else
      expect R "function pointer"
  else
    modVt 0 fun v => { v with r := v.r &&& ~~~VT_LVAL.toUInt16' }
  let s := (← vt).type.ref
  R.next
  let sx ← getSym s
  let mut sa := sx.next
  let mut nb_args : Int32 := 0
  let mut regsize : Int32 := 0
  let mut ret : SValue := { r2 := VT_CONST.toUInt16' }
  let mut ret_nregs : Int32 := 0
  let mut ret_align : Int32 := 0
  if sx.type.t &&& VT_BTYPE == VT_STRUCT then
    let variadic := b2i (sx.f_func_type == FUNC_ELLIPSIS)
    let (n, rt, ra, rs) ← gfunc_sret R sx.type variadic ret.type
    ret_nregs := n; ret := { ret with type := rt }; ret_align := ra; regsize := rs
    if ret_nregs ≤ 0 then
      let (size, align) ← R.type_size sx.type
      modify fun st => { st with loc := (st.loc - size) &&& (-align) }
      ret := { ret with type := sx.type, r := (VT_LOCAL ||| VT_LVAL).toUInt16' }
      vseti R VT_LOCAL (← get).loc
      ret := { ret with c := (← vt).c }
      if ret_nregs < 0 then vtopDec
      else nb_args := nb_args + 1
  else
    ret_nregs := 1
    ret := { ret with type := sx.type }
  if ret_nregs > 0 then
    ret := { ret with c := ret.c.setI 0, r := (R_RET ret.type.t).toUInt16', r2 := (R2_RET ret.type.t).toUInt16' }
  if (← getTok) != 41 then
    let fuel ← fuelM
    for _ in [0:fuel] do
      -- reverse_funcargs is 0
      R.expr_eq
      gfunc_param_typed R s sa
      nb_args := nb_args + 1
      if sa != 0 then sa := (← getSym sa).next
      if (← getTok) == 41 then break
      skip R 44
  if sa != 0 then tcc_error_s R "too few arguments to function"
  R.next
  vcheck_cmp R
  R.gfunc_call nb_args
  if ret_nregs < 0 then
    vsetc R ret.type ret.r.toInt32' ret.c
    arch_transfer_ret_regs R true
  else
    let mut n := ret_nregs
    let fuel ← fuelM
    for _ in [0:fuel] do
      if n ≤ 1 then break
      let mut rc := reg_classes[ret.r.toNat]! &&& ~~~(RC_INT ||| RC_FLOAT)
      n := n - 1
      rc := rc <<< n
      let mut r : Int32 := 0
      for k in [0:NB_REGS] do
        if reg_classes[k]! &&& rc != 0 then r := Int32.ofNat k; break
        r := Int32.ofNat (k + 1)
      vsetc R ret.type r ret.c
    vsetc R ret.type ret.r.toInt32' ret.c
    let r2 := ret.r2
    modVt 0 fun v => { v with r2 := r2 }
    if sx.type.t &&& VT_BTYPE == VT_STRUCT && ret_nregs != 0 then
      let (size0, align0) ← R.type_size sx.type
      let size := (size0 + regsize - 1) &&& (-regsize)
      let align := if ret_align > align0 then ret_align else align0
      modify fun st => { st with loc := (st.loc - size) &&& (-align) }
      let addr := (← get).loc
      let mut offset : Int32 := 0
      for _ in [0:fuel] do
        vset R ret.type (VT_LOCAL ||| VT_LVAL) (addr + offset)
        vswap R
        R.vstore
        vtopDec
        ret_nregs := ret_nregs - 1
        if ret_nregs == 0 then break
        offset := offset + regsize
      vset R sx.type (VT_LOCAL ||| VT_LVAL) addr
    let t := sx.type.t &&& VT_BTYPE
    if t == VT_BYTE || t == VT_SHORT || t == VT_BOOL then
      modVt 0 fun v => { v with r := (v.r.toInt32' ||| BFVAL_MUSTCAST 1).toUInt16' }
  if sx.f_func_noreturn != 0 then CODE_OFF
  gwrap (gm - 1) "call"

/-- `unary()` postfix operators -/
def unary_postfix (R : Rec) : M Unit := do
  let fuel ← fuelM
  for _ in [0:fuel] do
    let tk ← getTok
    if tk == TOK_INC || tk == TOK_DEC then
      inc R true tk
      R.next
      gnode (if tk == TOK_INC then "postinc" else "postdec") #[] 1
    else if tk == 46 || tk == TOK_ARROW then
      if tk == TOK_ARROW then indir R
      let qualifiers := (← vtt) &&& VT_QUAL
      test_lvalue R
      R.next
      let (s, cumofs) ← find_field fuel R (← vt).type (← getTok)
      gaddrof
      setVtType 0 (← get).char_pointer_type
      vpushi R cumofs
      R.gen_op 43
      setVtType 0 (← getSym s).type
      if qualifiers != 0 then
        setVtType 0 (← parse_btype_qualify R (← vt).type qualifiers)
      if (← vtt) &&& VT_ARRAY == 0 then
        modVt 0 fun v => { v with r := v.r ||| VT_LVAL.toUInt16' }
      gnode (if tk == TOK_ARROW then "arrow" else "member") #[← getTok] 1
      R.next
    else if tk == 91 then
      R.next
      R.gexpr
      R.gen_op 43
      indir R
      skip R 93
      gnode "index" #[] 2
    else if tk == 40 then
      unary_call R
    else
      break

/-- the ghost node kind of a primary/prefix expression starting with `tok` -/
def unaryGhostKind (tok : Int32) : String :=
  if C.TOK_HAS_VALUE tok then (if TOK_IS_STR tok then "str" else "const")
  else if tok == 42 then "deref" else if tok == 38 then "addr" else if tok == 33 then "lnot"
  else if tok == 126 then "bnot" else if tok == 43 then "uplus" else if tok == 45 then "neg"
  else if tok == TOK_INC then "preinc" else if tok == TOK_DEC then "predec"
  else if tok == TOK_SIZEOF then "sizeof"
  else if tok == TOK_ALIGNOF1 || tok == TOK_ALIGNOF2 || tok == TOK_ALIGNOF3 then "alignof"
  else if tok == TOK_GENERIC then "generic" else if tok == TOK_LAND then "labeladdr"
  else if tok == TOK___FUNC__ || tok == TOK___FUNCTION__ then "func"
  else if tok == TOK___NAN__ || tok == TOK___SNAN__ || tok == TOK___INF__ then "const"
  else if (tok ≥ TOK_builtin_types_compatible_p && tok ≤ TOK___atomic_nand_fetch) || tok == TOK___riscv64_clear_cache then "builtin"
  else "ident"

/-- `unary()` -/
def unary (R : Rec) : M Unit := do
  let fuel ← fuelM
  let mut tok ← getTok
  for _ in [0:fuel] do
    if tok == TOK_EXTENSION then
      R.next
      tok ← getTok
    else break
  let pushTokc (t : Int32) : M Unit := do
    vsetc R { t := t } VT_CONST (← get).tokc
    R.next
  let strInit (t0 : Int32) : M Unit := do
    let mut t := t0
    if (← get).warn_write_strings &&& WARN_ON != 0 then t := t ||| VT_CONSTANT
    let mut type ← mk_pointer R { t := t }
    type := { type with t := type.t ||| VT_ARRAY }
    let ad : AttributeDef := { sec := (← get).rodata_section }
    R.decl_initializer_alloc type ad VT_CONST 2 0 0
  let gm ← gmark
  let tc0 := (← get).tokc
  let mut gwrapped := false
  if tok == TOK_LCHAR || tok == TOK_CINT || tok == TOK_CCHAR then pushTokc VT_INT
  else if tok == TOK_U16CHAR then pushTokc (VT_SHORT ||| VT_UNSIGNED)
  else if tok == TOK_U32CHAR then pushTokc (VT_INT ||| VT_UNSIGNED)
  else if tok == TOK_CUINT then pushTokc (VT_INT ||| VT_UNSIGNED)
  else if tok == TOK_CLLONG then pushTokc VT_LLONG
  else if tok == TOK_CULLONG then pushTokc (VT_LLONG ||| VT_UNSIGNED)
  else if tok == TOK_CFLOAT then pushTokc VT_FLOAT
  else if tok == TOK_CDOUBLE then pushTokc VT_DOUBLE
  else if tok == TOK_CLDOUBLE then pushTokc VT_LDOUBLE
  else if tok == TOK_CLONG then pushTokc (VT_LLONG ||| VT_LONG)
  else if tok == TOK_CULONG then pushTokc (VT_LLONG ||| VT_LONG ||| VT_UNSIGNED)
  else if (tok == TOK___FUNCTION__ && (← get).gnu_ext != 0) || tok == TOK___FUNC__ then
    let fn := (← get).funcname
    let cs := fn.push 0
    modify fun s => { s with tok := TOK_STR, tokcstr := cs, tokc := s.tokc.setStr cs (Int32.ofNat cs.size) }
    strInit (← get).char_type.t
  else if tok == TOK_U16STR then strInit (VT_SHORT ||| VT_UNSIGNED)
  else if tok == TOK_U32STR then strInit (VT_INT ||| VT_UNSIGNED)
  else if tok == TOK_LSTR then strInit VT_INT
  else if tok == TOK_STR then strInit (← get).char_type.t
  else if tok == TOK_SOTYPE || tok == 40 then
    let t := tok
    R.next
    let (f, type0, ad0) ← R.parse_btype {} 0
    if f then
      let (type, _, _) ← R.type_decl type0 ad0 TYPE_ABSTRACT
      skip R 41
      if (← getTok) == 123 then
        let mut r : Int32 := if (← get).global_expr != 0 then VT_CONST else VT_LOCAL
        if type.t &&& VT_ARRAY == 0 then r := r ||| VT_LVAL
        R.decl_initializer_alloc type {} r 1 0 0
        gwrap gm "complit" #[type.t]
      else if t == TOK_SOTYPE then
        vpush R type
        gwrap gm "type" #[type.t]
        return
      else
        R.unary
        R.gen_cast type
        gwrap gm "cast" #[type.t]
      gwrapped := true
    else if (← getTok) == 123 then
      let saved_nocode_wanted := (← get).nocode_wanted
      if (← CONST_WANTED) && !(← NOEVAL_WANTED) then expect R "constant"
      if (← get).local_scope == 0 then tcc_error_s R "statement expression outside of function"
      save_regs R 0
      vpushi R 0
      setVtT 0 VT_VOID
      R.block STMT_EXPR
      if saved_nocode_wanted != 0 then modify fun s => { s with nocode_wanted := saved_nocode_wanted }
      skip R 41
      gwrap gm "stmtexpr"
      gwrapped := true
    else
      R.gexpr
      skip R 41
      gwrapped := true
  else if tok == 42 then
    R.next
    R.unary
    indir R
  else if tok == 38 then
    R.next
    R.unary
    let t ← vtt
    if t &&& VT_BTYPE != VT_FUNC && t &&& (VT_ARRAY ||| VT_VLA) == 0 then test_lvalue R
    let s := (← vt).sym
    if s != 0 then modSym s fun y => { y with a := setBits16 y.a 12 1 1 }
    setVtType 0 (← mk_pointer R (← vt).type)
    gaddrof
  else if tok == 33 then
    R.next
    R.unary
    gen_test_zero R TOK_EQ
  else if tok == 126 then
    R.next
    R.unary
    vpushi R (-1)
    R.gen_op 94
  else if tok == 43 then
    R.next
    R.unary
    if (← vtt) &&& VT_BTYPE == VT_PTR then tcc_error_s R "pointer not accepted for unary plus"
    if !is_float (← vtt) then
      vpushi R 0
      R.gen_op 43
  else if tok == TOK_SIZEOF || tok == TOK_ALIGNOF1 || tok == TOK_ALIGNOF2 || tok == TOK_ALIGNOF3 then
    let t := tok
    R.next
    if (← getTok) == 40 then setTok TOK_SOTYPE
    let type ← expr_type R.unary
    if t == TOK_SIZEOF then
      let _ ← R.vpush_type_size type
      R.gen_cast_s VT_SIZE_T
    else
      let (_, align0) ← R.type_size type
      let mut align := align0
      -- hack: accessing previous vtop
      let v1 ← vt 1
      let s := if v1.r.toInt32' &&& VT_SYM != 0 then v1.sym else 0
      if s != 0 then
        let sx ← getSym s
        if sx.a_aligned != 0 then align := (1 : Int32) <<< (sx.a_aligned.toInt32' - 1)
      vpushs R align.toInt64.toUInt64
  else if tok == TOK_builtin_expect then
    parse_builtin_params R false "ee"
    vpop R
  else if tok == TOK_builtin_types_compatible_p then
    parse_builtin_params R false "tt"
    let n ← compare_types R (← vt (-1)).type (← vt 0).type true
    modify fun s => { s with vtop := s.vtop - 2 }
    vpushi R (b2i n)
  else if tok == TOK_builtin_choose_expr then
    R.next
    skip R 40
    let c ← R.expr_const64
    skip R 44
    if c == 0 then modify fun s => { s with nocode_wanted := s.nocode_wanted + 1 }
    R.expr_eq
    if c == 0 then
      vpop R
      modify fun s => { s with nocode_wanted := s.nocode_wanted - 1 }
    skip R 44
    if c != 0 then modify fun s => { s with nocode_wanted := s.nocode_wanted + 1 }
    R.expr_eq
    if c != 0 then
      vpop R
      modify fun s => { s with nocode_wanted := s.nocode_wanted - 1 }
    skip R 41
  else if tok == TOK_builtin_constant_p then
    parse_builtin_params R true "e"
    let mut n : Int32 := 1
    let v ← vt
    if (v.r.toInt32' &&& (VT_VALMASK ||| VT_LVAL)) != VT_CONST then n := 0
    else if v.r.toInt32' &&& VT_SYM != 0 && (← getSym v.sym).a_addrtaken != 0 then n := 0
    vtopDec
    vpushi R n
  else if tok == TOK_builtin_unreachable then
    parse_builtin_params R false ""
    vpush R { t := VT_VOID }
    CODE_OFF
  else if tok == TOK_builtin_frame_address || tok == TOK_builtin_return_address then
    let tok1 := tok
    R.next
    skip R 40
    let mut level ← R.expr_const
    if level < 0 then tcc_error R (cfmt "%s only takes positive integers" [.s (← get_tok_str tok1 none)])
    skip R 41
    let type ← mk_pointer R { t := VT_VOID }
    vset R type VT_LOCAL 0
    for _ in [0:fuel] do
      if level == 0 then break
      level := level - 1
      vpushi R 16
      R.gen_op 45
      setVtType 0 (← mk_pointer R (← vt).type)
      indir R
    if tok1 == TOK_builtin_return_address then
      vpushi R 8
      R.gen_op 45
      setVtType 0 (← mk_pointer R (← vt).type)
      indir R
  else if tok == TOK_builtin_va_start then
    parse_builtin_params R false "ee"
    let mut r := (← vtr) &&& VT_VALMASK
    if r == VT_LLOCAL then r := VT_LOCAL
    if r != VT_LOCAL then tcc_error_s R "__builtin_va_start expects a local variable"
    gen_va_start R
    R.vstore
  else if tok == TOK___riscv64_clear_cache then
    parse_builtin_params R false "ee"
    vpop R
    vpop R
    gen_clear_cache
    vpushi R 0
    setVtT 0 VT_VOID
  else if tok ≥ TOK___atomic_store && tok ≤ TOK___atomic_nand_fetch then
    parse_atomic R tok
  else if tok == TOK_INC || tok == TOK_DEC then
    let t := tok
    R.next
    R.unary
    inc R false t
  else if tok == 45 then
    R.next
    R.unary
    if is_float (← vtt) then gen_opif R TOK_NEG
    else
      vpushi R 0
      vswap R
      R.gen_op 45
  else if tok == TOK_LAND && (← get).gnu_ext != 0 then
    R.next
    let tk ← getTok
    if tk < C.TOK_UIDENT then expect R "label identifier"
    let mut s ← label_find tk
    if s == 0 then
      s ← label_push true tk LABEL_FORWARD.toUInt32.toInt32
    else
      if (← getSym s).r == LABEL_DECLARED then
        let s' := s
        modSym s' fun y => { y with r := LABEL_FORWARD }
    if (← getSym s).type.t &&& VT_BTYPE != VT_PTR then
      let pt ← mk_pointer R { t := VT_VOID }
      let s' := s
      modSym s' fun y => { y with type := { pt with t := pt.t ||| VT_STATIC } }
    vpushsym R (← getSym s).type s
    R.next
  else if tok == TOK_GENERIC then
    let saved_nocode_wanted := (← get).nocode_wanted
    modify fun s => { s with nocode_wanted := s.nocode_wanted &&& ~~~CONST_WANTED_MASK }
    R.next
    skip R 40
    let ct0 ← expr_type R.expr_eq
    let controlling_type ← convert_expression_type R ct0
    modify fun s => { s with nocode_wanted := saved_nocode_wanted }
    let mut has_default := false
    let mut has_match := false
    let mut str : Option TokArr := none
    for _ in [0:fuel] do
      let mut learn := false
      skip R 44
      if (← getTok) == TOK_DEFAULT then
        if has_default then tcc_error_s R "too many 'default'"
        has_default := true
        if !has_match then learn := true
        R.next
      else
        let (_, type0, ad0) ← R.parse_btype {} 0
        let (type, _, _) ← R.type_decl type0 ad0 TYPE_ABSTRACT
        if ← compare_types R controlling_type type false then
          if has_match then tcc_error_s R "type match twice"
          has_match := true
          learn := true
      skip R 58
      if learn then
        str ← R.skip_or_save_block
      else
        let _ ← skip_or_save_block R false
      if (← getTok) == 41 then break
    match str with
    | none =>
      let buf ← type_to_str fuel 60 controlling_type none
      tcc_error R (cfmt "type '%s' does not match any association" [.s buf])
    | some toks =>
      let _ ← begin_macro .heap toks 0 1
      R.next
      R.expr_eq
      if (← getTok) != C.TOK_EOF then expect R ","
      end_macro
      R.next
  else if tok == TOK___NAN__ || tok == TOK___SNAN__ || tok == TOK___INF__ then
    let n : Int32 := if tok == TOK___NAN__ then 0x7fc00000 else if tok == TOK___SNAN__ then 0x7f800001 else 0x7f800000
    vpushi R n
    setVtT 0 VT_FLOAT
    R.next
  else
    -- tok_identifier:
    if tok < C.TOK_UIDENT then
      let st ← get
      tcc_error R (cfmt "expression expected before '%s'" [.s (← get_tok_str tok (some st.tokc))])
    let t := tok
    R.next
    let mut s ← sym_find t
    if s == 0 || IS_ASM_SYM (← getSym s) then
      let name ← get_tok_str t none
      if (← getTok) != 40 then tcc_error R (cfmt "'%s' undeclared" [.s name])
      if (← get).func_old == 0 then
        tcc_warning_c R WARNC_implicit_function_declaration (cfmt "implicit declaration of function '%s'" [.s name])
      s ← external_global_sym t (← get).func_old_type
    let sx ← getSym s
    let mut r := sx.r.toInt32'
    if (r &&& VT_VALMASK) < VT_CONST then r := (r &&& ~~~VT_VALMASK) ||| VT_LOCAL
    vset R sx.type r sx.c
    modVt 0 fun v => v.setSym s
    if r &&& VT_SYM != 0 then
      setVtCi 0 0
    else if r == VT_CONST && IS_ENUM_VAL sx.type.t then
      setVtCi 0 sx.enum_val.toUInt64
  if !gwrapped then
    gwrap gm (unaryGhostKind tok) #[tok, tc0.lo.toUInt32.toInt32, (tc0.lo >>> 32).toUInt32.toInt32]
  unary_postfix R


end Tcc.Model
