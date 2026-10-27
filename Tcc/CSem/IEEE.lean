import CerbIEEE

/-! Open F1/F7 connection. The recovered arithmetic is proved equal to Lean's
`Float.Model`; the compiled RV64I libgcc calls have not been connected to it.
An image-specific Sail call relation, with lp64 argument/result registers,
code bytes, stack/memory frame and resource preconditions, must instantiate
`SailCalls`. No libgcc machine summary is imported by this module. -/
namespace Tcc.CSem.IEEE
open CerbIEEE

inductive BinaryOp where
  | add | sub | mul | div
  deriving Inhabited

def eval (f : FFormat) (op : BinaryOp) (x y : Bits f) : Bits f :=
  match op with
  | .add => CerbIEEE.add f x y
  | .sub => CerbIEEE.sub f x y
  | .mul => CerbIEEE.mul f x y
  | .div => CerbIEEE.div f x y

/-- To be supplied by actual image-pinned Sail helper-call specifications.
These fields describe returned bits, including NaN sign/payload. They are
interfaces, not assumed correctness laws or a substitute for Sail runs. -/
structure SailCalls where
  binary : (f : FFormat) → BinaryOp → Bits f → Bits f → Bits f → Prop
  fromInt64 : (f : FFormat) → Bool → BitVec 64 → Bits f → Prop
  convert : (src dst : FFormat) → Bits src → Bits dst → Prop

/-- Correctness laws still owed by F7. The reverse implications demand an
actual returning call for the computed result, so an empty relation fails. -/
structure RuntimeAgreement (calls : SailCalls) : Prop where
  binary : ∀ f op x y out, calls.binary f op x y out ↔ out = eval f op x y
  fromInt64 : ∀ f signed n out, calls.fromInt64 f signed n out ↔
    out = CerbIEEE.ofInt f (if signed then n.toInt else (n.toNat : Int))
  convert : ∀ src dst x out, src ≠ dst → (calls.convert src dst x out ↔ out = CerbIEEE.convert src dst x)

/-- OPEN: supply concrete, byte-checked Sail relations, then prove these laws.
The statement alone asserts neither an image instantiation nor correctness. -/
def RV64LibgccIEEE_Statement (calls : SailCalls) : Prop := RuntimeAgreement calls

/-- OPEN, with a concrete target/model discrepancy retained in the report:
the image's strtof literal path must meet the selected direct-RNE policy, or
its implementation-defined literal accuracy must be reconciled explicitly.
A failed source never becomes a Supported exclusion. -/
def TccLiteralIEEE_Statement
    (parseLiteral : (f : FFormat) → String → Option (Bits f)) : Prop :=
  ∀ f text bits, CerbIEEE.ofLiteral f text = some bits → parseLiteral f text = some bits

/-- Actual image-pinned newlib calls, to be supplied with their ABI, resources
and memory/stdio frames. The current image has retained sqrtl/printf failures. -/
structure NewlibCalls where
  sqrt : (f : FFormat) → Bits f → Bits f → Prop
  printf : (f : FFormat) → PrintfSpec → Bits f → String → Prop

structure NewlibAgreement (calls : NewlibCalls) : Prop where
  sqrt : ∀ f x out, calls.sqrt f x out ↔ out = CerbIEEE.sqrt f x
  printf : ∀ f sp x out, calls.printf f sp x out ↔ out = CerbIEEE.format sp f x

/-- OPEN F7: the observations in the report prevent claiming this connection
for the present image. Repair or reconcile the library, never narrow CSem's
binary128 values to make the observed output agree. -/
def NewlibIEEE_Statement (calls : NewlibCalls) : Prop := NewlibAgreement calls

end Tcc.CSem.IEEE
