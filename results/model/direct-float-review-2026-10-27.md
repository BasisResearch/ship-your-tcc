# Direct literal rounding discrepancies, 2026-10-27

The four literals in `results/repro/tcc_float_literal_direct_rounding.c`
are just above a halfway point in their destination formats. Exact rational
calculation gives the next representable value; independent xPack GCC on
Sail agrees. The current tcc image on Sail rounds all four down to one.
Both programs exit zero; their differing output bytes are failures of the
selected direct-rounding policy, not successful semantic agreement.

| literal | exact/GCC bits | current tcc bits |
|---|---|---|
| hexadecimal float | 3f800001 | 3f800000 |
| hexadecimal double | 3ff0000000000001 | 3ff0000000000000 |
| hexadecimal long double | 3fff0000000000000000000000000001 | 3fff0000000000000000000000000000 |
| decimal float | 3f800001 | 3f800000 |

Complete input ELFs, raw console/trace data, commands, source/image pins and
rational numerators/denominators are retained in the adjacent directory.
tcc takes 7,273,638 steps to the run cut and 23,161 program steps; the complete
observation reaches exit after 7,296,824 steps. This is an observation, not a
Lean reachability theorem. No sum compile-cut files were regenerated.

The source/model inspection is consistent with two causes: decimal float
rounds through binary64 (`strtof = (float)strtod` in this newlib), while
hexadecimal literals first lose information in a fixed 128-bit accumulator
and then round through binary128 before destination conversion. The current
Lean compiler model mirrors those paths. Repair both compiler and model to
round the exact literal directly; retain C11 and Supported/Fits unchanged.
The independent pure CSem IEEE semantics already uses direct rounding.
This report asserts disagreement with that selected policy, not a blanket
claim about C11's implementation-defined literal accuracy requirements.
