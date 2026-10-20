import Tcc.Printf.Spec
import Tcc.Clight.Run

/-!
# An external-call semantics whose printf is the image's

`machX p L Img sym` is an `ExtEnv` for Certora's Clight whose `printf` clause
is read off the machine, not chosen: a printf step on arguments `evargs`
returning `n` exists exactly when the image (from a reset configuration `Img`
admits) reaches newlib's `printf` with those arguments laid out by the lp64 ABI
(`PrintfEntry`, pointers through `sym`) and that call returns `n`, having
appended to `stdout` exactly the bytes Clight's model prints for the event
(`printfOut p`); memory is unchanged at the Clight level (newlib's state is not
Clight-visible). Every other external and all inline assembly never return.

* `machX_shape`, `machX_extDeterm`: the step shape the Clight runner needs and
  determinism, for every image (no premise);
* `machX_model`: `PrintfModelOn (ImgMakes …) machX`: total exactly on the
  calls the image makes (`ImgMakes`), which is the restriction: an image only
  answers for calls it reaches, and for those it answers with its own return
  value and output;
* `imgMakes_of_spec`: with the open `PrintfSpec_Statement`, every call the
  image reaches in a stdio state (`StdioReach`) with the code present, with
  arguments in the integer subset, is one `ImgMakes` admits (when the strings
  Clight and the machine read agree), so the general spec would make `machX`
  total on all of them.
-/

namespace Tcc.Printf

open Vsa.Machine Vsa.Sim Tcc.Boot CC Tcc.Clight

/-- The image makes the printf call `evargs` (format `fmt`, the global `id`
at offset `ofs`, then `rest`), returning `n` after appending `out`: from a
reset configuration it reaches `printf`'s entry with the arguments laid out,
and that call returns. -/
inductive ImgCall (L : StdioLayout) (Img : Config → Prop) (sym : Ident → Option Nat)
    (fmt : List UInt8) : List EventVal → List UInt8 → Integers.Int → Prop where
  | intro (id : Ident) (ofs : Integers.Ptrofs) (rest : List EventVal) (out : List UInt8)
      (n : Integers.Int) (a : Nat) (margs : List (BitVec 64)) (c₀ c c' : Config)
      (addr : sym id = some a)
      (enc : rest.mapM (encArg sym) = some margs)
      (img : Img c₀) (reach : Steps c₀ c)
      (call : PrintfCall L (a + ofs.toNat) fmt margs out n c c') :
      ImgCall L Img sym fmt (.EVptr_global id ofs :: rest) out n

/-- The derived printf: a step exists exactly for the calls the image makes,
with the image's return value and output. -/
inductive MachPrintf (p : Program) (L : StdioLayout) (Img : Config → Prop)
    (sym : Ident → Option Nat) : ExtcallSem where
  | intro (ge : Senv) (vargs : List Val) (m : CC.Mem) (evargs : List EventVal) (id : Ident)
      (ofs : Integers.Ptrofs) (fmt out : List UInt8) (n : Integers.Int)
      (conv : vargs.mapM (toEventVal ge) = some evargs)
      (head : evargs.head? = some (.EVptr_global id ofs))
      (fmtStr : globalString p id ofs = some fmt)
      (outEq : printfOut p evargs = some out)
      (call : ImgCall L Img sym fmt evargs out n) :
      MachPrintf p L Img sym ge vargs m [printfEvent evargs n] (.Vint n) m

/-- The external-call semantics with the derived printf. -/
def machX (p : Program) (L : StdioLayout) (Img : Config → Prop) (sym : Ident → Option Nat) :
    ExtEnv where
  external name _ := if name = "printf" then MachPrintf p L Img sym else noSem
  inlineAsm _ _ := noSem

theorem machX_shape (p : Program) (L : StdioLayout) (Img : Config → Prop)
    (sym : Ident → Option Nat) : PrintfShape (machX p L Img sym) := by
  intro sg ge vargs m t vres m' h
  simp only [machX, ↓reduceIte] at h
  cases h with
  | intro _ _ _ evargs _ _ _ _ n hc _ _ _ _ => exact .intro evargs n hc

theorem machPrintf_determ (p : Program) (L : StdioLayout) (Img : Config → Prop)
    (sym : Ident → Option Nat) (ge : Senv) : ExtcallDeterm (MachPrintf p L Img sym) ge := by
  refine ⟨?_, ?_⟩
  · intro vargs m t1 vres1 m1 t2 vres2 m2 h1 h2
    cases h1 with
    | intro _ _ _ ev1 _ _ _ _ n1 hc1 _ _ _ _ =>
      cases h2 with
      | intro _ _ _ ev2 _ _ _ _ n2 hc2 _ _ _ _ =>
        rw [hc1] at hc2
        cases hc2
        refine ⟨.syscall _ _ _ _ trivial trivial rfl, fun ht => ?_⟩
        simp only [printfEvent, List.cons.injEq, Event.Event_syscall.injEq,
          EventVal.EVint.injEq, and_true, true_and] at ht
        subst ht
        exact ⟨rfl, rfl⟩
  · intro vargs m t vres m' h
    cases h
    simp

/-- **Determinism**, for every image. -/
theorem machX_extDeterm (p : Program) (L : StdioLayout) (Img : Config → Prop)
    (sym : Ident → Option Nat) : ExtDeterm (machX p L Img sym) where
  external name sg ge := by
    by_cases h : name = "printf"
    · subst h; simpa [machX] using machPrintf_determ p L Img sym ge
    · simpa [machX, h] using noSem_determ ge
  inlineAsm _ _ ge := noSem_determ ge

/-- The calls the image makes, as Clight event arguments. -/
def ImgMakes (p : Program) (L : StdioLayout) (Img : Config → Prop) (sym : Ident → Option Nat)
    (_ : Senv) (evargs : List EventVal) : Prop :=
  ∃ id ofs fmt out n, evargs.head? = some (.EVptr_global id ofs) ∧
    globalString p id ofs = some fmt ∧ printfOut p evargs = some out ∧
    ImgCall L Img sym fmt evargs out n

/-- **The derived printf is total exactly on the calls the image makes.** -/
theorem machX_model (p : Program) (L : StdioLayout) (Img : Config → Prop)
    (sym : Ident → Option Nat) : PrintfModelOn (ImgMakes p L Img sym) (machX p L Img sym) where
  shape := machX_shape p L Img sym
  total := by
    intro sg ge vargs m evargs hc hQ
    obtain ⟨id, ofs, fmt, out, n, hh, hf, ho, hcall⟩ := hQ
    exact ⟨_, _, _, by
      simp only [machX, ↓reduceIte]
      exact .intro ge vargs m evargs id ofs fmt out n hc hh hf ho hcall⟩

/-- When `machX` steps, the machine printed exactly what Clight's model
prints for the event: the bytes `traceOut` gives the event are the bytes the
image's printf appended to `stdout`. -/
theorem machX_out {p : Program} {L : StdioLayout} {Img : Config → Prop}
    {sym : Ident → Option Nat} {sg ge vargs m t vres m'}
    (h : (machX p L Img sym).external "printf" sg ge vargs m t vres m') :
    ∃ evargs n out, t = [printfEvent evargs n] ∧ traceOut p t = some out ∧
      ∃ fmt, ImgCall L Img sym fmt evargs out n := by
  simp only [machX, ↓reduceIte] at h
  cases h with
  | intro _ _ _ evargs id ofs fmt out n _ _ _ ho hcall =>
    refine ⟨evargs, n, out, rfl, ?_, fmt, hcall⟩
    rw [traceOut_printfEvent, ho]
    simp [traceOut]

/-- **The general spec makes `machX` answer every call the image reaches.**
If `PrintfSpec_Statement` holds, the image reaches `printf`'s entry `c` with
the code present, stdio in a reachable state and stack room, the arguments
are in the integer subset, and the `%s` strings the machine reads are the
ones Clight's model reads, then the image makes that call. -/
theorem imgMakes_of_spec {p : Program} {L : StdioLayout} {init : List Tcc.Boot.Chunk}
    {room : MState → Prop} {Img : Config → Prop} {sym : Ident → Option Nat}
    (hspec : PrintfSpec_Statement L init room)
    {c₀ c : Config} (img : Img c₀) (reach : Steps c₀ c)
    {id : Ident} {ofs : Integers.Ptrofs} {rest : List EventVal} {fmt : List UInt8}
    {a : Nat} {margs : List (BitVec 64)} {out : List UInt8} (ge : Senv)
    (hcode : CodeAt L c.σ) (hst : StdioReach L init room c.σ)
    (hentry : PrintfEntry L (a + ofs.toNat) fmt margs c)
    (hroom : ∃ sp, gprGet c.σ 2 = some sp ∧ L.stack ≤ sp.toNat ∧ L.heap.2 ≤ sp.toNat - L.stack)
    (addr : sym id = some a) (enc : rest.mapM (encArg sym) = some margs)
    (hint : IntArgs rest) (fmtStr : globalString p id ofs = some fmt)
    (hout : printfOut p (.EVptr_global id ofs :: rest) = some out)
    (hstr : format (machReader c.σ sym) fmt rest = some out) :
    ImgMakes p L Img sym ge (.EVptr_global id ofs :: rest) := by
  obtain ⟨c', hcall, _⟩ := hspec c sym (a + ofs.toNat) fmt rest margs out hcode hst hentry hroom
    enc hint hstr
  exact ⟨id, ofs, fmt, out, _, rfl, fmtStr, hout,
    .intro id ofs rest out _ a margs c₀ c c' addr enc img reach hcall⟩

end Tcc.Printf
