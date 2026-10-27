Binding rules: read AGENTS.md, CLAUDE.md, BRIEF.md, DESIGN-COMPILER.md, PHASES.md.
Follow MANAGER.md; inventory before proof work; generated files via generators.
No sorry/new axioms/native_decide/budget raises; keep C11 and frozen sum cuts.
Shared-lock Lean builds, systemd MemoryMax=12G, free -g >=25GB; two Sol slots max.
No external messages/push/main merge/built-in agents; work only in this worktree.
Target: repair tcc __LINE__ expansion across a spliced physical line and reflect it.
Read results/repro/tcc_line_splice.c and results/pre/trigraphs.md.
Without trigraphs, tcc expands the line-300 token to 301; GCC/CSem give 300.
Preserve physical token provenance across lexer lookahead and macro substitution.
Do not change the C11 spec, weaken Supported, or special-case this source shape.
Files: patches/0003*, Tcc/Model lexer/macro state, C11 test generator/reproducers,
Spec/Pre tests only if useful; vendor pristine, generated tokens via generator.
Cover repeated splices, trigraph backslashes, comments, macro wrappers/arguments,
#line remapping, include boundaries and __COUNTER__ interaction where relevant.
Done: reproducer agrees with GCC/spec, meaningful focused host/cross/model cases,
existing C11 suite passes, capped build, static gate, lead Sail test with exact pins.
Read build/lead-note.txt; freeze unique request/targets/sha256 in worker-build.request.
Lead service returns worker-build.result; do not end at denied systemd/DNS/Git.
Record dead ends and remaining issues. Finish RESULT: landed <commit> or stuck <where>.
