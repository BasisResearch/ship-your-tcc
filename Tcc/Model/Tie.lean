import Tcc.Model.Gen8
import Tcc.Model.Main

/-!
# TccModel: tying the knot

`tie (n+1)` is the record of all of tcc's functions, each applied to
`tie n` (behind a `Thunk`, so each level is built once, and only as deep
as calls actually nest). `tie 0` throws `.fuel`. Every field is
eta-expanded over the state so that building a record evaluates nothing.
-/

namespace Tcc.Model
namespace Tie

def fuelErr {α : Type} : M α := throw (.fuel "recursion depth")

/-- the record at depth 0 -/
def bot : Rec where
  tcc_set_options := fun _ => fuelErr
  next := fuelErr
  next_nomacro := fuelErr
  preprocess := fun _ => fuelErr
  pp_error := fuelErr
  macro_subst := fun _ _ _ => fuelErr
  expr_const := fuelErr
  gv := fun _ => fuelErr
  gv2 := fun _ _ => fuelErr
  get_reg := fun _ => fuelErr
  save_reg_upstack := fun _ _ => fuelErr
  save_regs := fun _ => fuelErr
  vrotb := fun _ => fuelErr
  vrott := fun _ => fuelErr
  vswap := fuelErr
  vpushi := fun _ => fuelErr
  vpushv := fun _ => fuelErr
  vset := fun _ _ _ => fuelErr
  vstore := fuelErr
  indir := fuelErr
  gen_op := fun _ => fuelErr
  gaddrof := fuelErr
  test_lvalue := fuelErr
  type_size := fun _ => fuelErr
  greloca := fun _ _ _ _ _ => fuelErr
  put_extern_sym := fun _ _ _ _ => fuelErr
  vpush_helper_func := fun _ => fuelErr
  gfunc_set_param := fun _ _ _ => fuelErr
  vset_VT_CMP := fun _ => fuelErr
  gsym := fun _ => fuelErr
  gen_cast := fun _ => fuelErr
  gen_cast_s := fun _ => fuelErr
  is_compatible_types := fun _ _ => fuelErr
  is_compatible_unqualified_types := fun _ _ => fuelErr
  parse_btype := fun _ _ => fuelErr
  type_decl := fun _ _ _ => fuelErr
  parse_expr_type := fuelErr
  init_putv := fun _ _ _ _ _ => fuelErr
  decl_initializer := fun _ _ _ _ _ _ => fuelErr
  decl_initializer_alloc := fun _ _ _ _ _ _ => fuelErr
  decl := fun _ => fuelErr
  block := fun _ => fuelErr
  expr_eq := fuelErr
  gexpr := fuelErr
  unary := fuelErr
  vpush_type_size := fun _ => fuelErr
  expr_const64 := fuelErr
  gvtst := fun _ _ => fuelErr
  gen_inline_functions := fuelErr
  skip_or_save_block := fuelErr
  gv_dup := fuelErr
  end_switch := fuelErr
  do_Static_assert := fuelErr
  gfunc_call := fun _ => fuelErr
  asm_instr := fuelErr
  asm_global_instr := fuelErr
  vpop := fuelErr
  tcc_compile := fun _ _ _ => fuelErr
  tcc_add_file_internal := fun _ _ => fuelErr

/-- the record at depth n+1 from the one at depth n -/
def step (r : Thunk Rec) : Rec where
  tcc_set_options := fun p s => (Tcc.Model.tcc_set_options r.get p).run s
  next := fun s => (Tcc.Model.next r.get).run s
  next_nomacro := fun s => (Tcc.Model.next_nomacro r.get).run s
  preprocess := fun b s => (Tcc.Model.preprocess r.get b).run s
  pp_error := fun s => (Tcc.Model.pp_error r.get).run s
  macro_subst := fun ts nl ms s => (Tcc.Model.macro_subst r.get ts nl ms).run s
  expr_const := fun s => (Tcc.Model.expr_const r.get).run s
  gv := fun rc s => (Tcc.Model.gv r.get rc).run s
  gv2 := fun a b s => (Tcc.Model.gv2 r.get a b).run s
  get_reg := fun rc s => (Tcc.Model.get_reg r.get rc).run s
  save_reg_upstack := fun a n s => (Tcc.Model.save_reg_upstack r.get a n).run s
  save_regs := fun n s => (Tcc.Model.save_regs r.get n).run s
  vrotb := fun n s => (Tcc.Model.vrotb r.get n).run s
  vrott := fun n s => (Tcc.Model.vrott r.get n).run s
  vswap := fun s => (Tcc.Model.vswap r.get).run s
  vpushi := fun v s => (Tcc.Model.vpushi r.get v).run s
  vpushv := fun v s => (Tcc.Model.vpushv r.get v).run s
  vset := fun t a v s => (Tcc.Model.vset r.get t a v).run s
  vstore := fun s => (Tcc.Model.vstore r.get).run s
  indir := fun s => (Tcc.Model.indir r.get).run s
  gen_op := fun op s => (Tcc.Model.gen_op r.get op).run s
  gaddrof := fun s => (Tcc.Model.gaddrof).run s
  test_lvalue := fun s => (Tcc.Model.test_lvalue r.get).run s
  type_size := fun t s => (do Tcc.Model.type_size (← fuelM) t).run s
  greloca := fun a b c d e s => (Tcc.Model.greloca a b c d e).run s
  put_extern_sym := fun a b c d s => (Tcc.Model.put_extern_sym a b c d).run s
  vpush_helper_func := fun v s => (Tcc.Model.vpush_helper_func r.get v).run s
  gfunc_set_param := fun a b c s => (Tcc.Model.gfunc_set_param a b c).run s
  vset_VT_CMP := fun op s => (Tcc.Model.vset_VT_CMP op).run s
  gsym := fun t s => (Tcc.Model.gsym r.get t).run s
  gen_cast := fun t s => (Tcc.Model.gen_cast r.get t).run s
  gen_cast_s := fun t s => (Tcc.Model.gen_cast_s r.get t).run s
  is_compatible_types := fun a b s => (Tcc.Model.is_compatible_types r.get a b).run s
  is_compatible_unqualified_types := fun a b s => (Tcc.Model.is_compatible_unqualified_types r.get a b).run s
  parse_btype := fun ad il s => (Tcc.Model.parse_btype r.get ad il).run s
  type_decl := fun t ad td s => (Tcc.Model.type_decl r.get t ad td).run s
  parse_expr_type := fun s => (Tcc.Model.parse_expr_type r.get).run s
  init_putv := fun sec lo fr t c s =>
    (Tcc.Model.init_putv r.get { sec := sec, local_offset := lo, flex_array_ref := fr } t c).run s
  decl_initializer := fun sec lo fr t c fl s =>
    (Tcc.Model.decl_initializer r.get { sec := sec, local_offset := lo, flex_array_ref := fr } t c fl).run s
  decl_initializer_alloc := fun t ad a hi v sc s => (Tcc.Model.decl_initializer_alloc r.get t ad a hi v sc).run s
  decl := fun l s => (Tcc.Model.decl r.get l).run s
  block := fun f s => (Tcc.Model.block r.get f).run s
  expr_eq := fun s => (Tcc.Model.expr_eq r.get).run s
  gexpr := fun s => (Tcc.Model.gexpr r.get).run s
  unary := fun s => (Tcc.Model.unary r.get).run s
  vpush_type_size := fun t s => (Tcc.Model.vpush_type_size r.get t).run s
  expr_const64 := fun s => (Tcc.Model.expr_const64 r.get).run s
  gvtst := fun i t s => (Tcc.Model.gvtst r.get i t).run s
  gen_inline_functions := fun s => (Tcc.Model.gen_inline_functions r.get).run s
  skip_or_save_block := fun s => (Tcc.Model.skip_or_save_block r.get true).run s
  gv_dup := fun s => (Tcc.Model.gv_dup r.get).run s
  end_switch := fun s => (Tcc.Model.end_switch).run s
  do_Static_assert := fun s => (Tcc.Model.do_Static_assert r.get).run s
  gfunc_call := fun n s => (Tcc.Model.gfunc_call r.get n).run s
  asm_instr := fun s => (unsupported "asm statement" : M Unit).run s
  asm_global_instr := fun s => (unsupported "global asm block" : M Unit).run s
  vpop := fun s => (Tcc.Model.vpop r.get).run s
  tcc_compile := fun ft fn fd s => (Tcc.Model.tcc_compile r.get ft fn fd).run s
  tcc_add_file_internal := fun f fl s => (Tcc.Model.tcc_add_file_internal r.get f fl).run s

/-- the knot: nesting depth bounded by `n` -/
def tie : Nat → Rec
  | 0 => bot
  | n + 1 => step (Thunk.mk fun _ => tie n)

end Tie
end Tcc.Model
