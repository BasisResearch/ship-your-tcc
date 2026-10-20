import Vsa.Sim.WriteLogNF

/-!
# Reading through a write log

The segment layer (`Vsa.Sim.segEval_sound`) states a segment's memory as
`writeLog m log` with a concrete first-order `log`. A concrete run of many
segments needs every byte of such a memory, so here is the one read lemma:
`(writeLog m log)[a]? = (logByte log a).or m[a]?`, with `logByte` the byte
the last entry covering `a` wrote — a function the kernel evaluates on
literals, so a pin through a log is one `decide`.
-/

namespace Tcc.Boot

open Vsa.Sim

/-- The byte one write-log entry leaves at `a`, if it covers `a`
(`applyW`'s per-width images). -/
def entryByte : WEntry → Nat → Option (BitVec 8)
  | (A, 1, d), a => if a = A then some (sbData d) else none
  | (A, 2, d), a =>
    if a = A then some ((shData d).extractLsb' 0 8)
    else if a = A + 1 then some ((shData d).extractLsb' 8 8) else none
  | (A, 4, d), a =>
    if A ≤ a ∧ a < A + 4 then some ((swData d).extractLsb' (8 * (a - A)) 8) else none
  | (A, 8, d), a =>
    if A ≤ a ∧ a < A + 8 then some ((sdData_val d).extractLsb' (8 * (a - A)) 8) else none
  | _, _ => none

/-- The byte the last entry of `log` covering `a` wrote. -/
def logByte : List WEntry → Nat → Option (BitVec 8)
  | [], _ => none
  | e :: log, a => (logByte log a).or (entryByte e a)

private theorem ins_get {α : Type} (m : Std.ExtHashMap Nat α) (k a : Nat) (v : α) :
    (m.insert k v)[a]? = if k = a then some v else m[a]? := by
  rw [Std.ExtHashMap.getElem?_insert]; simp only [beq_iff_eq]

theorem applyW_get (m : Std.ExtHashMap Nat (BitVec 8)) (e : WEntry) (a : Nat) :
    (applyW m e)[a]? = (entryByte e a).or m[a]? := by
  obtain ⟨A, w, d⟩ := e
  by_cases h1 : w = 1
  · subst h1
    show (m.insert A (sbData d))[a]? = _
    simp only [ins_get, entryByte]
    by_cases h : a = A
    · subst h; simp
    · simp [h, Ne.symm h]
  by_cases h2 : w = 2
  · subst h2
    show ((m.insert A _).insert (A + 1) _)[a]? = _
    simp only [ins_get, entryByte]
    by_cases h : a = A
    · subst h; simp
    by_cases h' : a = A + 1
    · subst h'; simp
    simp [h, h', show A ≠ a by omega, show A + 1 ≠ a by omega]
  by_cases h4 : w = 4
  · subst h4
    show (writeMap4 m A (swData d))[a]? = _
    simp only [writeMap4, ins_get, entryByte]
    have : a = A ∨ a = A + 1 ∨ a = A + 2 ∨ a = A + 3 ∨ ¬ (A ≤ a ∧ a < A + 4) := by omega
    rcases this with h | h | h | h | h
    all_goals first
      | (subst h; simp)
      | simp [h, show A ≠ a by omega, show A + 1 ≠ a by omega, show A + 2 ≠ a by omega,
          show A + 3 ≠ a by omega]
  by_cases h8 : w = 8
  · subst h8
    show (writeMap8 m A (sdData_val d))[a]? = _
    simp only [writeMap8, ins_get, entryByte]
    have : a = A ∨ a = A + 1 ∨ a = A + 2 ∨ a = A + 3 ∨ a = A + 4 ∨ a = A + 5 ∨ a = A + 6 ∨
        a = A + 7 ∨ ¬ (A ≤ a ∧ a < A + 8) := by omega
    rcases this with h | h | h | h | h | h | h | h | h
    all_goals first
      | (subst h; simp)
      | simp [h, show A ≠ a by omega, show A + 1 ≠ a by omega, show A + 2 ≠ a by omega,
          show A + 3 ≠ a by omega, show A + 4 ≠ a by omega, show A + 5 ≠ a by omega,
          show A + 6 ≠ a by omega, show A + 7 ≠ a by omega]
  rw [applyW_eq_of_ne m A w d h1 h2 h4 h8]
  have : entryByte (A, w, d) a = none := by
    unfold entryByte; split <;> simp_all
  simp [this]

/-- Reading a memory after a write log. -/
theorem writeLog_get (m : Std.ExtHashMap Nat (BitVec 8)) (log : List WEntry) (a : Nat) :
    (writeLog m log)[a]? = (logByte log a).or m[a]? := by
  induction log generalizing m with
  | nil => simp [writeLog, logByte]
  | cons e log ih =>
    show (writeLog (applyW m e) log)[a]? = _
    rw [ih, applyW_get, logByte, Option.or_assoc]

/-- The byte the first entry of a newest-first log covering `a` wrote. The
replay keeps its logs newest first, so that a row's log is its predecessor's
with entries prepended and the kernel walks it in linear time. -/
def firstByte : List WEntry → Nat → Option (BitVec 8)
  | [], _ => none
  | e :: log, a => (entryByte e a).or (firstByte log a)

theorem logByte_append (l1 l2 : List WEntry) (a : Nat) :
    logByte (l1 ++ l2) a = (logByte l2 a).or (logByte l1 a) := by
  induction l1 with
  | nil => simp [logByte]
  | cons e l ih => simp only [List.cons_append, logByte, ih, Option.or_assoc]

theorem logByte_reverse (log : List WEntry) (a : Nat) :
    logByte log.reverse a = firstByte log a := by
  induction log with
  | nil => rfl
  | cons e l ih =>
    rw [List.reverse_cons, logByte_append, ih]
    simp [logByte, firstByte]

end Tcc.Boot
