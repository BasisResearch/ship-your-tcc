import CSemCParser.Token

/-! # The C lexer: preprocessed bytes → tokens (port of csem/parsers/c/c_lexer.mll)

Translation phases 5–7 as Cerberus performs them: Cerberus runs an external
`cpp` and lexes its output (`backend/common/pipeline.ml` `cpp`, then
`C_parser_driver.parse_from_string`). This module is that lexer, byte for byte:

* the `initial` rule's longest-match discipline (ocamllex picks the longest
  match, ties to the earliest rule) is decided explicitly per first byte;
* `#` line markers (`hash` rule, gcc `-E` syntax `# N "file" ...`) feed the line
  map (`LineMark`), `#pragma` lines are skipped, any other `#` is an error;
* comments (`/* */` with the nested-`/*` error, `//`), and the magic-comment
  rules `/*@`, `/*$`, which act as comments because `at_magic_comments` is off;
* positions are ocamllex's: `lnum` is advanced only by `new_line` (at `\n` in
  `initial`, inside comments, after `//` comments and after line markers), so a
  line marker's file name that spans lines (the `[^ '\012' '\t' '"']*` class
  admits `\n`) advances one line, as in the OCaml.

Strings are byte carriers: one `Char` per source byte (`Char.ofNat b`), the
convention of `CabsImport` (header note Z2-J-02). Identifiers are ASCII (bytes
≥ 0x80 are `invalid symbol`, as in the OCaml; UCNs stay spelled `\uXXXX`).

The lexer is total: `lexLoop` consumes at least one byte per step and is run
with fuel `size + 1`.

**Composition point for F2.** The parser's input is the token array
(`Array RawTok` plus the `LineMark`s that give positions their source file and
line). F2's preprocessing spec (`CSem.Pre.phases14`) will produce
preprocessing tokens; its phase-7 conversion must produce this array. Until it
lands, `CSemCParser.frontEnd` composes this lexer with the preprocessed text the
external `cpp` writes, exactly as Cerberus does. -/

namespace CSemCParser

structure LexErr where
  pos : LPos
  msg : String
  deriving Inhabited

/-- Lexer configuration: `gnu` enables the keyword aliases of the GNU
extensions (csem/GRAMMAR.md); `false` is Cerberus's lexer exactly. -/
structure LexCfg where
  gnu : Bool := false
  deriving Inhabited

namespace Lex

/-- The byte at `i`, or 256 at end of input. -/
@[inline] def byteAt (s : ByteArray) (i : Nat) : Nat :=
  if h : i < s.size then (s[i]'h).toNat else 256

@[inline] def isDigit (c : Nat) : Bool := 48 ≤ c && c ≤ 57
@[inline] def isOct (c : Nat) : Bool := 48 ≤ c && c ≤ 55
@[inline] def isHex (c : Nat) : Bool := isDigit c || (65 ≤ c && c ≤ 70) || (97 ≤ c && c ≤ 102)
@[inline] def isNondigit (c : Nat) : Bool := c == 95 || (65 ≤ c && c ≤ 90) || (97 ≤ c && c ≤ 122)
@[inline] def ch (c : Char) : Nat := c.toNat

/-- Length of the run of bytes satisfying `p` from `i`. -/
def runLen (s : ByteArray) (p : Nat → Bool) (i : Nat) : Nat :=
  go (s.size + 1 - i) i 0
where
  go : Nat → Nat → Nat → Nat
  | 0, _, n => n
  | f+1, j, n => if p (byteAt s j) then go f (j+1) (n+1) else n

/-- The bytes `[i, j)` as a byte-carrier string. -/
def slice (s : ByteArray) (i j : Nat) : String :=
  String.ofList ((List.range (j - i)).map fun k => Char.ofNat (byteAt s (i + k)))

/-- `n` hex digits at `i`? -/
def hexN (s : ByteArray) (i n : Nat) : Bool :=
  (List.range n).all fun k => isHex (byteAt s (i + k))

/-- Length of a universal character name at `i` (`\u` hex_quad | `\U` hex_quad
hex_quad), 0 if none. -/
def ucnLen (s : ByteArray) (i : Nat) : Nat :=
  if byteAt s i != ch '\\' then 0
  else if byteAt s (i+1) == ch 'u' && hexN s (i+2) 4 then 6
  else if byteAt s (i+1) == ch 'U' && hexN s (i+2) 8 then 10
  else 0

/-- Length of the escape sequence at `i` (`escape_sequence`, the longest
match: up to three octal digits, every hex digit after `\x`), 0 if invalid. -/
def escapeLen (s : ByteArray) (i : Nat) : Nat :=
  if byteAt s i != ch '\\' then 0 else
  let c := byteAt s (i+1)
  if c == ch '\'' || c == ch '"' || c == ch '?' || c == ch '\\' || c == ch 'a' || c == ch 'b'
     || c == ch 'f' || c == ch 'n' || c == ch 'r' || c == ch 't' || c == ch 'v' then 2
  else if isOct c then
    if isOct (byteAt s (i+2)) then (if isOct (byteAt s (i+3)) then 4 else 3) else 2
  else if c == ch 'x' then
    let h := runLen s isHex (i+2)
    if h == 0 then 0 else 2 + h
  else ucnLen s i

/-- Identifier length at `i` (`identifier`), 0 if none. -/
def identLen (s : ByteArray) (i : Nat) : Nat :=
  let first := if isNondigit (byteAt s i) then 1 else ucnLen s i
  if first == 0 then 0 else go (s.size + 1) (i + first) first
where
  go : Nat → Nat → Nat → Nat
  | 0, _, n => n
  | f+1, j, n =>
    let c := byteAt s j
    if isNondigit c || isDigit c then go f (j+1) (n+1)
    else let u := ucnLen s j; if u == 0 then n else go f (j+u) (n+u)

/-- A character constant whose opening quote is at `q`: the c-char sequence and
the index after the closing quote (`"'" (c_char+ as str) "'"`). -/
def charConst (s : ByteArray) (q : Nat) : Option (String × Nat) :=
  go (s.size + 1) (q+1)
where
  go : Nat → Nat → Option (String × Nat)
  | 0, _ => none
  | f+1, j =>
    let c := byteAt s j
    if c == ch '\'' then (if j == q + 1 then none else some (slice s (q+1) j, j+1))
    else if c == ch '\\' then
      let e := escapeLen s j
      if e == 0 then none else go f (j + e)
    else if c == ch '\n' || c == 256 then none
    else go f (j+1)

/-- `s_char_sequence` after the opening quote at `q - 1`: the fragments (one
per s-char: an escape as its spelling, or one byte) and the index after the
closing quote; `none` is the OCaml's error (invalid string character, or the
ocamllex `empty token` failure at end of input). -/
def strChars (s : ByteArray) (q : Nat) : Option (List String × Nat) :=
  go (s.size + 1) q #[]
where
  go : Nat → Nat → Array String → Option (List String × Nat)
  | 0, _, _ => none
  | f+1, j, acc =>
    let c := byteAt s j
    if c == ch '"' then some (acc.toList, j+1)
    else if c == ch '\\' then
      let e := escapeLen s j
      if e == 0 then none else go f (j + e) (acc.push (slice s j (j + e)))
    else if c == ch '\n' || c == 256 then none
    else go f (j+1) (acc.push (slice s j (j+1)))

/-- Length of an exponent part at `k` (`[eE] sign? digit+`, or `[pP]` for
`binary`), 0 if none. -/
def expLen (s : ByteArray) (binary : Bool) (k : Nat) : Nat :=
  let c := byteAt s k
  let ok := if binary then c == ch 'p' || c == ch 'P' else c == ch 'e' || c == ch 'E'
  if !ok then 0 else
  let sg := if byteAt s (k+1) == ch '+' || byteAt s (k+1) == ch '-' then 1 else 0
  let d := runLen s isDigit (k + 1 + sg)
  if d == 0 then 0 else 1 + sg + d

/-- Longest `decimal_floating_constant` at `i` (0 if none). -/
def decFloatLen (s : ByteArray) (i : Nat) : Nat :=
  let d1 := runLen s isDigit i
  let j := i + d1
  if byteAt s j == ch '.' then
    let d2 := runLen s isDigit (j+1)
    if d1 == 0 && d2 == 0 then 0
    else let m := j + 1 + d2; m + expLen s false m - i
  else if d1 ≥ 1 then
    let e := expLen s false j
    if e == 0 then 0 else d1 + e
  else 0

/-- Longest `hexadecimal_floating_constant` at `i` (0 if none). -/
def hexFloatLen (s : ByteArray) (i : Nat) : Nat :=
  if !(byteAt s i == ch '0' && (byteAt s (i+1) == ch 'x' || byteAt s (i+1) == ch 'X')) then 0 else
  let k := i + 2
  let h1 := runLen s isHex k
  let j := k + h1
  let m? : Option Nat :=
    if byteAt s j == ch '.' then
      let h2 := runLen s isHex (j+1)
      if h1 + h2 == 0 then none else some (j + 1 + h2)
    else if h1 ≥ 1 then some j else none
  match m? with
  | none => 0
  | some m => let e := expLen s true m; if e == 0 then 0 else m + e - i

/-- Length of the `integer_constant` at `i` (0 if none): the longest of the
decimal, octal, hexadecimal and binary forms. -/
def intLen (s : ByteArray) (i : Nat) : Nat :=
  let c := byteAt s i
  if 49 ≤ c && c ≤ 57 then runLen s isDigit i
  else if c == ch '0' then
    let c1 := byteAt s (i+1)
    if (c1 == ch 'x' || c1 == ch 'X') && isHex (byteAt s (i+2)) then 2 + runLen s isHex (i+2)
    else if (c1 == ch 'b' || c1 == ch 'B') && (byteAt s (i+2) == ch '0' || byteAt s (i+2) == ch '1') then
      2 + runLen s (fun c => c == ch '0' || c == ch '1') (i+2)
    else 1 + runLen s isOct (i+1)
  else 0

/-- `cn_integer_width` at `k`: its length (8, 16, 32, 64, 128), 0 if none. -/
def cnWidthLen (s : ByteArray) (k : Nat) : Nat :=
  let d k' := byteAt s k'
  if d k == ch '1' && d (k+1) == ch '2' && d (k+2) == ch '8' then 3
  else if (d k == ch '1' && d (k+1) == ch '6') || (d k == ch '3' && d (k+1) == ch '2')
       || (d k == ch '6' && d (k+1) == ch '4') then 2
  else if d k == ch '8' then 1 else 0

/-- The integer rules of `initial` at `i` (integer constant plus suffix): the
token and the total length. -/
def intToken (s : ByteArray) (i : Nat) : Option (Tok × Nat) :=
  let n := intLen s i
  if n == 0 then none else
  let str := slice s i (i + n)
  let j := i + n
  let c := byteAt s j
  let isU c := c == ch 'u' || c == ch 'U'
  let isL c := c == ch 'l' || c == ch 'L'
  let ll k := (byteAt s k == ch 'l' && byteAt s (k+1) == ch 'l') || (byteAt s k == ch 'L' && byteAt s (k+1) == ch 'L')
  let int sfx len := some (Tok.CONSTANT (.CabsInteger_const (str, sfx)), n + len)
  if isU c then
    if ll (j+1) then int (some .CabsSuffix_ULL) 3
    else if isL (byteAt s (j+1)) then int (some .CabsSuffix_UL) 2
    else if c == ch 'u' && cnWidthLen s (j+1) > 0 then some (.CN_CONSTANT, n + 1 + cnWidthLen s (j+1))
    else int (some .CabsSuffix_U) 1
  else if ll j then
    if isU (byteAt s (j+2)) then int (some .CabsSuffix_ULL) 3 else int (some .CabsSuffix_LL) 2
  else if isL c then
    if isU (byteAt s (j+1)) then int (some .CabsSuffix_UL) 2 else int (some .CabsSuffix_L) 1
  else if c == ch 'i' && cnWidthLen s (j+1) > 0 then some (.CN_CONSTANT, n + 1 + cnWidthLen s (j+1))
  else int none 0

/-- The floating rules of `initial` at `i`. -/
def floatToken (s : ByteArray) (i : Nat) : Option (Tok × Nat) :=
  let n := max (decFloatLen s i) (hexFloatLen s i)
  if n == 0 then none else
  let str := slice s i (i + n)
  let c := byteAt s (i + n)
  if c == ch 'f' || c == ch 'F' then some (.CONSTANT (.CabsFloating_const (str, some .CabsFloatingSuffix_F)), n+1)
  else if c == ch 'l' || c == ch 'L' then some (.CONSTANT (.CabsFloating_const (str, some .CabsFloatingSuffix_L)), n+1)
  else some (.CONSTANT (.CabsFloating_const (str, none)), n)

/-- A number at `i`: the longer of the integer and floating matches (ties to
the integer rules, which come first). -/
def numberToken (s : ByteArray) (i : Nat) : Option (Tok × Nat) :=
  match intToken s i, floatToken s i with
  | some (t, n), some (t', n') => if n' > n then some (t', n') else some (t, n)
  | some r, none => some r
  | none, r => r

/-- The keyword table (c_lexer.mll `keywords`), and the GNU aliases of the
extensions (csem/GRAMMAR.md) when `gnu`. -/
def keyword (gnu : Bool) (id : String) : Option Tok :=
  match id with
  | "auto" => some .AUTO | "break" => some .BREAK | "case" => some .CASE | "char" => some .CHAR
  | "const" => some .CONST | "continue" => some .CONTINUE | "default" => some .DEFAULT
  | "do" => some .DO | "double" => some .DOUBLE | "else" => some .ELSE | "enum" => some .ENUM
  | "extern" => some .EXTERN | "float" => some .FLOAT | "for" => some .FOR | "goto" => some .GOTO
  | "if" => some .IF | "inline" => some .INLINE | "int" => some .INT | "long" => some .LONG
  | "register" => some .REGISTER | "restrict" => some .RESTRICT | "return" => some .RETURN
  | "short" => some .SHORT | "signed" => some .SIGNED | "sizeof" => some .SIZEOF
  | "static" => some .STATIC | "struct" => some .STRUCT | "switch" => some .SWITCH
  | "typedef" => some .TYPEDEF | "typeof" => some .TYPEOF | "union" => some .UNION
  | "unsigned" => some .UNSIGNED | "void" => some .VOID | "volatile" => some .VOLATILE
  | "while" => some .WHILE | "_Alignas" => some .ALIGNAS | "_Alignof" => some .ALIGNOF
  | "_Atomic" => some .ATOMIC | "_Bool" => some .BOOL | "_Complex" => some .COMPLEX
  | "_Generic" => some .GENERIC | "_Noreturn" => some .NORETURN
  | "_Static_assert" => some .STATIC_ASSERT | "_Thread_local" => some .THREAD_LOCAL
  | "assert" => some .ASSERT | "offsetof" => some .OFFSETOF
  | "__cerb_va_start" => some .VA_START | "__cerb_va_copy" => some .VA_COPY
  | "__cerb_va_arg" => some .VA_ARG | "__cerb_va_end" => some .VA_END
  | "__cerb_printtype" => some .PRINT_TYPE | "__BMC_ASSUME" => some .BMC_ASSUME
  | "asm" => some .ASM | "__asm__" => some .ASM | "__volatile__" => some .ASM_VOLATILE
  | "__builtin_types_compatible_p" => some .BUILTIN_TYPES_COMPATIBLE_P
  | "__builtin_choose_expr" => some .BUILTIN_CHOOSE_EXPR
  | _ => if gnu then gnuKeyword id else none
where
  /-- GNU spellings tcc's own lexer knows as keywords (tinycc tcctok.h), mapped
  to the token of the standard keyword they alias (csem/GRAMMAR.md §1). -/
  gnuKeyword : String → Option Tok
  | "__attribute__" | "__attribute" => some .ATTRIBUTE
  | "__typeof__" | "__typeof" => some .TYPEOF
  | "__inline__" | "__inline" => some .INLINE
  | "__restrict__" | "__restrict" => some .RESTRICT
  | "__signed__" | "__signed" => some .SIGNED
  | "__const__" | "__const" => some .CONST
  | "__volatile" => some .VOLATILE
  | "__alignof__" | "__alignof" => some .ALIGNOF
  | "__asm" => some .ASM
  | "__extension__" => some .EXTENSION
  | "__label__" => some .LABEL
  | "__thread" => some .THREAD_LOCAL
  | "__builtin_va_start" => some .VA_START
  | "__builtin_va_copy" | "__va_copy" => some .VA_COPY
  | "__builtin_va_arg" => some .VA_ARG
  | "__builtin_va_end" => some .VA_END
  | "__builtin_va_list" => some .BUILTIN_VA_LIST
  | _ => none

/-- The lexer state between tokens. -/
structure St where
  i : Nat
  lnum : Nat
  bol : Nat
  toks : Array RawTok
  marks : Array LineMark

@[inline] def St.pos (st : St) : LPos := ⟨st.lnum, st.bol, st.i⟩

/-- Emit `t` spanning `[st.i, st.i + n)` (no newline inside a token). -/
@[inline] def St.emit (st : St) (t : Tok) (n : Nat) : St :=
  { st with i := st.i + n, toks := st.toks.push ⟨t, st.pos, ⟨st.lnum, st.bol, st.i + n⟩⟩ }

/-- Decimal value of the digits `[i, j)` (the line number of a marker). -/
def decValue (s : ByteArray) (i j : Nat) : Nat :=
  (List.range (j - i)).foldl (fun acc k => acc * 10 + (byteAt s (i + k) - 48)) 0

/-- Index of the first `\n` at or after `j`, or `s.size`. -/
def lineEnd (s : ByteArray) (j : Nat) : Nat := j + runLen s (fun c => c != ch '\n' && c != 256) j

/-- The `hash` rule after a `#` at `st.i - 1`... called with `j` the byte after
the `#`. -/
def hash (s : ByteArray) (st : St) (j : Nat) : Except LexErr St :=
  -- rule A: ' ' ('0' | decimal_constant) " \"" file "\"" [^\n]* ('\n' | eof)
  let lineMarker : Option (Nat × String × Nat) := Id.run do
    if byteAt s j != ch ' ' then return none
    let k := j + 1
    let nlen := if byteAt s k == ch '0' then 1
                else if 49 ≤ byteAt s k && byteAt s k ≤ 57 then runLen s isDigit k else 0
    if nlen == 0 then return none
    let k' := k + nlen
    if !(byteAt s k' == ch ' ' && byteAt s (k'+1) == ch '"') then return none
    let f0 := k' + 2
    let flen := runLen s (fun c => c != 12 && c != ch '\t' && c != ch '"' && c != 256) f0
    if byteAt s (f0 + flen) != ch '"' then return none
    let e := lineEnd s (f0 + flen + 1)
    let after := if byteAt s e == ch '\n' then e + 1 else e
    let file := match String.fromUTF8? (s.extract f0 (f0 + flen)) with
      | some t => t
      | none => slice s f0 (f0 + flen)
    return some (decValue s k k', file, after)
  match lineMarker with
  | some (n, file, after) =>
    if n ≥ 4611686018427387904 then .error ⟨st.pos, "invalid line number"⟩ else
    let marks := if n > 0 then st.marks.push ⟨st.lnum, file, n⟩ else st.marks
    .ok { st with i := after, lnum := st.lnum + 1, bol := after, marks }
  | none =>
    let isPragma := (List.range 6).all fun k => byteAt s (j + k) == ("pragma".toList.getD k ' ').toNat
    let e := lineEnd s j
    if isPragma && byteAt s e == ch '\n' then
      .ok { st with i := e + 1, lnum := st.lnum + 1, bol := e + 1 }
    else if byteAt s e != ch '\n' then .error ⟨st.pos, "unexpected end of file"⟩
    else .error ⟨st.pos, "invalid symbol"⟩

/-- A `/* ... */` comment whose body starts at `j`. -/
def comment (s : ByteArray) (st : St) (j : Nat) : Except LexErr St :=
  go (s.size + 1) j st.lnum st.bol
where
  go : Nat → Nat → Nat → Nat → Except LexErr St
  | 0, _, _, _ => .error ⟨st.pos, "unterminated comment"⟩
  | f+1, k, lnum, bol =>
    let c := byteAt s k
    if c == 256 then .error ⟨st.pos, "unterminated comment"⟩
    else if c == ch '*' && byteAt s (k+1) == ch '/' then .ok { st with i := k + 2, lnum, bol }
    else if c == ch '/' && byteAt s (k+1) == ch '*' then .error ⟨⟨lnum, bol, k⟩, "nested comment"⟩
    else if c == ch '\n' then go f (k+1) (lnum+1) (k+1)
    else go f (k+1) lnum bol

/-- One step of `initial`: consume at least one byte (`st.i < s.size`). -/
def step (cfg : LexCfg) (s : ByteArray) (st : St) : Except LexErr St :=
  let i := st.i
  let c := byteAt s i
  let c1 := byteAt s (i+1)
  let c2 := byteAt s (i+2)
  let tok (t : Tok) (n : Nat) : Except LexErr St := .ok (st.emit t n)
  let err (m : String) : Except LexErr St := .error ⟨st.pos, m⟩
  let ident : Except LexErr St :=
    let n := identLen s i
    if n == 0 then err "invalid symbol" else
    let id := slice s i (i + n)
    tok ((keyword cfg.gnu id).getD (.NAME id)) n
  let charTok (pref : Option cabs_character_prefix) (q : Nat) : Option (Except LexErr St) :=
    (charConst s q).map fun (str, e) => tok (.CONSTANT (.CabsCharacter_const (pref, str))) (e - i)
  let strTok (pref : Option cabs_encoding_prefix) (q : Nat) : Except LexErr St :=
    match strChars s q with
    | some (strs, e) => tok (.STRING_LITERAL pref strs) (e - i)
    | none => err "invalid string character"
  if c == ch '\n' then .ok { st with i := i + 1, lnum := st.lnum + 1, bol := i + 1 }
  else if c == ch ' ' || c == ch '\t' || c == 12 || c == ch '\r' then .ok { st with i := i + 1 }
  else if c == ch '/' then
    if c1 == ch '*' then comment s st (i + 2)
    else if c1 == ch '/' then
      let e := lineEnd s (i + 2)
      let after := if byteAt s e == ch '\n' then e + 1 else e
      .ok { st with i := after, lnum := st.lnum + 1, bol := after }
    else if c1 == ch '=' then tok .SLASH_EQ 2 else tok .SLASH 1
  else if c == ch '#' then hash s st (i + 1)
  else if isDigit c || (c == ch '.' && isDigit c1) then
    match numberToken s i with
    | some (t, n) => tok t n
    | none => err "invalid symbol"
  else if c == ch '\'' then (charTok none i).getD (err "invalid symbol")
  else if c == ch '"' then strTok none (i + 1)
  else if (c == ch 'L' || c == ch 'u' || c == ch 'U') && c1 == ch '\'' then
    let pref := if c == ch 'L' then .CabsPrefix_L else if c == ch 'u' then .CabsPrefix_u else .CabsPrefix_U
    (charTok (some pref) (i + 1)).getD ident
  else if c == ch 'u' && c1 == ch '8' && c2 == ch '"' then strTok (some .CabsEncPrefix_u8) (i + 3)
  else if (c == ch 'L' || c == ch 'u' || c == ch 'U') && c1 == ch '"' then
    strTok (some (if c == ch 'L' then .CabsEncPrefix_L else if c == ch 'u' then .CabsEncPrefix_u
                  else .CabsEncPrefix_U)) (i + 2)
  else if c == ch '[' then
    let w := runLen s (fun c => c == ch ' ' || c == ch '\t' || c == 12 || c == ch '\r') (i + 1)
    if byteAt s (i + 1 + w) == ch '[' then tok .LBRACK_LBRACK (w + 2) else tok .LBRACK 1
  else if c == ch ']' then tok .RBRACK 1
  else if c == ch '(' then tok .LPAREN 1
  else if c == ch ')' then tok .RPAREN 1
  else if c == ch '{' then (if c1 == ch '-' && c2 == ch '{' then tok .LBRACES 3 else tok .LBRACE 1)
  else if c == ch '}' then (if c1 == ch '-' && c2 == ch '}' then tok .RBRACES 3 else tok .RBRACE 1)
  else if c == ch '.' then (if c1 == ch '.' && c2 == ch '.' then tok .ELLIPSIS 3 else tok .DOT 1)
  else if c == ch '-' then
    if c1 == ch '>' then tok .MINUS_GT 2 else if c1 == ch '-' then tok .MINUS_MINUS 2
    else if c1 == ch '=' then tok .MINUS_EQ 2 else tok .MINUS 1
  else if c == ch '+' then
    if c1 == ch '+' then tok .PLUS_PLUS 2 else if c1 == ch '=' then tok .PLUS_EQ 2 else tok .PLUS 1
  else if c == ch '&' then
    if c1 == ch '&' then tok .AMPERSAND_AMPERSAND 2 else if c1 == ch '=' then tok .AMPERSAND_EQ 2
    else tok .AMPERSAND 1
  else if c == ch '*' then (if c1 == ch '=' then tok .STAR_EQ 2 else tok .STAR 1)
  else if c == ch '~' then tok .TILDE 1
  else if c == ch '!' then (if c1 == ch '=' then tok .BANG_EQ 2 else tok .BANG 1)
  else if c == ch '%' then
    if c1 == ch '=' then tok .PERCENT_EQ 2 else if c1 == ch '>' then tok .RBRACE 2 else tok .PERCENT 1
  else if c == ch '<' then
    if c1 == ch '<' then (if c2 == ch '=' then tok .LT_LT_EQ 3 else tok .LT_LT 2)
    else if c1 == ch '=' then tok .LT_EQ 2 else if c1 == ch ':' then tok .LBRACK 2
    else if c1 == ch '%' then tok .LBRACE 2 else tok .LT 1
  else if c == ch '>' then
    if c1 == ch '>' then (if c2 == ch '=' then tok .GT_GT_EQ 3 else tok .GT_GT 2)
    else if c1 == ch '=' then tok .GT_EQ 2 else tok .GT 1
  else if c == ch '=' then (if c1 == ch '=' then tok .EQ_EQ 2 else tok .EQ 1)
  else if c == ch '^' then (if c1 == ch '=' then tok .CARET_EQ 2 else tok .CARET 1)
  else if c == ch '|' then
    if c1 == ch '|' then (if c2 == ch '|' then tok .PIPES 3 else tok .PIPE_PIPE 2)
    else if c1 == ch '=' then tok .PIPE_EQ 2 else tok .PIPE 1
  else if c == ch '?' then (if c1 == ch ':' then tok .QUESTION_COLON 2 else tok .QUESTION 1)
  else if c == ch ':' then
    if c1 == ch ':' then tok .COLON_COLON 2 else if c1 == ch '>' then tok .RBRACK 2 else tok .COLON 1
  else if c == ch ';' then tok .SEMICOLON 1
  else if c == ch ',' then tok .COMMA 1
  else ident

/-- The lexing loop; `fuel ≥ remaining bytes + 1` suffices since every step
consumes a byte. -/
def lexLoop (cfg : LexCfg) (s : ByteArray) : Nat → St → Except LexErr St
  | 0, st => .error ⟨st.pos, "lexer fuel exhausted"⟩
  | f+1, st =>
    if st.i ≥ s.size then .ok { st with toks := st.toks.push ⟨.EOF, st.pos, st.pos⟩ }
    else
      -- `step`'s token alternatives capture its state. Keep the accumulated
      -- token array outside those closures so an emitted token never copies
      -- the entire prefix (notably on large macro expansions).
      let pos := st.pos
      let toks := st.toks
      match step cfg s { st with toks := #[] } with
      | .error e => .error e
      | .ok st' =>
        if st'.i ≤ pos.cnum then .error ⟨pos, "lexer made no progress"⟩
        else
          let toks := st'.toks.foldl (fun acc t => acc.push t) toks
          lexLoop cfg s f { st' with toks }

end Lex

/-- Lex a preprocessed translation unit: its tokens (ending in `EOF`) and line
markers. -/
def lex (cfg : LexCfg) (src : ByteArray) : Except LexErr (Array RawTok × Array LineMark) := do
  let st ← Lex.lexLoop cfg src (src.size + 1) ⟨0, 1, 0, #[], #[]⟩
  return (st.toks, st.marks)

end CSemCParser
