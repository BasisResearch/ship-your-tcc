import Tcc.Printf.Spec
import Tcc.Boot.Run

/-!
# printf's machine facts read off replay rows

A replay row (`Tcc.Boot.RunRow v pc L log out`) pins every GPR (`L`), the
console (`out`) and memory as a view `v` overwritten by a write log `log`
(newest first). These lemmas turn a row into the facts `PrintfEntry` and
`PrintfReturn` ask for, each side condition a closed kernel `decide` on the
row's literals: register values by `lookupG`, bytes by `rowByte` (the log's
newest write, else the view), words by `rowWord`.
-/

namespace Tcc.Printf

open LeanRV64DExecutable Vsa.Machine Vsa.Sim Tcc.Boot

/-- The byte a row knows at `a`: the newest write of the log, else the view. -/
def rowByte (v : Nat → Option (BitVec 8)) (log : List WEntry) (a : Nat) : Option (BitVec 8) :=
  (firstByte log a).or (v a)

/-- The little-endian word a row knows at `a` (absent bytes read 0). -/
def rowWord (v : Nat → Option (BitVec 8)) (log : List WEntry) (a : Nat) : Nat :=
  (List.range 8).foldr (fun j acc => ((rowByte v log (a + j)).getD 0).toNat + 256 * acc) 0

theorem rdByte_row {v : Nat → Option (BitVec 8)} {pc : BitVec 64} {L : GRegs}
    {log : List WEntry} {out : String} {c : Config} (h : RunRow v pc L log out c)
    {a : Nat} {b : BitVec 8} (hb : rowByte v log a = some b) : rdByte c.σ a = b.toNat := by
  obtain ⟨m0, hv, hm⟩ := h.mem
  unfold rdByte
  rw [hm, writeLog_get, logByte_reverse, pin_view hv hb]
  rfl

/-- A word whose eight bytes the row knows. -/
theorem rdWord_row {v : Nat → Option (BitVec 8)} {pc : BitVec 64} {L : GRegs}
    {log : List WEntry} {out : String} {c : Config} (h : RunRow v pc L log out c)
    {a : Nat} (hs : ∀ j < 8, (rowByte v log (a + j)).isSome = true) :
    rdWord c.σ a = rowWord v log a := by
  have hj : ∀ j < 8, rdByte c.σ (a + j) = ((rowByte v log (a + j)).getD 0).toNat := by
    intro j hj
    obtain ⟨b, hb⟩ := Option.isSome_iff_exists.mp (hs j hj)
    rw [rdByte_row h hb, hb]
    rfl
  unfold rdWord rowWord
  rw [show List.range 8 = [0, 1, 2, 3, 4, 5, 6, 7] from rfl]
  simp only [List.foldr]
  rw [hj 0 (by decide), hj 1 (by decide), hj 2 (by decide), hj 3 (by decide), hj 4 (by decide),
    hj 5 (by decide), hj 6 (by decide), hj 7 (by decide)]

theorem gprGet_row {v : Nat → Option (BitVec 8)} {pc : BitVec 64} {L : GRegs}
    {log : List WEntry} {out : String} {c : Config} (h : RunRow v pc L log out c)
    {n : Nat} {x : BitVec 64} (hx : lookupG n L = some x) : gprGet c.σ n = some x :=
  gholds_lookup L h.regs hx

/-- Registers two rows pin to the same value agree. -/
theorem gprGet_rows {v v' : Nat → Option (BitVec 8)} {pc pc' : BitVec 64} {L L' : GRegs}
    {log log' : List WEntry} {out out' : String} {c c' : Config}
    (h : RunRow v pc L log out c) (h' : RunRow v' pc' L' log' out' c') {n : Nat}
    (hn : (lookupG n L').isSome = true ∧ lookupG n L' = lookupG n L) :
    gprGet c'.σ n = gprGet c.σ n := by
  obtain ⟨x, hx⟩ := Option.isSome_iff_exists.mp hn.1
  rw [gprGet_row h' hx, gprGet_row h (hn.2 ▸ hx)]

/-- `pending` when `__sinit` has not run (its `__cleanup` word reads 0). -/
theorem pending_of_cleanup {L : StdioLayout} {σ : MState}
    (h : rdWord σ (L.reent + L.cleanupOff) = 0) : pending L σ = [] := by
  simp [pending, h]

/-- `pending` when the buffer is empty (`_p = _bf._base`). -/
theorem pending_of_empty {L : StdioLayout} {σ : MState}
    (h : rdWord σ (L.stdoutFile + L.pOff) = rdWord σ (L.stdoutFile + L.baseOff)) :
    pending L σ = [] := by
  unfold pending
  split <;> simp [h]

theorem consoleBytes_of_row {v : Nat → Option (BitVec 8)} {pc : BitVec 64} {L : GRegs}
    {log : List WEntry} {out : String} {c : Config} (h : RunRow v pc L log out c) :
    consoleBytes c.σ = out.toList.map fun ch => ch.toNat.toUInt8 := by
  unfold consoleBytes
  rw [h.console]

end Tcc.Printf
