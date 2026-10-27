Binding rules: read AGENTS.md, CLAUDE.md, BRIEF.md, DESIGN-COMPILER.md, PHASES.md.
Follow MANAGER.md and tasks/DEAD-ENDS.md; reuse inventory before proof work.
No sorry, axioms, native_decide, raised budgets or generated-output edits.
Keep whole-compiler Supported/Fits target, CSem C11 and frozen sum compile-cut.
One shared-lock Lean build, systemd MemoryMax=12G, free -g >=25GB; max two Sols.
No external messages, push, main merge or built-in agents; work only here.
Target: repair C11 trigraph translation consistently in patched tcc and its model.
Confirmed: tcc -std=c11 -E leaves ??=define and ??< / ??> unchanged; GCC translates.
Spec/Pre/Types.lean correctly says C11 requires this; cfgTcc currently disables it.
Patch source only through patches/0003 and reproducer(s); vendor/tinycc pristine.
Files: patches/0003*, Tcc/Model/, Spec/Pre/Phase12.lean and tests,
scripts/gen_pre_cfg.py (regenerate Cfg.lean), C11 test scripts, results/repro/.
Preserve all existing C11 fixes, especially active-macro-aware _Pragma operands.
Cover all nine sequences, phase-1-before-splicing/comments, strings, includes,
buffer boundaries, overlap and macro-produced question marks (no second phase 1).
Do not weaken CSem to match the defect or restrict Supported.
Done: focused host/cross tests, independent model arena comparisons, capped build,
static gate and a report with exact pins; request a lead Sail probe if needed.
If systemd is denied, freeze SHA256 inputs/targets in build/worker-build.request,
wait for matching build/worker-build.result; never substitute an uncapped build.
Keep task under review until validation returns; Git read-only means lead commit.
Record failed approaches. Finish RESULT: landed <commit> or RESULT: stuck <where>.
