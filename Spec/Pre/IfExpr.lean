/-
The controlling expression of `#if` / `#elif`, C11 §6.10.1.

§6.10.1p4: after macro replacement (with `defined` evaluated, see
`Spec/Pre/Expand.lean`), "all remaining identifiers (including those
lexically identical to keywords) are replaced with the pp-number 0, and then
each preprocessing token is converted into a token. The resulting tokens
compose the controlling constant expression which is evaluated according to
the rules of 6.6. For the purposes of this token conversion and evaluation,
all signed integer types and all unsigned integer types act as if they have
the same representation as, respectively, the types intmax_t and uintmax_t
defined in the header <stdint.h>."

So every operand is an intmax_t or a uintmax_t (`Val`), the usual arithmetic
conversions (§6.3.1.8) reduce to "unsigned if either is", and:
* signed overflow, division by zero, and shifts by a negative amount or by
  the width or more are undefined (§6.5p5, §6.5.5p5, §6.5.7p3–4): errors,
  but only in an evaluated operand (§6.6p3 and the `&&`, `||`, `?:`
  short circuits, §6.5.13–15);
* `>>` of a negative value is implementation-defined (§6.5.7p5):
  arithmetic shift;
* integer constants per §6.4.4.1 (a decimal constant without `u` that does
  not fit intmax_t has no type: an error); floating constants and string
  literals are not allowed (§6.6p6);
* character constants per §6.4.4.4 (implementation-defined value:
  `PreCfg.charSigned`; a multi-character constant is the big-endian
  concatenation of its bytes, as an `int`).
The grammar is §6.6's conditional-expression (no comma or assignment
operators: §6.6p3).
-/
import Spec.Pre.Expand

namespace CSem.Pre

/-- The width of intmax_t / uintmax_t (RV64 lp64 and tcc: 64). -/
def intmaxBits : Nat := 64

/-- A value of the controlling expression: unsigned (uintmax_t) or signed
(intmax_t), with its mathematical value (in range). -/
structure Val where
  u : Bool
  v : Int
  deriving Repr, Inhabited, DecidableEq

def maxS : Int := 2 ^ (intmaxBits - 1) - 1
def minS : Int := -(2 ^ (intmaxBits - 1))
def modU : Int := 2 ^ intmaxBits

/-- Conversion to uintmax_t (§6.3.1.3p2). -/
def toU (x : Int) : Int := x % modU
/-- Two's complement reinterpretation of a uintmax_t value as intmax_t. -/
def toS (x : Int) : Int := let y := toU x; if y > maxS then y - modU else y

/-- A token of the controlling expression after conversion. -/
inductive ETok
  | num (v : Val)
  | op (s : Bytes)
  deriving Repr, Inhabited

/-- §6.4.4.1: the value of an integer constant spelled `s`; `none` if `s` is
not an integer constant. `bin`: accept `0b` (Ext.binaryLiterals). -/
def parseIntConst (bin : Bool) (s : Bytes) : Option Val :=
  let lower (c : UInt8) : UInt8 := if 65 ≤ c ∧ c ≤ 90 then c + 32 else c
  -- split off the suffix: the longest tail made of u, l
  let isSuf (c : UInt8) : Bool := lower c = 117 ∨ lower c = 108
  let digitsPart := (s.reverse.dropWhile isSuf).reverse
  let suf := (s.drop digitsPart.length).map lower
  let sufOk := suf ∈ [[], bs "u", bs "l", bs "ll", bs "ul", bs "lu", bs "ull", bs "llu"] ∧
    -- `lL` / `Ll` are not suffixes (§6.4.4.1p1: long-long-suffix is ll or LL)
    (let raw := s.drop digitsPart.length
     ¬(raw.any (· = 108) ∧ raw.any (· = 76)))
  let isU := suf.contains 117
  let digitVal (c : UInt8) : Nat :=
    if isDigit c then (c - 48).toNat else if 97 ≤ lower c ∧ lower c ≤ 102 then (lower c - 87).toNat else 99
  let (base, ds) : Nat × Bytes :=
    match digitsPart with
    | 48 :: x :: rest => if lower x = 120 then (16, rest)
                         else if bin ∧ lower x = 98 then (2, rest)
                         else (8, x :: rest)
    | 48 :: [] => (8, [])
    | _ => (10, digitsPart)
  let okDigits := ds.all (fun c => digitVal c < base)
  let hexEmpty := (base = 16 ∨ base = 2) ∧ ds.isEmpty
  if ¬sufOk ∨ ¬okDigits ∨ hexEmpty ∨ digitsPart.isEmpty then none
  else
    let n : Nat := ds.foldl (fun acc c => acc * base + digitVal c) 0
    let n : Int := n
    if n ≥ modU then none
    else if isU then some ⟨true, n⟩
    else if n ≤ maxS then some ⟨false, n⟩
    else if base ≠ 10 then some ⟨true, n⟩   -- octal/hex: unsigned type in the list
    else none                                -- decimal: no type (§6.4.4.1p6)

/-- Decode one UTF-8 sequence (for wide character constants). -/
def utf8Decode : Bytes → Option (Nat × Bytes)
  | c :: rest =>
    let c := c.toNat
    if c < 0x80 then some (c, rest)
    else
      let (n, init) := if c ≥ 0xF0 then (3, c &&& 0x07) else if c ≥ 0xE0 then (2, c &&& 0x0F)
                       else if c ≥ 0xC0 then (1, c &&& 0x1F) else (0, c)
      let cont := rest.take n
      if cont.length = n ∧ cont.all (fun b => b.toNat &&& 0xC0 = 0x80) then
        some (cont.foldl (fun a b => a * 64 + (b.toNat &&& 0x3F)) init, rest.drop n)
      else some (c, rest)
  | [] => none

/-- §6.4.4.4: the code units of a character constant's c-char-sequence.
`wide`: code points (decoding UTF-8), else bytes. Escapes: simple, octal,
hexadecimal, universal character names. -/
def charUnits (wide : Bool) : Nat → Bytes → List Nat → Option (List Nat)
  | 0, _, _ => none
  | _ + 1, [], acc => some acc.reverse
  | fuel + 1, 92 :: rest, acc =>
    match rest with
    | c :: rest' =>
      let simple : Option Nat :=
        if c = 39 then some 39 else if c = 34 then some 34 else if c = 63 then some 63
        else if c = 92 then some 92 else if c = 97 then some 7 else if c = 98 then some 8
        else if c = 102 then some 12 else if c = 110 then some 10 else if c = 114 then some 13
        else if c = 116 then some 9 else if c = 118 then some 11 else none
      match simple with
      | some v => charUnits wide fuel rest' (v :: acc)
      | none =>
        if 48 ≤ c ∧ c ≤ 55 then
          let ds := (rest.take 3).takeWhile (fun d => 48 ≤ d ∧ d ≤ 55)
          charUnits wide fuel (rest.drop ds.length) (ds.foldl (fun a d => a * 8 + (d - 48).toNat) 0 :: acc)
        else if c = 120 then
          let ds := rest'.takeWhile isHex
          if ds.isEmpty then none
          else
            let hv (d : UInt8) : Nat := if isDigit d then (d - 48).toNat
              else if 97 ≤ d then (d - 87).toNat else (d - 55).toNat
            charUnits wide fuel (rest'.drop ds.length) (ds.foldl (fun a d => a * 16 + hv d) 0 :: acc)
        else if c = 117 ∨ c = 85 then
          let n := if c = 117 then 4 else 8
          let ds := rest'.take n
          if ds.length = n ∧ ds.all isHex then
            let hv (d : UInt8) : Nat := if isDigit d then (d - 48).toNat
              else if 97 ≤ d then (d - 87).toNat else (d - 55).toNat
            let cp := ds.foldl (fun a d => a * 16 + hv d) 0
            let us := if wide then [cp] else (utf8Encode cp).map (·.toNat)
            charUnits wide fuel (rest'.drop n) (us.reverse ++ acc)
          else none
        else none
    | [] => none
  | fuel + 1, s, acc =>
    if wide then
      match utf8Decode s with
      | some (cp, rest) => charUnits wide fuel rest (cp :: acc)
      | none => none
    else
      match s with
      | c :: rest => charUnits wide fuel rest (c.toNat :: acc)
      | [] => some acc.reverse

/-- Wrap to a signed 32-bit `int`. -/
def wrapI32 (x : Int) : Int := let y := x % (2 ^ 32); if y ≥ 2 ^ 31 then y - 2 ^ 32 else y

/-- §6.4.4.4p10–11: the value of a character constant in `#if`. -/
def charConstVal (cfg : PreCfg) (s : Bytes) : Option Val :=
  let (pre, rest) : Bytes × Bytes :=
    match s with
    | 76 :: r => (bs "L", r) | 117 :: r => (bs "u", r) | 85 :: r => (bs "U", r) | r => ([], r)
  let body := (rest.drop 1).dropLast
  let wide := pre ≠ []
  match charUnits wide (body.length + 1) body [] with
  | none => none
  | some [] => none
  | some us =>
    if ¬wide then
      if us.any (· > 255) then none   -- §6.4.4.4p9: out of range for unsigned char
      else match us with
        | [c] => some ⟨false, if cfg.charSigned ∧ c ≥ 128 then (c : Int) - 256 else c⟩
        | _ => some ⟨false, wrapI32 ((us.foldl (fun a c => a * 256 + c) 0 : Nat) : Int)⟩
    else
      let c : Int := us.getLast?.getD 0
      if pre = bs "L" then some ⟨false, wrapI32 c⟩          -- wchar_t: int
      else if pre = bs "u" then some ⟨false, c % 65536⟩     -- char16_t
      else some ⟨false, c % (2 ^ 32)⟩                        -- char32_t

/-- §6.10.1p4: convert the macro-replaced tokens. -/
def convertIfToks (ctx : ExpCtx) (ts : List Tok) : Except PreError (List ETok) :=
  ts.mapM (fun t =>
    match t.kind with
    | .ident => pure (.num ⟨false, 0⟩)
    | .ppNumber =>
      match parseIntConst ctx.cfg.ext.binaryLiterals t.spell with
      | some v => pure (.num v)
      | none => throw (ctx.err .ifExpr t.line s!"invalid integer constant in #if (§6.10.1p1, §6.4.4.1)")
    | .charConst =>
      match charConstVal ctx.cfg t.spell with
      | some v => pure (.num v)
      | none => throw (ctx.err .ifExpr t.line "invalid character constant in #if (§6.4.4.4)")
    | .punct =>
      -- digraphs are the same punctuators (§6.4.6p3); none is an operator of #if
      pure (.op t.spell)
    | _ => throw (ctx.err .ifExpr t.line "token not allowed in #if (§6.6p6)"))

/-- The binary operators with their precedence (§6.5.5–6.5.14). -/
def binPrec (s : Bytes) : Option Nat :=
  if s = bs "*" ∨ s = bs "/" ∨ s = bs "%" then some 10
  else if s = bs "+" ∨ s = bs "-" then some 9
  else if s = bs "<<" ∨ s = bs ">>" then some 8
  else if s = bs "<" ∨ s = bs ">" ∨ s = bs "<=" ∨ s = bs ">=" then some 7
  else if s = bs "==" ∨ s = bs "!=" then some 6
  else if s = bs "&" then some 5
  else if s = bs "^" then some 4
  else if s = bs "|" then some 3
  else if s = bs "&&" then some 2
  else if s = bs "||" then some 1
  else none

/-- A signed result: in range, or undefined behaviour (error if evaluated). -/
def signedRes (ev : Bool) (x : Int) : Except String Val :=
  if ev ∧ (x > maxS ∨ x < minS) then .error "signed overflow in #if (§6.5p5)" else .ok ⟨false, toS x⟩

def bitop (f : Nat → Nat → Nat) (a b : Int) : Int := (f (toU a).toNat (toU b).toNat : Int)

/-- §6.5.5–6.5.14 on intmax_t/uintmax_t. `ev`: the operand is evaluated. -/
def applyBin (ev : Bool) (op : Bytes) (a b : Val) : Except String Val :=
  let u := a.u || b.u
  let ua := toU a.v
  let ub := toU b.v
  let arith (fs : Int → Int → Int) : Except String Val :=
    if u then .ok ⟨true, toU (fs ua ub)⟩ else signedRes ev (fs a.v b.v)
  let cmp (r : Bool) : Except String Val := .ok ⟨false, if r then 1 else 0⟩
  if op = bs "*" then arith (· * ·)
  else if op = bs "+" then arith (· + ·)
  else if op = bs "-" then arith (· - ·)
  else if op = bs "/" ∨ op = bs "%" then
    let d := if u then ub else b.v
    if d = 0 then (if ev then .error "division by zero in #if (§6.5.5p5)" else .ok ⟨u, 0⟩)
    else arith (if op = bs "/" then Int.tdiv else Int.tmod)
  else if op = bs "<<" ∨ op = bs ">>" then
    -- §6.5.7: the result has the (promoted) type of the left operand
    let n := if b.u then ub else b.v
    if ev ∧ (n < 0 ∨ n ≥ intmaxBits) then .error "shift count out of range in #if (§6.5.7p3)"
    else
      let k := n.toNat % intmaxBits
      if op = bs "<<" then
        if a.u then .ok ⟨true, toU (ua * 2 ^ k)⟩
        else if ev ∧ a.v < 0 then .error "left shift of a negative value in #if (§6.5.7p4)"
        else signedRes ev (a.v * 2 ^ k)
      else
        if a.u then .ok ⟨true, ua / 2 ^ k⟩
        else .ok ⟨false, a.v / 2 ^ k⟩     -- arithmetic shift (Int./ floors): impl.-defined
  else if op = bs "<" then cmp (if u then ua < ub else a.v < b.v)
  else if op = bs ">" then cmp (if u then ua > ub else a.v > b.v)
  else if op = bs "<=" then cmp (if u then ua ≤ ub else a.v ≤ b.v)
  else if op = bs ">=" then cmp (if u then ua ≥ ub else a.v ≥ b.v)
  else if op = bs "==" then cmp (if u then ua = ub else a.v = b.v)
  else if op = bs "!=" then cmp (if u then ua ≠ ub else a.v ≠ b.v)
  else if op = bs "&" then .ok ⟨u, if u then bitop (· &&& ·) a.v b.v else toS (bitop (· &&& ·) a.v b.v)⟩
  else if op = bs "^" then .ok ⟨u, if u then bitop (· ^^^ ·) a.v b.v else toS (bitop (· ^^^ ·) a.v b.v)⟩
  else if op = bs "|" then .ok ⟨u, if u then bitop (· ||| ·) a.v b.v else toS (bitop (· ||| ·) a.v b.v)⟩
  else .error "unknown operator"

/-- Recursive-descent evaluation of a conditional-expression (§6.5.15) by
precedence climbing. `ev`: evaluated (not under a short circuit). Returns
the value and the rest of the tokens. -/
def evalCond : Nat → Bool → List ETok → Except String (Val × List ETok)
  | 0, _, _ => .error "#if expression too deep"
  | fuel + 1, ev, ts => do
    let (c, ts) ← evalBin fuel ev 1 ts
    match ts with
    | .op q :: ts' =>
      if q = bs "?" then do
        let cv := c.v ≠ 0
        let (a, ts) ← evalCond fuel (ev && cv) ts'
        match ts with
        | .op col :: ts'' =>
          if col = bs ":" then do
            let (b, ts) ← evalCond fuel (ev && !cv) ts''
            let u := a.u || b.u
            let r := if cv then a else b
            pure (⟨u, if u then toU r.v else r.v⟩, ts)
          else .error "expected ':' in #if"
        | _ => .error "expected ':' in #if"
      else pure (c, ts)
    | _ => pure (c, ts)
where
  /-- Binary operators of precedence ≥ `minP`. -/
  evalBin : Nat → Bool → Nat → List ETok → Except String (Val × List ETok)
    | 0, _, _, _ => .error "#if expression too deep"
    | fuel + 1, ev, minP, ts => do
      let (lhs, ts) ← evalUnary fuel ev ts
      evalLoop fuel ev minP lhs ts
  evalLoop : Nat → Bool → Nat → Val → List ETok → Except String (Val × List ETok)
    | 0, _, _, _, _ => .error "#if expression too deep"
    | fuel + 1, ev, minP, lhs, ts =>
      match ts with
      | .op o :: ts' =>
        match binPrec o with
        | some p =>
          if p < minP then pure (lhs, ts)
          else if o = bs "&&" then do
            let (rhs, ts) ← evalBin fuel (ev && lhs.v ≠ 0) (p + 1) ts'
            evalLoop fuel ev minP ⟨false, if lhs.v ≠ 0 ∧ rhs.v ≠ 0 then 1 else 0⟩ ts
          else if o = bs "||" then do
            let (rhs, ts) ← evalBin fuel (ev && lhs.v = 0) (p + 1) ts'
            evalLoop fuel ev minP ⟨false, if lhs.v ≠ 0 ∨ rhs.v ≠ 0 then 1 else 0⟩ ts
          else do
            let (rhs, ts) ← evalBin fuel ev (p + 1) ts'
            let r ← applyBin ev o lhs rhs
            evalLoop fuel ev minP r ts
        | none => pure (lhs, ts)
      | _ => pure (lhs, ts)
  evalUnary : Nat → Bool → List ETok → Except String (Val × List ETok)
    | 0, _, _ => .error "#if expression too deep"
    | fuel + 1, ev, ts =>
      match ts with
      | .num v :: ts' => pure (v, ts')
      | .op o :: ts' =>
        if o = bs "(" then do
          let (v, ts) ← evalCond fuel ev ts'
          match ts with
          | .op c :: ts'' => if c = bs ")" then pure (v, ts'') else .error "expected ')' in #if"
          | _ => .error "expected ')' in #if"
        else if o = bs "+" then evalUnary fuel ev ts'
        else if o = bs "-" then do
          let (v, ts) ← evalUnary fuel ev ts'
          if v.u then pure (⟨true, toU (-v.v)⟩, ts) else do
            let r ← signedRes ev (-v.v)
            pure (r, ts)
        else if o = bs "~" then do
          let (v, ts) ← evalUnary fuel ev ts'
          pure (⟨v.u, if v.u then toU (modU - 1 - toU v.v) else -v.v - 1⟩, ts)
        else if o = bs "!" then do
          let (v, ts) ← evalUnary fuel ev ts'
          pure (⟨false, if v.v = 0 then 1 else 0⟩, ts)
        else .error s!"unexpected operator in #if"
      | [] => .error "missing operand in #if"

/-- §6.10.1: evaluate a controlling expression (already macro-replaced).
`true` iff it is nonzero. -/
def evalIf (ctx : ExpCtx) (line : Nat) (ts : List Tok) : Except PreError Bool := do
  if ts.isEmpty then throw (ctx.err .ifExpr line "#if with no expression (§6.10.1)")
  let es ← convertIfToks ctx ts
  match evalCond (4 * es.length + 8) true es with
  | .ok (v, []) => pure (v.v ≠ 0)
  | .ok (_, _) => throw (ctx.err .ifExpr line "garbage at the end of #if expression")
  | .error m => throw (ctx.err .ifExpr line m)

end CSem.Pre
