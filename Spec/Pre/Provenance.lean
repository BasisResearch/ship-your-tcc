import Spec.Pre.Types

/-! Provenance is additional data, never part of phase-four token equality.
Physical identities are resolved image paths; presumed names are labels and
never establish runtime/library membership. Byte offsets are half-open. -/
namespace CSem.Pre

structure SourcePoint where
  offset : Nat
  line : Nat
  column : Nat
  deriving Repr, Inhabited, DecidableEq

structure SourceSpan where
  /-- Path as opened, before any #line, plus resolved image identity. -/
  openedAs : Bytes
  identity : List Bytes
  start : SourcePoint
  stop : SourcePoint
  deriving Repr, Inhabited, DecidableEq

structure SourceSite where
  physical : SourceSpan
  presumed : Bytes
  line : Nat
  endLine : Nat
  /-- Innermost include directive first; repeated inclusions stay distinct. -/
  includes : List SourceSpan := []
  deriving Repr, Inhabited, DecidableEq

inductive Origin where
  /-- Only for internal synthetic lexing buffers, not an invented position. -/
  | unavailable
  | source (site : SourceSite)
  | replacement (invocation definition : Origin)
  | argument (invocation : Origin) (index : Nat) (operand : Origin)
  | stringify (invocation : Origin) (operands : List Origin)
  | paste (invocation left right : Origin)
  | builtin (invocation : Origin)
  | pragma (invocation : Origin)
  deriving Repr, Inhabited

/-- Diagnostic anchor. The full derivation remains available: argument tokens
retain their own anchors, replacement tokens use the macro invocation. -/
def Origin.site : Origin → Option SourceSite
  | .unavailable => none
  | .source s => some s
  | .replacement i _ | .stringify i _ | .builtin i | .pragma i => i.site
  | .argument _ _ a => a.site
  | .paste i _ _ => i.site

structure LocatedPPToken where
  token : PPToken
  origin : Origin
  deriving Repr, Inhabited

/-- An actual file opening, including empty or macro-only includes. Contents
are the physical bytes, not a #line label or a preprocessed reconstruction. -/
structure SourceFile where
  openedAs : Bytes
  identity : List Bytes
  contents : Bytes
  includedAt : Option SourceSpan
  deriving Repr, Inhabited

/-- The named preprocessing result; `erase` is the original phase-4 interface.
A consumer must not reconstruct missing provenance from token spellings. -/
structure PreprocessingResult where
  tokens : List LocatedPPToken
  sources : List SourceFile := []
  finalPack : List Nat := [0]
  deriving Repr, Inhabited

def PreprocessingResult.erase (r : PreprocessingResult) : List PPToken :=
  r.tokens.map (·.token)

end CSem.Pre
