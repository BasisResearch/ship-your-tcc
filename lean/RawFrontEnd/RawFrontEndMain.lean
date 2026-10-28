import Core_run
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
  obj [("site", siteJson o.site), ("identitySite", siteJson o.identitySite), ("actualLibrary", toJson o.actualLibrary),
    ("presumedLibrary", toJson o.presumedLibrary)]

/-- Exercise the five generated semantic consumers directly, including the
alternate Core_run engine. Each result is an actual semantic location. -/
def consumerLocations (location : CerbLocation.Loc) : Json := Id.run do
  letI : CerbImplCfg.Impl := ⟨CerbImplCfg.tccRV64⟩
  letI : LemFuel := ⟨100⟩
  letI : CerbGlobal.Switches := ⟨CerbGlobal.defaultSwitches⟩
  let caller : CerbLocation.Loc := .point {
    file := "caller.c", line := 123, col := 4, lexingFile := "caller.c", lexingLine := 123,
    sourceIdentity := some ⟨[bs "caller.c"], false⟩ }
  let undef : pexpr := Pexpr [] () (PEundef location UB045a_division_by_zero)
  let mem := CerbMem.initialMemState 0x10000
  let file : generic_file Unit core_run_annotation := default
  let locResult (o : Option CerbLocation.Loc) := (o.map CabsExport.loc).getD .null
  let pureLoc := match step_eval_pexpr fmapEmpty fmapEmpty 0 caller none fmapEmpty [] none file false undef with
    | .Result (.Undef loc _) => some loc
    | _ => none
  let state (arena : expr core_run_annotation) : thread_state :=
    { (default : thread_state) with arena, current_loc := caller, env := [fmapEmpty] }
  let evalArena : expr core_run_annotation := Expr [Aloc location] (Epure undef)
  let runLoc := match core_thread_step2 fmapEmpty fmapEmpty mem file fmapEmpty fmapEmpty 0 (none, state evalArena) with
    | [Step_eval _ m] => match m default with
      | .Result (.Undef loc _, _) => some loc
      | _ => none
    | _ => none
  let action : expr core_run_annotation :=
    Expr [Aloc location] (Eaction (Paction Pos (Action location default (Kill Dynamic0 (mk_nullptr_pe void)))))
  let killLoc := match core_thread_step2 fmapEmpty fmapEmpty mem file fmapEmpty fmapEmpty 0 (none, state action) with
    | [Step_action_request _ m] => match m default with
      | .Result (.Defined (KillRequest loc ..), _) => some loc
      | _ => none
    | _ => none
  let reductionLoc := match step_ctx fmapEmpty fmapEmpty mem file fmapEmpty 0 (none, state action) with
    | [Step_action_request2 _ loc _ _ _] => some loc
    | _ => none
  return obj [("pure-undef", locResult pureLoc), ("run-current-and-undef", locResult runLoc),
    ("run-kill", locResult killLoc), ("reduction-current-and-action", locResult reductionLoc)]


def resultJson (r : Tcc.CSem.Raw.Result) (probe : Bool := false) : Json :=
  obj [("stage", toJson ("parse-audit" : String)), ("digest", toJson r.digest), ("cabs", CabsExport.translationUnit r.cabs),
    ("phase4", ppJson r.preprocessing),
    ("sources", .arr (r.preprocessing.sources.toArray.map fun f => obj [
      ("openedAs", toJson (byteString f.openedAs)), ("identity", toJson (f.identity.map byteString)),
      ("byteLength", num f.contents.length), ("includedAt", (f.includedAt.map spanJson).getD .null)])),
    ("finalPack", toJson r.preprocessing.finalPack),
    ("pragmas", .arr (r.input.pragmas.map fun p => obj [
      ("beforeToken", num p.beforeToken), ("payload", .arr (p.payload.map tokenJson).toArray),
      ("origin", toJson (reprStr p.origin))])),
    ("libraryOrigins", .arr (r.libraryOrigins.map libraryJson)),
    ("consumers", .arr (if probe then r.input.positioned.tokens.filterMap fun t =>
      t.sp.sourceIdentity.map fun _ => obj [("input", CabsExport.pos t.sp),
        ("locations", consumerLocations (.point t.sp))] else #[]))]

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

/-- Diagnostic execution of the actual raw -> Cabs -> Ail -> Core -> driver
path. Finite executions are tests, not frontend/compiler correctness proofs. -/
def semanticJson (raw : Tcc.CSem.Raw.Config) (src : Bytes) : Json :=
  let base := { (Tcc.cfgTcc (Tcc.CSem.Raw.frontEnd raw) []) with runtime := "/neutral-core-labels" }
  let std := FileSys.normComponents (bs "/csem/runtime/libcore/std.core")
  let imp := FileSys.normComponents (bs ("/csem/runtime/libcore/impls/" ++ base.implFileName))
  let raw := { raw with libraryFiles := std :: imp :: raw.libraryFiles }
  let cfg := Tcc.CSem.Raw.semanticConfig raw base ⟨std, imp⟩
  letI : CerbImplCfg.Impl := ⟨cfg.implementation⟩
  letI : LemFuel := ⟨50000⟩
  letI : CerbGlobal.Switches := ⟨CerbGlobal.defaultSwitches⟩
  let diagnostic := fun (s : String) => obj [("status", toJson s)]
  match Tcc.CSem.Raw.checked raw src, Tcc.CSem.stdlib cfg with
  | .ok r, .ok stdlib =>
    letI : CerberusFresh.Digest := ⟨r.digest⟩
    match Tcc.CSem.elaborate cfg stdlib r.cabs with
    | .error _ => diagnostic "elaboration-error"
    | .ok (coreFile, supply) =>
      match link [coreFile] with
      | .Exception _ => diagnostic "link-error"
      | .Result linked =>
        let file := convert_file linked
        let (st, _) := initial_driver_state supply cfg.addressSpaceTop r.digest file
          (CerbFS.fs_initial_state_of [("/prog.c", src)])
        let leaves := CerbND.runND (drive file.enumDefs file.tagDefs false file cfg.argv) st
        .arr (leaves.toArray.map fun leaf =>
          match leaf with
          | (.Active result, _, _) =>
            match result.dres_core_value with
            | Vloaded (LVspecified (OVinteger iv)) =>
              obj [("status", toJson ("defined" : String)), ("stdout", toJson result.dres_stdout),
                ("exit", toJson (eval_integer_value iv))]
            | _ => diagnostic "non-integer-return"
          | (.Killed _ reason, _, _) =>
            match reason with
            | .Undef0 loc ub => obj [("status", toJson ("undefined" : String)),
                ("location", CabsExport.loc loc), ("diagnostic", toJson (ub.map stringFromUndefined_behaviour))]
            | .Error0 loc message => obj [("status", toJson ("error" : String)),
                ("location", CabsExport.loc loc), ("diagnostic", toJson message)]
            | .Other message => obj [("status", toJson ("other-driver-error" : String)),
                ("diagnostic", toJson (driverErrorBatchMsg message))])
  | .error _, _ => diagnostic "frontend-diagnostic"
  | _, .error _ => diagnostic "runtime-parse-error"

structure Options where
  file : Option String := none
  fslist : Option String := none
  layoutFile : Option String := none
  library : List (List Bytes) := []
  tcc : Bool := false
  gnu : Bool := false
  phase4 : Bool := false
  includes : List Bytes := []
  checked : Bool := false
  semantic : Bool := false

def options : List String → Options → Except String Options
  | [], o => .ok o
  | "--fslist" :: p :: rest, o => options rest { o with fslist := some p }
  | "--layout" :: p :: rest, o => options rest { o with layoutFile := some p }
  | "--include" :: p :: rest, o => options rest { o with includes := o.includes ++ [bs p] }
  | "--library" :: p :: rest, o => options rest { o with library := FileSys.normComponents (bs p) :: o.library }
  | "--tcc" :: rest, o => options rest { o with tcc := true }
  | "--gnu" :: rest, o => options rest { o with gnu := true }
  | "--semantic" :: rest, o => options rest { o with semantic := true }
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
    | none => IO.eprintln "usage: csem_raw [--tcc] [--gnu] [--phase4] [--semantic] [--include IMAGE_DIR] [--fslist FILE] [--layout FILE] [--library IMAGEPATH] SOURCE"; return 2
    | some f => pure f
  let src ← IO.FS.readBinFile file
  let fs ← match opts.fslist with | none => pure (⟨[]⟩ : FileSys) | some p => RawRunner.loadFs p
  let pre := if opts.tcc then CSem.Pre.Tcc.cfgTcc else cfgIso
  let cfg : Tcc.CSem.Raw.Config := {
    pre := { pre with mainName := bs file, sysIncludePaths := opts.includes ++ pre.sysIncludePaths }, fs,
    parser := { gnu := opts.gnu || opts.tcc }, libraryFiles := opts.library }
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
    let result := RawRunner.resultJson r opts.semantic
    let result := if opts.semantic then
      Json.mkObj [("parse", result), ("semantics", RawRunner.semanticJson cfg src.toList)]
      else result
    IO.println result.compress
    return 0
