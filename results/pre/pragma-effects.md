# F2 `_Pragma` expansion and effects — 2026-10-27

The operand of `_Pragma` is macro-expanded before requiring one string
literal. Expansion now returns its macro table, push/pop stack and counter
alongside tokens. Macro-stack pragmas execute during rescanning and argument
prescanning, so restoring a macro affects the following token immediately.
Direct `#pragma` and `_Pragma` share the stack operation. Other pragmas remain
explicit output markers; once markers are consumed by the file driver.
Include and line operands also propagate expansion effects instead of silently
dropping counter changes.

Validation: capped `lake build CSemPre pre_spec` and `scripts/check.sh` pass.
Six additional guards cover macro operands, immediate restoration, restoration
inside an argument, counter changes in #line and malformed operands. The
existing C11 pragma reproducer includes pack push/pop, a macro string operand,
wide strings, argument use and a restoration followed by the restored macro.
It is now token-identical to patched tcc.

`python3 scripts/pre_diff.py --jobs 4 --out results/pre/pragma-recheck.tsv`:
593 inputs, 585 exact tcc token agreements, one known cross-host character
signedness difference, two spec errors on UB examples (pp/21.c signed overflow;
pp/22.c directive in a macro argument), and five joint missing-fixture errors.
GCC has 580 exact agreements, six token differences, the same two spec errors
and five joint errors. These are inspected classifications; the comparison
script still needs a fail-closed acceptance contract. No theorem or F2
completion is claimed. Image filesystem path resolution and remaining C11
coverage still require review.
