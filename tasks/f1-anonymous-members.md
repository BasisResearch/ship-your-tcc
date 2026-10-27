Binding rules: read BRIEF.md, DESIGN-COMPILER.md, PHASES.md, CLAUDE.md, MANAGER.md and tasks/DEAD-ENDS.md.
No sorry/axiom/native_decide/bv_decide, raised budgets or hand-edited generated files.
Before proofs run Lua abs_inventory.sh and mandatory tool table; reuse with attribution/byte identity.
CSem stays C11; whole compiler Supported/Fits, no refusal escape. Never regenerate sum compile-cut.
One Lean build project-wide: root build/lake.lock, systemd MemoryMax=12G, free -g >=25 before builds.
No built-in agents; this worktree only; no push/main merge. Freeze capped build/commit requests if denied.
Target: repair CSem's anonymous struct/union representation, layout and initializer continuation.
Read CSEM-UPSTREAM.md CSEM-07, results/model/designator-continuation.md and designator/layout-minimized/.
Pristine 704c4e69 and local repaired OCaml/Lean get anonymous continuation wrong; explicit designators work.
Minimal: struct S{union{struct{int a,b;};int n;};int z;}; static struct S s={.a=1,2,3};
GCC and patched tcc return s.z=3; pristine CSem returns zero. No layout-dependent expected value here.
The anonymous container also has size/offset mismatches (20/16 vs GCC16/12 in the array probe).
Locate where flattening loses container identity; retain nested layout/current-object boundaries.
Review member access, address/offsetof, explicit and positional initialization, union switching and qualifiers.
Files: shared frontend/model anonymous-member desugaring/type/layout sources and hand-written adapters
only where necessary; regenerate Lean and isolated patched OCaml from the same shared source.
Extend isolated fork-oracle source manifest explicitly; never mutate pristine oracle or shared opam.
Do not retire or relax union override, aggregate-expression, bitfield or unknown cases by exclusion.
Keep existing selected-member union repair and independent IEEE changes intact.
Use complete strict-C11 probes, nested anonymous structs/unions, alternate storage, shadow/collision controls,
member pointers/offsets, mixed named/anonymous nesting and same/switch member initializer sequences.
Compare GCC, same-source patched OCaml, generated Lean and patched tcc/Sail; retain pristine failures.
Run the 34 existing union checks and 58 continuation probes, classifying known bitfield gaps separately.
Final scripts/check.sh --csem and parser/regeneration/ordinary-axiom checks required on frozen sources.
Record results/csem/anonymous-members.md with exact source/binary pins, provenance and remaining gaps.
No claim of compiler/model refinement or all C11 coverage from finite probes.
Do not edit STATUS/PHASES; supply ledger text and dead ends. Final RESULT: landed <commit> | stuck <where>.
