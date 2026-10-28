import Tcc.Model.Ghost

/-! Action-to-Cabs construction. This module never calls the specification
parser. It reads production children and their retained token ranges. In
particular it never attempts to recover a declarator from a folded CType.
Unimplemented syntax stays an error with its original range. -/
namespace Tcc.Model.Ghost
open CSemCParser CerbLocation

structure Context where
  input : Input
  production : Production
  range : Range
  children : Array Fragment

abbrev Build := Except Gap

def Context.fail (c : Context) (why : String) : Build α :=
  .error ⟨c.production, c.range, why⟩

def Context.token (c : Context) (i : Nat) : PTok :=
  c.input.language.positioned.tokens.getD i default

def Context.loc (c : Context) (r : Range) (cursor : Option Nat := none) : Loc :=
  let b := (c.token r.first).sp
  let e := if r.last > r.first then (c.token (r.last - 1)).ep
    else if r.first > 0 then (c.token (r.first - 1)).ep else b
  .region b e (match cursor with | none => .noCursor | some i => .pointCursor (c.token i).sp)

def Context.id (c : Context) (i : Nat) : Build identifier :=
  match (c.token i).tok with
  | .NAME s => .ok (Identifier (c.loc ⟨i,i+1⟩) s)
  | _ => c.fail "expected retained identifier"

/-- Expression/typedef references use Cabs point locations; declarations,
tags and member names use complete identifier regions. -/
def Context.pointId (c : Context) (i : Nat) : Build identifier := do
  match (c.token i).tok with
  | .NAME s => pure (Identifier (.point (c.token i).sp) s)
  | _ => c.fail "expected retained identifier reference"

/-- A declared name is identified by the modeled compiler action. The
independent spec lexer may classify builtin names differently; it does not
choose what the model parsed as a name. Canonical identity still preserves
UCN/UTF-8 spelling through the shared token-level identity conversion. -/
def Context.rawIdentifier (c : Context) (i : Nat) (point : Bool := false) : Build identifier := do
  let p ← match c.input.spelling[i]? with
    | some p => pure p | none => c.fail "missing declared-name spelling"
  if p.kind != .ident then return ← c.fail "declared name is not an identifier token"
  let canonical ← match CSemCParser.Phase7.canonicalIdentifier true p.spell with
    | .ok name => pure name | .error e => c.fail e
  let name := CSemCParser.Lex.slice ⟨canonical.toArray⟩ 0 canonical.length
  return Identifier (if point then .point (c.token i).sp else c.loc ⟨i,i+1⟩) name

def Context.values (c : Context) : Build (Array Syntax) :=
  c.children.mapM (fun f => f.value)

def expr : Syntax → Option cabs_expression
  | .expression e => some e | _ => none

def stmt : Syntax → Option cabs_statement
  | .statement s => some s
  | _ => none

def Context.expressions (c : Context) : Build (Array cabs_expression) := do
  let vs ← c.values
  vs.mapM (fun v => match expr v with
    | some e => pure e | none => c.fail "expression action has a non-expression child")

def Context.expressionAt (c : Context) (i : Nat) : Build (cabs_expression × Nat) := do
  match c.children.find? (fun f => f.range.first == i && (match f.value with
      | .ok (.expression _) => true | _ => false)) with
  | some f => match ← f.value with
    | .expression e => pure (e, f.range.last)
    | _ => c.fail "expression fragment kind"
  | none => c.fail s!"missing expression action at token {i}"

def unaryOperator : Production → Option cabs_unary_operator
  | .deref => some .CabsIndirection | .addr => some .CabsAddress
  | .lnot => some .CabsNot | .bnot => some .CabsBnot
  | .uplus => some .CabsPlus | .neg => some .CabsMinus | _ => none

def binaryOperator : Tok → Option cabs_binary_operator
  | .STAR => some .CabsMul | .SLASH => some .CabsDiv | .PERCENT => some .CabsMod
  | .PLUS => some .CabsAdd | .MINUS => some .CabsSub | .LT_LT => some .CabsShl
  | .GT_GT => some .CabsShr | .LT => some .CabsLt | .GT => some .CabsGt
  | .LT_EQ => some .CabsLe | .GT_EQ => some .CabsGe | .EQ_EQ => some .CabsEq
  | .BANG_EQ => some .CabsNe | .AMPERSAND => some .CabsBand | .CARET => some .CabsBxor
  | .PIPE => some .CabsBor | .AMPERSAND_AMPERSAND => some .CabsAnd
  | .PIPE_PIPE => some .CabsOr | _ => none

def assignmentOperator : Tok → Option cabs_assignment_operator
  | .EQ => some .Assign | .STAR_EQ => some .Assign_Mul | .SLASH_EQ => some .Assign_Div
  | .PERCENT_EQ => some .Assign_Mod | .PLUS_EQ => some .Assign_Add
  | .MINUS_EQ => some .Assign_Sub | .LT_LT_EQ => some .Assign_Shl | .GT_GT_EQ => some .Assign_Shr
  | .AMPERSAND_EQ => some .Assign_Band | .CARET_EQ => some .Assign_Bxor
  | .PIPE_EQ => some .Assign_Bor | _ => none

def qualifier : Tok → Option cabs_type_qualifier
  | .CONST => some .Q_const | .VOLATILE => some .Q_volatile
  | .RESTRICT => some .Q_restrict | .ATOMIC => some .Q_Atomic | _ => none

def scalarSpecifier : Tok → Option cabs_type_specifier_
  | .VOID => some .TSpec_void | .CHAR => some .TSpec_char | .SHORT => some .TSpec_short
  | .INT => some .TSpec_int | .LONG => some .TSpec_long | .FLOAT => some .TSpec_float
  | .DOUBLE => some .TSpec_double | .SIGNED => some .TSpec_signed
  | .UNSIGNED => some .TSpec_unsigned | .BOOL => some .TSpec_Bool
  | .COMPLEX => some .TSpec_Complex | _ => none

def storage : Tok → Option storage_class_specifier
  | .TYPEDEF => some .SC_typedef | .EXTERN => some .SC_extern | .STATIC => some .SC_static
  | .THREAD_LOCAL => some .SC_Thread_local | .AUTO => some .SC_auto
  | .REGISTER => some .SC_register | _ => none

def Context.rawSpelling (c : Context) (i : Nat) : String :=
  match c.input.spelling[i]? with
  | none => ""
  | some t => CSemCParser.Lex.slice ⟨t.spell.toArray⟩ 0 t.spell.length

/-- Fixed syntax projection: preserve the GNU attribute's raw identifier and
all argument tokens/locations in Cabs's generic attribute carrier. This does
not assert an alignment/cleanup/linkage meaning or erase unknown attributes.
The independent grammar/meaning connection is still an open F1 obligation. -/
def buildGnuAttributes (c : Context) : Build attributes := do
  let mut i := c.range.first
  let mut attrs : List attribute0 := []
  for _ in [0:c.range.last-c.range.first] do
    if i >= c.range.last then break
    if (c.token i).tok != .ATTRIBUTE || (c.token (i+1)).tok != .LPAREN ||
        (c.token (i+2)).tok != .LPAREN then return ← c.fail "GNU attribute group boundary"
    let ns ← c.rawIdentifier i
    i := i+3
    for _ in [0:c.range.last-c.range.first] do
      if (c.token i).tok == .RPAREN then break
      if (c.token i).tok == .COMMA then
        i := i+1
        continue
      let id ← c.rawIdentifier i
      i := i+1
      let mut args : List (Loc × String × List (Loc × String)) := []
      if (c.token i).tok == .LPAREN then
        i := i+1
        let mut first := i
        let mut depth := 0
        let mut tokens : List (Loc × String) := []
        for _ in [0:c.range.last-c.range.first] do
          if i >= c.range.last then return ← c.fail "unterminated GNU attribute argument"
          let t := (c.token i).tok
          if depth == 0 && (t == .RPAREN || t == .COMMA) then
            let loc := if i > first then c.loc ⟨first,first+1⟩
              else CerbLocation.Loc.region (c.token i).sp (c.token i).sp .noCursor
            args := args ++ [(loc,String.intercalate " " (tokens.map Prod.snd),tokens)]
            tokens := []
            i := i+1
            if t == .RPAREN then break
            first := i
          else
            if t == .LPAREN || t == .LBRACK || t == .LBRACE then depth := depth+1
            else if t == .RPAREN || t == .RBRACK || t == .RBRACE then depth := depth-1
            tokens := tokens ++ [(c.loc ⟨i,i+1⟩,c.rawSpelling i)]
            i := i+1
      attrs := attrs ++ [⟨some ns,id,args⟩]
      if (c.token i).tok == .COMMA then i := i+1
      else break
    if (c.token i).tok != .RPAREN || (c.token (i+1)).tok != .RPAREN then
      return ← c.fail "GNU attribute closing boundary"
    i := i+2
  if i != c.range.last then return ← c.fail "GNU attribute action range"
  return Attrs attrs

def Context.attributesAt (c : Context) (i : Nat) : Build (List attribute0 × Nat) := do
  match c.children.find? (fun f => f.production == .gnuAttrs && f.range.first == i) with
  | none => pure ([],i)
  | some f => match ← f.value with
    | .attributes (Attrs xs) => pure (xs,f.range.last)
    | _ => c.fail "attribute child kind"

/-- Recover a structured specifier only inside parse_btype's action, using
its member/declarator/expression children. Never infer syntax from layout. -/
def buildStructuredSpecifier (c : Context) (i : Nat) : Build (cabs_type_specifier × Nat) := do
  let keyword := (c.token i).tok
  let (attrsBefore,j0) ← c.attributesAt (i+1)
  let mut attrs := attrsBefore
  let mut j := j0
  let mut name := none
  if let .NAME _ := (c.token j).tok then name := some (← c.id j); j := j+1
  if keyword == .STRUCT || keyword == .UNION then
    let mut members := none
    if (c.token j).tok == .LBRACE then
      j := j+1
      let mut fields := []
      for _ in [0:c.range.last-i] do
        if (c.token j).tok == .RBRACE then break
        let sf ← match c.children.find? (fun f => f.production == .specifiers && f.range.first == j) with
          | some f => pure f | none => c.fail "missing aggregate-member specifier action"
        let sp ← match ← sf.value with
          | .specs sp => pure sp | _ => c.fail "member specifier kind"
        j := sf.range.last
        let mut ds := []
        for _ in [0:c.range.last-i] do
          if (c.token j).tok == .SEMICOLON then break
          let mut de := none
          if (c.token j).tok != .COLON then
            let df ← match c.children.find? (fun f => f.production == .declarator && f.range.first == j) with
              | some f => pure f | none => c.fail "missing member declarator action"
            match ← df.value with
            | .decl (.concrete d) => de := some d
            | _ => return ← c.fail "member declarator kind"
            j := df.range.last
          if (c.token j).tok == .COLON then
            let (e,k) ← c.expressionAt (j+1)
            ds := ds ++ [.SDecl_bitfield de e]; j := k
          else if let some d := de then ds := ds ++ [.SDecl_simple d]
          else return ← c.fail "missing member syntax"
          if (c.token j).tok == .COMMA then j := j+1
          else break
        if (c.token j).tok != .SEMICOLON then return ← c.fail "member declaration boundary"
        fields := fields ++ [Struct_declaration (Attrs []) sp.type_specifiers sp.type_qualifiers sp.alignment_specifiers ds]
        j := j+1
      if (c.token j).tok != .RBRACE then return ← c.fail "aggregate specifier boundary"
      members := some fields; j := j+1
    let (suffix,k) ← c.attributesAt j
    attrs := attrs ++ suffix; j := k
    let sp := if keyword == .STRUCT then .TSpec_struct (Attrs attrs) name members
      else .TSpec_union (Attrs attrs) name members
    return (TSpec (c.loc ⟨i,j⟩) sp,j)
  if keyword == .ENUM then
    if !attrs.isEmpty then return ← c.fail "enum attribute attachment remains explicit"
    let mut members := none
    if (c.token j).tok == .LBRACE then
      j := j+1
      let mut es := []
      for _ in [0:c.range.last-i] do
        if (c.token j).tok == .RBRACE then break
        let id ← c.id j
        j := j+1
        let mut v := none
        if (c.token j).tok == .EQ then
          let (e,k) ← c.expressionAt (j+1)
          v := some e; j := k
        es := es ++ [(id,v)]
        if (c.token j).tok == .COMMA then j := j+1
        else break
      if (c.token j).tok != .RBRACE then return ← c.fail "enum specifier boundary"
      members := some es; j := j+1
    return (TSpec (c.loc ⟨i,j⟩) (.TSpec_enum name members),j)
  c.fail "structured specifier needs typed construction"

/-- Specifier spelling/order survives tcc's merged bit mask. A NAME here is
known to be a typedef by the modeled parse_btype action, not by a second
feedback environment. -/
def buildSpecs (c : Context) : Build specifiers := do
  let mut s : specifiers := ⟨[], [], [], [], []⟩
  let mut i := c.range.first
  for _ in [0:c.range.last-c.range.first] do
    if i >= c.range.last then break
    let t := (c.token i).tok
    if let some f := c.children.find? (fun f => f.production == .typedefName && f.range.first == i) then
      match ← f.value with
      | .identifier id =>
        s := { s with type_specifiers := s.type_specifiers ++ [TSpec (c.loc f.range) (.TSpec_name id)] }
        i := f.range.last
        continue
      | _ => return ← c.fail "typedef-name action kind"
    if t == .STRUCT || t == .UNION || t == .ENUM then
      let (sp,j) ← buildStructuredSpecifier c i
      s := { s with type_specifiers := s.type_specifiers ++ [sp] }; i := j
      continue
    if let some x := scalarSpecifier t then
      s := { s with type_specifiers := s.type_specifiers ++ [TSpec (c.loc ⟨i,i+1⟩) x] }
    else if let some x := qualifier t then
      s := { s with type_qualifiers := s.type_qualifiers ++ [x] }
    else if let some x := storage t then
      s := { s with storage_classes := s.storage_classes ++ [x] }
    else if t == .INLINE then s := { s with function_specifiers := s.function_specifiers ++ [.FS_inline] }
    else if t == .NORETURN then s := { s with function_specifiers := s.function_specifiers ++ [.FS_Noreturn] }
    else if let .NAME _ := t then
      s := { s with type_specifiers := s.type_specifiers ++ [TSpec (c.loc ⟨i,i+1⟩) (.TSpec_name (← c.pointId i))] }
    else return ← c.fail s!"specifier action needs construction for token {i}"
    i := i+1
  if i != c.range.last then return ← c.fail "specifier boundary"
  pure s

/-! Pointer and direct-declarator structure is read only inside the range of
an actual modeled type_decl action. Parameters and array bounds must be
supplied by its modeled child actions; no expression/type spec parser runs. -/
mutual
 def buildDeclarator : Nat → Context → Nat → Nat → Build (ParsedDeclarator × Nat)
  | 0, c, _, _ => c.fail "declarator construction fuel"
  | fuel+1, c, first, stop => do
    if let some f := c.children.find? (fun f => f.production == .declarator && f.range.first == first && f.range.last <= stop) then
      match ← f.value with
      | .decl d => return (d, f.range.last)
      | _ => return ← c.fail "declarator child kind"
    let (ptr, i) ← buildPointer fuel c first stop
    let directStart := i
    let mut i := i
    let mut direct : Option direct_declarator := none
    let mut absDirect : Option direct_abstract_declarator := none
    if i < stop then
      if let some f := c.children.find? (fun f => f.production == .declName && f.range.first == i) then
        match ← f.value with
        | .identifier id =>
          direct := some (.DDecl_identifier (Attrs []) id)
          i := f.range.last
        | _ => return ← c.fail "declarator-name action kind"
      else
       match (c.token i).tok with
       | .NAME _ =>
         direct := some (.DDecl_identifier (Attrs []) (← c.id i)); i := i + 1
       | .LPAREN =>
         -- A child beginning after '(' is a parenthesized declarator. A
         -- parameter specifier child instead identifies an abstract function.
         let isParam := c.children.any (fun f => f.range.first == i+1 && f.production == .specifiers)
         if !isParam && (c.token (i+1)).tok != .RPAREN then
           let (d, j) ← buildDeclarator fuel c (i+1) stop
           if (c.token j).tok != .RPAREN then return ← c.fail "declarator parenthesis boundary"
           match d with
           | .concrete d => direct := some (.DDecl_declarator d)
           | .abstract (some d) => absDirect := some (.DAbs_abs_declarator d)
           | _ => return ← c.fail "empty parenthesized declarator"
           i := j+1
       | _ => pure ()
    let (d, a, j) ← buildSuffix fuel c directStart direct absDirect i stop
    match d, a, ptr with
    | some d, _, _ => pure (.concrete (declarator.Declarator ptr d), j)
    | none, some a, _ => pure (.abstract (some (.AbsDecl_direct ptr a)), j)
    | none, none, some p => pure (.abstract (some (.AbsDecl_pointer p)), j)
    | none, none, none => pure (.abstract none, j)

 def buildPointer : Nat → Context → Nat → Nat → Build (Option pointer_declarator × Nat)
  | 0, c, _, _ => c.fail "pointer construction fuel"
  | fuel+1, c, i, stop => do
    if i >= stop || (c.token i).tok != .STAR then return (none, i)
    let mut j := i+1
    let mut qs := []
    for _ in [0:stop-i] do
      if let some q := qualifier (c.token j).tok then qs := qs ++ [q]; j := j+1
      else break
    let (rest, k) ← buildPointer fuel c j stop
    pure (some (.PDecl (c.loc ⟨i,k⟩) qs rest), k)

 def buildSuffix : Nat → Context → Nat → Option direct_declarator → Option direct_abstract_declarator →
     Nat → Nat → Build (Option direct_declarator × Option direct_abstract_declarator × Nat)
  | 0, c, _, _, _, _, _ => c.fail "suffix construction fuel"
  | fuel+1, c, start, d, a, i, stop => do
    if i >= stop then return (d, a, i)
    match (c.token i).tok with
    | .LBRACK =>
      let mut j := i+1
      let mut qs := []
      let mut stat := false
      for _ in [0:stop-i] do
        if (c.token j).tok == .STATIC then stat := true; j := j+1
        else if let some q := qualifier (c.token j).tok then qs := qs ++ [q]; j := j+1
        else break
      let mut sz := none
      if (c.token j).tok == .STAR then sz := some .ADeclSize_asterisk; j := j+1
      else if (c.token j).tok != .RBRACK then
        let (e, k) ← c.expressionAt j
        sz := some (.ADeclSize_expression e); j := k
      if (c.token j).tok != .RBRACK then return ← c.fail "array boundary"
      let ar := ADecl (if d.isSome then c.loc ⟨start,j+1⟩ else .unknown)
        (if d.isSome then qs else qs.reverse) stat sz
      match d with
      | some d => buildSuffix fuel c start (some (.DDecl_array d ar)) none (j+1) stop
      | none => buildSuffix fuel c start none (some (.DAbs_array a ar)) (j+1) stop
    | .ATTRIBUTE =>
      let (xs,j) ← c.attributesAt i
      if j == i then return ← c.fail "missing GNU attribute parser action"
      match d with
      | some (.DDecl_identifier (Attrs old) id) =>
        buildSuffix fuel c start (some (.DDecl_identifier (Attrs (old++xs)) id)) none j stop
      | _ => c.fail "GNU suffix attribute placement needs a typed attachment"
    | .LPAREN =>
      let (ps, j) ← buildParams fuel c (i+1) stop
      match d with
      | some d => buildSuffix fuel c start (some (.DDecl_function d ps)) none j stop
      | none => buildSuffix fuel c start none (some (.DAbs_function a ps)) j stop
    | _ => pure (d, a, i)

 def buildParams : Nat → Context → Nat → Nat → Build (parameter_type_list × Nat)
  | 0, c, _, _ => c.fail "parameter construction fuel"
  | fuel+1, c, first, stop => do
    let mut i := first
    let mut ps := []
    let mut variadic := false
    for _ in [0:stop-first+1] do
      if (c.token i).tok == .RPAREN then return (Params ps variadic, i+1)
      if (c.token i).tok == .ELLIPSIS then
        variadic := true; i := i+1
        if (c.token i).tok != .RPAREN then return ← c.fail "ellipsis boundary"
        return (Params ps variadic, i+1)
      let sf ← match c.children.find? (fun f => f.production == .specifiers && f.range.first == i) with
        | some f => pure f | none => c.fail "K&R parameter or missing parameter specifier action"
      let sp ← match ← sf.value with
        | .specs s => pure s | _ => c.fail "parameter specifier kind"
      let (de, j) ← buildDeclarator fuel c sf.range.last stop
      let p := match de with
        | .concrete d => PDeclaration_decl sp d
        | .abstract a => PDeclaration_abs_decl sp a
      ps := ps ++ [p]
      i := j
      if (c.token i).tok == .COMMA then i := i+1
      else if (c.token i).tok != .RPAREN then return ← c.fail "parameter separator"
    c.fail "parameter boundary"
end

mutual
 def buildInitializer : Nat → Context → Nat → Nat → Build (initializer_ × Nat)
  | 0, c, _, _ => c.fail "initializer construction fuel"
  | fuel+1, c, i, stop => do
    if (c.token i).tok == .LBRACE then
      let (xs, j) ← buildInitItems fuel c (i+1) stop
      pure (Init_list (c.loc ⟨i,j⟩) xs, j)
    else
      let (e, j) ← c.expressionAt i
      pure (Init_expr e, j)

 def buildInitItems : Nat → Context → Nat → Nat → Build (List (Option (List designator) × initializer_) × Nat)
  | 0, c, _, _ => c.fail "initializer list construction fuel"
  | fuel+1, c, i, stop => do
    if i >= stop then return ← c.fail "unterminated initializer list"
    if (c.token i).tok == .RBRACE then return ([],i+1)
    let mut j := i
    let mut ds := []
    for _ in [0:stop-i] do
      if (c.token j).tok == .DOT then
        ds := ds ++ [.Desig_member (← c.id (j+1))]; j := j+2
      else if (c.token j).tok == .LBRACK then
        let (e,k) ← c.expressionAt (j+1)
        if (c.token k).tok != .RBRACK then return ← c.fail "designator boundary/range extension"
        ds := ds ++ [.Desig_array e]; j := k+1
      else break
    if !ds.isEmpty then
      if (c.token j).tok != .EQ then return ← c.fail "GNU missing-equals designator needs explicit normalization"
      j := j+1
    let (v,k) ← buildInitializer fuel c j stop
    let head := (if ds.isEmpty then none else some ds, v)
    if (c.token k).tok == .RBRACE then return ([head], k+1)
    if (c.token k).tok != .COMMA then return ← c.fail "initializer separator"
    let (xs,e) ← buildInitItems fuel c (k+1) stop
    pure (head::xs,e)
end

def Context.typeName (c : Context) : Build type_name := do
  let vs ← c.values
  let specs ← match vs.findSome? (fun v => match v with | .specs s => some s | _ => none) with
    | some s => pure s | none => c.fail "missing type-name specifier action"
  let d : Option abstract_declarator ← match vs.findSome? (fun v => match v with | .decl d => some d | _ => none) with
    | some (ParsedDeclarator.abstract a) => pure a
    | _ => c.fail "missing abstract declarator action"
  pure (Type_name specs.type_specifiers specs.type_qualifiers specs.alignment_specifiers d)

def Context.wrapExpression (c : Context) (e : cabs_expression_) (cursor : Option Nat := none) : Syntax :=
  .expression (CabsExpression (c.loc c.range cursor) e)

/-- Binary/logical/comma action children are in source order. Tcc folds a
whole logical chain at once; its source syntax is explicitly reconstructed
as the C grammar's left-associated chain, retaining every child expression. -/
def buildChain (c : Context) : Build Syntax := do
  let es ← c.expressions
  if es.size < 2 then return ← c.fail "operator action arity"
  let mut acc := es[0]!
  for i in [1:es.size] do
    let opIndex := c.children[i-1]!.range.last
    let endIndex := c.children[i]!.range.last
    let loc := c.loc ⟨c.range.first,endIndex⟩ (some opIndex)
    let next ← if c.production == .comma then pure (.CabsEcomma acc es[i]!)
      else if let some op := binaryOperator (c.token opIndex).tok then
        pure (.CabsEbinary op acc es[i]!)
      else c.fail s!"missing original binary operator at {opIndex}"
    acc := CabsExpression loc next
  pure (.expression acc)

def buildExpression (c : Context) : Build Syntax := do
  let start := c.range.first
  let t := c.token start
  if c.production == .ident then
    if !c.children.isEmpty then return ← c.fail "identifier has syntax children"
    return c.wrapExpression (.CabsEident (← c.pointId start))
  if c.production == .constant then
    if !c.children.isEmpty then return ← c.fail "constant has syntax children"
    match t.tok with
    | .CONSTANT v => return c.wrapExpression (.CabsEconst v)
    | _ => return ← c.fail "missing original constant spelling"
  if c.production == .string then
    let mut pref := none
    let mut parts := []
    for i in [start:c.range.last] do
      match (c.token i).tok with
      | .STRING_LITERAL p ss =>
        if p.isSome then
          if pref.isSome && pref != p then return ← c.fail "inconsistent string prefixes"
          pref := p
        parts := parts ++ [(c.loc ⟨i,i+1⟩, ss)]
      | _ => return ← c.fail "non-string in concatenation action"
    return c.wrapExpression (.CabsEstring (pref,parts))
  if c.production == .binop || c.production == .logop || c.production == .comma then
    return ← buildChain c
  if c.production == .cast then
    let ty ← c.typeName
    let vs ← c.values
    match vs.back? with
    | some (.expression e) => return c.wrapExpression (.CabsEcast ty e) (some start)
    | _ => return ← c.fail "cast operand action"
  if c.production == .sizeof || c.production == .alignof then
    let vs ← c.values
    if c.production == .sizeof then
      match vs.toList with
      | [.expression e] => return c.wrapExpression (.CabsEsizeof_expr e) (some start)
      | _ => pure ()
    let ty ← match vs.toList with
      | [.typeName ty] => pure ty
      | _ => c.typeName
    return c.wrapExpression (if c.production == .sizeof then .CabsEsizeof_type ty else .CabsEalignof ty) (some start)
  if c.production == .type then return .typeName (← c.typeName)
  if c.production == .stmtexpr then
    let vs ← c.values
    match vs.toList with
    | [.statement (CabsStatement _ _ (.CabsSblock ss))] =>
      return c.wrapExpression (.CabsEgcc_statement ss) (some (start+1))
    | _ => return ← c.fail "statement expression body action"
  let es ← c.expressions
  if let some op := unaryOperator c.production then
    if es.size != 1 then return ← c.fail "unary action arity"
    return c.wrapExpression (.CabsEunary op es[0]!) (some start)
  match c.production, es.toList with
  | .paren, [CabsExpression _ e] => pure (c.wrapExpression e)
  | .preinc, [e] => pure (c.wrapExpression (.CabsEpreincr e) (some start))
  | .predec, [e] => pure (c.wrapExpression (.CabsEpredecr e) (some start))
  | .postinc, [e] => pure (c.wrapExpression (.CabsEpostincr e) (some c.children[0]!.range.last))
  | .postdec, [e] => pure (c.wrapExpression (.CabsEpostdecr e) (some c.children[0]!.range.last))
  | .call, f::args => pure (c.wrapExpression (.CabsEcall f args none))
  | .index, [a,i] => pure (c.wrapExpression (.CabsEsubscript a i))
  | .member, [e] => pure (c.wrapExpression (.CabsEmemberof e (← c.id (c.range.last-1))) (some c.children[0]!.range.last))
  | .arrow, [e] => pure (c.wrapExpression (.CabsEmemberofptr e (← c.id (c.range.last-1))) (some c.children[0]!.range.last))
  | .assign, [a,b] =>
    let i := c.children[0]!.range.last
    match assignmentOperator (c.token i).tok with
    | some op => pure (c.wrapExpression (.CabsEassign op a b) (some i))
    | none => c.fail "assignment operator spelling"
  | .cond, [a,b,d] => pure (c.wrapExpression (.CabsEcond a b d) (some c.children[0]!.range.last))
  | .condGNU, [a,b] => pure (c.wrapExpression (.CabsEcondGNU a b) (some c.children[0]!.range.last))
  | _, _ => c.fail "expression action needs typed construction"

/-- Complete label headers in source order. A dangling label is an explicit
missing construction, not an inserted null statement. -/
def buildBlockItems (c : Context) (vs : Array Syntax) : Build (List cabs_statement) := do
  let mut out : List cabs_statement := []
  let mut labels : List (Range × Label) := []
  for i in [0:vs.size] do
    let r := c.children[i]!.range
    match vs[i]! with
    | .label l => labels := (r,l)::labels
    | v =>
      let mut s ← match v with
        | .statement s => pure s
        | .declaration d => pure (CabsStatement (c.loc r) (Attrs []) (.CabsSdecl d))
        | _ => c.fail "non-statement action in block"
      for (lr,l) in labels do
        let body := match l with
          | .named id => .CabsSlabel id s
          | .caseValue lo none => .CabsScase lo s
          | .caseValue lo (some hi) => .CabsScaseGNU lo hi s
          | .defaultValue => .CabsSdefault s
        s := CabsStatement (c.loc ⟨lr.first,r.last⟩) (Attrs []) body
      labels := []
      out := s::out
  if !labels.isEmpty then return ← c.fail "label without a following statement"
  pure out.reverse

def buildStatement (c : Context) : Build Syntax := do
  let vs ← c.values
  let ex := vs.filterMap expr
  let ss := vs.filterMap stmt
  let loc := c.loc c.range
  let wrap := fun s => Syntax.statement (CabsStatement loc (Attrs []) s)
  match c.production with
  | .block => return wrap (.CabsSblock (← buildBlockItems c vs))
  | .labelled =>
    let ss ← buildBlockItems c vs
    match ss with
    | [s] => return .statement s
    | _ => return ← c.fail "labelled-statement action arity"
  | .caseStmt =>
    match ex.toList with
    | [lo] => return .label (.caseValue lo none)
    | [lo,hi] => return .label (.caseValue lo (some hi))
    | _ => return ← c.fail "case expression action"
  | .defaultStmt =>
    if !vs.isEmpty then return ← c.fail "default action arity"
    return .label .defaultValue
  | .label =>
    if !vs.isEmpty then return ← c.fail "label action arity"
    return .label (.named (← c.id c.range.first))
  | .gotoStmt =>
    if !vs.isEmpty then return ← c.fail "goto action arity"
    return wrap (.CabsSgoto (← c.id (c.range.first+1)))
  | .returnStmt =>
    match vs.toList with
    | [] => return wrap (.CabsSreturn none)
    | [.expression e] => return wrap (.CabsSreturn (some e))
    | _ => return ← c.fail "return action arity"
  | .breakStmt | .continueStmt | .empty =>
    if !vs.isEmpty then return ← c.fail "jump/null action arity"
    if c.production == .empty then
      let b := if c.range.first == 0 then c.input.language.positioned.start
        else (c.token (c.range.first-1)).ep
      return .statement (CabsStatement (.region b (c.token (c.range.last-1)).ep .noCursor) (Attrs []) .CabsSnull)
    return wrap (if c.production == .breakStmt then .CabsSbreak else .CabsScontinue)
  | .exprStmt =>
    match vs.toList with
    | [.expression e] => return wrap (.CabsSexpr e)
    | _ => return ← c.fail "expression statement action arity"
  | .ifStmt =>
    match vs.toList with
    | [.expression e,.statement s] => return wrap (.CabsSif e s none)
    | [.expression e,.statement s,.statement d] => return wrap (.CabsSif e s (some d))
    | _ => return ← c.fail "if action arity"
  | .whileStmt | .switchStmt =>
    match vs.toList with
    | [.expression e,.statement s] =>
      return wrap (if c.production == .whileStmt then .CabsSwhile e s else .CabsSswitch e s)
    | _ => return ← c.fail "while/switch action arity"
  | .doStmt =>
    match vs.toList with
    | [.statement s,.expression e] => return wrap (.CabsSdo e s)
    | _ => return ← c.fail "do action arity"
  | .forStmt =>
    if ss.size != 1 then return ← c.fail "for body action"
    -- The three optional clauses retain their original token slots.
    let mut semis : Array Nat := #[]
    let mut depth := 0
    for i in [c.range.first+2:c.range.last] do
      let t := (c.token i).tok
      if t == .LPAREN || t == .LBRACK || t == .LBRACE then depth := depth+1
      else if t == .RPAREN then
        if depth == 0 then break else depth := depth-1
      else if t == .RBRACK || t == .RBRACE then depth := depth-1
      else if t == .SEMICOLON && depth == 0 then semis := semis.push i
    if semis.size != 2 then return ← c.fail "for clause boundaries"
    let mut init := none
    let mut cond := none
    let mut step := none
    for i in [0:vs.size] do
      let r := c.children[i]!.range
      match vs[i]! with
      | .declaration d => init := some (.FC_decl (c.loc ⟨r.first,semis[0]!+1⟩) d)
      | .expression e =>
        if r.first < semis[0]! then init := some (.FC_expr e)
        else if r.first < semis[1]! then cond := some e
        else step := some e
      | .statement _ => pure ()
      | _ => return ← c.fail "for clause action kind"
    return wrap (.CabsSfor init cond step ss[0]!)
  | _ => c.fail "statement action needs typed construction"

/-- Fixed asm-label carrier. It retains the raw asm keyword and every
literal token/location as a generic attribute on the declared identifier.
This is syntax data, not a claim that CSem already implements that binding.
NormalizationAgreement_Statement owes the independent syntax/meaning link. -/
def asmBindingAttribute (c : Context) (r : Range) : Build attribute0 := do
  let ns ← c.rawIdentifier r.first
  let id := Identifier (c.loc ⟨r.first,r.first+1⟩) "asm_label"
  let first := r.first+2
  let last := r.last-1
  let tokens := (List.range (last-first)).map (fun k =>
    (c.loc ⟨first+k,first+k+1⟩,c.rawSpelling (first+k)))
  pure ⟨some ns,id,[(c.loc ⟨first,first+1⟩,String.intercalate " " (tokens.map Prod.snd),tokens)]⟩

mutual
 def addDeclaratorAttribute (d : declarator) (attr : attribute0) : declarator :=
  match d with | .Declarator p dd => .Declarator p (addDirectAttribute dd attr)
 def addDirectAttribute (d : direct_declarator) (attr : attribute0) : direct_declarator :=
  match d with
  | .DDecl_identifier (Attrs xs) id => .DDecl_identifier (Attrs (xs++[attr])) id
  | .DDecl_declarator d => .DDecl_declarator (addDeclaratorAttribute d attr)
  | .DDecl_array d a => .DDecl_array (addDirectAttribute d attr) a
  | .DDecl_function d ps => .DDecl_function (addDirectAttribute d attr) ps
end

def build (c : Context) : Build Syntax := do
  match c.production with
  | .declName => return .identifier (← c.rawIdentifier c.range.first)
  | .typedefName => return .identifier (← c.rawIdentifier c.range.first true)
  | .gnuAttrs => return .attributes (← buildGnuAttributes c)
  | .asmLabel =>
    let value ← buildExpression { c with production := .string, range := ⟨c.range.first+2,c.range.last-1⟩, children := #[] }
    match value with
    | .expression (CabsExpression _ (.CabsEstring literal)) => return .asmBinding literal
    | _ => return ← c.fail "asm binding string construction"
  | .specifiers => return .specs (← buildSpecs c)
  | .declarator =>
    let (d,j) ← buildDeclarator (4*(c.range.last-c.range.first+2)) c c.range.first c.range.last
    if j != c.range.last then return ← c.fail s!"unconsumed declarator tokens at {j}"
    return .decl d
  | .initializer =>
    let (v,j) ← buildInitializer (4*(c.range.last-c.range.first+2)) c c.range.first c.range.last
    if j != c.range.last then return ← c.fail "unconsumed initializer syntax"
    return .initializer v
  | .initDecl =>
    let vs ← c.values
    match vs.toList with
    | [.decl (.concrete d)] =>
      return .initDecl (InitDecl (c.loc c.children[0]!.range) d none)
    | [.decl (.concrete d), .initializer v] =>
      return .initDecl (InitDecl (c.loc ⟨c.children[0]!.range.first,c.children[1]!.range.last⟩) d (some v))
    | [.decl (.concrete d), .functionBody s] => return .functionDeclarator d s
    | [.decl (.concrete d), .asmBinding _] =>
      let attr ← asmBindingAttribute c c.children[1]!.range
      return .initDecl (InitDecl (c.loc ⟨c.children[0]!.range.first,c.children[1]!.range.last⟩)
        (addDeclaratorAttribute d attr) none)
    | [.decl (.concrete d), .asmBinding _, .initializer v] =>
      let attr ← asmBindingAttribute c c.children[1]!.range
      return .initDecl (InitDecl (c.loc ⟨c.children[0]!.range.first,c.children[2]!.range.last⟩)
        (addDeclaratorAttribute d attr) (some v))
    | _ => return ← c.fail "init-declarator action children"
  | .declaration =>
    let vs ← c.values
    match vs.toList with
    | .specs sp :: [.functionDeclarator d s] =>
      return .external (.EDecl_func (FunDef (c.loc c.range) (Attrs []) sp d s))
    | .specs sp :: ids =>
      let ids ← ids.mapM (fun v => match v with
        | .initDecl i => pure i | _ => c.fail "declaration action child")
      return .declaration (.Declaration_base (Attrs []) sp ids)
    | _ => return ← c.fail "missing declaration specifiers (including implicit-int normalization)"
  | .fundef =>
    let vs ← c.values
    match vs.toList with
    | [.statement s] => return .functionBody s
    | _ => return ← c.fail "function body action children (VLA/inline replay needs construction)"
  | .tu =>
    let vs ← c.values
    let es ← vs.toList.mapM (fun v => match v with
      | .external e => pure e | .declaration d => pure (.EDecl_decl d)
      | _ => c.fail "translation unit has non-external action")
    return .translationUnit (TUnit es)
  | .block | .labelled | .caseStmt | .defaultStmt | .label | .gotoStmt | .returnStmt
  | .breakStmt | .continueStmt | .empty | .exprStmt | .ifStmt | .whileStmt | .switchStmt
  | .doStmt | .forStmt | .asmStmt => buildStatement c
  | .deferredFunction => c.fail "deferred inline function syntax awaits parser replay attachment"
  | .localLabels => c.fail "GNU local-label declaration requires a Cabs extension"
  | .staticAssert => c.fail "static assertion construction remains explicit"
  | _ => buildExpression c

end Tcc.Model.Ghost
