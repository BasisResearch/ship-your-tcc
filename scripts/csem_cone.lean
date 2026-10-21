import Tcc.CSem.Statement

/-! # The seam census of `Tcc.CSem.verdicts`' theorem cone (F1b, DESIGN-COMPILER §3.4)

Run under `--csem` (needs TccCSem built):

    lake env lean scripts/csem_cone.lean

Two walks from `Tcc.CSem.verdicts`:

* **kernel cone**: the constant-dependency closure (types and values of every
  reached constant; an `opaque`'s value is walked too, though the kernel never
  unfolds it). This is what a proof about `verdicts` can meet.
* **runtime cone**: the same, also entering each `partial def`'s
  `._unsafe_rec` body (the code that runs, hidden from the kernel behind the
  `opaque`). It shows what a totalization of the reached partials must cover.
  `implemented_by` targets are not entered in either walk (runtime-only
  bodies: panics, natively forced sequencing, Lean core fast paths).

For each walk, one line per reached constant that is

* `PARTIAL`  an `opaque` with a `._unsafe_rec` twin (a `partial def`),
* `OPAQUE`   any other `opaque` (no kernel definition),
* `EXTERN(def|ctor|opaque)` `@[extern]` on a definition (kernel-visible
  reference body), a constructor (kernel-visible), or an opaque (none),
* `IMPLBY(t)` `@[implemented_by t]` (the runtime runs `t`; the kernel sees the
  declaration itself),
* `UNSAFE`, `AXIOM`,

with its declaring module and the first path that reaches it from the root.
`EXTERN(def)`/`EXTERN(ctor)` rows are Lean core primitives with a kernel
definition; they are counted, and listed only when `CSEM_CONE_ALL` is set.
Then the counts and `#print axioms` (must stay within {propext,
Classical.choice, Quot.sound}). Results: results/csem/f1b-seams.md.
-/

open Lean

namespace CSemCone

def modOf (env : Environment) (n : Name) : Name :=
  match env.getModuleIdxFor? n with
  | some i => env.header.moduleNames[i.toNat]!
  | none => `_current

/-- The flags of one constant (empty: an ordinary safe definition). -/
def flags (env : Environment) (n : Name) (ci : ConstantInfo) : List String := Id.run do
  let mut fs : List String := []
  match ci with
  | .opaqueInfo _ =>
    if env.contains (n ++ `_unsafe_rec) then fs := fs ++ ["PARTIAL"] else fs := fs ++ ["OPAQUE"]
  | .axiomInfo _ => fs := fs ++ ["AXIOM"]
  | _ => pure ()
  if isExtern env n then
    fs := fs ++ [match ci with
      | .defnInfo _ => "EXTERN(def)" | .ctorInfo _ => "EXTERN(ctor)" | _ => "EXTERN(opaque)"]
  if let some t := Compiler.getImplementedBy? env n then
    fs := fs ++ [s!"IMPLBY({t})"]
  if ci.isUnsafe then fs := fs ++ ["UNSAFE"]
  return fs

/-- The dependency closure of `roots`, with each constant's first parent;
`runtime` also enters `partial` bodies. -/
def closure (env : Environment) (roots : List Name) (runtime : Bool) :
    Array Name × NameMap Name := Id.run do
  let mut seen : NameSet := {}
  let mut parent : NameMap Name := {}
  let mut out : Array Name := #[]
  let mut todo : Array Name := roots.toArray
  while h : todo.size > 0 do
    let n := todo[0]
    todo := todo.extract 1 todo.size
    if seen.contains n then continue
    seen := seen.insert n
    out := out.push n
    if let some ci := env.find? n then
      let mut used := ci.getUsedConstantsAsSet
      if runtime && ci matches .opaqueInfo _ && env.contains (n ++ `_unsafe_rec) then
        used := used.insert (n ++ `_unsafe_rec)
      for m in used do
        if !seen.contains m then
          if !parent.contains m then parent := parent.insert m n
          todo := todo.push m
  return (out, parent)

def path (parent : NameMap Name) (n : Name) : String := Id.run do
  let mut cur := n
  let mut ps : Array String := #[]
  for _ in [0:6] do
    match parent.find? cur with
    | some p => ps := ps.push (toString p); cur := p
    | none => break
  return " <- ".intercalate ps.toList

def report (env : Environment) (title : String) (roots : List Name) (runtime all : Bool) :
    IO Unit := do
  let (cone, parent) := closure env roots runtime
  IO.println s!"== {title}: cone of {roots}: {cone.size} constants"
  let mut rows : Array (String × String × String) := #[]
  for n in cone do
    if let some ci := env.find? n then
      let fs := flags env n ci
      if !fs.isEmpty then
        rows := rows.push (toString (modOf env n), toString n, " ".intercalate fs)
  rows := rows.qsort (fun a b => a.1 < b.1 || (a.1 == b.1 && a.2.1 < b.2.1))
  let isPrim (f : String) := f == "EXTERN(def)" || f == "EXTERN(ctor)"
  for (m, n, f) in rows do
    if all || !isPrim f then
      IO.println s!"{f}\t{n}\t[{m}]\tvia {path parent n.toName}"
  let has (s : String) (f : String) := (f.splitOn s).length > 1
  let count (p : String → Bool) := (rows.filter fun r => p r.2.2).size
  IO.println s!"counts ({title}): PARTIAL {count (has "PARTIAL")}, OPAQUE {count (has "OPAQUE")}, EXTERN(opaque) {count (has "EXTERN(opaque)")}, IMPLBY {count (has "IMPLBY")}, UNSAFE {count (has "UNSAFE")}, AXIOM {count (has "AXIOM")}; Lean-core primitives EXTERN(def) {count (has "EXTERN(def)")}, EXTERN(ctor) {count (has "EXTERN(ctor)")}"

def run (roots : List Name) : CoreM Unit := do
  let env ← getEnv
  let all := (← IO.getEnv "CSEM_CONE_ALL").isSome
  report env "kernel cone" roots false all
  report env "runtime cone" roots true all
  for r in roots do
    let axs ← collectAxioms r
    IO.println s!"axioms {r}: {axs.toList}"

end CSemCone

#eval CSemCone.run [``Tcc.CSem.verdicts]
