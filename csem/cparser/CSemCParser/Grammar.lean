import CSemCParser.Parser

/-! # The grammar proper (c_parser.mly from `primary_expression` to
`translation_unit`)

One mutual block, structurally recursive on the fuel `n`. Each function names
the menhir nonterminal(s) it implements. See `Parser.lean` for how the LR
decisions are reproduced. -/

namespace CSemCParser

open CerbLocation (Pos Loc Cursor)

/-- `scoped(X)`: run `act`, then restore the typedef context. -/
def withScope {α : Type} (act : P α) : P α := do
  let c ← getCtx
  let r ← act
  setCtx c
  return r

def exprStart : cabs_expression → Option Pos
  | CabsExpression (.region b _ _) _ => some b
  | _ => none

def applyPtr (ptr : Option pointer_declarator) (d : LFDecl) : LFDecl :=
  match ptr with
  | none => d
  | some p => { d with sort := .ptr p }

def isName : Tok → Bool
  | .NAME _ => true
  | _ => false

def fuelOut {α : Type} : P α := failAt "parser fuel exhausted"

mutual

/- ## §6.5 Expressions -/

/-- `primary_expression`, and the `postfix_expression` alternatives that are
not postfix operators (the builtins, and the compound literal after
`( type_name )`). -/
def pPrimary : Nat → P cabs_expression
  | 0 => fuelOut
  | n+1 => do
    let t ← cur
    match t.tok with
    | .NAME s =>
      if ← isTypedefName s then failAt s!"unexpected typedef name '{s}'"
      let _ ← advance
      return CabsExpression (rg t.sp t.ep) (CabsEident (Identifier (.point t.sp) s))
    | .CONSTANT c =>
      let _ ← advance
      return CabsExpression (rg t.sp t.ep) (CabsEconst c)
    | .STRING_LITERAL _ _ =>
      let lit ← pStringLiteral
      return CabsExpression (rg t.sp (← prevEnd)) (CabsEstring lit)
    | .LPAREN =>
      let t1 ← peekN 1
      if isTok .LBRACE t1 then
        -- GCC statement expression: NOT scoped (the scoped(compound_statement)
        -- form is commented out in c_parser.mly), so its declarations leak
        let _ ← advance
        let lb ← advance
        let items ← pBlockItems n #[]
        let _ ← expectT .RBRACE
        let _ ← expectT .RPAREN
        return CabsExpression (rgc t.sp (← prevEnd) lb.sp) (CabsEgcc_statement items)
      else if ← startsTypeName t1 then
        let _ ← advance
        let ty ← pTypeName n
        let _ ← expectT .RPAREN
        if !(← at? .LBRACE) then failAt "expected '{' (compound literal)"
        pCompoundLiteral n t.sp ty
      else
        let _ ← advance
        let CabsExpression _ e ← pExpr n
        let _ ← expectT .RPAREN
        return CabsExpression (rg t.sp (← prevEnd)) e
    | .GENERIC =>
      let _ ← advance
      let _ ← expectT .LPAREN
      let e ← pAssign n
      let _ ← expectT .COMMA
      let gas ← pGenericAssocs n []
      let _ ← expectT .RPAREN
      -- QUIRK: generic_assoc_list is built reversed and not reversed back
      return CabsExpression (rg t.sp (← prevEnd)) (CabsEgeneric e gas)
    | .ASSERT =>
      let _ ← advance; let _ ← expectT .LPAREN
      let e ← pAssign n
      let _ ← expectT .RPAREN
      return CabsExpression (rg t.sp (← prevEnd)) (CabsEassert e)
    | .VA_START =>
      let _ ← advance; let lp ← expectT .LPAREN
      let e ← pAssign n
      let _ ← expectT .COMMA
      let i ← pGeneralIdentifier
      let _ ← expectT .RPAREN
      return CabsExpression (rgc t.sp (← prevEnd) lp.sp) (CabsEva_start e i)
    | .VA_COPY =>
      let _ ← advance; let lp ← expectT .LPAREN
      let e1 ← pAssign n
      let _ ← expectT .COMMA
      let e2 ← pAssign n
      let _ ← expectT .RPAREN
      return CabsExpression (rgc t.sp (← prevEnd) lp.sp) (CabsEva_copy e1 e2)
    | .VA_ARG =>
      let _ ← advance; let lp ← expectT .LPAREN
      let e ← pAssign n
      let _ ← expectT .COMMA
      let ty ← pTypeName n
      let _ ← expectT .RPAREN
      return CabsExpression (rgc t.sp (← prevEnd) lp.sp) (CabsEva_arg e ty)
    | .VA_END =>
      let _ ← advance; let lp ← expectT .LPAREN
      let e ← pAssign n
      let _ ← expectT .RPAREN
      return CabsExpression (rgc t.sp (← prevEnd) lp.sp) (CabsEva_end e)
    | .OFFSETOF =>
      let _ ← advance; let _ ← expectT .LPAREN
      let ty ← pTypeName n
      let _ ← expectT .COMMA
      let i ← pGeneralIdentifier
      let _ ← expectT .RPAREN
      return CabsExpression (rg t.sp (← prevEnd)) (CabsEoffsetof ty i)
    | .BUILTIN_TYPES_COMPATIBLE_P =>
      let _ ← advance; let _ ← expectT .LPAREN
      let ty1 ← pTypeName n
      let _ ← expectT .COMMA
      let ty2 ← pTypeName n
      let _ ← expectT .RPAREN
      return CabsExpression (rg t.sp (← prevEnd)) (CabsEbuiltinGNU (.GNUbuiltin_types_compatible_p ty1 ty2))
    | .BUILTIN_CHOOSE_EXPR =>
      let _ ← advance; let _ ← expectT .LPAREN
      let c ← pAssign n
      let _ ← expectT .COMMA
      let e1 ← pAssign n
      let _ ← expectT .COMMA
      let e2 ← pAssign n
      let _ ← expectT .RPAREN
      return CabsExpression (rg t.sp (← prevEnd)) (CabsEbuiltinGNU (.GNUbuiltin_choose_expr c e1 e2))
    | .PRINT_TYPE =>
      let _ ← advance; let _ ← expectT .LPAREN
      let e ← pExpr n
      let _ ← expectT .RPAREN
      return CabsExpression (rg t.sp (← prevEnd)) (CabsEprint_type e)
    | .BMC_ASSUME =>
      let _ ← advance; let _ ← expectT .LPAREN
      let e ← pAssign n
      let _ ← expectT .RPAREN
      return CabsExpression (rg t.sp (← prevEnd)) (CabsEbmc_assume e)
    | _ => failAt "expected an expression"

/-- `( type_name ) { initializer_list ,? }` from the `{` on (`postfix_expression`). -/
def pCompoundLiteral : Nat → Pos → type_name → P cabs_expression
  | 0, _, _ => fuelOut
  | n+1, start, ty => do
    let _ ← expectT .LBRACE
    let inits ← pInitList n
    let _ ← accept .COMMA
    let _ ← expectT .RBRACE
    return CabsExpression (rg start (← prevEnd)) (CabsEcompound ty inits)

/-- `generic_assoc_list` (the result REVERSED, as menhir's action leaves it). -/
def pGenericAssocs : Nat → List cabs_generic_association → P (List cabs_generic_association)
  | 0, _ => fuelOut
  | n+1, acc => do
    let ga ← if ← at? .DEFAULT then
        let _ ← advance
        let _ ← expectT .COLON
        pure (GA_default (← pAssign n))
      else
        let ty ← pTypeName n
        let _ ← expectT .COLON
        pure (GA_type ty (← pAssign n))
    if ← accept .COMMA then pGenericAssocs n (ga :: acc) else return ga :: acc

/-- `postfix_expression`. -/
def pPostfix : Nat → P cabs_expression
  | 0 => fuelOut
  | n+1 => do
    let start := (← cur).sp
    let e ← pPrimary n
    pPostfixTail n start e

/-- The postfix operators after a `postfix_expression` starting at `start`. -/
def pPostfixTail : Nat → Pos → cabs_expression → P cabs_expression
  | 0, _, _ => fuelOut
  | n+1, start, e => do
    let t ← cur
    match t.tok with
    | .LBRACK =>
      let _ ← advance
      let i ← pExpr n
      let _ ← expectT .RBRACK
      pPostfixTail n start (CabsExpression (rg start (← prevEnd)) (CabsEsubscript e i))
    | .LPAREN =>
      let _ ← advance
      let args ← if ← at? .RPAREN then pure [] else pArgs n
      let _ ← expectT .RPAREN
      pPostfixTail n start (CabsExpression (rg start (← prevEnd)) (CabsEcall e args none))
    | .DOT =>
      let _ ← advance
      let i ← pGeneralIdentifier
      pPostfixTail n start (CabsExpression (rgc start (← prevEnd) t.sp) (CabsEmemberof e i))
    | .MINUS_GT =>
      let _ ← advance
      let i ← pGeneralIdentifier
      pPostfixTail n start (CabsExpression (rgc start (← prevEnd) t.sp) (CabsEmemberofptr e i))
    | .PLUS_PLUS =>
      let _ ← advance
      pPostfixTail n start (CabsExpression (rgc start (← prevEnd) t.sp) (CabsEpostincr e))
    | .MINUS_MINUS =>
      let _ ← advance
      pPostfixTail n start (CabsExpression (rgc start (← prevEnd) t.sp) (CabsEpostdecr e))
    | _ => return e

/-- `argument_expression_list` (in order). -/
def pArgs : Nat → P (List cabs_expression)
  | 0 => fuelOut
  | n+1 => do
    let a ← pAssign n
    if ← accept .COMMA then return a :: (← pArgs n) else return [a]

/-- `unary_expression`. -/
def pUnary : Nat → P cabs_expression
  | 0 => fuelOut
  | n+1 => do
    let t ← cur
    match t.tok with
    | .PLUS_PLUS =>
      let _ ← advance
      let e ← pUnary n
      return CabsExpression (rgc t.sp (← prevEnd) t.sp) (CabsEpreincr e)
    | .MINUS_MINUS =>
      let _ ← advance
      let e ← pUnary n
      return CabsExpression (rgc t.sp (← prevEnd) t.sp) (CabsEpredecr e)
    | .SIZEOF =>
      let typeNext ← if isTok .LPAREN (← peekN 1) then startsTypeName (← peekN 2) else pure false
      let _ ← advance
      if typeNext then
        let lp ← advance
        let ty ← pTypeName n
        let _ ← expectT .RPAREN
        if ← at? .LBRACE then
          let cl ← pCompoundLiteral n lp.sp ty
          let e ← pPostfixTail n lp.sp cl
          return CabsExpression (rgc t.sp (← prevEnd) t.sp) (CabsEsizeof_expr e)
        else
          return CabsExpression (rgc t.sp (← prevEnd) t.sp) (CabsEsizeof_type ty)
      else
        let e ← pUnary n
        return CabsExpression (rgc t.sp (← prevEnd) t.sp) (CabsEsizeof_expr e)
    | .ALIGNOF =>
      let _ ← advance
      let _ ← expectT .LPAREN
      if ← startsTypeName (← peekN 0) then
        let ty ← pTypeName n
        let _ ← expectT .RPAREN
        return CabsExpression (rgc t.sp (← prevEnd) t.sp) (CabsEalignof ty)
      else
        if !(← gnu) || t.spelling == "_Alignof" then
          failAt "C11 _Alignof requires a type name"
        let e ← pExpr n
        let _ ← expectT .RPAREN
        return CabsExpression (rgc t.sp (← prevEnd) t.sp) (CabsEalignof_expr e)
    | tk =>
      match unaryOp tk with
      | some op =>
        let _ ← advance
        let (e, _) ← pCast n
        return CabsExpression (rgc t.sp (← prevEnd) t.sp) (CabsEunary op e)
      | none => pPostfix n

/-- `cast_expression`; the flag says whether it is a `unary_expression`. -/
def pCast : Nat → P (cabs_expression × Bool)
  | 0 => fuelOut
  | n+1 => do
    let t ← cur
    let typeNext ← if isTok .LPAREN t.tok then startsTypeName (← peekN 1) else pure false
    if typeNext then
      let _ ← advance
      let ty ← pTypeName n
      let _ ← expectT .RPAREN
      if ← at? .LBRACE then
        let cl ← pCompoundLiteral n t.sp ty
        return (← pPostfixTail n t.sp cl, true)
      else
        let (e, _) ← pCast n
        return (CabsExpression (rgc t.sp (← prevEnd) t.sp) (CabsEcast ty e), false)
    else return (← pUnary n, true)

/-- The binary operator levels (`multiplicative_expression` ...
`logical_OR_expression`) by precedence climbing; all are left-associative
with `cast_expression` operands. -/
def pBinary : Nat → Nat → P (cabs_expression × Bool)
  | 0, _ => fuelOut
  | n+1, minPrec => do
    let start := (← cur).sp
    let (lhs, u) ← pCast n
    pBinTail n minPrec start lhs u

def pBinTail : Nat → Nat → Pos → cabs_expression → Bool → P (cabs_expression × Bool)
  | 0, _, _, _, _ => fuelOut
  | n+1, minPrec, start, lhs, u => do
    let t ← cur
    match binop t.tok with
    | some (p, op) =>
      if p ≥ minPrec then
        let _ ← advance
        let (rhs, _) ← pBinary n (p + 1)
        pBinTail n minPrec start (CabsExpression (rgc start (← prevEnd) t.sp) (CabsEbinary op lhs rhs)) false
      else return (lhs, u)
    | none => return (lhs, u)

/-- `conditional_expression`. -/
def pCond : Nat → P (cabs_expression × Bool)
  | 0 => fuelOut
  | n+1 => do
    let start := (← cur).sp
    let (c, u) ← pBinary n 1
    let t ← cur
    match t.tok with
    | .QUESTION =>
      let _ ← advance
      let e2 ← pExpr n
      let _ ← expectT .COLON
      let (e3, _) ← pCond n
      return (CabsExpression (rgc start (← prevEnd) t.sp) (CabsEcond c e2 e3), false)
    | .QUESTION_COLON =>
      let _ ← advance
      let (e3, _) ← pCond n
      return (CabsExpression (rgc start (← prevEnd) t.sp) (CabsEcondGNU c e3), false)
    | _ => return (c, u)

/-- `assignment_expression`. -/
def pAssign : Nat → P cabs_expression
  | 0 => fuelOut
  | n+1 => do
    let start := (← cur).sp
    let (e, u) ← pCond n
    let t ← cur
    match u, assignOp t.tok with
    | true, some op =>
      let _ ← advance
      let rhs ← pAssign n
      return CabsExpression (rgc start (← prevEnd) t.sp) (CabsEassign op e rhs)
    | _, _ => return e

/-- `expression`. -/
def pExpr : Nat → P cabs_expression
  | 0 => fuelOut
  | n+1 => do
    let start := (← cur).sp
    let e ← pAssign n
    pCommaTail n start e

def pCommaTail : Nat → Pos → cabs_expression → P cabs_expression
  | 0, _, _ => fuelOut
  | n+1, start, e => do
    let t ← cur
    match t.tok with
    | .COMMA =>
      let _ ← advance
      let rhs ← pAssign n
      pCommaTail n start (CabsExpression (rgc start (← prevEnd) t.sp) (CabsEcomma e rhs))
    | _ => return e

/-- `constant_expression`. -/
def pConstExpr : Nat → P cabs_expression
  | 0 => fuelOut
  | n+1 => do return (← pCond n).1

/- ## §6.7 Declarations -/

/-- `declaration_specifiers` / `declaration_specifiers_typedef` /
`specifier_qualifier_list`: the specifier loop (see `Parser.lean`). -/
def pSpecifiers : Nat → SpecMode → SpecAcc → P SpecAcc
  | 0, _, _ => fuelOut
  | n+1, mode, acc => do
    let t ← cur
    let isDecl := match mode with | .decl _ => true | .specQual => false
    let stop : P SpecAcc := do
      if acc.tstate == .none then failAt "expected a type specifier"
      if acc.typedefs > 1 then failAt "duplicate 'typedef'"
      return acc
    match t.tok with
    | .ATTRIBUTE =>
      let attrs ← pAttrSeqOpt n
      pSpecifiers n mode { acc with attrs := combineAttrs acc.attrs (toAttrs attrs) }
    | .EXTENSION =>
      let kw ← advance
      -- A diagnostic-mode marker, retained in Cabs/Ail. It does not change
      -- the declaration's C type or waive any semantic check.
      let attr : attribute0 := {
        attr_ns := some (Identifier (rg kw.sp kw.ep) kw.spelling),
        attr_id := Identifier (rg kw.sp kw.ep) "diagnostic_mode", attr_args := [] }
      pSpecifiers n mode { acc with attrs := combineAttrs acc.attrs (Attrs [attr]) }
    | .TYPEDEF =>
      if mode == .decl true then
        let _ ← advance
        pSpecifiers n mode { acc with typedefs := acc.typedefs + 1 }
      else stop
    | .ATOMIC =>
      if acc.tstate == .none && isTok .LPAREN (← peekN 1) then
        let ts ← pAtomicSpec n
        if ← at? .LBRACK_LBRACK then skipAttrs n
        pSpecifiers n mode { acc with ts := acc.ts.push ts, tstate := .uniq }
      else
        let _ ← advance
        if ← at? .LBRACK_LBRACK then skipAttrs n
        pSpecifiers n mode { acc with tq := acc.tq.push .Q_Atomic }
    | .ALIGNAS =>
      let a ← pAlignas n
      if ← at? .LBRACK_LBRACK then skipAttrs n
      pSpecifiers n mode { acc with al := acc.al.push a }
    | .NAME s =>
      if acc.tstate == .none && (← isTypedefName s) then
        let _ ← advance
        let ts := TSpec (rg t.sp t.ep) (.TSpec_name (Identifier (.point t.sp) s))
        if ← at? .LBRACK_LBRACK then skipAttrs n
        pSpecifiers n mode { acc with ts := acc.ts.push ts, tstate := .uniq }
      else stop
    | .VOID | .BOOL =>
      if acc.tstate == .none then
        let _ ← advance
        let ts := TSpec (rg t.sp t.ep) (match t.tok with | .VOID => .TSpec_void | _ => .TSpec_Bool)
        if ← at? .LBRACK_LBRACK then skipAttrs n
        pSpecifiers n mode { acc with ts := acc.ts.push ts, tstate := .uniq }
      else stop
    | .STRUCT | .UNION =>
      if acc.tstate == .none then
        let ts ← pStructOrUnion n
        if ← at? .LBRACK_LBRACK then skipAttrs n
        pSpecifiers n mode { acc with ts := acc.ts.push ts, tstate := .uniq }
      else stop
    | .ENUM =>
      if acc.tstate == .none then
        let ts ← pEnum n
        if ← at? .LBRACK_LBRACK then skipAttrs n
        pSpecifiers n mode { acc with ts := acc.ts.push ts, tstate := .uniq }
      else stop
    | .TYPEOF =>
      if acc.tstate == .none then
        let ts ← pTypeof n
        if ← at? .LBRACK_LBRACK then skipAttrs n
        pSpecifiers n mode { acc with ts := acc.ts.push ts, tstate := .uniq }
      else stop
    | tk =>
      match nonuniqueSpec tk, typeQualifier tk, storageClass tk, functionSpecifier tk with
      | some sp, _, _, _ =>
        if acc.tstate != .uniq then
          let _ ← advance
          if ← at? .LBRACK_LBRACK then skipAttrs n
          pSpecifiers n mode { acc with ts := acc.ts.push (TSpec (rg t.sp t.ep) sp), tstate := .nonuniq }
        else stop
      | none, some q, _, _ =>
        let _ ← advance
        if ← at? .LBRACK_LBRACK then skipAttrs n
        pSpecifiers n mode { acc with tq := acc.tq.push q }
      | none, none, some sc, _ =>
        if isDecl then
          let _ ← advance
          if ← at? .LBRACK_LBRACK then skipAttrs n
          pSpecifiers n mode { acc with sc := acc.sc.push sc }
        else stop
      | none, none, none, some f =>
        if isDecl then
          let _ ← advance
          if ← at? .LBRACK_LBRACK then skipAttrs n
          pSpecifiers n mode { acc with fs := acc.fs.push f }
        else stop
      | none, none, none, none => stop

/-- `atomic_type_specifier`. -/
def pAtomicSpec : Nat → P cabs_type_specifier
  | 0 => fuelOut
  | n+1 => do
    let kw ← advance
    let _ ← expectT .LPAREN
    let ty ← pTypeName n
    let _ ← expectT .RPAREN
    return TSpec (rg kw.sp (← prevEnd)) (.TSpec_Atomic ty)

/-- `TYPEOF LPAREN expression RPAREN | TYPEOF LPAREN type_name RPAREN`. -/
def pTypeof : Nat → P cabs_type_specifier
  | 0 => fuelOut
  | n+1 => do
    let kw ← advance
    let _ ← expectT .LPAREN
    let s ← if ← startsTypeName (← peekN 0) then pure (.TSpec_typeof_type (← pTypeName n))
      else pure (.TSpec_typeof_expr (← pExpr n))
    let _ ← expectT .RPAREN
    return TSpec (rg kw.sp (← prevEnd)) s

/-- `alignment_specifier`. -/
def pAlignas : Nat → P alignment_specifier
  | 0 => fuelOut
  | n+1 => do
    let _ ← advance
    let _ ← expectT .LPAREN
    let a ← if ← startsTypeName (← peekN 0) then pure (.AS_type (← pTypeName n))
      else pure (.AS_expr (← pConstExpr n))
    let _ ← expectT .RPAREN
    return a

/-- `struct_or_union_specifier`. -/
def pStructOrUnion : Nat → P cabs_type_specifier
  | 0 => fuelOut
  | n+1 => do
    let kw ← advance
    let attrs ← pAttrSeqOpt n
    let iopt ← if isName (← peek) then pure (some (← pGeneralIdentifier)) else pure none
    let body ← if ← at? .LBRACE then
        let _ ← advance
        if ← accept .RBRACE then pure (some [])
        else
          -- boption(SEMICOLON+)
          let _ ← pSemis n
          let ds ← pStructDecls n
          let _ ← expectT .RBRACE
          pure (some ds)
      else if iopt.isNone then failAt "expected '{' or a tag after struct/union"
      else pure none
    let trailing ← if ← at? .ATTRIBUTE then pAttrSeqOpt n else pure none
    let loc := rg kw.sp (← prevEnd)
    let a := combineAttrs (toAttrs attrs) (toAttrs trailing)
    match kw.tok with
    | .STRUCT => return TSpec loc (.TSpec_struct a iopt body)
    | _ => return TSpec loc (.TSpec_union a iopt body)

/-- `SEMICOLON*` (the extra semicolons menhir warns about). -/
def pSemis : Nat → P Unit
  | 0 => fuelOut
  | n+1 => do if ← accept .SEMICOLON then pSemis n else return ()

/-- `struct_declaration_list` (non-empty, until `}`). -/
def pStructDecls : Nat → P (List struct_declaration)
  | 0 => fuelOut
  | n+1 => do
    let d ← pStructDeclaration n
    if ← at? .RBRACE then return [d] else return d :: (← pStructDecls n)

/-- `struct_declaration`. -/
def pStructDeclaration : Nat → P struct_declaration
  | 0 => fuelOut
  | n+1 => do
    if ← at? .STATIC_ASSERT then return .Struct_assert (← pStaticAssert n)
    let attrs ← pAttrSeqOpt n
    let acc ← pSpecifiers n .specQual {}
    let ds ← if ← at? .SEMICOLON then pure [] else pStructDeclarators n
    let _ ← expectT .SEMICOLON
    let _ ← pSemis n
    return .Struct_declaration (combineAttrs (toAttrs attrs) acc.attrs) acc.ts.toList acc.tq.toList acc.al.toList ds

/-- `struct_declarator_list` (in order). -/
def pStructDeclarators : Nat → P (List struct_declarator)
  | 0 => fuelOut
  | n+1 => do
    let sd ← if ← at? .COLON then
        let _ ← advance
        pure (.SDecl_bitfield none (← pConstExpr n))
      else
        let d ← pDeclConcrete n
        if ← accept .COLON then pure (.SDecl_bitfield (some d.cabs) (← pConstExpr n))
        else pure (.SDecl_simple d.cabs)
    if ← accept .COMMA then return sd :: (← pStructDeclarators n) else return [sd]

/-- `enum_specifier`. -/
def pEnum : Nat → P cabs_type_specifier
  | 0 => fuelOut
  | n+1 => do
    let kw ← advance
    skipAttrs n
    let iopt ← if isName (← peek) then pure (some (← pGeneralIdentifier)) else pure none
    if ← at? .LBRACE then
      let _ ← advance
      let es ← pEnumerators n
      let _ ← expectT .RBRACE
      return TSpec (rg kw.sp (← prevEnd)) (.TSpec_enum iopt (some es))
    else match iopt with
      | some i => return TSpec (rg kw.sp (← prevEnd)) (.TSpec_enum (some i) none)
      | none => failAt "expected '{' or a tag after enum"

/-- `enumerator_list COMMA?` (in order). -/
def pEnumerators : Nat → P (List (identifier × Option cabs_expression))
  | 0 => fuelOut
  | n+1 => do
    let i ← pGeneralIdentifier
    match i with | Identifier _ s => declareVarname s   -- enumeration_constant
    skipAttrs n
    let e ← if ← accept .EQ then pure (some (← pConstExpr n)) else pure none
    if ← at? .COMMA then
      if isTok .RBRACE (← peekN 1) then let _ ← advance; return [(i, e)]
      let _ ← advance
      return (i, e) :: (← pEnumerators n)
    else return [(i, e)]

/-- `type_name`. -/
def pTypeName : Nat → P type_name
  | 0 => fuelOut
  | n+1 => do
    let acc ← pSpecifiers n .specQual {}
    if acc.attrs != Attrs [] then failAt "GNU attributes on a type name need an explicit type representation"
    let ad ← match ← peek with
      | .STAR | .LPAREN | .LBRACK =>
        match ← pDeclGen n .abstract with
        | .abs a => pure (some a)
        | .conc _ => failAt "expected an abstract declarator"
      | _ => pure none
    return Type_name acc.ts.toList acc.tq.toList acc.al.toList ad

/-- `pointer`. -/
def pPointer : Nat → P (pointer_declarator × attributes)
  | 0 => fuelOut
  | n+1 => do
    let st ← advance
    if ← at? .LBRACK_LBRACK then skipAttrs n
    let (tq, attrs) ← pPointerQuals n
    let (inner, nested) ← if ← at? .STAR then
        let (p, a) ← pPointer n
        pure (some p, a)
      else pure (none, Attrs [])
    return (PDecl (rg st.sp (← prevEnd)) tq inner, combineAttrs attrs nested)

/-- GNU attributes among pointer qualifiers retain their entity attachment.
Abstract type positions are diagnosed by the caller rather than erased. -/
def pPointerQuals : Nat → P (List cabs_type_qualifier × attributes)
  | 0 => fuelOut
  | n+1 => do
    if ← at? .ATTRIBUTE then
      let attrs ← pAttrSeqOpt n
      let (qs, rest) ← pPointerQuals n
      return (qs, combineAttrs (toAttrs attrs) rest)
    match typeQualifier (← peek) with
    | some q =>
      let _ ← advance
      let (qs, attrs) ← pPointerQuals n
      return (q :: qs, attrs)
    | none => return ([], Attrs [])

/-- `type_qualifier_list?` (in order). -/
def pTypeQuals : Nat → P (List cabs_type_qualifier)
  | 0 => fuelOut
  | n+1 => do
    match typeQualifier (← peek) with
    | some q => let _ ← advance; return q :: (← pTypeQuals n)
    | none => return []

/-- A declarator in mode `concrete` (`declarator`), `abstract`
(`abstract_declarator`) or `either` (a parameter declaration). -/
def pDeclGen : Nat → DMode → P GenDecl
  | 0, _ => fuelOut
  | n+1, mode => do
    let (ptr, pointerAttrs) ← if ← at? .STAR then
        let (p, a) ← pPointer n
        pure (some p, a)
      else pure (none, Attrs [])
    if mode == .abstract && pointerAttrs != Attrs [] then
      failAt "GNU attributes on an abstract pointer need a type representation"
    let t ← cur
    match t.tok with
    | .NAME s =>
      if mode == .abstract then failAt "unexpected identifier in an abstract declarator"
      let _ ← advance
      let attrs ← pAttrSeqOpt n
      let d : LFDecl := { id := s, sort := .ident,
                          direct := .DDecl_identifier (combineAttrs pointerAttrs (toAttrs attrs)) (Identifier (rg t.sp t.ep) s) }
      let d ← pConcSuffix n t.sp d
      return .conc (applyPtr ptr d)
    | .LPAREN =>
      let t1 ← peekN 1
      let funNow ← if mode == .concrete then pure false
        else if isTok .RPAREN t1 then pure true
        else startsParamDecl t1
      if funNow then
        if pointerAttrs != Attrs [] then failAt "GNU attributes on an abstract pointer need a type representation"
        let dabs ← pAbsFun n none
        let dabs ← pAbsSuffix n dabs
        return .abs (.AbsDecl_direct ptr dabs)
      else
        let _ ← advance
        let inner ← pDeclGen n mode
        let _ ← expectT .RPAREN
        match inner with
        | .conc d =>
          let d' : LFDecl := { d with direct := .DDecl_declarator d.cabs, sort := .other }
          let d' ← pConcSuffix n t.sp d'
          let dd ← addDirectAttrs n pointerAttrs d'.direct
          return .conc (applyPtr ptr { d' with direct := dd })
        | .abs a =>
          if pointerAttrs != Attrs [] then failAt "GNU attributes on an abstract pointer need a type representation"
          let dabs ← pAbsSuffix n (.DAbs_abs_declarator a)
          return .abs (.AbsDecl_direct ptr dabs)
    | .LBRACK =>
      if pointerAttrs != Attrs [] then failAt "GNU attributes on an abstract pointer need a type representation"
      if mode == .concrete then failAt "expected a declarator"
      let dabs ← pAbsArray n none
      let dabs ← pAbsSuffix n dabs
      return .abs (.AbsDecl_direct ptr dabs)
    | _ =>
      if pointerAttrs != Attrs [] then failAt "GNU attributes on an abstract pointer need a type representation"
      match ptr, mode with
      | some p, .abstract => return .abs (.AbsDecl_pointer p)
      | some p, .either => return .abs (.AbsDecl_pointer p)
      | _, _ => failAt "expected a declarator"

/-- A concrete declarator (`declarator`). -/
def pDeclConcrete : Nat → P LFDecl
  | 0 => fuelOut
  | n+1 => do
    match ← pDeclGen n .concrete with
    | .conc d => return d
    | .abs _ => failAt "expected a declarator"

/-- The array and function suffixes of a `direct_declarator` starting at
`start`. -/
def pConcSuffix : Nat → Pos → LFDecl → P LFDecl
  | 0, _, _ => fuelOut
  | n+1, start, d => do
    match ← peek with
    | .ATTRIBUTE =>
      let a ← pAttrSeqOpt n
      let dd ← addDirectAttrs n (toAttrs a) d.direct
      pConcSuffix n start { d with direct := dd }
    | .LBRACK =>
      let ad ← pConcArray n start
      if ← at? .LBRACK_LBRACK then skipAttrs n
      pConcSuffix n start { d with direct := .DDecl_array d.direct ad, sort := .other }
    | .LPAREN =>
      let t1 ← peekN 1
      if isTok .RPAREN t1 then
        -- direct_declarator LPAREN save_context RPAREN (no attributes after)
        let _ ← advance
        let c ← getCtx
        let _ ← advance
        pConcSuffix n start { d with direct := .DDecl_function d.direct (Params [] false), sort := .fn c }
      else if isName t1 && !(← tokIsTypeName t1) then
        -- direct_declarator LPAREN save_context identifier_list RPAREN
        let _ ← advance
        let c ← getCtx
        let ids ← pIdentList n none
        let _ ← expectT .RPAREN
        pConcSuffix n start { d with direct := .DDecl_function d.direct (Params [] false), sort := .fnIds c ids }
      else
        -- function_declarator ioption(attribute_specifier_sequence)
        let _ ← advance
        let (ptys, c) ← pParamTypeListScoped n
        let _ ← expectT .RPAREN
        if ← at? .LBRACK_LBRACK then skipAttrs n
        pConcSuffix n start { d with direct := .DDecl_function d.direct ptys, sort := .fn c }
    | _ => return d

/-- `identifier_list` (in order). QUIRK: `Identifier (point $startpos, id)` in
the left-recursive production is the start of the whole list, so every
identifier after the first is located at the first. -/
def pIdentList : Nat → Option Pos → P (List identifier)
  | 0, _ => fuelOut
  | n+1, first => do
    let (t, s) ← pVarName
    let p := first.getD t.sp
    let id := Identifier (.point p) s
    if ← accept .COMMA then return id :: (← pIdentList n (some p)) else return [id]

/-- `scoped(parameter_type_list)`: the parameters and the context saved after
them (`parameter_type_list`'s `save_context`), the context restored. -/
def pParamTypeListScoped : Nat → P (parameter_type_list × Ctx)
  | 0 => fuelOut
  | n+1 => do
    let saved ← getCtx
    let (ps, variadic) ← pParamList n
    let c ← getCtx
    setCtx saved
    return (Params ps variadic, c)

/-- `parameter_list (COMMA ELLIPSIS)?` (in order). -/
def pParamList : Nat → P (List parameter_declaration × Bool)
  | 0 => fuelOut
  | n+1 => do
    let p ← pParamDecl n
    if ← at? .COMMA then
      if isTok .ELLIPSIS (← peekN 1) then
        let _ ← advance; let _ ← advance
        return ([p], true)
      let _ ← advance
      let (ps, v) ← pParamList n
      return (p :: ps, v)
    else return ([p], false)

/-- `parameter_declaration`. -/
def pParamDecl : Nat → P parameter_declaration
  | 0 => fuelOut
  | n+1 => do
    if ← at? .LBRACK_LBRACK then skipAttrs n
    let leading ← pAttrSeqOpt n
    let acc ← pSpecifiers n (.decl false) {}
    let attrs := combineAttrs (toAttrs leading) acc.attrs
    let specs := acc.specifiers
    match ← peek with
    | .COMMA | .RPAREN =>
      if attrs != Attrs [] then failAt "GNU attributes on an unnamed parameter need a type representation"
      return PDeclaration_abs_decl specs none
    | _ =>
      match ← pDeclGen n .either with
      | .conc d =>
        declareVarname d.id    -- declarator_varname
        let direct ← addDirectAttrs n attrs d.direct
        return PDeclaration_decl specs ({ d with direct := direct }).cabs
      | .abs a =>
        if attrs != Attrs [] then failAt "GNU attributes on an abstract parameter need a type representation"
        return PDeclaration_abs_decl specs (some a)

/-- `array_declarator` from its `[` (the region starts at the direct
declarator's start). -/
def pConcArray : Nat → Pos → P array_declarator
  | 0, _ => fuelOut
  | n+1, start => do
    let _ ← advance
    if ← accept .STATIC then
      let tq ← pTypeQuals n
      let e ← pAssign n
      let _ ← expectT .RBRACK
      return ADecl (rg start (← prevEnd)) tq true (some (.ADeclSize_expression e))
    let tq ← pTypeQuals n
    if !tq.isEmpty then
      if ← accept .STATIC then
        let e ← pAssign n
        let _ ← expectT .RBRACK
        return ADecl (rg start (← prevEnd)) tq true (some (.ADeclSize_expression e))
    if (← at? .STAR) && isTok .RBRACK (← peekN 1) then
      let _ ← advance; let _ ← advance
      return ADecl (rg start (← prevEnd)) tq false (some .ADeclSize_asterisk)
    if ← accept .RBRACK then
      return ADecl (rg start (← prevEnd)) tq false none
    let e ← pAssign n
    let _ ← expectT .RBRACK
    return ADecl (rg start (← prevEnd)) tq false (some (.ADeclSize_expression e))

/-- `array_abstract_declarator ioption(attribute_specifier_sequence)` from its
`[`. QUIRK: the qualifiers are not reversed back (`Option.fold ~none:[]
~some:id tquals` on the reversed `type_qualifier_list`), and the location is
`Cerb_location.unknown`. -/
def pAbsArray : Nat → Option direct_abstract_declarator → P direct_abstract_declarator
  | 0, _ => fuelOut
  | n+1, inner => do
    let _ ← advance
    let r ← if ← accept .STATIC then
        let tq ← pTypeQuals n
        let e ← pAssign n
        let _ ← expectT .RBRACK
        pure (ADecl .unknown tq.reverse true (some (.ADeclSize_expression e)))
      else
        let tq ← pTypeQuals n
        if !tq.isEmpty && (← accept .STATIC) then
          let e ← pAssign n
          let _ ← expectT .RBRACK
          pure (ADecl .unknown tq.reverse true (some (.ADeclSize_expression e)))
        else if tq.isEmpty && (← at? .STAR) && isTok .RBRACK (← peekN 1) then
          let _ ← advance; let _ ← advance
          pure (ADecl .unknown [] false (some .ADeclSize_asterisk))
        else if ← accept .RBRACK then
          pure (ADecl .unknown tq.reverse false none)
        else
          let e ← pAssign n
          let _ ← expectT .RBRACK
          pure (ADecl .unknown tq.reverse false (some (.ADeclSize_expression e)))
    skipAttrs n
    return .DAbs_array inner r

/-- `function_abstract_declarator ioption(attribute_specifier_sequence)` from
its `(`. -/
def pAbsFun : Nat → Option direct_abstract_declarator → P direct_abstract_declarator
  | 0, _ => fuelOut
  | n+1, inner => do
    let _ ← advance
    let ptys ← if ← accept .RPAREN then pure (Params [] false)
      else
        let (ptys, _) ← pParamTypeListScoped n
        let _ ← expectT .RPAREN
        pure ptys
    skipAttrs n
    return .DAbs_function inner ptys

/-- The suffixes of a `direct_abstract_declarator`. -/
def pAbsSuffix : Nat → direct_abstract_declarator → P direct_abstract_declarator
  | 0, _ => fuelOut
  | n+1, d => do
    match ← peek with
    | .LBRACK => pAbsSuffix n (← pAbsArray n (some d))
    | .LPAREN => pAbsSuffix n (← pAbsFun n (some d))
    | _ => return d

/-- `initializer_`. -/
def pInitializer : Nat → P initializer_
  | 0 => fuelOut
  | n+1 => do
    if ← at? .LBRACE then
      let lb ← advance
      let inits ← pInitList n
      let _ ← accept .COMMA
      let _ ← expectT .RBRACE
      return Init_list (rg lb.sp (← prevEnd)) inits
    else return Init_expr (← pAssign n)

/-- `initializer_list` (in order; a trailing comma is left to the caller). -/
def pInitList : Nat → P (List (Option (List designator) × initializer_))
  | 0 => fuelOut
  | n+1 => do
    let desig ← match ← peek with
      | .LBRACK | .DOT =>
        let ds ← pDesignators n
        let _ ← expectT .EQ
        pure (some ds)
      | _ => pure none
    let init ← pInitializer n
    if (← at? .COMMA) && !isTok .RBRACE (← peekN 1) then
      let _ ← advance
      return (desig, init) :: (← pInitList n)
    else return [(desig, init)]

/-- `designator_list` (in order). -/
def pDesignators : Nat → P (List designator)
  | 0 => fuelOut
  | n+1 => do
    let d ← match ← peek with
      | .LBRACK =>
        let _ ← advance
        let e ← pConstExpr n
        let _ ← expectT .RBRACK
        pure (Desig_array e)
      | .DOT =>
        let _ ← advance
        pure (Desig_member (← pGeneralIdentifier))
      | _ => failAt "expected a designator"
    match ← peek with
    | .LBRACK | .DOT => return d :: (← pDesignators n)
    | _ => return [d]

/-- `static_assert_declaration`. -/
def pStaticAssert : Nat → P static_assert_declaration
  | 0 => fuelOut
  | n+1 => do
    let _ ← advance
    let _ ← expectT .LPAREN
    let e ← pConstExpr n
    let _ ← expectT .COMMA
    let lit ← pStringLiteral
    let _ ← expectT .RPAREN
    let _ ← expectT .SEMICOLON
    return Static_assert e lit

/-- `init_declarator_list(declarator_varname | declarator_typedefname)`. -/
def pInitDeclarators : Nat → Bool → P (List init_declarator)
  | 0, _ => fuelOut
  | n+1, isTD => do
    let start := (← cur).sp
    let d ← pDeclConcrete n
    let idecl ← pInitDeclTail n start isTD d
    if ← accept .COMMA then return idecl :: (← pInitDeclarators n isTD) else return [idecl]

/-- `init_declarator` after its declarator `d` (starting at `start`): the
`declarator_varname`/`declarator_typedefname` action, `ioption(asm_register)`,
`EQ initializer_`. -/
def pInitDeclTail : Nat → Pos → Bool → LFDecl → P init_declarator
  | 0, _, _, _ => fuelOut
  | n+1, start, isTD, d => do
    declareDecl isTD d
    let d ← if ← at? .ASM then
        if !(← gnu) then
          failAt "unimplemented keyword 'asm (inline assembly label on a declarator is unsupported)'"
        let kw ← advance
        let _ ← expectT .LPAREN
        match ← peek with
        | .STRING_LITERAL _ _ => pure ()
        | _ => failAt "expected an asm-label string literal"
        let args ← pGnuAttrArgs n [] [] []
        let _ ← expectT .RPAREN
        -- Syntax is retained, with its original alias and literal locations.
        -- The shared elaborator diagnoses the separate DESIGN §5 effect.
        let attr : attribute0 := {
          attr_ns := some (Identifier (rg kw.sp kw.ep) kw.spelling),
          attr_id := Identifier (rg kw.sp kw.ep) "asm_label", attr_args := args }
        let dd ← addDirectAttrs n (Attrs [attr]) d.direct
        pure { d with direct := dd }
      else pure d
    let init ← if ← accept .EQ then pure (some (← pInitializer n)) else pure none
    return InitDecl (rg start (← prevEnd)) d.cabs init

/-- The rest of a `declaration` after its specifiers. -/
def pDeclarationAfterSpecs : Nat → Option AttrSeq → SpecAcc → P cabs_declaration
  | 0, _, _ => fuelOut
  | n+1, attrs, acc => do
    let ids ← if ← at? .SEMICOLON then pure [] else pInitDeclarators n (acc.typedefs > 0)
    let _ ← expectT .SEMICOLON
    return .Declaration_base (combineAttrs (toAttrs attrs) acc.attrs) acc.specifiers ids

/-- `declaration`, after its leading attribute sequence (if any; `start` is
where the declaration begins). -/
def pDeclaration : Nat → Option AttrSeq → Pos → P cabs_declaration
  | 0, _, _ => fuelOut
  | n+1, attrs, start => do
    match attrs with
    | none =>
      if ← at? .STATIC_ASSERT then return .Declaration_static_assert (← pStaticAssert n)
    | some _ =>
      if ← accept .SEMICOLON then return attributeDeclaration start (← prevEnd)
    let acc ← pSpecifiers n (.decl true) {}
    pDeclarationAfterSpecs n attrs acc

/-- `declaration_list` (K&R; `no_leading_attribute_declaration`+). -/
def pDeclList : Nat → P (List cabs_declaration)
  | 0 => fuelOut
  | n+1 => do
    let d ← pDeclaration n none (← cur).sp
    let t ← peek
    if isTok .STATIC_ASSERT t || (← startsDeclSpecs t) then return d :: (← pDeclList n)
    else return [d]

/- ## §6.8 Statements -/

/-- `block_item_list?` until `}`. -/
def pBlockItems : Nat → Array cabs_statement → P (List cabs_statement)
  | 0, _ => fuelOut
  | n+1, acc => do
    if ← at? .RBRACE then return acc.toList
    let it ← pBlockItem n
    pBlockItems n (acc.push it)

/-- `block_item`. -/
def pBlockItem : Nat → P cabs_statement
  | 0 => fuelOut
  | n+1 => do
    let t ← cur
    match t.tok with
    | .LBRACK_LBRACK | .ATTRIBUTE =>
      let attrs ← pAttrSeq n
      let t' ← peek
      if isTok .SEMICOLON t' then
        let _ ← advance
        return CabsStatement (rg t.sp (← prevEnd)) (Attrs []) (CabsSdecl (attributeDeclaration t.sp (← prevEnd)))
      else if isName t' && isTok .COLON (← peekN 1) then pStatementAttrs n (some attrs) t.sp
      else if ← startsDeclSpecs t' then
        let d ← pDeclaration n (some attrs) t.sp
        return CabsStatement (rg t.sp (← prevEnd)) (Attrs []) (CabsSdecl d)
      else pStatementAttrs n (some attrs) t.sp
    | tk =>
      if isName tk && isTok .COLON (← peekN 1) then pStatementAttrs n none t.sp
      else if isTok .STATIC_ASSERT tk || (← startsDeclSpecs tk) then
        let d ← pDeclaration n none t.sp
        return CabsStatement (rg t.sp (← prevEnd)) (Attrs []) (CabsSdecl d)
      else pStatementAttrs n none t.sp

/-- `statement`. -/
def pStatement : Nat → P cabs_statement
  | 0 => fuelOut
  | n+1 => do
    let t ← cur
    if startsAttrs t.tok then
      let attrs ← pAttrSeq n
      pStatementAttrs n (some attrs) t.sp
    else pStatementAttrs n none t.sp

/-- `statement` after an optional attribute sequence beginning at `aStart`. -/
def pStatementAttrs : Nat → Option AttrSeq → Pos → P cabs_statement
  | 0, _, _ => fuelOut
  | n+1, attrs, aStart => do
    let t ← cur
    match t.tok with
    | .NAME _ =>
      if isTok .COLON (← peekN 1) then
        let i ← pGeneralIdentifier
        let _ ← advance
        let st ← pStatement n
        return CabsStatement (rg t.sp (← prevEnd)) (toAttrs attrs) (CabsSlabel i st)
      else pExprStatement n attrs aStart
    | .CASE =>
      let _ ← advance
      let e ← pConstExpr n
      if ← accept .ELLIPSIS then
        let e2 ← pConstExpr n
        let _ ← expectT .COLON
        let st ← pStatement n
        return CabsStatement (rg t.sp (← prevEnd)) (toAttrs attrs) (CabsScaseGNU e e2 st)
      let _ ← expectT .COLON
      let st ← pStatement n
      return CabsStatement (rg t.sp (← prevEnd)) (toAttrs attrs) (CabsScase e st)
    | .DEFAULT =>
      let _ ← advance
      let _ ← expectT .COLON
      let st ← pStatement n
      return CabsStatement (rg t.sp (← prevEnd)) (toAttrs attrs) (CabsSdefault st)
    | .LBRACE | .LBRACES => return injectAttr attrs (← withScope (pCompound n))
    | .IF | .SWITCH => return injectAttr attrs (← withScope (pSelection n))
    | .WHILE | .DO | .FOR => return injectAttr attrs (← withScope (pIteration n))
    | .GOTO | .CONTINUE | .BREAK | .RETURN => return injectAttr attrs (← pJump n)
    | .ASM =>
      if attrs.isSome then failAt "unexpected asm after attributes"
      pAsmStatement n
    | _ => pExprStatement n attrs aStart

/-- `expression_statement`. -/
def pExprStatement : Nat → Option AttrSeq → Pos → P cabs_statement
  | 0, _, _ => fuelOut
  | n+1, attrs, aStart => do
    match attrs with
    | none =>
      if ← at? .SEMICOLON then
        -- `expression?` absent: the region starts at the end of the previous token
        let s ← prevEnd
        let semi ← advance
        return CabsStatement (rg s semi.ep) (Attrs []) CabsSnull
      let start := (← cur).sp
      let e ← pExpr n
      let _ ← expectT .SEMICOLON
      return CabsStatement (rg start (← prevEnd)) (Attrs []) (CabsSexpr e)
    | some _ =>
      let e ← pExpr n
      let _ ← expectT .SEMICOLON
      return CabsStatement (rg aStart (← prevEnd)) (toAttrs attrs) (CabsSexpr e)

/-- `compound_statement` (unscoped; `statement` scopes it). -/
def pCompound : Nat → P cabs_statement
  | 0 => fuelOut
  | n+1 => do
    let t ← cur
    match t.tok with
    | .LBRACE =>
      let _ ← advance
      let items ← pBlockItems n #[]
      let _ ← expectT .RBRACE
      return CabsStatement (rg t.sp (← prevEnd)) (Attrs []) (CabsSblock items)
    | .LBRACES =>
      let _ ← advance
      let sts ← pParStatements n
      let _ ← expectT .RBRACES
      return CabsStatement (rg t.sp (← prevEnd)) (Attrs []) (CabsSpar sts)
    | _ => failAt "expected '{'"

/-- `separated_nonempty_list(PIPES, statement)`. -/
def pParStatements : Nat → P (List cabs_statement)
  | 0 => fuelOut
  | n+1 => do
    let s ← pStatement n
    if ← accept .PIPES then return s :: (← pParStatements n) else return [s]

/-- `selection_statement`. -/
def pSelection : Nat → P cabs_statement
  | 0 => fuelOut
  | n+1 => do
    let kw ← advance
    let _ ← expectT .LPAREN
    let e ← pExpr n
    let _ ← expectT .RPAREN
    let s1 ← withScope (pStatement n)
    match kw.tok with
    | .IF =>
      if ← accept .ELSE then
        let s2 ← withScope (pStatement n)
        return CabsStatement (rg kw.sp (← prevEnd)) (Attrs []) (CabsSif e s1 (some s2))
      return CabsStatement (rg kw.sp (← prevEnd)) (Attrs []) (CabsSif e s1 none)
    | _ => return CabsStatement (rg kw.sp (← prevEnd)) (Attrs []) (CabsSswitch e s1)

/-- `expression?` before `stop`. -/
def pOptExpr : Nat → Tok → P (Option cabs_expression)
  | 0, _ => fuelOut
  | n+1, stop => do if ← at? stop then return none else return some (← pExpr n)

/-- `iteration_statement`. -/
def pIteration : Nat → P cabs_statement
  | 0 => fuelOut
  | n+1 => do
    let kw ← advance
    match kw.tok with
    | .WHILE =>
      let _ ← expectT .LPAREN
      let e ← pExpr n
      let _ ← expectT .RPAREN
      let s ← withScope (pStatement n)
      return CabsStatement (rg kw.sp (← prevEnd)) (Attrs []) (CabsSwhile e s)
    | .DO =>
      let s ← withScope (pStatement n)
      let _ ← expectT .WHILE
      let _ ← expectT .LPAREN
      let e ← pExpr n
      let _ ← expectT .RPAREN
      let _ ← expectT .SEMICOLON
      return CabsStatement (rg kw.sp (← prevEnd)) (Attrs []) (CabsSdo e s)
    | _ =>
      let _ ← expectT .LPAREN
      let t ← cur
      if isTok .LBRACK_LBRACK t.tok || isTok .STATIC_ASSERT t.tok || (← startsDeclSpecs t.tok) then
        let attrs ← pAttrSeqOpt n
        let d ← pDeclaration n attrs t.sp
        let dl := rg t.sp (← prevEnd)
        let e2 ← pOptExpr n .SEMICOLON
        let _ ← expectT .SEMICOLON
        let e3 ← pOptExpr n .RPAREN
        let _ ← expectT .RPAREN
        let s ← withScope (pStatement n)
        return CabsStatement (rg kw.sp (← prevEnd)) (Attrs []) (CabsSfor (some (.FC_decl dl d)) e2 e3 s)
      let e1 ← pOptExpr n .SEMICOLON
      let _ ← expectT .SEMICOLON
      let e2 ← pOptExpr n .SEMICOLON
      let _ ← expectT .SEMICOLON
      let e3 ← pOptExpr n .RPAREN
      let _ ← expectT .RPAREN
      let s ← withScope (pStatement n)
      return CabsStatement (rg kw.sp (← prevEnd)) (Attrs []) (CabsSfor (e1.map .FC_expr) e2 e3 s)

/-- `jump_statement`. -/
def pJump : Nat → P cabs_statement
  | 0 => fuelOut
  | n+1 => do
    let kw ← advance
    let s ← match kw.tok with
      | .GOTO => pure (CabsSgoto (← pGeneralIdentifier))
      | .CONTINUE => pure CabsScontinue
      | .BREAK => pure CabsSbreak
      | _ => pure (CabsSreturn (← pOptExpr n .SEMICOLON))
    let _ ← expectT .SEMICOLON
    return CabsStatement (rg kw.sp (← prevEnd)) (Attrs []) s

/-- `asm_statement` (no trailing `;`: it is a separate null statement). -/
def pAsmStatement : Nat → P cabs_statement
  | 0 => fuelOut
  | n+1 => do
    let kw ← advance
    let (vol, inl) ← pAsmQuals n false false
    let _ ← expectT .LPAREN
    let (pref, strs) ← pStringLiteral
    if ← at? .COLON then pAsmSections n 0
    let _ ← expectT .RPAREN
    if pref.isSome then failAt "encoding prefix found inside a __asm__ ()"
    return CabsStatement (rg kw.sp (← prevEnd)) (Attrs []) (CabsSasm vol inl strs)

/-- `asm_qualifier*`. -/
def pAsmQuals : Nat → Bool → Bool → P (Bool × Bool)
  | 0, _, _ => fuelOut
  | n+1, vol, inl => do
    match ← peek with
    | .VOLATILE | .ASM_VOLATILE => let _ ← advance; pAsmQuals n true inl
    | .INLINE => let _ ← advance; pAsmQuals n vol true
    | .GOTO => let _ ← advance; pAsmQuals n vol inl
    | _ => return (vol, inl)

/-- `asm_with_output`, `asm_with_input`, `asm_with_clobbers`,
`asm_with_labels` (section `k` = 0..3), each from its `:`. -/
def pAsmSections : Nat → Nat → P Unit
  | 0, _ => fuelOut
  | n+1, k => do
    let _ ← expectT .COLON
    match k with
    | 0 | 1 =>
      match ← peek with
      | .STRING_LITERAL _ _ | .LBRACK => pAsmOperands n
      | _ => pure ()
    | 2 =>
      match ← peek with
      | .STRING_LITERAL _ _ => pAsmClobbers n
      | _ => pure ()
    | _ => pAsmLabels n
    if k < 3 && (← at? .COLON) then pAsmSections n (k + 1)

/-- `separated_nonempty_list(COMMA, asm_output_input)`. -/
def pAsmOperands : Nat → P Unit
  | 0 => fuelOut
  | n+1 => do
    if ← accept .LBRACK then
      let _ ← pVarName
      let _ ← expectT .RBRACK
    let _ ← pStringLiteral
    let _ ← expectT .LPAREN
    let _ ← pExpr n
    let _ ← expectT .RPAREN
    if ← accept .COMMA then pAsmOperands n

/-- `separated_nonempty_list(COMMA, asm_clobber)`. -/
def pAsmClobbers : Nat → P Unit
  | 0 => fuelOut
  | n+1 => do
    let _ ← pStringLiteral
    if ← accept .COMMA then pAsmClobbers n

/-- `asm_label*`. -/
def pAsmLabels : Nat → P Unit
  | 0 => fuelOut
  | n+1 => do
    if isName (← peek) then
      let _ ← pVarName
      pAsmLabels n

/- ## §6.9 External definitions -/

/-- `external_declaration` (`function_definition` or `declaration`). -/
def pExternalDecl : Nat → P external_declaration
  | 0 => fuelOut
  | n+1 => do
    let t ← cur
    let attrs ← pAttrSeqOpt n
    match attrs with
    | none =>
      if ← at? .STATIC_ASSERT then return .EDecl_decl (.Declaration_static_assert (← pStaticAssert n))
    | some _ =>
      if ← accept .SEMICOLON then return .EDecl_decl (attributeDeclaration t.sp (← prevEnd))
    let acc ← pSpecifiers n (.decl true) {}
    if acc.typedefs > 0 then return .EDecl_decl (← pDeclarationAfterSpecs n attrs acc)
    if ← accept .SEMICOLON then return .EDecl_decl (.Declaration_base (combineAttrs (toAttrs attrs) acc.attrs) acc.specifiers [])
    let dstart := (← cur).sp
    let d ← pDeclConcrete n
    declareVarname d.id     -- declarator_varname (both alternatives reduce it)
    let t' ← peek
    let isFun ← match t' with
      | .LBRACE | .LBRACES | .NAME _ | .STATIC_ASSERT | .TYPEDEF => pure true
      | tk => startsDeclSpecs tk
    if isFun then
      -- function_definition1: save the context, install the parameters'
      let ctxt ← getCtx
      reinstallFunctionContext d
      let decls ← match ← peek with
        | .LBRACE | .LBRACES => pure none
        | _ => pure (some (← pDeclList n))
      let body ← pCompound n
      let _ ← accept .SEMICOLON     -- boption(SEMICOLON)
      let loc := rg t.sp (← prevEnd)
      setCtx ctxt
      let f ← createFunctionDefinition loc attrs acc.specifiers d body decls
      match f with
      | FunDef l a sp de st => return .EDecl_func (FunDef l (combineAttrs a acc.attrs) sp de st)
    let first ← pInitDeclTail n dstart false d
    let rest ← if ← accept .COMMA then pInitDeclarators n false else pure []
    let _ ← expectT .SEMICOLON
    return .EDecl_decl (.Declaration_base (combineAttrs (toAttrs attrs) acc.attrs) acc.specifiers (first :: rest))

/-- `external_declaration_list` until `EOF`. -/
def pExternalDecls : Nat → Array external_declaration → P (List external_declaration)
  | 0, _ => fuelOut
  | n+1, acc => do
    if ← at? .EOF then return acc.toList
    let d ← pExternalDecl n
    pExternalDecls n (acc.push d)

end

/-- `translation_unit`. -/
def pTranslationUnit (n : Nat) : P translation_unit := do
  let ds ← pExternalDecls n #[]
  let _ ← expectT .EOF
  return TUnit ds

end CSemCParser
