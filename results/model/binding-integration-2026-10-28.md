# Binding meaning and model opening authority, 2026-10-28

The reviewed F1 binding checkpoint and F3 model authority checkpoint are
adopted together after the combined full gate passes with **3,597 unchanged
source pins**. Complete compiler-injected declarations now retain authenticated
opening identity and canonical runtime-alias meaning. A user opening of the
same header, a presumed-name change or a builtin-looking identifier cannot
supply that authority. User asm labels remain the DESIGN §5 exclusion.

The model records its actual predefine opening and implicit-header control
edge, full bytes and identities. It constructs Cabs from compiler actions;
independent raw preprocessing/parsing supplies comparison output only. The
shared `Phase7.identify` carrier preserves declaration identity separately
from invocation diagnostics and runtime-library membership. Full declaration
signatures and located asm arguments survive into the alias inventory.
See [F1 binding details](../csem/injected-bindings.md) and
[model authority details](injected-binding-origins.md).

Fresh combined results:

| Check | Result |
|---|---|
| Full frozen CSem gate | pass; 3,597 source pins |
| Independent authority, full Cabs/origins/opening graphs | 18/18 exact; six omission/spoof guards |
| Independent Raw.checked suite | 45/56 exact; eleven explicit nonsuccesses |
| Shared-token / model-token parser components | 45/56 / 46/56 exact; remaining cases explicit |
| Physical origins, openings and logical views | 58/58 exact; four omission guards |
| Complete ghost on/off arenas and output | 15/15 exact |
| Independent fast compiler arenas | 18/18 exact |
| Existing two-file inline invocation | one matching fast arena |
| New native Sail static/builtin memcpy shadowing arena | all 4 MiB match after 3,455,952 compiler steps |

The native run captures the actual compiler handoff; it does not execute the
compiled program or prove reachability. Prior worker full-program Sail runs
remain separately image-pinned historical observations. No broad-corpus count
is recomputed or pooled, and no sum compile-cut is regenerated. The older
46/56 `raw-spec` mode remains a parser-construction comparison on model tokens;
it is not substituted for the independent Raw.checked result.

The independent nonsuccesses are GNU omitted-middle conditional parsing,
model typeof/generic/choose/static-assert construction, two invalid inputs,
K&R construction/parsing, deferred inline replay, GNU local labels and checked
pack-layout consumption. None becomes a new Supported restriction. The generic
and static-assert model repairs are now slot 2's next task; slot 1 continues
CSem alignment/array constraints, whose new expression-alignof constructor is
not part of this frozen integration.

Runtime inventory remains **five loaded ordinary bodies, fifteen missing
ordinary targets and one alloca intrinsic duty**. Authentic declarations do
not supply missing library implementations. The F1 checks distinguish defined
executions, constraints, user-asm exclusions, absent entry points and missing
bodies; runtime gaps are never counted as successful semantic execution.

Four new raw contracts remain open: `CompilerEnvironmentImage_Statement`,
`CompilerOpeningConnection_Statement`, `BindingResolution_Statement` and
`RuntimeBindingCoverage_Statement`. Two new ghost contracts remain open:
`InjectedAuthorityTransport_Statement` and `InjectedAuthorityErasure_Statement`.
All prior raw/ghost obligations remain, including the six original and three
physical ghost contracts. Finite agreement and the standard-axiom census do
not prove these propositions. Whole-C grammar, complete multi-TU construction,
fuel adequacy and binary/library refinement remain required by the approved
Supported/Fits theorem with no refusal disjunct.

The [evidence manifest](binding-integration-2026-10-28/pins.json) covers 2,486
retained files: all fresh comparisons, combined gate binding outputs, source
pins, fixtures, arena pairs and complete native capture. The separate worker
archives retain 2,383 F1 and 6,046 model-authority artifacts, including failed
and superseded observations. Only attribution/dead-end append conflicts needed
resolution when combining the reviewed sources. The original branches remain
preserved. Verify with `python3 results/model/binding-integration-2026-10-28/verify.py`;
`--sources` additionally checks this exact integration snapshot.
