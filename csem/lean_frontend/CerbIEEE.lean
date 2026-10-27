/-!
# `CerbIEEE`: a pure, definitional IEEE 754 library for C's three floating types

DESIGN-COMPILER.md §3.4 "Native float seams": CSem's floats move off Lean's
native `Float` (opaque to the kernel's arithmetic, so no proof can relate it to
machine code) onto this module. It is a leaf (imports Lean core only).

## What is modelled

* **Formats.** `FFormat.binary32` (`float`), `binary64` (`double`) and
  `binary128` (`long double` on rv64 lp64, the soft-float quad of libgcc's
  `__addtf3` family). `FFormat.spec` is Lean core's `Float.Model.Format`
  (`binary128 = ⟨112, 15⟩`).
* **Values.** A datum of format `f` is its bit pattern, `Bits f :=
  BitVec f.spec.numBits` (32/64/128 bits, the IEEE interchange encoding, the
  bytes C stores). Every operation is `pack ∘ op ∘ unpack` over core's
  `Float.Model.UnpackedFloat`; `FVal` packs a format with its bits for callers
  that carry untyped values.
* **Specification = Lean core's `Float.Model`.** The arithmetic below is core's
  `UnpackedFloat` arithmetic with ONE change: the model's bit-at-a-time
  `ExtendedMantissa >>> s` (`Nat.repeat`) is replaced by its closed form
  (`Fast.shiftRight`, proved equal: `Fast.shiftRight_eq`). Hence for EVERY
  format each operation is the model's (`add_spec`, `mul_spec`, …), and for
  `binary64`/`binary32` it is `Float.Model`/`Float32.Model`'s — and so, in the
  kernel's logic, native `Float`/`Float32`'s (`add_model`, `add_float`,
  `add_model32`, `add_float32`, …). ship-your-lua's libgcc proofs target
  `Float.Model` through `Lua.Num.F64.*_eq_model`, which compose with these.
  No `implemented_by`/`extern`: the definition the kernel sees is the one that
  runs.
* **Rounding.** The selected configuration uses round-to-nearest, ties-to-even,
  gradual underflow and overflow to ±∞. Dynamic fenv/library coverage remains
  an F7 obligation; this module does not establish it or restrict Supported.
* **NaN policy.** Every NaN arithmetic produces is the canonical quiet NaN:
  positive, quiet bit only (`0x7fc00000`, `0x7ff8000000000000`,
  `0x7fff8000…0`). That is core's `UnpackedFloat.packedNaN` (`nan_bits32/64/128`
  check it by kernel reduction). Fresh volatile-operand tcc/xgcc Sail calls
  in `test/ieee/nan_policy.c` observe these arithmetic bits; widening has the
  separate payload-preserving policy below. The two
  sign-bit operations `neg` and `abs` act on the bits (payload kept): tcc's
  riscv64 `gen_negf` (tccgen.c) negates by an `xor` of the sign bit.
* **Comparisons** (C semantics): `eq lt le gt ge` are false and `ne` is true
  when an operand is a NaN; `-0 == +0`.
* **Integer conversions.** `ofInt f n` rounds a mathematical integer (RNE);
  `toInt? f x` truncates toward zero, `none` for NaN/±∞ (the caller checks the
  C target range: out of range is UB, C11 6.3.1.4).
* **Format conversions.** `convert src dst`: exact widening, RNE narrowing.
  Widening quiets NaNs while preserving sign and shifted payload; narrowing
  produces the canonical positive NaN. Equal-format C copies preserve all bits
  (`FVal.to`). The raw Sail observations correct the historical draft policy.
* **Literals.** `ofLiteral f s`: the correctly rounded value of a decimal or
  hexadecimal C floating literal (optional sign and `f F l L` suffix), `none`
  on malformed text.
* **printf.** `format spec f x`: `%f %F %e %E %g %G %a %A` with flags
  `- + space # 0`, width and precision, from the exact binary value
  (round-half-even on the exact value, as glibc and newlib's `_dtoa_r`).
* **Bytes.** `toBytesLE`/`ofBytesLE?`: little-endian, 4/8/16 bytes.

## C operation → function

| C | function |
|---|---|
| `x + y`, `x - y`, `x * y`, `x / y` | `add`, `sub`, `mul`, `div` |
| `-x` | `neg` (sign flip) |
| `x == y`, `!=`, `<`, `<=`, `>`, `>=` | `eq`, `ne`, `lt`, `le`, `gt`, `ge` |
| `(T)i` for integer `i`, floating `T` | `ofInt` |
| `(I)x` for integer type `I` | `toInt?` + range check |
| `(T)x` between floating types | `convert` |
| `x` as a condition (`x != 0`) | `isZero` |
| literal `1.5e3f` | `ofLiteral` |
| `sqrt(x)` (libm, correctly rounded) | `sqrt` |
| `fpclassify`, `isnan`, `isinf`, `signbit`, `fabs` | `classify`, `isNaN`, `isInf`, `signBit`, `abs` |
| `printf("%…f/e/g/a")` | `format` |
| object representation | `toBytesLE`, `ofBytesLE?` |
-/

namespace CerbIEEE

open Float.Model
open Float.Model.UnpackedFloat

/-! ## Formats and values -/

/-- C's three floating types on rv64 lp64. -/
inductive FFormat where
  /-- `float`: IEEE binary32. -/
  | binary32
  /-- `double`: IEEE binary64. -/
  | binary64
  /-- `long double`: IEEE binary128 (rv64 lp64). -/
  | binary128
deriving DecidableEq, Repr, Inhabited

namespace FFormat

/-- The `Float.Model.Format` of each C floating type. -/
@[reducible] def spec : FFormat → Format
  | binary32 => Format.binary32
  | binary64 => Format.binary64
  | binary128 => { mantissaBitsWithoutImplicit := 112, exponentBits := 15 }

/-- Width of the interchange encoding in bits. -/
def width : FFormat → Nat
  | binary32 => 32
  | binary64 => 64
  | binary128 => 128

theorem numBits_eq (f : FFormat) : f.spec.numBits = f.width := by cases f <;> rfl

/-- Size of the object representation in bytes (`sizeof`). -/
def bytes (f : FFormat) : Nat := f.width / 8

/-- The usual-arithmetic-conversions rank (C11 6.3.1.8). -/
def rank : FFormat → Nat
  | binary32 => 0
  | binary64 => 1
  | binary128 => 2

/-- The common real type of two floating types (C11 6.3.1.8). -/
def common (f g : FFormat) : FFormat := if f.rank ≤ g.rank then g else f

end FFormat

/-- A datum of format `f`: its IEEE interchange bit pattern. -/
abbrev Bits (f : FFormat) := BitVec f.spec.numBits

/-! ## The closed-form rounding kernel

Core's `UnpackedFloat` rounds by shifting an `ExtendedMantissa` right one bit
at a time (`Nat.repeat shiftRightOne`): exponent gaps of thousands of bits
(binary128 spans 2^15 binades) make that quadratic. `Fast.shiftRight` is the
same shift in closed form (one shift and one comparison), proved equal; the
`Fast` operations are core's definitions with that one substitution, each
proved equal to core's. -/

namespace Fast

/-- `em >>> s` in closed form: mantissa `⌊m / 2^s⌋`, round bit = bit `s-1` of
`m`, sticky bit = (bits below `s-1` nonzero) ∨ old round ∨ old sticky. -/
def shiftRight (em : ExtendedMantissa) : Nat → ExtendedMantissa
  | 0 => em
  | s + 1 =>
    let hi := em.mantissa >>> s
    { mantissa := hi >>> 1
      roundBit := hi % 2 != 0
      stickyBit := (hi <<< s != em.mantissa) || em.roundBit || em.stickyBit }

-- The model's shift in the closed form of ship-your-lua's `F64.shiftRight_succ`
-- (Lua/Num/F64.lean; ATTRIBUTION.md).
theorem model_shiftRight_succ (em : ExtendedMantissa) (s : Nat) :
    em >>> (s + 1) = ⟨em.mantissa / 2 ^ (s + 1), decide (em.mantissa / 2 ^ s % 2 = 1),
      decide (em.mantissa % 2 ^ s ≠ 0) || em.roundBit || em.stickyBit⟩ := by
  induction s with
  | zero =>
    show ExtendedMantissa.shiftRightOne em = _
    simp only [ExtendedMantissa.shiftRightOne, Nat.pow_zero, Nat.div_one, Nat.mod_one,
      ne_eq, not_true_eq_false, decide_false, Bool.false_or]
    congr 1
    cases h : em.mantissa % 2 == 1 <;> simp_all <;> omega
  | succ s ih =>
    show ExtendedMantissa.shiftRightOne (em >>> (s + 1)) = _
    rw [ih]
    simp only [ExtendedMantissa.shiftRightOne, Nat.div_div_eq_div_mul, ← Nat.pow_succ]
    have hm := @Nat.mod_pow_succ em.mantissa 2 s
    have hp : 0 < 2 ^ s := Nat.two_pow_pos s
    have h2 : em.mantissa / 2 ^ s % 2 < 2 := Nat.mod_lt _ (by decide)
    congr 1
    · cases h : em.mantissa / 2 ^ (s + 1) % 2 == 1 <;> simp_all <;> omega
    · cases em.roundBit <;> cases em.stickyBit <;>
        by_cases h1 : em.mantissa / 2 ^ s % 2 = 1 <;> by_cases h3 : em.mantissa % 2 ^ s = 0 <;>
        simp_all <;> omega

theorem shl_shr_ne (m s : Nat) : ((m >>> s) <<< s != m) = decide (m % 2 ^ s ≠ 0) := by
  rw [Nat.shiftLeft_eq, Nat.shiftRight_eq_div_pow]
  have h := Nat.mod_add_div m (2 ^ s)
  have hp : 0 < 2 ^ s := Nat.two_pow_pos s
  by_cases h0 : m % 2 ^ s = 0
  · have : m / 2 ^ s * 2 ^ s = m := by rw [Nat.mul_comm]; omega
    simp [h0, this]
  · have : m / 2 ^ s * 2 ^ s ≠ m := by rw [Nat.mul_comm]; omega
    simp [h0, this]

/-- **The kernel's one substitution is sound**: the closed-form shift is the model's. -/
theorem shiftRight_eq (em : ExtendedMantissa) (s : Nat) : shiftRight em s = em >>> s := by
  cases s with
  | zero => rfl
  | succ s =>
    rw [model_shiftRight_succ]
    simp only [shiftRight, shl_shr_ne]
    rw [Nat.shiftRight_eq_div_pow, Nat.shiftRight_eq_div_pow, Nat.div_div_eq_div_mul,
      ← Nat.pow_succ]
    congr 1
    cases h : em.mantissa / 2 ^ s % 2 == 1 <;> simp_all <;> omega

/-- `UnpackedFloat.shiftToExponent` with the closed-form shift. -/
def shiftToExponent (mantissa : Nat) (exponent : Int)
    (accuracy : Accuracy) (targetExponent : Int) : ExtendedMantissa × Int :=
  let shiftAmount := (targetExponent - exponent).toNat
  (shiftRight (ExtendedMantissa.ofMantissaAndAccuracy mantissa accuracy) shiftAmount,
    exponent + shiftAmount)

theorem shiftToExponent_eq (m : Nat) (e : Int) (a : Accuracy) (t : Int) :
    shiftToExponent m e a t = UnpackedFloat.shiftToExponent m e a t := by
  simp only [shiftToExponent, UnpackedFloat.shiftToExponent, shiftRight_eq]

/-- `UnpackedFloat.shiftToTargetExponent` with the closed-form shift. -/
def shiftToTargetExponent (spec : Format) (mantissa : Nat) (exponent : Int)
    (accuracy : Accuracy) : ExtendedMantissa × Int :=
  shiftToExponent mantissa exponent accuracy
    (spec.targetExponent (totalExponent mantissa exponent))

theorem shiftToTargetExponent_eq (spec : Format) (m : Nat) (e : Int) (a : Accuracy) :
    shiftToTargetExponent spec m e a = UnpackedFloat.shiftToTargetExponent spec m e a := by
  simp only [shiftToTargetExponent, UnpackedFloat.shiftToTargetExponent, shiftToExponent_eq]

/-- `UnpackedFloat.roundWithAccuracy` with the closed-form shift. -/
def roundWithAccuracy (spec : Format) (sign : Sign) (mantissa : Nat) (exponent : Int)
    (accuracy : Accuracy) : UnpackedFloat :=
  let (em₁, e₁) := shiftToTargetExponent spec mantissa exponent accuracy
  let roundedEm₁ := em₁.roundedMantissa
  let (finalExtendedMantissa, finalExponent) := shiftToTargetExponent spec roundedEm₁ e₁ .exact
  let finalMantissa := finalExtendedMantissa.mantissa
  if h : finalMantissa = 0 then
    .zero sign
  else
    .finite sign finalMantissa finalExponent (Nat.pos_of_ne_zero h)

theorem roundWithAccuracy_eq (spec : Format) (s : Sign) (m : Nat) (e : Int) (a : Accuracy) :
    roundWithAccuracy spec s m e a = UnpackedFloat.roundWithAccuracy spec s m e a := by
  simp only [roundWithAccuracy, UnpackedFloat.roundWithAccuracy, shiftToTargetExponent_eq]

/-- `UnpackedFloat.round` with the closed-form shift. -/
def round (spec : Format) (sign : Sign) (mantissa : Nat) (exponent : Int) : UnpackedFloat :=
  let targetExponent := spec.targetExponent (totalExponent mantissa exponent)
  let (mantissa, exponent) := decreaseExponent mantissa exponent targetExponent
  roundWithAccuracy spec sign mantissa exponent .exact

theorem round_eq (spec : Format) (s : Sign) (m : Nat) (e : Int) :
    round spec s m e = UnpackedFloat.round spec s m e := by
  simp only [round, UnpackedFloat.round, roundWithAccuracy_eq]

/-- `UnpackedFloat.normalize` with the closed-form shift. -/
def normalize (spec : Format) (mantissa : Int) (exponent : Int) (zeroSign : Sign) : UnpackedFloat :=
  match compare mantissa 0 with
  | .lt => round spec .negative (-mantissa).toNat exponent
  | .eq => .zero zeroSign
  | .gt => round spec .positive mantissa.toNat exponent

theorem normalize_eq (spec : Format) (m e : Int) (z : Sign) :
    normalize spec m e z = UnpackedFloat.normalize spec m e z := by
  unfold normalize UnpackedFloat.normalize
  cases compare m 0 <;> simp only [round_eq]

/-- `UnpackedFloat.add` with the closed-form shift. -/
def add (spec : Format) : UnpackedFloat → UnpackedFloat → UnpackedFloat
  | .notANumber, _ => .notANumber
  | _, .notANumber => .notANumber
  | .infinity sign₁, .infinity sign₂ =>
    if sign₁ == sign₂ then .infinity sign₁ else .notANumber
  | .infinity s, _ => .infinity s
  | _, .infinity s => .infinity s
  | .zero sign₁, .zero sign₂ =>
    if sign₁ == sign₂ then .zero sign₁ else .zero .positive
  | .zero _, y => y
  | x, .zero _ => x
  | .finite s₁ m₁ e₁ _, .finite s₂ m₂ e₂ _ =>
    let smallerExponent := min e₁ e₂
    let (m₁, _) := decreaseExponent m₁ e₁ smallerExponent
    let (m₂, _) := decreaseExponent m₂ e₂ smallerExponent
    let mantissa := s₁.apply m₁ + s₂.apply m₂
    normalize spec mantissa smallerExponent .positive

theorem add_eq (spec : Format) (x y : UnpackedFloat) :
    add spec x y = UnpackedFloat.add spec x y := by
  cases x <;> cases y <;> simp only [add, UnpackedFloat.add, normalize_eq]

/-- `UnpackedFloat.sub` with the closed-form shift. -/
def sub (spec : Format) : UnpackedFloat → UnpackedFloat → UnpackedFloat
  | .notANumber, _ => .notANumber
  | _, .notANumber => .notANumber
  | .infinity sign₁, .infinity sign₂ =>
    if sign₁ == -sign₂ then .infinity sign₁ else .notANumber
  | .infinity s, _ => .infinity s
  | _, .infinity s => .infinity (-s)
  | .zero sign₁, .zero sign₂ =>
    if sign₁ == -sign₂ then .zero sign₁ else .zero .positive
  | .zero _, .finite sign mantissa exponent hm => .finite (-sign) mantissa exponent hm
  | x, .zero _ => x
  | .finite s₁ m₁ e₁ _, .finite s₂ m₂ e₂ _ =>
    let smallerExponent := min e₁ e₂
    let (m₁, _) := decreaseExponent m₁ e₁ smallerExponent
    let (m₂, _) := decreaseExponent m₂ e₂ smallerExponent
    let mantissa := s₁.apply m₁ - s₂.apply m₂
    normalize spec mantissa smallerExponent .positive

theorem sub_eq (spec : Format) (x y : UnpackedFloat) :
    sub spec x y = UnpackedFloat.sub spec x y := by
  cases x <;> cases y <;> simp only [sub, UnpackedFloat.sub, normalize_eq]

/-- `UnpackedFloat.mul` with the closed-form shift. -/
def mul (spec : Format) : UnpackedFloat → UnpackedFloat → UnpackedFloat
  | .notANumber, _ => .notANumber
  | _, .notANumber => .notANumber
  | .infinity sign₁, .infinity sign₂ => .infinity (sign₁ * sign₂)
  | .infinity sign₁, .finite sign₂ .. => .infinity (sign₁ * sign₂)
  | .finite sign₁ .., .infinity sign₂ => .infinity (sign₁ * sign₂)
  | .infinity _, .zero _ => .notANumber
  | .zero _, .infinity _ => .notANumber
  | .finite sign₁ .., .zero sign₂ => .zero (sign₁ * sign₂)
  | .zero sign₁, .finite sign₂ .. => .zero (sign₁ * sign₂)
  | .zero sign₁, .zero sign₂ => .zero (sign₁ * sign₂)
  | .finite s₁ m₁ e₁ _, .finite s₂ m₂ e₂ _ =>
    roundWithAccuracy spec (s₁ * s₂) (m₁ * m₂) (e₁ + e₂) .exact

theorem mul_eq (spec : Format) (x y : UnpackedFloat) :
    mul spec x y = UnpackedFloat.mul spec x y := by
  cases x <;> cases y <;> simp only [mul, UnpackedFloat.mul, roundWithAccuracy_eq]

/-- `UnpackedFloat.div` with the closed-form shift. -/
def div (spec : Format) : UnpackedFloat → UnpackedFloat → UnpackedFloat
  | .notANumber, _ => .notANumber
  | _, .notANumber => .notANumber
  | .infinity _, .infinity _ => .notANumber
  | .infinity sign₁, .finite sign₂ .. => .infinity (sign₁ / sign₂)
  | .finite sign₁ .., .infinity sign₂ => .zero (sign₁ / sign₂)
  | .infinity sign₁, .zero sign₂ => .infinity (sign₁ / sign₂)
  | .zero sign₁, .infinity sign₂ => .zero (sign₁ / sign₂)
  | .finite sign₁ .., .zero sign₂ => .infinity (sign₁ / sign₂)
  | .zero sign₁, .finite sign₂ .. => .zero (sign₁ / sign₂)
  | .zero _, .zero _ => .notANumber
  | .finite s₁ m₁ e₁ _, .finite s₂ m₂ e₂ _ =>
    let (m, e, acc) := divCore spec m₁ e₁ m₂ e₂
    roundWithAccuracy spec (s₁ / s₂) m e acc

theorem div_eq (spec : Format) (x y : UnpackedFloat) :
    div spec x y = UnpackedFloat.div spec x y := by
  cases x <;> cases y <;> simp only [div, UnpackedFloat.div, roundWithAccuracy_eq]

/-- `UnpackedFloat.sqrt` with the closed-form shift. -/
def sqrt (spec : Format) : UnpackedFloat → UnpackedFloat
  | .notANumber => .notANumber
  | .infinity .positive => .infinity .positive
  | .infinity .negative => .notANumber
  | .finite .negative .. => .notANumber
  | .zero sign => .zero sign
  | .finite .positive m e _ =>
    let (m, e, acc) := sqrtCore spec m e
    roundWithAccuracy spec .positive m e acc

theorem sqrt_eq (spec : Format) (x : UnpackedFloat) :
    sqrt spec x = UnpackedFloat.sqrt spec x := by
  rcases x with s | _ | s | ⟨s, m, e, h⟩ <;> (try cases s) <;>
    simp only [sqrt, UnpackedFloat.sqrt, roundWithAccuracy_eq]

/-- `UnpackedFloat.ofInt` with the closed-form shift. -/
def ofInt (spec : Format) (n : Int) : UnpackedFloat :=
  normalize spec n 0 .positive

theorem ofInt_eq (spec : Format) (n : Int) : ofInt spec n = UnpackedFloat.ofInt spec n := by
  simp only [ofInt, UnpackedFloat.ofInt, normalize_eq]

/-- `UnpackedFloat.ofScientific` (`m · 10^e`, correctly rounded) with the
closed-form shift. -/
def ofScientific (spec : Format) (m : Nat) (e : Int) : UnpackedFloat :=
  if hm : m = 0 then
    .zero .positive
  else if e > 2 ^ spec.exponentBits then
    .infinity .positive
  else if e < -((2 ^ spec.exponentBits : Int) + m.log2) then
    .zero .positive
  else if 0 ≤ e then
    mul spec (.finite .positive (m <<< spec.mantissaBits) (-spec.mantissaBits)
        (by simpa [Nat.pos_iff_ne_zero, Nat.shiftLeft_eq] using
          Nat.mul_pos (Nat.pos_of_ne_zero hm) (Nat.two_pow_pos _)))
      (.finite .positive (10 ^ e.toNat) 0 (Nat.pow_pos (by decide)))
  else
    div spec (.finite .positive m 0 (Nat.pos_iff_ne_zero.2 hm))
      (.finite .positive (10 ^ (-e).toNat) 0 (Nat.pow_pos (by decide)))

theorem ofScientific_eq (spec : Format) (m : Nat) (e : Int) :
    ofScientific spec m e = UnpackedFloat.ofScientific spec m e := by
  simp only [ofScientific, UnpackedFloat.ofScientific, mul_eq, div_eq]

/-- `UnpackedFloat.roundToInt` (truncation toward zero) with the closed-form shift. -/
def roundToInt (sign : Sign) (mantissa : Nat) (exponent : Int) : Int :=
  let (mantissa, exponent) := decreaseExponent mantissa exponent 0
  let (em₁, _) := shiftToExponent mantissa exponent .exact 0
  sign.apply em₁.mantissa

theorem roundToInt_eq (s : Sign) (m : Nat) (e : Int) :
    roundToInt s m e = UnpackedFloat.roundToInt s m e := by
  simp only [roundToInt, UnpackedFloat.roundToInt, shiftToExponent_eq]

/-- Rounding of an unpacked value (exact in its own format) into `spec`:
`round` on a finite value, the specials unchanged. -/
def roundInto (spec : Format) : UnpackedFloat → UnpackedFloat
  | .finite s m e _ => round spec s m e
  | x => x

end Fast

/-! ## Packed operations: `Bits f → Bits f` -/

/-- Decode a datum (core's `UnpackedFloat.unpack`; every NaN ↦ `notANumber`). -/
def unpack (f : FFormat) (x : Bits f) : UnpackedFloat := UnpackedFloat.unpack f.spec x

/-- Encode a value already rounded to `f` (core's `UnpackedFloat.pack`). -/
def pack (f : FFormat) (u : UnpackedFloat) : Bits f := UnpackedFloat.pack f.spec u

/-- `Sign` of a Boolean sign bit. -/
def signOf (neg : Bool) : Sign := if neg then .negative else .positive

/-- The canonical quiet NaN (core's `packedNaN`). -/
def nan (f : FFormat) : Bits f := pack f .notANumber
/-- Signed infinity. -/
def inf (f : FFormat) (neg : Bool) : Bits f := pack f (.infinity (signOf neg))
/-- Signed zero. -/
def zero (f : FFormat) (neg : Bool) : Bits f := pack f (.zero (signOf neg))

/-- The canonical NaNs are libgcc RISC-V soft-fp's `_FP_NANFRAC`/`_FP_NANSIGN = 0`. -/
theorem nan_bits32 : nan .binary32 = 0x7fc00000#32 := by decide
theorem nan_bits64 : nan .binary64 = 0x7ff8000000000000#64 := by decide
theorem nan_bits128 : nan .binary128 = 0x7fff8000000000000000000000000000#128 := by decide

/-- The sign bit (C `signbit`), NaNs included. -/
def signBit (f : FFormat) (x : Bits f) : Bool := x.msb

/-- The sign-bit mask. -/
def signMask (f : FFormat) : Bits f := BitVec.twoPow _ (f.spec.numBits - 1)

/-- C unary minus: flip the sign bit, NaN payload kept (tcc riscv64 `gen_negf`,
tccgen.c: an `xor` of the sign byte; libgcc is not called). -/
def neg (f : FFormat) (x : Bits f) : Bits f := x ^^^ signMask f

/-- `fabs`: clear the sign bit (payload kept). -/
def abs (f : FFormat) (x : Bits f) : Bits f := x &&& ~~~(signMask f)

/-- C's `fpclassify` classes. -/
inductive FPClass where
  | nan | infinite | zero | subnormal | normal
deriving DecidableEq, Repr

/-- `fpclassify`. -/
def classify (f : FFormat) (x : Bits f) : FPClass :=
  let ex := unpackExponent (spec := f.spec) x
  let mt := unpackMantissa (spec := f.spec) x
  if ex = -1#_ then (if mt = 0#_ then .infinite else .nan)
  else if ex = 0#_ then (if mt = 0#_ then .zero else .subnormal)
  else .normal

/-- `isnan`. -/
def isNaN (f : FFormat) (x : Bits f) : Bool := (unpack f x).isNaN
/-- `isinf`. -/
def isInf (f : FFormat) (x : Bits f) : Bool := (unpack f x).isInf
/-- `x == 0` (either sign). -/
def isZero (f : FFormat) (x : Bits f) : Bool :=
  match unpack f x with
  | .zero _ => true
  | _ => false

/-- `x + y`. -/
def add (f : FFormat) (x y : Bits f) : Bits f := pack f (Fast.add f.spec (unpack f x) (unpack f y))
/-- `x - y`. -/
def sub (f : FFormat) (x y : Bits f) : Bits f := pack f (Fast.sub f.spec (unpack f x) (unpack f y))
/-- `x * y`. -/
def mul (f : FFormat) (x y : Bits f) : Bits f := pack f (Fast.mul f.spec (unpack f x) (unpack f y))
/-- `x / y`. -/
def div (f : FFormat) (x y : Bits f) : Bits f := pack f (Fast.div f.spec (unpack f x) (unpack f y))
/-- `sqrt` (correctly rounded, IEEE 754 §5.4.1; newlib's `sqrt`/`sqrtl` are). -/
def sqrt (f : FFormat) (x : Bits f) : Bits f := pack f (Fast.sqrt f.spec (unpack f x))

/-- `x == y` (IEEE: NaN unordered, `-0 == +0`). -/
def eq (f : FFormat) (x y : Bits f) : Bool := UnpackedFloat.beq (unpack f x) (unpack f y)
/-- `x != y` (true when unordered). -/
def ne (f : FFormat) (x y : Bits f) : Bool := !eq f x y
/-- `x < y`. -/
def lt (f : FFormat) (x y : Bits f) : Bool := UnpackedFloat.lt (unpack f x) (unpack f y)
/-- `x <= y`. -/
def le (f : FFormat) (x y : Bits f) : Bool := UnpackedFloat.le (unpack f x) (unpack f y)
/-- `x > y`. -/
def gt (f : FFormat) (x y : Bits f) : Bool := lt f y x
/-- `x >= y`. -/
def ge (f : FFormat) (x y : Bits f) : Bool := le f y x

/-- `(T)n` for a mathematical integer `n` (RNE; `0 ↦ +0`). -/
def ofInt (f : FFormat) (n : Int) : Bits f := pack f (Fast.ofInt f.spec n)

/-- Truncation toward zero; `none` for NaN and ±∞. The caller checks the C
target type's range (out of range is UB, C11 6.3.1.4#1). -/
def toInt? (f : FFormat) (x : Bits f) : Option Int :=
  match unpack f x with
  | .notANumber => none
  | .infinity _ => none
  | .zero _ => some 0
  | .finite s m e _ => some (Fast.roundToInt s m e)

/-- RISC-V libgcc extensions preserve the NaN sign and shifted payload and
quiet signalling inputs. Narrowing and arithmetic produce a positive canonical
quiet NaN. Fresh volatile-operand Sail bits: test/ieee/nan_policy.c. -/
def widenNaN (src dst : FFormat) (x : Bits src) : Bits dst :=
  let sm := src.spec.mantissaBitsWithoutImplicit
  let dm := dst.spec.mantissaBitsWithoutImplicit
  let payload := (x.toNat % 2 ^ sm) <<< (dm - sm)
  BitVec.ofNat _ ((if signBit src x then 2 ^ (dst.spec.numBits - 1) else 0) +
    (2 ^ dst.spec.exponentBits - 1) * 2 ^ dm + (payload ||| 2 ^ (dm - 1)))

def convert (src dst : FFormat) (x : Bits src) : Bits dst :=
  if isNaN src x && src.rank < dst.rank then widenNaN src dst x
  else pack dst (Fast.roundInto dst.spec (unpack src x))

/-- The ordinary rounding path is unchanged from the recovered core model. -/
theorem convert_not_nan (src dst : FFormat) (x : Bits src) (h : isNaN src x = false) :
    convert src dst x = pack dst (Fast.roundInto dst.spec (unpack src x)) := by
  simp [convert, h]

/-- OPEN: the payload-preserving extension still denotes the model's NaN.
The finite path is `convert_not_nan`; supply the interchange-field proof for
`widenNaN`, rather than identifying its bits with the canonical NaN. -/
def FormatConversionValue_Statement : Prop :=
  ∀ src dst x, unpack dst (convert src dst x) = Fast.roundInto dst.spec (unpack src x)

/-! ## Bridges to Lean core's `Float.Model` (and so to `Float`/`Float32`)

For every format, each operation is core's `UnpackedFloat` operation
(`*_spec`). For `binary64` that is `Float.Model`'s operation on the model's
bits (`*_model`), which in the kernel's logic is native `Float`'s
(`*_float`); likewise `binary32` with `Float32.Model`/`Float32`. -/

theorem add_spec (f : FFormat) (x y : Bits f) : add f x y =
    UnpackedFloat.pack f.spec (UnpackedFloat.add f.spec (UnpackedFloat.unpack f.spec x)
      (UnpackedFloat.unpack f.spec y)) := by
  simp only [add, pack, unpack, Fast.add_eq]
theorem sub_spec (f : FFormat) (x y : Bits f) : sub f x y =
    UnpackedFloat.pack f.spec (UnpackedFloat.sub f.spec (UnpackedFloat.unpack f.spec x)
      (UnpackedFloat.unpack f.spec y)) := by
  simp only [sub, pack, unpack, Fast.sub_eq]
theorem mul_spec (f : FFormat) (x y : Bits f) : mul f x y =
    UnpackedFloat.pack f.spec (UnpackedFloat.mul f.spec (UnpackedFloat.unpack f.spec x)
      (UnpackedFloat.unpack f.spec y)) := by
  simp only [mul, pack, unpack, Fast.mul_eq]
theorem div_spec (f : FFormat) (x y : Bits f) : div f x y =
    UnpackedFloat.pack f.spec (UnpackedFloat.div f.spec (UnpackedFloat.unpack f.spec x)
      (UnpackedFloat.unpack f.spec y)) := by
  simp only [div, pack, unpack, Fast.div_eq]
theorem sqrt_spec (f : FFormat) (x : Bits f) : sqrt f x =
    UnpackedFloat.pack f.spec (UnpackedFloat.sqrt f.spec (UnpackedFloat.unpack f.spec x)) := by
  simp only [sqrt, pack, unpack, Fast.sqrt_eq]
theorem ofInt_spec (f : FFormat) (n : Int) :
    ofInt f n = UnpackedFloat.pack f.spec (UnpackedFloat.ofInt f.spec n) := by
  simp only [ofInt, pack, Fast.ofInt_eq]
/-- On a finite datum, `toInt?` is the model's truncating `toInt` (whose
clamping bounds are then irrelevant). -/
theorem toInt?_spec (f : FFormat) (x : Bits f) (lo hi : Int) (h : (unpack f x).isFinite = true) :
    toInt? f x = some ((unpack f x).toInt lo hi) := by
  unfold toInt?
  revert h
  cases unpack f x <;> simp [UnpackedFloat.isFinite, UnpackedFloat.toInt, Fast.roundToInt_eq]

/-! ### binary64 = `Float.Model` = `Float` -/

theorem add_model (a b : Float.Model) :
    add .binary64 a.toBits.toBitVec b.toBits.toBitVec = (a + b).toBits.toBitVec :=
  (add_spec _ _ _).trans rfl
theorem sub_model (a b : Float.Model) :
    sub .binary64 a.toBits.toBitVec b.toBits.toBitVec = (a - b).toBits.toBitVec :=
  (sub_spec _ _ _).trans rfl
theorem mul_model (a b : Float.Model) :
    mul .binary64 a.toBits.toBitVec b.toBits.toBitVec = (a * b).toBits.toBitVec :=
  (mul_spec _ _ _).trans rfl
theorem div_model (a b : Float.Model) :
    div .binary64 a.toBits.toBitVec b.toBits.toBitVec = (a / b).toBits.toBitVec :=
  (div_spec _ _ _).trans rfl
theorem sqrt_model (a : Float.Model) :
    sqrt .binary64 a.toBits.toBitVec = a.sqrt.toBits.toBitVec :=
  (sqrt_spec _ _).trans rfl
theorem ofInt_model (n : Int) : ofInt .binary64 n = (Float.Model.ofInt n).toBits.toBitVec :=
  (ofInt_spec _ _).trans rfl
theorem eq_model (a b : Float.Model) :
    eq .binary64 a.toBits.toBitVec b.toBits.toBitVec = a.beq b := rfl
theorem lt_model (a b : Float.Model) :
    lt .binary64 a.toBits.toBitVec b.toBits.toBitVec = a.lt b := rfl
theorem le_model (a b : Float.Model) :
    le .binary64 a.toBits.toBitVec b.toBits.toBitVec = a.le b := rfl
theorem isNaN_model (a : Float.Model) : isNaN .binary64 a.toBits.toBitVec = a.isNaN := rfl
theorem isInf_model (a : Float.Model) : isInf .binary64 a.toBits.toBitVec = a.isInf := rfl

theorem add_float (a b : Float) :
    add .binary64 a.toBits.toBitVec b.toBits.toBitVec = (a + b).toBits.toBitVec :=
  add_model a.toModel b.toModel
theorem sub_float (a b : Float) :
    sub .binary64 a.toBits.toBitVec b.toBits.toBitVec = (a - b).toBits.toBitVec :=
  sub_model a.toModel b.toModel
theorem mul_float (a b : Float) :
    mul .binary64 a.toBits.toBitVec b.toBits.toBitVec = (a * b).toBits.toBitVec :=
  mul_model a.toModel b.toModel
theorem div_float (a b : Float) :
    div .binary64 a.toBits.toBitVec b.toBits.toBitVec = (a / b).toBits.toBitVec :=
  div_model a.toModel b.toModel
theorem sqrt_float (a : Float) : sqrt .binary64 a.toBits.toBitVec = a.sqrt.toBits.toBitVec :=
  sqrt_model a.toModel
theorem eq_float (a b : Float) :
    eq .binary64 a.toBits.toBitVec b.toBits.toBitVec = (a == b) := rfl

/-! ### binary32 = `Float32.Model` = `Float32` -/

theorem add_model32 (a b : Float32.Model) :
    add .binary32 a.toBits.toBitVec b.toBits.toBitVec = (a + b).toBits.toBitVec :=
  (add_spec _ _ _).trans rfl
theorem sub_model32 (a b : Float32.Model) :
    sub .binary32 a.toBits.toBitVec b.toBits.toBitVec = (a - b).toBits.toBitVec :=
  (sub_spec _ _ _).trans rfl
theorem mul_model32 (a b : Float32.Model) :
    mul .binary32 a.toBits.toBitVec b.toBits.toBitVec = (a * b).toBits.toBitVec :=
  (mul_spec _ _ _).trans rfl
theorem div_model32 (a b : Float32.Model) :
    div .binary32 a.toBits.toBitVec b.toBits.toBitVec = (a / b).toBits.toBitVec :=
  (div_spec _ _ _).trans rfl
theorem sqrt_model32 (a : Float32.Model) :
    sqrt .binary32 a.toBits.toBitVec = a.sqrt.toBits.toBitVec :=
  (sqrt_spec _ _).trans rfl
theorem ofInt_model32 (n : Int) :
    ofInt .binary32 n = (Float32.Model.ofInt n).toBits.toBitVec :=
  (ofInt_spec _ _).trans rfl
theorem eq_model32 (a b : Float32.Model) :
    eq .binary32 a.toBits.toBitVec b.toBits.toBitVec = a.beq b := rfl
theorem lt_model32 (a b : Float32.Model) :
    lt .binary32 a.toBits.toBitVec b.toBits.toBitVec = a.lt b := rfl
theorem le_model32 (a b : Float32.Model) :
    le .binary32 a.toBits.toBitVec b.toBits.toBitVec = a.le b := rfl

theorem add_float32 (a b : Float32) :
    add .binary32 a.toBits.toBitVec b.toBits.toBitVec = (a + b).toBits.toBitVec :=
  add_model32 a.toModel b.toModel
theorem sub_float32 (a b : Float32) :
    sub .binary32 a.toBits.toBitVec b.toBits.toBitVec = (a - b).toBits.toBitVec :=
  sub_model32 a.toModel b.toModel
theorem mul_float32 (a b : Float32) :
    mul .binary32 a.toBits.toBitVec b.toBits.toBitVec = (a * b).toBits.toBitVec :=
  mul_model32 a.toModel b.toModel
theorem div_float32 (a b : Float32) :
    div .binary32 a.toBits.toBitVec b.toBits.toBitVec = (a / b).toBits.toBitVec :=
  div_model32 a.toModel b.toModel
theorem sqrt_float32 (a : Float32) :
    sqrt .binary32 a.toBits.toBitVec = a.sqrt.toBits.toBitVec :=
  sqrt_model32 a.toModel

/-! ### Canonicalisation: `unpack ∘ pack ∘ unpack = unpack`

Re-encoding a decoded datum decodes to the same value, in every format. So
an operation does not see whether its operand's NaN was canonicalised, and the
bridges also hold in the shape of ship-your-lua's `F64.*_eq_model`, whose
operands are `Float.Model.ofBits x` (NaNs canonicalised): `add_ofBits`, … -/

theorem unpackSign_packComponents {spec : Format} {s : Sign} {e m} :
    unpackSign (packComponents spec s e m) = s.toBitVec := by
  ext i hi
  have : i = 0 := by omega
  subst this
  simp [unpackSign, packComponents, BitVec.getLsbD_eq_getElem, BitVec.getLsbD_append,
    BitVec.getElem_append]

theorem ofBitVec_toBitVec (s : Sign) : Sign.ofBitVec s.toBitVec = s := by
  cases s <;> decide

theorem neg_one_ne_zero (spec : Format) : (-1#spec.exponentBits) ≠ 0#_ := by
  have := spec.he
  intro h
  have h2 := congrArg BitVec.toNat h
  rw [BitVec.neg_one_eq_allOnes, BitVec.toNat_allOnes] at h2
  have : 2 ≤ 2 ^ spec.exponentBits := by
    calc 2 = 2 ^ 1 := rfl
      _ ≤ 2 ^ spec.exponentBits := Nat.pow_le_pow_right (by decide) (by omega)
  simp at h2; omega

theorem nanMantissa_ne_zero (spec : Format) :
    (1#spec.mantissaBitsWithoutImplicit <<< (spec.mantissaBitsWithoutImplicit - 1)) ≠ 0#_ := by
  have := spec.hm
  intro h
  have h2 := congrArg BitVec.toNat h
  simp only [BitVec.toNat_shiftLeft, BitVec.toNat_ofNat] at h2
  have h1 : 1 % 2 ^ spec.mantissaBitsWithoutImplicit = 1 :=
    Nat.mod_eq_of_lt (Nat.one_lt_two_pow (by omega))
  rw [h1, Nat.shiftLeft_eq, Nat.one_mul,
    Nat.mod_eq_of_lt (Nat.pow_lt_pow_right (by decide) (by omega))] at h2
  exact absurd h2 (Nat.ne_of_gt (Nat.two_pow_pos _))

theorem four_le (spec : Format) : 4 ≤ 2 ^ spec.exponentBits := by
  have := spec.he
  calc 4 = 2 ^ 2 := rfl
    _ ≤ 2 ^ spec.exponentBits := Nat.pow_le_pow_right (by decide) this

theorem unpack_pack_unpack (spec : Format) (b : BitVec spec.numBits) :
    UnpackedFloat.unpack spec (UnpackedFloat.pack spec (UnpackedFloat.unpack spec b)) =
      UnpackedFloat.unpack spec b := by
  have hm := spec.hm
  have h4 := four_le spec
  have hz := (neg_one_ne_zero spec).symm
  generalize hs : Sign.ofBitVec (unpackSign b) = s
  by_cases h1 : unpackExponent b = -1#_
  · by_cases h2 : unpackMantissa b = 0#_
    · have hu : UnpackedFloat.unpack spec b = .infinity s := by
        simp only [UnpackedFloat.unpack, h1, h2, ite_true, hs]
      rw [hu]
      simp [UnpackedFloat.pack, packedInfinity, UnpackedFloat.unpack, unpackSign_packComponents,
        ofBitVec_toBitVec]
    · have hu : UnpackedFloat.unpack spec b = .notANumber := by
        simp only [UnpackedFloat.unpack, h1, h2, ite_true, ite_false]
      rw [hu]
      simp [UnpackedFloat.pack, packedNaN, UnpackedFloat.unpack, nanMantissa_ne_zero]
  · by_cases h3 : unpackExponent b = 0#_
    · by_cases h2 : unpackMantissa b = 0#_
      · have hu : UnpackedFloat.unpack spec b = .zero s := by
          simp [UnpackedFloat.unpack, h3, h2, hs, hz]
        rw [hu]
        simp [UnpackedFloat.pack, packedZero, UnpackedFloat.unpack, unpackSign_packComponents,
          ofBitVec_toBitVec, hz]
      · have hpos : 0 < (unpackMantissa b).toNat := by
          simpa [BitVec.toNat_pos, BitVec.pos_iff_ne_zero] using h2
        have hu : UnpackedFloat.unpack spec b = .finite s (unpackMantissa b).toNat
            ((0 : Int) - (spec.exponentBias + spec.mantissaBitsWithoutImplicit) + 1) hpos := by
          simp only [UnpackedFloat.unpack, h3, h2, hs, hz,
            ite_false, ite_true, dite_false, BitVec.toNat_zero, Int.natCast_zero]
        rw [hu]
        have hlt : (unpackMantissa b).toNat < 2 ^ spec.mantissaBitsWithoutImplicit := BitVec.isLt _
        have hl := (Nat.log2_lt (Nat.ne_of_gt hpos)).mpr hlt
        have hb : ((0 : Int) - (spec.exponentBias + spec.mantissaBitsWithoutImplicit) + 1 +
            spec.exponentBias + spec.mantissaBitsWithoutImplicit).toNat = 1 := by omega
        have h5 : ¬ (unpackMantissa b).toNat.log2 + 1 = spec.mantissaBits := by
          unfold Format.mantissaBits; omega
        have h6 : ¬ 2 ^ spec.exponentBits ≤ 1 + 1 := by omega
        simp only [UnpackedFloat.pack, hb, h6, h5, ite_false, BitVec.ofNat_toNat, BitVec.setWidth_eq]
        simp [UnpackedFloat.unpack, unpackSign_packComponents, ofBitVec_toBitVec, hz, h2]
    · have hpos : 0 < (1#1 ++ unpackMantissa b).toNat := by
        simp only [BitVec.toNat_append]
        rw [← Nat.shiftLeft_add_eq_or_of_lt (BitVec.isLt _), Nat.shiftLeft_eq]
        have := Nat.two_pow_pos spec.mantissaBitsWithoutImplicit
        simp; omega
      have hu : UnpackedFloat.unpack spec b = .finite s (1#1 ++ unpackMantissa b).toNat
          (((unpackExponent b).toNat : Int) - (spec.exponentBias + spec.mantissaBitsWithoutImplicit))
          hpos := by
        simp only [UnpackedFloat.unpack, h1, h3, hs, ite_false]
      rw [hu]
      have hv : (1#1 ++ unpackMantissa b).toNat =
          2 ^ spec.mantissaBitsWithoutImplicit + (unpackMantissa b).toNat := by
        simp only [BitVec.toNat_append]
        rw [← Nat.shiftLeft_add_eq_or_of_lt (BitVec.isLt _), Nat.shiftLeft_eq]
        simp
      have hlt : (unpackMantissa b).toNat < 2 ^ spec.mantissaBitsWithoutImplicit := BitVec.isLt _
      have hl : (1#1 ++ unpackMantissa b).toNat.log2 = spec.mantissaBitsWithoutImplicit := by
        rw [Nat.log2_eq_iff (by omega), hv, Nat.pow_succ]; omega
      have hex : (unpackExponent b).toNat + 1 < 2 ^ spec.exponentBits := by
        have := BitVec.isLt (unpackExponent b)
        have hne : (unpackExponent b).toNat ≠ 2 ^ spec.exponentBits - 1 := by
          intro h; apply h1
          apply BitVec.eq_of_toNat_eq
          rw [h, BitVec.neg_one_eq_allOnes, BitVec.toNat_allOnes]
        omega
      have hb : (((unpackExponent b).toNat : Int) - (spec.exponentBias +
          spec.mantissaBitsWithoutImplicit) + spec.exponentBias +
          spec.mantissaBitsWithoutImplicit).toNat = (unpackExponent b).toNat := by omega
      have h5 : (1#1 ++ unpackMantissa b).toNat.log2 + 1 = spec.mantissaBits := by
        unfold Format.mantissaBits; omega
      have h6 : ¬ 2 ^ spec.exponentBits ≤ (unpackExponent b).toNat + 1 := by omega
      simp only [UnpackedFloat.pack, hb, h6, h5, ite_false, ite_true, BitVec.ofNat_toNat,
        BitVec.setWidth_eq]
      have hmt : BitVec.setWidth spec.mantissaBitsWithoutImplicit (1#1 ++ unpackMantissa b) =
          unpackMantissa b := by
        rw [← BitVec.ofNat_toNat]
        apply BitVec.eq_of_toNat_eq
        rw [BitVec.toNat_ofNat, hv, Nat.add_mod_left, Nat.mod_eq_of_lt hlt]
      rw [hmt]
      simp [UnpackedFloat.unpack, unpackSign_packComponents, ofBitVec_toBitVec, h1, h3]

/-- The model's `ofBits` (NaNs canonicalised) decodes as the raw bits do. -/
theorem unpack_ofBits (x : BitVec 64) :
    (Float.Model.ofBits (UInt64.ofBitVec x)).unpack = unpack .binary64 x :=
  unpack_pack_unpack Format.binary64 x

theorem unpack_ofBits32 (x : BitVec 32) :
    (Float32.Model.ofBits (UInt32.ofBitVec x)).unpack = unpack .binary32 x :=
  unpack_pack_unpack Format.binary32 x

/-- The model operand `Float.Model.ofBits x` of ship-your-lua's `F64.toModel x`. -/
abbrev modelOf (x : BitVec 64) : Float.Model := Float.Model.ofBits (UInt64.ofBitVec x)

/-- **Bridge in `F64` shape.** With `Lua.Num.F64.add_eq_model x y : F64.add x y =
(Float.Model.add (toModel x) (toModel y)).toBits.toBitVec` this gives
`F64.add x y = CerbIEEE.add .binary64 x y` (`toModel = modelOf`). -/
theorem add_ofBits (x y : BitVec 64) :
    add .binary64 x y = (Float.Model.add (modelOf x) (modelOf y)).toBits.toBitVec := by
  refine (add_spec _ _ _).trans ?_
  show _ = UnpackedFloat.pack _ (UnpackedFloat.add _ (modelOf x).unpack (modelOf y).unpack)
  rw [unpack_ofBits, unpack_ofBits]; rfl
theorem sub_ofBits (x y : BitVec 64) :
    sub .binary64 x y = (Float.Model.sub (modelOf x) (modelOf y)).toBits.toBitVec := by
  refine (sub_spec _ _ _).trans ?_
  show _ = UnpackedFloat.pack _ (UnpackedFloat.sub _ (modelOf x).unpack (modelOf y).unpack)
  rw [unpack_ofBits, unpack_ofBits]; rfl
theorem mul_ofBits (x y : BitVec 64) :
    mul .binary64 x y = (Float.Model.mul (modelOf x) (modelOf y)).toBits.toBitVec := by
  refine (mul_spec _ _ _).trans ?_
  show _ = UnpackedFloat.pack _ (UnpackedFloat.mul _ (modelOf x).unpack (modelOf y).unpack)
  rw [unpack_ofBits, unpack_ofBits]; rfl
theorem div_ofBits (x y : BitVec 64) :
    div .binary64 x y = (Float.Model.div (modelOf x) (modelOf y)).toBits.toBitVec := by
  refine (div_spec _ _ _).trans ?_
  show _ = UnpackedFloat.pack _ (UnpackedFloat.div _ (modelOf x).unpack (modelOf y).unpack)
  rw [unpack_ofBits, unpack_ofBits]; rfl
theorem sqrt_ofBits (x : BitVec 64) :
    sqrt .binary64 x = (Float.Model.sqrt (modelOf x)).toBits.toBitVec := by
  refine (sqrt_spec _ _).trans ?_
  show _ = UnpackedFloat.pack _ (UnpackedFloat.sqrt _ (modelOf x).unpack)
  rw [unpack_ofBits]; rfl
theorem compare_ofBits (x y : BitVec 64) :
    UnpackedFloat.compare (unpack .binary64 x) (unpack .binary64 y) =
      Float.Model.compare (modelOf x) (modelOf y) := by
  show _ = UnpackedFloat.compare (modelOf x).unpack (modelOf y).unpack
  rw [unpack_ofBits, unpack_ofBits]
theorem lt_ofBits (x y : BitVec 64) : lt .binary64 x y = Float.Model.lt (modelOf x) (modelOf y) := by
  show _ = UnpackedFloat.lt (modelOf x).unpack (modelOf y).unpack
  rw [unpack_ofBits, unpack_ofBits]; rfl
theorem le_ofBits (x y : BitVec 64) : le .binary64 x y = Float.Model.le (modelOf x) (modelOf y) := by
  show _ = UnpackedFloat.le (modelOf x).unpack (modelOf y).unpack
  rw [unpack_ofBits, unpack_ofBits]; rfl
theorem eq_ofBits (x y : BitVec 64) : eq .binary64 x y = Float.Model.beq (modelOf x) (modelOf y) := by
  show _ = UnpackedFloat.beq (modelOf x).unpack (modelOf y).unpack
  rw [unpack_ofBits, unpack_ofBits]; rfl

/-- Operations ignore NaN payloads: an operand may be canonicalised first. -/
theorem unpack_canon (f : FFormat) (x : Bits f) : unpack f (pack f (unpack f x)) = unpack f x :=
  unpack_pack_unpack f.spec x

/-! ## C floating literals (C11 6.4.4.2), correctly rounded

The exact-rational method of `CerbFloat.of_string` (semantics-audit repair
2026-09-11), generalised to the three formats and routed through the model:
a decimal literal `m · 10^e` is core's `ofScientific` (one rounding of the
exact value), a hexadecimal `m · 2^e` is core's `round`. -/

namespace Literal

/-- Decimal-digit run → value. -/
def digitsToNat (s : List Char) : Nat :=
  s.foldl (fun acc c => acc * 10 + (c.toNat - '0'.toNat)) 0

/-- Is `c` a hexadecimal digit? -/
def isHexDigit (c : Char) : Bool :=
  c.isDigit || ('a' ≤ c && c ≤ 'f') || ('A' ≤ c && c ≤ 'F')

/-- Value of a hexadecimal digit. -/
def hexVal (c : Char) : Nat :=
  if c.isDigit then c.toNat - '0'.toNat
  else if 'a' ≤ c && c ≤ 'f' then c.toNat - 'a'.toNat + 10
  else c.toNat - 'A'.toNat + 10

/-- `[+|-] D+` → (value, rest); `none` without a digit. -/
def parseExponent (rest : List Char) : Option (Int × List Char) :=
  let (neg, rest) := match rest with
    | '+' :: r => (false, r)
    | '-' :: r => (true, r)
    | r => (false, r)
  let digits := rest.takeWhile Char.isDigit
  if digits.isEmpty then none
  else
    let v : Int := (digitsToNat digits : Nat)
    some (if neg then -v else v, rest.drop digits.length)

/-- Decimal floating constant (no sign, no suffix):
`D* [. D*] [(e|E) [+|-] D+]` with at least one digit → `(m, e)`, value `m · 10^e`. -/
def parseDecimal (cs : List Char) : Option (Nat × Int) := do
  let intPart := cs.takeWhile Char.isDigit
  let cs := cs.drop intPart.length
  let (fracPart, cs) :=
    match cs with
    | '.' :: rest => (rest.takeWhile Char.isDigit, rest.drop (rest.takeWhile Char.isDigit).length)
    | _ => ([], cs)
  if intPart.isEmpty && fracPart.isEmpty then failure
  let (exp10, cs) ← match cs with
    | 'e' :: rest | 'E' :: rest => parseExponent rest
    | _ => some ((0 : Int), cs)
  if !cs.isEmpty then failure
  return (digitsToNat (intPart ++ fracPart), exp10 - fracPart.length)

/-- Hexadecimal floating constant after `0x` (no sign, no suffix):
`H* [. H*] (p|P) [+|-] D+` with at least one digit → `(m, e)`, value `m · 2^e`.
The binary exponent is mandatory (C11 6.4.4.2#1). -/
def parseHex (cs : List Char) : Option (Nat × Int) := do
  let intPart := cs.takeWhile isHexDigit
  let cs := cs.drop intPart.length
  let (fracPart, cs) :=
    match cs with
    | '.' :: rest => (rest.takeWhile isHexDigit, rest.drop (rest.takeWhile isHexDigit).length)
    | _ => ([], cs)
  if intPart.isEmpty && fracPart.isEmpty then failure
  let (p, cs) ← match cs with
    | 'p' :: rest | 'P' :: rest => parseExponent rest
    | _ => none
  if !cs.isEmpty then failure
  let mantissa := (intPart ++ fracPart).foldl (fun a c => a * 16 + hexVal c) 0
  return (mantissa, p - 4 * fracPart.length)

end Literal

/-- Apply a sign to an unpacked value (exact). -/
def applySign (neg : Bool) (u : UnpackedFloat) : UnpackedFloat := if neg then u.neg else u

/-- `(-1)^neg · m · 10^e`, correctly rounded to `f` (core's `ofScientific`). -/
def ofDecimal (f : FFormat) (neg : Bool) (m : Nat) (e : Int) : Bits f :=
  pack f (applySign neg (Fast.ofScientific f.spec m e))

/-- `(-1)^neg · m · 2^e`, correctly rounded to `f` (core's `round`). -/
def ofBinary (f : FFormat) (neg : Bool) (m : Nat) (e : Int) : Bits f :=
  pack f (if m = 0 then .zero (signOf neg) else Fast.round f.spec (signOf neg) m e)

/-- A decimal literal is `Float.Model.ofScientific` (the model of Lean's own
`OfScientific Float`). -/
theorem ofDecimal_model (m : Nat) (e : Int) :
    ofDecimal .binary64 false m e = (Float.Model.ofScientific m e).toBits.toBitVec := by
  simp only [ofDecimal, applySign, pack, Fast.ofScientific_eq]
  rfl

/-- The correctly rounded value of a C floating literal of format `f`: decimal
or hexadecimal, an optional leading `+`/`-`, an optional suffix `f F l L` (the
caller picks `f` from it). `none` on malformed text. -/
def ofLiteral (f : FFormat) (s : String) : Option (Bits f) :=
  let cs := s.toList
  let cs := match cs.getLast? with
    | some 'f' | some 'F' | some 'l' | some 'L' => cs.dropLast
    | _ => cs
  let (neg, cs) := match cs with
    | '-' :: rest => (true, rest)
    | '+' :: rest => (false, rest)
    | _ => (false, cs)
  match cs with
  | '0' :: 'x' :: rest | '0' :: 'X' :: rest =>
    (Literal.parseHex rest).map fun (m, e) => ofBinary f neg m e
  | _ => (Literal.parseDecimal cs).map fun (m, e) => ofDecimal f neg m e

/-! ## Object representation: little-endian bytes -/

/-- The `n` low bytes of `v`, least significant first. -/
def natToBytesLE : Nat → Nat → List UInt8
  | 0, _ => []
  | n + 1, v => UInt8.ofNat v :: natToBytesLE n (v / 256)

/-- Little-endian bytes → value. -/
def natOfBytesLE : List UInt8 → Nat
  | [] => 0
  | b :: bs => b.toNat + 256 * natOfBytesLE bs

theorem natToBytesLE_length (n v : Nat) : (natToBytesLE n v).length = n := by
  induction n generalizing v with
  | zero => rfl
  | succ n ih => simp [natToBytesLE, ih]

theorem natOfBytesLE_natToBytesLE (n v : Nat) :
    natOfBytesLE (natToBytesLE n v) = v % 256 ^ n := by
  induction n generalizing v with
  | zero => simp [natToBytesLE, natOfBytesLE, Nat.mod_one]
  | succ n ih =>
    simp only [natToBytesLE, natOfBytesLE, ih]
    rw [Nat.pow_succ, Nat.mul_comm (256 ^ n) 256, Nat.mod_mul]
    simp

/-- The object representation of a datum: `f.bytes` bytes, little-endian. -/
def toBytesLE (f : FFormat) (x : Bits f) : List UInt8 := natToBytesLE f.bytes x.toNat

/-- A datum from its object representation; `none` unless exactly `f.bytes` bytes. -/
def ofBytesLE? (f : FFormat) (bs : List UInt8) : Option (Bits f) :=
  if bs.length = f.bytes then some (BitVec.ofNat _ (natOfBytesLE bs)) else none

theorem toBytesLE_length (f : FFormat) (x : Bits f) : (toBytesLE f x).length = f.bytes :=
  natToBytesLE_length _ _

theorem bytes_pow (f : FFormat) : 256 ^ f.bytes = 2 ^ f.spec.numBits := by
  cases f <;> rfl

theorem ofBytesLE_toBytesLE (f : FFormat) (x : Bits f) : ofBytesLE? f (toBytesLE f x) = some x := by
  simp only [ofBytesLE?, toBytesLE_length, ↓reduceIte]
  simp only [toBytesLE, natOfBytesLE_natToBytesLE, bytes_pow]
  rw [Nat.mod_eq_of_lt x.isLt, BitVec.ofNat_toNat, BitVec.setWidth_eq]

/-! ## Values that carry their format -/

/-- A floating value with its format (for callers whose values are untyped). -/
structure FVal where
  /-- The C floating type. -/
  fmt : FFormat
  /-- Its bits. -/
  bits : Bits fmt

namespace FVal

/-- Convert to format `g`. -/
def to (g : FFormat) (v : FVal) : FVal := if v.fmt = g then v else ⟨g, convert v.fmt g v.bits⟩

/-- Apply a binary operation in the common type of the operands (C11 6.3.1.8). -/
def lift₂ (op : (f : FFormat) → Bits f → Bits f → Bits f) (a b : FVal) : FVal :=
  let g := FFormat.common a.fmt b.fmt
  ⟨g, op g (convert a.fmt g a.bits) (convert b.fmt g b.bits)⟩

/-- Apply a comparison in the common type of the operands. -/
def cmp₂ (op : (f : FFormat) → Bits f → Bits f → Bool) (a b : FVal) : Bool :=
  let g := FFormat.common a.fmt b.fmt
  op g (convert a.fmt g a.bits) (convert b.fmt g b.bits)

end FVal

/-! ## printf (C11 7.21.6.1) for floating arguments

All digits come from the exact binary value `m · 2^e`, rounded half-even
on the exact value (glibc; newlib's `_dtoa_r` in modes 2/3). -/

/-- One `%` conversion of a floating argument: flags, width, precision and the
conversion character (`f F e E g G a A`). -/
structure PrintfSpec where
  /-- The conversion character. -/
  conv : Char
  /-- `-`: left-justify. -/
  minus : Bool := false
  /-- `+`: always a sign. -/
  plus : Bool := false
  /-- space: a space where `+` would print a plus. -/
  space : Bool := false
  /-- `#`: alternative form (keep the point; `%g` keeps trailing zeros). -/
  hash : Bool := false
  /-- `0`: pad with zeros after the sign (finite values only). -/
  zero : Bool := false
  /-- Minimum field width. -/
  width : Nat := 0
  /-- Precision (`none`: the default, 6 for `f e g`, exact for `a`). -/
  precision : Option Nat := none
deriving Repr, DecidableEq

/-- How `%a` chooses the leading hex digit. C11 7.21.6.1#8 leaves it
unspecified except that it is nonzero for a normal number. -/
inductive HexStyle where
  /-- glibc/libquadmath: `1` for normal, `0` for subnormal numbers; a rounding
  carry may make it `2` (`printf("%.0a", 0.1)` = `0x2p-4`). -/
  | glibc
  /-- newlib (`vfprintf.c` `cvt` with `ch == 'a'`, through `frexp`): every
  nonzero value normalised to leading `1`, subnormals included. -/
  | newlib
deriving Repr, DecidableEq, Inhabited

namespace Printf

/-- Round-half-even of `m · 2^e · 10^p`, exactly. -/
def scaledRound (m : Nat) (e p : Int) : Nat :=
  let num := m * 2 ^ e.toNat * 10 ^ p.toNat
  let den := 2 ^ (-e).toNat * 10 ^ (-p).toNat
  let q := num / den
  let r := num % den
  if 2 * r > den then q + 1
  else if 2 * r < den then q
  else if q % 2 == 0 then q else q + 1

/-- `10^X ≤ m · 2^e`, exactly. -/
def pow10Le (m : Nat) (e X : Int) : Bool :=
  2 ^ (-e).toNat * 10 ^ X.toNat ≤ m * 2 ^ e.toNat * 10 ^ (-X).toNat

/-- `⌊log₁₀ (m · 2^e)⌋` for `m > 0`: the estimate `⌊k · 0.30103⌋` from the
binade `k`, corrected by exact comparisons (it is within 2 of the answer). -/
def decExp (m : Nat) (e : Int) : Int :=
  let k : Int := (m.log2 : Int) + e
  let est : Int := (k * 30103) / 100000
  let up := fun X => if pow10Le m e (X + 1) then X + 1 else X
  let down := fun X => if pow10Le m e X then X else X - 1
  down (down (up (up est)))

/-- `n` zeros. -/
def zeros (n : Nat) : String := String.ofList (List.replicate n '0')

/-- `%f` digits of `m · 2^e` with precision `prec` (no sign). -/
def fixedDigits (m : Nat) (e : Int) (prec : Nat) (hash : Bool) : String :=
  let s := toString (scaledRound m e prec)
  let s := if s.length < prec + 1 then zeros (prec + 1 - s.length) ++ s else s
  let cs := s.toList
  let ip := String.ofList (cs.take (cs.length - prec))
  let fp := String.ofList (cs.drop (cs.length - prec))
  if prec = 0 then (if hash then ip ++ "." else ip) else ip ++ "." ++ fp

/-- The `±dd` exponent text (at least two digits). -/
def expText (X : Int) : String :=
  let a := toString X.natAbs
  (if X < 0 then "-" else "+") ++ (if a.length < 2 then "0" ++ a else a)

/-- Decimal digits `d₀d₁…d_prec` and exponent `X` of `m · 2^e` rounded to
`prec + 1` significant digits (`m = 0`: all zeros, `X = 0`). -/
def sigDigits (m : Nat) (e : Int) (prec : Nat) : List Char × Int :=
  if m = 0 then (List.replicate (prec + 1) '0', 0)
  else
    let X := decExp m e
    let n := scaledRound m e ((prec : Int) - X)
    if n ≥ 10 ^ (prec + 1) then ((toString (n / 10)).toList, X + 1)
    else ((toString n).toList, X)

/-- `%e` digits of `m · 2^e` with precision `prec` (no sign). -/
def expDigits (m : Nat) (e : Int) (prec : Nat) (hash upper : Bool) : String :=
  let (ds, X) := sigDigits m e prec
  let mant := match ds with
    | d :: rest => String.ofList (d :: (if prec > 0 || hash then '.' :: rest else []))
    | [] => "0"
  mant ++ (if upper then "E" else "e") ++ expText X

/-- Drop trailing fraction zeros and then a bare point (`%g` without `#`). -/
def stripZeros (s : String) : String :=
  let cs := s.toList
  if cs.contains '.' then
    let cs := (cs.reverse.dropWhile (· = '0')).dropWhile (· = '.')
    String.ofList cs.reverse
  else s

/-- `%g` digits of `m · 2^e` (no sign). -/
def generalDigits (m : Nat) (e : Int) (prec : Nat) (hash upper : Bool) : String :=
  let P := if prec = 0 then 1 else prec
  let X := (sigDigits m e (P - 1)).2
  if X < P ∧ -4 ≤ X then
    let s := fixedDigits m e (P - 1 - X).toNat hash
    if hash then s else stripZeros s
  else
    let s := expDigits m e (P - 1) hash upper
    if hash then s
    else
      let cs := s.toList
      let mant := cs.takeWhile (fun c => c ≠ 'e' && c ≠ 'E')
      String.ofList ((stripZeros (String.ofList mant)).toList ++ cs.drop mant.length)

/-- Hexadecimal digit. -/
def hexDigit (upper : Bool) (d : Nat) : Char :=
  if d < 10 then Char.ofNat ('0'.toNat + d)
  else Char.ofNat ((if upper then 'A' else 'a').toNat + d - 10)

/-- `k` hex digits of `v` (most significant first). -/
def hexDigitsOf (upper : Bool) (v : Nat) : Nat → List Char
  | 0 => []
  | k + 1 => hexDigitsOf upper (v / 16) k ++ [hexDigit upper (v % 16)]

/-- `%a` digits (no sign, no `0x`) of a canonical `finite`/`zero` of format
`spec`: leading digit, `.`, fraction, `p`, binary exponent. -/
def hexBody (style : HexStyle) (spec : Format) (m : Nat) (e : Int) (prec : Option Nat)
    (hash upper : Bool) : String :=
  let mb := spec.mantissaBitsWithoutImplicit
  let fd := (mb + 3) / 4
  -- (leading digit, fraction bits (mb of them), binary exponent)
  let (lead, frac, X) : Nat × Nat × Int :=
    if m = 0 then (0, 0, 0)
    else if 2 ^ mb ≤ m then (1, m - 2 ^ mb, e + mb)
    else match style with
      | .glibc => (0, m, e + mb)
      | .newlib =>
        let k := mb - m.log2
        (1, (m <<< k) - 2 ^ mb, e + mb - k)
  let F := frac <<< (4 * fd - mb)               -- fd hex digits
  let P := match prec with
    | some p => p
    | none => fd - ((hexDigitsOf false F fd).reverse.takeWhile (· = '0')).length
  -- V = lead.digits as an integer of P hex fraction digits, rounded half-even
  let V : Nat :=
    if P ≥ fd then (lead * 16 ^ fd + F) * 16 ^ (P - fd)
    else
      let D := 4 * (fd - P)
      let q := (lead * 16 ^ fd + F) >>> D
      let r := (lead * 16 ^ fd + F) % 2 ^ D
      let h := 2 ^ (D - 1)
      if r > h || (r = h && q % 2 = 1) then q + 1 else q
  let lead' := V / 16 ^ P
  let fracDs := hexDigitsOf upper (V % 16 ^ P) P
  let point := if P > 0 || hash then "." else ""
  String.ofList [hexDigit upper lead'] ++ point ++ String.ofList fracDs ++
    (if upper then "P" else "p") ++ (if X < 0 then "-" else "+") ++ toString X.natAbs

/-- Justify `pre ++ body` in the field: `-` pads right with spaces; `0`
(when `zeroOk`) inserts zeros between `pre` (sign, `0x`) and `body`;
otherwise pad left with spaces. -/
def justify (sp : PrintfSpec) (pre body : String) (zeroOk : Bool) : String :=
  let len := pre.length + body.length
  if sp.width ≤ len then pre ++ body
  else if sp.minus then pre ++ body ++ String.ofList (List.replicate (sp.width - len) ' ')
  else if sp.zero && zeroOk then pre ++ zeros (sp.width - len) ++ body
  else String.ofList (List.replicate (sp.width - len) ' ') ++ pre ++ body

/-- One conversion of a datum of format `f` (`binary64` or `binary128`). -/
def formatCore (style : HexStyle) (sp : PrintfSpec) (f : FFormat) (x : Bits f) : String :=
  let c := sp.conv
  let upper := c = 'F' || c = 'E' || c = 'G' || c = 'A'
  let sign := if signBit f x then "-" else if sp.plus then "+" else if sp.space then " " else ""
  let special (s : String) := justify sp sign (if upper then s.toUpper else s) false
  let finite (m : Nat) (e : Int) : String :=
    let lc := c.toLower
    if lc = 'a' then
      justify sp (sign ++ (if upper then "0X" else "0x"))
        (hexBody style f.spec m e sp.precision sp.hash upper) true
    else
      let prec := sp.precision.getD 6
      let body :=
        if lc = 'e' then expDigits m e prec sp.hash upper
        else if lc = 'g' then generalDigits m e prec sp.hash upper
        else fixedDigits m e prec sp.hash
      justify sp sign body true
  match unpack f x with
  | .notANumber => special "nan"
  | .infinity _ => special "inf"
  | .zero _ => finite 0 0
  | .finite _ m e _ => finite m e

end Printf

/-- printf of a floating argument of format `f` under one conversion. A
`float` argument is first promoted to `double` (default argument promotion,
C11 6.5.2.2#7). `style` picks the `%a` leading digit (default: newlib, the
C library of the `tcc -run` image). -/
def format (sp : PrintfSpec) (f : FFormat) (x : Bits f) (style : HexStyle := .newlib) : String :=
  match f, x with
  | .binary32, x => Printf.formatCore style sp .binary64 (convert .binary32 .binary64 x)
  | .binary64, x => Printf.formatCore style sp .binary64 x
  | .binary128, x => Printf.formatCore style sp .binary128 x
