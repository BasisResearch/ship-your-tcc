# GNU attributes on individual bitfield declarators

Candidate on reviewed F1 base `e053f1e`. The four original
`gnu_member_{packed,aligned}_{auto,static}.c` failures were parser refusals at
attributes after widths. They were never established layout mismatches. Their
original sources and historical failures remain unchanged in the bitfield
report. This candidate preserves and elaborates their attributes, including
unnamed padding, without changing C11 constraints or Supported/Fits.

Cabs now gives `SDecl_bitfield` an explicit `Annot.attributes` carrier beside
its optional declarator and original width expression. Existing named
pre-width declarator attributes retain their original attachment; leading and
width-suffix field attributes stay with their own member. The shared frontend
combines them only when elaborating that member. An absent declarator stays
absent: no public identifier is fabricated for padding. Source coordinates,
qualifiers, width metadata, byte/bit offsets and initializer traversal survive.
The Lean grammar/export, legacy-compatible JSON import, Supported visitor,
compiler-action construction and both isolated OCaml printers/grammar consume
the new constructor. Generated Lean comes from the pinned generator.

The target audit found an additional compiler bug only after parsing worked.
TinyCC retained `ad1` from one comma-separated member when starting the next.
The same 42-row source gives **24 differing layout/byte rows** before repair
and zero afterwards. Patch 0005 and `Gen3.struct_decl` now reset each member
from the common specifier attributes; legitimate declaration-leading attributes
still apply to all its declarators. CSem independently keeps explicit member
alignment separate from aggregate packing, and honors GNU aligned barriers on
zero-width fields while ignoring packing there. `_Alignas` on a bitfield still
violates the C11 constraint.

The frozen focused build `bitattr-lean-4` passes with all 3,575 inputs verified.
The isolated default OCaml build `bitattr-oracle-1` verifies the same inputs
and all 55 artifact/overlay entries, at immutable key
`f77d8d458f5032a4dd0e87275084fe347652b45928183a37846dae8128c8668b`.
This oracle supplies independent Cabs and ordinary semantic regressions; its
configuration still does not execute target bitfields. No matching refusal
is counted as bitfield semantics.

Text/Menhir Cabs comparisons use the existing historical-position projection
only for `source_identity` metadata those paths cannot encode. Every AST node,
attribute argument, width and diagnostic coordinate is compared. The independent
compiler-action/Raw.checked controls retain full raw metadata.

The retained observations are separate:

- Focused raw cfgTcc: 46 defined-zero programs, including the four original
  sources. All 42 generated initialized-byte layouts match fresh GCC in size,
  alignment, tail offset, every byte and exit value. Complete raw/text Cabs
  agree for all 54 syntax controls. Eight exploratory pre-width/comma-leading
  storage variants receive GCC syntax diagnostics, separately from defined
  programs; none is called a layout disagreement or a Supported exclusion.
- Independent Menhir: 46 exact complete Cabs/digest comparisons and eight
  unsupported placements. These include the two valid GNU attributes between
  the base specifier and declarator, plus six of the separate GCC-diagnostic
  placements. Their actual diagnostic streams remain intact.
- Original four sources: four fresh full-compiler Sail runs and independent
  GCC executions return zero. Compiler/program steps respectively are
  3,402,649/130 and 3,400,239/104 for aligned auto/static, and
  3,414,274/162 and 3,406,929/110 for packed auto/static.
- Same-source full compiler Sail layout batch: all 42 rows equal fresh GCC,
  exit zero, 21,589,947 compiler steps and 679,065 program steps. The original
  before-image/console is retained separately (21,583,578/878,099 steps).
  Both runs embed exactly the same source SHA256
  `1536c32852c4794491a211ce6227744eefb0bbd0b716a335c10dc419a47fcd74`.
  Actual ELFs and embedded sources, runtime objects and toolchain pins are
  retained; these are complete compiler executions, not cross-program runs.
- Compiler/model: one exact independent fast arena for that generated batch.
  Twelve focused compiler-action Cabs controls, including all original four,
  unnamed/zero-width fields, mixed ordinary members, volatile and nested
  containers, equal independent Raw.checked Cabs. A thirteenth leading
  declaration-attribute control exposes the inherited generic specifier-action
  construction gap; it remains an explicit nonsuccess.
- Adopted semantic regressions: strict C11/GNU bitfield matrix 90/90, including
  ten C11 constraints; raw initializer continuation 58/58. The alignment matrix
  retains 54 defined agreements, twelve constraints, two function-policy
  obstructions and three parser-placement limitations. Fresh host/GCC/cross/ISA
  C11 checks pass 274 positives and 27 negatives, with assertion-message
  diagnostic classes still separate.

Full gate `bitattr-full-1` passes with all 3,575 frozen inputs verified. It
includes the exact lead-reviewed gate dependency fix from `79e1454`, rebuilding
both raw executables before consumers. Generator drift checks reproduce all
235 outputs. The 364-file token scan is clean; there are zero axiom declarations
and the audited theorem cones use only the permitted standard axioms. The
44 memory-scale and 63 fuel-contract entries pass at their original budgets.
Unit, union/anonymous, allocation, raw/binding/preprocessing and model checks
pass; the default execution baseline stays 90 MATCH / 18 UB_MATCH / 5 CERB_SKIP
over 113 files. The only input difference from the focused build/oracle
checkpoint is the reviewed gate dependency fix.

`bitattr-full-1` ran zero optional native arena/execution controls because
`capture_cut` was unavailable in this worktree. The separate 42-row layout
batch and four original-source full-compiler Sail captures supply the native
evidence listed above. The lead combined gate will repeat the inherited
complete-arena controls after integration.
The portable evidence is [evidence.tar.xz](bitfield-attributes/evidence.tar.xz),
with [manifest.json](bitfield-attributes/manifest.json). Run
`python3 results/csem/bitfield-attributes/verify.py` to check every retained
artifact, six actual native ELFs and their embedded sources, and the reported
finite counts. Failed/diagnostic captures and exact source/image/toolchain pins
are retained alongside successes. No source restriction, sum-cut regeneration,
push or main merge is involved.

The remaining whole-compiler obligations include universal parser/ghost
construction and agreement, the leading-specifier compiler-action gap above,
Menhir's explicitly recorded unsupported placements, F4/F6 memory and binary
refinement, volatile concrete-event transport, enum bitfield policy and the
previous four native C11 constraint-acceptance deviations. Original p05's
libc/format boundary remains as previously reported. Finite comparisons prove
none of those statements and add no refusal disjunct.

Dead ends retained: `bitattr-lean-1` fails service setup before a verified
completion; `bitattr-lean-2` verifies 3,531 inputs and rejects a mutable-name
shadow in GhostCabs. Build 3 corrects that name and passes. The eight GCC
syntax diagnostics are kept outside the defined native batch. The original
combined CSem printf-layout diagnostic made no result and was interrupted;
the wrapper returned 130, and the lead verified the exact orphan command/cwd
and source before SIGTERM after 1,542.695 seconds since capture creation. Its
termination/elapsed receipt is separate from all successes.
Use the bounded independent per-case layout programs rather than waiting on
that combined semantic execution or raising fuel/budgets. All old failures
remain failures. Lua's inventory/mandatory table was consulted, existing
attribute/layout/access machinery was reused, and no library instruction
proof is newly copied or claimed byte-identical.

Ordinary staging fails because this worker cannot create its Git index lock;
[the exact stderr](bitfield-attributes/commit-stage.stderr) is retained. Final
packaging uses the existing frozen lead commit handoff from MANAGER.md. No
alternate index/object store, permission escalation, push or main merge is used.
