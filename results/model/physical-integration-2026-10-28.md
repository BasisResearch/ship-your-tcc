# Physical-origin integration, 2026-10-28

The reviewed physical-map checkpoint is adopted on main's generator/alignment
snapshot after the combined full gate passes with **3,580 unchanged inputs**.
The two inherited finite splice/trigraph origin regressions are repaired.
Model-owned Host.read intervals, actual splice deletion records, lexer unget
identities and per-opening logs supply origins; independent spec output never
constructs those records. See [the implementation review](physical-origin-maps.md).

Fresh combined checks pass:

- 58/58 exact token, recursive-origin, opening-graph and logical-view controls,
  including CRLF, EOF synthesis and refill boundaries; four omission guards.
- 45/56 shared-token Cabs and 46/56 model-token parser-construction controls.
  The 11/10 explicit nonsuccesses remain. `raw-spec` independently parses
  model-produced tokens; this is not an independent Raw.checked comparison.
- 15/15 complete ghost on/off arena, console, status and stderr comparisons.
- 15/15 independent fast RV64I reference arenas, with no refusal or other error.
  Complete model/reference arena files are retained. This adds no native Sail
  execution, broad-corpus result, NoUB claim or sum compile-cut regeneration.

The focused origin comparison excludes exactly the actual implicit-header
prefix/opening from its user-source component. Captures retain it, but its
compilerInjected authority is a separate active task. Whole-C construction,
parser agreement, runtime binding coverage, fuel adequacy and binary refinement
remain open. Supported/Fits and the approved theorem are unchanged.

The ledger adds the unproved `Ghost.PhysicalTransformObligations`,
`Ghost.PhysicalOriginMap_Statement` and `Ghost.PhysicalReadView_Statement` to
the six existing ghost obligations. Their checked types and standard axiom
census do not prove their propositions. The logical view normalizes physically
adjacent CRLF and terminal LF explicitly; no completed-input premise hides
those cases.

[The retained evidence](physical-integration-2026-10-28/pins.json) includes
all fresh observations, source pins, final gate receipt/log, fixtures and arena
pairs. Run `python3 results/model/physical-integration-2026-10-28/verify.py`;
`--sources` additionally checks the exact integration snapshot. The worker
archive separately preserves failed and superseded builds. No failed result
is reclassified as success.
