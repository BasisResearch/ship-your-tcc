Binding rules: read BRIEF.md, DESIGN-COMPILER.md, PHASES.md, CLAUDE.md, MANAGER.md, tasks/DEAD-ENDS.md.
No sorry/axiom/native_decide/bv_decide, raised budgets or hand-edited generated outputs.
Run Lua inventory/mandatory table before proofs; reuse with attribution and byte-identity checks.
CSem stays C11; whole compiler Supported/Fits, no refusal escape. Never regenerate sum cuts.
One Lean build: root build/lake.lock, systemd MemoryMax=12G, free -g >=25. No built-in agents.
This worktree only; no push/main merge. Freeze sources until the matching completed build receipt.
Target: preserve GNU typedef alignment in tcc's type-alignment queries and the executable model.
Start from results/repro/tcc_gnu_typedef_alignment.c and its pinned review evidence.
RV64 Sail: tcc lays out Up/Down members at 16/1 but _Alignof(Up/Down) reports 4/4.
GCC reports 16/1; chained typedef Again also loses its query alignment in tcc.
Inspect parse_btype's sym_to_attr, expression/type parsing and unary alignment handling.
Files: patch 0003 (pristine vendor only), modeled Gen3/Gen4 and relevant CType/state interfaces.
Preserve sizeof, member offsets, nested aliases, qualifiers, pointer/array/function declarators.
Do not infer alignment from stale vstack entries or modify unrelated type attributes.
Compare GCC, patched tcc, CSem and model where each interface supports the same input.
Keep GNU expression-alignof on explicitly aligned members as a separate documented question.
Do not narrow Supported, strip attributes or count matching refusals as successes.
Add focused positives and negative constraints with aliases, scopes, and stronger/weaker alignments.
Run compiler C11/backend checks, strict fast arenas, full tcc-run Sail and the repo gate.
Include actual compiler/model source hashes, binary/image pins and failed baseline observations.
Done: valid type queries preserve declared typedef alignment without layout/sizeof regressions.
Write results/model/typedef-alignment.md and ledger suggestions; lead updates STATUS/PHASES.
Final RESULT: landed <commit> | stuck <where>.
