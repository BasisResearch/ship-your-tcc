/-
  clight_run FILE.lean [--fuel N] [--quiet]

  FILE.lean is the output of `clightgen -lean` (any flags).  The file is
  elaborated in-process against the precompiled CCLib / Clightdefs (no lake
  build, no .olean written), its `<Namespace>.prog : CC.Program` is evaluated,
  and the program is run natively-compiled through `ClightRun.runProgram`.

  stdout : exactly the bytes the C program wrote (printf/putchar/puts)
  exit   : the program's exit status (low 8 bits of main's result / exit(n))
           125  harness error (file does not elaborate, no prog, ...)
           126  undefined behaviour / stuck state of the Lean interpreter
           127  outside the model (unsupported external or printf conversion)
           124  out of fuel
  stderr : one line "[clight_run] ..." for every non-normal end, plus timings
           and the step count with --stats.
-/
import Lean
import ClightRun.Run

open Lean Elab

def fail (code : UInt32) (msg : String) : IO UInt32 := do
  IO.eprintln s!"[clight_run] {msg}"
  return code

unsafe def loadProg (file : System.FilePath) : IO (Except String CC.Program) := do
  let input ← IO.FS.readFile file
  -- generated files start with `import Clightdefs`; import it (and CCLib)
  -- ourselves and elaborate the rest.
  let lines := input.splitOn "\n"
  let body := String.intercalate "\n" (lines.map fun l =>
    if l.startsWith "import " then "" else l)
  let ns? := lines.findSome? fun l =>
    if l.startsWith "namespace " then some ((l.drop 10).trimAscii.copy) else none
  let some ns := ns? | return .error "no `namespace` line in the generated file"
  let env ← importModules #[{ module := `Clightdefs }, { module := `CCLib }] {} (loadExts := true)
  let (env, msgs) ← Lean.Elab.process body env {} (some file.toString)
  if msgs.hasErrors then
    let errs ← msgs.toList.filterMapM fun m => do
      if m.severity == .error then return some (← m.toString) else return none
    return .error s!"generated file does not elaborate:\n{String.intercalate "\n" (errs.take 5)}"
  let name := ns.toName ++ `prog
  match env.evalConst CC.Program {} name (checkMeta := false) with
  | .ok p => return .ok p
  | .error e => return .error s!"cannot evaluate {name}: {e}"

unsafe def main (args : List String) : IO UInt32 := do
  let mut file? : Option String := none
  let mut fuel := 0
  let mut stats := false
  let mut rest := args
  while !rest.isEmpty do
    match rest with
    | "--fuel" :: n :: r => fuel := n.toNat!; rest := r
    | "--stats" :: r => stats := true; rest := r
    | f :: r => file? := some f; rest := r
    | [] => rest := []
  let some file := file? | fail 125 "usage: clight_run FILE.lean [--fuel N] [--stats]"
  let sysroot ← findSysroot
  let extra := (← IO.getEnv "CLIGHT_LEAN_PATH").map (System.SearchPath.parse ·) |>.getD []
  initSearchPath sysroot extra
  enableInitializersExecution
  let t0 ← IO.monoMsNow
  let prog ← match ← loadProg file with
    | .ok p => pure p
    | .error e => return ← fail 125 e
  let t1 ← IO.monoMsNow
  let stdout ← IO.getStdout
  let outcome ← ClightRun.runProgram prog fuel fun b => do stdout.write b; stdout.flush
  stdout.flush
  let t2 ← IO.monoMsNow
  let report (steps : Nat) : IO Unit := do
    if stats then
      IO.eprintln s!"[clight_run] steps={steps} load_ms={t1 - t0} run_ms={t2 - t1}"
  match outcome with
  | .exit code steps => report steps; return (BitVec.ofInt 8 code).toNat.toUInt32
  | .stuck why steps => report steps; fail 126 s!"STUCK (undefined behaviour or outside the interpreter): {why}"
  | .unsupported why steps => report steps; fail 127 s!"UNSUPPORTED: {why}"
  | .fuel steps => report steps; fail 124 s!"FUEL exhausted after {steps} steps"
  | .noinit why => fail 125 s!"NOINIT: {why}"
