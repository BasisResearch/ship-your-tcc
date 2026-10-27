# Cleanup stress follow-up — 2026-10-27

The remaining instruction-limit diagnostic in the 1,556-input line-provenance
review is resolved by a separate run. TinyCC `tests/tests2/101_cleanup.c`
expands macros into 65,536 cleanup scopes. Compilation reaches handoff after
529,772,331 instructions, above the original runner limit of 400 million.
With `MAX_STEPS=4000000000`, the independent fast RV64I reference completes
at step 529,772,356. The unchanged model completes in 57.04 seconds and
produces the identical arena (2,436,821 serialized bytes including its header).

`cleanup-followup.json` records source/runtime/model/arena pins and the
reference completion record. No compiler or model change was needed for this
follow-up. This is a runtime instruction limit, not a Lean elaboration budget;
no proof budget changed. The native model run used a 12 GB systemd scope.
No full Sail observation is claimed for this 529-million-step run.

Together with `line-broad-review.json`, this gives 1,420 successful arena
agreements on those inputs, with 129 matching refusals and seven assembly
gaps still separate. The original broad report remains unchanged as evidence
of the original failing run. These diagnostics do not discharge the whole
compiler, Cabs ghost, fuel adequacy or refinement obligations.
