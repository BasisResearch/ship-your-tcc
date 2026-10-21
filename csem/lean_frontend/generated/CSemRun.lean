import CerbDriver

/-! # `csem_run`: the CSem oracle (ship-your-tcc, phase F0)

Ours, not upstream's. Runs a C file through the fork's current pipeline:

  `cerberus --cabs-json` (OCaml; the fork's menhir parser) → `CabsImport`
  → Cabs_to_ail → GenTyping → Translation (Core) → Driver/CerbND execution,

and prints the fork's batch verdict (`Defined {value: …, stdout: …}`, an
`Undefined …` line, or an error), exhaustively over the ND tree unless
`--first` is given.

  `csem_run [--first] [cerberus-lean options] FILE.c`

The OCaml front end is `$CSEM_ORACLE` (the `cerberus` driver built by
`scripts/csem_toolchain.sh oracle`), the runtime directory `--runtime DIR`
or `$CSEM_RUNTIME` (the same prefix is given to both, as the fork's lanes
do). Until F1's Lean grammar replaces it, this OCaml step is part of the
spec's front end (PHASES.md, F0).

With any argument list whose last element does not end in `.c`, `csem_run`
is the fork's `cerberus-lean` driver unchanged (`cerbMain`, upstream's
`main` in `CerbDriver.lean`), so the fork's lanes can use it as
`CERBERUS_LEAN_BIN`. Like that driver, it refuses to run unless
`LEAN_ABORT_ON_PANIC` is set. -/

set_option autoImplicit false

namespace CSemRun

/-- The value of `--runtime` in `args` (either spelling), if any. -/
def runtimeOf : List String → Option String
  | [] => none
  | "--runtime" :: v :: _ => some v
  | a :: rest => if a.startsWith "--runtime=" then some (a.drop "--runtime=".length).toString
                 else runtimeOf rest

def fail {α : Type} (msg : String) : IO α := do
  IO.eprintln s!"csem_run: {msg}"
  IO.Process.exit 2

/-- `FILE.c` mode: export the Cabs JSON with the OCaml front end into a
temporary file, then run the fork's batch driver on it. -/
def runC (opts : List String) (src : String) : IO Unit := do
  let some cerb ← IO.getEnv "CSEM_ORACLE"
    | fail "CSEM_ORACLE is not set (the cerberus OCaml driver; scripts/csem_env.sh)"
  let runtime ← match runtimeOf opts with
    | some r => pure r
    | none => match ← IO.getEnv "CSEM_RUNTIME" with
      | some r => pure r
      | none => fail "no runtime: pass --runtime DIR or set CSEM_RUNTIME"
  let out ← IO.Process.output
    { cmd := cerb, args := #[s!"--runtime={runtime}", "--cabs-json", src] }
  if out.exitCode != 0 then
    IO.eprint out.stderr
    IO.eprintln s!"csem_run: front end refused {src} (cerberus --cabs-json exit {out.exitCode})"
    IO.Process.exit 3
  -- the JSON outlives this process's control (cerbMain may exit), so it is
  -- left in $TMPDIR; the lanes point TMPDIR at their own scratch directory
  let (h, json) ← IO.FS.createTempFile
  h.putStr out.stdout
  h.flush
  let json := json.toString
  -- `--first` (single trace) may be given anywhere; the driver wants it
  -- right after `--batch`
  let first := if opts.contains "--first" then ["--first"] else []
  let rest := opts.filter (· != "--first")
  let rest := if (runtimeOf rest).isSome then rest else rest ++ [s!"--runtime={runtime}"]
  cerbMain (["--batch"] ++ first ++ rest ++ [json])

end CSemRun

def main (args : List String) : IO Unit := do
  match args.getLast? with
  | some src =>
    if src.endsWith ".c" then CSemRun.runC args.dropLast src else cerbMain args
  | none => cerbMain args
