# `_Pragma` operand macros in replacement streams — 2026-10-27

Sol minimized a valid-C11 refusal in the patched compiler:
`#define P "pack(push, 1)"`, `#define ENTER _Pragma(P)`, then `ENTER`.
The `_Pragma` hook executes while macro_subst is still rescanning, but
next() treats tokens read from a replacement stream as already expanded.
The operand therefore arrived as an identifier instead of a string literal.

Patch 0003 now expands that operand through macro_subst_tok using the
caller's active-macro list. This preserves recursion suppression and the
argument stream for function-like operands. Direct-source `_Pragma` supplies
an empty active list; the isolated pragma parser and restoration are unchanged.

`results/repro/tcc_pragma_macro_operand.c` covers object-like, function-like
and nested operands, and verifies pack restoration. It is included in both
the host/cross lane and the optional full compiler Sail lane.

Validation:

- `python3 scripts/test_tcc_c11.py`: 23 positive / 6 negative cases pass,
  including host tcc vs GCC expected output, -E round trips, cross compilation
  and RV64I object checks.
- `MAX_STEPS=200000000 ORACLE_META=build/pragma-full.meta
  scripts/run_tcc_sail.sh results/repro/tcc_pragma_macro_operand.c`, under
  a 12 GB systemd scope: stdout `pragma operands 0`, exit 0;
  12,007,709 compiler steps, 2,842 program steps, 12,010,576 total steps.
- `scripts/check.sh --static-only`: passes. No Lean source or sum compile-cut changed.

Input/image hashes and raw Sail stdout/metadata accompany this report.
The executable TccModel must be updated and freshly compared against this
patch; prior model comparisons describe the earlier patch revision.
