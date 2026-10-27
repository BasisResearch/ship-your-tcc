Binding rules: read AGENTS.md, CLAUDE.md, BRIEF.md, DESIGN-COMPILER.md, PHASES.md.
Follow MANAGER.md; inventory before proof work; generated files via generators.
No sorry/new axioms/native_decide/budget raises; keep C11 and frozen sum cuts.
Shared-lock Lean builds under systemd MemoryMax=12G; free -g >=25GB first.
No external messages/push/main merge; work only here, external Sol, max two slots.
Start after the current CSem repin evaluation is reviewed; do not disrupt it.
Target: correct preprocessing include traversal using a shared path-walk abstraction.
Read results/pre/path-resolution.md and results/repro/pre_include_missing_parent.c.
Also read results/model/duplicate-header-review.md: ModelMain wrongly keeps first entries.
FileSys.lookup currently erases a missing intermediate directory with dotdot.
Model.Host.resolve and CerbFS.walk already resolve components in order.
Factor the common traversal before adding a third implementation; preserve errors.
Files: Spec/Pre/Types.lean, tests, Tcc/Model/Host.lean, ModelMain, CerbFS source,
a pure shared helper and required root Lake/generator manifests.
Do not restrict Supported or replace directory existence with lexical normalization.
Validate absent/intermediate file paths, trailing/repeated slash, dot/dotdot/root,
empty paths and conflicting initial file-table entries against htif.c behavior.
Retain ordered initialization: later duplicate file contents win; files block descendants.
Done: failing reproducer corrected, focused path guards and full existing FS/config
checks pass, preprocessing corpus recheck has no unexplained regression, full gate.
Use frozen build request/result handoff when sandbox systemd is denied.
Record dead ends. Finish RESULT: landed <commit> or RESULT: stuck <where>.
