# GNU conditional bitfield promotion counterexample

Host minimization of the separate full-Sail GNU conditional batch failure.
For `unsigned int x:3` with value zero, GCC returns zero on all six sources.
Patched host tcc returns one for `gnu-type` and `gnu-negative`; ordinary
conditional, unary-plus and arithmetic controls return zero. In particular,
`(s.x ?: -1) < 0` incorrectly evaluates false. This is a compiler defect to
repair alongside the executable model, not a new cfgTcc policy or Supported
restriction. These six host checks are not six new Sail observations.

The corrected report pins the compiler binary, patched tccgen and driver.
The initial attempt omitted tcc's `-B` runtime path, so linking failed before
execution; its diagnostics and original driver are retained as harness failures.
The corrected driver supplies the existing host runtime directory.

Next task: `tasks/f3-gnu-conditional-bitfield-promotion.md`, after enum-colon
compiler repair. The GNU shared-semantics worker retains its independent raw
and native observations; the native batch's remaining failure is not agreement.
