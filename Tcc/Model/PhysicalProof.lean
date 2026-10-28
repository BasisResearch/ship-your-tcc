import Tcc.Model.PpLex
import Init.Data.Range.Lemmas
import Init.Data.ByteArray.Lemmas

namespace Tcc.Model.Ghost.PhysicalProof
universe u

/-- The model's table, factored only in the proof: no spec drives execution. -/
def replacement (x : UInt8) : Option UInt8 :=
  match x.toNat with
  | 61 => some 35 | 47 => some 92 | 39 => some 94
  | 40 => some 91 | 41 => some 93 | 33 => some 124
  | 60 => some 123 | 62 => some 125 | 45 => some 126
  | _ => none

/-- A single iteration records the exact consumed half-open interval. -/
def cell (buf : ByteArray) (i : Nat) : PhysicalChar :=
  let c := buf.get! i
  let r := if c == 63 && i + 2 < buf.size && buf.get! (i+1) == 63
    then replacement (buf.get! (i+2)) else none
  match r with
  | some v => { byte := v, start := i, stop := i+3 }
  | none => { byte := c, start := i, stop := i+1 }

theorem cell_start (buf : ByteArray) (i : Nat) : (cell buf i).start = i := by
  simp only [cell]
  split <;> rfl

theorem cell_bounds (buf : ByteArray) (i : Nat) (hi : i < buf.size) :
    i < (cell buf i).stop ∧ (cell buf i).stop ≤ buf.size := by
  simp only [cell]
  split
  · rename_i v h
    split at h
    · rename_i ht
      simp only [Bool.and_eq_true, decide_eq_true_eq] at ht
      dsimp
      omega
    · contradiction
  · dsimp
    omega

/-- Proof-only trace of the actual iterations, indexed by original bytes. -/
def cells (buf : ByteArray) (i : Nat) : List PhysicalChar :=
  if h : i < buf.size then
    cell buf i :: cells buf (cell buf i).stop
  else []
termination_by buf.size - i
decreasing_by have := (cell_bounds buf i h).1; omega

/-- Read arbitrary origin metadata at the original consumed endpoints. -/
def annotate (origins : Array PhysicalChar) (c : PhysicalChar) : PhysicalChar :=
  { byte := c.byte, start := (origins.getD c.start default).start,
    stop := (origins.getD (c.stop-1) default).stop }

abbrev LoopState := ByteArray × Array PhysicalChar × Nat

def body (buf : ByteArray) (origins : Array PhysicalChar) (_ : Nat)
    (st : LoopState) : Id (ForInStep LoopState) :=
  if st.2.2 ≥ buf.size then .done st else
    let c := cell buf st.2.2
    .yield (st.1.push c.byte,
      (if origins.isEmpty then st.2.1 else st.2.1.push (annotate origins c)),
      c.stop)

theorem array_push_cons (out : Array α) (c : α) (cs : List α) :
    out.push c ++ cs.toArray = out ++ (c::cs).toArray := by
  apply Array.ext'
  simp [List.append_assoc]

theorem byte_push_cons (out : ByteArray) (c : UInt8) (cs : List UInt8) :
    out.push c ++ cs.toByteArray = out ++ (c::cs).toByteArray := by
  apply ByteArray.ext
  apply Array.ext'
  simp [ByteArray.data_append, List.data_toByteArray, ByteArray.push, List.append_assoc]

/-- The shared loop invariant describes both projections with one trace. -/
theorem loop_trace (buf : ByteArray) (origins : Array PhysicalChar)
    (n k i : Nat) (mapped : Array PhysicalChar) (out : ByteArray)
    (hi : i ≤ buf.size) (hn : buf.size-i ≤ n) :
    (forIn (List.range' k n) (out,mapped,i) (body buf origins) : Id LoopState) =
      (out ++ ((cells buf i).map (·.byte)).toByteArray,
        mapped ++ (if origins.isEmpty then #[] else
          ((cells buf i).map (annotate origins)).toArray), buf.size) := by
  induction n generalizing k i mapped out with
  | zero =>
    have he : i = buf.size := by omega
    subst i
    simp [cells, pure, bind]
  | succ n ih =>
    by_cases h : i < buf.size
    · rw [List.range'_succ]
      simp only [List.forIn_cons, body, Nat.not_le.mpr h, ↓reduceIte, bind, pure]
      rw [ih (k+1) (cell buf i).stop _ _ (cell_bounds buf i h).2 (by
        have := (cell_bounds buf i h).1; omega)]
      rw [cells.eq_def buf i]
      simp only [h, ↓reduceDIte, ↓reduceIte, List.map_cons]
      cases he : origins.isEmpty <;>
        simp only [he, Bool.false_eq_true, ↓reduceIte, Array.append_empty,
          byte_push_cons, array_push_cons]
    · have he : i = buf.size := by omega
      subst i
      simp [List.range'_succ, body, cells, pure, bind]

/-- Lean generated the same numeric matcher at two declaration sites. Prove
that equality once, before reasoning about the loop's metadata projections. -/
theorem table_matcher_eq :
    @trigraphBlock.match_1.{u} = @replacement.match_1.{u} := by
  unfold trigraphBlock.match_1 replacement.match_1
  rfl

/-- A checked view of the existing executable loop; the definition is unchanged. -/
theorem block_loop (buf : ByteArray) (origins : Array PhysicalChar) :
    trigraphBlock buf origins =
      let st := (forIn [:buf.size] (ByteArray.empty,#[],0) (body buf origins) : Id LoopState)
      (st.1, st.2.1) := by
  unfold trigraphBlock
  apply congrArg (fun st : LoopState => (st.1, st.2.1))
  congr 1
  funext x st
  rcases st with ⟨out,mapped,i⟩
  unfold body cell annotate
  dsimp
  by_cases hi : i ≥ buf.size
  · simp [hi, pure]
  · simp only [hi, ↓reduceIte]
    by_cases ht : (buf.get! i == 63 && decide (i+2 < buf.size) && buf.get! (i+1) == 63) = true
    · simp only [ht, ↓reduceIte]
      rw [table_matcher_eq]
      cases hr : replacement (buf.get! (i+2)) <;>
        simp only [replacement] at hr ⊢ <;>
        rw [hr] <;>
        cases he : origins.isEmpty <;> simp [he, Nat.add_assoc, pure]
    · cases he : origins.isEmpty <;> simp [ht, he, pure]

/-- A list view of the model table, independent of phase-one normalization. -/
def listCells : List UInt8 → Nat → List PhysicalChar
  | [], _ => []
  | [c], i => [{ byte := c, start := i, stop := i+1 }]
  | c :: d :: rest, i =>
    if c == 63 && d == 63 then
      match rest with
      | x :: tail =>
        match replacement x with
        | some r => { byte := r, start := i, stop := i+3 } :: listCells tail (i+3)
        | none => { byte := c, start := i, stop := i+1 } :: listCells (d::x::tail) (i+1)
      | [] => [{ byte := c, start := i, stop := i+1 },
               { byte := d, start := i+1, stop := i+2 }]
    else { byte := c, start := i, stop := i+1 } :: listCells (d::rest) (i+1)
termination_by cs => cs.length

@[simp] theorem listCells_nil (i : Nat) : listCells [] i = [] := by
  rw [listCells.eq_def]

@[simp] theorem listCells_one (c : UInt8) (i : Nat) :
    listCells [c] i = [{ byte := c, start := i, stop := i+1 }] := by
  rw [listCells.eq_def]

theorem listCells_pair (c d : UInt8) (rest : List UInt8) (i : Nat) :
    listCells (c::d::rest) i =
    if c == 63 && d == 63 then
      match rest with
      | x :: tail => match replacement x with
        | some r => { byte := r, start := i, stop := i+3 } :: listCells tail (i+3)
        | none => { byte := c, start := i, stop := i+1 } :: listCells (d::x::tail) (i+1)
      | [] => [{ byte := c, start := i, stop := i+1 },
               { byte := d, start := i+1, stop := i+2 }]
    else { byte := c, start := i, stop := i+1 } :: listCells (d::rest) (i+1) := by
  rw [listCells.eq_def]

theorem listCells_nonQ (c : UInt8) (tail : List UInt8) (i : Nat) (h : c ≠ 63) :
    listCells (c::tail) i = { byte := c, start := i, stop := i+1 } :: listCells tail (i+1) := by
  cases tail with
  | nil => simp
  | cons d rest => rw [listCells_pair]; simp [h]

theorem replacement_spec (x : UInt8) : replacement x = CSem.Pre.trigraphOf x := by
  unfold replacement CSem.Pre.trigraphOf
  simp only [← UInt8.toNat_inj]
  split <;> simp_all

theorem replacement_not_newline (x r : UInt8) (h : replacement x = some r) :
    r ≠ 13 ∧ r ≠ 10 := by
  unfold replacement at h
  split at h <;> cases h <;> decide

/-- An ordinary emitted byte cannot acquire a CRLF transformation. -/
theorem normalize_nonCR (c : PhysicalChar) (cs : List PhysicalChar) (h : c.byte ≠ 13) :
    normalizePhysical (c::cs) = ⟨c.byte,c.start,c.stop,false⟩ :: normalizePhysical cs := by
  cases cs with
  | nil => simp [normalizePhysical]
  | cons d ds => simp [normalizePhysical, h]

/-- Replacing a trigraph cannot create a line-feed byte. -/
theorem listCells_head_nonLF (c : UInt8) (cs : List UInt8) (i : Nat) (h : c ≠ 10) :
    ∀ d ∈ (listCells (c::cs) i).head?.toList, d.byte ≠ 10 := by
  cases cs with
  | nil => simp [listCells, h]
  | cons d rest =>
    rw [listCells_pair]
    split
    · cases rest with
      | nil => simp [h]
      | cons x tail =>
        cases hr : replacement x with
        | none => simp [hr, h]
        | some r => simp [hr, (replacement_not_newline x r hr).2]
    · simp [h]

/-- CR followed by a non-LF head remains a lone physical CR. -/
theorem normalize_CR (i : Nat) (cs : List PhysicalChar)
    (h : ∀ d ∈ cs.head?.toList, d.byte ≠ 10) :
    normalizePhysical ({ byte := 13, start := i, stop := i+1 }::cs) =
      ⟨13,i,i+1,false⟩ :: normalizePhysical cs := by
  cases cs with
  | nil => simp [normalizePhysical]
  | cons d ds =>
    have hd := h d (by simp)
    simp [normalizePhysical, hd]

@[simp] theorem advance_offset (p : CSem.Pre.SourcePoint) (c : UInt8) :
    (CSem.Pre.advancePoint p c).offset = p.offset+1 := by
  unfold CSem.Pre.advancePoint
  split <;> rfl


/-- Both byte and interval observations of the old loop share this equation. -/
theorem block_trace (buf : ByteArray) (origins : Array PhysicalChar) :
    trigraphBlock buf origins =
      (((cells buf 0).map (·.byte)).toByteArray,
       if origins.isEmpty then #[] else ((cells buf 0).map (annotate origins)).toArray) := by
  rw [block_loop, Std.Legacy.Range.forIn_eq_forIn_range']
  simp only [Std.Legacy.Range.size, Nat.sub_zero, Nat.add_sub_cancel_right, Nat.div_one]
  rw [loop_trace buf origins buf.size 0 0 #[] ByteArray.empty (by omega) (by omega)]
  simp

/-- Metadata never changes the block's emitted bytes, even with a malformed map. -/
theorem block_erasure (buf : ByteArray) (origins : Array PhysicalChar) :
    (trigraphBlock buf origins).1 = tcc_trigraphs buf := by
  simp only [tcc_trigraphs, block_trace]

/-- The annotation/output shapes agree for every same-length origin map. -/
theorem block_shape (buf : ByteArray) (origins : Array PhysicalChar)
    (h : origins.size = buf.size) :
    (trigraphBlock buf origins).2.size = (trigraphBlock buf origins).1.size := by
  rw [block_trace]
  cases he : origins.isEmpty with
  | false => simp [he]
  | true =>
    have hz : buf.size = 0 := by
      have : origins.size = 0 := by simpa using he
      omega
    simp [he, cells, hz]

/-- Total byte reads through a remaining-list view; no read presence premise. -/
theorem get_drop (buf : ByteArray) (i n : Nat) :
    buf.get! (i+n) = (buf.data.toList.drop i)[n]! := by
  cases buf with
  | mk data =>
    change data[i+n]! = (data.toList.drop i)[n]!
    rw [← Array.getElem!_toList]
    simp only [List.getElem!_eq_getElem?_getD, List.getElem?_drop]

theorem suffix_length (buf : ByteArray) (i : Nat) (cs : List UInt8)
    (h : buf.data.toList.drop i = cs) : buf.size-i = cs.length := by
  have hh := congrArg List.length h
  simpa only [List.length_drop, Array.length_toList,
    ByteArray.size_data] using hh

theorem suffix_drop (buf : ByteArray) (i : Nat) (cs : List UInt8)
    (h : buf.data.toList.drop i = cs) (n : Nat) : buf.data.toList.drop (i+n) = cs.drop n := by
  simpa only [List.drop_drop] using congrArg (List.drop n) h

/-- The trace reads precisely the original remaining bytes and offsets. -/
theorem cells_suffix (buf : ByteArray) (i : Nat) (cs : List UInt8)
    (h : buf.data.toList.drop i = cs) : cells buf i = listCells cs i := by
  have hlen := suffix_length buf i cs h
  cases cs with
  | nil =>
    have hi : ¬ i < buf.size := by simp only [List.length_nil] at hlen; omega
    simp [cells, listCells, hi]
  | cons c tail =>
    have hi : i < buf.size := by simp only [List.length_cons] at hlen; omega
    have hc : buf.get! i = c := by simpa [h] using get_drop buf i 0
    have ht := suffix_drop buf i (c::tail) h 1
    simp only [List.drop_succ_cons, List.drop_zero] at ht
    rw [cells.eq_def buf i]
    simp only [hi, ↓reduceDIte, ↓reduceIte]
    cases tail with
    | nil =>
      have hshort : ¬ i+2 < buf.size := by simp only [List.length_cons, List.length_nil] at hlen; omega
      have ce : cell buf i = { byte := c, start := i, stop := i+1 } := by simp [cell, hc, hshort]
      rw [ce]
      dsimp only
      rw [cells_suffix buf (i+1) [] ht]
      simp [listCells]
    | cons d rest =>
      have hd : buf.get! (i+1) = d := by simpa [h] using get_drop buf i 1
      by_cases hq : c = 63 ∧ d = 63
      · rcases hq with ⟨rfl,rfl⟩
        cases rest with
        | nil =>
          have hshort : ¬ i+2 < buf.size := by
            simp only [List.length_cons, List.length_nil] at hlen; omega
          have ce : cell buf i = { byte := 63, start := i, stop := i+1 } := by simp [cell, hc, hshort]
          rw [ce]
          dsimp only
          rw [cells_suffix buf (i+1) [63] ht]
          simp only [listCells_pair, beq_self_eq_true, Bool.and_self, ↓reduceIte, listCells_one, Nat.add_assoc]
        | cons x tail =>
          have hx : buf.get! (i+2) = x := by simpa [h] using get_drop buf i 2
          have hlong : i+2 < buf.size := by
            simp only [List.length_cons] at hlen; omega
          cases hr : replacement x with
          | none =>
            have ce : cell buf i = { byte := 63, start := i, stop := i+1 } := by simp [cell, hc, hd, hx, hlong, hr]
            rw [ce]
            dsimp only
            rw [cells_suffix buf (i+1) (63::x::tail) ht]
            rw [listCells_pair 63 63 (x::tail) i]; simp only [beq_self_eq_true, Bool.and_self, ↓reduceIte, hr]
          | some r =>
            have ce : cell buf i = { byte := r, start := i, stop := i+3 } := by simp [cell, hc, hd, hx, hlong, hr]
            have htail := suffix_drop buf i (63::63::x::tail) h 3
            simp only [List.drop_succ_cons, List.drop_zero] at htail
            rw [ce]
            dsimp only
            rw [cells_suffix buf (i+3) tail htail]
            rw [listCells_pair 63 63 (x::tail) i]; simp only [beq_self_eq_true, Bool.and_self, ↓reduceIte, hr]
      · have he : (c == 63 && d == 63) = false := by simp_all
        have ce : cell buf i = { byte := c, start := i, stop := i+1 } := by
          simp only [cell, hc, hd]
          have hb : (c == 63 && decide (i+2 < buf.size) && d == 63) = false := by
            by_cases hc : c = 63 <;> by_cases hd : d = 63 <;> simp_all
          simp [hb]
        rw [ce]
        dsimp only
        rw [cells_suffix buf (i+1) (d::rest) ht]
        rw [listCells_pair c d rest i]; simp only [he, Bool.false_eq_true, ↓reduceIte]
termination_by buf.size-i
decreasing_by all_goals omega

/-- The independent spec observation retains the original offset pair. -/
def specObs (c : CSem.Pre.PChar) : UInt8 × Nat × Nat :=
  (c.c,(c.physical.getD default).1.offset,(c.physical.getD default).2.offset)

def viewObs (c : LogicalChar) : UInt8 × Nat × Nat := (c.byte,c.start,c.stop)

/-- Accumulators and line/column bookkeeping are generalized; offsets are exact. -/
theorem list_read_view (cs : List UInt8) (p : CSem.Pre.SourcePoint)
    (acc : List CSem.Pre.PChar) :
    (CSem.Pre.locatedPhase1 true cs p acc).map specObs =
      acc.reverse.map specObs ++ (normalizePhysical (listCells cs p.offset)).map viewObs := by
  cases cs with
  | nil => simp [CSem.Pre.locatedPhase1, listCells, normalizePhysical]
  | cons c tail =>
    cases tail with
    | nil => simp [CSem.Pre.locatedPhase1, listCells, normalizePhysical,
        specObs, viewObs, List.reverse_cons]
    | cons d rest =>
      by_cases hc : c = 13
      · subst c
        by_cases hd : d = 10
        · subst d
          rw [CSem.Pre.locatedPhase1.eq_def]
          dsimp only
          simp only [true_and, ↓reduceIte]
          rw [list_read_view]
          rw [listCells_nonQ 13 _ _ (by decide), listCells_nonQ 10 _ _ (by decide)]
          simp [normalizePhysical, specObs, viewObs,
            List.reverse_cons, List.append_assoc, Nat.add_assoc]
        · rw [CSem.Pre.locatedPhase1.eq_def]
          dsimp only
          simp only [hd, and_false, ↓reduceIte, Bool.true_eq, and_self, (show (13 : UInt8) ≠ 63 from by decide), false_and]
          rw [list_read_view]
          rw [listCells_pair]
          simp only [(show ((13 : UInt8) == 63) = false from rfl),
            Bool.false_and, Bool.false_eq_true, ↓reduceIte]
          rw [normalize_CR _ _ (listCells_head_nonLF d rest _ hd)]
          simp [specObs, viewObs, List.reverse_cons, List.append_assoc]
      · rw [CSem.Pre.locatedPhase1.eq_def]
        dsimp only
        simp only [hc, false_and, ↓reduceIte, Bool.true_eq, true_and]
        by_cases hq : c = 63 ∧ d = 63
        · rcases hq with ⟨rfl,rfl⟩
          simp only [and_self, ↓reduceIte]
          cases rest with
          | nil =>
            rw [list_read_view]
            simp [listCells, normalizePhysical, specObs, viewObs,
              List.reverse_cons, List.append_assoc, Nat.add_assoc]
          | cons x tail =>
            dsimp only
            cases hr : replacement x with
            | none =>
              rw [← replacement_spec, hr, list_read_view]
              rw [listCells_pair 63 63 (x::tail) p.offset]
              simp only [beq_self_eq_true, Bool.and_self, ↓reduceIte, hr]
              rw [normalize_nonCR _ _ (by change (63 : UInt8) ≠ 13; decide)]
              simp [specObs, viewObs, List.reverse_cons, List.append_assoc]
            | some r =>
              rw [← replacement_spec, hr, list_read_view]
              rw [listCells_pair 63 63 (x::tail) p.offset]
              simp only [beq_self_eq_true, Bool.and_self, ↓reduceIte, hr]
              rw [normalize_nonCR _ _ (replacement_not_newline x r hr).1]
              simp [specObs, viewObs, List.reverse_cons, List.append_assoc, Nat.add_assoc]
        · simp only [hq, ↓reduceIte]
          rw [list_read_view]
          have he : (c == 63 && d == 63) = false := by simp_all
          rw [listCells_pair c d rest p.offset]
          simp only [he, Bool.false_eq_true, ↓reduceIte]
          rw [normalize_nonCR _ _ hc]
          simp [specObs, viewObs, List.reverse_cons, List.append_assoc]
termination_by cs.length
decreasing_by all_goals subst_vars; simp only [List.length_cons, List.length_nil]; omega

/-- The library's executable byte-list loop agrees with its array view. -/
theorem byteList_loop (buf : ByteArray) (i : Nat) (acc : List UInt8) :
    ByteArray.toList.loop buf i acc = acc.reverse ++ buf.data.toList.drop i := by
  rw [ByteArray.toList.loop]
  split
  · rename_i hi
    rw [byteList_loop]
    have hg : buf.data.toList[i]? = some (buf.get! i) := by
      cases buf with
      | mk data =>
        change data.toList[i]? = some data[i]!
        have hi' : i < data.size := hi
        simp [hi']
    rw [List.drop_eq_getElem?_toList_append (l := buf.data.toList) (i := i), hg]
    simp [List.reverse_cons, List.append_assoc]
  · rename_i hi
    have hd : buf.data.toList.drop i = [] := by
      apply List.drop_of_length_le
      simpa using Nat.le_of_not_gt hi
    simp [hd]
termination_by buf.size-i

/-- This observation equality changes neither the byte array nor its iteration. -/
theorem byteList (buf : ByteArray) : buf.toList = buf.data.toList := by
  simpa only [ByteArray.toList, List.reverse_nil, List.nil_append, List.drop_zero] using
    byteList_loop buf 0 []

/-- A bounded total byte read is the original indexed byte. -/
theorem get_bounded (buf : ByteArray) (i : Nat) (hi : i < buf.size) :
    buf.get! i = buf[i] := by
  cases buf with
  | mk data =>
    change data[i]! = data[i]
    have hi' : i < data.size := hi
    simp [hi']

/-- Slice computations reuse the same trace theorem as byte erasure and shape. -/
theorem block_bytes_list (cs : List UInt8) :
    (trigraphBlock cs.toByteArray #[]).1 = ((listCells cs 0).map (·.byte)).toByteArray := by
  rw [block_trace]
  have hs : cells cs.toByteArray 0 = listCells cs 0 := by
    apply cells_suffix
    simp
  simp only [hs]

/-- Each emitted cell translates its exact original byte slice to its one byte. -/
theorem cell_slice (buf : ByteArray) (i : Nat) (hi : i < buf.size) :
    (trigraphBlock (buf.extract (cell buf i).start (cell buf i).stop) #[]).1 =
      [((cell buf i).byte)].toByteArray := by
  simp only [cell]
  split
  · rename_i r hr
    split at hr
    · rename_i ht
      simp only [Bool.and_eq_true, beq_iff_eq, decide_eq_true_eq] at ht
      dsimp only
      rw [ByteArray.extract_add_three (by omega)]
      rw [← get_bounded buf i hi, ← get_bounded buf (i+1) (by omega),
        ← get_bounded buf (i+2) (by omega), ht.1.1, ht.2, block_bytes_list]
      rw [listCells_pair]
      simp [hr]
    · contradiction
  · dsimp only
    rw [ByteArray.extract_add_one (by omega), block_bytes_list]
    simp [get_bounded buf i hi]

structure CellProperties (buf : ByteArray) (c : PhysicalChar) : Prop where
  nonempty : c.start < c.stop
  bounded : c.stop ≤ buf.size
  unremoved : c.removed = false
  slice : (trigraphBlock (buf.extract c.start c.stop) #[]).1 = [c.byte].toByteArray

structure TraceProperties (buf : ByteArray) (i : Nat) (cs : List PhysicalChar) : Prop where
  ordered : cs.Pairwise (fun a b => a.stop ≤ b.start)
  starts : ∀ c ∈ cs, i ≤ c.start
  entries : ∀ c ∈ cs, CellProperties buf c

theorem cell_properties (buf : ByteArray) (i : Nat) (hi : i < buf.size) :
    CellProperties buf (cell buf i) where
  nonempty := by rw [cell_start]; exact (cell_bounds buf i hi).1
  bounded := (cell_bounds buf i hi).2
  unremoved := by simp only [cell]; split <;> rfl
  slice := cell_slice buf i hi

/-- Ordering, bounds and slice meaning are proved together over the shared trace. -/
theorem trace_properties (buf : ByteArray) (i : Nat) : TraceProperties buf i (cells buf i) := by
  rw [cells.eq_def]
  split
  · rename_i hi
    have tail := trace_properties buf (cell buf i).stop
    have head := cell_properties buf i hi
    refine ⟨?_, ?_, ?_⟩
    · exact List.pairwise_cons.mpr ⟨tail.starts,tail.ordered⟩
    · intro c hc
      rcases List.mem_cons.mp hc with hc | hc
      · subst c; rw [cell_start]; exact Nat.le_refl i
      · have h := tail.starts c hc
        have := head.nonempty
        rw [cell_start] at this
        omega
    · intro c hc
      rcases List.mem_cons.mp hc with hc | hc
      · subst c; exact head
      · exact tail.entries c hc
  · exact ⟨by simp,by simp,by simp⟩
termination_by buf.size-i
decreasing_by have := (cell_bounds buf i ‹i < buf.size›).1; omega

/-- Exactly the opening-local physical identity map used by the public contracts. -/
def rawOrigins (buf : ByteArray) : Array PhysicalChar :=
  buf.data.mapIdx (fun i c => { byte := c, start := i, stop := i+1 })

theorem annotate_raw (buf : ByteArray) (c : PhysicalChar) (hc : CellProperties buf c) :
    annotate (rawOrigins buf) c = c := by
  have hs : c.start < buf.data.size := by have := hc.nonempty; have := hc.bounded; simpa using (by omega : c.start < buf.size)
  have he : c.stop-1 < buf.data.size := by have := hc.nonempty; have := hc.bounded; simpa using (by omega : c.stop-1 < buf.size)
  have hp : c.stop-1+1 = c.stop := by have := hc.nonempty; omega
  rcases c with ⟨byte,start,stop,removed⟩
  have hr := hc.unremoved
  dsimp only at hr hs he hp
  simp only [annotate, rawOrigins, Array.getD_eq_getD_getElem?, Array.getElem?_mapIdx]
  rw [getElem?_pos buf.data start hs, getElem?_pos buf.data (stop-1) he]
  simp [hp, hr]

/-- The actual mapped trace equals the model's independently proved physical cells. -/
theorem block_raw (buf : ByteArray) :
    (trigraphBlock buf (rawOrigins buf)).2 = (cells buf 0).toArray := by
  rw [block_trace]
  cases he : (rawOrigins buf).isEmpty with
  | true =>
    have hz : buf.size = 0 := by
      have : (rawOrigins buf).size = 0 := by simpa using he
      simpa [rawOrigins] using this
    simp [he, cells, hz]
  | false =>
    simp only [he, Bool.false_eq_true, ↓reduceIte]
    congr 1
    calc
      (cells buf 0).map (annotate (rawOrigins buf)) = (cells buf 0).map id :=
        List.map_congr_left (fun c hc => annotate_raw buf c ((trace_properties buf 0).entries c hc))
      _ = _ := by simp

theorem origin_map (buf : ByteArray) :
    let mapped := (trigraphBlock buf (rawOrigins buf)).2
    mapped.toList.Pairwise (fun a b => a.stop ≤ b.start) ∧
    ∀ c ∈ mapped.toList, c.start < c.stop ∧ c.stop ≤ buf.size ∧
      (trigraphBlock (buf.extract c.start c.stop) #[]).1 = ⟨#[c.byte]⟩ := by
  rw [block_raw]
  simp only [List.toList_toArray]
  have h := trace_properties buf 0
  refine ⟨h.ordered, ?_⟩
  intro c hc
  have p := h.entries c hc
  exact ⟨p.nonempty,p.bounded,p.slice⟩

theorem read_view (buf : ByteArray) :
    (normalizePhysical (trigraphBlock buf (rawOrigins buf)).2.toList).map viewObs =
      (CSem.Pre.locatedPhase1 true buf.toList ⟨0,1,1⟩ []).map specObs := by
  rw [block_raw]
  simp only [List.toList_toArray]
  rw [byteList, cells_suffix buf 0 buf.data.toList (by simp)]
  simpa using (list_read_view buf.data.toList ⟨0,1,1⟩ []).symm

end Tcc.Model.Ghost.PhysicalProof
