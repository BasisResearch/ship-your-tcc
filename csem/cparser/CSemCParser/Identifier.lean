import CSemCParser.Parser
import Spec.Pre.Lex

namespace CSemCParser.Phase7
open CSem.Pre

/-- ISO C11 N1570 Annex D.1 (fixed numeric intervals, not a host Unicode
library whose version can change). -/
def identifierRanges : List (Nat × Nat) :=
  [(0xA8,0xA8),(0xAA,0xAA),(0xAD,0xAD),(0xAF,0xAF),(0xB2,0xB5),
   (0xB7,0xBA),(0xBC,0xBE),(0xC0,0xD6),(0xD8,0xF6),(0xF8,0xFF),
   (0x100,0x167F),(0x1681,0x180D),(0x180F,0x1FFF),
   (0x200B,0x200D),(0x202A,0x202E),(0x203F,0x2040),(0x2054,0x2054),(0x2060,0x206F),
   (0x2070,0x218F),(0x2460,0x24FF),(0x2776,0x2793),(0x2C00,0x2DFF),(0x2E80,0x2FFF),
   (0x3004,0x3007),(0x3021,0x302F),(0x3031,0x303F),(0x3040,0xD7FF),
   (0xF900,0xFD3D),(0xFD40,0xFDCF),(0xFDF0,0xFE44),(0xFE47,0xFFFD)]

def inRanges (n : Nat) (ranges : List (Nat × Nat)) : Bool :=
  ranges.any (fun (lo, hi) => lo ≤ n ∧ n ≤ hi)

def allowedNonAscii (n : Nat) : Bool :=
  inRanges n identifierRanges ∨ (0x10000 ≤ n ∧ n ≤ 0xEFFFD ∧ n % 0x10000 ≤ 0xFFFD)

/-- Annex D.2: combining marks that cannot start an identifier. -/
def disallowedInitially (n : Nat) : Bool :=
  inRanges n [(0x300,0x36F),(0x1DC0,0x1DFF),(0x20D0,0x20FF),(0xFE20,0xFE2F)]

def hexValue (b : UInt8) : Nat :=
  if b ≤ 57 then b.toNat - 48 else if b ≤ 70 then b.toNat - 55 else b.toNat - 87

/-- Additional C11 §6.4.3 constraints on UCNs, also within literals. Other
escapes are skipped as units, so "\\\\u0041" is not a UCN. -/
def validUniversalEscapes : Bytes → Bool
  | [] => true
  | 92 :: c :: rest =>
    if c = 117 ∨ c = 85 then
      let count := if c = 117 then 4 else 8
      let digits := rest.take count
      let cp := digits.foldl (fun n d => n * 16 + hexValue d) 0
      digits.length = count ∧ digits.all CSem.Pre.isHex ∧
        (0xA0 ≤ cp ∨ cp = 0x24 ∨ cp = 0x40 ∨ cp = 0x60) ∧
        cp ≤ 0x10FFFF ∧ ¬(0xD800 ≤ cp ∧ cp ≤ 0xDFFF) ∧
        validUniversalEscapes (rest.drop count)
    else validUniversalEscapes rest
  | _ :: rest => validUniversalEscapes rest
termination_by cs => cs.length

def hexDigits (n width : Nat) : Bytes :=
  (List.range width).reverse.map (fun k =>
    let d := n / 16 ^ k % 16
    (if d < 10 then 48 + d else 87 + d).toUInt8)

/-- Parser identity is a canonical ASCII UCN spelling. Both raw UCN forms
and literal UTF-8 reach the same name; source spelling remains in PPToken.
Canonical UCNs let the independent unchanged Menhir lexer see the same
language identifiers without requiring it to accept literal UTF-8 names. -/
def canonicalIdentifier (gnu : Bool) (spell : Bytes) : Except String Bytes := do
  if !validUniversalEscapes spell then throw "invalid identifier universal character name"
  let key := identifierKey spell
  let text ← match String.fromUTF8? ⟨key.toArray⟩ with
    | some s => pure s
    | none => throw "invalid UTF-8 identifier"
  let chars := text.toList
  if chars.isEmpty then throw "empty identifier"
  let mut out : Bytes := []
  for (c, i) in chars.zipIdx do
    let cp := c.toNat
    if cp < 128 then
      if !(Lex.isNondigit cp ∨ (i > 0 ∧ Lex.isDigit cp) ∨ (gnu ∧ cp = 36)) then
        throw "invalid identifier character"
      out := out ++ [cp.toUInt8]
    else
      if !allowedNonAscii cp ∨ (i = 0 ∧ disallowedInitially cp) then
        throw "invalid C11 identifier character"
      out := out ++ (if cp ≤ 0xFFFF then [92,117] ++ hexDigits cp 4
        else [92,85] ++ hexDigits cp 8)
  pure out

/-- Spelling of the language token in the declared virtual lexer view.
Only identifiers are converted; strings, characters and numbers stay raw. -/
def parserSpelling (cfg : ParseCfg) (p : PPToken) : Except String Bytes :=
  if p.kind = .ident then canonicalIdentifier cfg.gnu p.spell else .ok p.spell

end CSemCParser.Phase7
