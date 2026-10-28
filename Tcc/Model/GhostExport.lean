import Tcc.Model.CompilerOpening
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
  | .specs s => CSemCParser.CabsExport.tag "specifiers" [("value",CSemCParser.CabsExport.specs s.specifiers),("attributes",CabsExport.attributes s.attributes)]
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

def spanJson (s : CSem.Pre.SourceSpan) : Json := Json.mkObj [
  ("opened_as",toJson (s.openedAs.map UInt8.toNat)),
  ("identity",toJson (s.identity.map (List.map UInt8.toNat))),
  ("start",toJson s.start.offset),("stop",toJson s.stop.offset)]

def logicalJson (cs : List Ghost.LogicalChar) : Json := Json.arr (cs.toArray.map fun c =>
  Json.arr #[toJson c.byte.toNat,toJson c.start,toJson c.stop,toJson c.synthesized])

def physicalLog (s : TccSt) (id : Nat) : Option (Array Ghost.PhysicalChar) :=
  match s.file.find? (fun f => f.id == id) with
  | some f => some f.ghostPhysical
  | none => s.ghostClosedPhysical.get? id

def bindingMetadata (tu : translation_unit) : List (String × Json) :=
  [("excluded",toJson ((Tcc.CSem.excluded tu).map reprStr)),
   ("bindings",Json.arr ((Tcc.CSem.Raw.injectedBindings tu).toArray.map fun b => Json.mkObj [
     ("name",toJson b.identity.binding_name),("target",toJson b.identity.binding_linker_name),
     ("intrinsic",toJson (match b.identity.binding_kind with | .Function_allocation_intrinsic => true | _ => false)),
     ("declaration",CabsExport.declaration b.declaration)]))]

def eventJson (e : Ghost.CompilerEvent) : Json := Json.mkObj [
  ("root_id",toJson e.root.source.id),("header_id",toJson e.header.id),
  ("predefined",toJson (e.root.source.contents.toList.map UInt8.toNat)),
  ("header_contents",toJson (e.header.contents.toList.map UInt8.toNat)),
  ("directive",toJson (reprStr e.root.directive)),
  ("authenticated",toJson e.authority.isSome)]

def modelMetadata (s : TccSt) : List (String × Json) :=
  let input := s.ghostInput
  [("compiler_event",(s.ghostCompilerEvent.map eventJson).getD Json.null),
   ("opening_files",Json.arr ((Ghost.openingFiles s).toArray.map fun f => Json.mkObj [
     ("opened_as",toJson (f.openedAs.map UInt8.toNat)),("identity",toJson (f.identity.map (List.map UInt8.toNat))),
     ("contents",toJson (f.contents.map UInt8.toNat)),("included_at",toJson (f.includedAt.map reprStr))])),
   ("normalizations",GhostExport.normalizations s),("tokens",toJson (((input.map (·.spelling)).getD #[]).toList.map fun t => (reprStr t.kind,t.spell.map UInt8.toNat))),
   ("origins",toJson (((input.map (·.language.origins)).getD #[]).toList.map reprStr)),
   ("model_origins",Json.arr (s.ghostOrigins.map fun o => Json.mkObj [
     ("source",toJson o.source),("buffer_offset",toJson o.bufferOffset),
     ("physical_offset",toJson o.physicalOffset),("presumed",toJson (o.presumed.toList.map UInt8.toNat)),
     ("line",toJson o.line),("includes",toJson o.includeFiles),("macro_frames",toJson o.macroFrames),("physical_segments",toJson (o.segments.toList.map spanJson))])),
   ("sources",Json.arr (s.ghostSources.map fun f => Json.mkObj [
     ("id",toJson f.id),("opened_as",toJson (f.openedAs.toList.map UInt8.toNat)),("contents",toJson (f.contents.toList.map UInt8.toNat)),
     ("identity",toJson (f.identity.map (List.map UInt8.toNat))), ("generated",toJson f.generated),
     ("included_at",toJson (f.includedAt.map reprStr)), ("include_complete",toJson f.includeComplete),
     ("logical_view",match physicalLog s f.id with
       | some chars => logicalJson (Ghost.logicalView chars)
       | none => Json.null)])),
   ("pragmas",Json.arr (s.ghostPragmas.map fun p => Json.mkObj [
     ("before_token",toJson p.beforeToken),("keyword",toJson p.keyword.toInt),
     ("pack_before",toJson (p.packBefore.toList.map Int32.toInt)),
     ("pack_after",toJson (p.packAfter.toList.map Int32.toInt)),
     ("pack_pointer_before",toJson p.packPointerBefore),("pack_pointer_after",toJson p.packPointerAfter),
     ("libraries_before",toJson (p.librariesBefore.toList.map (fun b => b.toList.map UInt8.toNat))),
     ("libraries_after",toJson (p.librariesAfter.toList.map (fun b => b.toList.map UInt8.toNat))),
     ("counter_before",toJson p.counterBefore.toInt),("counter_after",toJson p.counterAfter.toInt)]))]

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
/-- Ordered compiler handoff bundle. Active legacy fields remain a component
view only; consumers must inspect every record in units. -/
def bundle (s : TccSt) : Json := Json.mkObj [
  ("version",toJson (1 : Nat)),
  ("units",Json.arr (s.ghostUnits.map fun u =>
    let view := unitView u
    let tree := match u.tree with
      | .ok tu => [("status",toJson "ok"),("cabs",CabsExport.translationUnit tu)] ++ bindingMetadata tu
      | .error g => [("status",toJson "unimplemented"),("gap",gap g)]
    Json.mkObj (tree ++ [
      ("ordinal",toJson u.ordinal),("filename",toJson (u.filename.toList.map UInt8.toNat)),
      ("filetype",toJson u.filetype.toInt),
      ("compiler_result",toJson (u.compilerResult.map Int32.toInt)),
      ("source",(u.source.map fun src => Json.mkObj [
        ("opened_as",toJson (src.openedAs.toList.map UInt8.toNat)),
        ("resolved",toJson (src.resolved.toList.map UInt8.toNat)),
        ("contents",toJson (src.contents.toList.map UInt8.toNat))]).getD Json.null),
      ("configuration",Json.mkObj [
        ("cversion",toJson u.cversion.toInt),
        ("include_paths",toJson (u.includePaths.toList.map (fun p => p.toList.map UInt8.toNat))),
        ("system_paths",toJson (u.systemPaths.toList.map (fun p => p.toList.map UInt8.toNat))),
        ("command_defines",toJson (u.commandDefines.toList.map UInt8.toNat)),
        ("command_includes",toJson (u.commandIncludes.toList.map UInt8.toNat))]),
      ("audit",Json.arr (u.audit.map fragment)),("token_errors",toJson u.tokenErrors)] ++ modelMetadata view)))]

end Tcc.Model.GhostExport
