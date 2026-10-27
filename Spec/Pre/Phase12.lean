/-
Translation phases 1 and 2 (C11 §5.1.1.2p1).

Phase 1: "Physical source file multibyte characters are mapped, in an
implementation-defined manner, to the source character set (introducing
new-line characters for end-of-line indicators) if necessary. Trigraph
sequences are replaced by corresponding single-character internal
representations."
* The source character set is the set of bytes (`Bytes`): no decoding.
* The end-of-line indicator CR LF becomes new-line; a lone CR stays (phase 3
  may treat it as white space, `PreCfg.extraSpace`).
* Trigraphs (§5.2.1.1) only when `cfg.trigraphs` (required for C11).
  This pass runs exactly once per physical file, including headers, before
  splicing, comment recognition and literals; macro results never reenter it.

Phase 2: "Each instance of a backslash character (\) immediately followed by
a new-line character is deleted, splicing physical source lines to form
logical source lines. … A source file that is not empty shall end in a
new-line character, which shall not be immediately preceded by a backslash
character before any such splicing takes place." The last sentence makes a
missing final new-line undefined behaviour; the specification supplies one
(as tcc and gcc do).

Each character of the result remembers the physical line it came from
(1-based): `__LINE__` (§6.10.8.1) and `#line` (§6.10.4) are about physical
lines.
-/
import Spec.Pre.Types

namespace CSem.Pre

/-- A character after phase 2, with the physical line it is on. -/
structure PChar where
  c : UInt8
  line : Nat
  deriving Repr, Inhabited, DecidableEq

/-- §5.2.1.1: the character a trigraph `??x` stands for. -/
def trigraphOf (x : UInt8) : Option UInt8 :=
  if x = 61 then some 35        -- ??=  #
  else if x = 40 then some 91   -- ??(  [
  else if x = 47 then some 92   -- ??/  \
  else if x = 41 then some 93   -- ??)  ]
  else if x = 39 then some 94   -- ??'  ^
  else if x = 60 then some 123  -- ??<  {
  else if x = 33 then some 124  -- ??!  |
  else if x = 62 then some 125  -- ??>  }
  else if x = 45 then some 126  -- ??-  ~
  else none

/-- Phase 1 on a byte list (accumulator reversed). -/
def phase1Go (tri : Bool) : Bytes → Bytes → Bytes
  | [], acc => acc.reverse
  | [c], acc => (c :: acc).reverse
  | c :: d :: rest, acc =>
    if c = 13 ∧ d = 10 then phase1Go tri rest (10 :: acc)
    else if tri ∧ c = 63 ∧ d = 63 then
      match rest with
      | x :: rest' =>
        match trigraphOf x with
        | some r => phase1Go tri rest' (r :: acc)
        | none => phase1Go tri (d :: x :: rest') (c :: acc)
      | [] => phase1Go tri [d] (c :: acc)
    else phase1Go tri (d :: rest) (c :: acc)
termination_by l => l.length

/-- Translation phase 1. -/
def phase1 (cfg : PreCfg) (src : Bytes) : Bytes := phase1Go cfg.trigraphs src []

/-- Phase 2 on a byte list: delete backslash–new-line, number the lines. -/
def phase2Go : Bytes → Nat → List PChar → List PChar
  | [], _, acc => acc.reverse
  | [c], ln, acc => (⟨c, ln⟩ :: acc).reverse
  | c :: d :: rest, ln, acc =>
    if c = 92 ∧ d = 10 then phase2Go rest (ln + 1) acc
    else if c = 10 then phase2Go (d :: rest) (ln + 1) (⟨10, ln⟩ :: acc)
    else phase2Go (d :: rest) ln (⟨c, ln⟩ :: acc)
termination_by l => l.length

/-- Translation phase 2; a non-empty file gets a final new-line if it lacks
one. -/
def phase2 (b : Bytes) : List PChar :=
  let cs := phase2Go b 1 []
  match cs.getLast? with
  | none => []
  | some l => if l.c = 10 then cs else cs ++ [⟨10, l.line⟩]

/-- Phases 1 and 2. -/
def phases12 (cfg : PreCfg) (src : Bytes) : List PChar := phase2 (phase1 cfg src)

end CSem.Pre
