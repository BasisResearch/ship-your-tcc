import CerbDriver
import Tcc.CSem.Supported
import Tcc.Boot.Reset

/-!
# `tcc_run_correct`, stated over CSem (phase F0)

DESIGN-COMPILER.md §1, in its F0 form: the headline over our fork's current
pipeline. Every definition below is the real one where the fork already has
it, and a named, typed parameter where a later phase supplies it; each
parameter's doc comment says which phase, and PHASES.md lists them (Phase 4,
F0). Nothing is defined as `True`.

What is real now:
* `CSem.verdicts`: the fork's Lean pipeline after the front end, as one pure
  function: `desugar` → `annotate_program` → `translate` (the calls of the
  driver's `frontendTU`, `csem/lean_frontend/CerbDriver.lean`), `link`,
  `initial_driver_state`, `drive`, and the exhaustive ND runner `CerbND.runND`,
  at fuel `n`; the Core standard library and implementation file are the
  fork's `csem/runtime/libcore` texts, parsed by its `CoreParser`.
* `CSem.outcomes`, `CSem.NoUB`: read off `CSem.verdicts` for all large
  enough fuel (fuel is a parameter of no semantic weight in the fork; small
  fuel can reach a pure worker's opaque `fuelExhausted` value, hence
  "eventually").
* `Supported`: the front end accepts and no DESIGN §5 construct occurs
  (`Tcc.CSem.excluded`); decidable.
* `Sail.Observes`: the machine's `Halts`, and divergence with the console as
  the limit of the output.

What is provisional (F0Params, each a later phase's deliverable):
* the front end (`F0Params.frontEnd`): C bytes → Cabs. Until F1's Lean grammar
  (DESIGN §3.1) it stands for the fork's OCaml parser, `cerberus --cabs-json`
  read by `CabsImport`, which is outside Lean; `csem_run` runs exactly that.
* the resource limits (`F0Params.limits`): the static half is F3's (computed
  from `TccModel`), the dynamic half F4/F5's (stack and heap of CSem's
  executions).
* the image (`F0Params.image`): "the fixed runtime image with /prog.c = src";
  the image generator's model, F6 (the witness uses `TccSumElf`).

Known F0 gaps of the definitions themselves (PHASES.md, F0):
* `cfgTcc` supplies the patched RV64I/lp64 implementation reader (F1).
  Binary32/128 execution and bit-field lowering remain concrete residual
  obligations (results/csem/f1-config.md). Runtime allocation resolutions
  include checked finite RV64 placements as well as the unchanged bump baseline.
* Native float operations and partial frontend/Core parsers remain inherited
  seams. The pure per-TU Digest reader is retained.
* divergence: `outcomes` admits `.diverges o` when, for all large fuel, some
  fuel-exhausted branch has printed a prefix of `o`; it does not yet require
  `o` to be exactly the limit of one branch's output (F4's relational outcome
  set replaces this).
-/

namespace Tcc

open Vsa.Machine (Config Halts Diverges StepsN output)

/-- What a run of `tcc -run` shows: the console and exit code of a halting
run, or a diverging run's console as the limit of its output (character `k`,
if ever printed). -/
inductive Observation where
  | halts (out : String) (code : Nat)
  | diverges (out : Nat → Option Char)

/-- A set of observations. -/
structure OutcomeSet where
  mem : Observation → Prop

instance : Membership Observation OutcomeSet := ⟨fun s o => s.mem o⟩

namespace Sail

/-- **`Sail.Observes c₀ obs`**: the machine started at `c₀` shows `obs`:
it halts with console `out` and exit code `e` (`Halts`), or it never halts
and `out` is the limit of its console. -/
def Observes (c₀ : Config) : Observation → Prop
  | .halts out e => Halts c₀ out e
  | .diverges o => Diverges c₀ ∧
      ∀ k ch, o k = some ch ↔ ∃ n c, StepsN n c₀ c ∧ (output c.σ).toList[k]? = some ch

end Sail

namespace CSem

/-- The front end: translation phases 1–7 up to Cabs, giving the TU's symbol
digest and its Cabs. **F0: a parameter** standing for the fork's OCaml parser
(`cerberus --cabs-json`, imported by `CabsImport.parseJson`); F1 replaces it
by the Lean grammar (DESIGN §3.1) and F2 its phases 1–4 by `CSem.Pre`. -/
abbrev FrontEnd := List UInt8 → Option (String × translation_unit)

/-- Actual resolved identities of the two Core runtime source openings.
The image adapter supplies these independently of their diagnostic labels. -/
structure RuntimeIdentities where
  stdCore : List (List UInt8)
  implementation : List (List UInt8)

structure IdentityConfig where
  library : CerbLocation.LibraryConfig
  runtime : RuntimeIdentities

/-- The configuration of the semantics (DESIGN §3.3): the parameters the
fork's driver takes, as data. -/
structure Config where
  frontEnd : FrontEnd
  /-- Implementation-defined ABI and builtin choices, read throughout the pipeline. -/
  implementation : CerbImplCfg.Params
  implFileName : String
  /-- the runtime directory the library files are stamped with (their
  locations are library locations, as in the fork's driver) -/
  runtime : String
  /-- the text of the Core standard library `libcore/std.core` -/
  stdCore : String
  /-- the text of the implementation file `libcore/impls/…impl` -/
  implFile : String
  /-- the allocator's initial cursor (`--address-space-top`) -/
  addressSpaceTop : Int
  /-- `main`'s argv -/
  argv : List String
  /-- the observable exit status of `main`'s value -/
  exitStatus : Int → Nat
  /-- the file system's embedded files other than `/prog.c` (DESIGN §3.3 "filesystem", §3.4 pathleak):
  absolute paths as gen_embed.sh writes them, with their bytes; `verdicts` starts the program in
  `CerbFS.fs_initial_state_of` of this table with `/prog.c` = the source put first, as
  run_tcc_sail.sh's gen_embed.sh call orders it -/
  fsFiles : List (String × List UInt8)
  /-- Absent only for the historical text/JSON F0 interface. Raw configurations
  install explicit membership for Cabs and both Core runtime files. -/
  identity : Option IdentityConfig := none

/-- What one execution (one branch of the ND tree) ends in. -/
inductive Verdict where
  /-- `main` returned (or `exit` was called with) this integer, having
  printed `stdout` -/
  | defined (stdout : String) (exit : Int)
  /-- an undefined behaviour, static (desugaring, typing) or dynamic -/
  | undef
  /-- the model gives no meaning: an elaboration error, a link error, a
  Cerberus error, a non-integer exit value -/
  | refused
  /-- An invalid allocation resolution, distinct from C UB or model refusal. -/
  | invalidAllocation
  /-- this fuel bound was reached on this branch, having printed `stdout` -/
  | outOfFuel (stdout : String)

/-- The Core standard library and implementation, as the fork's driver loads
them (`runPipeline`: `CoreParser.parseLibraryFile`, `loadCoreStdlib`,
`loadCoreImpl`). -/
def stdlib (cfg : Config) : Except String ((Fmap String sym × fun_map Unit) × impl) := do
  let lib := filenameConcat cfg.runtime "libcore"
  let parse (resolved : IdentityConfig → List (List UInt8)) (label text : String) :=
    match cfg.identity with
    | none => @CoreParser.parseLibraryFile ⟨cfg.implementation⟩ label text
    | some c => @CoreParser.parseIdentityFile ⟨cfg.implementation⟩ c.library (resolved c) label text
  let std ← parse (·.runtime.stdCore) (filenameConcat lib "std.core") cfg.stdCore
  let imp ← parse (·.runtime.implementation)
    (filenameConcat (filenameConcat lib "impls") cfg.implFileName) cfg.implFile
  pure (loadCoreStdlib std, loadCoreImpl imp)

/-- An elaboration failure at `loc`: fuel exhaustion, a static UB, or a
refusal. -/
def elabFailure (loc : CerbLocation.Loc) (ub : Bool) : Verdict :=
  if loc == CerbFuel.fuelExhaustedLoc then .outOfFuel ""
  else if ub then .undef else .refused

/-- Identity-aware programs retain their own function definitions. A Core
proxy is used for an unresolved external library declaration, never for a
same-named definition in this TU. The historical oracle interface is unchanged.
Filtering by definition symbols also handles user redeclarations and calls
before the definition, without requiring a standard header for valid forward
library declarations (C11 7.1.4). -/
def identityProxyNames (names : Fmap String sym)
    (prog : ail_program genTypeCategory) : Fmap String sym :=
  Lem_Map.fromList ((fmapElements names).filter fun (name, _) =>
    !prog.2.function_definitions.any (fun (symbol, _) =>
      match symbol with
      | Symbol _ _ (SD_Id declared) => declared == name
      | _ => false))

/-- Cabs → Core for one TU: the fork driver's `frontendTU` without its IO
(same calls, same arguments; the supply starts at 0 as in `runPipeline`). -/
def elaborate [CerbImplCfg.Impl] [LemFuel] [CerbGlobal.Switches] [CerberusFresh.Digest] (cfg : Config)
    (std : (Fmap String sym × fun_map Unit) × impl) (tu : translation_unit) :
    Except Verdict (file Unit × Nat) :=
  let ((ailnames, stdFunMap), coreImpl) := std
  match desugar fmapEmpty fmapEmpty 0 cfg.addressSpaceTop (ailnames, stdFunMap, coreImpl)
      empty_init "main" tu with
  | .Exception (loc, cause) =>
    .error (elabFailure loc (match cause with
      | .DESUGAR (.Desugar_UndefinedBehaviour _) => true
      | _ => false))
  | .Result (_, (mainSym, ailProg), supplyAfterDesugar) =>
    let enumDefs : Fmap sym integerType := Lem_Map.fromList ailProg.enum_definitions
    match to_exception (fun (p : CerbLocation.Loc × typing_error) => (p.1, AIL_TYPING p.2))
        (annotate_program enumDefs fmapEmpty (mainSym, ailProg)) with
    | .Exception (loc, cause) =>
      .error (elabFailure loc (match cause with
        | .AIL_TYPING (.TError_UndefinedBehaviour _) => true
        | _ => false))
    | .Result (typedProg, _annots) =>
      let ailnames := if cfg.identity.isSome then identityProxyNames ailnames typedProg else ailnames
      .ok (translate enumDefs fmapEmpty supplyAfterDesugar (ailnames, stdFunMap) Normal_callconv
        coreImpl typedProg)

/-- What a killed branch has printed (`driver_result`'s `dres_stdout` rule). -/
def stdoutOf (st : driver_state) : String :=
  lemListFoldr String.append "" (toList st.core_state0.io.stdout)

/-- The verdict of one leaf of the ND runner (the fork driver's batch
classification, `runPipeline`). -/
def leafVerdict : nd_status driver_result driver_error driver_state × List String × driver_state →
    Verdict
  | (.Active r, _, _) =>
    match r.dres_core_value with
    | Vloaded (LVspecified (OVinteger iv)) =>
      match eval_integer_value iv with
      | some v => .defined r.dres_stdout v
      | none => .refused
    | _ => .refused
  | (.Killed st reason, _, _) =>
    if st.layout_state.allocationRejected then .invalidAllocation else
    match reason with
    | .Undef0 _ _ => .undef
    | .Error0 loc _ => if loc == CerbFuel.fuelExhaustedLoc then .outOfFuel (stdoutOf st) else .refused
    | .Other _ => .refused

/-- **The executions of `src` at fuel `n`**: front end, elaboration, link,
and the exhaustive ND run of `main` (the fork's `runPipeline` for one TU,
`--nolibc`, the default switch set). -/
def verdictsWithAllocation (policy : CerbMem.AllocPolicy)
    (cfg : Config) (src : List UInt8) (n : Nat) : List Verdict :=
  letI : CerbImplCfg.Impl := ⟨cfg.implementation⟩
  letI : LemFuel := ⟨n⟩
  letI : CerbGlobal.Switches := ⟨CerbGlobal.defaultSwitches⟩
  match cfg.frontEnd src, stdlib cfg with
  | none, _ => [.refused]
  | _, .error _ => [.refused]
  | some (digest, tu), .ok std =>
    letI : CerberusFresh.Digest := ⟨digest⟩
    match elaborate cfg std tu with
    | .error v => [v]
    | .ok (coreFile, supply) =>
      match link [coreFile] with
      | .Exception _ => [.refused]
      | .Result linked =>
        let runFile := convert_file linked
        let (drSt, _) := initial_driver_state supply cfg.addressSpaceTop digest runFile
          (CerbFS.fs_initial_state_of (("/prog.c", src) :: cfg.fsFiles))
        (CerbND.runND (drive runFile.enumDefs runFile.tagDefs false runFile cfg.argv)
          (withAllocationPolicy policy drSt)).map
          leafVerdict

/-- Historical executable baseline: exactly one allocation resolution. -/
def verdicts (cfg : Config) (src : List UInt8) (n : Nat) : List Verdict :=
  verdictsWithAllocation .bump cfg src n

/-- The default-policy specialization preserves the baseline definition. -/
theorem verdicts_default (cfg : Config) (src : List UInt8) (n : Nat) :
    verdictsWithAllocation .bump cfg src n = verdicts cfg src n := rfl

/-- A single allocation resolution witnessing a stable halting observation. Data
fields expose the resolution and bound without an anonymous existential tower. -/
structure HaltingResolution (cfg : Config) (src : List UInt8) (out : String) (e : Nat) where
  policy : CerbMem.AllocPolicy
  exitValue : Int
  exit_status : cfg.exitStatus exitValue = e
  fuelBound : Nat
  computed : ∀ n, fuelBound ≤ n →
    .defined out exitValue ∈ verdictsWithAllocation policy cfg src n

/-- F0's provisional divergence witness, holding allocation policy fixed. Its
single-branch/output-limit adequacy remains F4's existing obligation. -/
structure DivergingResolution (cfg : Config) (src : List UInt8) (o : Nat → Option Char) where
  policy : CerbMem.AllocPolicy
  fuelBound : Nat
  prefixes : ∀ n, fuelBound ≤ n → ∃ out,
    .outOfFuel out ∈ verdictsWithAllocation policy cfg src n ∧
    ∀ k ch, out.toList[k]? = some ch → o k = some ch

/-- **`CSem.outcomes cfg src`**: arbitrary allocation functions range over all
placements, held fixed across fuel bounds. Core's exhaustive evaluation-order
ND remains independent. No sampled placement list defines this set. -/
def outcomes (cfg : Config) (src : List UInt8) : OutcomeSet where
  mem
    | .halts out e => Nonempty (HaltingResolution cfg src out e)
    | .diverges o => Nonempty (DivergingResolution cfg src o)

/-- A halting observation computed under any one resolution is admitted by the
actual outcome set, including the historical bump resolution. -/
theorem halts_of_allocation_resolution (cfg : Config) (src : List UInt8)
    (policy : CerbMem.AllocPolicy) (out : String) (v : Int)
    (h : ∃ N, ∀ n, N ≤ n → .defined out v ∈ verdictsWithAllocation policy cfg src n) :
    Observation.halts out (cfg.exitStatus v) ∈ outcomes cfg src := by
  obtain ⟨N, hn⟩ := h
  exact ⟨{ policy := policy, exitValue := v, exit_status := rfl, fuelBound := N, computed := hn }⟩

/-- **`CSem.NoUB cfg src`**: the front end accepts `src` and, for every large
enough fuel, no execution has undefined behaviour and none is refused by the
model (a refusal has no C meaning to compare against). -/
structure NoUB (cfg : Config) (src : List UInt8) : Prop where
  accepted : (cfg.frontEnd src).isSome
  /-- Rejected allocation resolutions have no C meaning and are neither UB nor
  refusal. A UB reached on any valid prefix still counts, even when that policy
  would reject a later request. Each policy has its own eventual fuel bound. -/
  eventually : ∀ policy : CerbMem.AllocPolicy, ∃ N, ∀ n, N ≤ n →
    ∀ v ∈ verdictsWithAllocation policy cfg src n, v ≠ .undef ∧ v ≠ .refused

end CSem

/-- The provisional parts of the F0 statement, each supplied by a later
phase (PHASES.md, Phase 4 F0). -/
structure F0Params where
  /-- the front end; F1 (the Lean grammar) and F2 (`CSem.Pre`) replace it -/
  frontEnd : CSem.FrontEnd
  /-- the static resource limits, decidable on `src`: the code and data fit
  tcc's 4 MiB arena, `VSTACK_SIZE = 512`, the heap tcc compiles in, include
  and macro-expansion depth; F3 defines it from `TccModel` -/
  staticLimits : List UInt8 → Prop
  /-- the dynamic limits: every allowed execution stays within the machine
  stack (call depth × tcc's frame size) and newlib's heap; F4/F5 define it
  over CSem's executions -/
  dynamicLimits : List UInt8 → Prop
  /-- "`elf` is the fixed runtime image with `/prog.c = src`"; F6 defines it
  (an `ElfPin` of the base image with the embedded file table's `/prog.c`
  entry holding `src`) -/
  image : List UInt8 → ELF64File → Prop
  /-- the image's embedded file table other than `/prog.c` (the header tree, the archives, tcc's own
  files: c/build/fs.list); F6 pins it as the table of the same base image `image` describes -/
  imageFiles : List (String × List UInt8)

/-- **`Supported src`** (DESIGN §1, §5): the front end accepts `src` and no
construct of §5's first table occurs in it. -/
def Supported (fe : CSem.FrontEnd) (src : List UInt8) : Prop :=
  ∃ d tu, fe src = some (d, tu) ∧ CSem.excluded tu = []

instance (fe : CSem.FrontEnd) (src : List UInt8) : Decidable (Supported fe src) :=
  match h : fe src with
  | none => isFalse fun ⟨_, _, h', _⟩ => by rw [h] at h'; cases h'
  | some (d, tu) =>
    if he : CSem.excluded tu = [] then isTrue ⟨d, tu, h, he⟩
    else isFalse fun ⟨_, _, h', he'⟩ => by rw [h] at h'; cases h'; exact he he'

/-- **`Fits src`** (DESIGN §1): the stated resource bound, one named field
per half. -/
structure Fits (P : F0Params) (src : List UInt8) : Prop where
  static : P.staticLimits src
  dynamic : P.dynamicLimits src

/-- **`TccImage src elf`**: `elf` is the `tcc -run` image with `/prog.c = src`. -/
def TccImage (P : F0Params) (src : List UInt8) (elf : ELF64File) : Prop := P.image src elf

/-- **`cfgTcc`** (DESIGN §3.3): patched TinyCC RV64I/lp64, pinned
newlib errno, and the ordinary no-extra-library image command line. The
allocator cursor is an executable resolution, pending allocation recovery;
it is not a claim about the image's stack/data/heap addresses. -/
def cfgTcc (fe : CSem.FrontEnd) (files : List (String × List UInt8)) : CSem.Config where
  frontEnd := fe
  implementation := CerbImplCfg.tccRV64
  implFileName := CerbImplCfg.tccRV64.implFileName
  runtime := "/csem/runtime"
  stdCore := include_str "../../csem/runtime/libcore/std.core"
  implFile := CerbImplCfg.tccRV64.implFileText
  addressSpaceTop := 2 ^ (8 * CerbImplCfg.tccRV64.sizeofPointer) - 1
  /- `tcc -run /prog.c` passes argv = {"/prog.c"} (c/src/gen_embed.sh,
  tcc's main.c) -/
  argv := [CerbImplCfg.tccRV64.argv0]
  /- the image's `_exit` sends `(uint32_t)code` through HTIF (c/src/htif.c) -/
  exitStatus := fun v => (v % 2 ^ CerbImplCfg.tccRV64.exitBits).toNat
  fsFiles := files

/-- **The headline, F0 form** (DESIGN-COMPILER.md §1, no refusal disjunct):
for every C source with no undefined behaviour under CSem, supported and
fitting, any observation of `tcc -run` on the Sail RV64I model from reset on
the image is one CSem allows. -/
def TccRunCorrect_Statement (P : F0Params) : Prop :=
  ∀ src : List UInt8,
  CSem.NoUB (cfgTcc P.frontEnd P.imageFiles) src →
  Supported P.frontEnd src →
  Fits P src →
  ∀ elf, TccImage P src elf →
  ∀ c₀, Boot.ElfReset elf c₀ →
  ∀ obs, Sail.Observes c₀ obs →
  obs ∈ CSem.outcomes (cfgTcc P.frontEnd P.imageFiles) src

/-- The preprocessing spec's interface (DESIGN §1.1), as named placeholders
until F2's `CSem.Pre` (Spec/Pre/) and F3's `TccModel` exist. -/
structure TccppParams where
  /-- preprocessing tokens (F2) -/
  Token : Type
  /-- C11 §5.1.1.2 phases 1–4 with tcc's predefined macros and include path,
  `CSem.Pre.phases14 cfgTcc.pre` (F2) -/
  phases14 : List UInt8 → Option (List Token)
  /-- the token stream `tccpp`'s `next()` delivers, `TccModel.preprocess` (F3) -/
  preprocess : List UInt8 → Option (List Token)

/-- **`tccpp_correct`, F0 form** (DESIGN §1.1). -/
def TccppCorrect_Statement (P : F0Params) (Q : TccppParams) : Prop :=
  ∀ src, Fits P src → Q.preprocess src = Q.phases14 src

/-- `c/tests/p3/sum.c`, the witness program. -/
def sumSrc : List UInt8 := (include_str "../../c/tests/p3/sum.c").toUTF8.toList

/-- **The non-vacuity witness, F0 form** (DESIGN §1.2), for an ELF the
caller pins (F8 instantiates it with `TccSumElf`). -/
def TccRunCorrectWitnessSum_Statement (P : F0Params) (sumElf : ELF64File) : Prop :=
  CSem.NoUB (cfgTcc P.frontEnd P.imageFiles) sumSrc ∧ Supported P.frontEnd sumSrc ∧ Fits P sumSrc ∧
  ∃ c₀ out, Boot.ElfReset sumElf c₀ ∧ Halts c₀ out 0 ∧
    Observation.halts out 0 ∈ CSem.outcomes (cfgTcc P.frontEnd P.imageFiles) sumSrc

end Tcc
