import Tcc.Model.Preprocess

/-! Dedicated raw-token protocol driver; successful empty streams still have
an explicit completion trailer. Errors never carry a partial token stream. -/
open Tcc.Model

def rawHex (b : ByteArray) : String :=
  String.join (b.toList.map fun v =>
    let ds := "0123456789abcdef".toList
    String.ofList [ds[v.toNat / 16]!, ds[v.toNat % 16]!])

def rawClass : PPClass → String
  | .ident => "I" | .number => "N" | .charLit => "C" | .stringLit => "S"
  | .punct => "P" | .other => "O" | .pragmaBegin => "[" | .pragmaEnd => "]"

structure RawOpts where
  file : String := ""
  fs : String := ""
  plain : Bool := false

private def opts : RawOpts → List String → Except String RawOpts
  | o, [] => if o.file == "" then .error "missing --prog" else .ok o
  | o, "--prog" :: p :: r => opts { o with file := p } r
  | o, "--fs" :: p :: r => opts { o with fs := p } r
  | o, "--plain" :: r => opts { o with plain := true } r
  | _, _ => .error "usage: tcc_pre_tokens --prog FILE [--fs LIST]"

def main (args : List String) : IO UInt32 := do
  let o ← match opts {} args with
    | .ok o => pure o
    | .error e => IO.eprintln e; return 2
  let src ← IO.FS.readBinFile o.file
  let mut fs := CSem.Pre.Tcc.imageFS.files
  if o.fs != "" then
    fs := []
    for row in (← IO.FS.readFile o.fs).splitOn "\n" do
      if row == "" then continue
      let parts := row.splitOn "="
      if parts.length < 2 then throw (IO.userError "malformed fs.list")
      let p := parts.head!
      if p.endsWith ".a" || p.endsWith ".o" then continue
      let b ← IO.FS.readBinFile ("=".intercalate parts.tail!)
      fs := fs ++ [(p.toUTF8.toList, b.toList)]
  IO.println "TCCPP 1"
  match runPreprocess (preprocessState src fs) (!o.plain) with
  | .error e s =>
    IO.eprint (Bytes.toStr s.console)
    let (cl, msg, rc) := match e with
      | .longjmp => ("refuse", "tcc_error", 1)
      | .exit n => ("refuse", s!"exit {n}", 1)
      | .fuel w => ("model", s!"fuel {w}", 2)
      | .unsupported w => ("model", s!"unsupported {w}", 2)
      | .internal w => ("model", s!"internal {w}", 2)
    IO.println s!"ERROR {cl} {rawHex msg.toUTF8}"
    return UInt32.ofNat rc
  | .ok _ s =>
    IO.eprint (Bytes.toStr s.console)
    for t in s.ppObserved do
      IO.println s!"{rawClass t.kind} {t.spell.size} {rawHex t.spell}"
    IO.println s!"STATE {s.pack_stack_ptr} {s.pack_stack[s.pack_stack_ptr]!} {s.pp_counter} {s.pragma_libs.size}"
    IO.println s!"END {s.ppObserved.size}"
    return 0
