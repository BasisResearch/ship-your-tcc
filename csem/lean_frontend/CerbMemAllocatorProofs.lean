import CerbMem

/-! # CerbMemAllocatorProofs — the concrete allocator's soundness contract

Hand-written proof seam beside its subject, `CerbMem.allocator` — the mirror of
`memory/concrete/impl_mem.ml:1247-1270` `allocator` after remedy 1 of upstream-tray
draft 44 (`docs/upstream-tray/44-concrete-allocator-euclidean-align-down-overlap-at-
exhaustion.md`; charter `docs/2026-09-16_charter-allocator-soundness-address-bound.md`
§2 C1(c); [USER 2026-09-16] ruled it unambiguously wrong and allowed a fix ahead of upstream).

The legacy bump statement over every state, size and alignment is proved from the Euclidean
`Int` division facts (`Int.emod_nonneg`, `Int.emod_def`) and linear arithmetic —
never an enumeration over literals ([USER 2026-09-08]: nothing new out of policy). The
four draft-44 states are a RUNTIME test, `test/Unit/AllocatorSoundnessTest.lean`, which
imports this module so every `test_unit.sh` run compiles the theorem. Kernel-only
tactics (the D14-banned non-kernel decision procedures are absent — the axiom gate's
grep leg enforces it); no `decide` on literals as a load-bearing step; no option
bumps. Axioms: `propext`, `Classical.choice`, `Quot.sound` (the `omega` steps). -/

namespace CerbMem

/-- Observe the allocator's single monadic node on a state — the shape of
    `CerbFail.step` (CerbFailProofs.lean, which sits above `CerbND`), restated so
    this seam depends on `CerbMem` alone. -/
def allocatorStep (sz align : Int) (st : MemState) :
    nd_action (StorageInstanceId × Address) String mem_error
      (mem_constraint IntegerValue) MemState × MemState :=
  match allocator sz align with | ND f => f st

/-- Remedy 1 itself, in kernel terms: when the cursor is below the request the
    allocator KILLS with the out-of-memory error `oomKill` (the NAMED kill,
    seam-hygiene H3) and leaves the state unchanged —
    the regime where pristine `b9aeedcb4` (impl_mem.ml:1253) could return an
    overlapping, misaligned address (draft 44). -/
theorem allocator_below_request_kills (st : MemState) (sz align : Int)
    (hp : st.allocPolicy = .bump) (h : st.lastAddress - sz < 0) :
    allocatorStep sz align st = (NDkilled oomKill, st) := by
  simp [allocatorStep, allocator, hp, bumpAllocator, h]

/-- The allocator's contract: an ACTIVE allocation `a` is `align`-aligned, strictly
    positive, its end `a + sz` at or below the cursor (`a + sz ≤ st.lastAddress` —
    disjoint from everything at or above the cursor, for `sz ≥ 0`; the statement
    itself needs no such hypothesis), and the new cursor is `a`.
    For the bump policy no hypothesis on `sz` or `align` is needed: `align = 0` is the refusal arm
    (never active), the fork's alignment refusal (item G2, 2026-10-20) is a kill
    carrying its `failwithI` as the message (never active, by construction — see
    `allocator`), and for `align ≠ 0` the Euclidean remainder is non-negative, so
    the align-down step only lowers the base — the charter's
    `0 < align → 0 ≤ sz → …` form is this statement weakened. `id`/`a` are the
    `StorageInstanceId`/`Address` components, both abbrevs of `Int`; the relations
    are stated at `Int` (omega reads relations at `Int` only, not at an abbrev). -/
theorem allocator_active_sound (st st' : MemState) (sz align id a : Int)
    (hp : st.allocPolicy = .bump)
    (h : allocatorStep sz align st = (NDactive (id, a), st')) :
    align ∣ a ∧ 0 < a ∧ a + sz ≤ (st.lastAddress : Int) ∧ st'.lastAddress = a := by
  simp only [allocatorStep, allocator, hp, bumpAllocator] at h
  split at h
  · simp at h                       -- z < 0: the out-of-memory kill, never active
  · split at h
    · simp at h                     -- align == 0: the refusal, never active
    · split at h
      · simp at h                   -- alignmentRefused: the fork's refusal (item G2), a kill, never active
      · rename_i hz hne hal
        split at h
        · simp at h                 -- z' ≤ 0: the out-of-memory kill, never active
        · rename_i hz'
          simp only [Prod.mk.injEq, nd_action.NDactive.injEq] at h
          obtain ⟨⟨_, ha⟩, hst⟩ := h
          subst ha
          subst hst
          have hne' : align ≠ 0 := fun heq => by simp [heq] at hne
          have hm := Int.emod_nonneg (st.lastAddress - sz) hne'
          have hdef := Int.emod_def (st.lastAddress - sz) align
          -- `hz'` is stated at the abbrev `Address`; omega reads relations at `Int`
          -- only, so restate it there, then name the two non-linear terms so the
          -- remaining facts are linear in atoms
          have hpos := Int.not_le.mp hz'
          generalize (st.lastAddress - sz) % align = M at hpos hm hdef ⊢
          generalize (st.lastAddress - sz) / align = Q at hdef ⊢
          refine ⟨⟨Q, ?_⟩, ?_, ?_, rfl⟩ <;> omega

/-- Default execution preservation, including every kill arm and all state fields. -/
theorem allocator_default_preserved (st : MemState) (sz align : Int)
    (hp : st.allocPolicy = .bump) :
    allocatorStep sz align st = (match bumpAllocator sz align with | ND f => f st) := by
  simp only [allocatorStep, allocator, hp]
  rfl

/-- Accepted oracle answers satisfy the request's full finite-placement contract. -/
structure PlacementSound (r : AllocRequest) (a : Address) : Prop where
  size_nonnegative : 0 ≤ r.size
  alignment_positive : 0 < r.align
  alignment_supported : alignmentRefused r.align = false
  base_positive : 0 < a
  end_bounded : a + r.size < pointerLimit
  aligned : r.align ∣ a
  fresh : ∀ b s, (b, s) ∈ r.live →
    r.size = 0 ∨ s = 0 ∨ a + r.size ≤ b ∨ b + s ≤ a

theorem oracleAnswerOk_sound (r : AllocRequest) (a : Address)
    (h : oracleAnswerOk r a = true) : PlacementSound r a := by
  simp only [oracleAnswerOk, Bool.and_eq_true, decide_eq_true_eq,
    Bool.not_eq_true', beq_iff_eq] at h
  obtain ⟨⟨⟨⟨⟨⟨hs, hal⟩, hp⟩, ha⟩, hb⟩, hm⟩, hd⟩ := h
  refine ⟨hs, hal, hp, ha, hb, Int.dvd_of_emod_eq_zero hm, ?_⟩
  intro b s hmem
  have := List.all_eq_true.mp hd (b, s) hmem
  simpa only [Bool.or_eq_true, beq_iff_eq, decide_eq_true_eq, or_assoc] using this

/-- Generic active-result freshness, alignment and finite bounds. This theorem
uses the actual pre-allocation live set, not the bump cursor. -/
theorem oracleAllocate_active_sound (choose : AllocRequest → Option Address)
    (st st' : MemState) (sz align id a : Int)
    (h : oracleAllocate choose sz align st = (NDactive (id, a), st')) :
    PlacementSound ⟨st.nextAllocId, sz, align, liveRanges st⟩ a := by
  dsimp only [oracleAllocate] at h
  split at h
  · split at h
    · rename_i answer hchoose hok
      simp only [Prod.mk.injEq, nd_action.NDactive.injEq, Prod.mk.injEq] at h
      obtain ⟨⟨_, rfl⟩, _⟩ := h
      exact oracleAnswerOk_sound _ _ hok
    · simp at h
  · simp at h

/-- The production dispatcher inherits the checked-oracle contract. -/
theorem allocator_oracle_active_sound (choose : AllocRequest → Option Address)
    (st st' : MemState) (sz align id a : Int)
    (hp : st.allocPolicy = .oracle choose)
    (h : allocatorStep sz align st = (NDactive (id, a), st')) :
    PlacementSound ⟨st.nextAllocId, sz, align, liveRanges st⟩ a := by
  apply oracleAllocate_active_sound choose st st' sz align id a
  simpa only [allocatorStep, allocator, hp] using h

/-- Every checked placement is executable, with no finite candidate restriction. -/
theorem oracleAllocate_accepts (choose : AllocRequest → Option Address)
    (st : MemState) (sz align a : Int)
    (hc : choose ⟨st.nextAllocId, sz, align, liveRanges st⟩ = some a)
    (hok : oracleAnswerOk ⟨st.nextAllocId, sz, align, liveRanges st⟩ a = true) :
    oracleAllocate choose sz align st =
      (NDactive (st.nextAllocId, a),
       { st with nextAllocId := st.nextAllocId + 1, lastUsed := some st.nextAllocId }) := by
  simp only [oracleAllocate, hc, hok, ↓reduceIte]

/-- Invalid answers terminate the resolution without updating allocation IDs. -/
theorem oracleAllocate_rejects (choose : AllocRequest → Option Address)
    (st : MemState) (sz align : Int)
    (h : ∀ a, choose ⟨st.nextAllocId, sz, align, liveRanges st⟩ = some a →
      oracleAnswerOk ⟨st.nextAllocId, sz, align, liveRanges st⟩ a = false) :
    oracleAllocate choose sz align st =
      (NDkilled invalidAllocationKill, { st with allocationRejected := true }) := by
  dsimp only [oracleAllocate]
  split
  · rename_i a ha
    simp only [h a ha, Bool.false_eq_true, ↓reduceIte]
  · rfl

/-- The cursor invariant needed only by the unchanged bump resolution. The
runtime memory simulation supplies it; oracle freshness does not use a cursor. -/
structure BumpPlacementPre (st : MemState) (sz align : Int) : Prop where
  size_nonnegative : 0 ≤ sz
  alignment_positive : 0 < align
  alignment_supported : alignmentRefused align = false
  cursor_bounded : st.lastAddress < pointerLimit
  live_above : ∀ b s, (b, s) ∈ liveRanges st → st.lastAddress ≤ b

/-- Generic dispatcher soundness. Bump execution uses its explicit cursor
invariant; checked oracle execution needs no state invariant for byte freshness,
alignment or finite bounds. This quantifies states and requests, not literals. -/
theorem allocator_active_placement_sound (st st' : MemState) (sz align id a : Int)
    (hbump : st.allocPolicy = .bump → BumpPlacementPre st sz align)
    (h : allocatorStep sz align st = (NDactive (id, a), st')) :
    PlacementSound ⟨st.nextAllocId, sz, align, liveRanges st⟩ a := by
  cases hp : st.allocPolicy with
  | bump =>
    have pre := hbump hp
    obtain ⟨hd, ha, hend, _⟩ := allocator_active_sound st st' sz align id a hp h
    refine ⟨pre.size_nonnegative, pre.alignment_positive, pre.alignment_supported,
      ha, Int.lt_of_le_of_lt hend pre.cursor_bounded, hd, ?_⟩
    intro b s hm
    exact Or.inr (Or.inr (Or.inl (Int.le_trans hend (pre.live_above b s hm))))
  | oracle choose =>
    exact allocator_oracle_active_sound choose st st' sz align id a hp h

#print axioms allocator_default_preserved
#print axioms oracleAnswerOk_sound
#print axioms oracleAllocate_active_sound

end CerbMem
