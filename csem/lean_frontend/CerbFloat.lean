import CerbStopSites
import LemLib
import CerbIEEE

/-! Pure floating seams. Values retain their format and object bits. Arithmetic
uses the recovered `CerbIEEE.Fast` operations, proved equal to `Float.Model`.
No machine summary is imported here. The libgcc/image connection is an open
obligation, independently of the definitional arithmetic specification. -/
namespace CerbIEEE.FVal

instance : Inhabited FVal := ⟨⟨.binary64, CerbIEEE.zero .binary64 false⟩⟩
instance : BEq FVal := ⟨cmp₂ CerbIEEE.eq⟩
instance : Add FVal := ⟨lift₂ CerbIEEE.add⟩
instance : Sub FVal := ⟨lift₂ CerbIEEE.sub⟩
instance : Mul FVal := ⟨lift₂ CerbIEEE.mul⟩
instance : Div FVal := ⟨lift₂ CerbIEEE.div⟩
instance : Neg FVal := ⟨fun v => ⟨v.fmt, CerbIEEE.neg v.fmt v.bits⟩⟩
instance (n : Nat) : OfNat FVal n := ⟨⟨.binary64, CerbIEEE.ofInt .binary64 n⟩⟩
instance : OfScientific FVal := ⟨fun m neg e =>
  ⟨.binary64, CerbIEEE.ofDecimal .binary64 false m (if neg then -(e : Int) else e)⟩⟩
def isNaN (v : FVal) : Bool := CerbIEEE.isNaN v.fmt v.bits
def isInf (v : FVal) : Bool := CerbIEEE.isInf v.fmt v.bits
instance : Ord FVal where
  compare a b :=
    if a.isNaN then (if b.isNaN then .eq else .lt)
    else if b.isNaN then .gt
    else if cmp₂ CerbIEEE.lt a b then .lt
    else if cmp₂ CerbIEEE.lt b a then .gt else .eq
end CerbIEEE.FVal

namespace CerbFloat
open CerbIEEE
abbrev Value := FVal

def floatEq (x y : Value) : Bool := FVal.cmp₂ CerbIEEE.eq x y
def floatLt (x y : Value) : Bool := FVal.cmp₂ CerbIEEE.lt x y
def floatLe (x y : Value) : Bool := FVal.cmp₂ CerbIEEE.le x y
def floatAdd (x y : Value) : Value := x + y
def floatSub (x y : Value) : Value := x - y
def floatMul (x y : Value) : Value := x * y
def floatDiv (x y : Value) : Value := x / y
def of_int (n : Int) : Value := ⟨.binary64, CerbIEEE.ofInt .binary64 n⟩

def ofStringAs (fmt : FFormat) (s : String) : Value :=
  match ofLiteral fmt s with
  | some bits => ⟨fmt, bits⟩
  | none => failwithI (CerbStop.render (.upstreamCrash (CerbStopSites.floatParse s!"CerbFloat.ofStringAs: malformed literal {s}")))

/-- Untyped Core/legacy Float literals are binary64; C literals use `strFval`
which selects the configured format before parsing any digits. -/
def of_string (s : String) : Value := ofStringAs .binary64 s

def formatIEEE (conv : Char) (minus plus space hash zero : Bool) (width : Nat)
    (precision : Option Nat) (v : Value) : String :=
  CerbIEEE.format { conv, minus, plus, space, hash, zero, width, precision } v.fmt v.bits

def formatFixed (prec : Nat) (v : Value) : String :=
  CerbIEEE.format { conv := 'f', precision := some prec } v.fmt v.bits

def string_of_float (v : Value) : String :=
  let s := CerbIEEE.format { conv := 'g', precision := some 12 } v.fmt v.bits
  if v.isNaN || v.isInf || s.contains '.' || s.contains 'e' then s else s ++ "."

def truncToInt (v : Value) : Int :=
  match CerbIEEE.toInt? v.fmt v.bits with
  | some n => n
  | none => failwithI (CerbStop.render (.upstreamCrash (CerbStopSites.floatToInt "CerbFloat.truncToInt: non-finite Core value")))
def to_int (v : Value) : Int := truncToInt v
end CerbFloat
