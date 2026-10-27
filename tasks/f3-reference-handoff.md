Binding rules: read AGENTS.md, CLAUDE.md, DESIGN-COMPILER.md, PHASES.md and MANAGER.md.
No proof escapes/budget raises. Generated outputs via generators; no sum compile-cut regeneration.
C11, whole compiler, Supported/Fits, no refusal escape. No built-in agents or push/merge main.
One shared-lock Lean build under systemd MemoryMax=12G; free -g >=25 GB before heavy jobs.
At most 4 emulator jobs in this slot; lead/other slot share a project-wide limit of about 8 Sail jobs.
Target: make the fast F3 arena reference stop at compiler handoff, independently of program termination.
Files: scripts/rv64emu.c, scripts/ref_arena.sh, focused harness tests, results/model/ref-handoff.md.
Another worker owns model_diff.py and TccModel; do not edit those files.
Currently rv64emu dumps the arena at tcc_baremetal_enter but keeps executing the program.
Nonterminating/trapping generated programs then produce harness errors even though compilation finished.
Add an explicit stop-at-handoff option; ref_arena uses it, ordinary emulator runs retain their behavior.
Report compiler handoff distinctly; never present it as a successful generated-program exit.
Keep compile limits/traps/missing completion records as errors, and validate arena writes.
Existing model_diff parses 'rv64emu: steps=... compile_steps=... exit=...'; preserve compatibility
or document a required lead/other-worker integration change without editing that worker's file.
Do not silently accept a post-timeout arena file as a completed run; stop exactly at the traced handoff.
Done: compare full-run and handoff arenas for a terminating program; compile an infinite-loop program
successfully at handoff while legacy execution times out; pre-handoff limits/traps still fail.
Also test a program that traps after handoff: compiler collection succeeds, legacy execution fails.
Run appropriate C build and repository static gate; no Lean build is needed for pure C/shell edits.
Record exact commands/pins/limitations; this diagnostic runner is not the Sail exit criterion.
No STATUS/PHASES edits; record dead ends. RESULT: landed <commit> or stuck <where>.
