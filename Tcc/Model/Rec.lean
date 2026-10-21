import Tcc.Model.State

/-!
# TccModel: the recursion record

tcc's functions call each other recursively across files (next →
preprocess → expr_preprocess → expr_const → unary → next; unary → block →
decl → unary; gen_op ↔ the code generator; ...). The model keeps every
function a separate, non-recursive Lean definition that takes `R : Rec`
and makes its backward or cross-module calls through `R`'s fields (the
calls C makes through prototypes). `Tie.lean` ties the knot with fuel:
`tie (n+1)` fills the fields with the definitions applied to `tie n`, so
nesting depth is bounded by the fuel and every function is total.

Module order (each imports the previous ones): Base, Types, State, Rec,
Host, Libtcc, PpLex, Syms, PpDir, PpMacro, Elf, Riscv (riscv64-gen.c:
calls tccgen only through `R`), Gen* (tccgen.c: calls the backend
directly), Link/Run (tccelf.c/riscv64-link.c/tccrun.c/libtcc.c's file
layer), Tie, ModelMain.
-/

namespace Tcc.Model

structure Rec where
  -- tccpp.c
  next : M Unit
  next_nomacro : M Unit
  preprocess : Int32 → M Unit
  pp_error : M ByteArray
  macro_subst : TokenString → Option SymId → TokPtr → M (TokenString × Option SymId × Int32)
  -- tccgen.c: what riscv64-gen.c and the preprocessor call
  expr_const : M Int32
  gv : Int32 → M Int32
  gv2 : Int32 → Int32 → M Unit
  get_reg : Int32 → M Int32
  save_reg_upstack : Int32 → Int32 → M Unit
  save_regs : Int32 → M Unit
  vrotb : Int32 → M Unit
  vrott : Int32 → M Unit
  vswap : M Unit
  vpushi : Int32 → M Unit
  vpushv : SValue → M Unit
  /-- `vset(type, r, v)` -/
  vset : CType → Int32 → Int32 → M Unit
  vstore : M Unit
  indir : M Unit
  gen_op : Int32 → M Unit
  gaddrof : M Unit
  test_lvalue : M Unit
  /-- `type_size(type, &align)`: (size, align) -/
  type_size : CType → M (Int32 × Int32)
  /-- `greloca(s, sym, offset, type, addend)` -/
  greloca : SecId → SymId → UInt64 → Int32 → Int64 → M Unit
  /-- `put_extern_sym(sym, section, value, size)` -/
  put_extern_sym : SymId → SecId → UInt64 → UInt64 → M Unit
  vpush_helper_func : Int32 → M Unit
  /-- `gfunc_set_param(s, c, byref)` -/
  gfunc_set_param : SymId → Int32 → Int32 → M SymId
  vset_VT_CMP : Int32 → M Unit
  gsym : Int32 → M Unit
  -- tccgen.c: forward references inside tccgen
  gen_cast : CType → M Unit
  gen_cast_s : Int32 → M Unit
  is_compatible_types : CType → CType → M Bool
  is_compatible_unqualified_types : CType → CType → M Bool
  parse_btype : AttributeDef → Int32 → M (Bool × CType × AttributeDef)
  /-- `type_decl(type, ad, &v, td)`: (type, ad, v) -/
  type_decl : CType → AttributeDef → Int32 → M (CType × AttributeDef × Int32)
  parse_expr_type : M CType
  /-- `init_putv(p, type, c)` with p = (sec, local_offset, flex_array_ref) -/
  init_putv : SecId → Int32 → SymId → CType → UInt64 → M Unit
  decl_initializer : SecId → Int32 → SymId → CType → UInt64 → Int32 → M Unit
  /-- `decl_initializer_alloc(type, ad, r, has_init, v, scope)` -/
  decl_initializer_alloc : CType → AttributeDef → Int32 → Int32 → Int32 → Int32 → M Unit
  decl : Int32 → M Int32
  block : Int32 → M Unit
  expr_eq : M Unit
  gexpr : M Unit
  unary : M Unit
  vpush_type_size : CType → M Int32
  expr_const64 : M Int64
  gvtst : Int32 → Int32 → M Int32
  gen_inline_functions : M Unit
  skip_or_save_block : M (Option TokArr)
  gv_dup : M Unit
  end_switch : M Unit
  do_Static_assert : M Unit
  gfunc_call : Int32 → M Unit
  /-- `vpop()` (riscv64-gen.c's gfunc_call and gen_vla_alloc) -/
  vpop : M Unit
  -- libtcc.c / tccelf.c
  /-- `tcc_compile(s1, filetype, filename, fd)` -/
  tcc_compile : Int32 → ByteArray → Int32 → M Int32
  /-- `tcc_add_file_internal(s1, filename, flags)` -/
  tcc_add_file_internal : ByteArray → Int32 → M Int32

end Tcc.Model
