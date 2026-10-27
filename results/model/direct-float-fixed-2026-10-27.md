# Direct source-literal rounding repair

Patch 0004 now rounds decimal and hexadecimal source literals once, directly
to binary32, binary64 or binary128. The prior failures and original image
captures are retained in `direct-float-review-2026-10-27.md`. No subset or
Supported exclusion resolves them. Public newlib conversions are unchanged.

The compiler-side adapter reuses newlib's integer bignums. It accumulates all
source digits, forms an exact rational, finds the destination quantum, and
rounds the integer quotient using the complete remainder (nearest, ties even).
The same route handles subnormal carry and overflow. Definite zero/infinity
guards avoid allocating billion-bit shifts for large exponents. Hexadecimal
conversion no longer consumes the inherited scanner's truncated `bn[4]` or
rounds through a binary128 temporary. Decimal binary32 no longer calls
newlib's binary64-then-binary32 `strtof` path. Host tcc's libc path is unchanged;
the target compiler image and its independently implemented Lean model are
the objects compared here.

`Tcc.Model.PpDir` accumulates hexadecimal digits in a natural number and calls
`SF.ofBinary` at the destination format. Its decimal path calls `SF.ofDecimal`
at that format. This is executable-model agreement work, not a proof of either
rounding implementation. The typed literal-connection Statement remains open.

## Checks

- The four original same-program counterexamples now print the expected
  binary32/binary64/binary128 bit patterns on full compiler Sail execution:
  `3f800001`, `3ff0000000000001`,
  `3fff0000000000000000000000000001`, `3f800001`. Exit zero;
  compile 7,312,126 steps, program 23,161 steps, total 7,335,312.
- Its fresh native Sail handoff has all 4 MiB and handoff registers equal to
  the independently produced model arena. Compressed captures, model bytes
  and an offline comparison inventory are retained in `native-captures/`.
  This is a native observation, not kernel-certified reachability.
- Generated exact-rational matrix: 189 cases across all three formats,
  decimal/hexadecimal ties, carries, subnormal/normal boundaries, overflow,
  huge exponents, long tails, leading zeros and signs. xPack GCC/Sail passes
  189/189; the target compiler's complete fast-reference arena equals the
  model. The original 40-case binary128 fixture also retains arena equality.
- Deterministic extended matrix: 789/789 pass the fast RV64 execution and
  independent xPack GCC/Sail execution; its complete target/model arena is
  equal. The initial 400-million-instruction compilation limit was exhausted;
  a separately recorded 1.6-billion limit completes at 841,087,712 compile
  steps. The exhausted run is not counted as success.
- C11 regressions: 147 positive and 14 invalid cases pass host/GCC,
  preprocessing, cross-object and ISA checks.
- The separate 15-case native boundary probe passes tcc/Sail and xPack/Sail:
  tcc compile 17,840,159 steps, program 7,771, total 17,847,955. Its independent
  fast arena also matches. Complete native inputs, traces and console bytes
  are retained.
- Broad fresh comparison: 1,339 matching arenas / 1,813 attempts, 466
  compiler refusals, seven unsupported-assembly cases and one instruction-limit
  failure; zero byte mismatches or model errors. The separate larger-limit
  cleanup run matches. Refusals/gaps are not successful compiler correctness.
- Full `scripts/check.sh --csem` passes: ISA, discipline, generated sources,
  explicit compiler-model targets, CSem tests/baseline, pure-floating cones
  and standard axioms. A final static gate checks the subsequently added
  generated 15-case fixture; semantic sources and executable hashes are fixed.

The generator factors the existing binary128 exact-rational oracle into a
format-parameterized quotient/remainder function. Existing generated
`ld_boundary.c` is byte-identical; new generated fixtures are checked for
drift by the repository gate. Neither compiler nor model results supply
expected bytes. The stress source is retained compressed with its source pin.

## Retained attempts and limits

The full 189-case tcc/Sail execution was stopped by the lead after a fast
measurement showed 232,926,522 compilation steps. It has no traced exit and
is not successful evidence. A separate 15-case boundary probe exercises the
important rounding branches on Sail. Full-matrix execution remains available
as a reproducible longer check. Broad native coverage is still an F3 task.

The first broad launch lacked the generated corpus and stopped before any
comparison. Its inputs were then restored from the prior integration archive,
checking every extracted model-corpus source against that report's hashes.
The archive guard stopped at its unrelated preprocessing fixtures; all model
corpus inputs had been restored. No unrelated archived path was extracted.
The inherited cleanup stress input also requires more than 400 million
instructions; its separately retained larger-limit comparison matches.

No `sum.c` compile-cut chain was regenerated. IEEE library printf/sqrtl and
public conversion defects are separate F7 obligations. No elaboration budget,
permitted axiom set or theorem premise was weakened.
