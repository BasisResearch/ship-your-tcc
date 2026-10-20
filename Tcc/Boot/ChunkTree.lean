import Tcc.Boot.View

/-!
# Search trees of byte chunks

A partial byte map given by many chunks (an ELF pin has about 4,000) is looked
up in a balanced search tree of them, so the kernel follows a dozen nodes per
byte instead of scanning a list of chunks (`chunkView`). Lookups only ever
return a byte of a leaf chunk that covers the address (`CTree.lookup_some`);
nothing depends on the tree being well formed, an ill-formed one only answers
`none` more often.
-/

namespace Tcc.Boot

inductive CTree where
  | nil : CTree
  | leaf (c : Chunk) : CTree
  | node (split : Nat) (l r : CTree) : CTree

/-- The byte at `k`: a leaf covering it, reached by the split keys. -/
def CTree.lookup : CTree → Nat → Option (BitVec 8)
  | .nil, _ => none
  | .leaf c, k => match Nat.ble c.base k && Nat.blt k (c.base + c.len) with
    | true => some (c.byte k)
    | false => none
  | .node s l r, k => match Nat.ble s k with
    | true => r.lookup k
    | false => l.lookup k

def CTree.leaves : CTree → List Chunk
  | .nil => []
  | .leaf c => [c]
  | .node _ l r => l.leaves ++ r.leaves

theorem CTree.lookup_some : ∀ {t : CTree} {k : Nat} {b : BitVec 8}, t.lookup k = some b →
    ∃ c ∈ t.leaves, c.base ≤ k ∧ k < c.base + c.len ∧ c.byte k = b
  | .nil, _, _, h => nomatch h
  | .leaf c, k, b, h => by
    simp only [CTree.lookup] at h
    split at h
    · rename_i hc
      simp only [Bool.and_eq_true, Nat.ble_eq, Nat.blt_eq] at hc
      exact ⟨c, by simp [CTree.leaves], hc.1, hc.2, Option.some.inj h⟩
    · nomatch h
  | .node s l r, k, b, h => by
    simp only [CTree.lookup] at h
    split at h
    · obtain ⟨c, hc, h1⟩ := CTree.lookup_some h
      exact ⟨c, by simp [CTree.leaves, hc], h1⟩
    · obtain ⟨c, hc, h1⟩ := CTree.lookup_some h
      exact ⟨c, by simp [CTree.leaves, hc], h1⟩

/-- A tree of zero chunks whose leaves are ranges of `rs` marks addresses of `rs`. -/
theorem CTree.inRanges_of {t : CTree} {rs : List (Nat × Nat)}
    (h : t.leaves.all (fun c => rs.contains (c.base, c.len)) = true) {k : Nat}
    (hk : (t.lookup k).isSome = true) : inRanges rs k = true := by
  obtain ⟨b, hb⟩ := Option.isSome_iff_exists.mp hk
  obtain ⟨c, hc, h1, h2, _⟩ := CTree.lookup_some hb
  have := List.all_eq_true.mp h c hc
  simp only [List.contains_iff_mem, decide_eq_true_eq] at this
  simp only [inRanges, List.any_eq_true, decide_eq_true_eq]
  exact ⟨_, List.elem_iff.mp (by simpa using this), h1, h2⟩

end Tcc.Boot
