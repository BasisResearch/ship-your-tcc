import CerbMemAllocatorProofs
import CerbND

/-! Runtime adversarial checks of the production allocator, independent of the
arithmetic proof. Finite replay is test input only, never an outcome definition. -/
open CerbMem

def request (sz al : Int) (live : List (Int × Int) := []) : AllocRequest :=
  ⟨0, sz, al, live⟩

def activeAt (st : MemState) (sz al addr : Int) : Bool :=
  match oracleAllocate (fun _ => some addr) sz al st with
  | (NDactive (_, a), _) => a == addr
  | _ => false

def rejects (st : MemState) (sz al : Int) (answer : Option Int) : Bool :=
  match oracleAllocate (fun _ => answer) sz al st with
  | (NDkilled (Other (MerrOther msg)), st') =>
    msg == "CSem: invalid allocation oracle resolution" && st'.allocationRejected
  | _ => false

/-- Exercise the actual region/free APIs. A new lifetime gets fresh provenance,
reuses the released address, and does not inherit the old byte value. -/
def reuse : Bool := Id.run do
  letI : LemFuel := ⟨1000⟩
  letI : CerbGlobal.Switches := ⟨CerbGlobal.defaultSwitches⟩
  let st : MemState := { lastAddress := 4096, allocPolicy := .oracle (fun _ => some 128) }
  let action := nd_bind (allocateRegion 0 (PrefOther "") (.IV .Prov_none 8) (.IV .Prov_none 8)) fun p =>
    nd_bind (ND fun s => (NDactive (), writeBytesTo s 128 [{ value := some 42 }])) fun _ =>
    nd_bind (killM CerbLocation.Loc.unknown true p) fun _ =>
    allocateRegion 0 (PrefOther "") (.IV .Prov_none 8) (.IV .Prov_none 8)
  match CerbND.runND action st with
  | [(Active (.PV (.Prov_some id) (.PVconcrete _ a)), _, st')] =>
    return id == 1 && a == 128 && st'.deadAllocations.contains 0 &&
      (readBytesFrom st' 128 1).all (fun b => b.value.isNone)
  | _ => return false

def main : IO UInt32 := do
  let live : MemState := { lastAddress := 4096, allocations := (Std.TreeMap.empty.insert 0 { base := 128, size := 16 }), nextAllocId := 1 }
  let checks : List (String × Bool) := [
    ("placement 64", activeAt live 8 8 64),
    ("placement 256", activeAt live 8 8 256),
    ("overlap rejected", rejects live 8 8 (some 136)),
    ("unaligned rejected", rejects live 8 8 (some 65)),
    ("zero address rejected", rejects live 8 8 (some 0)),
    ("negative address rejected", rejects live 8 8 (some (-8))),
    ("width rejected", rejects live 8 8 (some pointerLimit)),
    ("end overflow rejected", rejects live 8 8 (some (pointerLimit - 8))),
    ("negative size rejected", rejects live (-8) 8 (some 64)),
    ("zero alignment rejected", rejects live 8 0 (some 64)),
    ("negative alignment rejected", rejects live 8 (-8) (some 64)),
    ("non-power-two alignment rejected", rejects live 8 3 (some 63)),
    ("missing answer rejected, not OOM", rejects live 8 8 none),
    ("zero size is empty", activeAt live 0 8 136),
    ("zero live range is empty", oracleAnswerOk (request 8 8 [(68, 0)]) 64),
    ("touching boundary allowed", activeAt live 8 8 120),
    ("largest representable one-past", oracleAnswerOk (request 7 8) (pointerLimit - 8)),
    ("released range available", activeAt { live with deadAllocations := [0] } 8 8 128),
    ("free/reallocation resets bytes and provenance", reuse) ]
  let mut failed := false
  for (name, ok) in checks do
    IO.println s!"{if ok then "PASS" else "FAIL"} {name}"
    failed := failed || !ok
  return if failed then 1 else 0
