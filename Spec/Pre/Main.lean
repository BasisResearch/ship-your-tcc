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
def phases14WithOrigins (cfg : PreCfg) (fs : FileSys) (src : Bytes) : Except PreError PreprocessingResult := do
  let fuel := fuelBound cfg fs src
  let pre ← phases123 cfg cfg.predefsName cfg.predefs
  let initial := { (initState cfg) with sources :=
    [⟨cfg.predefsName, FileSys.normComponents cfg.predefsName, cfg.predefs, none⟩] }
  let st ← procFile cfg fs fuel initial
    { path := cfg.predefsName
      presumed := cfg.predefsName
      lineDelta := 0
      foundAt := none
      level := 2
      isMain := false } pre true []
  let main ← phases123 cfg cfg.mainName src
  let st := { st with sources :=
    ⟨cfg.mainName, FileSys.normComponents cfg.mainName, src, none⟩ :: st.sources }
  let st ← procFile cfg fs fuel st
    { path := cfg.mainName
      presumed := cfg.mainName
      lineDelta := 0
      foundAt := none
      level := 1
      isMain := true } main true []
  pure ⟨st.locatedOut.reverse, st.sources.reverse, st.pack⟩

/-- The strict F2 stream is the erasure of the provenance-carrying run.
Spellings, pragma boundaries and their order are unchanged. -/
def phases14 (cfg : PreCfg) (fs : FileSys) (src : Bytes) : Except PreError (List PPToken) :=
  (phases14WithOrigins cfg fs src).map PreprocessingResult.erase

/-- Structural erasure, not a proof of C11 preprocessing correctness. -/
theorem phases14WithOrigins_erase (cfg : PreCfg) (fs : FileSys) (src : Bytes) :
    (phases14WithOrigins cfg fs src).map PreprocessingResult.erase = phases14 cfg fs src := rfl

end CSem.Pre
