import Tcc.CSem.RawFrontEnd
import CSemCParser.CabsExport
import Spec.Pre.Iso
import Spec.Pre.Tcc.Cfg

/-! IO/test boundary only. The independent layout serializer is not imported
by any pure frontend module. -/
open Lean (Json toJson)
open CSem.Pre
open CSemCParser
open Tcc.CSem.Raw

namespace RawRunner

def obj := Json.mkObj
def num (n : Nat) : Json := toJson n

def byteString (b : Bytes) : String := Lex.slice (Phase7.bytes b) 0 b.length

def pointJson (p : SourcePoint) : Json :=
  obj [("offset", num p.offset), ("line", num p.line), ("column", num p.column)]

def spanJson (s : SourceSpan) : Json :=
  obj [("openedAs", toJson (byteString s.openedAs)),
    ("identity", toJson (s.identity.map byteString)),
    ("start", pointJson s.start), ("stop", pointJson s.stop)]

def siteJson (s : SourceSite) : Json :=
  obj [("physical", spanJson s.physical), ("presumed", toJson (byteString s.presumed)),
    ("line", num s.line), ("endLine", num s.endLine),
    ("includes", .arr (s.includes.map spanJson).toArray)]

def tokenJson (p : LocatedPPToken) : Json :=
  obj [("kind", toJson (reprStr p.token.kind)), ("spelling", toJson (byteString p.token.spell)),
    ("derivation", toJson (reprStr p.origin)),
    ("site", (p.origin.site.map siteJson).getD .null)]

def ppJson (pp : PreprocessingResult) : Json := .arr (pp.tokens.map tokenJson).toArray

def libraryJson (o : LibraryOrigin) : Json :=
  obj [("site", siteJson o.site), ("actualLibrary", toJson o.actualLibrary),
    ("presumedLibrary", toJson o.presumedLibrary)]

def resultJson (r : Tcc.CSem.Raw.Result) : Json :=
  obj [("stage", toJson ("parse-audit" : String)), ("digest", toJson r.digest), ("cabs", CabsExport.translationUnit r.cabs),
    ("phase4", ppJson r.preprocessing),
    ("sources", .arr (r.preprocessing.sources.toArray.map fun f => obj [
      ("openedAs", toJson (byteString f.openedAs)), ("identity", toJson (f.identity.map byteString)),
      ("byteLength", num f.contents.length), ("includedAt", (f.includedAt.map spanJson).getD .null)])),
    ("finalPack", toJson r.preprocessing.finalPack),
    ("pragmas", .arr (r.input.pragmas.map fun p => obj [
      ("beforeToken", num p.beforeToken), ("payload", .arr (p.payload.map tokenJson).toArray),
      ("origin", toJson (reprStr p.origin))])),
    ("libraryOrigins", .arr (r.libraryOrigins.map libraryJson))]

/-- Independent oracle encoding of the declared virtual lexing stream.
One additional terminal LF compensates for Cerberus's input_line/join cpp
reader, which removes the final LF. No escaping is guessed for file labels
that its line-marker grammar cannot represent. -/
def layout (cfg : ParseCfg) (pp : PreprocessingResult) : Except String String := do
  let mut chunks : Array String := #[]
  let mut pragma := false
  for p in pp.tokens do
    if p.token.kind = .pragmaBegin then pragma := true
    else if p.token.kind = .pragmaEnd then pragma := false
    else if !pragma then
      let site ← match p.origin.site with
        | some s => pure s
        | none => throw "cannot serialize an unavailable source position"
      let fname := byteString site.presumed
      if fname.contains '"' ∨ fname.contains '\n' ∨ fname.contains '\r' ∨ fname.contains '\t' then
        throw "file label is outside the oracle line-marker encoding"
      let spelling ← Phase7.parserSpelling cfg p.token
      let pad := String.ofList (List.replicate (site.physical.start.column - 1) ' ')
      chunks := chunks.push s!"# {site.line} \"{fname}\"\n{pad}{byteString spelling}\n"
  if chunks.isEmpty then pure "" else pure (String.join chunks.toList ++ "\n")

structure Options where
  file : Option String := none
  fslist : Option String := none
  layoutFile : Option String := none
  library : List (List Bytes) := []
  tcc : Bool := false
  gnu : Bool := false
  phase4 : Bool := false
  checked : Bool := false

def options : List String → Options → Except String Options
  | [], o => .ok o
  | "--fslist" :: p :: rest, o => options rest { o with fslist := some p }
  | "--layout" :: p :: rest, o => options rest { o with layoutFile := some p }
  | "--library" :: p :: rest, o => options rest { o with library := FileSys.normComponents (bs p) :: o.library }
  | "--tcc" :: rest, o => options rest { o with tcc := true }
  | "--gnu" :: rest, o => options rest { o with gnu := true }
  | "--checked" :: rest, o => options rest { o with checked := true }
  | "--phase4" :: rest, o => options rest { o with phase4 := true }
  | p :: rest, o =>
    if o.file.isSome then .error "unexpected argument" else options rest { o with file := some p }

def loadFs (path : String) : IO FileSys := do
  let text ← IO.FS.readFile path
  let mut files := []
  for line in text.splitOn "\n" do
    match line.splitOn "=" with
    | p :: h :: more =>
      let b ← IO.FS.readBinFile ("=".intercalate (h :: more))
      files := files ++ [(bs p, b.toList)]
    | _ => pure ()
  return ⟨files⟩

end RawRunner

def main (args : List String) : IO UInt32 := do
  let opts ← match RawRunner.options args {} with
    | .error e => IO.eprintln e; return 2
    | .ok o => pure o
  let file ← match opts.file with
    | none => IO.eprintln "usage: csem_raw [--tcc] [--gnu] [--phase4] [--fslist FILE] [--layout FILE] [--library IMAGEPATH] SOURCE"; return 2
    | some f => pure f
  let src ← IO.FS.readBinFile file
  let fs ← match opts.fslist with | none => pure (⟨[]⟩ : FileSys) | some p => RawRunner.loadFs p
  let pre := if opts.tcc then CSem.Pre.Tcc.cfgTcc else cfgIso
  let cfg : Tcc.CSem.Raw.Config := {
    pre := { pre with mainName := bs file }, fs,
    parser := { gnu := opts.gnu }, libraryFiles := opts.library }
  if opts.phase4 then
    match phases14WithOrigins cfg.pre fs src.toList with
    | .error e => IO.eprintln e.msg; return 1
    | .ok pp => IO.println (RawRunner.ppJson pp).compress; return 0
  if opts.checked then
    match Tcc.CSem.Raw.checked cfg src.toList with
    | .error e => IO.eprintln e.code; return 1
    | .ok _ => pure ()
  match Tcc.CSem.Raw.run cfg src.toList with
  | .error e => IO.eprintln e; return 1
  | .ok r =>
    if let some path := opts.layoutFile then
      match RawRunner.layout cfg.parser r.preprocessing with
      | .error e => IO.eprintln e; return 2
      | .ok text => IO.FS.writeFile path text
    IO.println (RawRunner.resultJson r).compress
    return 0
