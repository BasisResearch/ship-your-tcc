import Tcc.Model.Gen5

/-!
# TccModel: tccgen.c, part 6 — statements

tccgen.c lines 6950–7650: `cleanup` attribute calls, VLA stack restore,
scopes, `lblock`, `block`.
-/

namespace Tcc.Model
open C
open Tok

/-- `end_switch()` -/
def end_switch : M Unit := do
  let s ← get
  let sw := s.switches[s.cur_switch]!
  set { s with cur_switch := sw.prev }

/-! ## cleanup attribute -/

/-- `save_lvalues()` -/
def save_lvalues (R : Rec) : M Unit := do
  let mut svi := (← get).vtop
  let fuel ← fuelM
  for _ in [0:fuel] do
    if svi < 1 then break
    let sv := (← get).vstack[svi]!
    if sv.sym != 0 && sv.r.toInt32' &&& VT_LVAL != 0 then
      let (size, align) ← R.type_size sv.type
      let (l, r2) ← get_temp_local_var size align
      vset R sv.type (VT_LOCAL ||| VT_LVAL) l
      modVt 0 fun v => { v with r2 := r2.toUInt16' }
      vpushv R sv
      let tm1 ← vt (-1)
      modify fun s => { s with vstack := s.vstack.set! svi tm1 }
      R.vstore
      vtopDec
    svi := svi - 1

/-- `try_call_scope_cleanup(stop)` -/
def try_call_scope_cleanup (R : Rec) (stop : SymId) : M Unit := do
  let mut cls := (← getScope (← get).cur_scope).cl_s
  let fuel ← fuelM
  for _ in [0:fuel] do
    if cls == stop then break
    let c ← getSym cls
    let fs := c.cleanup_func
    let vs := c.prev_tok
    save_lvalues R
    vpushsym R (← getSym fs).type fs
    let vsx ← getSym vs
    vset R vsx.type vsx.r.toInt32' vsx.c
    modVt 0 fun v => v.setSym vs
    setVtType 0 (← mk_pointer R (← vt).type)
    gaddrof
    R.gfunc_call 1
    cls := c.next

/-- `try_call_cleanup_goto(cleanupstate)` -/
def try_call_cleanup_goto (R : Rec) (cleanupstate : SymId) : M Unit := do
  let cs ← getScope (← get).cur_scope
  if cs.cl_s == 0 then return
  let csv := (← getSym cleanupstate).v
  let mut ocd : Int32 := if cleanupstate != 0 then csv &&& ~~~SYM_FIELD else 0
  let mut ccd := cs.cl_n
  let mut oc := cleanupstate
  let fuel ← fuelM
  for _ in [0:fuel] do
    if !(ocd > ccd) then break
    ocd := ocd - 1
    oc := (← getSym oc).next
  let mut cc := cs.cl_s
  for _ in [0:fuel] do
    if !(ccd > ocd) then break
    ccd := ccd - 1
    cc := (← getSym cc).next
  for _ in [0:fuel] do
    if cc == oc then break
    cc := (← getSym cc).next
    oc := (← getSym oc).next
    ccd := ccd - 1
  try_call_scope_cleanup R cc

/-- `block_cleanup(o)` -/
def block_cleanup (R : Rec) (o : Nat) : M Unit := do
  let mut jmp : Int32 := 0
  let os ← getScope o
  let mut owner : Option SymId := none   -- pg = &pending_gotos or &owner->prev
  let fuel ← fuelM
  for _ in [0:fuel] do
    let st ← get
    let g ← match owner with
      | none => pure st.pending_gotos
      | some ow => pure (st.syms[ow]!).prev
    if g == 0 then break
    let gx ← getSym g
    if !(gx.c > os.cl_n) then break
    let lbl ← getSym gx.prev_tok
    let mut remove := false
    if lbl.r &&& LABEL_FORWARD != 0 then
      let pcl := gx.next
      if jmp == 0 then jmp ← gjmp_acs 0
      gsym R (← getSym pcl).jnext
      try_call_scope_cleanup R os.cl_s
      let j ← gjmp_acs 0
      modSym pcl fun y => { y with aux := j.toUInt32 }
      if os.cl_n == 0 then remove := true
      else
        let n := os.cl_n
        modSym g fun y => { y with c := n }
        owner := some g
    else remove := true
    if remove then
      match owner with
      | none => modify fun s => { s with pending_gotos := gx.prev }
      | some ow => modSym ow fun y => { y with prev := gx.prev }
      sym_free g
  gsym R jmp
  try_call_scope_cleanup R os.cl_s

/-! ## VLA and scopes -/

/-- `vla_restore(loc)` -/
def vla_restore (loc : Int32) : M Unit := do
  if loc != 0 then gen_vla_sp_restore loc

/-- `vla_leave(o)` -/
def vla_leave (o : Nat) : M Unit := do
  let mut c := (← get).cur_scope
  let mut v : Nat := 0
  let fuel ← fuelM
  for _ in [0:fuel] do
    if c == o || c == 0 then break
    let cs ← getScope c
    if cs.vla_num != 0 then v := c
    c := cs.prev
  if v != 0 then vla_restore (← getScope v).vla_locorig

/-- a fresh scope object (block()'s `struct scope o`) -/
def newScopeObj : M Nat := modifyGet fun s => (s.scopes.size, { s with scopes := s.scopes.push {} })

/-- `new_scope(o)` -/
def new_scope (o : Nat) : M Unit := do
  let s ← get
  let cur := s.scopes[s.cur_scope]!
  modScope o fun _ => { cur with prev := s.cur_scope, vla_num := 0, lstk := s.local_stack, llstk := s.local_label_stack }
  modify fun s => { s with cur_scope := o, local_scope := s.local_scope + 1 }

/-- `prev_scope(o, is_expr)` -/
def prev_scope (R : Rec) (o : Nat) (is_expr : Bool) : M Unit := do
  let os ← getScope o
  vla_leave os.prev
  if os.cl_s != (← getScope os.prev).cl_s then block_cleanup R os.prev
  let nt ← label_pop R (← get).local_label_stack os.llstk is_expr
  modify fun s => { s with local_label_stack := nt }
  let ns ← sym_pop (← get).local_stack os.lstk is_expr
  modify fun s => { s with local_stack := ns, cur_scope := os.prev, local_scope := s.local_scope - 1 }

/-- `leave_scope(o)` -/
def leave_scope (R : Rec) (o : Nat) : M Unit := do
  if o == 0 then return
  try_call_scope_cleanup R (← getScope o).cl_s
  vla_leave o

/-- `new_scope_s(o)` -/
def new_scope_s (o : Nat) : M Unit := do
  let ls := (← get).local_stack
  modScope o fun x => { x with lstk := ls }
  modify fun s => { s with local_scope := s.local_scope + 1 }

/-- `prev_scope_s(o)` -/
def prev_scope_s (o : Nat) : M Unit := do
  let ns ← sym_pop (← get).local_stack (← getScope o).lstk false
  modify fun s => { s with local_stack := ns, local_scope := s.local_scope - 1 }

/-- `lblock(bsym, csym)` (cells; 0 is NULL) -/
def lblock (R : Rec) (bsym csym : Nat) : M Unit := do
  let lo := (← get).loop_scope
  let co := (← get).cur_scope
  let cs ← getScope co
  let b := cs.bsym
  let c := cs.csym
  if csym != 0 then
    modScope co fun x => { x with csym := csym }
    modify fun s => { s with loop_scope := co }
  modScope co fun x => { x with bsym := bsym }
  R.block 0
  modScope co fun x => { x with bsym := b }
  if csym != 0 then
    modScope co fun x => { x with csym := c }
    modify fun s => { s with loop_scope := lo }

/-- `gexpr_decl()` -/
def gexpr_decl (R : Rec) : M Unit := do
  let v ← R.decl VT_JMP
  if v > 1 && (← getTok) != 59 then
    let s ← sym_find v
    let sx ← getSym s
    vset R sx.type sx.r.toInt32' (if sx.r.toInt32' &&& VT_SYM != 0 then 0 else sx.c)
    modVt 0 fun x => x.setSym s
  else
    if v != 0 then skip R 59
    R.gexpr

/-! ## block -/

/-- `switch` statement -/
def block_switch (R : Rec) : M Unit := do
  let a ← newCell 0
  let st ← get
  let swi := st.switches.size
  let sw0 : SwitchT := { bsym := a, scope := st.cur_scope, prev := st.cur_switch, nocode_wanted := st.nocode_wanted }
  modify fun s => { s with switches := s.switches.push sw0, cur_switch := swi }
  let o ← newScopeObj
  new_scope_s o
  skip R 40
  gexpr_decl R
  if !is_integer_btype ((← vtt) &&& VT_BTYPE) then tcc_error_s R "switch value not an integer"
  skip R 41
  let sv ← vt
  modify fun s => { s with switches := s.switches.modify swi fun w => { w with sv := sv } }
  vtopDec
  setCell a 0
  let b ← gjmp_acs 0
  lblock R a 0
  setCell a (← gjmp_acs (← getCell a))
  gsym R b
  prev_scope_s o
  if (← get).switches[swi]!.nocode_wanted == 0 then
    case_sort R swi
    modify fun s => { s with switches := s.switches.modify swi fun w => { w with bsym := 0 } }
    vpushv R (← get).switches[swi]!.sv
    let _ ← R.gv RC_INT
    let sw := (← get).switches[swi]!
    let d ← gcase (← fuelM) R sw.p 0 sw.p.size 0
    vpop R
    let sw := (← get).switches[swi]!
    if sw.def_sym != 0 then gsym_addr R d sw.def_sym
    else gsym R d
  gsym R (← getCell a)
  end_switch

/-- `goto` statement -/
def block_goto (R : Rec) : M Unit := do
  vla_restore (← getScope (← get).cur_scope).vla_locorig
  if (← getTok) == 42 && (← get).gnu_ext != 0 then
    R.next
    R.gexpr
    if (← vtt) &&& VT_BTYPE != VT_PTR then expect R "pointer"
    ggoto R
  else if (← getTok) ≥ C.TOK_UIDENT then
    let tk ← getTok
    let mut s ← label_find tk
    if s == 0 then
      s ← label_push true tk LABEL_FORWARD.toUInt32.toInt32
    else if (← getSym s).r == LABEL_DECLARED then
      let s' := s
      modSym s' fun y => { y with r := LABEL_FORWARD }
    if (← getSym s).r &&& LABEL_FORWARD != 0 then
      let cs ← getScope (← get).cur_scope
      if cs.cl_s != 0 && (← get).nocode_wanted == 0 then
        let pg ← sym_push2 (← get).pending_gotos SYM_FIELD 0 cs.cl_n
        modify fun st => { st with pending_gotos := pg }
        let s' := s
        modSym pg fun y => { y with prev_tok := s' }
        let s2 ← sym_push2 (← getSym s).next SYM_FIELD 0 0
        modSym s' fun y => { y with next := s2 }
        modSym pg fun y => { y with next := s2 }
        s := s2
      let j ← gjmp_acs (← getSym s).jnext
      let s' := s
      modSym s' fun y => { y with aux := j.toUInt32 }
    else
      try_call_cleanup_goto R (← getSym s).next
      gjmp_addr_acs (← getSym s).jind
    R.next
  else
    expect R "label identifier"
  skip R 59

/-- a label `t:` (returns after `block_after_label` processing is left to the caller) -/
def block_label (R : Rec) (t : Int32) : M Unit := do
  R.next
  let mut s ← label_find t
  if s != 0 then
    let sx ← getSym s
    if sx.r == LABEL_DEFINED then
      tcc_error R (cfmt "duplicate label '%s'" [.s (← get_tok_str sx.v none)])
    let s' := s
    modSym s' fun y => { y with r := LABEL_DEFINED }
    if sx.next != 0 then
      let mut pcl := sx.next
      let fuel ← fuelM
      for _ in [0:fuel] do
        if pcl == 0 then break
        let px ← getSym pcl
        gsym R px.jnext
        pcl := px.prev
      let ns ← sym_pop sx.next 0 false
      modSym s' fun y => { y with next := ns }
    else
      gsym R sx.jnext
  else
    s ← label_push true t LABEL_DEFINED.toUInt32.toInt32
  let j ← gind
  let cls := (← getScope (← get).cur_scope).cl_s
  let s' := s
  modSym s' fun y => { y with aux := j.toUInt32, next := cls }

/-- `case` label -/
def block_case (R : Rec) : M Unit := do
  let st ← get
  if st.cur_switch == 0 then expect R "switch"
  let tt := st.switches[st.cur_switch]!.sv.type.t
  let v1 := value64 (← R.expr_const64).toUInt64 tt
  let mut cr : CaseT := { v1 := v1.toInt64, v2 := v1.toInt64 }
  if (← getTok) == TOK_DOTS && (← get).gnu_ext != 0 then
    R.next
    let v2 := value64 (← R.expr_const64).toUInt64 tt
    cr := { cr with v2 := v2.toInt64 }
    if case_cmp (tt &&& VT_UNSIGNED != 0) cr.v2.toUInt64 cr.v1.toUInt64 < 0 then
      tcc_warning R "empty case range".toUTF8
  let csw := (← get).cur_switch
  if (← get).switches[csw]!.nocode_wanted == 0 then
    cr := { cr with ind := ← gind }
  cr := { cr with line := (← curFile).line_num }
  let cr' := cr
  modify fun s => { s with switches := s.switches.modify csw fun w => { w with p := w.p.push cr' } }
  skip R 58

/-- `return` statement -/
def block_return (R : Rec) : M Unit := do
  let s ← get
  let mut b := s.func_vt.t &&& VT_BTYPE != VT_VOID
  if (← getTok) != 59 then
    R.gexpr
    if b then gen_assign_cast R (← get).func_vt
    else
      if (← vtt) != VT_VOID then tcc_warning R "void function returns a value".toUTF8
      vtopDec
  else if b && s.func_old != 0 && s.func_vt.t &&& VT_BTYPE == VT_INT then
    vpushi R 0
  else if b then
    tcc_warning R "'return' with no value".toUTF8
    b := false
  leave_scope R (← get).root_scope
  if b then gfunc_return R (← get).func_vt
  skip R 59
  if (← getTok) != 125 || (← get).local_scope != 1 then
    let r ← gjmp_acs (← get).rsym
    modify fun s => { s with rsym := r }
  CODE_OFF

/-- one pass of `block(flags)`: returns true for `goto again` -/
def blockOnce (R : Rec) (flags : Int32) : M Bool := do
  let t ← getTok
  let mut afterLabel := false
  let mut isExpr := false
  if TOK_HAS_VALUE t then
    isExpr := true
  else
    R.next
  if isExpr then
    pure ()
  else if t == TOK_IF then
    let o ← newScopeObj
    new_scope_s o
    skip R 40
    gexpr_decl R
    let a ← gvtst R 1 0
    skip R 41
    R.block 0
    if (← getTok) == TOK_ELSE then
      let d ← gjmp_acs 0
      gsym R a
      R.next
      R.block 0
      gsym R d
    else
      gsym R a
    prev_scope_s o
  else if t == TOK_WHILE then
    let o ← newScopeObj
    new_scope_s o
    let d ← gind
    skip R 40
    R.gexpr
    let a ← newCell (← gvtst R 1 0)
    skip R 41
    let b ← newCell 0
    lblock R a b
    gjmp_addr_acs d
    gsym_addr R (← getCell b) d
    gsym R (← getCell a)
    prev_scope_s o
  else if t == 123 then
    let o ← newScopeObj
    new_scope o
    let fuel ← fuelM
    for _ in [0:fuel] do
      if (← getTok) != TOK_LABEL then break
      for _ in [0:fuel] do
        R.next
        let tk ← getTok
        if tk < C.TOK_UIDENT then expect R "label identifier"
        let _ ← label_push false tk LABEL_DECLARED.toUInt32.toInt32
        R.next
        if (← getTok) != 44 then break
      skip R 59
    for _ in [0:fuel] do
      if (← getTok) == 125 then break
      let _ ← R.decl VT_LOCAL
      if (← getTok) != 125 then
        R.block (flags ||| STMT_COMPOUND)
    prev_scope R o (flags &&& STMT_EXPR != 0)
    if (← get).local_scope != 0 then R.next
    else if (← get).nocode_wanted == 0 then check_func_return R
  else if t == TOK_RETURN then
    block_return R
  else if t == TOK_BREAK then
    let cs ← getScope (← get).cur_scope
    if cs.bsym == 0 then tcc_error_s R "cannot break"
    let st ← get
    if st.cur_switch != 0 && cs.bsym == st.switches[st.cur_switch]!.bsym then
      leave_scope R st.switches[st.cur_switch]!.scope
    else
      leave_scope R st.loop_scope
    let cs ← getScope (← get).cur_scope
    setCell cs.bsym (← gjmp_acs (← getCell cs.bsym))
    skip R 59
  else if t == TOK_CONTINUE then
    let cs ← getScope (← get).cur_scope
    if cs.csym == 0 then tcc_error_s R "cannot continue"
    leave_scope R (← get).loop_scope
    let cs ← getScope (← get).cur_scope
    setCell cs.csym (← gjmp_acs (← getCell cs.csym))
    skip R 59
  else if t == TOK_FOR then
    let o ← newScopeObj
    new_scope o
    skip R 40
    if (← getTok) != 59 then
      if (← R.decl VT_JMP) == 0 then
        R.gexpr
        vpop R
    skip R 59
    let a ← newCell 0
    let b ← newCell 0
    let c ← gind
    let mut d := c
    if (← getTok) != 59 then
      R.gexpr
      setCell a (← gvtst R 1 0)
    skip R 59
    if (← getTok) != 41 then
      let e ← gjmp_acs 0
      d ← gind
      R.gexpr
      vpop R
      gjmp_addr_acs c
      gsym R e
    skip R 41
    lblock R a b
    gjmp_addr_acs d
    gsym_addr R (← getCell b) d
    gsym R (← getCell a)
    prev_scope R o false
  else if t == TOK_DO then
    let o ← newScopeObj
    new_scope_s o
    let a ← newCell 0
    let b ← newCell 0
    let d ← gind
    lblock R a b
    gsym R (← getCell b)
    skip R TOK_WHILE
    skip R 40
    R.gexpr
    let c ← gvtst R 0 0
    skip R 41
    skip R 59
    gsym_addr R c d
    gsym R (← getCell a)
    prev_scope_s o
  else if t == TOK_SWITCH then
    block_switch R
  else if t == TOK_CASE then
    block_case R
    afterLabel := true
  else if t == TOK_DEFAULT then
    let st ← get
    if st.cur_switch == 0 then expect R "switch"
    let csw := st.cur_switch
    if st.switches[csw]!.def_sym != 0 then tcc_error_s R "too many 'default'"
    let ds ← if st.switches[csw]!.nocode_wanted != 0 then pure (-1) else gind
    modify fun s => { s with switches := s.switches.modify csw fun w => { w with def_sym := ds } }
    skip R 58
    afterLabel := true
  else if t == TOK_GOTO then
    block_goto R
  else if t == TOK_ASM1 || t == TOK_ASM2 || t == TOK_ASM3 then
    R.asm_instr
  else
    if (← getTok) == 58 && t ≥ C.TOK_UIDENT then
      block_label R t
      afterLabel := true
    else
      if t != 59 then
        unget_tok t
        isExpr := true
  if afterLabel then
    let _ ← parse_attribute R {}
    vla_restore (← getScope (← get).cur_scope).vla_loc
    if (← getTok) != 125 then
      if flags &&& STMT_COMPOUND == 0 then return true
    else
      tcc_warning_c R WARNC_all "deprecated use of label at end of compound statement".toUTF8
  if isExpr then
    if flags &&& STMT_EXPR != 0 then
      vpop R
      R.gexpr
    else
      R.gexpr
      vpop R
    skip R 59
  return false

/-- `block(flags)` -/
def block (R : Rec) (flags : Int32) : M Unit := do
  let fuel ← fuelM
  for _ in [0:fuel] do
    if !(← blockOnce R flags) then return
  throw (.fuel "block")

end Tcc.Model
