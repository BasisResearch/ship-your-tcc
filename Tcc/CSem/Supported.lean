import Cabs

/-!
# The constructs `Supported` excludes, found in a Cabs translation unit (F0)

DESIGN-COMPILER.md §5: the constructs tcc accepts that CSem gives no meaning.
`Tcc.CSem.excluded tu` lists every occurrence of one of them in the fork's
Cabs (`csem/lean_frontend/generated/Cabs.lean`), by a structural walk over
the whole mutual Cabs family (shaped like the generated `lemSize`). §5's rows,
and how each shows in Cabs:

| §5 row | in Cabs |
|---|---|
| inline assembly (statements, top level, asm labels) | `CabsSasm`; top-level `asm` and asm labels have no Cabs form: the fork's parser refuses them, so the front end fails |
| labels as values, computed `goto` | no Cabs form (the parser refuses `&&l` and `goto *e`) |
| `__builtin_frame_address`, `__builtin_return_address` | an identifier of that name |
| `_Atomic`, `__atomic_*` | `TSpec_Atomic`, `Q_Atomic`, an identifier starting `__atomic_` or `__sync_` |
| `section`/`visibility`/`weak`/`alias` attributes | an `attribute0` with that name (either spelling, `x` or `__x__`) |
| `_Complex` | `TSpec_Complex` |

Rows with no Cabs form are excluded by the front end refusing them, which
`Supported` records as the front end's acceptance (`Tcc.Supported`).
-/

namespace Tcc.CSem

/-- A construct of DESIGN-COMPILER.md §5's first table. -/
inductive Excluded where
  | asm
  | frameAddress (name : String)
  | atomic
  | attribute (name : String)
  | complex
  deriving Repr, BEq, DecidableEq

/-- §5: identifiers whose use is an excluded construct. -/
def excludedIdent (name : String) : List Excluded :=
  if name == "__builtin_frame_address" || name == "__builtin_return_address" then [.frameAddress name]
  else if name.startsWith "__atomic_" || name.startsWith "__sync_" then [.atomic]
  else []

/-- §5: attributes whose effect is linker-level. -/
def excludedAttrName (name : String) : Bool :=
  ["section", "visibility", "weak", "alias"].any fun a => name == a || name == s!"__{a}__"

def exclAttrs : attributes → List Excluded
  | .Attrs as => as.filterMap fun a => match a.attr_id with
    | .Identifier _ n => if excludedAttrName n then some (.attribute n) else none

def exclOptAttrs : Option attributes → List Excluded
  | none => []
  | some a => exclAttrs a

def exclIdent : identifier → List Excluded
  | .Identifier _ n => excludedIdent n

def exclTq : cabs_type_qualifier → List Excluded
  | .Q_Atomic => [.atomic]
  | _ => []

def exclTqs : List cabs_type_qualifier → List Excluded
  | [] => []
  | q :: qs => exclTq q ++ exclTqs qs

mutual
def exclExpr_ : cabs_expression_ → List Excluded
  | .CabsEident i => exclIdent i
  | .CabsEconst _ => []
  | .CabsEstring _ => []
  | .CabsEgeneric e gas => exclExpr e ++ exclGas gas
  | .CabsEsubscript e1 e2 => exclExpr e1 ++ exclExpr e2
  | .CabsEcall e es a => exclExpr e ++ exclExprs es ++ exclOptAttrs a
  | .CabsEmemberof e _ => exclExpr e
  | .CabsEmemberofptr e _ => exclExpr e
  | .CabsEpostincr e => exclExpr e
  | .CabsEpostdecr e => exclExpr e
  | .CabsEcompound t inits => exclTname t ++ exclInits inits
  | .CabsEpreincr e => exclExpr e
  | .CabsEpredecr e => exclExpr e
  | .CabsEunary _ e => exclExpr e
  | .CabsEsizeof_expr e => exclExpr e
  | .CabsEsizeof_type t => exclTname t
  | .CabsEalignof t => exclTname t
  | .CabsEcast t e => exclTname t ++ exclExpr e
  | .CabsEbinary _ e1 e2 => exclExpr e1 ++ exclExpr e2
  | .CabsEcond e1 e2 e3 => exclExpr e1 ++ exclExpr e2 ++ exclExpr e3
  | .CabsEassign _ e1 e2 => exclExpr e1 ++ exclExpr e2
  | .CabsEcomma e1 e2 => exclExpr e1 ++ exclExpr e2
  | .CabsEassert e => exclExpr e
  | .CabsEoffsetof t _ => exclTname t
  | .CabsEva_start e _ => exclExpr e
  | .CabsEva_copy e1 e2 => exclExpr e1 ++ exclExpr e2
  | .CabsEva_arg e t => exclExpr e ++ exclTname t
  | .CabsEva_end e => exclExpr e
  | .CabsEprint_type e => exclExpr e
  | .CabsEbmc_assume e => exclExpr e
  | .CabsEgcc_statement ss => exclStmts ss
  | .CabsEcondGNU e1 e2 => exclExpr e1 ++ exclExpr e2
  | .CabsEbuiltinGNU b => exclGnu b
def exclGnu : gnu_builtin_function → List Excluded
  | .GNUbuiltin_types_compatible_p t1 t2 => exclTname t1 ++ exclTname t2
  | .GNUbuiltin_choose_expr e1 e2 e3 => exclExpr e1 ++ exclExpr e2 ++ exclExpr e3
def exclExpr : cabs_expression → List Excluded
  | .CabsExpression _ e => exclExpr_ e
def exclGa : cabs_generic_association → List Excluded
  | .GA_type t e => exclTname t ++ exclExpr e
  | .GA_default e => exclExpr e
def exclDecl : cabs_declaration → List Excluded
  | .Declaration_base a s ids => exclAttrs a ++ exclSpecs s ++ exclInitDecls ids
  | .Declaration_static_assert sa => exclSassert sa
def exclSpecs : specifiers → List Excluded
  | .mk _ ts qs _ as => exclTspecs ts ++ exclTqs qs ++ exclAspecs as
def exclInitDecl : init_declarator → List Excluded
  | .InitDecl _ d i => exclDeclr d ++ exclOptInit i
def exclTspec_ : cabs_type_specifier_ → List Excluded
  | .TSpec_Complex => [.complex]
  | .TSpec_Atomic t => .atomic :: exclTname t
  | .TSpec_struct a _ sds => exclAttrs a ++ exclOptSdecls sds
  | .TSpec_union a _ sds => exclAttrs a ++ exclOptSdecls sds
  | .TSpec_enum _ es => exclOptEnums es
  | .TSpec_typeof_expr e => exclExpr e
  | .TSpec_typeof_type t => exclTname t
  | _ => []
def exclTspec : cabs_type_specifier → List Excluded
  | .TSpec _ t => exclTspec_ t
def exclSdecl : struct_declaration → List Excluded
  | .Struct_declaration a ts qs as sds => exclAttrs a ++ exclTspecs ts ++ exclTqs qs ++ exclAspecs as ++ exclSdeclrs sds
  | .Struct_assert sa => exclSassert sa
def exclSdeclr : struct_declarator → List Excluded
  | .SDecl_simple d => exclDeclr d
  | .SDecl_bitfield d e => exclOptDeclr d ++ exclExpr e
def exclAspec : alignment_specifier → List Excluded
  | .AS_type t => exclTname t
  | .AS_expr e => exclExpr e
def exclDeclr : declarator → List Excluded
  | .Declarator p dd => exclOptPdecl p ++ exclDdeclr dd
def exclDdeclr : direct_declarator → List Excluded
  | .DDecl_identifier a _ => exclAttrs a
  | .DDecl_declarator d => exclDeclr d
  | .DDecl_array dd ad => exclDdeclr dd ++ exclAdecl ad
  | .DDecl_function dd ps => exclDdeclr dd ++ exclPtl ps
def exclAdecl : array_declarator → List Excluded
  | .ADecl _ qs _ sz => exclTqs qs ++ exclOptAdeclSize sz
def exclAdeclSize : array_declarator_size → List Excluded
  | .ADeclSize_expression e => exclExpr e
  | .ADeclSize_asterisk => []
def exclPdecl : pointer_declarator → List Excluded
  | .PDecl _ qs p => exclTqs qs ++ exclOptPdecl p
def exclPtl : parameter_type_list → List Excluded
  | .Params ps _ => exclPdeclarations ps
def exclPdeclaration : parameter_declaration → List Excluded
  | .PDeclaration_decl s d => exclSpecs s ++ exclDeclr d
  | .PDeclaration_abs_decl s a => exclSpecs s ++ exclOptAbsdecl a
def exclTname : type_name → List Excluded
  | .Type_name ts qs as a => exclTspecs ts ++ exclTqs qs ++ exclAspecs as ++ exclOptAbsdecl a
def exclAbsdecl : abstract_declarator → List Excluded
  | .AbsDecl_pointer p => exclPdecl p
  | .AbsDecl_direct p d => exclOptPdecl p ++ exclDabsdecl d
def exclDabsdecl : direct_abstract_declarator → List Excluded
  | .DAbs_abs_declarator a => exclAbsdecl a
  | .DAbs_array d ad => exclOptDabsdecl d ++ exclAdecl ad
  | .DAbs_function d ps => exclOptDabsdecl d ++ exclPtl ps
def exclInit : initializer_ → List Excluded
  | .Init_expr e => exclExpr e
  | .Init_list _ inits => exclInits inits
def exclDesig : designator → List Excluded
  | .Desig_array e => exclExpr e
  | .Desig_member _ => []
def exclSassert : static_assert_declaration → List Excluded
  | .Static_assert e _ => exclExpr e
def exclStmt_ : cabs_statement_ → List Excluded
  | .CabsSlabel _ s => exclStmt s
  | .CabsScase e s => exclExpr e ++ exclStmt s
  | .CabsSdefault s => exclStmt s
  | .CabsSblock ss => exclStmts ss
  | .CabsSdecl d => exclDecl d
  | .CabsSnull => []
  | .CabsSexpr e => exclExpr e
  | .CabsSif e s1 s2 => exclExpr e ++ exclStmt s1 ++ exclOptStmt s2
  | .CabsSswitch e s => exclExpr e ++ exclStmt s
  | .CabsSwhile e s => exclExpr e ++ exclStmt s
  | .CabsSdo e s => exclExpr e ++ exclStmt s
  | .CabsSfor fc e1 e2 s => exclOptForc fc ++ exclOptExpr e1 ++ exclOptExpr e2 ++ exclStmt s
  | .CabsSgoto _ => []
  | .CabsScontinue => []
  | .CabsSbreak => []
  | .CabsSreturn e => exclOptExpr e
  | .CabsSpar ss => exclStmts ss
  | .CabsSasm _ _ _ => [.asm]
  | .CabsScaseGNU e1 e2 s => exclExpr e1 ++ exclExpr e2 ++ exclStmt s
  | .CabsSmarker s => exclStmt s
def exclStmt : cabs_statement → List Excluded
  | .CabsStatement _ a s => exclAttrs a ++ exclStmt_ s
def exclForc : for_clause → List Excluded
  | .FC_expr e => exclExpr e
  | .FC_decl _ d => exclDecl d
-- the nested containers, one walker each (as the generated `lemSize_aux*`)
def exclExprs : List cabs_expression → List Excluded
  | [] => []
  | e :: es => exclExpr e ++ exclExprs es
def exclGas : List cabs_generic_association → List Excluded
  | [] => []
  | g :: gs => exclGa g ++ exclGas gs
def exclInits : List (Option (List designator) × initializer_) → List Excluded
  | [] => []
  | (ds, i) :: rest => exclOptDesigs ds ++ exclInit i ++ exclInits rest
def exclOptDesigs : Option (List designator) → List Excluded
  | none => []
  | some ds => exclDesigs ds
def exclDesigs : List designator → List Excluded
  | [] => []
  | d :: ds => exclDesig d ++ exclDesigs ds
def exclStmts : List cabs_statement → List Excluded
  | [] => []
  | s :: ss => exclStmt s ++ exclStmts ss
def exclInitDecls : List init_declarator → List Excluded
  | [] => []
  | d :: ds => exclInitDecl d ++ exclInitDecls ds
def exclTspecs : List cabs_type_specifier → List Excluded
  | [] => []
  | t :: ts => exclTspec t ++ exclTspecs ts
def exclAspecs : List alignment_specifier → List Excluded
  | [] => []
  | a :: as => exclAspec a ++ exclAspecs as
def exclOptSdecls : Option (List struct_declaration) → List Excluded
  | none => []
  | some sds => exclSdecls sds
def exclSdecls : List struct_declaration → List Excluded
  | [] => []
  | s :: ss => exclSdecl s ++ exclSdecls ss
def exclOptEnums : Option (List (identifier × Option cabs_expression)) → List Excluded
  | none => []
  | some es => exclEnums es
def exclEnums : List (identifier × Option cabs_expression) → List Excluded
  | [] => []
  | (_, e) :: es => exclOptExpr e ++ exclEnums es
def exclSdeclrs : List struct_declarator → List Excluded
  | [] => []
  | s :: ss => exclSdeclr s ++ exclSdeclrs ss
def exclOptDeclr : Option declarator → List Excluded
  | none => []
  | some d => exclDeclr d
def exclOptPdecl : Option pointer_declarator → List Excluded
  | none => []
  | some p => exclPdecl p
def exclOptAdeclSize : Option array_declarator_size → List Excluded
  | none => []
  | some s => exclAdeclSize s
def exclPdeclarations : List parameter_declaration → List Excluded
  | [] => []
  | p :: ps => exclPdeclaration p ++ exclPdeclarations ps
def exclOptAbsdecl : Option abstract_declarator → List Excluded
  | none => []
  | some a => exclAbsdecl a
def exclOptDabsdecl : Option direct_abstract_declarator → List Excluded
  | none => []
  | some d => exclDabsdecl d
def exclOptInit : Option initializer_ → List Excluded
  | none => []
  | some i => exclInit i
def exclOptExpr : Option cabs_expression → List Excluded
  | none => []
  | some e => exclExpr e
def exclOptStmt : Option cabs_statement → List Excluded
  | none => []
  | some s => exclStmt s
def exclOptForc : Option for_clause → List Excluded
  | none => []
  | some f => exclForc f
end

def exclFundef : function_definition → List Excluded
  | .FunDef _ a s d body => exclAttrs a ++ exclSpecs s ++ exclDeclr d ++ exclStmt body

/-- External declarations; the CN-only forms (`EDecl_*CN`, magic comments)
carry no C code. -/
def exclExtDecl : external_declaration → List Excluded
  | .EDecl_func f => exclFundef f
  | .EDecl_decl d => exclDecl d
  | _ => []

/-- Every occurrence of a §5 construct in a translation unit. -/
def excluded : translation_unit → List Excluded
  | .TUnit ds => ds.flatMap exclExtDecl

end Tcc.CSem
