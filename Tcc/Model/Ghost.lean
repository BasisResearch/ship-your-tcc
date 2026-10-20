/-!
# TccModel: the provisional ghost AST

DESIGN-COMPILER.md §4.1/§6.2: tcc parses in one pass, so the model's parse
functions also produce the abstract syntax they parsed, for Layer B's
parser agreement with the fork's Cabs. Until F1 has the fork's Lean Cabs
in this repository, the ghost output is this small, generic tree; F1
replaces this module (and the model's few `ghost*` calls) with Cabs.

A `GNode` is a constructor name (`"binop"`, `"call"`, `"if"`, `"decl"`,
...), the source tokens it carries (operator, identifier, literal text) and
its children, in source order.
-/

namespace Tcc.Model

inductive GNode where
  | node (kind : String) (toks : Array Int32) (kids : Array GNode)
  deriving Inhabited

namespace GNode
def kind : GNode → String
  | .node k _ _ => k
def kids : GNode → Array GNode
  | .node _ _ ks => ks
def leaf (k : String) (toks : Array Int32 := #[]) : GNode := .node k toks #[]
end GNode

end Tcc.Model
