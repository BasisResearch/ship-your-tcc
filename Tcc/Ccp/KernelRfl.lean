import Lean

/-!
# `kernel_rfl`: close `a = b` by `Eq.refl a`, checked by the kernel only

`decide +kernel` reduces a `Bool` to `true`; for a chunk of the evaluator's
run the useful statement is an equation `run code k s = some t` between two
big states that share almost all of their memory tree. The kernel's
definitional-equality check compares shared subtrees by pointer, so an
equation costs the run plus the changed paths, where a `Bool` comparison
would walk the whole memory.

`kernel_rfl` assigns `Eq.refl a` to the goal without the elaborator's own
(slow, `Meta`-level) defeq check; the kernel checks the proof when the
declaration is added, exactly as for `decide +kernel`. Nothing is trusted that
the kernel does not check.
-/

namespace Tcc.Ccp

open Lean Elab Tactic Meta in
elab "kernel_rfl" : tactic => do
  let g ← getMainGoal
  let t ← instantiateMVars (← g.getType)
  let some (α, a, _) := t.eq? | throwError "kernel_rfl: the goal is not an equation"
  let u ← getLevel α
  g.assign (mkApp2 (mkConst ``Eq.refl [u]) α a)
  replaceMainGoal []

end Tcc.Ccp
