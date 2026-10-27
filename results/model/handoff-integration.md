# Handoff collector integration — 2026-10-27

Integrated the reviewed `ref_arena.sh` / `rv64emu --stop-at-handoff` collector
with the patched C11 model comparator. Success requires exactly one explicit
handoff record and a valid, complete arena. Status 2 and all other harness
errors cannot become cached compiler refusals. Compiler refusal requires the
separate completed-exit record before handoff. No generated-program exit is
invented. The new cache schema and runner-source hashes invalidate old records;
cached arena payloads are validated on read too.

Six protocol tests pass, including injected missing/duplicate/legacy records,
status mismatches and truncated arenas. The real-image suite also passes under
a 12 GB scope with the macro-operand compiler patch: full-run versus handoff
arenas match for a program exiting 7, a nonterminating program, and a deliberate
post-handoff null-access trap. This last input tests the harness, not NoUB.
Pre-handoff limits/traps, write failures, compiler refusal and libm retry checks
remain failures or separate classifications as specified. `check.sh
--static-only` passes; this change does not alter Lean semantic source.

A fresh independent comparison under a 12 GB scope and four jobs produced:

| group | inputs | identical arenas | compiler refusals | harness/model errors |
|---|---:|---:|---:|---:|
| C11 focused | 6 | 6 | 0 | 0 |
| model fixtures | 6 | 3 | 3 | 0 |
| generated cgen | 1,000 | 899 | 101 | 0 |
| generated Csmith | 400 | 400 | 0 | 0 |
| total | 1,412 | 1,308 | 104 | 0 |

All 1,299 generated successes are independent arena comparisons; no reference
bytes are fed to the model. There are zero mismatches. The 198 cgen and 50
Csmith post-handoff limits in the prior report now collect successful arenas.
The refusals remain explicit work items, not coverage or a correctness escape.
Raw case classifications and exact runtime, executable and source pins are in
`handoff-integration.json`. This fast emulator remains diagnostic: the F3
all-corpus Sail arena equality, ghost Cabs and fuel obligations are still open.
No sum compile-cut artifacts changed.

Commands: `python3 scripts/tests/test_model_ref_protocol.py`,
`python3 scripts/tests/test_ref_handoff.py --image`,
`python3 scripts/model_diff.py c11 model gen-cgen gen-csmith -j 4 --require-success 1000 --no-report --json-out results/model/handoff-integration.json`.
The image suite and comparison used `systemd-run --user --scope -p MemoryMax=12G`
after checking available memory. Build/validation logs are local under build/.
