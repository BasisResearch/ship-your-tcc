import Lean.Data.Json
import Cabs

/-! # Cabs → JSON, in the schema of the oracle's printer

`backend/lean_export/cabs_json.ml` (the OCaml printer behind `cerberus
--cabs-json`), as `CabsImport.lean` reads it: the inverse of `CabsImport`.
Used to compare the Lean parser's Cabs with menhir's, and to hand a Lean-parsed
translation unit to the fork's batch driver unchanged.

Byte-carrier strings (string-literal fragments, character constants, asm
parts) are written as the Lean strings they are (one code point per byte),
which is what `json_of_bytes` writes; identifiers, file names and attribute
strings are text. -/

namespace CSemCParser.CabsExport

open Lean (Json)
open CerbLocation

def tag (name : String) (fields : List (String × Json)) : Json :=
  Json.mkObj (("tag", Json.str name) :: fields)

def opt {α : Type} (f : α → Json) : Option α → Json
  | none => Json.null
  | some a => f a

def list {α : Type} (f : α → Json) (xs : List α) : Json := Json.arr (xs.map f).toArray

def nat (n : Nat) : Json := Json.num (n : Int)

def pos (p : Pos) : Json :=
  Json.mkObj [("file", Json.str p.file), ("line", nat p.line), ("col", nat p.col),
              ("lexing_file", Json.str p.lexingFile), ("lexing_line", nat p.lexingLine)]

def cursor : Cursor → Json
  | .noCursor => Json.str "NoCursor"
  | .pointCursor p => tag "PointCursor" [("pos", pos p)]
  | .regionCursor b e => tag "RegionCursor" [("begin", pos b), ("end", pos e)]

def loc : Loc → Json
  | .unknown => Json.str "Loc_unknown"
  | .other s => tag "Loc_other" [("str", Json.str s)]
  | .point p => tag "Loc_point" [("pos", pos p)]
  | .region b e c => tag "Loc_region" [("begin", pos b), ("end", pos e), ("cursor", cursor c)]
  | .regions rs c => tag "Loc_regions"
      [("regions", list (fun (b, e) => Json.arr #[pos b, pos e]) rs), ("cursor", cursor c)]

def ident : identifier → Json
  | Identifier l s => Json.mkObj [("loc", loc l), ("name", Json.str s)]

def intSuffix : cabs_integer_suffix → Json
  | .CabsSuffix_U => .str "CabsSuffix_U" | .CabsSuffix_UL => .str "CabsSuffix_UL"
  | .CabsSuffix_ULL => .str "CabsSuffix_ULL" | .CabsSuffix_L => .str "CabsSuffix_L"
  | .CabsSuffix_LL => .str "CabsSuffix_LL"

def floatSuffix : cabs_floating_suffix → Json
  | .CabsFloatingSuffix_F => .str "CabsFloatingSuffix_F"
  | .CabsFloatingSuffix_L => .str "CabsFloatingSuffix_L"

def charPrefix : cabs_character_prefix → Json
  | .CabsPrefix_L => .str "CabsPrefix_L" | .CabsPrefix_u => .str "CabsPrefix_u"
  | .CabsPrefix_U => .str "CabsPrefix_U"

def encPrefix : cabs_encoding_prefix → Json
  | .CabsEncPrefix_u8 => .str "CabsEncPrefix_u8" | .CabsEncPrefix_u => .str "CabsEncPrefix_u"
  | .CabsEncPrefix_U => .str "CabsEncPrefix_U" | .CabsEncPrefix_L => .str "CabsEncPrefix_L"

def constant : cabs_constant → Json
  | .CabsInteger_const (s, sfx) => tag "CabsInteger_const" [("val", Json.arr #[.str s, opt intSuffix sfx])]
  | .CabsFloating_const (s, sfx) => tag "CabsFloating_const" [("val", Json.arr #[.str s, opt floatSuffix sfx])]
  | .CabsCharacter_const (p, s) => tag "CabsCharacter_const" [("val", Json.arr #[opt charPrefix p, .str s])]

def stringLiteral (sl : cabs_string_literal) : Json :=
  Json.arr #[opt encPrefix sl.1,
    list (fun (l, strs) => Json.arr #[loc l, list Json.str strs]) sl.2]

def unaryOp : cabs_unary_operator → Json
  | .CabsAddress => .str "CabsAddress" | .CabsIndirection => .str "CabsIndirection"
  | .CabsPlus => .str "CabsPlus" | .CabsMinus => .str "CabsMinus" | .CabsBnot => .str "CabsBnot"
  | .CabsNot => .str "CabsNot"

def binaryOp : cabs_binary_operator → Json
  | .CabsMul => .str "CabsMul" | .CabsDiv => .str "CabsDiv" | .CabsMod => .str "CabsMod"
  | .CabsAdd => .str "CabsAdd" | .CabsSub => .str "CabsSub" | .CabsShl => .str "CabsShl"
  | .CabsShr => .str "CabsShr" | .CabsLt => .str "CabsLt" | .CabsGt => .str "CabsGt"
  | .CabsLe => .str "CabsLe" | .CabsGe => .str "CabsGe" | .CabsEq => .str "CabsEq"
  | .CabsNe => .str "CabsNe" | .CabsBand => .str "CabsBand" | .CabsBxor => .str "CabsBxor"
  | .CabsBor => .str "CabsBor" | .CabsAnd => .str "CabsAnd" | .CabsOr => .str "CabsOr"

def assignOp : cabs_assignment_operator → Json
  | .Assign => .str "Assign" | .Assign_Mul => .str "Assign_Mul" | .Assign_Div => .str "Assign_Div"
  | .Assign_Mod => .str "Assign_Mod" | .Assign_Add => .str "Assign_Add"
  | .Assign_Sub => .str "Assign_Sub" | .Assign_Shl => .str "Assign_Shl"
  | .Assign_Shr => .str "Assign_Shr" | .Assign_Band => .str "Assign_Band"
  | .Assign_Bxor => .str "Assign_Bxor" | .Assign_Bor => .str "Assign_Bor"

def storageClass : storage_class_specifier → Json
  | .SC_typedef => .str "SC_typedef" | .SC_extern => .str "SC_extern" | .SC_static => .str "SC_static"
  | .SC_Thread_local => .str "SC_Thread_local" | .SC_auto => .str "SC_auto"
  | .SC_register => .str "SC_register"

def typeQualifier : cabs_type_qualifier → Json
  | .Q_const => .str "Q_const" | .Q_restrict => .str "Q_restrict" | .Q_volatile => .str "Q_volatile"
  | .Q_Atomic => .str "Q_Atomic"

def functionSpecifier : function_specifier → Json
  | .FS_inline => .str "FS_inline" | .FS_Noreturn => .str "FS_Noreturn"

def attr (a : attribute0) : Json :=
  Json.mkObj [("tag", .str "Attribute"), ("ns", opt ident a.attr_ns), ("id", ident a.attr_id),
    ("args", list (fun (l, s, toks) =>
      Json.arr #[loc l, .str s, list (fun (l', s') => Json.arr #[loc l', .str s']) toks]) a.attr_args)]

def attributes : attributes → Json
  | Attrs xs => tag "Attrs" [("attrs", list attr xs)]

mutual

def expr : cabs_expression → Json
  | CabsExpression l e => tag "CabsExpression" [("loc", loc l), ("expr", expr_ e)]

def exprList : List cabs_expression → List Json
  | [] => []
  | e :: es => expr e :: exprList es

def expr_ : cabs_expression_ → Json
  | .CabsEident i => tag "CabsEident" [("id", ident i)]
  | .CabsEconst c => tag "CabsEconst" [("val", constant c)]
  | .CabsEstring s => tag "CabsEstring" [("val", stringLiteral s)]
  | .CabsEgeneric e gas => tag "CabsEgeneric" [("expr", expr e), ("assocs", Json.arr (genericAssocs gas).toArray)]
  | .CabsEsubscript a i => tag "CabsEsubscript" [("arr", expr a), ("idx", expr i)]
  | .CabsEcall f args attrs => tag "CabsEcall"
      [("fun", expr f), ("args", Json.arr (exprList args).toArray), ("attrs", opt attributes attrs)]
  | .CabsEmemberof e m => tag "CabsEmemberof" [("expr", expr e), ("member", ident m)]
  | .CabsEmemberofptr e m => tag "CabsEmemberofptr" [("expr", expr e), ("member", ident m)]
  | .CabsEpostincr e => tag "CabsEpostincr" [("expr", expr e)]
  | .CabsEpostdecr e => tag "CabsEpostdecr" [("expr", expr e)]
  | .CabsEcompound ty inits => tag "CabsEcompound" [("type", typeName ty), ("inits", Json.arr (initList inits).toArray)]
  | .CabsEpreincr e => tag "CabsEpreincr" [("expr", expr e)]
  | .CabsEpredecr e => tag "CabsEpredecr" [("expr", expr e)]
  | .CabsEunary op e => tag "CabsEunary" [("op", unaryOp op), ("expr", expr e)]
  | .CabsEsizeof_expr e => tag "CabsEsizeof_expr" [("expr", expr e)]
  | .CabsEsizeof_type ty => tag "CabsEsizeof_type" [("type", typeName ty)]
  | .CabsEalignof ty => tag "CabsEalignof" [("type", typeName ty)]
  | .CabsEcast ty e => tag "CabsEcast" [("type", typeName ty), ("expr", expr e)]
  | .CabsEbinary op l r => tag "CabsEbinary" [("op", binaryOp op), ("lhs", expr l), ("rhs", expr r)]
  | .CabsEcond c t e => tag "CabsEcond" [("cond", expr c), ("then_", expr t), ("else_", expr e)]
  | .CabsEassign op l r => tag "CabsEassign" [("op", assignOp op), ("lhs", expr l), ("rhs", expr r)]
  | .CabsEcomma l r => tag "CabsEcomma" [("lhs", expr l), ("rhs", expr r)]
  | .CabsEassert e => tag "CabsEassert" [("expr", expr e)]
  | .CabsEoffsetof ty m => tag "CabsEoffsetof" [("type", typeName ty), ("member", ident m)]
  | .CabsEva_start e p => tag "CabsEva_start" [("expr", expr e), ("param", ident p)]
  | .CabsEva_copy d s => tag "CabsEva_copy" [("dst", expr d), ("src", expr s)]
  | .CabsEva_arg e ty => tag "CabsEva_arg" [("expr", expr e), ("type", typeName ty)]
  | .CabsEva_end e => tag "CabsEva_end" [("expr", expr e)]
  | .CabsEprint_type e => tag "CabsEprint_type" [("expr", expr e)]
  | .CabsEbmc_assume e => tag "CabsEbmc_assume" [("expr", expr e)]
  | .CabsEgcc_statement ss => tag "CabsEgcc_statement" [("stmts", Json.arr (stmtList ss).toArray)]
  | .CabsEcondGNU c e => tag "CabsEcondGNU" [("cond", expr c), ("else_", expr e)]
  | .CabsEbuiltinGNU b => tag "CabsEbuiltinGNU" [("builtin", gnuBuiltin b)]

def gnuBuiltin : gnu_builtin_function → Json
  | .GNUbuiltin_types_compatible_p a b => tag "GNUbuiltin_types_compatible_p" [("type1", typeName a), ("type2", typeName b)]
  | .GNUbuiltin_choose_expr c t e => tag "GNUbuiltin_choose_expr" [("cond", expr c), ("then_", expr t), ("else_", expr e)]

def genericAssocs : List cabs_generic_association → List Json
  | [] => []
  | .GA_type ty e :: gs => tag "GA_type" [("type", typeName ty), ("expr", expr e)] :: genericAssocs gs
  | .GA_default e :: gs => tag "GA_default" [("expr", expr e)] :: genericAssocs gs

def initList : List (Option (List designator) × initializer_) → List Json
  | [] => []
  | (ds, i) :: rest => Json.arr #[(match ds with | none => Json.null | some ds => Json.arr (designators ds).toArray), initializer i]
      :: initList rest

def designators : List designator → List Json
  | [] => []
  | .Desig_array e :: ds => tag "Desig_array" [("expr", expr e)] :: designators ds
  | .Desig_member m :: ds => tag "Desig_member" [("member", ident m)] :: designators ds

def initializer : initializer_ → Json
  | .Init_expr e => tag "Init_expr" [("expr", expr e)]
  | .Init_list l inits => tag "Init_list" [("loc", loc l), ("inits", Json.arr (initList inits).toArray)]

def alignSpec : alignment_specifier → Json
  | .AS_type ty => tag "AS_type" [("type", typeName ty)]
  | .AS_expr e => tag "AS_expr" [("expr", expr e)]

def alignSpecs : List alignment_specifier → List Json
  | [] => []
  | a :: as => alignSpec a :: alignSpecs as

def typeSpec : cabs_type_specifier → Json
  | TSpec l s => tag "TSpec" [("loc", loc l), ("spec", typeSpec_ s)]

def typeSpecs : List cabs_type_specifier → List Json
  | [] => []
  | t :: ts => typeSpec t :: typeSpecs ts

def typeSpec_ : cabs_type_specifier_ → Json
  | .TSpec_void => .str "TSpec_void" | .TSpec_char => .str "TSpec_char"
  | .TSpec_short => .str "TSpec_short" | .TSpec_int => .str "TSpec_int"
  | .TSpec_long => .str "TSpec_long" | .TSpec_float => .str "TSpec_float"
  | .TSpec_double => .str "TSpec_double" | .TSpec_signed => .str "TSpec_signed"
  | .TSpec_unsigned => .str "TSpec_unsigned" | .TSpec_Bool => .str "TSpec_Bool"
  | .TSpec_Complex => .str "TSpec_Complex"
  | .TSpec_Atomic ty => tag "TSpec_Atomic" [("type", typeName ty)]
  | .TSpec_struct a i ms => tag "TSpec_struct"
      [("attrs", attributes a), ("id", opt ident i), ("members", match ms with | none => .null | some ms => Json.arr (structDecls ms).toArray)]
  | .TSpec_union a i ms => tag "TSpec_union"
      [("attrs", attributes a), ("id", opt ident i), ("members", match ms with | none => .null | some ms => Json.arr (structDecls ms).toArray)]
  | .TSpec_enum i es => tag "TSpec_enum"
      [("id", opt ident i), ("enumerators", match es with | none => .null | some es => Json.arr (enumerators es).toArray)]
  | .TSpec_name i => tag "TSpec_name" [("id", ident i)]
  | .TSpec_typeof_expr e => tag "TSpec_typeof_expr" [("expr", expr e)]
  | .TSpec_typeof_type ty => tag "TSpec_typeof_type" [("type", typeName ty)]

def enumerators : List (identifier × Option cabs_expression) → List Json
  | [] => []
  | (i, e) :: es => Json.arr #[ident i, (match e with | none => .null | some e => expr e)] :: enumerators es

def structDecls : List struct_declaration → List Json
  | [] => []
  | .Struct_declaration a ts tq al ds :: rest => tag "Struct_declaration"
      [("attrs", attributes a), ("type_specifiers", Json.arr (typeSpecs ts).toArray),
       ("type_qualifiers", list typeQualifier tq), ("alignment_specifiers", Json.arr (alignSpecs al).toArray),
       ("declarators", Json.arr (structDeclarators ds).toArray)] :: structDecls rest
  | .Struct_assert sa :: rest => tag "Struct_assert" [("assert", staticAssert sa)] :: structDecls rest

def structDeclarators : List struct_declarator → List Json
  | [] => []
  | .SDecl_simple d :: rest => tag "SDecl_simple" [("declarator", declarator d)] :: structDeclarators rest
  | .SDecl_bitfield d w :: rest => tag "SDecl_bitfield"
      [("declarator", match d with | none => .null | some d => declarator d), ("width", expr w)] :: structDeclarators rest

def pointerDecl : pointer_declarator → Json
  | PDecl l tq p => tag "PDecl" [("loc", loc l), ("type_qualifiers", list typeQualifier tq),
      ("pointer", match p with | none => .null | some p => pointerDecl p)]

def arrayDecl : array_declarator → Json
  | ADecl l tq st sz => tag "ADecl" [("loc", loc l), ("type_qualifiers", list typeQualifier tq),
      ("is_static", .bool st), ("size", match sz with
        | none => .null
        | some (.ADeclSize_expression e) => tag "ADeclSize_expression" [("expr", expr e)]
        | some .ADeclSize_asterisk => .str "ADeclSize_asterisk")]

def paramTypeList : parameter_type_list → Json
  | Params ps v => tag "Params" [("params", Json.arr (paramDecls ps).toArray), ("is_variadic", .bool v)]

def paramDecls : List parameter_declaration → List Json
  | [] => []
  | .PDeclaration_decl s d :: rest => tag "PDeclaration_decl" [("specifiers", specs s), ("declarator", declarator d)] :: paramDecls rest
  | .PDeclaration_abs_decl s a :: rest => tag "PDeclaration_abs_decl"
      [("specifiers", specs s), ("abstract_declarator", match a with | none => .null | some a => absDecl a)] :: paramDecls rest

def directDecl : direct_declarator → Json
  | .DDecl_identifier a i => tag "DDecl_identifier" [("attrs", attributes a), ("id", ident i)]
  | .DDecl_declarator d => tag "DDecl_declarator" [("declarator", declarator d)]
  | .DDecl_array d a => tag "DDecl_array" [("direct", directDecl d), ("array", arrayDecl a)]
  | .DDecl_function d p => tag "DDecl_function" [("direct", directDecl d), ("params", paramTypeList p)]

def declarator : declarator → Json
  | Declarator p d => tag "Declarator" [("pointer", match p with | none => .null | some p => pointerDecl p), ("direct", directDecl d)]

def absDecl : abstract_declarator → Json
  | .AbsDecl_pointer p => tag "AbsDecl_pointer" [("pointer", pointerDecl p)]
  | .AbsDecl_direct p d => tag "AbsDecl_direct" [("pointer", match p with | none => .null | some p => pointerDecl p), ("direct", directAbsDecl d)]

def directAbsDecl : direct_abstract_declarator → Json
  | .DAbs_abs_declarator a => tag "DAbs_abs_declarator" [("abstract_declarator", absDecl a)]
  | .DAbs_array d a => tag "DAbs_array" [("direct", match d with | none => .null | some d => directAbsDecl d), ("array", arrayDecl a)]
  | .DAbs_function d p => tag "DAbs_function" [("direct", match d with | none => .null | some d => directAbsDecl d), ("params", paramTypeList p)]

def typeName : type_name → Json
  | Type_name ts tq al a => tag "Type_name" [("type_specifiers", Json.arr (typeSpecs ts).toArray),
      ("type_qualifiers", list typeQualifier tq), ("alignment_specifiers", Json.arr (alignSpecs al).toArray),
      ("abstract_declarator", match a with | none => .null | some a => absDecl a)]

def specs : specifiers → Json
  | ⟨sc, ts, tq, fs, al⟩ => tag "Specifiers" [("storage_classes", list storageClass sc),
      ("type_specifiers", Json.arr (typeSpecs ts).toArray), ("type_qualifiers", list typeQualifier tq),
      ("function_specifiers", list functionSpecifier fs), ("alignment_specifiers", Json.arr (alignSpecs al).toArray)]

def staticAssert : static_assert_declaration → Json
  | Static_assert e msg => tag "Static_assert" [("expr", expr e), ("msg", stringLiteral msg)]

def initDecls : List init_declarator → List Json
  | [] => []
  | InitDecl l d i :: rest => tag "InitDecl" [("loc", loc l), ("declarator", declarator d),
      ("initializer", match i with | none => .null | some i => initializer i)] :: initDecls rest

def declaration : cabs_declaration → Json
  | .Declaration_base a s ids => tag "Declaration_base" [("attrs", attributes a), ("specifiers", specs s),
      ("init_declarators", Json.arr (initDecls ids).toArray)]
  | .Declaration_static_assert sa => tag "Declaration_static_assert" [("assert", staticAssert sa)]

def forClause : for_clause → Json
  | .FC_expr e => tag "FC_expr" [("expr", expr e)]
  | .FC_decl l d => tag "FC_decl" [("loc", loc l), ("decl", declaration d)]

def stmtList : List cabs_statement → List Json
  | [] => []
  | s :: ss => stmt s :: stmtList ss

def stmt : cabs_statement → Json
  | CabsStatement l a s => tag "CabsStatement" [("loc", loc l), ("attrs", attributes a), ("stmt", stmt_ s)]

def stmt_ : cabs_statement_ → Json
  | .CabsSlabel i s => tag "CabsSlabel" [("label", ident i), ("stmt", stmt s)]
  | .CabsScase e s => tag "CabsScase" [("expr", expr e), ("stmt", stmt s)]
  | .CabsSdefault s => tag "CabsSdefault" [("stmt", stmt s)]
  | .CabsSblock ss => tag "CabsSblock" [("stmts", Json.arr (stmtList ss).toArray)]
  | .CabsSdecl d => tag "CabsSdecl" [("decl", declaration d)]
  | .CabsSnull => .str "CabsSnull"
  | .CabsSexpr e => tag "CabsSexpr" [("expr", expr e)]
  | .CabsSif c t e => tag "CabsSif" [("cond", expr c), ("then_", stmt t), ("else_", match e with | none => .null | some e => stmt e)]
  | .CabsSswitch e s => tag "CabsSswitch" [("expr", expr e), ("stmt", stmt s)]
  | .CabsSwhile c b => tag "CabsSwhile" [("cond", expr c), ("body", stmt b)]
  | .CabsSdo c b => tag "CabsSdo" [("cond", expr c), ("body", stmt b)]
  | .CabsSfor i c st b => tag "CabsSfor" [("init", match i with | none => .null | some i => forClause i),
      ("cond", match c with | none => .null | some c => expr c), ("step", match st with | none => .null | some s => expr s), ("body", stmt b)]
  | .CabsSgoto i => tag "CabsSgoto" [("label", ident i)]
  | .CabsScontinue => .str "CabsScontinue"
  | .CabsSbreak => .str "CabsSbreak"
  | .CabsSreturn e => tag "CabsSreturn" [("expr", match e with | none => .null | some e => expr e)]
  | .CabsSpar ss => tag "CabsSpar" [("stmts", Json.arr (stmtList ss).toArray)]
  | .CabsSasm v i parts => tag "CabsSasm" [("is_volatile", .bool v), ("is_inline", .bool i),
      ("parts", list (fun (l, strs) => Json.arr #[loc l, list Json.str strs]) parts)]
  | .CabsScaseGNU lo hi s => tag "CabsScaseGNU" [("lo", expr lo), ("hi", expr hi), ("stmt", stmt s)]
  | .CabsSmarker s => tag "CabsSmarker" [("stmt", stmt s)]

end

def functionDef : function_definition → Json
  | FunDef l a s d b => tag "FunDef" [("loc", loc l), ("attrs", attributes a), ("specifiers", specs s),
      ("declarator", declarator d), ("body", stmt b)]

/-- External declarations. The CN constructors never come out of the C
grammar without magic comments; like the oracle's printer (which drops them
with `filter_map`), they are skipped. -/
def externalDecl : external_declaration → Option Json
  | .EDecl_func f => some (tag "EDecl_func" [("def", functionDef f)])
  | .EDecl_decl d => some (tag "EDecl_decl" [("decl", declaration d)])
  | .EDecl_magic (l, s) => some (tag "EDecl_magic" [("loc", loc l), ("str", .str s)])
  | _ => none

def translationUnit : translation_unit → Json
  | TUnit ds => tag "TUnit" [("decls", Json.arr (ds.filterMap externalDecl).toArray)]

end CSemCParser.CabsExport
