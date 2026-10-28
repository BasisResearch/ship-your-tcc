import Tcc.Model.Types

/-! The RV64 LP64 memory face of the added alignment carriers. These are
representation obligations, not a proof that the C binary computes the model.
The offsets are checked by a C layout probe built with the image toolchain. -/
namespace Tcc.Model

/-- A synchronized compiler boundary; F6 must supply this from the actual
pinned image and the corresponding modeled parser/backend action. -/
structure AlignmentBoundary where
  memory : ByteArray
  typeOffset : Nat
  valueOffset : Nat
  type : CType
  value : SValue
  symbolAddress : SymId → UInt64

/-- `CType` occupies 24 bytes on RV64 LP64. -/
structure CTypeAt (memory : ByteArray) (offset : Nat)
    (symbolAddress : SymId → UInt64) (type : CType) : Prop where
  present : offset + 24 ≤ memory.size
  tag : Bytes.r32 memory offset = type.t.toUInt32
  reference : Bytes.r64 memory (offset + 8) = symbolAddress type.ref
  alignment : Bytes.r32 memory (offset + 16) = type.aligned

/-- The live value's expression alignment is distinct from its CType and
from the relocation symbol kept in SValue's union. -/
structure AlignmentBoundaryRep (b : AlignmentBoundary) : Prop where
  nullAddress : b.symbolAddress 0 = 0
  type : CTypeAt b.memory b.typeOffset b.symbolAddress b.type
  valueType : CTypeAt b.memory b.valueOffset b.symbolAddress b.value.type
  valuePresent : b.valueOffset + 64 ≤ b.memory.size
  expressionAlignment : Bytes.r32 b.memory (b.valueOffset + 28) = b.value.query_aligned

/-- OPEN F6 obligation. `imageBoundary` must be the reachability/synchronization
predicate for the actual source- and image-pinned compiler, including every
constructor, copy and reset in its executed closure. Neither host tests nor
arena comparisons instantiate that predicate or prove this proposition. -/
def TypeAlignmentRepr_Statement (imageBoundary : AlignmentBoundary → Prop) : Prop :=
  ∀ b, imageBoundary b → AlignmentBoundaryRep b

end Tcc.Model
