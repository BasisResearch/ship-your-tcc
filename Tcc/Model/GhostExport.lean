import Tcc.Model.GhostDriver
import CSemCParser.CabsExport

/-! Diagnostic serialization only. Retain successful typed fragments even
when another production has a gap; such an artifact is not a successful TU.
No serialized field is an input to model computation. -/
namespace Tcc.Model.GhostExport
open Lean Ghost CSemCParser

def range (r : Range) : Json := Json.arr #[toJson r.first,toJson r.last]
def gap (g : Gap) : Json := Json.mkObj [
  ("production",toJson (reprStr g.production)),("range",range g.range),("reason",toJson g.reason)]

def value : Ghost.Syntax → Json
  | .expression e => CSemCParser.CabsExport.tag "expression" [("value",CSemCParser.CabsExport.expr e)]
  | .typeName t => CSemCParser.CabsExport.tag "typeName" [("value",CSemCParser.CabsExport.typeName t)]
  | .typeSpecifier t => CSemCParser.CabsExport.tag "typeSpecifier" [("value",CSemCParser.CabsExport.typeSpec t)]
  | .identifier id => CSemCParser.CabsExport.tag "identifier" [("value",CSemCParser.CabsExport.ident id)]
  | .attributes a => CSemCParser.CabsExport.tag "attributes" [("value",CSemCParser.CabsExport.attributes a)]
  | .asmBinding s => CSemCParser.CabsExport.tag "asmBinding" [("literal",CSemCParser.CabsExport.stringLiteral s)]
  | .specs s => CSemCParser.CabsExport.tag "specifiers" [("value",CSemCParser.CabsExport.specs s)]
  | .decl (.concrete d) => CSemCParser.CabsExport.tag "declarator" [("value",CSemCParser.CabsExport.declarator d)]
  | .decl (.abstract a) => CSemCParser.CabsExport.tag "abstractDeclarator" [("value",CSemCParser.CabsExport.opt CSemCParser.CabsExport.absDecl a)]
  | .initializer i => CSemCParser.CabsExport.tag "initializer" [("value",CSemCParser.CabsExport.initializer i)]
  | .initDecl i => CSemCParser.CabsExport.tag "initDeclarator" [("value",Json.arr (CSemCParser.CabsExport.initDecls [i]).toArray)]
  | .declaration d => CSemCParser.CabsExport.tag "declaration" [("value",CSemCParser.CabsExport.declaration d)]
  | .functionBody s => CSemCParser.CabsExport.tag "functionBody" [("value",CSemCParser.CabsExport.stmt s)]
  | .functionDeclarator d s => CSemCParser.CabsExport.tag "functionDeclarator" [("declarator",CSemCParser.CabsExport.declarator d),("body",CSemCParser.CabsExport.stmt s)]
  | .external e => CSemCParser.CabsExport.tag "external" [("value",(CSemCParser.CabsExport.externalDecl e).getD Json.null)]
  | .statement s => CSemCParser.CabsExport.tag "statement" [("value",CSemCParser.CabsExport.stmt s)]
  | .label (.named i) => CSemCParser.CabsExport.tag "namedLabel" [("identifier",CSemCParser.CabsExport.ident i)]
  | .label (.caseValue lo hi) => CSemCParser.CabsExport.tag "caseLabel" [("lo",CSemCParser.CabsExport.expr lo),("hi",CSemCParser.CabsExport.opt CSemCParser.CabsExport.expr hi)]
  | .label .defaultValue => CSemCParser.CabsExport.tag "defaultLabel" []
  | .translationUnit t => CSemCParser.CabsExport.translationUnit t

def fragment (f : Fragment) : Json := Json.mkObj [
  ("production",toJson (reprStr f.production)),("range",range f.range),
  ("gap",match f.value with | .ok _ => Json.null | .error e => gap e),
  ("syntax",match f.value with | .ok v => value v | .error _ => Json.null)]

def normalizations (s : TccSt) : Json := Json.arr (s.ghostNormalizations.map fun n => Json.mkObj [
  ("kind",toJson (reprStr n.kind)),("range",range n.range)])

def artifact (s : TccSt) : Json :=
  let fields := match output s with
    | .ok tu => [("status",toJson "ok"),("cabs",CSemCParser.CabsExport.translationUnit tu)]
    | .error g => [("status",toJson "unimplemented"),("gap",gap g)]
  Json.mkObj (fields ++ [("normalizations",normalizations s),("audit",Json.arr (s.ghostAudit.map fragment)),
    ("compiler_opening",toJson ((s.ghostCompilerEvent.bind CompilerEvent.authority).map fun o => reprStr o.directive)),
    ("token_errors",toJson s.ghostTokenErrors),
    ("tokens",toJson (((s.ghostInput.map (·.spelling)).getD #[]).toList.map fun t =>
      (reprStr t.kind,t.spell.map UInt8.toNat))),
    ("origins",toJson (((s.ghostInput.map (·.language.origins)).getD #[]).toList.map reprStr)),
    ("pragma_effects",toJson s.ghostPragmas.size)])
end Tcc.Model.GhostExport
