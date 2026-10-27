/-
  The implementation-defined parameters of the semantics, as DATA
  (ship-your-tcc, phase F1, DESIGN-COMPILER.md §3.3).

  Upstream Cerberus fixes one implementation, `DefaultImpl` of
  ocaml_frontend/ocaml_implementation.ml (LP64, x86_64-apple-darwin: signed
  `char`, every real floating type held in an OCaml float of 8 bytes). This
  fork makes the choice a parameter: the record `Params` below, read by the
  lem model through the instance reader `impl_params`
  (frontend/model/lean_impl.lem, the `lean_switches.lem` mechanism) — every
  definition that (transitively) consults a layout or signedness fact binds
  `[CerbImplCfg.Impl]`, and the entry point supplies the instance.

  Two values:
  * `cerberusDefault`: upstream's DefaultImpl, field for field (including its
    8-byte `float`/`long double` hack, all three in binary64), so the fork's
    own lanes against the OCaml oracle run unchanged;
  * `tccRV64`: tcc's riscv64 lp64 ABI (vendor/tinycc riscv64-gen.c:
    PTR_SIZE 8, LDOUBLE_SIZE 16, LDOUBLE_ALIGN 16, MAX_ALIGN 16,
    CHAR_IS_UNSIGNED; tcc.h: int 4, short 2, long 8 on lp64), IEEE binary32 /
    binary64 / binary128 floating types, tcc's enum rule (tccgen.c
    struct_decl, enum part) and tcc's bit-field layout (tccgen.c
    struct_layout).

  This is a leaf module: no imports.
-/

namespace CerbImplCfg

/-- An IEEE 754 binary interchange format (the value representation of a
real floating type). -/
inductive FloatFormat where
  | binary32
  | binary64
  | binary128
  deriving BEq, DecidableEq, Repr, Inhabited

/-- Size, alignment and value format of one real floating type. -/
structure FloatTy where
  format : FloatFormat
  /-- `sizeof`, bytes -/
  size : Nat
  /-- `_Alignof`, bytes -/
  align : Nat
  deriving BEq, Repr, Inhabited

/-- How an enumerated type's compatible integer type is chosen. -/
inductive EnumRule where
  /-- GCC without -fshort-enums (ocaml_implementation.ml:130-136): `signed
  int` iff an enumerator is negative, else `unsigned int` -/
  | gcc
  /-- tcc (tccgen.c struct_decl, "set integral type of the enum"): no negative
  enumerator: `unsigned int`, or `unsigned long` when the largest exceeds
  `UINT_MAX`; otherwise `int`, or `long` when an enumerator is outside
  `int`. On enumerators representable in `int` (the only ones C11 §6.7.2.2#2
  allows) both rules agree. -/
  | tcc
  deriving BEq, DecidableEq, Repr, Inhabited

/-- How bit-field members are laid out (and whether they are accepted). -/
inductive BitfieldRule where
  /-- upstream Cerberus: bit-fields are not supported (cabs_to_ail.lem
  `Desugar_NotYetSupported "bit-fields"`) -/
  | unsupported
  /-- tcc's `struct_layout` (tccgen.c) -/
  | tcc
  deriving BEq, DecidableEq, Repr, Inhabited

/-- The implementation-defined parameters (C11 Annex J.3 items the model
consults), one named field each. -/
structure Params where
  /-- a name for diagnostics -/
  name : String
  /-- J.3.4: plain `char` has the range of `signed char` -/
  charSigned : Bool
  /-- J.3.5 widths, bytes (`signed`/`unsigned` alike); `char` is 1 -/
  sizeofShort : Nat
  sizeofInt : Nat
  sizeofLong : Nat
  sizeofLongLong : Nat
  /-- object and function pointers -/
  sizeofPointer : Nat
  /-- alignments, bytes -/
  alignofShort : Nat
  alignofInt : Nat
  alignofLong : Nat
  alignofLongLong : Nat
  alignofPointer : Nat
  /-- J.3.6 the real floating types -/
  float : FloatTy
  double : FloatTy
  longDouble : FloatTy
  /-- the largest fundamental alignment (`max_align_t`) -/
  maxAlignment : Nat
  /-- J.3.9 the compatible type of an enumerated type -/
  enumRule : EnumRule
  /-- J.3.9 bit-fields: plain `int` signedness, allocation order, straddling -/
  bitfields : BitfieldRule
  /-- Core implementation choices, including signed narrowing and negative
  right shift. Signed arithmetic overflow is still C11 UB (std.core). -/
  implFileName : String
  implFileText : String
  /-- The image runner without additional libraries: argc = 1. The runner's
  optional -lm retry uses "--" instead; its caller must select that argv. -/
  argv0 : String
  /-- Exit encoding on the HTIF wire (not the host shell's low eight bits). -/
  exitBits : Nat
  /-- Builtin errno numbers: upstream arbitrary enumeration, or pinned newlib. -/
  newlibErrno : Bool
  deriving Repr, Inhabited

/-- Upstream Cerberus's `DefaultImpl` (ocaml_implementation.ml:151-253):
LP64, signed `char`, and the `(* TODO:hack ==> 4 *)`/`(* TODO:hack ==> 16 *)`
8-byte `float`/`long double`, all held as binary64 (an OCaml float). -/
def cerberusDefault : Params where
  name := "cerberus-default"
  charSigned := true
  sizeofShort := 2
  sizeofInt := 4
  sizeofLong := 8
  sizeofLongLong := 8
  sizeofPointer := 8
  alignofShort := 2
  alignofInt := 4
  alignofLong := 8
  alignofLongLong := 8
  alignofPointer := 8
  float := { format := .binary64, size := 8, align := 8 }
  double := { format := .binary64, size := 8, align := 8 }
  longDouble := { format := .binary64, size := 8, align := 8 }
  maxAlignment := 8
  enumRule := .gcc
  bitfields := .unsupported
  implFileName := "gcc_4.9.0_x86_64-apple-darwin10.8.0.impl"
  implFileText := include_str "../../runtime/libcore/impls/gcc_4.9.0_x86_64-apple-darwin10.8.0.impl"
  argv0 := "cmdname"
  exitBits := 8
  newlibErrno := false

/-- tcc's riscv64 lp64 ABI (`cfgTcc`, DESIGN-COMPILER.md §3.3):
riscv64-gen.c:24-31 (`PTR_SIZE 8`, `LDOUBLE_SIZE 16`, `LDOUBLE_ALIGN 16`,
`MAX_ALIGN 16`, `CHAR_IS_UNSIGNED`), the lp64 integer sizes, IEEE binary32 /
binary64 / binary128 for `float`/`double`/`long double` (the soft-float
libgcc routines tcc's patched backend calls). -/
def tccRV64 : Params where
  name := "tcc-rv64"
  charSigned := false
  sizeofShort := 2
  sizeofInt := 4
  sizeofLong := 8
  sizeofLongLong := 8
  sizeofPointer := 8
  alignofShort := 2
  alignofInt := 4
  alignofLong := 8
  alignofLongLong := 8
  alignofPointer := 8
  float := { format := .binary32, size := 4, align := 4 }
  double := { format := .binary64, size := 8, align := 8 }
  longDouble := { format := .binary128, size := 16, align := 16 }
  maxAlignment := 16
  enumRule := .tcc
  bitfields := .tcc
  implFileName := "tcc_rv64_lp64.impl"
  implFileText := include_str "../../runtime/libcore/impls/tcc_rv64_lp64.impl"
  argv0 := "/prog.c"
  exitBits := 32
  newlibErrno := true

/-- The parameters by name (the driver's `--impl NAME`). -/
def byName : String → Option Params
  | "cerberus-default" => some cerberusDefault
  | "tcc-rv64" => some tccRV64
  | _ => none

/-- THE IMPLEMENTATION, as an instance-implicit parameter (the
`CerbGlobal.Switches` shape): lem's instance reader
`declare {lean} reader val impl_params = instance `CerbImplCfg.Impl.params``
(frontend/model/lean_impl.lem) emits this projection, and every lifted
definition binds `[CerbImplCfg.Impl]`. No instance is declared in the
library: the entry point supplies one (`letI : CerbImplCfg.Impl :=
⟨cerberusDefault⟩`), a theorem binds `[CerbImplCfg.Impl]` or states a
particular implementation with `@f ⟨tccRV64⟩`. -/
class Impl where
  params : Params

end CerbImplCfg
