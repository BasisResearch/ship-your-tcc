Binding rules: read AGENTS.md, CLAUDE.md, BRIEF.md, DESIGN-COMPILER.md, PHASES.md.
Follow MANAGER.md; run Lua abstraction inventory, read mandatory tool table.
No sorry/new axioms/native_decide/budget raises; C11, whole compiler, no refusal exit.
Shared-lock Lean builds under systemd MemoryMax=12G; free -g >=25GB first.
Keep frozen sum cuts; generated files via generators. No extra workers/subagents.
Target: correct positional continuation after a nested C11 initializer designator.
Read results/repro/tcc_designator_continuation.c and the union-repair report.
Valid { .a[0]=1, 2, 3 } for struct { int a[2]; int z; } must initialize all three.
Current tcc discards the nested cursor, advances to z too early, then refuses.
Files: patches/0003-tcc-c11.patch, Tcc/Model/Gen7.lean and necessary named state,
focused fixture generator, tests and result report; vendor/tinycc stays pristine.
Patch compiler and exact executable model together, keeping union override repair.
Use a named cursor/path structure if continuation needs nested state; factor it.
Cover arrays/structs/unions, nested designators, brace elision, explicit braces,
member switches, incomplete arrays, positional continuation, subsequent designators,
and invalid/excess controls. Do not suppress diagnostics to force acceptance.
GCC plus patched CSem provide independent semantic expectations; preserve strings.
Done: host/GCC/cross+ISA tests, full compiler Sail valid minimal and nested cases,
independent model arenas on focused/broad corpus, capped build and full gate.
Count refusals/unsupported assembly/harness errors separately; no reference bytes
may enter the model. Keep any missing proof as a typed Statement, never a new axiom.
Use frozen build handoff if sandbox denies systemd; wait for matching receipt.
Commit for lead review; record dead ends. RESULT: landed <commit> | stuck <where>.
