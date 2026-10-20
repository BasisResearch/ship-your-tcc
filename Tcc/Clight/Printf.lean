/-
  printf without a host: a pure model of C's `printf` conversion language over
  CompCert event values (C11 7.21.6.1, lp64: `int` 32-bit, `long` 64-bit).

  `format rd fmt args` is the bytes `printf(fmt, args...)` writes, or `none`
  when the call is outside the model: an incomplete or unknown conversion, an
  argument of the wrong kind or a missing one, a flag the standard leaves
  undefined for that conversion (`#` with `d`, `0` with `c`/`s`, ...), `*`
  widths, `%n`, `%p`, `%a`, `long double`. Extra arguments are ignored, as in
  C. `rd` reads the NUL-terminated string an `EVptr_global` names (for `%s`).

  Supported: literal bytes, `%%`, conversions `d i u x X o c s f F e E g G`,
  flags `- + space # 0`, decimal width and precision, length modifiers
  `hh h l ll z j t` (only `l`, which has no effect, with the floating
  conversions).

  Floating conversions are exact: the double's bits are read as `m * 2^k`
  and every digit comes from integer arithmetic on that value, rounded half
  to even (glibc's output in the default rounding mode). Infinities print
  `inf`/`INF`, NaNs `nan`/`NAN`, each with the sign bit's `-`.

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
abbrev f : UInt8 := 102
abbrev F : UInt8 := 70
abbrev e : UInt8 := 101
abbrev E : UInt8 := 69
abbrev g : UInt8 := 103
abbrev G : UInt8 := 71
end Ch

def isFloatConv (c : UInt8) : Bool :=
  c == Ch.f || c == Ch.F || c == Ch.e || c == Ch.E || c == Ch.g || c == Ch.G

def isConvByte (c : UInt8) : Bool :=
  c == Ch.d || c == Ch.i || c == Ch.u || c == Ch.x || c == Ch.X || c == Ch.o ||
  c == Ch.c || c == Ch.s || c == Ch.pct || isFloatConv c

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

/-! ## Floating conversions -/

/-- `a / b` rounded half to even (`b > 0`). -/
def divEven (a b : Nat) : Nat :=
  let q := a / b
  let r := a % b
  if 2 * r > b then q + 1 else if 2 * r < b then q else if q % 2 = 1 then q + 1 else q

/-- A positive finite double as the exact fraction `num / den`. -/
structure Ratio where
  num : Nat
  den : Nat

/-- `num / den * 10^j`, rounded half to even. -/
def Ratio.scaled (v : Ratio) (j : Int) : Nat :=
  if 0 ≤ j then divEven (v.num * 10 ^ j.toNat) v.den
  else divEven v.num (v.den * 10 ^ j.natAbs)

/-- `t ≥ 1` with `num * 10^t ≥ den`, the least one (fuel: a double is at
    least 10^-324). -/
def negExpAux (v : Ratio) : Nat → Nat → Nat
  | 0, t => t
  | fuel + 1, t => if v.num * 10 ^ t ≥ v.den then t else negExpAux v fuel (t + 1)

/-- `⌊log10 (num / den)⌋` for `num > 0`. -/
def Ratio.exp10 (v : Ratio) : Int :=
  if v.num ≥ v.den then ((digits 10 false (v.num / v.den)).length : Int) - 1
  else -((negExpAux v 400 1 : Nat) : Int)

/-- The decimal digits of `q`, left-padded with zeros to at least `n`. -/
def digitsPad (q n : Nat) : List UInt8 :=
  let ds := digits 10 false q
  List.replicate (n - ds.length) Ch.zero ++ ds

/-- `%f` body: `q` holds the value times `10^p`. -/
def fixedBody (hash : Bool) (q p : Nat) : List UInt8 :=
  let ds := digitsPad q (p + 1)
  let ip := ds.take (ds.length - p)
  let fp := ds.drop (ds.length - p)
  if p = 0 && !hash then ip else ip ++ Ch.dot :: fp

/-- The rounded significand (`p + 1` digits) and decimal exponent of
    `%.pe`; zero is `(0, 0)`. -/
def sciParts (v : Ratio) (p : Nat) : Nat × Int :=
  if v.num = 0 then (0, 0) else
  let x := v.exp10
  let q := v.scaled (p - x)
  if q ≥ 10 ^ (p + 1) then (v.scaled (p - (x + 1)), x + 1) else (q, x)

/-- `%e` body from `sciParts`. -/
def sciBody (upper hash : Bool) (q p : Nat) (x : Int) : List UInt8 :=
  let ds := digitsPad q (p + 1)
  let mant := if p = 0 && !hash then ds.take 1 else ds.take 1 ++ Ch.dot :: ds.drop 1
  let ex := digitsPad x.natAbs 2
  mant ++ (if upper then Ch.E else Ch.e) :: (if x < 0 then Ch.minus else Ch.plus) :: ex

/-- `%g` without `#`: drop trailing fraction zeros, then a bare point. -/
def stripZeros (bs : List UInt8) : List UInt8 :=
  if bs.contains Ch.dot then
    let r := (bs.reverse.dropWhile (· == Ch.zero))
    (if r.head? == some Ch.dot then r.tail else r).reverse
  else bs

/-- `%g` body: style `e` when the exponent is below -4 or at least the
    precision, else style `f`; then `stripZeros` unless `#`. -/
def generalBody (upper hash : Bool) (v : Ratio) (p0 : Nat) : List UInt8 :=
  let P := if p0 = 0 then 1 else p0
  let (q, x) := sciParts v (P - 1)
  let body :=
    if -4 ≤ x && x < P then fixedBody hash (v.scaled (P - 1 - x)) (P - 1 - x).toNat
    else sciBody upper hash q (P - 1) x
  if hash then body
  else match body.findIdx? (fun b => b == Ch.e || b == Ch.E) with
    | some i => stripZeros (body.take i) ++ body.drop i
    | none => stripZeros body

/-- A floating conversion of the double with bits `bits`. -/
def fmtFloat (sp : Spec) (conv : UInt8) (bits : Nat) : List UInt8 :=
  let neg : Bool := bits / 2 ^ 63 % 2 = 1
  let ex : Nat := bits / 2 ^ 52 % 2 ^ 11
  let frac : Nat := bits % 2 ^ 52
  let upper := conv == Ch.F || conv == Ch.E || conv == Ch.G
  let pre : List UInt8 :=
    if neg then [Ch.minus] else if sp.plus then [Ch.plus] else if sp.space then [Ch.space] else []
  if ex = 2047 then
    let w : String := if frac = 0 then (if upper then "INF" else "inf") else (if upper then "NAN" else "nan")
    pad sp false pre w.toUTF8.toList
  else
    let m := if ex = 0 then frac else frac + 2 ^ 52
    let k : Int := ((if ex = 0 then 1 else ex : Nat) : Int) - 1075
    let v : Ratio := if 0 ≤ k then ⟨m * 2 ^ k.toNat, 1⟩ else ⟨m, 2 ^ k.natAbs⟩
    let p := sp.prec.getD 6
    let body :=
      if conv == Ch.f || conv == Ch.F then fixedBody sp.hash (v.scaled p) p
      else if conv == Ch.e || conv == Ch.E then
        let (q, x) := sciParts v p
        sciBody upper sp.hash q p x
      else generalBody upper sp.hash v p
    pad sp true pre body

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
        else if isFloatConv conv then
          if sp.len == .none || sp.len == .l then
            match a with
            | .EVfloat x => some (fmtFloat sp conv (Floats.Float.toBits x).toNat, rest)
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
private def f64 (bits : Nat) : EventVal := .EVfloat ⟨BitVec.ofNat 64 bits⟩
-- 0.1, 2.5, 1e21, 5e-324, -0.0, 1/3, inf, nan (bit patterns)
example : format noStr (b "%f %.0f %.0f %.20f") [f64 0x3FB999999999999A, f64 0x4004000000000000,
    f64 0x400C000000000000, f64 0x3FB999999999999A] =
    some (b "0.100000 2 4 0.10000000000000000555") := by decide +kernel
example : format noStr (b "%e %.0E %g %g %g") [f64 0x3FB999999999999A, f64 0x444B1AE4D6E2EF50,
    f64 0x0000000000000001, f64 0x8000000000000000, f64 0x3FD5555555555555] =
    some (b "1.000000e-01 1E+21 4.94066e-324 -0 0.333333") := by decide +kernel
example : format noStr (b "[%8.3f|%-8.2e|%+g|%#g|%08.2f|%G|%f]") [f64 0x400921FB54442D18,
    f64 0x400921FB54442D18, f64 0x4059000000000000, f64 0x3FF0000000000000,
    f64 0xC00921FB54442D18, f64 0x7FF0000000000000, f64 0xFFF8000000000000] =
    some (b "[   3.142|3.14e+00|+100|1.00000|-0003.14|INF|-nan]") := by decide +kernel
example : format noStr (b "%g %g %g %.3g") [f64 0x412E848000000000, f64 0x4123880000000000,
    f64 0x3F1A36E2EB1C432D, f64 0x40FE240C9FBE76C9] =
    some (b "1e+06 640000 0.0001 1.23e+05") := by decide +kernel
example : format noStr (b "%Lf") [f64 0] = none := by decide +kernel
example : format noStr (b "%") [] = none := by decide +kernel
end

end Tcc.Clight
