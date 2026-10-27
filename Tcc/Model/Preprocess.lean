import Tcc.Model.Tie
import Spec.Pre.Compare

/-! Exact phase-4 interface. No pretty printing, literal decoding, adjacent
string concatenation or spelling canonicalisation occurs in this projection.
The recursion and loop fuel remain explicit and finite. -/
namespace Tcc.Model
open C

/-- The parser-facing `next()` stream with TOK_NUM/TOK_STR disabled.
Pragma observations travel through macro expansion to their stream position. -/
def preprocessTokens (R : Rec) (observe : Bool := true) : M Unit := do
  modify fun s => { s with
    observePP := observe, error_set_jmp_enabled := 1,
    cversion := 201112, output_type := TCC_OUTPUT_MEMORY, warn_none := 1,
    tcc_lib_path := "/lib/tcc".toUTF8,
    sysinclude_paths := #["/lib/tcc/include".toUTF8, "/include".toUTF8] }
  if (← tcc_open "/prog.c".toUTF8) < 0 then throw (.internal "missing /prog.c")
  preprocess_start R AFF_TYPE_C
  tccgen_init R
  modify fun s => { s with
    parse_flags := PARSE_FLAG_PREPROCESS }
  let fuel ← fuelM
  for _ in [0:fuel] do
    R.next
    if (← getTok) == TOK_EOF then return
    let obs ← ppObservation
    if obs.event != 0 && (← get).ppSeen.contains obs.event then continue
    modify fun s => { s with
      ppObserved := s.ppObserved.push obs,
      ppSeen := if obs.event == 0 then s.ppSeen else s.ppSeen.insert obs.event () }
  throw (.fuel "preprocessTokens")

/-- Image directory nodes, as provided to the host file model by the driver. -/
def ppDirs (files : Std.HashMap ByteArray ByteArray) : Std.HashMap ByteArray Unit := Id.run do
  let mut dirs := ({} : Std.HashMap ByteArray Unit).insert ByteArray.empty ()
  for (p, _) in files do
    for i in [1:p.size] do
      if p.get! i == 47 then dirs := dirs.insert (p.extract 0 i) ()
  return dirs

/-- Pure initial state, with the source installed as a physical file so the
modeled block refiller performs trigraph translation and splicing. -/
def preprocessState (src : ByteArray) (fs : List (CSem.Pre.Bytes × CSem.Pre.Bytes)) : TccSt := Id.run do
  let mut files : Std.HashMap ByteArray ByteArray := {}
  let mut total := src.size
  for (p, b) in fs do
    let p := ByteArray.mk p.toArray
    if !files.contains p then
      files := files.insert p (ByteArray.mk b.toArray)
      total := total + b.length
  files := files.insert "/prog.c".toUTF8 src
  return {
    fsFiles := files, fsDirs := ppDirs files, fds := Array.replicate 3 none,
    fuel := 16 * total + 4096 }

/-- Full observation, including the state after the final token. -/
def runPreprocess (s : TccSt) (observe : Bool := true) : EStateM.Result Err TccSt Unit :=
  (preprocessTokens (Tie.tie s.fuel) observe).run s

/-- The comparison projection is constructor by constructor, preserving bytes. -/
def PPObservation.toSpec (t : PPObservation) : CSem.Pre.PPToken :=
  ⟨match t.kind with
    | .ident => .ident | .number => .ppNumber | .charLit => .charConst
    | .stringLit => .strLit | .punct => .punct | .other => .other
    | .pragmaBegin => .pragmaBegin | .pragmaEnd => .pragmaEnd,
   t.spell.toList⟩
end Tcc.Model

namespace TccModel
open Tcc.Model

/-- Executable named interface from DESIGN §1.1, parameterised by the image FS
for differential fixtures. This uses the modeled lexer, directives and macros. -/
def preprocessWith (fs : List (CSem.Pre.Bytes × CSem.Pre.Bytes))
    (src : CSem.Pre.Bytes) : Except Err (List CSem.Pre.PPToken) :=
  match runPreprocess (preprocessState (ByteArray.mk src.toArray) fs) with
  | .ok _ s => .ok (s.ppObserved.toList.map PPObservation.toSpec)
  | .error e _ => .error e

/-- The image's phase-4 token stream; no host/reference data enters this function. -/
def preprocess (src : CSem.Pre.Bytes) : Except Err (List CSem.Pre.PPToken) :=
  preprocessWith CSem.Pre.Tcc.imageFS.files src

/-- Open spec/model connection, to be supplied by F2's correctness proof.
Differential agreement does not inhabit this proposition. -/
def PreprocessCorrect_Statement (fits : CSem.Pre.Bytes → Prop) : Prop :=
  CSem.Pre.TccppCorrect_Statement preprocess fits

/-- Observable phase-4 effects kept by pragma instrumentation. -/
structure PragmaEffectsEqual (a b : TccSt) : Prop where
  pack : a.pack_stack = b.pack_stack
  packPointer : a.pack_stack_ptr = b.pack_stack_ptr
  counter : a.pp_counter = b.pp_counter
  libraries : a.pragma_libs = b.pragma_libs
  definitions : a.cmdline_defs = b.cmdline_defs

/-- Open observation noninterference: F6 must connect this observer to the
binary's parser-facing stream. Executable comparisons do not prove it. -/
def PragmaObservationEffects_Statement (R : Rec) : Prop :=
  ∀ s : TccSt, s.observePP = false →
    match (pragma_parse R).run s,
        (pragma_parse R).run { s with observePP := true } with
    | .ok b a, .ok b' a' => b = b' ∧ PragmaEffectsEqual a a'
    | .error _ _, .error _ _ => True
    | _, _ => False
/-- Erase only complete event payloads to compare with ordinary `next()`.
This is an observation noninterference obligation, never the spec comparison. -/
def erasePragmaEvents (ts : List PPObservation) : List CSem.Pre.PPToken :=
  let (_, out) := ts.foldl (fun (depth, out) t =>
    match t.kind with
    | .pragmaBegin => (depth + 1, out)
    | .pragmaEnd => (depth - 1, out)
    | _ => if depth == 0 then (depth, t.toSpec :: out) else (depth, out))
    ((0 : Nat), ([] : List CSem.Pre.PPToken))
  out.reverse

/-- Named postcondition for the observer's connection to ordinary model runs. -/
structure PreprocessObservationEqual (observed plain : TccSt) : Prop where
  tokens : erasePragmaEvents observed.ppObserved.toList =
    plain.ppObserved.toList.map PPObservation.toSpec
  effects : PragmaEffectsEqual observed plain

/-- Open whole-run connection between instrumented and ordinary modeled
preprocessing. F6 still owes its separate binary-to-model refinement. -/
def PreprocessObservationNoninterference_Statement : Prop :=
  ∀ src fs,
    let init := preprocessState src fs
    match runPreprocess init, runPreprocess init false with
    | .ok _ a, .ok _ b => PreprocessObservationEqual a b
    | .error e _, .error e' _ => e = e'
    | _, _ => False
end TccModel
