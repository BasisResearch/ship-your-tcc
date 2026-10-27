# Anonymous-container qualifiers in tcc

Patch 0003 now carries `const`/`volatile` qualifiers through promoted anonymous
struct/union members. The CSem anonymous-member lane exposed this independent
compiler bug; its semantic repair does not justify changing C11 to match tcc.

The minimized `results/repro/tcc_anonymous_member_qualifiers.c` selects the
types of `&s.a` and `&s.b` with `_Generic`. Before repair, full tcc/Sail prints
`const=0 volatile=0` and exits one; xPack GCC/Sail prints
`const=1 volatile=1` and exits zero. The repaired compiler gives GCC's result
on full Sail. The before image's `.text` hash equals the retained, adopted
direct-literal image and differs from the repaired image; these observations
are not accidentally runs of the same build.

`find_field` previously accumulated only the physical offset through anonymous
containers, returning the leaf symbol. Postfix access then applied only the
root expression's qualifiers. The new shared lookup also accumulates qualifiers
on each successful anonymous edge. Failed sibling searches do not contribute
qualifiers. The existing field-type qualification operation applies the result,
including the distinction between const pointer members and const pointees.
Initializer lookup keeps its existing interface through a wrapper.

The Lean model mirrors the successful path using named `FieldLookup` fields.
It still compiles source independently; no captured bytes, inferred reference
types or reference Cabs enter the computation.

## Checks

- Full Sail before/after reproducer: compile steps 13,415,661 / 13,415,846;
  program steps 3,246 / 3,248. Before exits one with both wrong selections;
  after exits zero with both correct selections.
- `c/tests/c11/anonymous_qualifiers.c` checks const arrays, pointer members,
  volatile unions, static and automatic objects, nested combined qualifiers,
  named-member controls, const root objects and exactly-once base evaluation.
  GCC, patched host tcc, xPack GCC/Sail and patched tcc/Sail print
  `anonymous qualifiers 0`. The tcc Sail run has 13,643,231 compile steps and
  3,215 program steps, exit zero.
- The invalid `tcc_anonymous_const_assignment.c` now receives a read-only
  assignment warning from host and cross tcc. GCC diagnoses it as an error.
  A warning satisfies the diagnostic obligation; zero compiler exit is not
  treated as a missing diagnostic. The harness requires the diagnostic's
  message, not a match against `const` in its filename.
- C11 regression lane: 149 positive and 15 invalid checks pass, retaining
  prior designator, pragma, UCN and UTF-literal coverage.
- Four focused independent arenas match, including the type probes, the
  direct-literal boundary fixture and nested-designator regression.
- Broad comparison: 1,342 matching arenas / 1,815 inputs, including 1,082
  cgen successes; 466 matching refusals and seven assembly gaps stay separate.
  Zero byte mismatches, model errors or harness errors. This run explicitly
  sets `MAX_STEPS=1200000000`, covering the cleanup stress input's known
  larger requirement; it exits one for the seven unresolved assembly cases.
- The minimized reproducer has a complete 4 MiB native Sail arena and handoff
  register match against the independent model; compressed offline-recheckable
  captures are retained.
- Full `scripts/check.sh --csem` passes, including explicit compiler-model
  targets, regenerated CSem, pure floating cones and standard axioms.

All runs use `-std=c11`. Full Sail executions and native captures are
observations, not kernel-certified reachability or compiler correctness.
Ghost Cabs, fuel adequacy and whole-corpus native coverage remain open. The
frozen `sum.c` compile-cut chain is unchanged.
