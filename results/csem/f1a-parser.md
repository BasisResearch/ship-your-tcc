# F1a recovered parser validation

2026-10-21. Recovery base: `7dabfe1`. The handwritten Lean grammar targets the
whole C11 compiler and the GNU forms in DESIGN-COMPILER.md §5. F1 remains open:
Menhir itself rejects some approved syntax; agreement on its accepted inputs
does not establish the whole grammar or change Supported.

The recovered token comparator omitted ELSE, DEFAULT, STATIC, STATIC_ASSERT
and ASM, despite their use in lookahead/shift helpers. `Tok` now derives exact
BEq, and every accept/expectT/at?/isTok call shares that comparison. This fixes
the dispatch causes of all 239 original ORACLE_ONLY rows (237 else, one generic
default, one other punctuator diagnostic). `expect`'s unused token argument and
redundant predicate wrapper are removed.

The comparator now fails on LEAN_ONLY and every unexpected failure. Oracle
rejection requires exit 1, no JSON, and a known CPARSER diagnostic from the
pinned oracle's pp_errors.ml. Signals, OCaml exceptions, unknown exits,
execution errors and timeouts are separate failing outcomes. Oracle success
requires a TUnit JSON document with declarations and a source digest. Lean's
single status line, exit code and oracle acceptance must agree. The CLI returns
0 for AGREE/BOTH_REJECT and 1 for mismatches/ROUNDTRIP. Empty/missing corpora
fail before processes start; ad-hoc reports do not overwrite corpus reports.
Each run gets a fresh directory and each absolute input path a SHA-256 cache
directory, retaining all three processes' stdout, stderr and status.

Cabs comparison is unchanged: every semantic field and every location is
compared. Only the source digest, which is oracle metadata absent from the
Cabs datatype, is stripped. The oracle import/export round trip is checked
before comparing the Lean parse. No source rewriting or semantic
normalization was introduced.

Commands executed in this worktree:

```sh
free -g  # 88 GB available before the initial build attempt
flock "$SYT_LAKE_LOCK" systemd-run --user --scope -p MemoryMax=12G lake build csem_cparse
python3 -m unittest discover -s scripts/tests -p test_csem_parse_cmp.py -v
python3 scripts/csem_parse_cmp.py --oracle-only --jobs 6 --timeout 120 --report results/csem/f1a-oracle.tsv
python3 scripts/csem_parse_cmp.py --jobs 6 --timeout 120
python3 scripts/csem_parse_cmp.py --jobs 2 --timeout 30 --report results/csem/f1a-regressions.tsv csem/cparser/tests/token_dispatch.c csem/cparser/tests/reject.c
python3 scripts/check_discipline.py
python3 abstractions/gate.py
```

The worker's systemd invocation failed with `Failed to connect to user scope
bus via local transport: Operation not permitted`. The lead consumed
`build/worker-build.request` and performed the capped build: 110 jobs passed at
default budgets (`build/worker-build.log`, `build/worker-build.previous.result`).
No uncapped build was attempted. No compile-cut artifacts were regenerated.
The ten comparator regressions pass; discipline (8 rules, 1046 files) and the
abstraction gate pass. The two new C fixtures give AGREE=1, BOTH_REJECT=1.

Independent oracle census (`f1a-oracle.tsv`): 664 files, 614 ORACLE_OK,
41 ORACLE_REJECT, 9 CPP_FAIL, no crashes/timeouts. Full retained oracle outputs
were also rechecked by the final, stricter classifier with identical counts.
Artifacts: `build/f1a/run-1s4f4wl8` (first census).

First hardened comparison, after token fix: **614 AGREE, 40 BOTH_REJECT,
9 CPP_FAIL, 1 LEAN_TIMEOUT**, exit 1. All 200 Csmith agree. No DIFF,
ORACLE_ONLY, LEAN_ONLY, ROUNDTRIP or oracle failures. Artifacts:
`build/f1a/run-l2lmoun6`; intermediate report retained as
`build/f1a-parser-token-only.tsv`. The timeout is `tests2/101_cleanup.c`, whose
15 MB preprocessing expansion is rejected by Menhir at the first GNU cleanup
attribute. It is not counted as BOTH_REJECT.

The large-expansion performance fix is validated: lexer step closures
retained the accumulated token array while pushing the next token. The source
now runs each step with an empty emission array, then appends that step's
emissions to the accumulator outside the closures. It preserves all tokens,
positions and fuel bounds. The lead's second capped build passed (110 jobs,
`build/worker-build-2.log`, `build/worker-build.result`). The final full rerun
with the same command exits **0: 614 AGREE, 41 BOTH_REJECT, 9 CPP_FAIL**,
with zero unexplained mismatch, crash, timeout, protocol error or round-trip
failure. All 200 Csmith agree. Final artifacts: `build/f1a/run-dj9852gn`;
report path: `results/csem/f1a-parser.tsv`. Both new C regressions
were rerun against this binary with the same successful counts.

A process-wall-time check of 1k/2k/4k/8k repeated declarations measured
0.037/0.057/0.138/0.484 seconds before and 0.046/0.049/0.049/0.071 after
(the 1k/2k differences are startup noise). The large cleanup case now completes
and is explicitly BOTH_REJECT, rather than interrupted or timed out.

Oracle refusals, by first cause (41 total; duplicates from copied corpora
remain separate inputs):

| count | first cause | ruling / next work |
|---:|---|---|
| 6 | GNU attributes (`regr_struct12`, `101_cleanup`, `108_constructor`, `120_alias`, `128_run_atexit`, `82_attribs_position`) | Grammar gap; preserve attributes and supply their meaning. Alias is separately listed in §5; cleanup must run at scope exit. |
| 4 | GNU typeof spellings (`100_c99array-decls`, `150_return_qualifiers`, `33_ternary_op`, `94_generic`) | Lexer alias / grammar gap; canonical typeof already has Cabs forms. |
| 3 | builtin offsetof (`117_builtins`, `95_bitfields`, `95_bitfields_ms`) | Alias / grammar gap. |
| 1 | GNU thread spelling (`146_tls_extern`) | Lexer alias gap. |
| 6 | empty initializers (`t_35_sizeof`, `t_80_flexarray`, `35_sizeof`, `73_arm64`, `80_flexarray`, `90_struct-init`) | GNU grammar extension remains; not outside Supported. |
| 3 | K&R declaration lists (`t41_oldstyle_ldva`, `t_133_old_func`, `133_old_func`) | Both implementations deliberately reject them today; this must be fixed for approved scope. |
| 3 | dollar identifiers (`regr_dollars`, `03_struct`, `76_dollars_in_identifiers`) | Lexer extension remains. |
| 4 | undefined `__SIZE_TYPE__` (`t_103_implicit_memmove`, `103_implicit_memmove`, `123_vla_bug`, `88_codeopt`) | `-undef` removes compiler predefined macros; environment gap. Do not invent a typedef or reinterpret the token. |
| 2 | omitted type in `_Alignas(16) i3` (two `102_alignas` copies) | Non-C11 source. Do not copy tcc's implicit-int acceptance into C11 CSem. |
| 1 | weak attribute at declaration start (`104+_inline`) | GNU grammar / binding test; no parser equality claim. |
| 8 | computed goto (1), asm labels (3), asm goto (1), top-level asm (3) | Explicit §5 exclusions; classify, do not give them invented C11 meaning. |

The nine preprocessing failures are: missing alloca.h (`t42_builtins`),
missing target endianness defines (`bench_aes`), unsupported pthread.h
(`106_versym`, `114_bound_signal`, `124_atomic_counter`, `144_tls`), unsupported
setjmp.h (`115_bound_setjmp`, `116_bound_setjmp2`), and missing windows.h
(`145_winarm64_interlocked`). They remain explicit preprocessing cases;
they do not count as accepted grammar tests.

Residual work and dead ends: GNU ATTRIBUTE/EXTENSION/LABEL/BUILTIN_VA_LIST
are tokens without productions in the draft. The Menhir source must also be
extended to validate these forms; `--gnu` keyword aliases alone are not a
completed grammar. Cabs has no local-label-declaration node, and the Lem
elaborator has no cleanup lowering: deleting those tokens would lose scope or
behavior. See `csem/GRAMMAR.md`. K&R, F2 token conversion and raw-source CSem
integration remain open. The final theorem remains whole-C11 with exactly the
approved Supported exclusions and Fits assumptions; no refusal escape was
added.

Suggested ledger text (STATUS.md/PHASES.md deliberately untouched):

> F1 parser: token equality and comparison harness hardened; all 614
> Menhir-accepted F1 inputs agree exactly including locations (200 Csmith).
> 41 oracle refusals and 9 preprocessing failures classified explicitly.
> Full grammar remains open for approved GNU forms, K&R, and F2 integration.
> Large cleanup expansion lexer prefix-copying fixed and validated; full
> 664-file comparison exits 0 (614 AGREE, 41 BOTH_REJECT, 9 CPP_FAIL).

Git staging/commit is blocked by the sandbox: `git add` fails with
`Unable to create .../.git/worktrees/agent-a31b97b8f1c090141/index.lock:
Read-only file system`. No Git metadata workaround, push or merge was
attempted. The stable source diff is left for lead review and commit. The
remaining GNU grammar work is explicitly open; this validation does not
claim the whole approved grammar is implemented.
