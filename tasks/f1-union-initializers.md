Binding rules: read AGENTS.md, CLAUDE.md, BRIEF.md, DESIGN-COMPILER.md, PHASES.md.
Follow MANAGER.md; inventory before proofs; generated files via matching pinned lem.
No sorry/new axioms/native_decide/bv_decide/budget raises. Preserve frozen sum cuts.
Keep C11, whole compiler and Supported/Fits; no new refusal or subset end state.
Shared-lock builds, systemd MemoryMax=12G, free -g >=25GB; two Sol slots max.
No external messages/push/main merge/built-in agents; own worktree only.
Continue the reviewed sol/csem-repin candidate (704c4e69/e6d8db56) and its fixes.
Main keeps the old pin until this regression is repaired and gates pass.
Target: implement C11 union designated-initializer semantics in CSem.
Read CSEM-UPSTREAM.md CSEM-05 and results/csem/repin-2026-10-27/probes/union-second.c.
Current upstream refuses every non-first designator because its initializer map
loses member identity and constructValue_aux always builds the first member.
Carry selected member identity through initializer traversal, override and value
construction, including nested unions/structs/arrays and implicit initialization.
Do not merely remove the refusal: implement the lost semantics in shared sources.
Files: csem/frontend/model/desugaring_init.lem and necessary shared definitions,
generated Lean via scripts/csem_regen.sh, focused tests/report and bug disposition.
Validate both OCaml/Lean engines generated from the patched sources against GCC
and patched tcc/Sail; keep pristine-upstream observations separately classified.
Cover repeated same-member designators, switching members, nested descendants,
partial arrays, differing member types, pointers/relocations and local/static data.
Reuse c/tests/c11/union.c and results/repro/tcc_union_designator_override.c.
Done: focused semantic results agree; baseline, parser, generator and axiom gates
pass without weakening them. Replace upstream refusal tests with positive coverage
only for genuinely implemented cases; document exact remaining gaps honestly.
Use unique frozen build/toolchain requests for denied sandbox systemd/DNS/Git.
Record dead ends. Finish RESULT: landed <commit> or RESULT: stuck <where>.
