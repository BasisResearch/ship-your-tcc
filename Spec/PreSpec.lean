/-
`pre_spec`: the native driver of `CSem.Pre.phases14` for the differential
(scripts/pre_diff.py).

    pre_spec [--cfg run|E] [--fslist FILE] [--image-fs] [--date S] [--time S] FILE
        phases 1–4 of FILE as the main file (/prog.c); one token per line,
        `K<TAB>spelling` (K: H header-name, I identifier, N pp-number,
        C character constant, S string literal, P punctuator, O other,
        [ / ] pragma begin / end); on an error, `!<TAB>kind<TAB>file:line<TAB>msg`
        and exit status 1.
    pre_spec --lex FILE      phase 1–3 tokens only (same format, no directives)
    pre_spec --check-fs FILE the compiled-in image FS equals FILE's headers

The file system is read from an fs.list (`/path/in/image=host/file` lines,
the `.h` entries), or is the compiled-in `imageFS` (`--image-fs`).
-/
import Spec.Pre
import Spec.Pre.Tcc.Cfg
import Spec.Pre.Tcc.ImageFS

open CSem.Pre

def kindChar : PPKind → String
  | .headerName => "H" | .ident => "I" | .ppNumber => "N" | .charConst => "C"
  | .strLit => "S" | .punct => "P" | .other => "O" | .pragmaBegin => "[" | .pragmaEnd => "]"

def hexDigit (n : Nat) : Char := "0123456789abcdef".toList.getD n '0'

def escBytes (b : Bytes) : String :=
  String.join (b.map (fun c =>
    if c = 92 then "\\\\"
    else if 32 ≤ c ∧ c < 127 then (Char.ofNat c.toNat).toString
    else "\\x" ++ (hexDigit (c.toNat / 16)).toString ++ (hexDigit (c.toNat % 16)).toString))

def readBytes (p : String) : IO Bytes := do
  let b ← IO.FS.readBinFile p
  pure b.toList

def loadFsList (p : String) : IO FileSys := do
  let txt ← IO.FS.readFile p
  let mut files : List (Bytes × Bytes) := []
  for line in txt.splitOn "\n" do
    match line.splitOn "=" with
    | path :: rest =>
      if path.endsWith ".h" ∧ !rest.isEmpty then
        let host := "=".intercalate rest
        let c ← readBytes host
        files := files ++ [(bs path, c)]
    | [] => pure ()
  pure ⟨files⟩

def errKindStr : ErrKind → String
  | .lexical => "lexical" | .directive => "directive" | .include => "include" | .macro => "macro"
  | .ifExpr => "ifExpr" | .errorDirective => "error" | .limit => "limit" | .fuel => "fuel"

structure Opts where
  cfg : String := "run"
  fslist : Option String := none
  imageFs : Bool := false
  date : Option String := none
  time : Option String := none
  lex : Bool := false
  checkFs : Bool := false
  file : Option String := none

partial def parseOpts : List String → Opts → Opts
  | "--cfg" :: v :: r, o => parseOpts r { o with cfg := v }
  | "--fslist" :: v :: r, o => parseOpts r { o with fslist := some v }
  | "--image-fs" :: r, o => parseOpts r { o with imageFs := true }
  | "--date" :: v :: r, o => parseOpts r { o with date := some v }
  | "--time" :: v :: r, o => parseOpts r { o with time := some v }
  | "--lex" :: r, o => parseOpts r { o with lex := true }
  | "--check-fs" :: r, o => parseOpts r { o with checkFs := true }
  | f :: r, o => parseOpts r { o with file := some f }
  | [], o => o

def printToks (out : IO.FS.Stream) (ts : List PPToken) : IO Unit := do
  for t in ts do
    out.putStrLn (kindChar t.kind ++ "\t" ++ escBytes t.spell)

def main (args : List String) : IO UInt32 := do
  let o := parseOpts args {}
  let out ← IO.getStdout
  let some file := o.file | do IO.eprintln "usage: pre_spec [options] FILE"; return 2
  if o.checkFs then
    let fs ← loadFsList file
    let img := Tcc.imageFS
    let ok := fs.files.length = img.files.length ∧
      fs.files.all (fun (p, c) => img.lookup p = some c)
    out.putStrLn (if ok then s!"image FS matches {fs.files.length} headers" else "image FS differs")
    return (if ok then 0 else 1)
  let base := if o.cfg = "E" then Tcc.cfgTccE else Tcc.cfgTcc
  let cfg := { base with
    date := (o.date.map bs).getD base.date,
    time := (o.time.map bs).getD base.time }
  let src ← readBytes file
  if o.lex then
    match phases123 cfg (bs file) src with
    | .ok items =>
      printToks out (items.filterMap (fun i => match i with
        | .tok t => some (⟨t.kind, t.spell⟩ : PPToken) | .nl _ => none))
      return 0
    | .error e =>
      out.putStrLn s!"!\t{errKindStr e.kind}\t{escBytes e.file}:{e.line}\t{e.msg}"
      return 1
  let fs ← if o.imageFs then pure Tcc.imageFS else
    match o.fslist with
    | some p => loadFsList p
    | none => pure Tcc.imageFS
  match phases14 cfg fs src with
  | .ok ts => printToks out ts; return 0
  | .error e =>
    out.putStrLn s!"!\t{errKindStr e.kind}\t{escBytes e.file}:{e.line}\t{e.msg}"
    return 1
