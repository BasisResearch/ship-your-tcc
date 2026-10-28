import Tcc.Model.Types
import Tcc.Model.GhostCabs

/-!
# TccModel: the global state `TccSt` and the monad `M`

`TccSt` is tcc's globals: the `TCCState` fields the -run path reads, and
the file-level statics of tccpp.c, tccgen.c, riscv64-gen.c, tccelf.c and
tccrun.c, under their C names. Pointer-valued globals are indices into the
stores (`syms`, `secs`, `scopes`, `switches`, `cells`).

The image's environment is part of the state as read-only inputs: the
embedded file system (`fsFiles`, htif.c), and the image addresses tcc's
-run link uses (`imageSyms`: patches/0002's `tcc_baremetal_syms`, `rt_exit`,
the arena `__tccrun_start/end`). The console (`console`) collects what tcc
writes to stdout/stderr (both are the HTIF console).
-/

namespace Tcc.Model

/-- an open file descriptor of htif.c's file system -/
structure FdEntry where
  data : ByteArray := ByteArray.empty
  pos : Nat := 0
  isDir : Bool := false
  deriving Inhabited

structure TccSt where
  -- ===== inputs =====
  /-- the image's files: absolute normalised path → contents -/
  fsFiles : Std.HashMap ByteArray ByteArray := {}
  /-- the image's directories (absolute normalised paths, "" is the root) -/
  fsDirs : Std.HashMap ByteArray Unit := {}
  /-- image symbol addresses: name → address -/
  imageSyms : Std.HashMap String UInt64 := {}
  /-- the loop bound (fuel) of every C loop, from the input size -/
  fuel : Nat := 0
  /-- Enable phase-4 pragma observation; ordinary compilation leaves this off. -/
  observePP : Bool := false
  ppObserved : Array PPObservation := #[]
  ppSeen : Std.HashMap Nat Unit := {}
  -- ===== host environment =====
  fds : Array (Option FdEntry) := #[]
  console : ByteArray := ByteArray.empty
  nextId : Nat := 1
  -- ===== TCCState =====
  verbose : Int32 := 0
  nostdinc : Int32 := 0
  nostdlib : Int32 := 0
  nostdlib_paths : Int32 := 0
  nocommon : Int32 := 1
  static_link : Int32 := 0
  filetype : Int32 := 0
  optimize : Int32 := 0
  option_pthread : Int32 := 0
  cversion : Int32 := 199901
  char_is_unsigned : Int32 := 1
  leading_underscore : Int32 := 0
  ms_extensions : Int32 := 1
  dollars_in_identifiers : Int32 := 1
  ms_bitfields : Int32 := 0
  reverse_funcargs : Int32 := 0
  gnu89_inline : Int32 := 0
  unwind_tables : Int32 := 1
  warn_none : Int32 := 0
  warn_all : Int32 := 0
  warn_error : Int32 := 0
  warn_write_strings : Int32 := 0
  warn_unsupported : Int32 := 0
  warn_implicit_function_declaration : Int32 := 1
  warn_discarded_qualifiers : Int32 := 1
  /-- tcc_warning_c's selector (0: none; else which warn_ field) -/
  warn_num : Int32 := 0
  do_debug : Int32 := 0
  do_backtrace : Int32 := 0
  test_coverage : Int32 := 0
  gnu_ext : Int32 := 1
  tcc_ext : Int32 := 1
  dflag : Int32 := 0
  tcc_lib_path : ByteArray := ByteArray.empty
  elf_entryname : Option ByteArray := none
  output_type : Int32 := 0
  output_format : Int32 := 0
  run_test : Int32 := 0
  include_paths : Array ByteArray := #[]
  sysinclude_paths : Array ByteArray := #[]
  library_paths : Array ByteArray := #[]
  crt_paths : Array ByteArray := #[]
  cmdline_defs : ByteArray := ByteArray.empty
  cmdline_incl : ByteArray := ByteArray.empty
  error_set_jmp_enabled : Int32 := 0
  nb_errors : Int32 := 0
  /-- include_stack: the ids of the includer files -/
  include_stack : Array Nat := #[]
  ifdef_stack : Array Int32 := Array.replicate 64 0
  ifdef_stack_ptr : Nat := 0
  cached_includes_hash : Array Int32 := Array.replicate 32 0
  cached_includes : Array CachedInclude := #[]
  pack_stack : Array Int32 := Array.replicate 8 0
  pack_stack_ptr : Nat := 0
  pragma_libs : Array ByteArray := #[]
  inline_fns : Array InlineFunc := #[]
  /-- the section store; index 0 unused -/
  secs : Array Section := #[default]
  sections : Array SecId := #[]
  priv_sections : Array SecId := #[]
  text_section : SecId := 0
  data_section : SecId := 0
  rodata_section : SecId := 0
  bss_section : SecId := 0
  common_section : SecId := 0
  cur_text_section : SecId := 0
  symtab_section : SecId := 0
  dynsymtab_section : SecId := 0
  dynsym : SecId := 0
  got : SecId := 0
  plt : SecId := 0
  eh_frame_section : SecId := 0
  eh_frame_hdr_section : SecId := 0
  eh_start : UInt64 := 0
  sym_attrs : Array SymAttrX := #[]
  pcrel_hi_entries : Array (UInt64 × UInt64) := #[]
  run_main : ByteArray := ByteArray.empty
  run_ptr : UInt64 := 0
  run_size : UInt64 := 0
  current_filename : Option ByteArray := none
  /-- s1->files: (type, name) -/
  files : Array (Int32 × ByteArray) := #[]
  nb_libraries : Int32 := 0
  /-- the program's argv after -run -/
  run_argv : Array ByteArray := #[]
  total_idents : Int32 := 0
  total_lines : Int32 := 0
  total_bytes : UInt32 := 0
  -- ===== tccpp.c =====
  tok_flags : Int32 := 0
  parse_flags : Int32 := 0
  /-- the BufferedFile chain, current first (`file`, `file->prev`, ...) -/
  file : List BufferedFile := []
  tok : Int32 := 0
  /-- Presumed line of the token start, independent of physical lookahead. -/
  tok_line : Int32 := 0
  tokc : CValue := {}
  macro_ptr : Option TokPtr := none
  tokcstr : ByteArray := ByteArray.empty
  tok_ident : Int32 := 0
  table_ident : Array TokenSym := #[]
  tok_pragma_op : Int32 := 0
  /-- One emission bit per opaque -E output event, indexed by its identity. -/
  pragma_emitted : TokenString := {}
  pp_expr : Int32 := 0
  hash_ident : Std.HashMap ByteArray Int32 := {}
  tokstr_buf : TokenString := {}
  unget_buf : TokenString := {}
  isidnum_table : Array UInt8 := Array.replicate 257 0
  pp_counter : Int32 := 0
  pp_debug_tok : Int32 := 0
  pp_debug_symv : Int32 := 0
  macro_stack : List MacroFrame := []
  -- ===== tccgen.c =====
  rsym : Int32 := 0
  anon_sym : Int32 := 0
  ind : Int32 := 0
  loc : Int32 := 0
  global_stack : SymId := 0
  local_stack : SymId := 0
  define_stack : SymId := 0
  global_label_stack : SymId := 0
  local_label_stack : SymId := 0
  sym_free_first : SymId := 0
  /-- the Sym store; index 0 is NULL -/
  syms : Array Sym := #[default]
  all_cleanups : SymId := 0
  pending_gotos : SymId := 0
  local_scope : Int32 := 0
  debug_modes : Int32 := 0
  /-- `_vstack[1 + VSTACK_SIZE]`: `vtop` indexes it, `vstack` is index 1 -/
  vstack : Array SValue := Array.replicate 513 default
  vtop : Nat := 0
  nocode_wanted : Int32 := 0
  global_expr : Int32 := 0
  func_vt : CType := {}
  func_var : Int32 := 0
  func_vc : Int32 := 0
  func_ind : Int32 := 0
  func_old : Int32 := 0
  funcname : ByteArray := ByteArray.empty
  int_type : CType := {}
  func_old_type : CType := {}
  char_type : CType := {}
  char_pointer_type : CType := {}
  initstr : ByteArray := ByteArray.empty
  /-- The current init_params.unions callback context. Nested initializer
  allocations save/restore this list, just as C passes a distinct init_params. -/
  init_unions : Array InitUnion := #[]
  cur_switch : Nat := 0
  switches : Array SwitchT := #[default]
  arr_temp_local_vars : Array TempLocal := Array.replicate 8 default
  nb_temp_local_vars : Int32 := 0
  cur_scope : Nat := 0
  loop_scope : Nat := 0
  root_scope : Nat := 0
  scopes : Array Scope := #[default]
  /-- `int` cells for tcc's pointers to local `int`s (jump chains) -/
  cells : Array Int32 := #[0]
  prec : Array UInt8 := Array.replicate 256 0
  -- ===== riscv64-gen.c =====
  func_bound_offset : UInt64 := 0
  func_bound_ind : UInt64 := 0
  func_bound_add_epilog : Int32 := 0
  func_sub_sp_offset : Int32 := 0
  num_va_regs : Int32 := 0
  func_va_list_ofs : Int32 := 0
  -- ===== tccrun.c =====
  bm_code_size : UInt64 := 0
  /-- the arena's bytes (from run_ptr) as relocated -/
  arena : ByteArray := ByteArray.empty
  -- ===== ghost =====
  /-- Optional positioned token fixture. No AST/digest/oracle fields. -/
  ghostEnabled : Bool := false
  ghostFixture : Bool := false
  ghostParsing : Bool := false
  ghostNextDepth : Nat := 0
  ghostStringCapture : Bool := false
  ghostLexOffset : Nat := 0
  ghostLexPhysicalStart : Option Nat := none
  ghostLexCharStart : Option Nat := none
  ghostLexSourceId : Nat := 0
  ghostPriorOrigin : Nat := 0
  ghostTokOrigin : Nat := 0
  ghostExpansionOrigin : Nat := 0
  ghostOriginTable : Array Ghost.OriginRecord := #[⟨.unavailable,none,false⟩]
  ghostSymD : Std.HashMap SymId Ghost.OriginMap := {}
  ghostSymE : Std.HashMap SymId Ghost.OriginMap := {}
  ghostArgIndex : Std.HashMap SymId Nat := {}
  ghostLastOrigins : Ghost.OriginMap := #[]
  ghostSaved : Array Ghost.SavedTokens := #[]
  ghostReplay : Std.HashMap Nat (Array (Nat × Nat)) := {}
  ghostFdSources : Std.HashMap Nat Ghost.OpenedFile := {}
  ghostDirective : Option CSem.Pre.SourceSpan := none
  ghostSources : Array Ghost.ModelSource := #[]
  ghostClosedPhysical : Std.HashMap Nat (Array Ghost.PhysicalChar) := {}
  ghostOrigins : Array Ghost.ModelOrigin := #[]
  ghostPragmas : Array Ghost.PragmaEffect := #[]
  ghostInput : Option Ghost.Input := none
  ghostCurrent : Nat := 0
  ghostPrior : Nat := 0
  ghost : Array Ghost.Fragment := #[]
  ghostAudit : Array Ghost.Fragment := #[]
  ghostNormalizations : Array Ghost.NormalizationUse := #[]
  ghostDrops : Array Ghost.Drop := #[]
  ghostTokenErrors : Array String := #[]
  deriving Inhabited

abbrev M := EStateM Err TccSt

namespace M
@[inline] def throwE (e : Err) : M α := throw e
end M

/-- a fresh identity -/
def freshId : M Nat := modifyGet fun s => (s.nextId, { s with nextId := s.nextId + 1 })

/-- the loop bound -/
@[inline] def fuelM : M Nat := do return (← get).fuel

def unsupported (what : String) : M α := throw (.unsupported what)
def internal (what : String) : M α := throw (.internal what)

/-! ## stores -/

@[inline] def getSym (s : SymId) : M Sym := do
  return (← get).syms[s]!
/-- in-place update (the field is taken out first, so the array stays unshared) -/
@[inline] def modSym (i : SymId) (f : Sym → Sym) : M Unit :=
  modify fun st =>
    let a := st.syms
    let st := { st with syms := #[] }
    { st with syms := a.modify i f }
@[inline] def getSec (s : SecId) : M Section := do
  return (← get).secs[s]!
@[inline] def modSec (i : SecId) (f : Section → Section) : M Unit :=
  modify fun st =>
    let a := st.secs
    let st := { st with secs := #[] }
    { st with secs := a.modify i f }
/-- update a section's data in place -/
@[inline] def modSecData (i : SecId) (f : ByteArray → ByteArray) : M Unit :=
  modSec i fun sec =>
    let d := sec.data
    let sec := { sec with data := ByteArray.empty }
    { sec with data := f d }
@[inline] def getCell (c : Nat) : M Int32 := do return (← get).cells[c]!
@[inline] def setCell (c : Nat) (v : Int32) : M Unit :=
  modify fun st =>
    let a := st.cells
    let st := { st with cells := #[] }
    { st with cells := a.set! c v }
def newCell (v : Int32 := 0) : M Nat :=
  modifyGet fun st =>
    let a := st.cells
    let st := { st with cells := #[] }
    (a.size, { st with cells := a.push v })
@[inline] def getScope (c : Nat) : M Scope := do return (← get).scopes[c]!
@[inline] def modScope (c : Nat) (f : Scope → Scope) : M Unit :=
  modify fun st =>
    let a := st.scopes
    let st := { st with scopes := #[] }
    { st with scopes := a.modify c f }

/-! ## the value stack -/

/-- `vtop[k]` -/
@[inline] def vt (k : Int := 0) : M SValue := do
  let s ← get
  return s.vstack[(s.vtop : Int) + k |>.toNat]!
@[inline] def modVt (k : Int) (f : SValue → SValue) : M Unit :=
  modify fun s =>
    let a := s.vstack
    let i := ((s.vtop : Int) + k).toNat
    let s := { s with vstack := #[] }
    { s with vstack := a.modify i f }
@[inline] def setVt (k : Int) (v : SValue) : M Unit := modVt k (fun _ => v)
@[inline] def vtopIdx : M Nat := do return (← get).vtop
/-- `vtop - vstack` (−1 when empty) -/
@[inline] def vdepth : M Int := do return ((← get).vtop : Int) - 1

/-! ## tokens -/

@[inline] def getTok : M Int32 := do return (← get).tok
@[inline] def setTok (t : Int32) : M Unit := modify fun s => { s with tok := t }
@[inline] def tokSym (t : Int32) : M TokenSym := do
  return (← get).table_ident[(t - C.TOK_IDENT).toNat']!
@[inline] def modTokSym (t : Int32) (f : TokenSym → TokenSym) : M Unit :=
  modify fun s =>
    let a := s.table_ident
    let s := { s with table_ident := #[] }
    { s with table_ident := a.modify (t - C.TOK_IDENT).toNat' f }

/-! ## files -/

def curFile : M BufferedFile := do
  match (← get).file with
  | f :: _ => return f
  | [] => internal "no current file"
@[inline] def modFile (f : BufferedFile → BufferedFile) : M Unit :=
  modify fun s =>
    let fl := s.file
    let s := { s with file := [] }
    match fl with
    | b :: r => { s with file := f b :: r }
    | [] => { s with file := [] }
/-- update the current file's buffer in place -/
@[inline] def modFileBuf (f : ByteArray → ByteArray) : M Unit :=
  modFile fun b =>
    let d := b.buf
    let b := { b with buf := ByteArray.empty }
    { b with buf := f d }

/-! ## Typed ghost output (DESIGN §4.1/§6.2)

Actions only modify the named ghost fields. Syntax errors are retained as
Ghost.Gap values and do not change compilation or its output bytes. -/

def gorigin (origin : CSem.Pre.Origin) (anchor : Option Ghost.ModelOrigin)
    (complete : Bool) : M Nat := do
  let s ← get
  if !s.ghostEnabled then return 0
  let id := s.ghostOriginTable.size
  set { s with ghostOriginTable := s.ghostOriginTable.push ⟨origin,anchor,complete⟩ }
  return id

/-- Derivations refer only to origins constructed by the modeled lexer and
macro engine. Missing physical mappings remain explicitly incomplete. -/
def gderive (kind : Ghost.Derivation) (invocation : Nat) (operands : Array Nat := #[]) : M Nat := do
  let s ← get
  if !s.ghostEnabled then return 0
  let inv := s.ghostOriginTable.getD invocation default
  let args := operands.map (fun i => s.ghostOriginTable.getD i default)
  let first := args.getD 0 default
  let origin : CSem.Pre.Origin := match kind with
    | .replacement => .replacement inv.origin first.origin
    | .argument i => .argument inv.origin i first.origin
    | .stringify => .stringify inv.origin (args.toList.map (·.origin))
    | .paste => .paste inv.origin first.origin (args.getD 1 default).origin
    | .builtin => .builtin inv.origin
  let anchor := match kind with | .argument _ => first.anchor | _ => inv.anchor
  gorigin origin anchor (inv.complete && args.all (·.complete))

@[inline] def gmark : M Ghost.Mark := do
  let s ← get
  return ⟨s.ghost.size, s.ghostCurrent⟩

/-- Extend an operator's mark to include its already-parsed left operand. -/
def gleft (m : Ghost.Mark) : M Ghost.Mark := do
  let s ← get
  if m.height == 0 then return m
  return ⟨m.height-1, (s.ghost.getD (m.height-1) default).range.first⟩

/-- Fold exactly the production's children into a typed Cabs fragment. -/
def gwrap (mark : Ghost.Mark) (kind : Ghost.Production) : M Unit :=
  modify fun s =>
    match s.ghostInput with
    | none => s
    | some input =>
      let g := s.ghost
      let m := min mark.height g.size
      let kids := g.extract m g.size
      let last := kids.foldl (fun n f => max n f.range.last) s.ghostCurrent
      let last := if kind == .block && s.tok == 125 && s.local_scope == 0 then last+1 else last
      let range : Ghost.Range := ⟨mark.first, last⟩
      let f : Ghost.Fragment := ⟨kind, range, Ghost.build ⟨input,kind,range,kids⟩⟩
      let normalization : Option Ghost.NormalizationKind := match f.value with
        | .error _ => none
        | .ok _ =>
          if !s.ghostParsing then none
          else if kind == .gnuAttrs then some .gnuAttributeCarrier
          else if kind == .logop then some .logicalChainRestoration
          else if kind == .initDecl && kids.any (fun child => match child.value with
              | .ok (.asmBinding _) => true | _ => false) then some .asmBindingCarrier
          else none
      { s with
        ghost := (g.extract 0 m).push f, ghostAudit := s.ghostAudit.push f,
        ghostNormalizations := match normalization with
          | none => s.ghostNormalizations
          | some kind => s.ghostNormalizations.push ⟨kind,range⟩ }

def gnode (kind : Ghost.Production) (n : Nat := 0) : M Unit := do
  let s ← get
  let h := s.ghost.size - n
  let first := if n == 0 then s.ghostCurrent else (s.ghost.getD h default).range.first
  gwrap ⟨h,first⟩ kind

/-- Only duplicated initializer dry runs and preprocessing expressions may
be dropped; their syntax and the reason remain in the audit. -/
def gdrop (mark : Ghost.Mark) (reason : Ghost.DropReason) : M Unit :=
  modify fun s =>
    if s.ghostInput.isNone then s else
    let m := min mark.height s.ghost.size
    let dropped := s.ghost.extract m s.ghost.size
    { s with
      ghost := s.ghost.extract 0 m,
      ghostDrops := s.ghostDrops.push ⟨⟨mark.first,s.ghostCurrent⟩,reason,dropped⟩ }

/-- console output (stdout and stderr share the HTIF console) -/
def putConsole (b : ByteArray) : M Unit :=
  modify fun s =>
    let c := s.console
    let s := { s with console := ByteArray.empty }
    { s with console := c ++ b }

end Tcc.Model
