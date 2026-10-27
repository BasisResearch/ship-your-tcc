# F2 UCN identifier identity — 2026-10-27

Identifiers use a canonical UTF-8 key for macro definitions, lookup, formal
parameters, `defined`/`ifdef`, `undef` and hide sets. Their token spelling is
unchanged, including when `#` stringifies an argument or `##` pastes tokens.
UTF-8 encoding is shared with the character-constant evaluator.

Validation: capped `lake build CSemPre pre_spec` and `scripts/check.sh` pass.
Seven focused guards cover short/long UCN spelling, literal UTF-8, parameters,
conditional lookup/undefinition, token pasting, recursion suppression and raw
stringification. `python3 scripts/pre_diff.py --jobs 4 --out
results/pre/ucn-recheck.tsv` records 593 inputs: 584 token-equal to patched tcc,
three spec errors, one token difference and five joint errors. The UCN C11
reproducer is now equal. The remaining token difference is the documented
cross-host character signedness case; `_Pragma` macro operands remain open.
The comparison script still needs a fail-closed acceptance contract, so its
process exit alone is not evidence of agreement; these counts were inspected.
This is executable validation, not `tccpp_correct`.
