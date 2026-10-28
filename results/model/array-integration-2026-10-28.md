# Combined array-decay integration, 2026-10-28

The reviewed repair `c08dae5` is applied as `8019c85` on main `8dd9eaa`,
including adopted alignment semantics and generic/static-assert construction.
Only actual array/VLA conversion clears the array alias's outer alignment;
its referent graph and ordinary pointer-alias alignment remain intact. The
five C actions have matching Lean actions. Comparing regenerated C trees
finds changes only in tcc.h, tccgen.c and riscv64-gen.c; vendor remains pristine.

The combined full CSem gate `array-integrated-1` passes with **3,646 frozen
inputs**. The image is rebuilt from current patches, and the serialized strict
C11 lane passes **210 positive / 27 negative** controls. Ordinary budgets,
ISA, discipline, abstraction, generator, purity and axiom checks pass.

| Fresh combined observation | Result |
|---|---|
| Alignment semantics | 54 defined-zero agreements, 12 constraint diagnostics |
| Remaining GNU alignment duties | Two function-policy gaps, three parser-placement limits |
| Complete alignment Cabs | 71 raw/text and 68 Menhir-accepted exact |
| Array-decay fast arenas | 29 successful, five separate constraint refusals |
| Focused generic/static-assert syntax | 21 exact per scope; 12 diagnostics and one wide-message residual separate |
| Original 56 shared-token / model-token component / independent raw | 47 / 48 / 47 exact |
| Opening authority and physical provenance | 18 / 58 exact; six / four guards pass |
| Ghost erasure and inherited fast arenas | 15 identical pairs / 18 identical arenas |
| Explicit two-TU inline invocation | One identical arena |

The same original decay predicate now returns zero in all four current
engines: patched tcc, GCC, checked Lean and isolated OCaml. The earlier
wrong32-versus-independent8 machine observations remain unchanged in the
worker archive and are not relabeled as successes.

Four fresh native handoff observations each compare **all 4,194,304 bytes**
with independently executed model output. Each has a separate complete
compiler/program Sail execution with exit zero and empty output:

| Source | Native handoff steps | Full compile / program / total steps |
|---|---:|---:|
| Original array-alias-decay | 3,380,808 | 3,377,597 / 89 / 3,377,711 |
| Calls and spilling | 3,464,585 | 3,461,362 / 216 / 3,461,603 |
| Aligned element referent | 3,392,324 | 3,389,135 / 89 / 3,389,249 |
| Inherited generic/assert batch, rebuilt compiler | 3,487,216 | 3,484,071 / 126 / 3,484,222 |

These are new combined-source observations. The generic batch's new compiler
step counts are distinct from its earlier image's counts. No historical broad
corpus count is recomputed or pooled. The 52 retained arena pairs consist of
18 inherited fast, one explicit invocation, 29 array fast and four native
pairs. Five matching constraint refusals are separately classified and never
contribute successful coverage.

The original host/reference selection and setup failures remain explicit in
[the worker report](array-alignment-decay.md). During lead setup, parallel image
creation and the strict C11 checker both rebuilt build/cross/riscv64-tcc;
the checker observed PermissionError during replacement. Its failure log is
retained, and the successful run starts after image creation completes. An
attempt to move the first output tree used a wrong relative path, so that
build directory was reused; no unchanged first-tree archive is claimed.
This failed setup supplies no accepted comparison.

The array conversion sites still belong to the open
`TypeAlignmentRepr_Statement`. All twelve ghost contracts, complete parser
construction, remaining grammar/pack/multi-TU coverage, runtime bodies, fuel
and binary refinement remain open at this checkpoint. The local replay proof
is reviewed separately. GNU function/member policy and CSEM-14's inherited
compound-assignment bug are separate tasks, not source exclusions. Supported/
Fits and the no-refusal headline are unchanged. No sum cuts are regenerated.

Run `python3 results/model/array-integration-2026-10-28/verify.py --sources`
on this snapshot to verify every archive member, source input, complete native
arena and cut manifest. The archive retains gate receipts, image/checker logs,
source programs, commands and separate parser/semantic observations. Its raw
Cabs component parses model tokens; the separately labeled independent raw
suite calls `Raw.checked cfgTcc` on source/image files.
