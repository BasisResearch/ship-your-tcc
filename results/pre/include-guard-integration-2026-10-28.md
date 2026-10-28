# Completed-header opening policy and EOF identifiers

This candidate repairs the independent preprocessor's include-guard opening
schedule and an executable compiler-model EOF identifier defect. It does not
change the C11 dialect, patched compiler image, Supported/Fits or sum compile
cuts. The full gate and independent postchecks pass on the same 4,163 frozen inputs.

## Implementation

The independent phase-4 state tracks a candidate first `#ifndef`, its matching
outer `#endif` and a cache of completed physical includes. Outer `#else`/`#elif`
and substantive trailing input invalidate recognition. Comments, whitespace
and null directives preserve the appropriate boundary. Each later include
rechecks whether the guard macro is defined, so undef/redefine and recursive
includes follow the actual target behavior. The cache uses exact opened paths,
separately from pragma-once keys. Main files and synthetic predefinitions are
not cached. The ordinary ISO configuration defaults this policy off; the tcc
configuration enables it through its generator.

The model's slow identifier path now takes the character already returned by
PEEKC. At physical EOF that result can be CH_EOF while the buffer cursor still
addresses the CH_EOB sentinel. Rereading the cursor incorrectly interpreted the
sentinel as the start of a UCN. Preserving the actual character repairs the
original header ending in `#endif` without newline, plus CRLF, splice and
trigraph variants. No newline or synthetic source byte is introduced.

## Observations and retained failures

The original 32-case comparison has 14 exact observations; after the cache
repair it has 31, with the EOF model error still explicit. The final maintained
suite has 35/35 exact host/model/independent-spec comparisons of opening counts,
full positioned tokens, origins and physical source views. All original source
and header hashes are retained unchanged. Cases cover leading/trailing nulls,
comments and empty macro expansions, inactive groups, outer/nested alternatives,
predefined/undefined/redefined guards, recursion, line directives and physical
encoding boundaries. Host observations are distinct from native Sail evidence.

Six separately retained post-freeze path-identity controls also pass: dot and
parent aliases, different basenames, equal basenames in different directories,
predefined guards and undef between aliases. Each has exactly two host/model/
independent-spec openings. The initial parent-alias fixture created a host
directory without a virtual counterpart; both virtual observers reject it.
That mismatched-input attempt remains retained. The corrected inputs provide
an empty marker file in both environments; these six are not added to the
maintained 35-case count. Source review also confirms per-TU cache lifetime:
preprocess_start calls tccpp_new, and preprocess_end calls tccpp_delete.

The broad preprocessing comparison uses the identical 846 primary source
inputs before and after. Both have 838 exact tcc comparisons, one difference,
five joint errors and two spec errors; GCC has 833 exact, six differences,
five joint errors and two spec errors. Classifications and token counts are
unchanged. Five temporary-path contexts and one actual clock context differ;
the TSVs are not byte-identical. Primary and sibling fixture bytes are retained
as explicitly supplemental evidence, not retroactive full-gate input pins.

Focused Lean builds 1 and 2 fail at a new record literal's layout and a former
positional FileCtx constructor respectively. Requests 3 and 4 pass. Each request
has its own original source archive; the failures are not described using only
the final source version. The corpus-after setup initially lacked ignored
fixtures in this worktree; that setup failure and the exact copied-input ledger
remain separate from the completed 846-case run.

Three original native comparisons match the complete 4 MiB arenas. The first
full-execution follow-up failed because model_diff removes successful ELF files.
That failure and driver remain retained. The corrected follow-up relinks each
unchanged input, requires the historical ELF hash, then takes a fresh native
capture and full execution of the same retained ELF. The fresh capture is
independently compared to the complete arena; source and header bytes are
checked inside the actual ELF. All three fresh executions exit zero with empty
stdout: no-final-newline uses 3,378,144 compiler + 89 program steps, recursive
guard 3,385,111 + 89, and leading-trigraph guard 3,377,201 + 89. No reconstruction
is presented as an earlier retained execution.

The original multi-TU guarded fixture is promoted only with its source,
companion and virtual-header bytes unchanged. The independent postcheck verifies
those exact bytes against the preceding adopted observation. The separate
pack-layout residual remains an explicit semantic-transport task.

## Remaining obligations

These changes supply executable behavior and finite evidence. They do not prove
PreprocessCorrect, pragma-effects/noninterference, raw Cabs construction,
whole-run erasure, multi-TU construction/erasure, read/refill, host/binary
refinement, semantic preservation or fuel adequacy. The previously adopted pure
physical-origin/read-view and block erasure/shape theorems retain their scope.
The headline whole-compiler theorem remains open.

The portable verifier is
`python3 results/pre/include-guard-integration-2026-10-28/verify.py --sources`.
The independently verified archive has 12,092 members, all 4,163 final gate
inputs and five original source epochs. It retains six inherited gate native
captures, three original focused captures and three fresh same-ELF captures
and executions. Independent postchecks pass: 15 TU records, five completely
checked bundles and six fast arenas; 51 shared / 52 component / 51 independent
Cabs agreements among 56 controls; 58 physical controls with four guards and
15 ghost-on/off pairs. The original multi-TU fixtures retain identical bytes.

The first archive verifier used the gate artifact parent rather than its
`comparison` subdirectory and failed. Its exact source and failure are retained
and pinned under `verification-initial/`; the corrected verifier is separately
hash-pinned. No observation was rerun or substituted to correct this lookup.
Its source epochs, raw observations, complete captures and recorded failures
remain separately identifiable. Runtime filesystem bytes and image objects
are separate evidence, not retroactive additions to the gate input manifest.
