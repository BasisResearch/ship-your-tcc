# F3 corpus include fixtures

The shared fixture materializer now gives preprocessing and compiler comparisons
the same explicit local files: the source itself, sibling headers and directly
quoted local includes, with basename and parent-directory aliases. The old F3
harness supplied only sibling headers. Consequently tests2 `18_include.c`,
`95_bitfields.c` and `95_bitfields_ms.c` previously stopped on both engines
because their input files were missing. Their old observations remain in
`results/model/anonymous-members/broad.json`; those refusals were never arena
coverage. The repair changes the harness, not the compiler, model or image.

The final 1,815-input comparison has **1,345 identical successful arenas**,
**463 matching refusals** and **seven assembly gaps**, with no arena difference,
other model error, harness failure or skip. This recovers exactly those three
inputs from the prior 1,342-success/466-refusal observation. Generated successes
remain 1,082 C generator cases and 80 mutants. The broad command exits **1**
because the seven explicit assembly cases remain unsupported; this is not an
all-corpus passing exit. Assembly exclusions and model obligations remain as
specified in DESIGN §5, without changing Supported.

All three recovered programs also match the **complete 4 MiB arena and handoff
metadata on native Sail**:

| input | native Sail steps to handoff |
|---|---:|
| `18_include.c` | 13,652,355 |
| `95_bitfields.c` | 34,581,933 |
| `95_bitfields_ms.c` | 34,047,039 |

Complete compressed arenas, presence maps, registers and cut metadata are in
[corpus-fixtures-2026-10-28/captures/](corpus-fixtures-2026-10-28/captures/).
Each capture was independently re-compared while freezing the evidence. The
successful capture protocol retains exact ELF hashes and input/image pins;
it removes successful temporary ELFs. These are finite native observations,
not reachability theorems or whole-corpus Sail coverage. They test the compiler
model's bitfields, not the still-open CSem bitfield semantics.

The final broad run uses 600 million fast-reference steps, covering the known
529-million-step cleanup case. Native captures use a 100-million-step bound;
the include capture uses a 900-second wall limit and bitfields use 3,600 seconds.
The first two bitfield captures timed out at 900 seconds and remain explicit
harness failures in `native.json`. A stopped overlapping broad diagnostic is
also retained: model_diff invocations with overlapping inputs currently share
scratch names. The accepted native and broad runs were serialized. Neither
failure is reclassified as a compiler refusal or a successful comparison.

The initial focused fast result predates the final source/fixture pin checks
and is retained as `focused-initial.*`, with its own script hash. The final
broad result and three native observations verify current source/fixture,
image, helper and binary pins. Runtime input hashes are retained separately.
The final static gate passes, along with 14 preprocessing integration methods,
six reference-protocol methods and seven native-capture protocol methods.
`manifest.json` hashes the retained evidence. Sum's compile-cut was not
regenerated. Whole-C ghost construction, fuel adequacy, compiler refinement
and all-corpus native Sail equality remain open.
