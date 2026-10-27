Binding rules: read AGENTS.md, CLAUDE.md, BRIEF.md, DESIGN-COMPILER.md, PHASES.md.
Follow MANAGER.md; inventory before proving; generated outputs via generators.
No sorry/axioms/native_decide/budget raises. Keep C11, whole compiler and frozen sum.
Shared-lock Lean builds, systemd MemoryMax=12G, free -g >=25GB; two external Sols max.
No external messages/push/main merge/built-in agents; work only here.
Target: expose the model's exact phase-4 token stream and compare without -E re-encoding.
Read DESIGN §1.1 and Spec/Pre/Compare.lean: spellings/prefixes/pragma positions matter.
-E escapes question marks in literals created by splicing/stringification; both GCC
and patched tcc do this. Decoding all literal escapes would weaken exact equality.
Implement the existing named preprocess interface using modeled lexer/macro functions,
with TOK_NUM/TOK_STR conversion disabled, preserving phase-4 effects and pragma events.
Use an independent internal-token reference observation harness, not model-derived data;
keep ordinary tcc/GCC -E as auxiliary evidence with explicit encoding classifications.
Files: Tcc/Model*, Spec/Pre/Compare.lean, dedicated driver/harness/tests and report.
Do not invent new axioms, weaken Supported, drop pragmas or canonicalize away spellings.
Cover C11 prefixes/digraphs/UCNs, stringification/concat, counters, line/include state,
pack and push/pop effects, trigraph-spelling literals, macro recursion, invalid inputs.
Done: capped executable build, strict raw-token protocol/failure tests, focused and
corpus comparison with exact success counts and separately retained errors/residuals.
State any still-open connection as a typed Statement; do not claim the theorem proved.
Use frozen build request/result handoff when sandbox systemd is denied; await results.
Record dead ends. Finish RESULT: landed <commit> or RESULT: stuck <where>.
