/-
  Whole-program runner for Certora's Lean Clight interpreter (CCLib.ClightExec)
  with an executable model of a few libc externals.

  `CC.doStep` is proved sound (`doStep_sound`) but returns `none` at every
  `EF_external` call: CompCert (and the Lean port) leave external functions
  uninterpreted (`externalFunctionsSem` is an axiom).  This runner drives
  `doStep` and, when it is stuck at
      Callstate (External (EF_external name sg) ..) vargs k m
  for a supported `name`, performs the call itself (writing to stdout) and
  resumes with `Returnstate vres k m`, memory unchanged.  Such a step is an
  instance of Clight's `step_external_function` rule *provided* the axiom
  `externalFunctionsSem name sg` admits (vargs, m) ↦ (event, vres, m); that is
  the trusted part of this runner and is outside `doStep_sound`.

  Supported externals:  printf, putchar, puts, exit.  printf arrives either as
  an `EF_external` Callstate (plain clightgen: but then any call with more
  arguments than the prototype is already stuck at `Scall`, because Clight's
  eval_exprlist needs one type per argument) or, with the patched
  `clightgen -fprintf-builtin`, as `Sbuiltin (EF_external "printf" sg)` with the
  actual argument types, which is how `ccomp -interp` sees it too.
  (`ccomp -interp` supports only printf.)  malloc / free / memcpy-builtin are
  already executed by CCLib's own `doExternalCall`.

  printf's output is `Tcc.Clight.format`, the pure formatter of the Phase 3
  printf premise, so this oracle runs the same printf model the proofs assume.
  A call outside that model (`%p`, `%n`, `%a`, `*` widths, an argument of the
  wrong kind or length, ...) ends the run with exit 127.
-/
import CCLib
import Tcc.Clight.Printf

namespace ClightRun
open CC

/-- Why the pure run loop stopped. -/
inductive Stop where
  | done (r : Integers.Int)
  | stuck (s : State)
  | fuel (s : State)

instance : Inhabited Stop := ⟨.done 0⟩

/-- Run `doStep` until a final state, a stuck state, or `limit` steps
    (`limit = 0`: unbounded). Tail-recursive, so it compiles to a loop. -/
partial def runPure (ge : CGenv) (s : State) (n limit : Nat) : Stop × Nat :=
  match s with
  | .Returnstate (.Vint r) .Kstop _ => (.done r, n)
  | _ =>
    if limit != 0 && n ≥ limit then (.fuel s, n) else
    match doStep ge s with
    | some (_, s') => runPure ge s' (n + 1) limit
    | none => (.stuck s, n)

/-- Errors raised by the external-call model. -/
inductive ExtErr where
  | unsupported (msg : String)   -- outside the model (e.g. `%n`, unknown external)
  | ub (msg : String)            -- the call itself is undefined (bad pointer, ...)

abbrev ExtM := ExceptT ExtErr IO

def maxStr : Nat := 1 <<< 24

/-- Read a NUL-terminated C string (at most `lim` bytes) from Clight memory. -/
def readCString (m : Mem) (b : Block) (ofs : Integers.Ptrofs) (lim : Nat := maxStr) :
    Except ExtErr ByteArray := Id.run do
  let base := Integers.Ptrofs.unsigned ofs
  let mut acc := ByteArray.empty
  for i in [0:lim] do
    match Mem.load .Mint8unsigned m b (base + i) with
    | some (.Vint c) =>
      if c.toNat == 0 then return .ok acc
      acc := acc.push c.toNat.toUInt8
    | _ => return .error (.ub s!"string read at offset {base + i} of block is not a defined byte")
  return .ok acc

/-- Integer view of a `putchar`/`exit` argument at a given width (bits). -/
def intArg (v : Val) (bits : Nat) (signed : Bool) : Option Int :=
  let raw : Option Int := match v with
    | .Vint n => some (Integers.Int.signed n)
    | .Vlong n => some (Integers.Int64.signed n)
    | _ => none
  raw.map fun z =>
    let bv := BitVec.ofInt bits z
    if signed then bv.toInt else (bv.toNat : Int)

/-- A printf argument as the event value the printf model reads. A pointer
    becomes `EVptr_global b ofs` naming its block `b` (both are `Positive`),
    so the model's string reader can read any block, not only globals. -/
def eventArg : Val → Option EventVal
  | .Vint n => some (.EVint n)
  | .Vlong n => some (.EVlong n)
  | .Vfloat f => some (.EVfloat f)
  | .Vsingle f => some (.EVsingle f)
  | .Vptr b ofs => some (.EVptr_global b ofs)
  | .Vundef => none

/-- C `printf`: the pure model `Tcc.Clight.format` (the formatter of the
    Phase 3 printf premise `PrintfModel`), reading `%s` strings from `m`. -/
def doPrintf (m : Mem) (fmt : ByteArray) (args : List Val) : Except ExtErr ByteArray := do
  let some evs := args.mapM eventArg
    | throw (.ub "printf: an argument is undefined")
  let rd : Tcc.Clight.StrReader := fun b ofs =>
    match readCString m b ofs with
    | .ok bs => some bs.toList
    | .error _ => none
  match Tcc.Clight.format rd fmt.toList evs with
  | some out => return ⟨out.toArray⟩
  | none => throw (.unsupported "printf: outside the printf model (Tcc.Clight.format)")

/-- Result of an external call the model performed. -/
inductive ExtRes where
  | ret (v : Val)
  | exit (code : Int)

/-- The external-call model. `none`: not an external this model knows. -/
def callExternal (name : String) (vargs : List Val) (m : Mem) :
    Option (Except ExtErr (ByteArray × ExtRes)) :=
  match name, vargs with
  | "printf", .Vptr b ofs :: rest => some do
      let fmt ← readCString m b ofs
      let out ← doPrintf m fmt rest
      return (out, .ret (.Vint (Integers.Int.repr out.size)))
  | "putchar", [v] => some do
      match intArg v 8 false with
      | some z => return (ByteArray.empty.push z.toNat.toUInt8, .ret (.Vint (Integers.Int.repr z)))
      | none => throw (.ub "putchar: argument is not an int")
  | "puts", [.Vptr b ofs] => some do
      let s ← readCString m b ofs
      -- newlib's puts returns 0 on success
      return (s.push '\n'.toUInt8, .ret (.Vint (Integers.Int.repr 0)))
  | "exit", [v] => some do
      match intArg v 32 true with
      | some z => return (ByteArray.empty, .exit z)
      | none => throw (.ub "exit: argument is not an int")
  | _, _ => none

/-- How a whole run ended. -/
inductive Outcome where
  | exit (code : Int) (steps : Nat)
  | stuck (why : String) (steps : Nat)
  | unsupported (why : String) (steps : Nat)
  | fuel (steps : Nat)
  | noinit (why : String)

def describeStuck : State → String
  | .State _ s _ _ _ _ =>
    let k := match s with
      | .Sskip => "Sskip" | .Sassign .. => "Sassign" | .Sset .. => "Sset"
      | .Scall .. => "Scall" | .Sbuiltin _ ef .. =>
          (match ef with
           | .EF_builtin n _ => s!"Sbuiltin {n}" | .EF_external n _ => s!"Sbuiltin(external) {n}"
           | .EF_runtime n _ => s!"Sbuiltin(runtime) {n}"
           | .EF_vload _ => "volatile load" | .EF_vstore _ => "volatile store"
           | .EF_inline_asm .. => "inline asm" | _ => "Sbuiltin")
      | .Ssequence .. => "Ssequence" | .Sifthenelse .. => "Sifthenelse"
      | .Sloop .. => "Sloop" | .Sbreak => "Sbreak" | .Scontinue => "Scontinue"
      | .Sreturn .. => "Sreturn" | .Sswitch .. => "Sswitch" | .Slabel .. => "Slabel"
      | .Sgoto .. => "Sgoto"
    s!"stuck at statement {k}"
  | .Callstate (.External ef _ _ _) _ _ _ =>
    match ef with
    | .EF_external n _ => s!"call to unsupported external function {n}"
    | .EF_builtin n _ => s!"call to unsupported builtin {n}"
    | .EF_runtime n _ => s!"call to runtime helper {n}"
    | .EF_malloc => "malloc failed" | .EF_free => "invalid free"
    | _ => "stuck at external call"
  | .Callstate (.Internal _) _ _ _ => "stuck at function entry"
  | .Returnstate v .Kstop _ => s!"main returned a non-int value {repr v}"
  | .Returnstate .. => "stuck at return"

/-- Run program `p` from `main`, streaming external output through `emit`. -/
def runProgram (p : Program) (limit : Nat) (emit : ByteArray → IO Unit) : IO Outcome := do
  let ge := p.globalenv
  let some m0 := p.initMem | return .noinit "global initialisation failed"
  let some b := Genv.findSymbol ge.genv_genv p.prog_main | return .noinit "no main"
  let some f := Genv.findFunctPtr ge.genv_genv b | return .noinit "main is not a function"
  -- like ccomp -interp's fixup_main: int main(void) / int main() run as is;
  -- int main(int, char **) gets argc = 0 and argv = NULL.
  let args : List Val ← match typeOfFundef f with
    | .Tfunction [] _ _ => pure []
    | .Tfunction [_, _] _ _ => pure [.Vint 0, .Vlong 0]
    | _ => return .noinit "unsupported type for main"
  let mut s : State := .Callstate f args .Kstop m0
  let mut total := 0
  repeat
    let (stop, n) := runPure ge s 0 (if limit == 0 then 0 else limit - total)
    total := total + n
    match stop with
    | .done r => return .exit (Integers.Int.signed r) total
    | .fuel _ => return .fuel total
    | .stuck st =>
      match st with
      | .Callstate (.External (.EF_external name _) _ _ _) vargs k m =>
        match callExternal name vargs m with
        | none => return .stuck (describeStuck st) total
        | some (.error (.ub msg)) => return .stuck msg total
        | some (.error (.unsupported msg)) => return .unsupported msg total
        | some (.ok (out, res)) =>
          emit out
          total := total + 1
          match res with
          | .exit code => return .exit code total
          | .ret v => s := .Returnstate v k m
      -- printf as compiled by `clightgen -fprintf-builtin` (and by ccomp -interp):
      -- Sbuiltin optid (EF_external "printf" sg) tyargs al, sg/tyargs = the
      -- actual argument types.  Clight rule step_builtin + external_call.
      | .State f (.Sbuiltin optid (.EF_external name _) tyargs al) k e le m =>
        match doEvalExprlist ge e le m al tyargs with
        | none => return .stuck s!"cannot evaluate the arguments of builtin {name}" total
        | some vargs =>
          match callExternal name vargs m with
          | none => return .stuck (describeStuck st) total
          | some (.error (.ub msg)) => return .stuck msg total
          | some (.error (.unsupported msg)) => return .unsupported msg total
          | some (.ok (out, res)) =>
            emit out
            total := total + 1
            match res with
            | .exit code => return .exit code total
            | .ret v => s := .State f .Sskip k e (setOpttemp optid v le) m
      | _ => return .stuck (describeStuck st) total
  return .fuel total

end ClightRun
