/-!
# TccModel: IEEE 754 binary32/64/128, definitionally

tcc's own floating-point arithmetic (constant folding, literal parsing) runs
on the image as libgcc's soft-float routines (binary32 `float`, binary64
`double`, binary128 `long double` on rv64 lp64) and newlib's conversions.
This module computes them exactly: every operation is the exact rational
result rounded to nearest-even, as libgcc's soft-fp does (no traps; NaN
results are the canonical quiet NaN, RISC-V's `_FP_NANFRAC`).

A format is given by its exponent and fraction widths; values are their
bit patterns (`Nat`).
-/

namespace Tcc.Model.SF

structure Fmt where
  ebits : Nat
  mbits : Nat   -- fraction bits (without the hidden bit)
  deriving Inhabited

def F32 : Fmt := ⟨8, 23⟩
def F64 : Fmt := ⟨11, 52⟩
def F128 : Fmt := ⟨15, 112⟩

namespace Fmt
def width (f : Fmt) : Nat := 1 + f.ebits + f.mbits
def bias (f : Fmt) : Nat := 2 ^ (f.ebits - 1) - 1
def emaxBits (f : Fmt) : Nat := 2 ^ f.ebits - 1
def signBit (f : Fmt) : Nat := 2 ^ (f.ebits + f.mbits)
def inf (f : Fmt) (neg : Bool) : Nat := (if neg then f.signBit else 0) + f.emaxBits * 2 ^ f.mbits
/-- the canonical quiet NaN (RISC-V: positive, quiet bit only) -/
def qnan (f : Fmt) : Nat := f.emaxBits * 2 ^ f.mbits + 2 ^ (f.mbits - 1)
end Fmt

/-- decoded value -/
inductive Val where
  | zero (neg : Bool)
  | inf (neg : Bool)
  | nan
  /-- (-1)^neg * m * 2^e, m > 0 -/
  | fin (neg : Bool) (m : Nat) (e : Int)
  deriving Inhabited

def decode (f : Fmt) (x : Nat) : Val :=
  let neg := (x / f.signBit) % 2 == 1
  let ex := (x / 2 ^ f.mbits) % 2 ^ f.ebits
  let fr := x % 2 ^ f.mbits
  if ex == f.emaxBits then (if fr == 0 then .inf neg else .nan)
  else if ex == 0 then
    (if fr == 0 then .zero neg else .fin neg fr (1 - (f.bias : Int) - f.mbits))
  else .fin neg (fr + 2 ^ f.mbits) ((ex : Int) - f.bias - f.mbits)

def bitlen (n : Nat) : Nat := Nat.log2 n + 1

/-- round (-1)^neg * (q + sticky·ε) * 2^e to nearest-even in `f`, where
`sticky` says whether a nonzero remainder below q's last bit was cut -/
def roundQ (f : Fmt) (neg : Bool) (q : Nat) (e : Int) (sticky : Bool) (trunc : Bool := false) : Nat :=
  let sgn := if neg then f.signBit else 0
  if q == 0 && !sticky then sgn else
  if q == 0 then sgn else
  let k := bitlen q
  let E : Int := e + k - 1              -- value in [2^E, 2^(E+1))
  let emin : Int := 1 - f.bias
  let qe : Int := if E < emin then emin - f.mbits else E - f.mbits   -- quantum exponent
  -- round q*2^e to a multiple of 2^qe
  let (m, inexactHalf) :=
    if qe ≤ e then (q * 2 ^ (e - qe).toNat, (0 : Nat))
    else
      let sh := (qe - e).toNat
      let m0 := q / 2 ^ sh
      let rem := q % 2 ^ sh
      let half := 2 ^ (sh - 1)
      let up := rem > half || (rem == half && (sticky || m0 % 2 == 1))
      let up := if rem == half && !sticky && m0 % 2 == 0 then false else up
      let up := if rem < half then false else up
      let up := if trunc then false else up
      (if up then m0 + 1 else m0, 1)
  let _ := inexactHalf
  -- m * 2^qe, m < 2^(mbits+1) (maybe = after carry)
  let (m, qe) := if m == 2 ^ (f.mbits + 1) then (m / 2, qe + 1) else (m, qe)
  if m == 0 then sgn
  else if m < 2 ^ f.mbits then
    -- subnormal (qe = emin - mbits)
    sgn + m
  else
    let bexp : Int := qe + f.mbits + f.bias
    if bexp ≥ f.emaxBits then
      (if trunc then (if neg then f.signBit else 0) + (f.emaxBits - 1) * 2 ^ f.mbits + (2 ^ f.mbits - 1) else f.inf neg)
    else sgn + bexp.toNat * 2 ^ f.mbits + (m - 2 ^ f.mbits)

/-- round the exact rational (-1)^neg * num/den * 2^e2 -/
def roundRat (f : Fmt) (neg : Bool) (num den : Nat) (e2 : Int := 0) (trunc : Bool := false) : Nat :=
  if num == 0 then (if neg then f.signBit else 0) else
  -- scale so the quotient has at least mbits+3 bits
  let need := f.mbits + 3
  let lnum := bitlen num
  let lden := bitlen den
  let sh : Int := (need : Int) + lden - lnum + 1
  let (n', d') := if sh ≥ 0 then (num * 2 ^ sh.toNat, den) else (num, den * 2 ^ (-sh).toNat)
  let q := n' / d'
  let r := n' % d'
  roundQ f neg q (e2 - sh) (r != 0) trunc

/-- encode an exact finite value -/
def ofVal (f : Fmt) : Val → Nat
  | .zero n => if n then f.signBit else 0
  | .inf n => f.inf n
  | .nan => f.qnan
  | .fin n m e => roundQ f n m e false

/-- convert between formats -/
def cvt (src dst : Fmt) (x : Nat) : Nat := ofVal dst (decode src x)

/-! ## arithmetic -/

private def toRat (m : Nat) (e : Int) : Nat × Nat × Int := (m, 1, e)

def add (f : Fmt) (a b : Nat) : Nat :=
  match decode f a, decode f b with
  | .nan, _ | _, .nan => f.qnan
  | .inf n1, .inf n2 => if n1 == n2 then f.inf n1 else f.qnan
  | .inf n, _ | _, .inf n => f.inf n
  | .zero n1, .zero n2 => if n1 && n2 then f.signBit else 0
  | .zero _, .fin .. => b
  | .fin .., .zero _ => a
  | .fin n1 m1 e1, .fin n2 m2 e2 =>
    let e := min e1 e2
    let v1 : Int := (if n1 then -1 else 1) * ((m1 * 2 ^ (e1 - e).toNat : Nat) : Int)
    let v2 : Int := (if n2 then -1 else 1) * ((m2 * 2 ^ (e2 - e).toNat : Nat) : Int)
    let s := v1 + v2
    if s == 0 then 0 else roundQ f (s < 0) s.natAbs e false

def neg (f : Fmt) (a : Nat) : Nat := (a + f.signBit) % (2 * f.signBit)

def sub (f : Fmt) (a b : Nat) : Nat :=
  match decode f b with
  | .nan => f.qnan
  | _ => add f a (neg f b)

def mul (f : Fmt) (a b : Nat) : Nat :=
  match decode f a, decode f b with
  | .nan, _ | _, .nan => f.qnan
  | .inf _, .zero _ | .zero _, .inf _ => f.qnan
  | .inf n1, .inf n2 | .inf n1, .fin n2 .. | .fin n1 .., .inf n2 => f.inf (n1 != n2)
  | .zero n1, .zero n2 | .zero n1, .fin n2 .. | .fin n1 .., .zero n2 => if n1 != n2 then f.signBit else 0
  | .fin n1 m1 e1, .fin n2 m2 e2 => roundQ f (n1 != n2) (m1 * m2) (e1 + e2) false

def div (f : Fmt) (a b : Nat) : Nat :=
  match decode f a, decode f b with
  | .nan, _ | _, .nan => f.qnan
  | .inf _, .inf _ | .zero _, .zero _ => f.qnan
  | .inf n1, .zero n2 | .inf n1, .fin n2 .. => f.inf (n1 != n2)
  | .fin n1 .., .zero n2 => f.inf (n1 != n2)
  | .zero n1, .inf n2 | .zero n1, .fin n2 .. | .fin n1 .., .inf n2 => if n1 != n2 then f.signBit else 0
  | .fin n1 m1 e1, .fin n2 m2 e2 => roundRat f (n1 != n2) m1 m2 (e1 - e2)

/-- comparison: none if unordered, else -1/0/1 -/
def cmp (f : Fmt) (a b : Nat) : Option Int :=
  let key : Val → Option (Int × Nat × Int) := fun v => match v with
    | .nan => none
    | _ => some (0, 0, 0)
  let _ := key
  match decode f a, decode f b with
  | .nan, _ | _, .nan => none
  | va, vb =>
    -- exact rational comparison via common exponent
    let toInt (v : Val) : Option (Int × Int) := match v with   -- (num, exp) or inf markers
      | .zero _ => some (0, 0)
      | .fin n m e => some ((if n then -1 else 1) * (m : Int), e)
      | _ => none
    match va, vb with
    | .inf n1, .inf n2 => some (if n1 == n2 then 0 else if n1 then -1 else 1)
    | .inf n1, _ => some (if n1 then -1 else 1)
    | _, .inf n2 => some (if n2 then 1 else -1)
    | _, _ =>
      match toInt va, toInt vb with
      | some (x, ex), some (y, ey) =>
        let e := min ex ey
        let x' := x * 2 ^ (ex - e).toNat
        let y' := y * 2 ^ (ey - e).toNat
        some (if x' < y' then -1 else if x' == y' then 0 else 1)
      | _, _ => none

/-! ## integer conversions -/

/-- int → float (exact value rounded) -/
def ofInt (f : Fmt) (v : Int) : Nat :=
  if v == 0 then 0 else roundQ f (v < 0) v.natAbs 0 false

/-- float → integer, truncating toward zero; `none` for NaN/inf -/
def toIntTrunc (f : Fmt) (a : Nat) : Option Int :=
  match decode f a with
  | .nan | .inf _ => none
  | .zero _ => some 0
  | .fin n m e =>
    let mag : Nat := if e ≥ 0 then m * 2 ^ e.toNat else m / 2 ^ (-e).toNat
    some (if n then -(mag : Int) else mag)

/-- libgcc's __fix*: truncate, saturating to [lo, hi] (soft-fp's
behaviour on overflow, NaN → the unsigned/large result per soft-fp) -/
def fixSat (f : Fmt) (a : Nat) (lo hi : Int) : Int :=
  match decode f a with
  | .nan => hi
  | .inf n => if n then lo else hi
  | _ => match toIntTrunc f a with
    | some v => if v < lo then lo else if v > hi then hi else v
    | none => 0

/-- libgcc soft-fp `__fix*` to a signed `bits`-bit integer (two's complement
result as Nat mod 2^bits): truncation; NaN/inf/overflow saturate by sign -/
def fixS (f : Fmt) (a : Nat) (bits : Nat) : Nat :=
  let lo : Int := -(2 ^ (bits - 1))
  let hi : Int := 2 ^ (bits - 1) - 1
  let sat (neg : Bool) : Int := if neg then lo else hi
  let r : Int := match decode f a with
    | .nan => sat ((a / f.signBit) % 2 == 1)
    | .inf n => sat n
    | .zero _ => 0
    | v@(.fin n _ _) => match toIntTrunc f a with
      | some x => if x < lo || x > hi then sat n else x
      | none => let _ := v; 0
  (r % (2 ^ bits : Int)).toNat

/-- libgcc soft-fp `__fixuns*` to an unsigned `bits`-bit integer -/
def fixU (f : Fmt) (a : Nat) (bits : Nat) : Nat :=
  let hi : Int := 2 ^ bits - 1
  let sat (neg : Bool) : Int := if neg then 0 else hi
  let r : Int := match decode f a with
    | .nan => sat ((a / f.signBit) % 2 == 1)
    | .inf n => sat n
    | .zero _ => 0
    | .fin n _ _ => match toIntTrunc f a with
      | some x => if x < 0 then (if x == 0 then 0 else sat n) else if x > hi then sat n else x
      | none => 0
  r.toNat

/-- int64/uint64 → format, as libgcc's __float[un]di* -/
def ofU64 (f : Fmt) (v : Nat) : Nat := ofInt f v
def ofS64 (f : Fmt) (v : Nat) : Nat := ofInt f (if v ≥ 2 ^ 63 then (v : Int) - 2 ^ 64 else v)

/-- 0 test (`x != 0` with NaN unequal) -/
def isNonzero (f : Fmt) (a : Nat) : Bool :=
  match decode f a with
  | .zero _ => false
  | _ => true

/-! ## decimal strings (correctly rounded, as newlib's strtod) -/

/-- parse the C decimal floating literal text `[digits][.digits][e[+-]digits]`
into (mantissa digits as Nat, decimal exponent) -/
def parseDec (s : List Char) : Nat × Int := Id.run do
  let mut m : Nat := 0
  let mut e : Int := 0
  let mut seenDot := false
  let mut rest := s
  let mut i := 0
  for c in s do
    if c.isDigit then
      m := m * 10 + (c.toNat - 48)
      if seenDot then e := e - 1
      i := i + 1
    else if c == '.' then seenDot := true; i := i + 1
    else break
  rest := s.drop i
  match rest with
  | c :: r =>
    if c == 'e' || c == 'E' then
      let (neg, r) := match r with
        | '-' :: r' => (true, r') | '+' :: r' => (false, r') | _ => (false, r)
      let mut x : Int := 0
      for d in r do
        if d.isDigit then
          if x < 100000000 then x := x * 10 + (d.toNat - 48)
        else break
      e := e + (if neg then -x else x)
  | [] => pure ()
  return (m, e)

/-- strtod-like: the decimal value correctly rounded into `f` -/
def ofDecimal (f : Fmt) (s : String) (trunc : Bool := false) : Nat :=
  let (m, e) := parseDec s.toList
  if m == 0 then 0
  else
    let lm := (toString m).length
    if e + lm > 6000 then f.inf false
    else if e + lm < -6000 then 0
    else if e ≥ 0 then roundQ f false (m * 10 ^ e.toNat) 0 false trunc
    else roundRat f false m (10 ^ (-e).toNat) 0 trunc

/-- newlib's `strtold` on rv64: `_strtold_r` calls gdtoa's `_strtorx`
(newlib/libc/stdlib/strtold.c, as `_LDBL_EQ_DBL` is not defined), which
writes the x87 80-bit extended format into the 16-byte binary128 `long
double`: bytes 0–7 the 64-bit significand with its explicit integer bit,
bytes 8–9 sign and biased exponent; bytes 10–15 are not written (they are
TccModel takes them as an oracle). newlib passes rounding mode 0 to
_strtorx, gdtoa's FPI_Round_zero: the value is truncated to 64 bits. -/
def ofDecimalX87 (s : String) : Nat :=
  let x : Fmt := ⟨15, 63⟩
  -- newlib passes rounding 0 (gdtoa's FPI_Round_zero): truncation
  let b := ofDecimal x s true
  let neg := (b / x.signBit) % 2 == 1
  let ex := (b / 2 ^ 63) % 2 ^ 15
  let fr := b % 2 ^ 63
  let (e16, mant) :=
    if ex == 0 then (0, fr)                                    -- zero / denormal
    else if ex == 2 ^ 15 - 1 then (0x7fff, 2 ^ 63)              -- infinity
    else (ex, fr + 2 ^ 63)
  let e16 := if neg then e16 + 0x8000 else e16
  mant + e16 * 2 ^ 64

end Tcc.Model.SF
