import Lake
open Lake DSL System

/-! Runner for Certora/clean's Lean Clight interpreter.  Depends on the
    `export/` Lake project of the Certora checkout under build/certora. -/

package clightrun

require «compcert-clight-lean» from ".." / ".." / "build" / "certora" / "src" / "export"

lean_lib ClightRun

target fmt.o pkg : FilePath := do
  let oFile := pkg.buildDir / "c" / "fmt.o"
  let srcJob ← inputTextFile <| pkg.dir / "c" / "fmt.c"
  let weakArgs := #["-I", (← getLeanIncludeDir).toString]
  buildO oFile srcJob weakArgs #["-fPIC", "-O2"] "cc" getLeanTrace

extern_lib libclightfmt pkg := do
  let fmtO ← fetch <| pkg.target ``fmt.o
  let name := nameToStaticLib "clightfmt"
  buildStaticLib (pkg.staticLibDir / name) #[fmtO]

@[default_target]
lean_exe clight_run where
  root := `Main
  supportInterpreter := true
