# C11 preprocessing recheck, 2026-10-21

Main has patches 0001–0004. `scripts/test_tcc_c11.py` passes all 22 positive
and six negative cases on host/cross, with GCC and -E round trips. The main
gate and capped `pre_spec`/`csem_cparse` builds pass after updating the old
C99-prefix expectation to the selected C11 mode.

`python3 scripts/pre_diff.py` checks 593 inputs: tcc 583 equal, two different,
three spec errors and five joint errors. The raw classifications are in
c11-recheck.log. These counts do not claim completed F2:

- `c/tests/c11/ucn.c`: the spec does not identify a UCN-spelled macro name
  with the corresponding UTF-8 identifier. Patched tcc and GCC expand it.
- `c/tests/c11/pragma.c`: the spec rejects a macro operand of `_Pragma`.
  It must implement operand expansion and phase-4 effects during rescanning.
- `c/tests/pre/ifexpr.c`: the previously recorded cross-host signed-char
  difference remains; the actual RV64 image agrees with the unsigned spec
  (f2-diff.md). Do not change CSem to match the cross-host artifact.
- The other two spec errors are the known undefined preprocessor examples
  pp/21.c and pp/22.c. Five joint errors lack files in the image fixture.

The harness currently reports differences without a failing exit status;
its status contract needs hardening before it is an acceptance gate.
CSem stays normative C11; no refusal disjunct or Supported restriction was added.
