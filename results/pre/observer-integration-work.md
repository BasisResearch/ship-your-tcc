# Observer integration checkpoint (not validated for landing)

Lead review repaired the three observer counterexamples from pragma-spec.md.
At the earlier candidate, build/observer-repair/combined-final.json has three
exact model/reference comparisons and identical observed/plain effects. The
spec had not yet been rebuilt with the cherry-picked pragma repair: two spec
errors in that capture are historical, not the current verdict. Final combined
validation remains required; no old report certifies the current source hashes.

The ignored-operator counter case exposed a real compiler defect: discarded
_Pragma operands expanded __COUNTER__. Compilation now discards without macro
expansion, mirrored by PpMacro. The two new runtime reproducers are registered
in test_tcc_c11.py, with the same round-trip requirements as every other case.

The -E path also expanded unknown pragma operands, so the ignored-counter
round trip failed. The current patch preserves raw operands in emitted pragma
lines and temporarily disables expansion when copying the operator's output.
It calls next() with PREPROCESS cleared, retaining buffered/unget token stacks.
A deferred output sink handles direct nested operators; the model carries the
corresponding optional TokenString and raw concatenation. Cached operand spaces
are skipped by a named helper in C/model. These later changes are NOT yet built
in Lean or accepted by the full gate.

Remaining observed failure: the registered nested-macro runtime reproducer
passes direct host compilation, but `tcc -std=c11 -E` refuses its string operand.
An inner P("unknown inner") executes during the outer macro's argument prescan,
before pragma_operator creates its sink. The current sink therefore cannot
make all pragma output zero width. Preserve the failing test and repair the
representation. Direct `_Pragma(_Pragma("unknown inner") "unknown outer")`
now emits both directives and compiles. Both source forms are valid GCC C11.

Dead ends: replacing next() by next_nomacro() while draining -E output drops
buffered #pragma tokens (it reads files, not macro stacks). Skipping cached
operand spaces alone does not fix prescan output. Per-operator output capture
alone is too late for nested macro argument prescan. None is a complete fix.

Current raw-case generator calls these OBSERVER_REGRESSIONS and includes all
three in strict positives; integration tests now require exact spec/raw equality
and preserved effects, not failure. Historical pragma-*.json reports and their
freeze script retain the old failed-observer lane unchanged. Record current
results under new names after rebuilding. Existing frozen sum chains remain
unchanged. Three correctness/noninterference Statements stay open.

Useful local evidence: build/observer-repair/c11-tests-01 through -06 logs
(the first is c11-tests.log), ref-build logs, combined-final.json, and
integrated-build{,-02}.log. The earlier 101-job capped build passed, but changes
since it require a new capped build. The root build service handles frozen
worker requests; shared lock is the root repository's build/lake.lock.
