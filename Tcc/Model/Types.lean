import Std.Data.HashMap
import Tcc.Model.Base
import Tcc.Model.Tok
import Tcc.Model.Ghost

/-!
# TccModel: tcc's data structures and global state

One Lean structure per C structure of tcc (tcc.h), with pointers as indices
into stores held by the state (`SymId` into `TccSt.syms`, `SecId` into
`TccSt.secs`; 0 is NULL), and C unions kept as the raw bits they share
where tcc reads one member after writing another (`CValue`, the
`cmp_op/cmp_r/sym` union of `SValue`, `Sym.c`/`enum_val`).

`TccSt` mirrors tcc's globals (tccpp.c, tccgen.c, riscv64-gen.c, tccelf.c,
libtcc.c, tccrun.c) and the `TCCState` fields the -run path uses. `M` is
the state-and-error monad: `tcc_error`'s longjmp is `throw .longjmp`, with
the state at the throw kept (as longjmp keeps globals).
-/

namespace Tcc.Model

abbrev SymId := Nat
abbrev SecId := Nat
abbrev TokArr := Array Int32

/-! ## tcc.h constants -/

namespace C
-- value locations (SValue.r)
def VT_VALMASK : Int32 := 0x003f
def VT_CONST : Int32 := 0x0030
def VT_LLOCAL : Int32 := 0x0031
def VT_LOCAL : Int32 := 0x0032
def VT_CMP : Int32 := 0x0033
def VT_JMP : Int32 := 0x0034
def VT_JMPI : Int32 := 0x0035
def VT_LVAL : Int32 := 0x0100
def VT_SYM : Int32 := 0x0200
def VT_MUSTCAST : Int32 := 0x0C00
def VT_NONCONST : Int32 := 0x1000
def VT_MUSTBOUND : Int32 := 0x4000
def VT_BOUNDED : Int32 := 0x8000
-- types
def VT_BTYPE : Int32 := 0x000f
def VT_VOID : Int32 := 0
def VT_BYTE : Int32 := 1
def VT_SHORT : Int32 := 2
def VT_INT : Int32 := 3
def VT_LLONG : Int32 := 4
def VT_PTR : Int32 := 5
def VT_FUNC : Int32 := 6
def VT_STRUCT : Int32 := 7
def VT_FLOAT : Int32 := 8
def VT_DOUBLE : Int32 := 9
def VT_LDOUBLE : Int32 := 10
def VT_BOOL : Int32 := 11
def VT_QLONG : Int32 := 13
def VT_QFLOAT : Int32 := 14
def VT_UNSIGNED : Int32 := 0x0010
def VT_DEFSIGN : Int32 := 0x0020
def VT_ARRAY : Int32 := 0x0040
def VT_BITFIELD : Int32 := 0x0080
def VT_CONSTANT : Int32 := 0x0100
def VT_VOLATILE : Int32 := 0x0200
def VT_RESTRICT : Int32 := 0x0400
def VT_QUAL : Int32 := 0x0700
def VT_VLA : Int32 := 0x0800
def VT_LONG : Int32 := 0x1000
def VT_EXTERN : Int32 := 0x00002000
def VT_STATIC : Int32 := 0x00004000
def VT_TYPEDEF : Int32 := 0x00008000
def VT_INLINE : Int32 := 0x00010000
def VT_TLS : Int32 := 0x00020000
def VT_STRUCT_SHIFT : Int32 := 20
def VT_STRUCT_MASK : Int32 := 0xfff00080
def VT_UNION : Int32 := 0x00100007
def VT_ENUM : Int32 := 0x00200000
def VT_ENUM_VAL : Int32 := 0x00300000
def VT_ATOMIC : Int32 := 0x0200
def VT_STORAGE : Int32 := 0x00002000 ||| 0x00004000 ||| 0x00008000 ||| 0x00010000 ||| 0x00020000
def VT_TYPE : Int32 := ~~~(VT_STORAGE ||| VT_STRUCT_MASK)
def VT_ASM : Int32 := 0x00400000
def VT_ASM_FUNC : Int32 := 0x00500000
def VT_BT_ARRAY : Int32 := 0x00600000
@[inline] def BIT_POS (t : Int32) : Int32 := (t >>> 20) &&& 0x3f
@[inline] def BIT_SIZE (t : Int32) : Int32 := (t >>> 26) &&& 0x3f
@[inline] def IS_ENUM (t : Int32) : Bool := (t &&& VT_STRUCT_MASK) == VT_ENUM
@[inline] def IS_ENUM_VAL (t : Int32) : Bool := (t &&& VT_STRUCT_MASK) == VT_ENUM_VAL
@[inline] def IS_UNION (t : Int32) : Bool := (t &&& (VT_STRUCT_MASK ||| VT_BTYPE)) == VT_UNION
@[inline] def IS_ASM_FUNC (t : Int32) : Bool := (t &&& (VT_BTYPE ||| VT_STRUCT_MASK)) == VT_ASM_FUNC
@[inline] def IS_BT_ARRAY (t : Int32) : Bool := (t &&& VT_STRUCT_MASK) == VT_BT_ARRAY

def SYM_STRUCT : Int32 := 0x40000000
def SYM_FIELD : Int32 := 0x20000000
def SYM_FIRST_ANOM : Int32 := 0x10000000

def FUNC_NEW : UInt32 := 1
def FUNC_OLD : UInt32 := 2
def FUNC_ELLIPSIS : UInt32 := 3
def FUNC_CDECL : UInt32 := 0

def MACRO_OBJ : Int32 := 0
def MACRO_FUNC : Int32 := 1
def MACRO_JOIN : Int32 := 2

def LABEL_DEFINED : UInt16 := 0
def LABEL_FORWARD : UInt16 := 1
def LABEL_DECLARED : UInt16 := 2
def LABEL_GONE : UInt16 := 3

def TYPE_ABSTRACT : Int32 := 1
def TYPE_DIRECT : Int32 := 2
def TYPE_PARAM : Int32 := 4
def TYPE_NEST : Int32 := 8

def IO_BUF_SIZE : Nat := 8192
def CH_EOB : Int32 := 92   -- '\\'
def CH_EOF : Int32 := -1

def INCLUDE_STACK_SIZE : Nat := 32
def IFDEF_STACK_SIZE : Nat := 64
def VSTACK_SIZE : Nat := 512
def STRING_MAX_SIZE : Nat := 1024
def TOKSTR_MAX_SIZE : Nat := 256
def PACK_STACK_SIZE : Nat := 8
def TOK_HASH_SIZE : Nat := 16384
def TOK_ALLOC_INCR : Nat := 512
def TOK_MAX_SIZE : Nat := 4
def CACHED_INCLUDES_HASH_SIZE : Nat := 32

-- token values (tcc.h)
def TOK_LAND : Int32 := 0x90
def TOK_LOR : Int32 := 0x91
def TOK_ULT : Int32 := 0x92
def TOK_UGE : Int32 := 0x93
def TOK_EQ : Int32 := 0x94
def TOK_NE : Int32 := 0x95
def TOK_ULE : Int32 := 0x96
def TOK_UGT : Int32 := 0x97
def TOK_Nset : Int32 := 0x98
def TOK_Nclear : Int32 := 0x99
def TOK_LT : Int32 := 0x9c
def TOK_GE : Int32 := 0x9d
def TOK_LE : Int32 := 0x9e
def TOK_GT : Int32 := 0x9f
@[inline] def TOK_ISCOND (t : Int32) : Bool := t ≥ TOK_LAND && t ≤ TOK_GT
def TOK_DEC : Int32 := 0x80
def TOK_MID : Int32 := 0x81
def TOK_INC : Int32 := 0x82
def TOK_UDIV : Int32 := 0x83
def TOK_UMOD : Int32 := 0x84
def TOK_PDIV : Int32 := 0x85
def TOK_UMULL : Int32 := 0x86
def TOK_ADDC1 : Int32 := 0x87
def TOK_ADDC2 : Int32 := 0x88
def TOK_SUBC1 : Int32 := 0x89
def TOK_SUBC2 : Int32 := 0x8a
def TOK_SHL : Int32 := 60   -- '<'
def TOK_SAR : Int32 := 62   -- '>'
def TOK_SHR : Int32 := 0x8b
def TOK_NEG : Int32 := 0x81
def TOK_ARROW : Int32 := 0xa0
def TOK_DOTS : Int32 := 0xa1
def TOK_TWODOTS : Int32 := 0xa2
def TOK_TWOSHARPS : Int32 := 0xa3
def TOK_PLCHLDR : Int32 := 0xa4
def TOK_PPJOIN : Int32 := 0xa3 ||| 0x20000000
def TOK_SOTYPE : Int32 := 0xa7
def TOK_A_ADD : Int32 := 0xb0
def TOK_A_SUB : Int32 := 0xb1
def TOK_A_MUL : Int32 := 0xb2
def TOK_A_DIV : Int32 := 0xb3
def TOK_A_MOD : Int32 := 0xb4
def TOK_A_AND : Int32 := 0xb5
def TOK_A_OR : Int32 := 0xb6
def TOK_A_XOR : Int32 := 0xb7
def TOK_A_SHL : Int32 := 0xb8
def TOK_A_SAR : Int32 := 0xb9
@[inline] def TOK_ASSIGN (t : Int32) : Bool := t ≥ TOK_A_ADD && t ≤ TOK_A_SAR
def TOK_ASSIGN_OP (t : Int32) : Int32 :=
  let s : Array Int32 := #[43, 45, 42, 47, 37, 38, 124, 94, 60, 62]   -- "+-*/%&|^<>"
  s[(t - TOK_A_ADD).toNat']!
def TOK_CCHAR : Int32 := 0xc0
def TOK_LCHAR : Int32 := 0xc1
def TOK_CINT : Int32 := 0xc2
def TOK_CUINT : Int32 := 0xc3
def TOK_CLLONG : Int32 := 0xc4
def TOK_CULLONG : Int32 := 0xc5
def TOK_CLONG : Int32 := 0xc6
def TOK_CULONG : Int32 := 0xc7
def TOK_STR : Int32 := 0xc8
def TOK_LSTR : Int32 := 0xc9
def TOK_CFLOAT : Int32 := 0xca
def TOK_CDOUBLE : Int32 := 0xcb
def TOK_CLDOUBLE : Int32 := 0xcc
def TOK_PPNUM : Int32 := 0xcd
def TOK_PPSTR : Int32 := 0xce
def TOK_LINENUM : Int32 := 0xcf
@[inline] def TOK_HAS_VALUE (t : Int32) : Bool := (t ≥ TOK_CCHAR && t ≤ TOK_U32CHAR) || t == TOK_PPLINE
@[inline] def TOK_IS_STR (t : Int32) : Bool :=
  t == TOK_STR || t == TOK_LSTR || t == TOK_U16STR || t == TOK_U32STR
@[inline] def TOK_STR_SIZE (t : Int32) : Int32 :=
  if t == TOK_STR then 1 else if t == TOK_U16STR then 2 else 4

def TOK_EOF : Int32 := -1
def TOK_LINEFEED : Int32 := 10
def TOK_IDENT : Int32 := 256
def TOK_UIDENT : Int32 := Tok.TOK_DEFINE

def TOK_FLAG_BOL : Int32 := 0x0001
def TOK_FLAG_BOF : Int32 := 0x0002
def TOK_FLAG_ENDIF : Int32 := 0x0004

def PARSE_FLAG_PREPROCESS : Int32 := 0x0001
def PARSE_FLAG_TOK_NUM : Int32 := 0x0002
def PARSE_FLAG_LINEFEED : Int32 := 0x0004
def PARSE_FLAG_ASM_FILE : Int32 := 0x0008
def PARSE_FLAG_SPACES : Int32 := 0x0010
def PARSE_FLAG_ACCEPT_STRAYS : Int32 := 0x0020
def PARSE_FLAG_TOK_STR : Int32 := 0x0040

def IS_SPC : UInt8 := 1
def IS_ID : UInt8 := 2
def IS_NUM : UInt8 := 4

-- output types (libtcc.h)
def TCC_OUTPUT_MEMORY : Int32 := 1
def TCC_OUTPUT_EXE : Int32 := 2
def TCC_OUTPUT_DLL : Int32 := 4
def TCC_OUTPUT_OBJ : Int32 := 3
def TCC_OUTPUT_PREPROCESS : Int32 := 5

-- file types (tcc.h)
def AFF_PRINT_ERROR : Int32 := 0x10
def AFF_REFERENCED_DLL : Int32 := 0x20
def AFF_TYPE_BIN : Int32 := 0x40
def AFF_WHOLE_ARCHIVE : Int32 := 0x80
def AFF_TYPE_NONE : Int32 := 0
def AFF_TYPE_C : Int32 := 1
def AFF_TYPE_ASM : Int32 := 2
def AFF_TYPE_ASMPP : Int32 := 4
def AFF_TYPE_LIB : Int32 := 8
def AFF_TYPE_MASK : Int32 := 7 ||| 0x40
def AFF_BINTYPE_REL : Int32 := 1
def AFF_BINTYPE_DYN : Int32 := 2
def AFF_BINTYPE_AR : Int32 := 3
def FILE_NOT_FOUND : Int32 := -2
def FILE_NOT_RECOGNIZED : Int32 := -3

-- riscv64-gen.c (with patches/0001 TCC_RV64I_SOFT)
def NB_REGS : Nat := 19
def RC_INT : Int32 := 1
def RC_FLOAT : Int32 := 2
@[inline] def RC_R (x : Int32) : Int32 := (1 : Int32) <<< (2 + x)
@[inline] def RC_F (x : Int32) : Int32 := (1 : Int32) <<< (10 + x)
def RC_IRET : Int32 := 4
def RC_IRE2 : Int32 := 8
def RC_FRET : Int32 := 4
def REG_IRET : Int32 := 0
def REG_IRE2 : Int32 := 1
def REG_FRET : Int32 := 0
def TREG_RA : Int32 := 17
def TREG_SP : Int32 := 18
def PTR_SIZE : Int32 := 8
def LONG_SIZE : Int32 := 8
def LDOUBLE_SIZE : Int32 := 16
def LDOUBLE_ALIGN : Int32 := 16
def MAX_ALIGN : Int32 := 16
def reg_classes : Array Int32 :=
  #[RC_INT ||| RC_FLOAT ||| RC_R 0, RC_INT ||| RC_FLOAT ||| RC_R 1, RC_INT ||| RC_FLOAT ||| RC_R 2,
    RC_INT ||| RC_FLOAT ||| RC_R 3, RC_INT ||| RC_FLOAT ||| RC_R 4, RC_INT ||| RC_FLOAT ||| RC_R 5,
    RC_INT ||| RC_FLOAT ||| RC_R 6, RC_INT ||| RC_FLOAT ||| RC_R 7,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0x20000, 0x40000]

-- tccgen.c
def DATA_ONLY_WANTED : Int32 := Int32.ofInt (-0x80000000)
def CODE_OFF_BIT : Int32 := 0x20000000
def NOEVAL_MASK : Int32 := 0x0000FFFF
def CONST_WANTED_BIT : Int32 := 0x00010000
def CONST_WANTED_MASK : Int32 := 0x0FFF0000
def VT_SIZE_T : Int32 := 0x1000 ||| 4 ||| 0x10
def VT_PTRDIFF_T : Int32 := 0x1000 ||| 4
def STMT_EXPR : Int32 := 1
def STMT_COMPOUND : Int32 := 2
def MAX_TEMP_LOCAL_VARIABLE_NUMBER : Nat := 8

-- ELF
def SHT_NULL : Int32 := 0
def SHT_PROGBITS : Int32 := 1
def SHT_SYMTAB : Int32 := 2
def SHT_STRTAB : Int32 := 3
def SHT_RELA : Int32 := 4
def SHT_HASH : Int32 := 5
def SHT_DYNAMIC : Int32 := 6
def SHT_NOTE : Int32 := 7
def SHT_NOBITS : Int32 := 8
def SHT_REL : Int32 := 9
def SHT_DYNSYM : Int32 := 11
def SHT_INIT_ARRAY : Int32 := 14
def SHT_FINI_ARRAY : Int32 := 15
def SHT_PREINIT_ARRAY : Int32 := 16
def SHT_GROUP : Int32 := 17
def SHT_SYMTAB_SHNDX : Int32 := 18
def SHT_GNU_HASH : Int32 := 0x6ffffff6
def SHT_GNU_verdef : Int32 := 0x6ffffffd
def SHT_GNU_verneed : Int32 := 0x6ffffffe
def SHT_GNU_versym : Int32 := 0x6fffffff
def SHT_RISCV_ATTRIBUTES : Int32 := 0x70000003
def SHF_WRITE : Int32 := 1
def SHF_ALLOC : Int32 := 2
def SHF_EXECINSTR : Int32 := 4
def SHF_MERGE : Int32 := 0x10
def SHF_STRINGS : Int32 := 0x20
def SHF_INFO_LINK : Int32 := 0x40
def SHF_GROUP : Int32 := 0x200
def SHF_TLS : Int32 := 0x400
def SHF_PRIVATE : Int32 := Int32.ofInt (-0x80000000)
def SHF_DYNSYM : Int32 := 0x40000000
def SHN_UNDEF : Int32 := 0
def SHN_LORESERVE : Int32 := 0xff00
def SHN_ABS : Int32 := 0xfff1
def SHN_COMMON : Int32 := 0xfff2
def STB_LOCAL : Int32 := 0
def STB_GLOBAL : Int32 := 1
def STB_WEAK : Int32 := 2
def STT_NOTYPE : Int32 := 0
def STT_OBJECT : Int32 := 1
def STT_FUNC : Int32 := 2
def STT_SECTION : Int32 := 3
def STT_FILE : Int32 := 4
def STT_COMMON : Int32 := 5
def STT_TLS : Int32 := 6
def STT_GNU_IFUNC : Int32 := 10
def STV_DEFAULT : Int32 := 0
def STV_INTERNAL : Int32 := 1
def STV_HIDDEN : Int32 := 2
def STV_PROTECTED : Int32 := 3
@[inline] def ELF_ST_BIND (i : Int32) : Int32 := (i &&& 0xff) >>> 4
@[inline] def ELF_ST_TYPE (i : Int32) : Int32 := i &&& 0xf
@[inline] def ELF_ST_INFO (b t : Int32) : Int32 := (b <<< 4) + (t &&& 0xf)
@[inline] def ELF_ST_VISIBILITY (o : Int32) : Int32 := o &&& 0x3
def ELFSYM_SIZE : Nat := 24
def ELFRELA_SIZE : Nat := 24
end C

/-! ## CType, CValue, SValue -/

structure CType where
  t : Int32 := 0
  ref : SymId := 0
  deriving Inhabited, BEq

/-- `union CValue` (16 bytes on rv64): `i`/`d`/`f` share `lo`, `ld` is
`lo`/`hi`, `tab[0..3]` are the four 32-bit words, `str.size` is the low
word of `hi`; `str.data` (a pointer) is kept as the bytes it points to. -/
structure CValue where
  lo : UInt64 := 0
  hi : UInt64 := 0
  sdata : ByteArray := ByteArray.empty
  deriving Inhabited

namespace CValue
@[inline] def i (c : CValue) : UInt64 := c.lo
@[inline] def setI (c : CValue) (v : UInt64) : CValue := { c with lo := v }
@[inline] def ofI (v : UInt64) : CValue := { lo := v }
@[inline] def jtrue (c : CValue) : Int32 := c.lo.toUInt32.toInt32
@[inline] def jfalse (c : CValue) : Int32 := (c.lo >>> (32 : UInt64)).toUInt32.toInt32
@[inline] def setJtrue (c : CValue) (v : Int32) : CValue :=
  { c with lo := (c.lo &&& (0xffffffff00000000 : UInt64)) ||| v.toUInt32.toUInt64 }
@[inline] def setJfalse (c : CValue) (v : Int32) : CValue :=
  { c with lo := (c.lo &&& (0xffffffff : UInt64)) ||| (v.toUInt32.toUInt64 <<< (32 : UInt64)) }
@[inline] def strSize (c : CValue) : Int32 := c.hi.toUInt32.toInt32
@[inline] def setStr (c : CValue) (data : ByteArray) (size : Int32) : CValue :=
  { c with sdata := data, hi := (c.hi &&& (0xffffffff00000000 : UInt64)) ||| size.toUInt32.toUInt64 }
/-- the 32-bit word `tab[k]` -/
@[inline] def tab (c : CValue) (k : Nat) : UInt32 :=
  match k with
  | 0 => c.lo.toUInt32 | 1 => (c.lo >>> (32 : UInt64)).toUInt32
  | 2 => c.hi.toUInt32 | _ => (c.hi >>> (32 : UInt64)).toUInt32
@[inline] def setTab (c : CValue) (k : Nat) (v : UInt32) : CValue :=
  match k with
  | 0 => { c with lo := (c.lo &&& (0xffffffff00000000 : UInt64)) ||| v.toUInt64 }
  | 1 => { c with lo := (c.lo &&& (0xffffffff : UInt64)) ||| (v.toUInt64 <<< (32 : UInt64)) }
  | 2 => { c with hi := (c.hi &&& (0xffffffff00000000 : UInt64)) ||| v.toUInt64 }
  | _ => { c with hi := (c.hi &&& (0xffffffff : UInt64)) ||| (v.toUInt64 <<< (32 : UInt64)) }
/-- float bits (`c.f`) -/
@[inline] def fbits (c : CValue) : UInt32 := c.lo.toUInt32
@[inline] def setFbits (c : CValue) (v : UInt32) : CValue :=
  { c with lo := (c.lo &&& (0xffffffff00000000 : UInt64)) ||| v.toUInt64 }
end CValue

/-- `SValue`: `u` is the union of `{cmp_op, cmp_r}` and `sym`. -/
structure SValue where
  type : CType := {}
  r : UInt16 := 0
  r2 : UInt16 := 0
  c : CValue := {}
  u : UInt64 := 0
  deriving Inhabited

namespace SValue
@[inline] def sym (v : SValue) : SymId := v.u.toNat
@[inline] def setSym (v : SValue) (s : SymId) : SValue := { v with u := s.toUInt64 }
@[inline] def cmp_op (v : SValue) : Int32 := (v.u &&& (0xffff : UInt64)).toUInt32.toInt32
@[inline] def cmp_r (v : SValue) : Int32 := ((v.u >>> (16 : UInt64)) &&& (0xffff : UInt64)).toUInt32.toInt32
@[inline] def setCmp (v : SValue) (op r : Int32) : SValue :=
  { v with u := (v.u &&& (0xffffffff00000000 : UInt64)) ||| (op.toUInt32.toUInt64 &&& (0xffff : UInt64)) |||
      ((r.toUInt32.toUInt64 &&& (0xffff : UInt64)) <<< (16 : UInt64)) }
@[inline] def jtrue (v : SValue) : Int32 := v.c.jtrue
@[inline] def jfalse (v : SValue) : Int32 := v.c.jfalse
end SValue

/-! ## Sym -/

/-- `struct Sym`. `a` holds the SymAttr bit-fields (aligned:5, packed:1,
weak:1, visibility:2, dllexport:1, nodecorate:1, dllimport:1, addrtaken:1,
nodebug:1); `aux` is the union of sym_scope / jnext / jind / FuncAttr f /
auxtype, and with `c` forms `enum_val`; `d`, `e` and `vla_array_str` are
the token-stream pointers of the other union members (tcc reads them only
through the member it wrote). -/
structure Sym where
  v : Int32 := 0
  r : UInt16 := 0
  a : UInt16 := 0
  c : Int32 := 0
  aux : UInt32 := 0
  d : Option TokArr := none
  cleanup_func : SymId := 0
  type : CType := {}
  next : SymId := 0
  e : Option TokArr := none
  vla_array_str : Option TokArr := none
  prev : SymId := 0
  prev_tok : SymId := 0
  deriving Inhabited

namespace Sym
@[inline] def sym_scope (s : Sym) : Int32 := s.aux.toInt32
@[inline] def jnext (s : Sym) : Int32 := s.aux.toInt32
@[inline] def jind (s : Sym) : Int32 := s.aux.toInt32
@[inline] def auxtype (s : Sym) : Int32 := s.aux.toInt32
@[inline] def enum_val (s : Sym) : Int64 :=
  ((s.aux.toUInt64 <<< (32 : UInt64)) ||| s.c.toUInt32.toUInt64).toInt64
@[inline] def setEnumVal (s : Sym) (v : Int64) : Sym :=
  { s with c := v.toUInt64.toUInt32.toInt32, aux := (v.toUInt64 >>> (32 : UInt64)).toUInt32 }
@[inline] def asm_label (s : Sym) : Int32 := (Int32.ofNat s.next)
-- FuncAttr f: func_call:3, func_type:2, func_noreturn:1, func_ctor:1, func_dtor:1, func_args:8, func_alwinl:1
@[inline] def f_func_call (s : Sym) : UInt32 := s.aux &&& 7
@[inline] def f_func_type (s : Sym) : UInt32 := (s.aux >>> 3) &&& 3
@[inline] def f_func_noreturn (s : Sym) : UInt32 := (s.aux >>> 5) &&& 1
@[inline] def f_func_ctor (s : Sym) : UInt32 := (s.aux >>> 6) &&& 1
@[inline] def f_func_dtor (s : Sym) : UInt32 := (s.aux >>> 7) &&& 1
@[inline] def f_func_args (s : Sym) : UInt32 := (s.aux >>> 8) &&& 0xff
@[inline] def f_func_alwinl (s : Sym) : UInt32 := (s.aux >>> 16) &&& 1
-- SymAttr a
@[inline] def a_aligned (s : Sym) : UInt16 := s.a &&& 0x1f
@[inline] def a_packed (s : Sym) : UInt16 := (s.a >>> 5) &&& 1
@[inline] def a_weak (s : Sym) : UInt16 := (s.a >>> 6) &&& 1
@[inline] def a_visibility (s : Sym) : UInt16 := (s.a >>> 7) &&& 3
@[inline] def a_dllexport (s : Sym) : UInt16 := (s.a >>> 9) &&& 1
@[inline] def a_nodecorate (s : Sym) : UInt16 := (s.a >>> 10) &&& 1
@[inline] def a_dllimport (s : Sym) : UInt16 := (s.a >>> 11) &&& 1
@[inline] def a_addrtaken (s : Sym) : UInt16 := (s.a >>> 12) &&& 1
@[inline] def a_nodebug (s : Sym) : UInt16 := (s.a >>> 13) &&& 1
end Sym

/-- set the bit-field (width `w` at `pos`) of a word -/
@[inline] def setBits32 (x : UInt32) (pos w : UInt32) (v : UInt32) : UInt32 :=
  let m : UInt32 := ((1 <<< w) - 1) <<< pos
  (x &&& ~~~m) ||| ((v <<< pos) &&& m)
@[inline] def setBits16 (x : UInt16) (pos w : UInt16) (v : UInt16) : UInt16 :=
  let m : UInt16 := ((1 <<< w) - 1) <<< pos
  (x &&& ~~~m) ||| ((v <<< pos) &&& m)

/-- `struct AttributeDef` -/
structure AttributeDef where
  a : UInt16 := 0            -- SymAttr
  f : UInt32 := 0            -- FuncAttr
  sec : SecId := 0
  cleanup_func : SymId := 0
  alias_target : Int32 := 0
  asm_label : Int32 := 0
  attr_mode : Int32 := 0
  new_section : Int32 := 0
  deriving Inhabited

/-! ## tokens and preprocessor -/

structure TokenSym where
  sym_define : SymId := 0
  sym_label : SymId := 0
  sym_struct : SymId := 0
  sym_identifier : SymId := 0
  tok : Int32 := 0
  canonical : Int32 := 0
  str : ByteArray := ByteArray.empty
  deriving Inhabited

/-- a pointer into a token array (`const int *`) -/
structure TokPtr where
  arr : TokArr := #[]
  pos : Nat := 0
  deriving Inhabited

@[inline] def TokPtr.get (p : TokPtr) (k : Nat := 0) : Int32 := p.arr[p.pos + k]?.getD 0

/-- `TokenString` used as an accumulator (tok_str_add & co.): `str` holds
`len` valid words. -/
structure TokenString where
  str : TokArr := #[]
  len : Int32 := 0
  need_spc : Int32 := 0
  allocated_len : Int32 := 0
  last_line_num : Int32 := -1
  deriving Inhabited

/-- which TokenString a macro-stack frame belongs to (tccpp.c's statics
`tokstr_buf`/`unget_buf` are reset by end_macro; others are freed) -/
inductive TSRef where
  | tokstrBuf | ungetBuf | heap
  deriving Inhabited, BEq

/-- an entry of `macro_stack` (begin_macro/end_macro) -/
structure MacroFrame where
  ref : TSRef := .heap
  id : Nat := 0
  prev_ptr : Option TokPtr := none
  save_line_num : Int32 := 0
  save_tok_line : Int32 := 0
  alloc : Int32 := 0
  /-- the frame's string (`str->str`) -/
  str : TokPtr := {}
  deriving Inhabited

/-- `BufferedFile`: the whole input is in `buf` (tcc reads 8 KiB blocks;
the bytes it sees are the same); `buf[off ..]` holds the data, with
`off` bytes of room in front for tcc's ungets (`*--p = c`), and
`buf[buf_end] = CH_EOB`. -/
structure BufferedFile where
  buf : ByteArray := ByteArray.empty
  buf_ptr : Nat := 0
  buf_end : Nat := 0
  bufStart : Nat := 0          -- index of `buffer[0]`
  fd : Int32 := -1
  trigraph_pending : Nat := 0  -- trailing question marks across physical reads
  line_num : Int32 := 1
  line_ref : Int32 := 0
  ifndef_macro : Int32 := 0
  ifndef_macro_saved : Int32 := 0
  ifdef_stack_ptr : Nat := 0
  include_next_index : Int32 := 0
  prev_tok_flags : Int32 := 0
  filename : ByteArray := ByteArray.empty
  true_filename : ByteArray := ByteArray.empty
  id : Nat := 0
  deriving Inhabited

structure CachedInclude where
  ifndef_macro : Int32 := 0
  once : Int32 := 0
  hash_next : Int32 := 0
  filename : ByteArray := ByteArray.empty
  deriving Inhabited

/-! ## ELF sections -/

structure Section where
  data_offset : Nat := 0
  data : ByteArray := ByteArray.empty     -- size = data_allocated
  sh_name : Int32 := 0
  sh_num : Int32 := 0
  sh_type : Int32 := 0
  sh_flags : Int32 := 0
  sh_info : Int32 := 0
  sh_addralign : Int32 := 0
  sh_entsize : Int32 := 0
  sh_size : UInt64 := 0
  sh_addr : UInt64 := 0
  sh_offset : UInt64 := 0
  nb_hashed_syms : Int32 := 0
  link : SecId := 0
  reloc : SecId := 0
  hash : SecId := 0
  prev : SecId := 0
  name : ByteArray := ByteArray.empty
  deriving Inhabited

/-- `struct sym_attr` -/
structure SymAttrX where
  got_offset : UInt32 := 0
  plt_offset : UInt32 := 0
  plt_sym : Int32 := 0
  dyn_index : Int32 := 0
  linker_sym : UInt8 := 0
  deriving Inhabited

/-! ## tccgen's local structures -/

structure CaseT where
  v1 : Int64 := 0
  v2 : Int64 := 0
  ind : Int32 := 0
  line : Int32 := 0
  deriving Inhabited

/-- `struct switch_t`; `bsym` is a cell (see `TccSt.cells`) -/
structure SwitchT where
  p : Array CaseT := #[]
  def_sym : Int32 := 0
  nocode_wanted : Int32 := 0
  bsym : Nat := 0
  scope : Nat := 0
  sv : SValue := {}
  prev : Nat := 0
  deriving Inhabited

/-- `struct scope`; `bsym`/`csym` are cells (0 = NULL) -/
structure Scope where
  prev : Nat := 0
  vla_loc : Int32 := 0
  vla_locorig : Int32 := 0
  vla_num : Int32 := 0
  cl_s : SymId := 0
  cl_n : Int32 := 0
  bsym : Nat := 0
  csym : Nat := 0
  lstk : SymId := 0
  llstk : SymId := 0
  deriving Inhabited

/-- Patch 0003's active union member at one initialized subobject. -/
structure InitUnion where
  type : SymId := 0
  member : SymId := 0
  addr : UInt64 := 0
  deriving Inhabited

structure TempLocal where
  location : Int32 := 0
  size : Int32 := 0
  align : Int32 := 0
  deriving Inhabited

structure InlineFunc where
  func_str : Option TokArr := none
  sym : SymId := 0
  filename : ByteArray := ByteArray.empty
  deriving Inhabited

/-! ## errors and the monad -/

inductive Err where
  /-- tcc_error's longjmp to the setjmp in tcc_compile (the message is
      already on the console) -/
  | longjmp
  /-- the program called exit(n) (tcc_error outside a compilation) -/
  | exit (code : Int32)
  /-- the model's fuel ran out -/
  | fuel (where_ : String)
  /-- a construct the model does not cover yet (never a tcc behaviour) -/
  | unsupported (what : String)
  /-- an internal inconsistency of the model -/
  | internal (what : String)
  deriving Inhabited

end Tcc.Model
