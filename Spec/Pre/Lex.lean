/-
Translation phase 3 (C11 §5.1.1.2p1): "The source file is decomposed into
preprocessing tokens and sequences of white-space characters (including
comments). … Each comment is replaced by one space character. New-line
characters are retained."

The decomposition follows §6.4: "the next preprocessing token is the longest
sequence of characters that could constitute a preprocessing token"
(§6.4p4), with the categories of §6.4p1:
* header-name (§6.4.7), only within a `#include` directive (§6.4p4) — and,
  with `Ext.hasInclude`, as the operand of `__has_include(` in `#if`/`#elif`;
* identifier (§6.4.2), with universal character names (§6.4.3) and the
  implementation-defined characters `PreCfg.extraIdChar`;
* pp-number (§6.4.8);
* character-constant (§6.4.4.4) with prefixes `L`, `u`, `U`;
* string-literal (§6.4.5) with prefixes `u8`, `u`, `U`, `L` (the first
  three only with `PreCfg.c11Prefixes`: C99 has only `L`);
* punctuator (§6.4.6), digraphs included;
* any other single non-white-space character.

A `'` or `"` that does not begin a complete character constant or string
literal on its line is undefined behaviour (§6.4p3). The lexer makes it an
`other` token (so that skipped groups, §6.10.1p6, may contain apostrophes);
phase 4 rejects it where it is not skipped. An unterminated comment is an
error (§5.1.1.2p2: a file shall not end in a partial comment).

The result is a list of `LItem`s: tokens, and the retained new-lines (with
their physical line), which phase 4 needs to find directive lines.
-/
import Spec.Pre.Phase12

namespace CSem.Pre

/-- A preprocessing token inside phase 4: kind, spelling, whether white space
(a space, a comment, a new-line) precedes it, the physical line it is
attributed to (for `__LINE__`), and its hide set (Prosser's algorithm,
`Spec/Pre/Expand.lean`). -/
structure Tok where
  kind : PPKind
  spell : Bytes
  ws : Bool
  line : Nat
  hs : List Bytes
  deriving Repr, Inhabited

/-- The output of phase 3: a token or a retained new-line. -/
inductive LItem
  | tok (t : Tok)
  | nl (line : Nat)
  deriving Repr, Inhabited

/-- Character classes. -/
def isDigit (c : UInt8) : Bool := 48 ≤ c ∧ c ≤ 57
def isNondigit (c : UInt8) : Bool := (97 ≤ c ∧ c ≤ 122) ∨ (65 ≤ c ∧ c ≤ 90) ∨ c = 95
def isHex (c : UInt8) : Bool := isDigit c ∨ (97 ≤ c ∧ c ≤ 102) ∨ (65 ≤ c ∧ c ≤ 70)
/-- §6.4p3 white space other than new-line: space, HT, VT, FF (plus the
implementation's extra white space). -/
def isSpace (cfg : PreCfg) (c : UInt8) : Bool :=
  c = 32 ∨ c = 9 ∨ c = 11 ∨ c = 12 ∨ cfg.extraSpace c

/-- §6.4.2.1 identifier-nondigit, single-character part. -/
def isIdStart (cfg : PreCfg) (c : UInt8) : Bool := isNondigit c ∨ cfg.extraIdChar c

/-- §6.4.3: the length of a universal character name `\uXXXX` / `\UXXXXXXXX`
at the head of the stream, or 0. -/
def ucnLen (cs : List PChar) : Nat :=
  match cs with
  | b :: u :: rest =>
    if b.c = 92 ∧ (u.c = 117 ∨ u.c = 85) then
      let n := if u.c = 117 then 4 else 8
      if (rest.take n).length = n ∧ (rest.take n).all (fun p => isHex p.c) then n + 2 else 0
    else 0
  | _ => 0

/-- Consume an identifier tail: identifier-nondigits, digits, UCNs. -/
def identTail (cfg : PreCfg) : Nat → List PChar → Bytes → Bytes × List PChar
  | 0, cs, acc => (acc.reverse, cs)
  | fuel + 1, cs, acc =>
    match cs with
    | [] => (acc.reverse, [])
    | p :: rest =>
      if isIdStart cfg p.c ∨ isDigit p.c then identTail cfg fuel rest (p.c :: acc)
      else
        let n := ucnLen cs
        if n > 0 then identTail cfg fuel (cs.drop n) ((cs.take n).reverse.map (·.c) ++ acc)
        else (acc.reverse, cs)

/-- §6.4.8 pp-number tail: digit, identifier-nondigit, `e+ e- E+ E- p+ p- P+ P-`, `.`. -/
def ppNumTail (cfg : PreCfg) : Nat → List PChar → Bytes → Bytes × List PChar
  | 0, cs, acc => (acc.reverse, cs)
  | fuel + 1, cs, acc =>
    match cs with
    | [] => (acc.reverse, [])
    | p :: rest =>
      let sign : Bool := match rest with
        | q :: _ => (p.c = 101 ∨ p.c = 69 ∨ p.c = 112 ∨ p.c = 80) ∧ (q.c = 43 ∨ q.c = 45)
        | [] => false
      if sign then ppNumTail cfg fuel rest.tail ((rest.head!).c :: p.c :: acc)
      else if isDigit p.c ∨ isIdStart cfg p.c ∨ p.c = 46 then ppNumTail cfg fuel rest (p.c :: acc)
      else
        let n := ucnLen cs
        if n > 0 then ppNumTail cfg fuel (cs.drop n) ((cs.take n).reverse.map (·.c) ++ acc)
        else (acc.reverse, cs)

/-- The body of a character constant or string literal after its opening
quote `q`, up to and including the closing quote: c-chars / s-chars are any
character but the quote, backslash and new-line, or an escape sequence
(a backslash and the character after it — the escape's own syntax is a
phase-7 matter). `none` if the line ends first. -/
def quotedTail (q : UInt8) : Nat → List PChar → Bytes → Option (Bytes × List PChar)
  | 0, _, _ => none
  | fuel + 1, cs, acc =>
    match cs with
    | [] => none
    | p :: rest =>
      if p.c = 10 then none
      else if p.c = q then some ((q :: acc).reverse, rest)
      else if p.c = 92 then
        match rest with
        | r :: rest' => if r.c = 10 then none else quotedTail q fuel rest' (r.c :: p.c :: acc)
        | [] => none
      else quotedTail q fuel rest (p.c :: acc)

/-- §6.4.7 header-name tail after the opening `<` or `"`: any characters but
new-line and the closing delimiter. -/
def headerTail (close : UInt8) : Nat → List PChar → Bytes → Option (Bytes × List PChar)
  | 0, _, _ => none
  | fuel + 1, cs, acc =>
    match cs with
    | [] => none
    | p :: rest =>
      if p.c = 10 then none
      else if p.c = close then some ((close :: acc).reverse, rest)
      else headerTail close fuel rest (p.c :: acc)

/-- The punctuators of §6.4.6p1, longest first. -/
def punctuators : List Bytes :=
  [bs "%:%:", bs "...", bs "<<=", bs ">>=",
   bs "->", bs "++", bs "--", bs "<<", bs ">>", bs "<=", bs ">=", bs "==", bs "!=",
   bs "&&", bs "||", bs "*=", bs "/=", bs "%=", bs "+=", bs "-=", bs "&=", bs "^=",
   bs "|=", bs "##", bs "<:", bs ":>", bs "<%", bs "%>", bs "%:",
   bs "[", bs "]", bs "(", bs ")", bs "{", bs "}", bs ".", bs "&", bs "*", bs "+",
   bs "-", bs "~", bs "!", bs "/", bs "%", bs "<", bs ">", bs "^", bs "|", bs "?",
   bs ":", bs ";", bs "=", bs ",", bs "#"]

/-- The longest punctuator at the head of the stream. -/
def punctAt (cs : List PChar) : Option Bytes :=
  let head := (cs.take 4).map (·.c)
  punctuators.find? (fun p => p.isPrefixOf head)

/-- Lex one preprocessing token at the head of `cs` (which starts with a
non-white-space character). `hdr`: a header-name may be recognised here.
`fuel` bounds the inner loops (the caller passes a bound on `cs.length`).
Returns kind, spelling and the rest. -/
def lexToken (cfg : PreCfg) (hdr : Bool) (fuel : Nat) (cs : List PChar) : PPKind × Bytes × List PChar :=
  match cs with
  | [] => (.other, [], [])
  | p :: rest =>
    let c := p.c
    -- header-name (§6.4.7), only where `hdr`
    let hdrTok : Option (PPKind × Bytes × List PChar) :=
      if hdr ∧ (c = 60 ∨ c = 34) then
        (headerTail (if c = 60 then 62 else 34) fuel rest [c]).map (fun (s, r) => (.headerName, s, r))
      else none
    -- character constants and string literals with their prefixes
    let lit (pre : Bytes) (r : List PChar) : Option (PPKind × Bytes × List PChar) :=
      match r with
      | q :: r' =>
        if q.c = 39 then (quotedTail 39 fuel r' (39 :: pre.reverse)).map (fun (s, r'') => (.charConst, s, r''))
        else if q.c = 34 then (quotedTail 34 fuel r' (34 :: pre.reverse)).map (fun (s, r'') => (.strLit, s, r''))
        else none
      | [] => none
    let prefixed : Option (PPKind × Bytes × List PChar) :=
      if c = 39 ∨ c = 34 then lit [] cs
      else if c = 76 ∨ (cfg.c11Prefixes ∧ c = 85) then lit [c] rest    -- L U
      else if cfg.c11Prefixes ∧ c = 117 then                          -- u, u8
        match rest with
        | e :: r' =>
          if e.c = 56 then
            match r' with
            | q :: _ => if q.c = 34 then lit [117, 56] r' else lit [c] rest
            | [] => none
          else lit [c] rest
        | [] => none
      else none
    match hdrTok with
    | some t => t
    | none =>
    match prefixed with
    | some t => t
    | none =>
    if isDigit c ∨ (c = 46 ∧ (rest.head?.map (fun q => isDigit q.c)).getD false) then
      let (s, r) := ppNumTail cfg fuel rest [c]
      (.ppNumber, s, r)
    else if isIdStart cfg c ∨ ucnLen cs > 0 then
      let (s, r) := identTail cfg fuel cs []
      (.ident, s, r)
    else
      match punctAt cs with
      | some s => (.punct, s, cs.drop s.length)
      | none => (.other, [c], rest)

/-- Skip a block comment body after `/*`; `none` if the file ends first. -/
def skipBlockComment : Nat → List PChar → Option (List PChar)
  | 0, _ => none
  | fuel + 1, cs =>
    match cs with
    | [] => none
    | p :: rest =>
      if p.c = 42 ∧ (rest.head?.map (·.c)) = some 47 then some rest.tail
      else skipBlockComment fuel rest

/-- Skip a line comment body after `//`, up to (not including) the new-line. -/
def skipLineComment : Nat → List PChar → List PChar
  | 0, cs => cs
  | fuel + 1, cs =>
    match cs with
    | [] => []
    | p :: rest => if p.c = 10 then cs else skipLineComment fuel rest

/-- Where header-names may be recognised (§6.4p4): after `#` `include` (or
`include_next`) at the start of a line, and after `__has_include` `(` in a
`#if`/`#elif` line. -/
inductive HdrCtx
  | none | hash | expectHdr | ifLine | hasInc
  deriving DecidableEq, Repr

def isHashTok (k : PPKind) (s : Bytes) : Bool := k = .punct ∧ (s = bs "#" ∨ s = bs "%:")

/-- The header-name context after a token. -/
def nextHdrCtx (cfg : PreCfg) (ctx : HdrCtx) (bol : Bool) (k : PPKind) (s : Bytes) : HdrCtx :=
  if bol then (if isHashTok k s then .hash else .none)
  else match ctx with
    | .hash =>
      if k = .ident ∧ (s = bs "include" ∨ (cfg.ext.includeNext ∧ s = bs "include_next")) then .expectHdr
      else if k = .ident ∧ (s = bs "if" ∨ s = bs "elif") then .ifLine
      else .none
    | .ifLine =>
      if cfg.ext.hasInclude ∧ k = .ident ∧ (s = bs "__has_include" ∨ s = bs "__has_include_next")
      then .hasInc else .ifLine
    | .hasInc => if k = .punct ∧ s = bs "(" then .expectHdr else .ifLine
    | .expectHdr => .none
    | .none => .none

/-- The phase-3 loop. `ws`: white space precedes the next token; `bol`: the
next token starts a line. -/
def lexGo (cfg : PreCfg) (file : Bytes) :
    Nat → List PChar → Bool → Bool → HdrCtx → List LItem → Except PreError (List LItem)
  | 0, _, _, _, _, _ => .error ⟨.fuel, file, 0, "lexer fuel"⟩
  | fuel + 1, cs, ws, bol, ctx, acc =>
    match cs with
    | [] => .ok acc.reverse
    | p :: rest =>
      if p.c = 10 then lexGo cfg file fuel rest true true .none (.nl p.line :: acc)
      else if isSpace cfg p.c then lexGo cfg file fuel rest true bol ctx acc
      else if p.c = 47 ∧ (rest.head?.map (·.c)) = some 42 ∧ ctx ≠ .expectHdr then
        match skipBlockComment fuel rest.tail with
        | some r => lexGo cfg file fuel r true bol ctx acc
        | none => .error ⟨.lexical, file, p.line, "unterminated comment"⟩
      else if p.c = 47 ∧ (rest.head?.map (·.c)) = some 47 ∧ ctx ≠ .expectHdr then
        lexGo cfg file fuel (skipLineComment fuel rest) true bol ctx acc
      else
        let (k, s, r) := lexToken cfg (ctx = .expectHdr) (fuel + 1) cs
        let t : Tok := { kind := k, spell := s, ws := ws || bol, line := p.line, hs := [] }
        lexGo cfg file fuel r false false (nextHdrCtx cfg ctx bol k s) (.tok t :: acc)

/-- Translation phase 3 on the output of phase 2. -/
def phase3 (cfg : PreCfg) (file : Bytes) (cs : List PChar) : Except PreError (List LItem) :=
  lexGo cfg file (cs.length + 1) cs true true .none []

/-- Phases 1–3 of a source file. -/
def phases123 (cfg : PreCfg) (file : Bytes) (src : Bytes) : Except PreError (List LItem) :=
  phase3 cfg file (phases12 cfg src)

/-- Lex a byte string that must form exactly one preprocessing token
(the result of `##`, §6.10.3.3p3, or of `#`, §6.10.3.2p2). -/
def lexOne (cfg : PreCfg) (b : Bytes) : Option (PPKind × Bytes) :=
  match b with
  | [] => none
  | _ =>
    let cs := b.map (fun c => (⟨c, 0⟩ : PChar))
    match lexGo cfg [] (cs.length + 1) cs false false .none [] with
    | .ok [.tok t] => if t.spell = b then some (t.kind, t.spell) else none
    | _ => none

end CSem.Pre
