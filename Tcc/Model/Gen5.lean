import Tcc.Model.Gen4

/-!
# TccModel: tccgen.c, part 5 — the expression parser and statements

tccgen.c lines 6550–7650: precedence parser, `?:`, assignment, comma,
constant expressions, `return`, `switch`/`case` (sorting, binary search),
`cleanup` attribute calls, VLA stack restore, scopes, `block`.

`struct scope` and `struct switch_t` objects (block()'s locals in tcc)
live in the `scopes`/`switches` stores; the `int` jump chains whose
addresses tcc stores in them are `cells`.
-/

namespace Tcc.Model
open C
open Tok

/-! ## expressions -/

/-- `precedence(tok)` -/
def precedence (t : Int32) : Int32 :=
  if t == TOK_LOR then 1 else if t == TOK_LAND then 2 else if t == 124 then 3
  else if t == 94 then 4 else if t == 38 then 5 else if t == TOK_EQ || t == TOK_NE then 6
  else if t == TOK_ULT || t == TOK_UGE then 7 else if t == TOK_SHL || t == TOK_SAR then 8
  else if t == 43 || t == 45 then 9 else if t == 42 || t == 47 || t == 37 then 10
  else if t ≥ TOK_ULE && t ≤ TOK_GT then 7 else 0

/-- `condition_3way()` -/
def condition_3way (R : Rec) : M Int32 := do
  let v ← vt
  let mut c : Int32 := -1
  if (v.r.toInt32' &&& (VT_VALMASK ||| VT_LVAL)) == VT_CONST &&
     (v.r.toInt32' &&& VT_SYM == 0 || (← getSym v.sym).a_weak == 0) then
    vdup R
    R.gen_cast_s VT_BOOL
    c := (← vtci).toUInt32.toInt32
    vpop R
  return c


/-- `expr_infix(p)` and `expr_landor(op)` (mutually recursive; fuel-bounded) -/
def expr_infix : Nat → Rec → Int32 → M Unit
  | 0, _, _ => throw (.fuel "expr_infix")
  | fuel+1, R, p => do
    let lfuel ← fuelM
    let mut t ← getTok
    for _ in [0:lfuel] do
      let p2 := precedence t
      if p2 < p then break
      if t == TOK_LOR || t == TOK_LAND then
        expr_landor fuel R t
      else
        R.next
        R.unary
        if precedence (← getTok) > p2 then expr_infix fuel R (p2 + 1)
        R.gen_op t
      t ← getTok
where
  expr_landor : Nat → Rec → Int32 → M Unit
  | 0, _, _ => throw (.fuel "expr_landor")
  | fuel+1, R, op => do
    let mut t : Int32 := 0
    let mut cc := true
    let mut f := false
    let i : Int32 := b2i (op == TOK_LAND)
    let lfuel ← fuelM
    for _ in [0:lfuel] do
      let c ← if f then pure i else condition_3way R
      if c < 0 then
        save_regs R 1; cc := false
      else if c != i then
        modify fun s => { s with nocode_wanted := s.nocode_wanted + 1 }
        f := true
      if (← getTok) != op then break
      if c < 0 then t ← gvtst R i t
      else vpop R
      R.next
      -- expr_landor_next(op): unary(), expr_infix(precedence(op) + 1)
      R.unary
      expr_infix fuel R (precedence op + 1)
    if cc || f then
      vpop R
      vpushi R (i ^^^ b2i f)
      gsym R t
      if f then modify fun s => { s with nocode_wanted := s.nocode_wanted - 1 }
    else
      gvtst_set R i t

/-- `is_cond_bool(sv)` -/
def is_cond_bool (sv : SValue) : Bool :=
  if (sv.r.toInt32' &&& (VT_VALMASK ||| VT_LVAL ||| VT_SYM)) == VT_CONST && sv.type.t &&& VT_BTYPE == VT_INT then
    sv.c.i.toUInt32 < 2
  else sv.r.toInt32' == VT_CMP

/-- `expr_lor()`: unary(), expr_infix(1) -/
def expr_lor (R : Rec) : M Unit := do
  R.unary
  expr_infix (← fuelM) R 1

/-- `expr_cond()` -/
def expr_cond : Nat → Rec → M Unit
  | 0, _ => throw (.fuel "expr_cond")
  | fuel+1, R => do
    expr_lor R
    if (← getTok) != 63 then return
    R.next
    let c ← condition_3way R
    let g := (← getTok) == 58 && (← get).gnu_ext != 0
    let mut tt : Int32 := 0
    if !g then
      if c < 0 then
        save_regs R 1
        tt ← gvtst R 1 0
      else vpop R
    else if c < 0 then
      save_regs R 1
      R.gv_dup
      tt ← gvtst R 0 0
    if c == 0 then modify fun s => { s with nocode_wanted := s.nocode_wanted + 1 }
    if !g then R.gexpr
    if (← vtt) &&& VT_BTYPE == VT_FUNC then setVtType 0 (← mk_pointer R (← vt).type)
    let sv ← vt
    vtopDec
    let mut u : Int32 := 0
    if g then u := tt
    else if c < 0 then
      u ← gjmp_acs 0
      gsym R tt
    if c == 0 then modify fun s => { s with nocode_wanted := s.nocode_wanted - 1 }
    if c == 1 then modify fun s => { s with nocode_wanted := s.nocode_wanted + 1 }
    skip R 58
    expr_cond fuel R
    if (← vtt) &&& VT_BTYPE == VT_FUNC then setVtType 0 (← mk_pointer R (← vt).type)
    let (ok, type) ← combine_types R sv (← vt) 63
    if !ok then
      type_incompatibility_error R sv.type (← vt).type
        "type mismatch in conditional expression (have '%s' and '%s')"
    if c < 0 && is_cond_bool (← vt) && is_cond_bool sv then
      let t1 ← gvtst R 0 0
      let t2 ← gjmp_acs 0
      gsym R u
      vpushv R sv
      gvtst_set R 0 t1
      gvtst_set R 1 t2
      R.gen_cast type
      return
    let islv := VT_STRUCT == (type.t &&& VT_BTYPE)
    if c != 1 then
      R.gen_cast type
      if islv then
        setVtType 0 (← mk_pointer R (← vt).type)
        gaddrof
    let mut rc := RC_TYPE type.t
    if USING_TWO_WORDS type.t then rc := RC_RET type.t
    tt := 0
    let mut r2 : Int32 := 0
    if c < 0 then
      if type.t != VT_VOID then r2 ← R.gv rc
      tt ← gjmp_acs 0
    gsym R u
    if c == 1 then modify fun s => { s with nocode_wanted := s.nocode_wanted - 1 }
    if c != 0 then
      setVt 0 sv
      R.gen_cast type
      if islv then
        setVtType 0 (← mk_pointer R (← vt).type)
        gaddrof
    if c < 0 then
      if type.t != VT_VOID then
        let r1 ← R.gv rc
        move_reg R r2 r1 (if islv then VT_PTR else type.t)
        setVtR 0 r2
      gsym R tt
    if islv then indir R

/-- `expr_eq()` -/
def expr_eq (R : Rec) : M Unit := do
  expr_cond (← fuelM) R
  let t ← getTok
  if t == 61 || TOK_ASSIGN t then
    test_lvalue R
    R.next
    if t == 61 then
      R.expr_eq
    else
      vdup R
      R.expr_eq
      R.gen_op (TOK_ASSIGN_OP t)
    R.vstore

/-- `gexpr()` -/
def gexpr (R : Rec) : M Unit := do
  R.expr_eq
  if (← getTok) == 44 then
    let fuel ← fuelM
    for _ in [0:fuel] do
      vpop R
      R.next
      R.expr_eq
      if (← getTok) != 44 then break
    setVtType 0 (← convert_expression_type R (← vt).type)
    if ((← vtr) &&& VT_VALMASK) == VT_CONST && (← get).nocode_wanted != 0 && !(← CONST_WANTED) then
      let t ← vtt
      if t != VT_VOID && t &&& VT_BTYPE != VT_STRUCT then
        let _ ← R.gv (RC_TYPE t)

/-- `expr_const1()` -/
def expr_const1 (R : Rec) : M Unit := do
  modify fun s => { s with nocode_wanted := s.nocode_wanted + CONST_WANTED_BIT }
  expr_cond (← fuelM) R
  modify fun s => { s with nocode_wanted := s.nocode_wanted - CONST_WANTED_BIT }

/-- `expr_const64()` -/
def expr_const64 (R : Rec) : M Int64 := do
  expr_const1 R
  if ((← vtr) &&& (VT_VALMASK ||| VT_LVAL ||| VT_SYM ||| VT_NONCONST)) != VT_CONST then
    expect R "constant expression"
  let c := (← vtci).toInt64
  vpop R
  return c

/-- `expr_const()` -/
def expr_const (R : Rec) : M Int32 := do
  let wc ← expr_const64 R
  let c := wc.toInt32
  if c.toInt64 != wc && c.toUInt32.toUInt64.toInt64 != wc then
    tcc_error_s R "constant exceeds 32 bit"
  return c

/-! ## return -/

/-- `gfunc_return(func_type)` -/
def gfunc_return (R : Rec) (func_type : CType) : M Unit := do
  if func_type.t &&& VT_BTYPE == VT_STRUCT then
    let (ret_nregs, ret_type, ret_align, regsize) ← gfunc_sret R func_type (← get).func_var {}
    if ret_nregs < 0 then
      arch_transfer_ret_regs R false
    else if ret_nregs == 0 then
      let type ← mk_pointer R func_type
      vset R type (VT_LOCAL ||| VT_LVAL) (← get).func_vc
      indir R
      vswap R
      R.vstore
    else
      let (size0, align0) ← R.type_size func_type
      let mut size := size0
      let mut align := align0
      let v ← vt
      if ret_nregs * regsize > size ||
         ((align &&& (ret_align - 1)) != 0 &&
          ((v.r.toInt32' &&& VT_VALMASK) < VT_CONST || (v.c.i.toUInt32.toInt32 &&& (ret_align - 1)) != 0)) then
        if ret_nregs * regsize > size then size := ret_nregs * regsize
        if ret_align > align then align := ret_align
        modify fun s => { s with loc := (s.loc - size) &&& (-align) }
        let addr := (← get).loc
        vset R func_type (VT_LOCAL ||| VT_LVAL) addr
        vswap R
        R.vstore
        vpop R
        vset R ret_type (VT_LOCAL ||| VT_LVAL) addr
      setVtType 0 ret_type
      let mut rc := RC_RET ret_type.t
      let mut n := ret_nregs
      let fuel ← fuelM
      for _ in [0:fuel] do
        n := n - 1
        if n ≤ 0 then break
        vdup R
        let _ ← R.gv rc
        vswap R
        incr_offset R regsize
        rc := rc <<< 1
      let _ ← R.gv rc
      modify fun s => { s with vtop := s.vtop - (ret_nregs - 1).toNat' }
  else
    let _ ← R.gv (RC_RET func_type.t)
  vtopDec

/-- `check_func_return()` -/
def check_func_return (R : Rec) : M Unit := do
  let s ← get
  if s.func_vt.t &&& VT_BTYPE == VT_VOID then return
  if (s.funcname == "main".toUTF8 || s.func_old != 0) && s.func_vt.t &&& VT_BTYPE == VT_INT then
    vpushi R 0
    gfunc_return R s.func_vt
  else
    tcc_warning R (cfmt "function might return no value: '%s'" [.s s.funcname])

/-! ## switch -/

def case_cmp (unsig : Bool) (a b : UInt64) : Int32 :=
  if unsig then (if a < b then -1 else if a > b then 1 else 0)
  else (if a.toInt64 < b.toInt64 then -1 else if a.toInt64 > b.toInt64 then 1 else 0)

/-- insertion sort by v1 (equal keys are an error below either way) -/
def sortCases (unsig : Bool) (p : Array CaseT) : Array CaseT := Id.run do
  let mut a := p
  for i in [1:a.size] do
    let mut j := i
    for _ in [0:i] do
      if j == 0 then break
      if case_cmp unsig a[j-1]!.v1.toUInt64 a[j]!.v1.toUInt64 > 0 then
        let x := a[j-1]!
        a := (a.set! (j-1) a[j]!).set! j x
        j := j - 1
      else break
  return a

/-- `case_sort(sw)` -/
def case_sort (R : Rec) (swi : Nat) : M Unit := do
  let sw := (← get).switches[swi]!
  if sw.p.size < 2 then return
  let unsig := sw.sv.type.t &&& VT_UNSIGNED != 0
  let mut p := sortCases unsig sw.p
  let mut k := 0
  let fuel ← fuelM
  for _ in [0:fuel] do
    if k + 1 ≥ p.size then break
    let a := p[k]!
    let b := p[k+1]!
    if case_cmp unsig a.v2.toUInt64 b.v1.toUInt64 ≥ 0 then
      let l1 := a.line
      let l2 := b.line
      let jmp ← error1 R .error "duplicate case value".toUTF8 (if l1 > l2 then l1 else l2)
      if jmp then throw .longjmp else throw (.exit 1)
    else if a.v2 + 1 == b.v1 && a.ind == b.ind then
      p := p.set! (k+1) { b with v1 := a.v1 }
      p := p.eraseIdx! k
    else
      k := k + 1
  modify fun s => { s with switches := s.switches.modify swi fun w => { w with p := p } }

/-- `gcase(base, len, dsym)` over cases [base, base+len) -/
def gcase : Nat → Rec → Array CaseT → Nat → Nat → Int32 → M Int32
  | 0, _, _, _, _, _ => throw (.fuel "gcase")
  | fuel+1, R, cases, base0, len0, dsym0 => do
    let mut base := base0
    let mut len := len0
    let mut dsym := dsym0
    let mut t := (← vtt) &&& VT_BTYPE
    if t != VT_LLONG then t := VT_INT
    let lfuel ← fuelM
    for _ in [0:lfuel] do
      if len == 0 then break
      let l2 := if len > 8 then len / 2 else 0
      let p := cases[base + l2]!
      vdup R
      vpush64 R t p.v2.toUInt64
      if l2 == 0 && p.v1 == p.v2 then
        R.gen_op TOK_EQ
        gsym_addr R (← gvtst R 0 0) p.ind
      else
        R.gen_op TOK_GT
        let mut e : Int32 := 0
        if len == 1 then
          dsym ← gvtst R 0 dsym
          e := 0
        else
          e ← gvtst R 0 0
        vdup R
        vpush64 R t p.v1.toUInt64
        R.gen_op TOK_GE
        gsym_addr R (← gvtst R 0 0) p.ind
        dsym ← gcase fuel R cases base l2 dsym
        gsym R e
      let l2' := l2 + 1
      base := base + l2'
      len := len - l2'
    gjmp_acs dsym

