import CSemCParser.Lexer
import Std.Data.HashSet

/-! # The C grammar: tokens → Cabs (port of csem/parsers/c/c_parser.mly)

A recursive-descent / precedence-climbing parser for the language of
Cerberus's menhir grammar (Jourdan & Pottier's C11 grammar plus Cerberus's
extensions), building the same Cabs with the same locations. The menhir rules
are cited by their nonterminal names; every action is transcribed, including
its list-order quirks (marked QUIRK).

## How the LR grammar's decisions are reproduced

* **Typedef-name feedback** (`lexer_feedback.ml`). menhir's lexer emits, after
  each `NAME`, a `TYPE` or `VARIABLE` token computed from the context *when the
  parser demands it*, which is right after the `NAME` is shifted. All context
  changes are actions of reductions that happen before that shift
  (`declarator_varname`/`declarator_typedefname` after a complete declarator,
  `enumeration_constant`, `save_context`/`scoped` restores, the function-body
  context of `function_definition1`/`function_definition`). This parser
  performs each of those actions at the point the corresponding construct is
  complete and classifies a `NAME` (`isTypedefName`) only when it is about to
  decide on it, so the classification sees the same context. The one place a
  `NAME` is classified before the construct preceding it is reduced is a
  declarator followed by a `NAME` at file scope: menhir reduces
  `function_definition1` (installing the parameter context) before shifting the
  `NAME`, and so does `pExternalDecl` (it commits to a function definition on a
  `NAME`, then classifies it inside the K&R declaration list).
* **Declaration specifiers** (`list_eq1`, `list_ge1`, `list_eq1_eq1`,
  `list_eq1_ge1`, `list_tuple3_*`): exactly one "unique" type specifier
  (`void`, `_Bool`, struct/union/enum, a typedef name, `typeof`,
  `_Atomic(...)`) or at least one "nonunique" one (`char`, `int`, ...), never
  both; at most one `typedef`. `pSpecifiers` tracks this state; in particular a
  typedef name is a type specifier only while no type specifier has been seen,
  otherwise it ends the specifiers and starts the declarator (`T T;`,
  `unsigned T;`), and `_Atomic (` is the atomic type specifier only in that same
  state (C11 6.7.2.4p4; after a type specifier it is the qualifier).
* **The reduce/reduce conflict** (state 365 of `menhir --explain`: after
  `( T` with `T` a typedef name, in a parameter declaration, on `)`, `(`, `[`,
  `[[`), resolved by menhir in favour of `typedef_name_spec` (declared first):
  in a context where both a declarator and an abstract declarator are allowed,
  `(` followed by a typedef name always opens a parameter list (C11
  6.7.6.3p11). `pDeclGen` with mode `either` does that.
* **The shift/reduce conflict** (state 109: consecutive string literals inside
  attribute arguments), resolved by shifting: `pStringLiteral` is greedy.
* **Dangling `else`** (`%prec THEN`/`ELSE`): shift; the `else` binds to the
  nearest `if`.
* **Declaration vs expression**: decided by the first token (after any
  attribute sequence) and, for a `NAME`, its classification; a `NAME` followed
  by `:` is a label whatever its class (`general_identifier`).
* **Casts, compound literals, statement expressions, `sizeof`**: after `(`, a
  token that starts a type name selects `type_name`; `{` selects a statement
  expression; else a parenthesised expression. After `( type_name )`, `{`
  selects a compound literal (also after `sizeof`, where no conflict exists:
  `sizeof (T){...}` is the size of a compound literal).
* **Assignment**: the left operand must be a `unary_expression`; the
  expression parsers return a flag saying whether the expression is one
  (no cast, binary or conditional operator at its top).
* **Locations**: `$startpos`/`$endpos` of a production are the start of its
  first token and the end of its last; for an empty production (an absent
  `option`), the end of the most recently shifted token (`prevEnd`). The only
  production whose region starts at an absent option is `expression_statement`
  (`expression? SEMICOLON`): a null statement's region starts at the end of the
  previous token.

Errors raised by actions are errors here too: K&R definitions with a
declaration list (`KnR_declaration`), asm labels on declarators (the fork's
refusal in `asm_register`), encoding prefixes in `asm`, inconsistent string
prefixes. CN (`/*@ ... @*/` under `at_magic_comments`) is not supported: the
switch is off, so `CERB_MAGIC` never occurs.

The parser is total: every function takes a fuel argument `n` and recurses on
its predecessor (structural recursion), and the driver gives fuel proportional
to the number of tokens (each recursion level either consumes a token or is one
of a bounded number of grammar levels above one that does). -/

namespace CSemCParser

open CerbLocation (Pos Loc Cursor)

abbrev Ctx := Std.HashSet String

/-- `Lexer_feedback.cerb_builtin_types`, prefixed `__cerbty_`: the initial
typedef names. -/
def initialCtx : Ctx :=
  ["jmp_buf", "sig_atomic_t", "va_list", "ptrdiff_t", "ptraddr_t", "wchar_t",
   "int8_t", "int16_t", "int32_t", "int64_t", "int128_t",
   "uint8_t", "uint16_t", "uint32_t", "uint64_t", "uint128_t",
   "int_least8_t", "int_least16_t", "int_least32_t", "int_least64_t",
   "uint_least8_t", "uint_least16_t", "uint_least32_t", "uint_least64_t",
   "int_fast8_t", "int_fast16_t", "int_fast32_t", "int_fast64_t",
   "uint_fast8_t", "uint_fast16_t", "uint_fast32_t", "uint_fast64_t",
   "intptr_t", "uintptr_t", "intmax_t", "uintmax_t", "size_t", "ssize_t",
   "FILE", "fpos_t", "cnd_t", "thrd_t", "tss_t", "mtx_t", "once_flag",
   "time_t", "timer_t", "clock_t", "clockid_t", "suseconds_t",
   "blkcnt_t", "blksize_t", "dev_t", "fsblkcnt_t", "fsfilcnt_t",
   "gid_t", "id_t", "uid_t", "pid_t", "key_t", "mode_t", "ino_t", "nlink_t", "nlink_t", "off_t",
   "pthread_attr_t", "pthread_barrier_t", "pthread_barrierattr_t",
   "pthread_cond_t", "pthread_condattr_t", "pthread_key_t",
   "pthread_mutex_t", "pthread_mutexattr_t", "pthread_once_t",
   "pthread_rwloc_t", "pthread_rwlockattr_t", "pthread_spinlock_t", "pthread_t",
   "trace_attr_t", "trace_event_id_t", "trace_event_set_t", "trace_id_t",
   "max_align_t"].foldl (fun s n => s.insert ("__cerbty_" ++ n)) {}

/-- A token with its positions mapped to Cerberus positions. -/
structure PTok where
  tok : Tok
  sp : Pos
  ep : Pos
  deriving Inhabited

/-- Parser configuration: `gnu` turns on the grammar extensions of
csem/GRAMMAR.md; `false` is the menhir grammar exactly. -/
structure ParseCfg where
  gnu : Bool := false
  deriving Inhabited

structure PState where
  toks : Array PTok
  idx : Nat
  /-- end position of the last shifted token (menhir's position of an empty
  production) -/
  prevEnd : Pos
  ctx : Ctx
  cfg : ParseCfg

structure PErr where
  pos : Pos
  msg : String
  deriving Inhabited

abbrev P := EStateM PErr PState

/-! ## Lexer-feedback declarators (`lexer_feedback.ml`) -/

inductive DeclSort where
  | ident
  | fn (ctx : Ctx)
  | fnIds (ctx : Ctx) (ids : List identifier)
  | ptr (p : pointer_declarator)
  | other

/-- `Lexer_feedback.declarator`. -/
structure LFDecl where
  id : String
  sort : DeclSort
  direct : direct_declarator

def LFDecl.cabs (d : LFDecl) : declarator :=
  match d.sort with
  | .ptr p => Declarator (some p) d.direct
  | _ => Declarator none d.direct

/-- Result of a declarator that may be concrete or abstract. -/
inductive GenDecl where
  | conc (d : LFDecl)
  | abs (a : abstract_declarator)

inductive DMode where
  | concrete | abstract | either
  deriving BEq

/-! ## Attributes as menhir builds them -/

abbrev AttrArg := Loc × String × List (Loc × String)
/-- `attribute`: `((ns, id), args_opt)`. -/
abbrev Attr1 := (Option identifier × identifier) × Option (List AttrArg)
/-- `attribute_specifier_sequence`: specifiers in REVERSE order, each an
in-order list of attributes. -/
abbrev AttrSeq := List (List Attr1)

def mkAttr (a : Attr1) : attribute0 :=
  { attr_ns := a.1.1, attr_id := a.1.2, attr_args := a.2.getD [] }

/-- `to_attrs` (c_parser.mly:57-68). -/
def toAttrs : Option AttrSeq → attributes
  | none => Attrs []
  | some z => Attrs ((z.reverse.flatten).map mkAttr)

/-- `inject_attr`. -/
def injectAttr (a : Option AttrSeq) : cabs_statement → cabs_statement
  | CabsStatement loc (Attrs xs) s =>
    match toAttrs a with
    | Attrs xs' => CabsStatement loc (Attrs (xs ++ xs')) s

/-! ## Specifier accumulator -/

inductive TState where
  | none | uniq | nonuniq
  deriving BEq

inductive SpecMode where
  /-- `declaration_specifiers` (`typedef` allowed iff the flag) -/
  | decl (allowTypedef : Bool)
  /-- `specifier_qualifier_list` -/
  | specQual
  deriving BEq

structure SpecAcc where
  sc : Array storage_class_specifier := #[]
  ts : Array cabs_type_specifier := #[]
  tq : Array cabs_type_qualifier := #[]
  fs : Array function_specifier := #[]
  al : Array alignment_specifier := #[]
  typedefs : Nat := 0
  tstate : TState := .none

def SpecAcc.specifiers (a : SpecAcc) : specifiers :=
  { storage_classes := (if a.typedefs > 0 then [SC_typedef] else []) ++ a.sc.toList,
    type_specifiers := a.ts.toList, type_qualifiers := a.tq.toList,
    function_specifiers := a.fs.toList, alignment_specifiers := a.al.toList }

/-! ## Token predicates -/

def binop : Tok → Option (Nat × cabs_binary_operator)
  | .STAR => some (10, .CabsMul) | .SLASH => some (10, .CabsDiv) | .PERCENT => some (10, .CabsMod)
  | .PLUS => some (9, .CabsAdd) | .MINUS => some (9, .CabsSub)
  | .LT_LT => some (8, .CabsShl) | .GT_GT => some (8, .CabsShr)
  | .LT => some (7, .CabsLt) | .GT => some (7, .CabsGt) | .LT_EQ => some (7, .CabsLe)
  | .GT_EQ => some (7, .CabsGe)
  | .EQ_EQ => some (6, .CabsEq) | .BANG_EQ => some (6, .CabsNe)
  | .AMPERSAND => some (5, .CabsBand) | .CARET => some (4, .CabsBxor) | .PIPE => some (3, .CabsBor)
  | .AMPERSAND_AMPERSAND => some (2, .CabsAnd) | .PIPE_PIPE => some (1, .CabsOr)
  | _ => none

def assignOp : Tok → Option cabs_assignment_operator
  | .EQ => some .Assign | .STAR_EQ => some .Assign_Mul | .SLASH_EQ => some .Assign_Div
  | .PERCENT_EQ => some .Assign_Mod | .PLUS_EQ => some .Assign_Add | .MINUS_EQ => some .Assign_Sub
  | .LT_LT_EQ => some .Assign_Shl | .GT_GT_EQ => some .Assign_Shr | .AMPERSAND_EQ => some .Assign_Band
  | .CARET_EQ => some .Assign_Bxor | .PIPE_EQ => some .Assign_Bor
  | _ => none

def unaryOp : Tok → Option cabs_unary_operator
  | .AMPERSAND => some .CabsAddress | .STAR => some .CabsIndirection | .PLUS => some .CabsPlus
  | .MINUS => some .CabsMinus | .TILDE => some .CabsBnot | .BANG => some .CabsNot
  | _ => none

def storageClass : Tok → Option storage_class_specifier
  | .EXTERN => some .SC_extern | .STATIC => some .SC_static | .THREAD_LOCAL => some .SC_Thread_local
  | .AUTO => some .SC_auto | .REGISTER => some .SC_register
  | _ => none

/-- `type_qualifier` (ATOMIC included; the caller decides `_Atomic (`). -/
def typeQualifier : Tok → Option cabs_type_qualifier
  | .CONST => some .Q_const | .RESTRICT => some .Q_restrict | .VOLATILE => some .Q_volatile
  | .ATOMIC => some .Q_Atomic
  | _ => none

def functionSpecifier : Tok → Option function_specifier
  | .INLINE => some .FS_inline | .NORETURN => some .FS_Noreturn
  | _ => none

/-- `type_specifier_nonunique`. -/
def nonuniqueSpec : Tok → Option cabs_type_specifier_
  | .CHAR => some .TSpec_char | .SHORT => some .TSpec_short | .INT => some .TSpec_int
  | .LONG => some .TSpec_long | .FLOAT => some .TSpec_float | .DOUBLE => some .TSpec_double
  | .SIGNED => some .TSpec_signed | .UNSIGNED => some .TSpec_unsigned | .COMPLEX => some .TSpec_Complex
  | _ => none

/-- Keywords that begin a `type_specifier_unique` (a typedef name aside). -/
def isUniqueKw : Tok → Bool
  | .VOID | .BOOL | .ATOMIC | .STRUCT | .UNION | .ENUM | .TYPEOF => true
  | _ => false

/-- `c_keyword_as_string`. -/
def keywordString : Tok → Option String
  | .AUTO => "auto" | .BREAK => "break" | .CASE => "case" | .CHAR => "char" | .CONST => "const"
  | .CONTINUE => "continue" | .DEFAULT => "default" | .DO => "do" | .DOUBLE => "double"
  | .ELSE => "else" | .ENUM => "enum" | .EXTERN => "extern" | .FLOAT => "float" | .FOR => "for"
  | .GOTO => "goto" | .IF => "if" | .INLINE => "inline" | .INT => "int" | .LONG => "long"
  | .REGISTER => "register" | .RESTRICT => "restrict" | .RETURN => "return" | .SHORT => "short"
  | .SIGNED => "signed" | .SIZEOF => "sizeof" | .STATIC => "static" | .STRUCT => "struct"
  | .SWITCH => "switch" | .TYPEDEF => "typedef" | .UNION => "union" | .UNSIGNED => "unsigned"
  | .VOID => "void" | .VOLATILE => "volatile" | .WHILE => "while" | .ALIGNAS => "_Alignas"
  | .ALIGNOF => "_Alignof" | .ATOMIC => "_Atomic" | .BOOL => "_Bool" | .COMPLEX => "_Complex"
  | .GENERIC => "_Generic" | .NORETURN => "_Noreturn" | .STATIC_ASSERT => "_Static_assert"
  | .THREAD_LOCAL => "_Thread_local"
  | _ => none

/-! ## Parser state primitives -/

def rg (b e : Pos) : Loc := .region b e .noCursor
def rgc (b e c : Pos) : Loc := .region b e (.pointCursor c)

@[inline] def cur : P PTok := do
  let s ← get
  return s.toks.getD s.idx default

@[inline] def peek : P Tok := do return (← cur).tok

/-- The token `k` positions ahead (the last token, `EOF`, past the end). -/
@[inline] def peekN (k : Nat) : P Tok := do
  let s ← get
  return (s.toks.getD (min (s.idx + k) (s.toks.size - 1)) default).tok

/-- Shift the current token. -/
@[inline] def advance : P PTok := do
  let s ← get
  let t := s.toks.getD s.idx default
  let idx := match t.tok with | .EOF => s.idx | _ => s.idx + 1
  set { s with idx, prevEnd := t.ep }
  return t

@[inline] def prevEnd : P Pos := do return (← get).prevEnd
@[inline] def getCtx : P Ctx := do return (← get).ctx
@[inline] def setCtx (c : Ctx) : P Unit := modify fun s => { s with ctx := c }
@[inline] def gnu : P Bool := do return (← get).cfg.gnu

def failAt {α : Type} (msg : String) : P α := do
  let t ← cur
  throw ⟨t.sp, s!"{msg} (at {t.tok.describe})"⟩

def isTypedefName (s : String) : P Bool := do return (← getCtx).contains s
def declareTypedefname (s : String) : P Unit := modify fun st => { st with ctx := st.ctx.insert s }
def declareVarname (s : String) : P Unit := modify fun st => { st with ctx := st.ctx.erase s }

/-- Is the token a typedef name in the current context? -/
def tokIsTypeName : Tok → P Bool
  | .NAME s => isTypedefName s
  | _ => pure false

/-- Does `t` begin a `type_name` (a `specifier_qualifier_list`)? -/
def startsTypeName (t : Tok) : P Bool := do
  match t with
  | .NAME s => isTypedefName s
  | .ALIGNAS => pure true
  | .BUILTIN_VA_LIST => gnu
  | _ => pure ((nonuniqueSpec t).isSome || isUniqueKw t || (typeQualifier t).isSome)

/-- Does `t` begin `declaration_specifiers` (or the typedef variant)? -/
def startsDeclSpecs (t : Tok) : P Bool := do
  match t with
  | .TYPEDEF => pure true
  | .EXTENSION | .ATTRIBUTE => gnu
  | _ => pure ((storageClass t).isSome || (functionSpecifier t).isSome || (← startsTypeName t))

/-- Does `t` begin a `parameter_declaration`? -/
def startsParamDecl (t : Tok) : P Bool := do
  match t with
  | .LBRACK_LBRACK => pure true
  | .TYPEDEF => pure false
  | _ => startsDeclSpecs t

def expect (t : Tok) (what : String) (ok : Tok → Bool) : P PTok := do
  if ok (← peek) then advance else failAt s!"expected {what}"

def isTok (a : Tok) (b : Tok) : Bool :=
  match a, b with
  | .LPAREN, .LPAREN | .RPAREN, .RPAREN | .LBRACE, .LBRACE | .RBRACE, .RBRACE
  | .LBRACK, .LBRACK | .RBRACK, .RBRACK | .SEMICOLON, .SEMICOLON | .COMMA, .COMMA
  | .COLON, .COLON | .EQ, .EQ | .ELLIPSIS, .ELLIPSIS | .WHILE, .WHILE | .RBRACES, .RBRACES
  | .LBRACK_LBRACK, .LBRACK_LBRACK | .DOT, .DOT | .STAR, .STAR | .PIPES, .PIPES
  | .COLON_COLON, .COLON_COLON | .QUESTION, .QUESTION | .EOF, .EOF => true
  | _, _ => false

def expectT (t : Tok) : P PTok := expect t (t.describe) (isTok t)

@[inline] def at? (t : Tok) : P Bool := do return isTok t (← peek)

/-- Shift `t` if it is next. -/
def accept (t : Tok) : P Bool := do
  if ← at? t then let _ ← advance; return true else return false

def nameTok : Tok → Option String
  | .NAME s => some s
  | _ => none

/-- `general_identifier`: any name, typedef or variable. -/
def pGeneralIdentifier : P identifier := do
  let t ← cur
  match t.tok with
  | .NAME s => let _ ← advance; return Identifier (rg t.sp t.ep) s
  | _ => failAt "expected an identifier"

/-- `var_name`: a name that is not a typedef name. -/
def pVarName : P (PTok × String) := do
  let t ← cur
  match t.tok with
  | .NAME s =>
    if ← isTypedefName s then failAt s!"'{s}' is a typedef name here"
    else let _ ← advance; return (t, s)
  | _ => failAt "expected an identifier"

/-- `string_literal`: one or more adjacent literals, the encoding prefixes
merged (`merge_prefix`; an inconsistent pair is an error). -/
def pStringLiteral : P cabs_string_literal := do
  let t ← cur
  match t.tok with
  | .STRING_LITERAL _ _ => go (← get).toks.size none #[]
  | _ => failAt "expected a string literal"
where
  go : Nat → Option cabs_encoding_prefix → Array (Loc × List String) → P cabs_string_literal
  | 0, pref, acc => return (pref, acc.toList)
  | f+1, pref, acc => do
    let t ← cur
    match t.tok with
    | .STRING_LITERAL p strs =>
      let pref' ← match pref, p with
        | _, none => pure pref
        | none, some _ => pure p
        | some a, some b => if a == b then pure pref else failAt "non-standard string concatenation"
      let _ ← advance
      go f pref' (acc.push (rg t.sp t.ep, strs))
    | _ => return (pref, acc.toList)

/-- `located_string_literal`. -/
def pLocatedStringLiteral : P AttrArg := do
  let s := (← cur).sp
  let (_, comps) ← pStringLiteral
  let e ← prevEnd
  let strs := comps.map fun (l, ss) => (l, String.join ss)
  return (rg s e, String.join (strs.map (·.2)), strs)

/-- `attribute_identifier`. -/
def pAttrIdent : P identifier := do
  let t ← cur
  match t.tok with
  | .NAME s => let _ ← advance; return Identifier (rg t.sp t.ep) s
  | tk => match keywordString tk with
    | some str => let _ ← advance; return Identifier (.point t.sp) str
    | none => failAt "expected an attribute name"

/-- `balanced_token_sequence` (non-empty; the result in order, each element a
`balanced_token`'s strings). -/
def pBalancedSeq : Nat → P (List (List AttrArg))
  | 0 => failAt "parser fuel exhausted"
  | n+1 => do
    let one ← pBalancedToken n
    match ← peek with
    | .LPAREN | .LBRACK | .LBRACE | .STRING_LITERAL _ _ => return one :: (← pBalancedSeq n)
    | _ => return [one]
where
  pBalancedToken : Nat → P (List AttrArg)
  | 0 => failAt "parser fuel exhausted"
  | n+1 => do
    match ← peek with
    | .LPAREN => let _ ← advance; let _ ← pBalancedSeq n; let _ ← expectT .RPAREN; return []
    | .LBRACK => let _ ← advance; let _ ← pBalancedSeq n; let _ ← expectT .RBRACK; return []
    | .LBRACE => let _ ← advance; let _ ← pBalancedSeq n; let _ ← expectT .RBRACE; return []
    | .STRING_LITERAL _ _ => strList n
    | _ => failAt "expected a balanced token"
  strList : Nat → P (List AttrArg)
  | 0 => failAt "parser fuel exhausted"
  | n+1 => do
    let s ← pLocatedStringLiteral
    if ← at? .COMMA then
      let _ ← advance
      match ← peek with
      | .STRING_LITERAL _ _ => return s :: (← strList n)
      | _ => failAt "expected a string literal"
    else return [s]

/-- `attribute_specifier`: `[[ attribute_list ]]`, the attributes in order. -/
def pAttrSpec (n : Nat) : P (List Attr1) := do
  let _ ← expectT .LBRACK_LBRACK
  let attrs ← list n
  let _ ← expectT .RBRACK
  let _ ← expectT .RBRACK
  return attrs
where
  list : Nat → P (List Attr1)
  | 0 => failAt "parser fuel exhausted"
  | n+1 => do
    let i1 ← pAttrIdent
    let tokn ← if ← accept .COLON_COLON then
        let i2 ← pAttrIdent; pure (some i1, i2)
      else pure (none, i1)
    let args ← if ← at? .LPAREN then
        let _ ← advance
        let seq ← pBalancedSeq n
        let _ ← expectT .RPAREN
        pure (some seq.flatten)
      else pure none
    if ← accept .COMMA then return (tokn, args) :: (← list n)
    else return [(tokn, args)]

/-- `attribute_specifier_sequence` (at least one specifier; result reversed). -/
def pAttrSeq : Nat → P AttrSeq
  | 0 => failAt "parser fuel exhausted"
  | n+1 => do
    let a ← pAttrSpec n
    go n [a]
where
  go : Nat → AttrSeq → P AttrSeq
  | 0, acc => return acc
  | n+1, acc => do
    if ← at? .LBRACK_LBRACK then go n ((← pAttrSpec n) :: acc) else return acc

def pAttrSeqOpt (n : Nat) : P (Option AttrSeq) := do
  if ← at? .LBRACK_LBRACK then return some (← pAttrSeq n) else return none

/-- `ioption(attribute_specifier_sequence)` whose value the action drops. -/
def skipAttrs (n : Nat) : P Unit := do let _ ← pAttrSeqOpt n

/-- `declarator_varname`/`declarator_typedefname`. -/
def declareDecl (isTypedef : Bool) (d : LFDecl) : P Unit :=
  if isTypedef then declareTypedefname d.id else declareVarname d.id

/-- `LF.reinstall_function_context`. -/
def reinstallFunctionContext (d : LFDecl) : P Unit :=
  match d.sort with
  | .fn c => do setCtx c; declareVarname d.id
  | _ => pure ()

/-- The dummy declaration of `attribute_declaration` (c_parser.mly
`declaration` last production). -/
def attributeDeclaration (b e : Pos) : cabs_declaration :=
  let loc := rgc b e b
  let specs : specifiers :=
    { storage_classes := [], type_specifiers := [], type_qualifiers := [],
      function_specifiers := [], alignment_specifiers := [] }
  .Declaration_base (Attrs []) specs
    [InitDecl loc (Declarator none (.DDecl_identifier (Attrs []) (Identifier loc "test"))) none]

/-- `LF.create_function_definition` (without magic comments). -/
def createFunctionDefinition (loc : Loc) (attrOpt : Option AttrSeq) (specs : specifiers)
    (d : LFDecl) (stmt : cabs_statement) (revDecls : Option (List cabs_declaration)) :
    P function_definition := do
  let xs := match attrOpt with
    | none => []
    | some z => z.flatten.map mkAttr      -- QUIRK: not reversed (to_attrs reverses)
  let attrs := Attrs xs
  match d.sort, revDecls with
  | .fnIds _ ids, none =>
    let signedInt : specifiers :=
      { storage_classes := [], type_specifiers := [TSpec .unknown .TSpec_int], type_qualifiers := [],
        function_specifiers := [], alignment_specifiers := [] }
    let params := ids.map fun id =>
      PDeclaration_decl signedInt (Declarator none (.DDecl_identifier (Attrs []) id))
    match d.direct with
    | .DDecl_function dd _ => return FunDef loc attrs specs (Declarator none (.DDecl_function dd (Params params false))) stmt
    | _ => failAt "internal: K&R declarator is not a function declarator"
  | .fnIds _ _, some _ =>
    match loc with
    | .region b _ _ => throw ⟨b, "found K&R-style declaration (unsupported)"⟩
    | _ => failAt "found K&R-style declaration (unsupported)"
  | _, _ => return FunDef loc attrs specs d.cabs stmt

end CSemCParser
