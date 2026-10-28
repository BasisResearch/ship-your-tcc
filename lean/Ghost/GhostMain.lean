import Tcc.Model.GhostExport
import CSemCParser.CabsExport
import Spec.Pre.Main
import Spec.Pre.Tcc.Cfg
import Spec.Pre.Tcc.ImageFS
import Tcc.Model.CompilerOpening

open Lean Tcc.Model CSemCParser

def rangeJson (r : Ghost.Range) : Json := Json.arr #[toJson r.first, toJson r.last]
def gapJson (g : Ghost.Gap) : Json := Json.mkObj [
  ("production", toJson (reprStr g.production)), ("range", rangeJson g.range), ("reason", toJson g.reason)]
def fragmentJson := GhostExport.fragment

def errorText : Err → String
  | .exit i => s!"exit {i}" | .longjmp => "compiler rejection"
  | .fuel w => s!"model fuel {w}" | .unsupported w => s!"model unimplemented {w}"
  | .internal w => s!"model internal {w}"

def sectionBytes (s : TccSt) : Json := toJson (s.secs.toList.map fun sec =>
  (sec.name.toList.map UInt8.toNat, sec.data.toList.map UInt8.toNat, sec.data_offset, sec.reloc))

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

def main (args : List String) : IO UInt32 := do
  let (mode,path,opts) ← match args with
    | mode :: path :: opts => pure (mode,path,opts)
    | _ => IO.eprintln "usage: tcc_ghost MODE SOURCE [--files JSON_STRING_PAIRS] [--library IMAGEPATH]"; return 2
  let mut filesPath : Option String := none
  let mut libraries : List (List CSem.Pre.Bytes) := []
  let mut rest := opts
  for _ in [:opts.length] do
    match rest with
    | "--files" :: p :: tail => filesPath := some p; rest := tail
    | "--library" :: p :: tail =>
      libraries := libraries ++ [CSem.Pre.FileSys.normComponents p.toUTF8.toList]
      rest := tail
    | [] => break
    | _ => IO.eprintln "invalid option"; return 2
  let extra ← match filesPath with
    | none => pure []
    | some path =>
      let text ← IO.FS.readFile path
      let parsed := Json.parse text >>= (fromJson? (α := List (String × String)))
      match parsed with
      | .ok fs => pure (fs.map (fun (p,b) => (p.toUTF8.toList,b.toUTF8.toList)))
      | .error e => IO.eprintln e; return 2
  let fs := { CSem.Pre.Tcc.imageFS with files := CSem.Pre.Tcc.imageFS.files ++ extra }
  let src ← IO.FS.readBinFile path
  if mode == "independent-raw" then
    let cfg : Tcc.CSem.Raw.Config := {
      pre := CSem.Pre.Tcc.cfgTcc, fs := fs, parser := { gnu := true },
      compilerEnvironment := some Tcc.CSem.Raw.tccEnvironment, libraryFiles := libraries }
    match Tcc.CSem.Raw.checked cfg src.toList with
    | .error e =>
      IO.println ((Json.mkObj [("status",toJson "raw-checked-error"),("error",toJson e.code)]).compress)
    | .ok r =>
      IO.println ((Json.mkObj ([("status",toJson "ok"),("cabs",CabsExport.translationUnit r.cabs),
        ("tokens",toJson (r.preprocessing.tokens.map fun t => (reprStr t.token.kind,t.token.spell.map UInt8.toNat))),
        ("origins",toJson (r.input.origins.toList.map reprStr)),
        ("compiler_opening",toJson ((Tcc.CSem.Raw.compilerOpening cfg r.preprocessing).map fun o => reprStr o.directive)),
        ("sources",Json.arr (r.preprocessing.sources.toArray.map fun f => Json.mkObj [
          ("opened_as",toJson (f.openedAs.map UInt8.toNat)),("identity",toJson (f.identity.map (List.map UInt8.toNat))),
          ("contents",toJson (f.contents.map UInt8.toNat)),("included_at",toJson (f.includedAt.map reprStr))]))] ++ bindingMetadata r.cabs)).compress)
    return 0
  if mode == "raw-model" || mode == "raw-spec" then
    let result := Ghost.runRaw src fs.files true libraries
    let plain := Ghost.runRaw src fs.files false libraries
    let s := match result with | .ok _ s | .error _ s => s
    let p := match plain with | .ok _ s | .error _ s => s
    let base := [("console_equal",toJson (s.console == p.console)),
      ("section_bytes_equal",toJson (sectionBytes s == sectionBytes p)),
      ("audit",Json.arr (s.ghostAudit.map fragmentJson)),
      ("token_errors",toJson s.ghostTokenErrors),
      ("pragma_effects",toJson s.ghostPragmas.size)]
    let fields := match result with
      | .error e _ => [("status",toJson "model-rejected"),("error",toJson (errorText e))]
      | .ok _ s =>
        if mode == "raw-spec" then
          match s.ghostInput with
          | none => [("status",toJson "unimplemented"),("error",toJson "no model token view")]
          | some input => match parsePositioned { gnu := true } input.language.positioned with
            | .ok tu => [("status",toJson "ok"),("cabs",CabsExport.translationUnit tu)] ++ bindingMetadata tu
            | .error e => [("status",toJson "spec-rejected"),("error",toJson e.msg)]
        else match Ghost.output s with
          | .ok tu => [("status",toJson "ok"),("cabs",CabsExport.translationUnit tu)] ++ bindingMetadata tu
          | .error g => [("status",toJson "unimplemented"),("gap",gapJson g)]
    IO.println ((Json.mkObj (fields ++ base ++ modelMetadata s)).compress)
    return 0
  -- Shared-token fixtures explicitly omit predefined text. The raw modes
  -- above retain the actual compiler-injected header and its diagnostics.
  let cfg := { CSem.Pre.Tcc.cfgTcc with
    mainName := "/prog.c".toUTF8.toList, predefs := [] }
  let pp ← match CSem.Pre.phases14WithOrigins cfg fs src.toList with
    | .ok pp => pure pp
    | .error e => IO.println ((Json.mkObj [("status",toJson "preprocessing-error"),("error",toJson e.msg)]).compress); return 0
  let input ← match Phase7.convert { gnu := true } "/prog.c" pp with
    | .ok input => pure input
    | .error e => IO.println ((Json.mkObj [("status",toJson "phase7-error"),("error",toJson e)]).compress); return 0
  if mode == "origin-spec" then
    if !input.pragmas.isEmpty then
      IO.println ((Json.mkObj [("status",toJson "unimplemented"),("error",toJson "origin fixture has pragma events")]).compress)
      return 0
    IO.println ((Json.mkObj [("status",toJson "ok"),
      ("tokens",toJson (pp.tokens.map fun t => (reprStr t.token.kind,t.token.spell.map UInt8.toNat))),
      ("origins",toJson (input.origins.toList.map reprStr)),
      ("sources",Json.arr (pp.sources.toArray.map fun f => Json.mkObj [
        ("opened_as",toJson (f.openedAs.map UInt8.toNat)), ("contents",toJson (f.contents.map UInt8.toNat)),
        ("identity",toJson (f.identity.map (List.map UInt8.toNat))), ("included_at",toJson (f.includedAt.map reprStr)),
        ("logical_view",logicalJson ((CSem.Pre.phases12 cfg f.contents).map fun c =>
          let p := c.physical.getD default
          ⟨c.c,p.1.offset,p.2.offset,p.1.offset == p.2.offset⟩))]))]).compress)
    return 0
  if mode == "spec" then
    match parsePositioned { gnu := true } input.positioned with
    | .error e => IO.println ((Json.mkObj [("status",toJson "spec-rejected"),("error",toJson e.msg)]).compress)
    | .ok tu => IO.println ((Json.mkObj [("status",toJson "ok"),("cabs",CabsExport.translationUnit tu)]).compress)
    return 0
  if mode != "model" then IO.eprintln "mode must be model or spec"; return 2
  if !input.pragmas.isEmpty then
    IO.println ((Json.mkObj [("status",toJson "unimplemented"),("error",toJson "pragma effects need direct model stream connection")]).compress)
    return 0
  let raw := pp.tokens.filterMap fun t => if t.token.kind == .pragmaBegin || t.token.kind == .pragmaEnd then none else some t.token
  let fixture : Ghost.Input := ⟨input,raw.toArray⟩
  let result := Ghost.run fixture
  let plain := Ghost.run fixture false
  let s := match result with | .ok _ s | .error _ s => s
  let bytesEqual := match plain with
    | .ok _ p => sectionBytes s == sectionBytes p
    | .error _ p => sectionBytes s == sectionBytes p
  let audit := s.ghostAudit.map fragmentJson
  let drops := s.ghostDrops.map fun d => Json.mkObj [
    ("reason",toJson (reprStr d.reason)), ("range",rangeJson d.range),
    ("fragments",Json.arr (d.fragments.map fragmentJson))]
  let base := [("audit",Json.arr audit),("drops",Json.arr drops),
    ("section_bytes_equal",toJson bytesEqual), ("token_count",toJson raw.length),
    ("tokens",Json.arr (raw.toArray.map fun p => Json.arr #[toJson (reprStr p.kind),toJson (p.spell.map UInt8.toNat)])),
    ("origins",toJson (input.origins.toList.map reprStr))]
  let fields ← match result with
    | .error e _ => pure [("status",toJson "model-rejected"),("error",toJson (errorText e))]
    | .ok _ s => match Ghost.output s with
      | .error g => pure [("status",toJson "unimplemented"),("gap",gapJson g)]
      | .ok tu => pure [("status",toJson "ok"),("cabs",CabsExport.translationUnit tu)]
  IO.println ((Json.mkObj (fields++base)).compress)
  return 0
