# Model-owned origins: combined adoption, 2026-10-28

The reviewed continuation (291bbb2, scope correction 442c2d2) is combined
with main 5ec6094, including NULL-symbol reservation and the independent
identical-macro-redefinition repair. Modeled compiler actions retain macro
origins, include-opening identities and typed GNU/header fragments. The
comparison scope is selected from actual model openings; three omission
perturbations fail as required. No spec AST or reference bytes enter model
computation. This is an incomplete F3 instrumentation checkpoint.

The combined full `scripts/check.sh --csem` gate passes under the shared
12 GB build lock, with all **2,722 frozen source hashes unchanged**. Subsequent
checks use those built executables and retain their hashes.

| Check | Result |
|---|---|
| Recursive token origins and opening graph | 17 required exact; two explicit physical-map gaps |
| Comparator omission controls | 3/3 reject the deliberately incomplete observation |
| Shared-token typed Cabs | 44 exact / 56; 12 explicit nonsuccesses |
| Ordinary model-only Cabs | 7/7 constructed |
| Strict ordinary model/spec Cabs | 0/3; spec rejects injected GNU header; exit 1 retained |
| Focused independent fast arenas | 7/7 equal |
| Ordinary ghost-on/off | 7/7 equal arena, console, stdout and stderr |
| Broad fast reference | 1,345 equal arenas / 1,815; 463 refusals, seven assembly gaps; exit 1 |
| Retained complete native Sail captures | Three fresh model runs agree on all 4 MiB and metadata |

The repeated identical macro definition is now a required exact origin case:
the independent spec repair preserves the latest definition, matching modeled
compiler behavior. Trigraph and splice shadow mappings remain explicit gaps.
The actual injected header remains in compiler/model execution and strict Cabs
checks; only the documented user-origin component comparison excludes its
identified initial opening. Ordinary whole-unit parser agreement is still open.

The native comparisons reuse the original include/bitfield captures in
results/model/corpus-fixtures-2026-10-28; **there are no new Sail executions**
in this integration. Native observations are not pooled with fast references.
No sum compile cuts are regenerated.

## Validation setup failures retained

The first broad invocation set an unrecognized environment variable instead
of `MAX_STEPS`; the default 400-million-step bound failed the known cleanup
case. It also used 400 different generated mutants, so its 1,312 successes
were not comparable with the adopted corpus. Its exact report, log and mutant
sources remain in the capture archive. The final serialized run restores all
400 historical inputs from hash-verified copies and uses
`MAX_STEPS=600000000`; it reproduces 1,345 successes with no arena differences,
model errors or harness failures. Matching refusals are never successes.

The first erasure driver invocation lacked its source manifest. Its failed
log and exit are retained; after restoring all seven hash-verified fixtures
and their manifest, the unchanged driver succeeds. These setup corrections
change no compiler/model/spec sources. The full gate's source manifest was
verified again before packaging.

## Remaining obligations

Six typed statements remain unproved: Ghost.Erasure_Statement,
RawErasure_Statement, ParserAgreement_Statement, RawParserAgreement_Statement,
RawConstruction_Statement and NormalizationAgreement_Statement. Complete
construction, physical transformation mapping, GNU typeof/generic/choose
expressions, static assertions, K&R definitions, deferred inline bodies and
local labels remain work. Fuel adequacy, state simulation, runtime alias
meaning and binary refinement remain open. None becomes a Supported exclusion
or a refusal disjunct. The independent GNU parser/layout candidate and typedef
compiler repair are separate ongoing tasks.

[Evidence](ghost-origins-integration-2026-10-28/pins.json) includes compressed
gate receipts, exact output reports, binary hashes and an archive whose every
member was rehashed against the loose captures. The manifest records source
revision d9bdf50 before this documentation commit. Historical worker evidence
is unchanged; its source verifiers apply only to their stated old snapshots.
