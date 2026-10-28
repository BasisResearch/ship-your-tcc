import CSemCParser.Phase7

/-! Typed syntax retained by the modeled compiler's parsing actions. The input
is a positioned *token* fixture, never a reference syntax tree. Ordinary
compilation does not enable this testing seam. Production fragments are built
in parser order, before CType/value folding; an unimplemented construction is
an explicit diagnostic, never a successful empty translation unit. -/
namespace Tcc.Model.Ghost
open CSemCParser

structure Input where
  language : Phase7.Input
  spelling : Array CSem.Pre.PPToken

/-- Model-owned file identity/content and token origin. Buffer offsets are
kept separately from physical offsets; transformations and macro derivations
that lack a complete mapping cause an explicit gap, never an invented site. -/
structure ModelSource where
  id : Nat
  openedAs : ByteArray
  contents : ByteArray
  /-- Resolved by the modeled host opening; empty for generated buffers. -/
  identity : List CSem.Pre.Bytes := []
  generated : Bool := false
  includedAt : Option CSem.Pre.SourceSpan := none
  includes : List CSem.Pre.SourceSpan := []
  includeComplete : Bool := true

/-- Physical byte points come only from the retained opening's bytes. -/
def ModelSource.span (s : ModelSource) (offset width : Nat) : CSem.Pre.SourceSpan :=
  let point := fun p =>
    let before := (s.contents.extract 0 p).toList
    { offset := p, line := before.count 10+1,
      column := (before.reverse.takeWhile (· != 10)).length+1 : CSem.Pre.SourcePoint }
  { openedAs := s.openedAs.toList, identity := s.identity,
    start := point offset, stop := point (offset+width) }

/-- A successful modeled host opening, retained before descriptors are reused. -/
structure OpenedFile where
  openedAs : ByteArray
  resolved : ByteArray
  contents : ByteArray

/-- One byte emitted by the actual model refiller. Trigraphs own three
physical bytes; deleted splice bytes remain recorded with `removed = true`.
Indices into this opening-local log survive buffer replacement and ungets. -/
structure PhysicalChar where
  byte : UInt8 := 0
  start : Nat := 0
  stop : Nat := 0
  removed : Bool := false
  deriving Inhabited, BEq

/-- A logical view distinguishes actual bytes from a terminal LF supplied
only for the C11 comparison. It never becomes a compiler input or an origin. -/
structure LogicalChar where
  byte : UInt8
  start : Nat
  stop : Nat
  synthesized : Bool := false

/-- CRLF is normalized only when the two retained bytes were physically
adjacent. Removing a splice must not create a new phase-one CRLF pair. -/
def normalizePhysical : List PhysicalChar → List LogicalChar
  | [] => []
  | [c] => [⟨c.byte,c.start,c.stop,false⟩]
  | c :: d :: rest =>
    if c.byte == 13 && d.byte == 10 && c.stop == d.start then
      ⟨10,c.start,d.stop,false⟩ :: normalizePhysical rest
    else ⟨c.byte,c.start,c.stop,false⟩ :: normalizePhysical (d :: rest)
termination_by cs => cs.length

/-- Observation of the real shadow log after actual splice deletions.
A missing terminal newline has no physical byte: its marker is explicit. -/
def logicalView (chars : Array PhysicalChar) : List LogicalChar :=
  let cs := normalizePhysical ((chars.filter (!·.removed)).toList)
  match cs.getLast? with
  | none => []
  | some last => if last.byte == 10 then cs else cs ++ [⟨10,last.stop,last.stop,true⟩]

structure ModelOrigin where
  source : Nat
  bufferOffset : Nat
  physicalOffset : Option Nat
  presumed : ByteArray
  line : Nat
  includeFiles : Array Nat
  macroFrames : Array Nat
  /-- Surviving physical pieces, before the independent bounding-span projection. -/
  segments : Array CSem.Pre.SourceSpan := #[]

/-- Original source identities indexed by word offset in a saved compiler
initializer/body token string. Replaying does not create source occurrences. -/
structure SavedTokens where
  tokens : Array Int32
  positions : Array (Nat × Nat)

/-- Shadow metadata: compiler token words remain untouched. -/
abbrev OriginMap := Array (Nat × Nat)

def OriginMap.at (m : OriginMap) (word : Nat) : Nat :=
  ((m.find? (fun p => p.1 == word)).map (·.2)).getD 0

structure OriginRecord where
  origin : CSem.Pre.Origin
  anchor : Option ModelOrigin
  complete : Bool
  deriving Inhabited

inductive Derivation where
  | replacement | argument (index : Nat) | stringify | paste | builtin

structure PragmaEffect where
  beforeToken : Nat
  keyword : Int32
  packBefore : Array Int32
  packAfter : Array Int32
  packPointerBefore : Nat
  packPointerAfter : Nat
  librariesBefore : Array ByteArray
  librariesAfter : Array ByteArray
  counterBefore : Int32
  counterAfter : Int32

/-- Half-open indices into the shared source token/provenance table. Replays
retain these identities rather than allocating new source occurrences. -/
structure Range where
  first : Nat := 0
  last : Nat := 0
  deriving Inhabited, BEq, Repr

inductive Production where
  | ident | constant | string | paren | deref | addr | lnot | bnot | uplus | neg
  | preinc | predec | postinc | postdec | sizeof | alignof | func | builtin
  | generic | labeladdr | call | arrow | member | index | complit | type | cast | stmtexpr
  | binop | logop | cond | condGNU | assign | comma
  | specifiers | declarator | initializer | initDecl | declaration | fundef | tu
  | ifStmt | whileStmt | block | returnStmt | breakStmt | continueStmt | forStmt | doStmt
  | switchStmt | caseStmt | defaultStmt | gotoStmt | asmStmt | empty | label | exprStmt
  | labelled | staticAssert | deferredFunction | localLabels | gnuAttrs | asmLabel | declName | typedefName
  deriving Inhabited, BEq, Repr

structure Gap where
  production : Production
  range : Range
  reason : String
  deriving Inhabited, Repr

/-- Labels are completed by the next statement in the modeled block, rather
than discarded or represented as a null statement. -/
inductive Label where
  | named (id : identifier)
  | caseValue (lo : cabs_expression) (hi : Option cabs_expression)
  | defaultValue
  deriving Inhabited

inductive ParsedDeclarator where
  | concrete (value : declarator)
  | abstract (value : Option abstract_declarator)
  deriving Inhabited

inductive Syntax where
  | expression (value : cabs_expression)
  | typeName (value : type_name)
  | identifier (value : identifier)
  | attributes (value : attributes)
  | asmBinding (value : cabs_string_literal)
  | specs (value : specifiers)
  | decl (value : ParsedDeclarator)
  | initializer (value : initializer_)
  | initDecl (value : init_declarator)
  | declaration (value : cabs_declaration)
  | functionBody (value : cabs_statement)
  | functionDeclarator (decl : declarator) (body : cabs_statement)
  | external (value : external_declaration)
  | statement (value : cabs_statement)
  | label (value : Label)
  | translationUnit (value : translation_unit)
  deriving Inhabited

/-- Fixed, visible syntax encodings. None supplies semantic agreement. -/
inductive NormalizationKind where
  | gnuAttributeCarrier | asmBindingCarrier | logicalChainRestoration
  deriving Repr

structure NormalizationUse where
  kind : NormalizationKind
  range : Range

structure Fragment where
  production : Production
  range : Range
  value : Except Gap Syntax
  deriving Inhabited

structure Mark where
  height : Nat := 0
  first : Nat := 0
  deriving Inhabited

/-- Dropping a repeated dry run is explicit. The original token/provenance
input and the audit of its parser actions remain available. -/
inductive DropReason where
  | preprocessorExpression | initializerDryRun
  deriving Repr

structure Drop where
  range : Range
  reason : DropReason
  fragments : Array Fragment

end Tcc.Model.Ghost
