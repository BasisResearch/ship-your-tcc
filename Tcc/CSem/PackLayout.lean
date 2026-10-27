import CSemCParser.Phase7

/-! Fixed reference meaning for tcc's pack alignment cap on ordinary aggregate
members. It does not invent Cabs fields or claim packed-bitfield support.
Actual Cabs/Core layout consumption is an open connection. -/
namespace Tcc.CSem.Raw
open CSem.Pre
open CSemCParser

structure LayoutField where
  byteSize : Nat
  naturalAlignment : Nat
  deriving Inhabited, BEq, Repr

structure LayoutQuery where
  beforeToken : Nat
  union : Bool
  fields : List LayoutField
  deriving Inhabited

structure AggregateLayout where
  offsets : List Nat
  byteSize : Nat
  alignment : Nat
  deriving Inhabited, BEq, Repr

def aligned (offset alignment : Nat) : Nat :=
  if alignment = 0 then offset else (offset + alignment - 1) / alignment * alignment

def cappedAlignment (cap natural : Nat) : Nat :=
  if cap = 0 then natural else min cap natural

/-- Fixed struct/union offsets, extent and aggregate alignment after the cap.
The caller supplies natural field layouts from real semantic type layout. -/
def aggregateLayout (cap : Nat) (isUnion : Bool) (fields : List LayoutField) : AggregateLayout := Id.run do
  let mut offsets := []
  let mut extent := 0
  let mut alignment := 1
  for f in fields do
    let a := max 1 (cappedAlignment cap f.naturalAlignment)
    alignment := max alignment a
    let offset := if isUnion then 0 else aligned extent a
    offsets := offset :: offsets
    extent := max extent (offset + f.byteSize)
  return ⟨offsets.reverse, aligned extent alignment, alignment⟩

/-- Interpret pack commands at a boundary using the implementation's fixed
stack policy (the same explicit check used by phase 4). Malformed commands
remain diagnostics, even if a permissive preprocessing cfg retained them. -/
def packAt (cfg : PreCfg) (input : Phase7.Input) (boundary : Nat) : Except String Nat := do
  let fc : FileCtx := ⟨cfg.mainName, cfg.mainName, 0, none, 1, true, []⟩
  let cfg := { cfg with ext := { cfg.ext with pragmaPack := true } }
  let mut stack := [0]
  for event in input.pragmas do
    if event.beforeToken ≤ boundary then
      let payload := event.payload.map (fun p => ({
        kind := p.token.kind,
        spell := p.token.spell, ws := false, line := (p.origin.site.map (·.line)).getD 0,
        hs := [], origin := p.origin } : CSem.Pre.Tok))
      match checkPragma cfg fc stack payload with
      | .error e => throw e.msg
      | .ok next => stack := next
  return stack.headD 0

def layoutReference (cfg : PreCfg) (input : Phase7.Input) (q : LayoutQuery) :
    Except String AggregateLayout := do
  return aggregateLayout (← packAt cfg input q.beforeToken) q.union q.fields

-- Concrete obstruction to silently using natural layout as packed layout.
#guard aggregateLayout 0 false [⟨1,1⟩,⟨4,4⟩] == ⟨[0,4],8,4⟩
#guard aggregateLayout 1 false [⟨1,1⟩,⟨4,4⟩] == ⟨[0,1],5,1⟩
#guard aggregateLayout 1 true [⟨1,1⟩,⟨4,4⟩] == ⟨[0,0],4,1⟩

end Tcc.CSem.Raw
