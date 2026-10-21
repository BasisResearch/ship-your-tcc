/-
CSem.Pre — an executable specification of ISO C11 translation phases 1–4
(§5.1.1.2) and of the preprocessing directives (§6.10).

This file: the vocabulary shared by all phases. Bytes, preprocessing-token
kinds (§6.4p1), the token the specification outputs (`PPToken`), errors,
the read-only file system the `#include` search runs over, and `PreCfg`,
the implementation's choices (what ISO C leaves implementation-defined plus
the extensions the implementation accepts).

The specification is written from the standard. Where a clause is
implementation-defined, the choice is a `PreCfg` field; `Spec/Pre/Tcc/Cfg.lean`
sets them to tcc's choices.
-/
import Std.Data.TreeMap

namespace CSem.Pre

/-- Source text and token spellings are byte strings: tcc reads the source
file byte by byte (UTF-8 or ASCII alike, no decoding), so the specification's
source character set (§5.1.1.2p1, phase 1) is the set of bytes. -/
abbrev Bytes := List UInt8

/-- A byte string from an ASCII string literal (spec-level constants). -/
def bs (s : String) : Bytes := s.toUTF8.toList

/-- The categories of preprocessing tokens, §6.4p1:
header-name, identifier, pp-number, character-constant, string-literal,
punctuator, and "each non-white-space character that cannot be one of the
above" (`other`). Two more kinds occur only in the output of phase 4:
`pragmaBegin`/`pragmaEnd` bracket the preprocessing tokens of a `#pragma`
directive (or `_Pragma` operator, §6.10.9) whose effect is left to later
phases (`#pragma pack`, `#pragma STDC …`). -/
inductive PPKind
  | headerName | ident | ppNumber | charConst | strLit | punct | other
  | pragmaBegin | pragmaEnd
  deriving DecidableEq, Repr, Inhabited

/-- A preprocessing token as phase 4 delivers it to phases 5–7: its kind and
its spelling (the bytes it was written with; a digraph keeps its digraph
spelling, §6.4.6p3). White space between tokens is not part of the output:
after phase 4 it only separates tokens (§5.1.1.2p1, phase 7). -/
structure PPToken where
  kind : PPKind
  spell : Bytes
  deriving DecidableEq, Repr, Inhabited

/-- What went wrong. The specification stops at the first error: a violated
constraint of §6.10 (a diagnostic is required, §5.1.1.3), undefined
behaviour the specification chooses to reject (each site names its
clause), `#error` (§6.10.5), a missing header (§6.10.2p2–3), an
implementation limit (§5.2.4.1) or fuel exhaustion. -/
inductive ErrKind
  | lexical       -- phases 1–3: unterminated comment, …
  | directive     -- a malformed directive (§6.10 constraints)
  | include       -- §6.10.2: header not found, bad header name
  | macro         -- §6.10.3: macro definition or invocation
  | ifExpr        -- §6.10.1: controlling expression
  | errorDirective -- §6.10.5 `#error`
  | limit         -- an implementation limit (§5.2.4.1): include or #if nesting
  | fuel          -- the fuel bound (see `Spec/Pre/Main.lean`) ran out
  deriving DecidableEq, Repr, Inhabited

/-- An error: kind, presumed file name and physical line, message. -/
structure PreError where
  kind : ErrKind
  file : Bytes
  line : Nat
  msg : String
  deriving Repr, Inhabited

/-- The read-only file system the header search runs over (§6.10.2:
"a sequence of implementation-defined places"). Paths are absolute
byte strings; `lookup` resolves `.`, `..` and repeated `/` as the image's
file system does (c/src/htif.c `resolve`: the working directory is the root
and the root's parent is the root). -/
structure FileSys where
  files : List (Bytes × Bytes)
  deriving Inhabited

namespace FileSys

/-- Split a path at `/`, dropping empty components and `.`, resolving `..`
(the root's parent is the root). -/
def normComponents (p : Bytes) : List Bytes :=
  let comps := (p.splitOn 47).filter (fun c => c ≠ [] ∧ c ≠ [46])
  (comps.foldl (fun acc c => if c = [46, 46] then acc.drop 1 else c :: acc) []).reverse

/-- The contents of the file at `p`, if any. -/
def lookup (fs : FileSys) (p : Bytes) : Option Bytes :=
  let k := normComponents p
  (fs.files.find? (fun e => normComponents e.1 = k)).map (·.2)

/-- Total size in bytes (part of the fuel bound). -/
def size (fs : FileSys) : Nat := fs.files.foldl (fun n e => n + e.2.length) 0

end FileSys

/-- Extensions beyond ISO C11 that the implementation accepts. Each is
either in the space the standard leaves to the implementation (a
non-directive, a `#pragma`, an implementation-reserved identifier such as
`__COUNTER__`) or is a construct that violates a constraint and that the
implementation accepts with (or without) a diagnostic. With every flag false
the specification is plain ISO C11. -/
structure Ext where
  /-- `#include_next` (GNU): continue the header search after the place the
  current file was found. -/
  includeNext : Bool
  /-- `#pragma once`: a file is not included again (the same file: its path
  after resolving `.`, `..` and repeated `/`). -/
  pragmaOnce : Bool
  /-- `#pragma push_macro("X")` / `#pragma pop_macro("X")`. -/
  pushPopMacro : Bool
  /-- `__COUNTER__`: expands to 0, 1, 2, … on successive expansions. -/
  counter : Bool
  /-- `__has_include(…)` and `__has_include_next(…)` in `#if`; both names
  count as defined for `defined`/`#ifdef`. -/
  hasInclude : Bool
  /-- `#warning …`: no effect on the token sequence. -/
  warning : Bool
  /-- `# digit-sequence "name" …` (the GNU line marker) means `#line`; extra
  tokens are ignored. -/
  linemarker : Bool
  /-- `#!…` as the first line of the main file is ignored. -/
  hashBang : Bool
  /-- GNU named variadic parameter `name...` (§6.10.3 has only `...`). -/
  namedVariadic : Bool
  /-- GNU `, ## __VA_ARGS__`: the comma is deleted when the variable
  argument has no tokens; otherwise the `##` is deleted (and the variable
  argument is macro-replaced as an ordinary parameter occurrence). -/
  commaPaste : Bool
  /-- A variadic macro invoked with no variable argument at all
  (`F(a)` for `F(x, ...)`, a constraint violation in C11 §6.10.3p4): the
  variable argument is empty. -/
  emptyVariadic : Bool
  /-- `0b…` binary integer constants in `#if` (GNU). -/
  binaryLiterals : Bool
  /-- A redefinition that is not identical to the current definition
  (a constraint violation, §6.10.3p2) replaces it, after a diagnostic
  (tcc, gcc). newlib's own headers need it: `<limits.h>` included first
  defines `ARG_MAX` twice differently (results/pre/f2-diff.md). -/
  redefine : Bool
  deriving Repr, Inhabited

/-- The implementation's choices for phases 1–4.

`Spec/Pre/Tcc/Cfg.lean` (generated from tcc's sources) sets them for tcc. -/
structure PreCfg where
  /-- Phase 1 (§5.1.1.2p1): replace the nine trigraph sequences (§5.2.1.1).
  ISO C11 requires it; tcc does not implement trigraphs (`cfgTcc`: false). -/
  trigraphs : Bool
  /-- The `u8`, `u` and `U` prefixes of string literals and character
  constants (§6.4.5, §6.4.4.4), new in C11. tcc's default mode is C99
  (`__STDC_VERSION__` 199901L), whose grammar has only `L`: there `u8"a"` is
  the identifier `u8` followed by a string literal (`cfgTcc`: false). -/
  c11Prefixes : Bool
  /-- §6.4.2.1p1: "other implementation-defined characters" allowed in
  identifiers (tcc: `$` and every byte ≥ 0x80). -/
  extraIdChar : UInt8 → Bool
  /-- Bytes treated as white space besides space, HT, VT, FF and new-line
  (tcc: CR). Phase 1 maps the end-of-line indicator CR LF to new-line first. -/
  extraSpace : UInt8 → Bool
  /-- The text processed (as a source file named `predefsName`) before the
  main file: the definitions of the predefined macros of §6.10.8 that are
  not dynamic (`__STDC__`, `__STDC_HOSTED__`, `__STDC_VERSION__`, …) and of
  the implementation's own predefined macros. Its tokens, if any, precede
  the main file's in the output (tcc's `<command line>` buffer, which ends
  with `#include <tccdefs.h>`). -/
  predefs : Bytes
  predefsName : Bytes
  /-- The presumed name of the main source file (`__FILE__`, §6.10.8.1). -/
  mainName : Bytes
  /-- §6.10.2: places searched for both forms (`-I`), then `sysIncludePaths`;
  the `"…"` form first searches the directory of the including file. -/
  includePaths : List Bytes
  sysIncludePaths : List Bytes
  /-- §6.10.2p4: when a macro-replaced `#include` forms `<…>` from several
  tokens, whether white space between them becomes one space (gcc) or
  nothing (tcc). -/
  hdrJoinSpaces : Bool
  /-- §6.10.8.1: the spellings (without quotes) of `__DATE__` ("Mmm dd yyyy")
  and `__TIME__` ("hh:mm:ss"). -/
  date : Bytes
  time : Bytes
  /-- §5.2.4.1 limits: nesting of included files (counting the main file
  and the predefined text) and of conditional inclusion. -/
  maxIncludeDepth : Nat
  maxIfDepth : Nat
  /-- §6.10.1p4: a character constant in `#if` has the value it has in an
  expression (implementation-defined); `charSigned` is whether plain `char`
  is signed (tcc RV64: unsigned). -/
  charSigned : Bool
  /-- §6.10.6p1: whether the tokens of a non-`STDC` pragma after its first
  token are macro-replaced (tcc: yes). -/
  pragmaExpand : Bool
  ext : Ext

end CSem.Pre
