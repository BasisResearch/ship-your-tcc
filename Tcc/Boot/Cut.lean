import Tcc.Boot.Regs
import Tcc.Boot.View

/-!
# Captured cuts

A captured cut of `tcc -run P` is a machine configuration at the jump into
P's generated code, described by what the program phase depends on, after
ship-your-interpreter's partial-view entry form (`Vsa/Sim/Boot/Audit.lean`,
`proofElf_halts_entry`: `EntryRegs`, `output σ = ""`, `PartialView`, a tick
bound, any step count):

* `regs`: `GoodState`, the PC, an idle HTIF mailbox, every GPR as captured;
* `view`: every byte the program phase fetches or loads before writing it,
  present with its captured value; nothing is said of any other byte;
* `console`: nothing printed yet;
* `tick`: the platform's tick counter is in range (`plat_insns_per_tick = 2`).

The step count is free: the program phase's behaviour does not depend on it.
-/

namespace Tcc.Boot

open Vsa.Machine

structure CapturedCut (g : Nat → BitVec 64) (pc : BitVec 64) (v : Nat → Option (BitVec 8))
    (c : Config) : Prop where
  regs : EntryRegs c.σ g pc
  view : PartialView c.σ.mem v
  console : output c.σ = ""
  tick : c.tick < 2

/-- The representative configuration of a cut: `cutState` over the memory
holding exactly the view's chunks. -/
def cutConfig (g : Nat → BitVec 64) (pc : BitVec 64) (cs : List Chunk) (steps : Nat) : Config :=
  ⟨cutState (chunksMem cs) g pc, steps % 2, steps⟩

/-- Every captured cut description is satisfiable: its representative
configuration satisfies it. -/
theorem cutConfig_captured (g : Nat → BitVec 64) (pc : BitVec 64) (cs : List Chunk) (steps : Nat) :
    CapturedCut g pc (chunkView cs) (cutConfig g pc cs steps) where
  regs := cutState_entryRegs _ g pc
  view := chunksMem_view cs
  console := cutState_output _ g pc
  tick := Nat.mod_lt _ (by decide)

end Tcc.Boot
