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

open Tcc.Model.GhostExport

def main (args : List String) : IO UInt32 := do
  let (mode,path,opts) ← match args with
    | mode :: path :: opts => pure (mode,path,opts)
    | _ => IO.eprintln "usage: tcc_ghost MODE SOURCE [--files JSON_STRING_PAIRS] [--library IMAGEPATH]"; return 2
  let mut mainName := "/prog.c"
  let mut filesPath : Option String := none
  let mut libraries : List (List CSem.Pre.Bytes) := []
  let mut rest := opts
  for _ in [:opts.length] do
    match rest with
    | "--main-name" :: p :: tail => mainName := p; rest := tail
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
  if mode == "independent-raw" || mode == "independent-parse" then
    let predefined := CSem.Pre.Tcc.predefsFor mainName
    let cfg : Tcc.CSem.Raw.Config := {
      pre := { CSem.Pre.Tcc.cfgTcc with mainName := mainName.toUTF8.toList, predefs := predefined }, fs := fs, parser := { gnu := true },
      compilerEnvironment := some { Tcc.CSem.Raw.tccEnvironment with predefined := predefined }, libraryFiles := libraries }
    let result := if mode == "independent-raw" then Tcc.CSem.Raw.checked cfg src.toList
      else (Tcc.CSem.Raw.run cfg src.toList).mapError Tcc.CSem.Raw.FrontEndError.parse
    match result with
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
