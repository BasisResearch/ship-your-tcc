import Tcc.Ccp.Eval

/-!
# Lemmas about the evaluator's data structures

Everything the soundness proof (`Tcc.Ccp.Sound`) needs to know about the
evaluator's packed fields, register file, page tree, loads, stores and runs,
stated over `Nat` only. One generic pair of `testBit` lemmas
(`testBit_fldAt`, `testBit_updAt`) gives every field fact.
-/

namespace Tcc.Ccp

/-! ## `Nat.beq` / `Nat.ble` facts -/

theorem beq_of_ne {a b : Nat} (h : a ≠ b) : Nat.beq a b = false := by
  cases h' : Nat.beq a b
  · rfl
  · exact absurd (Nat.eq_of_beq_eq_true h') h

theorem beq_self' (a : Nat) : Nat.beq a a = true := Nat.beq_refl a

theorem ble_of_le {a b : Nat} (h : a ≤ b) : Nat.ble a b = true := Nat.ble_eq.mpr h

theorem ble_of_lt {a b : Nat} (h : b < a) : Nat.ble a b = false := by
  cases h' : Nat.ble a b
  · rfl
  · have := Nat.le_of_ble_eq_true h'; omega

/-! ## Packed fields -/

theorem testBit_fldAt (sh n R k : Nat) :
    (fldAt sh n R).testBit k = (decide (k < n) && R.testBit (sh + k)) := by
  simp [fldAt, Nat.testBit_mod_two_pow, Nat.testBit_shiftRight]

theorem testBit_updAt (sh n R v j : Nat) :
    (updAt sh n R v).testBit j =
      if sh ≤ j ∧ j < sh + n then v.testBit (j - sh) else R.testBit j := by
  simp only [updAt, Nat.testBit_or, Nat.testBit_xor, Nat.testBit_and, Nat.testBit_shiftLeft,
    Nat.testBit_two_pow_sub_one, Nat.testBit_mod_two_pow]
  by_cases h1 : sh ≤ j
  · by_cases h2 : j < sh + n
    · have h3 : j - sh < n := by omega
      simp [h1, h2, h3]
    · have h3 : ¬ j - sh < n := by omega
      simp [h1, h2, h3]
  · have h3 : ¬ (sh ≤ j ∧ j < sh + n) := by omega
    simp [h1, h3]

theorem fldAt_lt (sh n R : Nat) : fldAt sh n R < 2 ^ n := Nat.mod_lt _ (Nat.two_pow_pos n)

/-- Reading back the field just written. -/
theorem fldAt_updAt_same (sh n R v : Nat) : fldAt sh n (updAt sh n R v) = v % 2 ^ n := by
  apply Nat.eq_of_testBit_eq
  intro k
  rw [testBit_fldAt, Nat.testBit_mod_two_pow]
  by_cases hk : k < n
  · rw [testBit_updAt, if_pos ⟨by omega, by omega⟩]; simp [hk]
  · simp [hk]

/-- A field disjoint from the one written is unchanged. -/
theorem fldAt_updAt_disj {sh n sh' n' : Nat} (R v : Nat) (h : sh' + n' ≤ sh ∨ sh + n ≤ sh') :
    fldAt sh' n' (updAt sh n R v) = fldAt sh' n' R := by
  apply Nat.eq_of_testBit_eq
  intro k
  rw [testBit_fldAt, testBit_fldAt]
  by_cases hk : k < n'
  · rw [testBit_updAt, if_neg (by omega)]
  · simp [hk]

/-- A field inside the one written reads the written value's bits. -/
theorem fldAt_updAt_inner {sh n sh' n' : Nat} (R v : Nat) (h1 : sh ≤ sh') (h2 : sh' + n' ≤ sh + n) :
    fldAt sh' n' (updAt sh n R v) = fldAt (sh' - sh) n' v := by
  apply Nat.eq_of_testBit_eq
  intro k
  rw [testBit_fldAt, testBit_fldAt]
  by_cases hk : k < n'
  · rw [testBit_updAt, if_pos (by omega)]
    simp only [hk, decide_true, Bool.true_and]
    congr 1; omega
  · simp [hk]

/-- A field of a field. -/
theorem fldAt_fldAt {sh n sh' n' : Nat} (R : Nat) (h : sh' + n' ≤ n) :
    fldAt sh' n' (fldAt sh n R) = fldAt (sh + sh') n' R := by
  apply Nat.eq_of_testBit_eq
  intro k
  rw [testBit_fldAt, testBit_fldAt, testBit_fldAt]
  by_cases hk : k < n'
  · have : sh' + k < n := by omega
    simp [hk, this, Nat.add_assoc]
  · simp [hk]

/-! ## Registers -/

theorem getR_lt (R i : Nat) : getR R i < 2 ^ 64 := fldAt_lt _ _ _

theorem setR_zero (R v : Nat) : setR R 0 v = R := rfl

theorem setR_ne {R i : Nat} (v : Nat) (h : i ≠ 0) : setR R i v = updAt (64 * i) 64 R v := by
  unfold setR
  rw [beq_of_ne h]

theorem getR_setR (R i v j : Nat) :
    getR (setR R i v) j = if j = i ∧ i ≠ 0 then v % 2 ^ 64 else getR R j := by
  by_cases hi : i = 0
  · subst hi; simp [setR_zero]
  · rw [setR_ne v hi]
    unfold getR
    by_cases hj : j = i
    · subst hj; simp [hi, fldAt_updAt_same]
    · rw [if_neg (by omega)]
      apply fldAt_updAt_disj
      rcases Nat.lt_or_gt_of_ne hj with h | h
      · left; have := Nat.mul_le_mul_left 64 h; omega
      · right; have := Nat.mul_le_mul_left 64 h; omega

/-! ## The page tree -/

theorem MT.get_set (M : MT) (pg d m pg' : Nat) :
    (M.set pg d m).get pg' = if pg' = pg then (M.get pg).map (fun _ => (d, m)) else M.get pg' := by
  induction M with
  | nil => simp [MT.set, MT.get]
  | leaf p d0 m0 =>
    by_cases hp : p = pg
    · subst hp
      by_cases h' : pg' = p
      · subst h'; simp [MT.set, MT.get, beq_self']
      · simp [MT.set, MT.get, h', beq_self', beq_of_ne (Ne.symm h')]
    · by_cases h' : pg' = pg
      · subst h'; simp [MT.set, MT.get, beq_of_ne hp]
      · simp [MT.set, MT.get, beq_of_ne hp, h']
  | node s l r ihl ihr =>
    by_cases hs : s ≤ pg
    · by_cases hs' : s ≤ pg'
      · simp only [MT.set, MT.get, ble_of_le hs, ble_of_le hs', ihr]
      · have hne : pg' ≠ pg := by omega
        simp only [MT.set, MT.get, ble_of_le hs, ble_of_lt (show pg' < s by omega), hne, if_false]
    · by_cases hs' : s ≤ pg'
      · have hne : pg' ≠ pg := by omega
        simp only [MT.set, MT.get, ble_of_lt (show pg < s by omega), ble_of_le hs', hne, if_false]
      · simp only [MT.set, MT.get, ble_of_lt (show pg < s by omega),
          ble_of_lt (show pg' < s by omega), ihl]

/-- The bytes the evaluator knows: byte `a` of its page, if its mask bit is set. -/
def MT.byte (M : MT) (a : Nat) : Option Nat :=
  match M.get (a / 256) with
  | some (d, m) => if m.testBit (a % 256) then some (fldAt (8 * (a % 256)) 8 d) else none
  | none => none

theorem shr_8 (a : Nat) : a >>> 8 = a / 256 := by
  rw [Nat.shiftRight_eq_div_pow]

/-- A load reads known bytes: byte `j` of the value is the memory's byte `a + j`. -/
theorem load_bytes {M : MT} {a w v : Nat} (h : load M a w = some v) :
    v < 2 ^ (8 * w) ∧ ∀ j, j < w → M.byte (a + j) = some (fldAt (8 * j) 8 v) := by
  unfold load at h
  cases hb : Nat.ble (a % 256 + w) 256 with
  | false => simp [hb] at h
  | true =>
    have hle : a % 256 + w ≤ 256 := Nat.ble_eq.mp hb
    simp only [hb, shr_8] at h
    cases hg : M.get (a / 256) with
    | none => simp [hg] at h
    | some dm =>
      obtain ⟨d, m⟩ := dm
      simp only [hg] at h
      cases hm : Nat.beq (fldAt (a % 256) w m) (2 ^ w - 1) with
      | false => simp [hm] at h
      | true =>
        simp only [hm, Option.some.injEq] at h
        subst h
        refine ⟨fldAt_lt _ _ _, fun j hj => ?_⟩
        have hpg : (a + j) / 256 = a / 256 := by omega
        have hoff : (a + j) % 256 = a % 256 + j := by omega
        have hmask : m.testBit (a % 256 + j) = true := by
          have := congrArg (fun x => x.testBit j) (Nat.eq_of_beq_eq_true hm)
          simp only [testBit_fldAt, Nat.testBit_two_pow_sub_one] at this
          simpa [hj] using this
        simp only [MT.byte, hpg, hg, hoff, hmask, if_true, Option.some.injEq]
        rw [fldAt_fldAt d (by omega)]
        congr 1; omega

/-- A store changes exactly the bytes it covers, to the value's bytes. -/
theorem store_bytes {M M' : MT} {a w v : Nat} (h : store M a w v = some M') (x : Nat) :
    M'.byte x = if a ≤ x ∧ x < a + w then some (fldAt (8 * (x - a)) 8 v) else M.byte x := by
  unfold store at h
  cases hb : Nat.ble (a % 256 + w) 256 with
  | false => simp [hb] at h
  | true =>
    have hle : a % 256 + w ≤ 256 := Nat.ble_eq.mp hb
    simp only [hb, shr_8] at h
    cases hg : M.get (a / 256) with
    | none => simp [hg] at h
    | some dm =>
      obtain ⟨d, m⟩ := dm
      simp only [hg] at h
      split at h
      · simp only [Option.some.injEq] at h
        subst h
        unfold MT.byte
        rw [MT.get_set, hg]
        by_cases hx : x / 256 = a / 256
        · simp only [hx, if_true, Option.map_some]
          have hoff : x % 256 = x - a + a % 256 ∨ ¬ (a ≤ x ∧ x < a + w) := by omega
          by_cases hin : a ≤ x ∧ x < a + w
          · have ho : x % 256 = a % 256 + (x - a) := by omega
            have hmask : (updAt (a % 256) w m (2 ^ w - 1)).testBit (x % 256) = true := by
              rw [testBit_updAt, if_pos (by omega), Nat.testBit_two_pow_sub_one]; simp; omega
            simp only [hin, and_self, if_true, hmask, Option.some.injEq]
            rw [fldAt_updAt_inner d v (by omega) (by omega)]
            congr 1; omega
          · have hmask : (updAt (a % 256) w m (2 ^ w - 1)).testBit (x % 256) = m.testBit (x % 256) := by
              rw [testBit_updAt, if_neg (by omega)]
            simp only [hin, if_false, hmask, hg]
            rw [fldAt_updAt_disj d v (by omega)]
        · have hin : ¬ (a ≤ x ∧ x < a + w) := by omega
          simp only [hx, hin, if_false]
      · simp at h

/-! ## Runs -/

theorem run_add (c : Code) : ∀ (m n : Nat) (s : St),
    run c (m + n) s = (run c m s).bind (run c n)
  | 0, n, s => by simp [run]
  | m + 1, n, s => by
    rw [Nat.add_right_comm, run, run]
    cases step c s with
    | none => rfl
    | some s' => exact run_add c m n s'

theorem run_trans {c : Code} {m n : Nat} {s t u : St} (h1 : run c m s = some t)
    (h2 : run c n t = some u) : run c (m + n) s = some u := by
  rw [run_add, h1]; exact h2

end Tcc.Ccp
