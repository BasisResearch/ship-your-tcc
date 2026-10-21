import Tcc.Model.Main
import Tcc.Model.Tie

/-!
# tcc_model: the executable TccModel driver

    tcc_model --fs c/build/fs.list --syms build/image.syms --prog prog.c
              [--extra /path=hostfile]... [--arg A]... [--larg L]...
              --arena-out OUT.arena [--console-out OUT.console]

Builds the image's file system (fs.list, /prog.c, extra files), its symbol
addresses (scripts/gen_image_syms.sh) and the command line gen_embed.sh
would embed (`{"tcc", args..., "-run", "/prog.c"}`, or with largs
`{"tcc", args..., "/prog.c", largs..., "-run", "--"}`; default args
`-w`), runs the model of tcc's main, and writes, when tcc reaches the
hand-off, the TCCARENA file in scripts/rv64emu.c's format. The console
(tcc's diagnostics) goes to --console-out. stdout gets one status line:
`ran`, `exit N` (tcc stopped before the program; N is its exit status),
or `model-error ...` (exit status 2).
-/

open Tcc.Model

def hex (n : UInt64) : String := String.mk (Nat.toDigits 16 n.toNat)

structure Opts where
  fs : String := "c/build/fs.list"
  syms : String := "build/image.syms"
  prog : String := ""
  extra : Array String := #[]
  args : Array String := #[]
  largs : Array String := #[]
  argsSet : Bool := false
  arenaOut : String := ""
  consoleOut : String := ""

partial def parseOpts (o : Opts) : List String → Except String Opts
  | [] => .ok o
  | "--fs" :: v :: r => parseOpts { o with fs := v } r
  | "--syms" :: v :: r => parseOpts { o with syms := v } r
  | "--prog" :: v :: r => parseOpts { o with prog := v } r
  | "--extra" :: v :: r => parseOpts { o with extra := o.extra.push v } r
  | "--arg" :: v :: r => parseOpts { o with args := o.args.push v, argsSet := true } r
  | "--no-args" :: r => parseOpts { o with argsSet := true } r
  | "--larg" :: v :: r => parseOpts { o with largs := o.largs.push v } r
  | "--arena-out" :: v :: r => parseOpts { o with arenaOut := v } r
  | "--console-out" :: v :: r => parseOpts { o with consoleOut := v } r
  | a :: _ => .error s!"unknown argument {a}"

/-- every directory prefix of an absolute path ("" is the root) -/
def dirsOf (p : String) : List String :=
  let comps := (p.splitOn "/").filter (· ≠ "")
  let rec go (acc : String) : List String → List String
    | [] => []
    | [_] => []
    | c :: rest => let d := acc ++ "/" ++ c; d :: go d rest
  go "" comps

def main (argv : List String) : IO UInt32 := do
  let o ← match parseOpts {} argv with
    | .ok o => pure o
    | .error e => IO.eprintln e; return 2
  let mut files : Std.HashMap ByteArray ByteArray := {}
  let mut dirs : Std.HashMap ByteArray Unit := {}
  let mut total : Nat := 0
  let mut specs : Array String := #["/prog.c=" ++ o.prog] ++ o.extra
  for l in (← IO.FS.readFile o.fs).splitOn "\n" do
    if l.trim ≠ "" then specs := specs.push l.trim
  for sp in specs do
    match sp.splitOn "=" with
    | path :: host :: rest =>
      let host := "=".intercalate (host :: rest)
      let data ← IO.FS.readBinFile host
      -- htif.c's fs_init: the first spec of a path wins (child lookup finds it)
      if !files.contains path.toUTF8 then
        files := files.insert path.toUTF8 data
        total := total + data.size
      for d in dirsOf path do dirs := dirs.insert d.toUTF8 ()
    | _ => pure ()
  let mut syms : Std.HashMap String UInt64 := {}
  for l in (← IO.FS.readFile o.syms).splitOn "\n" do
    match (l.trim.splitOn " ").filter (· ≠ "") with
    | [name, a] =>
      let v := a.foldl (fun acc c => acc * 16 + (if c.isDigit then c.toNat - 48 else c.toLower.toNat - 87)) 0
      syms := syms.insert name (UInt64.ofNat v)
    | _ => pure ()
  let args := if o.argsSet then o.args else #["-w"]
  let tccArgv : Array String :=
    if o.largs.isEmpty then #["tcc"] ++ args ++ #["-run", "/prog.c"]
    else #["tcc"] ++ args ++ #["/prog.c"] ++ o.largs ++ #["-run", "--"]
  -- the loop bound of every C loop: proportional to the input's size
  let fuel := 16 * total + 4096
  let st0 : TccSt := { fsFiles := files, fsDirs := dirs, imageSyms := syms, fuel := fuel,
                       fds := Array.replicate 3 none }
  let R := Tie.tie fuel
  let res := (tcc_main R (tccArgv.map (·.toUTF8))).run st0
  let (outcome, st) : MainOutcome × TccSt ← match res with
    | .ok r s => pure (r, s)
    | .error (.exit n) s => pure (.exited n, s)
    | .error e s =>
      let msg := match e with
        | .longjmp => "longjmp outside tcc_compile"
        | .exit n => s!"exit {n}"
        | .fuel w => s!"fuel exhausted in {w}"
        | .unsupported w => s!"unsupported: {w}"
        | .internal w => s!"internal: {w}"
      if o.consoleOut ≠ "" then IO.FS.writeBinFile o.consoleOut s.console
      IO.println s!"model-error {msg}"
      return 2
  if o.consoleOut ≠ "" then IO.FS.writeBinFile o.consoleOut st.console
  match outcome with
  | .exited n =>
    IO.println s!"exit {n.toUInt32.toUInt8.toNat}"
    return 0
  | .ran r =>
    let a := st.arena
    let mut len := a.size
    for _ in [0:a.size] do
      if len > 0 && a.get! (len - 1) == 0 then len := len - 1 else break
    let hdr := s!"TCCARENA 1\nbase {hex st.run_ptr}\nentry {hex r.entry}\ncode {hex r.code} {hex r.codeSize}\nsize {hex len.toUInt64}\n"
    if o.arenaOut ≠ "" then
      IO.FS.writeBinFile o.arenaOut (hdr.toUTF8 ++ a.extract 0 len)
    IO.println "ran"
    return 0
