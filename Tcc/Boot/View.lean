import Vsa.Densify

/-!
# Partial memory views and packed byte chunks

A captured cut does not pin the whole memory (tcc's compile phase writes
about 1.25 MB of distinct bytes); it pins the bytes the program phase reads,
as a partial view (ship-your-interpreter's `Vsa/Sim/Boot/Store.lean`
`PartialView`, `readLEv` from its `View.lean`, `insertRange` from
ship-your-ocaml's `Vsa/Sim/Boot/Image.lean`).

Views are given by generated data as `Chunk`s: up to 256 consecutive bytes
packed little-endian into one `Nat` literal (ship-your-ocaml's page
representation), so the kernel reads a byte with one shift and one
`BitVec.ofNat`. `chunksMem cs` is a memory whose reads ARE `chunkView cs`
(`chunksMem_get`), proved by induction over the list, never by evaluating a
map.
-/

namespace Tcc.Boot

/-- The model's memory: a map from byte addresses. -/
abbrev Mem := Std.ExtHashMap Nat (BitVec 8)

/-- Every byte the view defines is present in `m` with that value. -/
def PartialView (m : Mem) (v : Nat → Option (BitVec 8)) : Prop :=
  ∀ k b, v k = some b → m[k]? = some b

/-- Little-endian read of `n` bytes from a map. -/
def readLE (m : Mem) (a : Nat) : Nat → Option Nat
  | 0 => some 0
  | k + 1 => do
    let b ← m[a]?
    let rest ← readLE m (a + 1) k
    pure (b.toNat + 256 * rest)

/-- The same read on a view. -/
def readLEv (v : Nat → Option (BitVec 8)) (a : Nat) : Nat → Option Nat
  | 0 => some 0
  | k + 1 => do
    let b ← v a
    let rest ← readLEv v (a + 1) k
    pure (b.toNat + 256 * rest)

theorem PartialView.get {m : Mem} {v : Nat → Option (BitVec 8)} (h : PartialView m v)
    {k : Nat} {b : BitVec 8} (hb : v k = some b) : m[k]? = some b := h k b hb

theorem PartialView.present {m : Mem} {v : Nat → Option (BitVec 8)} (h : PartialView m v)
    {k : Nat} (hs : (v k).isSome = true) : ∃ b : BitVec 8, m[k]? = some b := by
  obtain ⟨b, hb⟩ := Option.isSome_iff_exists.mp hs
  exact ⟨b, h k b hb⟩

theorem PartialView.readLE {m : Mem} {v : Nat → Option (BitVec 8)} (h : PartialView m v)
    {a n x : Nat} (hr : readLEv v a n = some x) : readLE m a n = some x := by
  induction n generalizing a x with
  | zero => exact hr
  | succ n ih =>
    simp only [readLEv] at hr
    cases hb : v a with
    | none => rw [hb] at hr; cases hr
    | some b =>
      rw [hb] at hr
      cases hrest : readLEv v (a + 1) n with
      | none => rw [hrest] at hr; cases hr
      | some rest =>
        rw [hrest] at hr
        simp only [Tcc.Boot.readLE, h a b hb, ih hrest]
        exact hr

/-- A view survives the densification (`fillZero`) the step lemmas run on. -/
theorem PartialView.fill {m : Mem} {v : Nat → Option (BitVec 8)} (h : PartialView m v) :
    PartialView (Vsa.Densify.fillZeroMem m) v :=
  fun k b hk => Vsa.Densify.fillZeroMem_some (h k b hk)

/-- A smaller view. -/
theorem PartialView.mono {m : Mem} {v w : Nat → Option (BitVec 8)} (h : PartialView m v)
    (hw : ∀ k b, w k = some b → v k = some b) : PartialView m w :=
  fun k b hk => h k b (hw k b hk)

/-! ## Packed chunks -/

/-- `len` (at most 256) bytes from `base`, packed little-endian in `packed`. -/
structure Chunk where
  base : Nat
  len : Nat
  packed : Nat

/-- The byte at absolute address `k` (meaningful for `base ≤ k < base + len`). -/
def Chunk.byte (c : Chunk) (k : Nat) : BitVec 8 :=
  BitVec.ofNat 8 (c.packed >>> (8 * (k - c.base)))

def Chunk.covers (c : Chunk) (k : Nat) : Bool :=
  decide (c.base ≤ k ∧ k < c.base + c.len)

/-- The view of a chunk list: the first chunk covering `k`. -/
def chunkView : List Chunk → Nat → Option (BitVec 8)
  | [], _ => none
  | c :: cs, k => if c.covers k then some (c.byte k) else chunkView cs k

/-- Insert `n` bytes `byte base, …, byte (base + n - 1)`. -/
def insertRange (m : Mem) (base : Nat) (byte : Nat → BitVec 8) : Nat → Mem
  | 0 => m
  | n + 1 => (insertRange m base byte n).insert (base + n) (byte (base + n))

theorem insertRange_get (m : Mem) (base : Nat) (byte : Nat → BitVec 8) (n x : Nat) :
    (insertRange m base byte n)[x]? =
      if base ≤ x ∧ x < base + n then some (byte x) else m[x]? := by
  induction n with
  | zero => simp [insertRange]; omega
  | succ n ih =>
    rw [insertRange, Std.ExtHashMap.getElem?_insert, ih]
    by_cases h : base + n = x
    · subst h; simp
    · simp only [beq_iff_eq, h, ↓reduceIte]
      split <;> split <;> first | rfl | omega

/-- A memory holding exactly the chunks' bytes (earlier chunks win). -/
def chunksMem : List Chunk → Mem
  | [] => ∅
  | c :: cs => insertRange (chunksMem cs) c.base c.byte c.len

theorem chunksMem_get (cs : List Chunk) (x : Nat) : (chunksMem cs)[x]? = chunkView cs x := by
  induction cs with
  | nil => simp [chunksMem, chunkView]
  | cons c cs ih =>
    rw [chunksMem, insertRange_get, chunkView, ih]
    simp only [Chunk.covers, decide_eq_true_eq]

theorem chunksMem_view (cs : List Chunk) : PartialView (chunksMem cs) (chunkView cs) :=
  fun k _ hk => (chunksMem_get cs k).trans hk

/-! ## Checking chunks against a view (one kernel `decide` per chunk) -/

/-- Every byte of `c` is defined by `v` with `c`'s value. -/
def Chunk.agrees (c : Chunk) (v : Nat → Option (BitVec 8)) : Bool :=
  (List.range c.len).all fun j => v (c.base + j) == some (c.byte (c.base + j))

theorem Chunk.agrees_sound {c : Chunk} {v : Nat → Option (BitVec 8)} (h : c.agrees v = true)
    {k : Nat} (hk : c.covers k = true) : v k = some (c.byte k) := by
  simp only [Chunk.covers, decide_eq_true_eq] at hk
  have := List.all_eq_true.mp h (k - c.base) (List.mem_range.mpr (by omega))
  rw [show c.base + (k - c.base) = k by omega] at this
  simpa using this

/-- A chunk list's view is contained in `v` when every chunk agrees with it. -/
theorem chunkView_sub {v : Nat → Option (BitVec 8)} :
    ∀ {cs : List Chunk}, (∀ c ∈ cs, c.agrees v = true) →
      ∀ k b, chunkView cs k = some b → v k = some b
  | [], _, _, _, h => by simp [chunkView] at h
  | c :: cs, hall, k, b, h => by
    simp only [chunkView] at h
    split at h
    · rename_i hk
      cases h
      exact Chunk.agrees_sound (hall c (List.mem_cons_self ..)) hk
    · exact chunkView_sub (fun c' hc' => hall c' (List.mem_cons_of_mem _ hc')) k b h

/-- Membership in a list of half-open ranges `(base, len)`. -/
def inRanges (rs : List (Nat × Nat)) (k : Nat) : Bool :=
  rs.any fun r => decide (r.1 ≤ k ∧ k < r.1 + r.2)

end Tcc.Boot
