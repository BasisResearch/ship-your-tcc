# Typedef alignment: combined adoption, 2026-10-28

The compiler/model repair in [typedef-alignment.md](typedef-alignment.md) is
adopted with the GNU frontend and model-owned origins. The original typedef
query now agrees with GCC. Its three full `tcc -run` observations remain
separate from 76 cross-compiled program pairs. None is a compiler theorem.

The combined full CSem gate passes with **3,473 unchanged frozen inputs**;
**104 additional shared Lem inputs** match the already adopted GNU gate before
and after this run. These are supplementary checks, not extra inputs silently
added to the immutable gate request. The gate now explicitly builds and audits
`Tcc.Model.AlignmentAudit`. The actual compiler image was rebuilt under the
12 GB cap. No sum compile-cut artifact was regenerated.

Fresh combined-model checks give 45/56 shared-token Cabs agreements and
**46/56 raw-source Cabs agreements**. The raw run retains the actual injected
header and covers all 56 fixtures; it extends the previous three-case ordinary
check. In this mode the independent grammar parses model-produced positioned
tokens; this is parser-construction agreement, not an independent raw
preprocessing comparison. The eleven/shared and ten/raw nonsuccesses remain explicit. Seventeen
origin controls and three omission guards pass; splice and trigraph physical
origins remain the same two gaps. Seven ghost on/off checks preserve arena,
console and status bytes, and seven independent fast-reference arenas match.

The lead captured **two new full 4 MiB native Sail arenas**, for the original
typedef reproducer and expression-address probe. Both match the worker model,
including handoff registers. Fresh combined-model executions also match these
captures, using the captured image's pinned filesystem and symbol map. This
second comparison reuses the two captures; it is not two additional Sail runs.
Capture steps are 13,584,356 and 13,513,412 respectively, distinct from the
worker's complete-program traces. Both compiler patches are identical between
worker and combined sources; the runtime image pins remain explicit.

The historical corpus was rerun on the frozen worker candidate with its exact
400 prior mutant sources and `MAX_STEPS=600000000`: **1,345 identical arenas /
1,815 inputs**, 463 matching refusals and seven unsupported-assembly cases.
No other difference or harness/model error occurs. Exit 1 preserves those
assembly gaps. This is the worker candidate's corpus result, not a new broad
combined-model run. The combined checks above establish narrower regression
coverage. The earlier two denied native attempts remain retained as failures.

[verify.py](typedef-integration-2026-10-28/verify.py) checks 1,436 captured files,
source receipts and summary assertions; `--sources` additionally checks the
exact integrated source snapshot. The worker archive's 2,350 members were
independently verified. Both archives retain commands, sources, binaries/image
hashes, observations and failed approaches. Inactive inherited parser captures
were separately archived only after all 7,191 members matched; the storage
record is in `results/review/parser-run-dj9852gn-archive-2026-10-28.json`.

`Tcc.Model.TypeAlignmentRepr_Statement` remains an open F6 obligation; its
image-boundary predicate must come from the actual binary refinement. All
six ghost Statements, complete whole-C coverage and fuel adequacy remain open.
CSem alignment propagation, expression-alignof and invalid array constraints
are queued separately in `tasks/f1-gnu-alignment-semantics.md`. The old root raw
binary's GNU rejection is historical, not a claim about current main source.
Function-type and explicitly aligned member query policies remain independently
recorded GNU questions. Supported/Fits and the headline theorem are unchanged.
