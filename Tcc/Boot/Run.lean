import Tcc.Boot.Cut
import Tcc.Boot.WriteLog
import Vsa.Sim.SegEvalSound
import Vsa.Sim.DeriveCase
import Vsa.Sim.DecodeNF

/-!
# Replaying a captured run segment by segment

From a captured cut every value of the program phase is concrete, so the
program phase can be replayed: one `#derive_case` segment per block of the
trace (`scripts/gen_tcc_segs.py`), chained between rows `RunRow v pc L log`
(a GPR pin list `L`, the program phase's write log `log` over a memory with
the view `v`). `row_step` is the one combinator: it runs a segment with
`segEval_sound` and moves to the next row; every side condition is a closed
kernel `decide` except the segment's `ChainFacts`, which `tcc_facts` closes
from the view (code pins, decode facts, load pins through the log, address
and guard arithmetic).
-/

namespace Tcc.Boot

open LeanRV64DExecutable Vsa Vsa.Machine Vsa.Sim

/-- A row of the replay: at `pc`, the GPRs pinned by `L`, memory = a memory
with the view `v` after the write log `log`, the console holding `out`, the
HTIF mailbox idle. -/
structure RunRow (v : Nat → Option (BitVec 8)) (pc : BitVec 64) (L : GRegs) (log : List WEntry)
    (out : String) (c : Config) : Prop where
  good : GoodState c.σ
  pc : c.σ.regs.get? Register.PC = some pc
  regs : GHolds c.σ L
  keys : KeysOK (keysG L)
  mem : ∃ m0, PartialView m0 v ∧ c.σ.mem = writeLog m0 log
  console : output c.σ = out
  payload : c.σ.regs.get? Register.htif_payload_writes = some (0#4)
  tick : c.tick < 2

/-- The 31 GPRs of a captured cut as a pin list. -/
def cutPins (g : Nat → BitVec 64) : GRegs :=
  (List.range 31).map fun i => (i + 1, g (i + 1))

theorem gholds_of_get {σ : MState} : ∀ (L : GRegs),
    (∀ p ∈ L, gprGet σ p.1 = some p.2) → GHolds σ L
  | [], _ => trivial
  | p :: L, h => ⟨h p (List.mem_cons_self ..), gholds_of_get L fun q hq => h q (List.mem_cons_of_mem _ hq)⟩

theorem EntryRegs.gholds {σ : MState} {g : Nat → BitVec 64} {pc : BitVec 64}
    (E : EntryRegs σ g pc) : GHolds σ (cutPins g) := by
  apply gholds_of_get
  intro p hp
  obtain ⟨i, hi, rfl⟩ := List.mem_map.1 hp
  have hi' := List.mem_range.1 hi
  match i, hi' with
  | 0, h | 1, h | 2, h | 3, h | 4, h | 5, h | 6, h | 7, h | 8, h | 9, h | 10, h | 11, h
  | 12, h | 13, h | 14, h | 15, h | 16, h | 17, h | 18, h | 19, h | 20, h | 21, h | 22, h
  | 23, h | 24, h | 25, h | 26, h | 27, h | 28, h | 29, h | 30, h => exact E.gpr _ h
  | _ + 31, h => exact absurd h (by omega)

theorem cutPins_keys (g : Nat → BitVec 64) : KeysOK (keysG (cutPins g)) := by
  intro n hn
  have : keysG (cutPins g) = (List.range 31).map (· + 1) := by
    simp only [cutPins]
    generalize List.range 31 = l
    induction l with
    | nil => rfl
    | cons a l ih => simp [keysG, ih]
  rw [this] at hn
  obtain ⟨i, hi, rfl⟩ := List.mem_map.1 hn
  have := List.mem_range.1 hi
  omega

/-- A captured cut is the replay's first row. -/
theorem CapturedCut.row {g : Nat → BitVec 64} {pc : BitVec 64} {v : Nat → Option (BitVec 8)}
    {c : Config} (h : CapturedCut g pc v c) : RunRow v pc (cutPins g) [] "" c where
  good := h.regs.good
  pc := h.regs.pc
  regs := h.regs.gholds
  keys := cutPins_keys g
  mem := ⟨c.σ.mem, h.view, rfl⟩
  console := h.console
  payload := h.regs.payload
  tick := h.tick

/-! ## Reading pins through the log -/

/-- The byte one instruction stores at `x`, if any. -/
def stepByte (a : MInstr) (L : GRegs) (x : Nat) : Option (BitVec 8) :=
  match a.kind with
  | .sw | .sd | .sb | .sh => entryByte (wentryM a L) x
  | _ => none

theorem stepMemM_get (m : Std.ExtHashMap Nat (BitVec 8)) (a : MInstr) (L : GRegs) (x : Nat) :
    (stepMemM m a L)[x]? = (stepByte a L x).or m[x]? := by
  cases h : a.kind <;> simp [stepMemM, stepByte, h, applyW_get]

theorem pin_view {m0 : Std.ExtHashMap Nat (BitVec 8)} {v : Nat → Option (BitVec 8)}
    (hv : PartialView m0 v) {o : Option (BitVec 8)} {x : Nat} {b : BitVec 8}
    (h : o.or (v x) = some b) : o.or m0[x]? = some b := by
  cases o with
  | some y => exact h
  | none => exact hv x b h

theorem pinD_view {m0 : Std.ExtHashMap Nat (BitVec 8)} {v : Nat → Option (BitVec 8)}
    (hv : PartialView m0 v) {o : Option (BitVec 8)} {x : Nat} {b : BitVec 8}
    (h : (o.or (v x)).isSome = true ∧ (o.or (v x)).getD 0 = b) : (o.or m0[x]?).getD 0 = b := by
  obtain ⟨hs, hb⟩ := h
  obtain ⟨y, hy⟩ := Option.isSome_iff_exists.mp hs
  rw [pin_view hv hy]; rw [hy] at hb; exact hb

open Lean Elab Tactic Meta in
/-- Replace the goal by its weak-head normal form (fails without progress). -/
elab "tcc_whnf" : tactic => liftMetaTactic fun g => do
  let t ← g.getType
  let t' ← whnfD t
  if t == t' then throwError "tcc_whnf: no progress"
  return [← g.replaceTargetDefEq t']

/-- Close a segment's `ChainFacts` from a view hypothesis `hv : PartialView m0 v`
(memory `writeLog m0 log`): split conjunctions, decode facts by `decodeW`,
byte pins through the log by `pin_view`/`pinD_view` and one kernel `decide`,
closed arithmetic by kernel `decide`, unfolding one layer otherwise. -/
macro "tcc_facts " hv:term : tactic => `(tactic| repeat' (first
  | exact True.intro
  | (refine ⟨?_, ?_⟩)
  | (intro s h1 h2 h3; exact Vsa.Sim.decodeW s h1 h2 h3)
  | (simp only [Tcc.Boot.writeLog_get, Tcc.Boot.stepMemM_get, ← Option.or_assoc]
     first
       | (apply Tcc.Boot.pin_view $hv; decide +kernel)
       | (apply Tcc.Boot.pinD_view $hv; decide +kernel))
  | decide +kernel
  | tcc_whnf))

/-! ## The segment combinator -/

/-- `P` leads to `Q`: every configuration satisfying `P` runs to one
satisfying `Q`. -/
def Leads (P Q : Config → Prop) : Prop := ∀ c, P c → ∃ c', Steps c c' ∧ Q c'

theorem Leads.trans {P Q R : Config → Prop} (h1 : Leads P Q) (h2 : Leads Q R) : Leads P R := by
  intro c hc
  obtain ⟨c1, s1, q1⟩ := h1 c hc
  obtain ⟨c2, s2, r2⟩ := h2 c1 q1
  exact ⟨c2, Steps.trans s1 s2, r2⟩

theorem gholds_of_lookup {σ : MState} {L : GRegs} (hL : GHolds σ L) :
    ∀ (L' : GRegs), (∀ p ∈ L', lookupG p.1 L = some p.2) → GHolds σ L' :=
  fun L' h => gholds_of_get L' fun p hp => gholds_lookup L hL (h p hp)

theorem gprReg_ne_payload : ∀ n, (gprReg n == Register.htif_payload_writes) = false := by
  intro n; unfold gprReg; split <;> decide

/-- Run one generated segment `bs` from a row to the next. -/
theorem row_step {v : Nat → Option (BitVec 8)} (bs : List BBlock) {pc0 pc1 : BitVec 64}
    {L0 L1 : GRegs} {log0 log1 : List WEntry} {out : String} (lds : List (List (BitVec 8)))
    (hwf : ChainOK pc0 (keysG L0) bs)
    (hfacts : ∀ m0, PartialView m0 v →
      ChainFacts (writeLog m0 log0) (writeLog m0 log0) L0 lds bs)
    (hpc : evalBlocksPC pc0 (SegEvalState.init L0 lds) bs = pc1)
    (hregs : ∀ p ∈ L1, lookupG p.1 (evalBlocks bs (SegEvalState.init L0 lds)).regs = some p.2)
    (hkeys : KeysOK (keysG L1))
    (hlog : log0 ++ (evalBlocks bs (SegEvalState.init L0 lds)).log = log1) :
    Leads (RunRow v pc0 L0 log0 out) (RunRow v pc1 L1 log1 out) := by
  rintro ⟨σ, i, u⟩ ⟨hG, hpc0, hL, hk, ⟨m0, hv, hm⟩, hout, hpw, hi⟩
  obtain ⟨vm, hmi⟩ := hG.minstret
  have hf := hfacts m0 hv
  rw [← hm] at hf
  obtain ⟨σ', i', hs, hi', hG', hmem', hso, hpc', _, hregs', hframe⟩ :=
    segEval_sound bs σ i u pc0 vm L0 lds hG hpc0 hmi hL hk hf hwf hi
  refine ⟨⟨σ', i', u + evalBlocksFuel bs⟩, hs, hG', ?_, gholds_of_lookup hregs' L1 hregs, hkeys,
    ⟨m0, hv, ?_⟩, ?_, ?_, hi'⟩
  · rw [hpc', hpc]
  · simp only at hmem' ⊢
    rw [hmem', hm, ← writeLog_append, hlog]
  · simp only [output] at hout ⊢
    rw [hso]; exact hout
  · rw [hframe _ (by decide) (fun n _ => gprReg_ne_payload n)]; exact hpw

end Tcc.Boot
