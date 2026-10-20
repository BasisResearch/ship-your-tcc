/-
  printf without a host: a pure model of C's `printf` conversion language over
  CompCert event values (C11 7.21.6.1, lp64: `int` 32-bit, `long` 64-bit).

  `format rd fmt args` is the bytes `printf(fmt, args...)` writes, or `none`
  when the call is outside the model: an incomplete or unknown conversion, an
  argument of the wrong kind or a missing one, a flag the standard leaves
  undefined for that conversion (`#` with `d`, `0` with `c`/`s`, ...), `*`
  widths, `%n`, `%p`, floating conversions. Extra arguments are ignored, as in
  C. `rd` reads the NUL-terminated string an `EVptr_global` names (for `%s`).

  Supported: literal bytes, `%%`, conversions `d i u x X o c s`, flags
  `- + space # 0`, decimal width and precision, length modifiers
  `hh h l ll z j t`.

  The loop is structural on the format string (one byte per call), so the
  kernel evaluates it (`decide +kernel` in `Tcc.Prog.*.ClightFacts`).
-/
import CCLib

namespace Tcc.Clight
open CC

/-- Reads the bytes (before the NUL) of the string an event pointer names. -/
abbrev StrReader := Ident → Integers.Ptrofs → Option (List UInt8)

/-- A C length modifier. -/
inductive LenMod where
  | none | hh | h | l | ll | z | j | t
  deriving DecidableEq, Repr

/-- lp64: `l ll z j t` select a 64-bit argument. -/
def LenMod.is64 : LenMod → Bool
  | .l | .ll | .z | .j | .t => true
  | _ => false

/-- A conversion specification being read. `phase`: 0 flags, 1 width,
    2 precision, 3 length modifier. -/
structure Spec where
  minus : Bool := false
  plus : Bool := false
  space : Bool := false
  hash : Bool := false
  zero : Bool := false
  width : Option Nat := none
  prec : Option Nat := none
  len : LenMod := .none
  phase : Nat := 0
  deriving DecidableEq, Repr

/-- Result of feeding one byte to a specification. -/
inductive SpecStep where
  | more (sp : Spec)
  | conv (c : UInt8)
  | bad

-- ASCII bytes the format language uses.
namespace Ch
abbrev pct : UInt8 := 37    -- '%'
abbrev minus : UInt8 := 45  -- '-'
abbrev plus : UInt8 := 43   -- '+'
abbrev space : UInt8 := 32  -- ' '
abbrev hash : UInt8 := 35   -- '#'
abbrev zero : UInt8 := 48   -- '0'
abbrev dot : UInt8 := 46    -- '.'
abbrev h : UInt8 := 104
abbrev l : UInt8 := 108
abbrev z : UInt8 := 122
abbrev j : UInt8 := 106
abbrev t : UInt8 := 116
abbrev d : UInt8 := 100
abbrev i : UInt8 := 105
abbrev u : UInt8 := 117
abbrev x : UInt8 := 120
abbrev X : UInt8 := 88
abbrev o : UInt8 := 111
abbrev c : UInt8 := 99
abbrev s : UInt8 := 115
end Ch

def isConvByte (c : UInt8) : Bool :=
  c == Ch.d || c == Ch.i || c == Ch.u || c == Ch.x || c == Ch.X || c == Ch.o ||
  c == Ch.c || c == Ch.s || c == Ch.pct

def isDigitByte (c : UInt8) : Bool := 48 ≤ c && c ≤ 57

def addDigit (n : Option Nat) (c : UInt8) : Nat := n.getD 0 * 10 + (c.toNat - 48)

/-- Feed one byte of a conversion specification. -/
def specStep (sp : Spec) (c : UInt8) : SpecStep :=
  if isConvByte c then .conv c
  else if sp.phase == 0 && c == Ch.minus then .more { sp with minus := true }
  else if sp.phase == 0 && c == Ch.plus then .more { sp with plus := true }
  else if sp.phase == 0 && c == Ch.space then .more { sp with space := true }
  else if sp.phase == 0 && c == Ch.hash then .more { sp with hash := true }
  else if sp.phase == 0 && c == Ch.zero then .more { sp with zero := true }
  else if sp.phase ≤ 1 && isDigitByte c then
    .more { sp with width := some (addDigit sp.width c), phase := 1 }
  else if sp.phase ≤ 1 && c == Ch.dot then .more { sp with prec := some 0, phase := 2 }
  else if sp.phase == 2 && isDigitByte c then
    .more { sp with prec := some (addDigit sp.prec c) }
  else if c == Ch.h then
    match sp.len with
    | .none => if sp.phase ≤ 2 then .more { sp with len := .h, phase := 3 } else .bad
    | .h => .more { sp with len := .hh }
    | _ => .bad
  else if c == Ch.l then
    match sp.len with
    | .none => if sp.phase ≤ 2 then .more { sp with len := .l, phase := 3 } else .bad
    | .l => .more { sp with len := .ll }
    | _ => .bad
  else if (c == Ch.z || c == Ch.j || c == Ch.t) && sp.len == .none && sp.phase ≤ 2 then
    .more { sp with len := if c == Ch.z then .z else if c == Ch.j then .j else .t, phase := 3 }
  else .bad

/-! ## Arguments -/

/-- The value a signed conversion prints (C's default promotions undone by
    `h`/`hh`). -/
def signedArg (len : LenMod) : EventVal → Option Int
  | .EVint n =>
      match len with
      | .none => some n.toInt
      | .h => some (n.setWidth 16).toInt
      | .hh => some (n.setWidth 8).toInt
      | _ => none
  | .EVlong n => if len.is64 then some n.toInt else none
  | _ => none

/-- The value an unsigned conversion prints. -/
def unsignedArg (len : LenMod) : EventVal → Option Nat
  | .EVint n =>
      match len with
      | .none => some n.toNat
      | .h => some (n.setWidth 16).toNat
      | .hh => some (n.setWidth 8).toNat
      | _ => none
  | .EVlong n => if len.is64 then some n.toNat else none
  | _ => none

/-! ## Digits and padding -/

def digitByte (upper : Bool) (d : Nat) : UInt8 :=
  if d < 10 then (48 + d).toUInt8 else ((if upper then 55 else 87) + d).toUInt8

/-- Digits of `n` in `base`, most significant first; fuel `n + 1` suffices. -/
def digitsAux (base : Nat) (upper : Bool) : Nat → Nat → List UInt8 → List UInt8
  | 0, _, acc => acc
  | fuel + 1, n, acc =>
      let acc' := digitByte upper (n % base) :: acc
      if n / base = 0 then acc' else digitsAux base upper fuel (n / base) acc'

def digits (base : Nat) (upper : Bool) (n : Nat) : List UInt8 :=
  digitsAux base upper (n + 1) n []

/-- Pad `body` (after a prefix `pre` that zero padding goes behind) to the
    field width. -/
def pad (sp : Spec) (zeroOk : Bool) (pre body : List UInt8) : List UInt8 :=
  let n := pre.length + body.length
  let w := sp.width.getD 0
  if w ≤ n then pre ++ body
  else if sp.minus then pre ++ body ++ List.replicate (w - n) Ch.space
  else if zeroOk && sp.zero then pre ++ List.replicate (w - n) Ch.zero ++ body
  else List.replicate (w - n) Ch.space ++ pre ++ body

/-- An integer conversion: sign or `0x` prefix, precision, `#`, width. -/
def fmtInt (sp : Spec) (conv : UInt8) (neg : Bool) (mag : Nat) : List UInt8 :=
  let base := if conv == Ch.o then 8 else if conv == Ch.x || conv == Ch.X then 16 else 10
  let ds0 := if sp.prec == some 0 && mag == 0 then [] else digits base (conv == Ch.X) mag
  let ds1 := match sp.prec with
    | some p => List.replicate (p - ds0.length) Ch.zero ++ ds0
    | none => ds0
  let ds := if conv == Ch.o && sp.hash && ds1.head? != some Ch.zero then Ch.zero :: ds1 else ds1
  let pre : List UInt8 :=
    if conv == Ch.d || conv == Ch.i then
      if neg then [Ch.minus] else if sp.plus then [Ch.plus] else if sp.space then [Ch.space] else []
    else if sp.hash && mag != 0 && conv == Ch.x then [Ch.zero, Ch.x]
    else if sp.hash && mag != 0 && conv == Ch.X then [Ch.zero, Ch.X]
    else []
  pad sp sp.prec.isNone pre ds

/-- `%c`/`%s` admit only `-`, a width and (for `s`) a precision. -/
def plainFlags (sp : Spec) : Bool :=
  !sp.plus && !sp.space && !sp.hash && !sp.zero && sp.len == .none

/-- One conversion: its output and the remaining arguments. -/
def convert (rd : StrReader) (sp : Spec) (conv : UInt8) :
    List EventVal → Option (List UInt8 × List EventVal)
  | args =>
    if conv == Ch.pct then
      if sp == {} then some ([Ch.pct], args) else none
    else
      match args with
      | [] => none
      | a :: rest =>
        if conv == Ch.d || conv == Ch.i then
          if sp.hash then none else
          (signedArg sp.len a).map fun v => (fmtInt sp conv (decide (v < 0)) v.natAbs, rest)
        else if conv == Ch.u || conv == Ch.x || conv == Ch.X || conv == Ch.o then
          if conv == Ch.u && sp.hash then none else
          (unsignedArg sp.len a).map fun v => (fmtInt sp conv false v, rest)
        else if conv == Ch.c then
          if plainFlags sp && sp.prec.isNone then
            match a with
            | .EVint n => some (pad sp false [] [(n.toNat % 256).toUInt8], rest)
            | _ => none
          else none
        else if conv == Ch.s then
          if plainFlags sp then
            match a with
            | .EVptr_global id ofs =>
                (rd id ofs).map fun bs =>
                  (pad sp false [] (match sp.prec with | some p => bs.take p | none => bs), rest)
            | _ => none
          else none
        else none

/-- The format loop: `none` = between conversions, `some sp` = inside one. -/
def formatGo (rd : StrReader) : Option Spec → List UInt8 → List EventVal → Option (List UInt8)
  | none, [], _ => some []
  | some _, [], _ => none
  | none, c :: cs, args =>
      if c == Ch.pct then formatGo rd (some {}) cs args
      else (c :: ·) <$> formatGo rd none cs args
  | some sp, c :: cs, args =>
      match specStep sp c with
      | .more sp' => formatGo rd (some sp') cs args
      | .conv k =>
          match convert rd sp k args with
          | some (o, rest) => (o ++ ·) <$> formatGo rd none cs rest
          | none => none
      | .bad => none

/-- `printf(fmt, args...)`'s output, or `none` outside the model. -/
def format (rd : StrReader) (fmt : List UInt8) (args : List EventVal) : Option (List UInt8) :=
  formatGo rd none fmt args

/-! ## Regression cases (each checked against glibc's printf) -/

section
private def noStr : StrReader := fun _ _ => none
private def b (s : String) : List UInt8 := s.toUTF8.toList
private def i32 (z : Int) : EventVal := .EVint (Integers.Int.repr z)

example : format noStr (b "%d\n") [i32 55] = some (b "55\n") := by decide +kernel
example : format noStr (b "%d") [i32 (-2147483648)] = some (b "-2147483648") := by decide +kernel
example : format noStr (b "[%5d|%-5d|%05d]") [i32 (-42), i32 7, i32 (-42)] =
    some (b "[  -42|7    |-0042]") := by decide +kernel
example : format noStr (b "%u %x %X %o %#x %#o") [i32 (-1), i32 255, i32 255, i32 8, i32 255, i32 8] =
    some (b "4294967295 ff FF 10 0xff 010") := by decide +kernel
example : format noStr (b "%+d % d %.3d %.0d|%8.3d") [i32 5, i32 5, i32 7, i32 0, i32 (-7)] =
    some (b "+5  5 007 |    -007") := by decide +kernel
example : format noStr (b "%c%%%3c") [i32 65, i32 66] = some (b "A%  B") := by decide +kernel
example : format noStr (b "%ld %lu") [.EVlong (Integers.Int64.repr (-9223372036854775808)),
    .EVlong (Integers.Int64.repr (-1))] =
    some (b "-9223372036854775808 18446744073709551615") := by decide +kernel
example : format noStr (b "%hd %hhu") [i32 65537, i32 257] = some (b "1 1") := by decide +kernel
example : format noStr (b "%d") [] = none := by decide +kernel
example : format noStr (b "%#d") [i32 1] = none := by decide +kernel
example : format noStr (b "%f") [i32 1] = none := by decide +kernel
example : format noStr (b "%") [] = none := by decide +kernel
end

end Tcc.Clight
