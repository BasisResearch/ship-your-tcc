import Tcc.Model.PpLex

/-!
# TccModel: tcc's symbol allocator and token table

From tccgen.c: `sym_malloc`/`sym_free` (a free list, as tcc's pools),
`sym_push2`, `sym_find2`, the token-table lookups (`sym_find`,
`struct_find`, `label_find`, `define_find`); from tccpp.c: `tok_alloc`.
-/

namespace Tcc.Model
open C

/-- `sym_malloc()` -/
def sym_malloc : M SymId := do
  let s ← get
  if s.sym_free_first != 0 then
    let id := s.sym_free_first
    set { s with sym_free_first := s.syms[id]!.next }
    return id
  else
    modifyGet fun s =>
      let a := s.syms
      let s := { s with syms := #[] }
      (a.size, { s with syms := a.push default })

/-- `sym_free(sym)` -/
def sym_free (sym : SymId) : M Unit := do
  let ff := (← get).sym_free_first
  modSym sym fun x => { x with next := ff }
  modify fun s => { s with sym_free_first := sym }

/-- `sym_push2(ps, v, t, c)` with `ps` the stack top; returns the new
symbol (the caller stores it as the new top) -/
def sym_push2 (top : SymId) (v t c : Int32) : M SymId := do
  let s ← sym_malloc
  modSym s fun _ => { v := v, type := { t := t }, c := c, prev := top }
  return s

/-- `sym_find2(s, v)` -/
def sym_find2 (s0 : SymId) (v : Int32) : M SymId := do
  let fuel ← fuelM
  let mut s := s0
  for _ in [0:fuel] do
    if s == 0 then return 0
    let x ← getSym s
    if x.v == v then return s
    s := x.prev
  return 0

@[inline] def tokIdx (v : Int32) : M (Option Nat) := do
  let s ← get
  let i := v - TOK_IDENT
  if i < 0 || i ≥ s.tok_ident - TOK_IDENT then return none
  return some i.toNat'

/-- `struct_find(v)` -/
def struct_find (v : Int32) : M SymId := do
  match ← tokIdx v with
  | some i => return (← get).table_ident[i]!.sym_struct
  | none => return 0

/-- `sym_find(v)` -/
def sym_find (v : Int32) : M SymId := do
  match ← tokIdx v with
  | some i => return (← get).table_ident[i]!.sym_identifier
  | none => return 0

/-- `label_find(v)` -/
def label_find (v : Int32) : M SymId := do
  match ← tokIdx v with
  | some i => return (← get).table_ident[i]!.sym_label
  | none => return 0

/-- `define_find(v)` -/
def define_find (v : Int32) : M SymId := do
  match ← tokIdx v with
  | some i => return (← get).table_ident[i]!.sym_define
  | none => return 0

/-- `tok_alloc_new(pts, str, len)` -/
def tok_alloc_new (R : Rec) (str : ByteArray) : M TokenSym := do
  let s ← get
  if s.tok_ident ≥ SYM_FIRST_ANOM then tcc_error_s R "memory full (symbols)"
  let ts : TokenSym := { tok := s.tok_ident, str := str }
  modify fun s =>
    let a := s.table_ident
    let h := s.hash_ident
    let s := { s with table_ident := #[], hash_ident := {} }
    { s with table_ident := a.push ts, hash_ident := h.insert str ts.tok, tok_ident := s.tok_ident + 1 }
  return ts

/-- `tok_alloc(str, len)` -/
def tok_alloc (R : Rec) (str : ByteArray) : M TokenSym := do
  let s ← get
  match s.hash_ident.get? str with
  | some t => return s.table_ident[(t - TOK_IDENT).toNat']!
  | none => tok_alloc_new R str

/-- `tok_alloc_const(str)` -/
def tok_alloc_const (R : Rec) (str : String) : M Int32 := do
  return (← tok_alloc R str.toUTF8).tok

end Tcc.Model
