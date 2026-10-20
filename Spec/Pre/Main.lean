/-
`CSem.Pre.phases14`: C11 translation phases 1–4 (§5.1.1.2) on a source file.

The predefined-macro text `cfg.predefs` is processed first, as a source file
named `cfg.predefsName` (it defines the non-dynamic predefined macros of
§6.10.8 and the implementation's own, and may include headers, as tcc's
`#include <tccdefs.h>` does); then the main file, named `cfg.mainName`.
The output is the token sequence phase 5 receives: the tokens of the
predefined text (if it has any) followed by those of the main file, with
pragma markers where a `#pragma`/`_Pragma` is left to later phases.

Totality: every loop is structurally recursive on a fuel argument. The fuel
is `fuelBound`, a function of the input size (source, predefined text, file
system); exhausting it is the error `ErrKind.fuel`, which the resource
premise of the eventual theorem (`Fits`) excludes.
-/
import Spec.Pre.Phase4

namespace CSem.Pre

/-- The fuel: quadratic in the total input size. Each step of a loop consumes
one unit; the bound exceeds the work on every input whose macro expansion
is not super-quadratic in its size. -/
def fuelBound (cfg : PreCfg) (fs : FileSys) (src : Bytes) : Nat :=
  let n := src.length + cfg.predefs.length + fs.size + 1
  64 * n * n

/-- The initial phase-4 state. -/
def initState (cfg : PreCfg) : PState :=
  { macros := initMacros cfg, counter := 0, once := [], pushed := [], out := [] }

/-- C11 translation phases 1–4 of the main source file `src`. -/
def phases14 (cfg : PreCfg) (fs : FileSys) (src : Bytes) : Except PreError (List PPToken) := do
  let fuel := fuelBound cfg fs src
  let pre ← phases123 cfg cfg.predefsName cfg.predefs
  let st ← procFile cfg fs fuel (initState cfg)
    ⟨cfg.predefsName, cfg.predefsName, 0, none, 2, false⟩ pre true []
  let main ← phases123 cfg cfg.mainName src
  let st ← procFile cfg fs fuel st ⟨cfg.mainName, cfg.mainName, 0, none, 1, true⟩ main true []
  pure st.out.reverse

end CSem.Pre
