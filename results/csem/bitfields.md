# C11 bitfields: representation, target layout and access

Candidate for CSEM-08, on the reviewed alignment base `2101837`. This implements
bitfields in the shared C11 frontend and the `cfgTcc` memory adapter. It does not
change Supported/Fits, compiler binding authority, or frozen sum cuts. STATUS.md
and PHASES.md are untouched. The final frozen full gate passes.

The requested `p05_bitfields.c` is named `c/tests/cfg/p05_bitfield.c` here.
Its original source remains intact. GNU extensions and compiler constraint
acceptance differences are recorded separately from defined C11 programs.

## Implementation

The shared Ctype layer retains named/unnamed width, byte offset and bit offset
in a typed `bitfield_info` view of existing private attribute metadata. Cabs
checks integer constant widths, selected integer base types, `_Bool` width one,
nonnegative/in-range widths and named-zero constraints. Unnamed fields remain
physical padding; public lookup and initializer cursors skip them. Physical
anonymous aggregate containers and the adopted union table correction remain.

RV64 placement is explicitly an implementation policy: little-endian PCC
allocation, natural allocation-unit crossing, packed straddling, zero-width
barriers, explicit alignment, union offsets and signed plain-int policy.
Patched tcc's struct layout supplies this policy, not C semantic rules.
Default Cerberus configuration still refuses bitfields explicitly; neither its
reader flag nor its OCaml oracle supplies target semantic agreement.

Ail delayed integer types preserve field width through promotions. Core reads
extract and sign-extend; assignment, compound assignment and prefix/postfix
updates narrow to field width and preserve expression promotion. Shared compound
assignment now evaluates the left operand once, repairing the inherited ordinary
scalar bug as well. This uses the actual evaluated pointer and old value, not a
new C temporary or duplicated lvalue expression. Shift promotions remain distinct.

The memory adapter merges only written bits and tracks known bits in otherwise
indeterminate bytes. Neighboring unknown fields remain unknown. Aggregate
serialization overlays overlapping storage for fields, arrays and unions.
Bit footprints identify distinct scalar fields sharing bytes, while ordinary
byte accesses overlap their bits. Volatile reads/writes persist in a separate
semantic event sequence independently of passive diagnostic capture. Each typed
`VolatileAccess` contains an abstract receipt and the exact known-bit
representation domain, before any padding-value choice. A choice does not create
another access. Concrete-value/event transport, machine refinement and headline
observation theorems remain obligations.

Lead review found that character copies lost known bits through unspecified
padding. The current repair enumerates every character value consistent with a
partially known byte, preserving live bits without zeroing unknown padding.
Entirely indeterminate bytes keep the existing unspecified-value behavior.
Automatic/static plain, signed and unsigned character-copy cases now join the
focused matrix. Build 13 and all 82 required focused checks pass.

## Evidence boundaries

All files below are under `results/csem/bitfields/`. Complete raw captures,
commands and source/binary/toolchain hashes accompany reported measurements.
Native results use fresh ordinary `scripts/run_tcc_sail.sh` runs, not a fast
runner or cross-program execution. Compressed actual compiler/program ELFs and
exact embedded sources are retained and independently hash-checked.

* Original native batch: 48/48 zero checks, 16,816,507 compiler steps and 83,786
  program steps. `native-batch/input.c` is the exact historical source extracted
  from its ELF and verified against its original pin, before fixture expansion.
* Expanded native batch: 72/72 zero checks (64 C11, eight GNU), 18,733,525
  compiler steps and 131,118 program steps. `native-expanded-receipt.json` pins
  this separate run. Independent same-source GCC output agrees on all 72 rows.
* Final expanded native batch: 78/78 zero checks (70 C11, eight GNU), adding
  automatic/static unsigned and signed character representation copies.
  `native-final/receipt.json` pins 19,289,281 compiler steps and 143,952 program
  steps; all 78 rows equal the independently compiled GCC batch. Its actual ELF,
  source and 15 artifacts were hash-verified. This does not supply CSem results.
* Native layout: eight size/alignment/tail-offset/object-byte rows, 14,450,258
  compiler steps and 65,460 program steps. Independent GCC and final-build-7
  CSem match all eight rows. Objects are explicitly byte-initialized before
  writes, so these byte observations do not choose unspecified padding silently.
* Build-7 raw cfgTcc focused matrix: 64/64 defined C11 and 10/10 rejected
  constraints; four/eight GNU controls succeed. The other four are parser
  failures at attributes following bitfield widths, not measured layout failures.
  `focused-final/report.json` retains all 82 outcomes and exact captures.
* Build-13 raw cfgTcc expanded matrix: 72/72 defined C11 programs and 10/10
  rejected constraints, including all eight character-copy storage/type cases.
  `focused-character-final/report.json` records 86/90 total successes: the same
  four GNU trailing-attribute parse gaps remain separately labelled. This is
  82/82 required checks, not a claim that every GNU source succeeds.
* Build-13 independent raw-source layout (`layout-raw-final/`) returns zero and
  matches all eight GCC/native size/alignment/offset/byte rows, using actual
  preprocessing and the injected fslist. No imported oracle AST supplies this
  final layout measurement.
* `controls-final/` retains all seven compound-assignment controls (six zero,
  callback counter one), three character-copy controls, and the negative
  same-field race. The formerly failing three-bit byte copy has 32 permitted
  outcomes, all defined zero. The same-field expression remains UB035.
* `native-signed-char/` adds separate automatic/static explicit signed-char
  copies, both zero; compiler/program steps are 3,421,480/237 and 3,411,125/200.
  These two runs are separate from the 78-row batch, with their own actual ELFs.
* Fresh raw cfgTcc continuation: 58/58 defined zero outcomes in
  `continuation-final/`. This is separate from the historical default-engine
  54/58 checkpoint. The raw lane uses actual independent preprocessing and the
  injected `c/build/fs.list`, including all adopted type aliases.
* `lead-byte-copy/` preserves the lead's minimized pre-fix candidate failure,
  full-byte and aggregate-copy controls, and a fresh native three-bit copy with
  exit zero (3,421,389 compiler steps, 242 program steps). The archived native
  ELF and embedded-source hashes were verified. It also preserves a separate
  patched-host-tcc check with warning suppression removed: all four invalid
  controls still compile with exit zero and empty diagnostics. Thus the native
  `-w` captures alone are not used to infer absence of compiler diagnostics.
* Native invalid controls: six/ten compiler refusals, four accepted and executed
  with exit zero under the actual `-std=c11 -w` invocation: `_Bool a:2`, address
  of a field, sizeof of a field, and `_Alignas` on a field. Strict GCC and CSem
  reject all ten. `native-invalid/report.json` retains each complete run and
  image. These four are compiler constraint deviations, not valid-program
  semantic successes; they do not justify weakening CSem or adding a refusal.

The earlier build-7 layout lane imported captured JSON Cabs into the cfgTcc
driver; it stays a separately classified checkpoint. Build-13 layout and focused
observations instead use the independent raw preprocessing/parser path. Plain-char host/target signedness
may differ; initialized bytes agree, not necessarily plain-char value ranges.

The original p05 result is kept as a format/runtime boundary, not a proved
formatter defect. `p05-candidate-consistent-final/` uses the final candidate
parser/executable with pinned pristine textual libc Core and metadata, retaining
the exact runtime directories those metadata locations name. Original p05 reports
`UB153b_illtyped_argument_for_format` at the raw-byte `%x` call. A separately
named variant adds explicit `(unsigned)` casts only to those four print arguments:
it is defined with value 1 and exactly matches GCC output, including original
p05 sizes and representation bytes. That return is the program's final signed
field check, not a zero-return focused case. No original source is rewritten and
no new native observation is claimed for this variant.

Earlier `p05-final.*` and `p05-pristine-runtime-final/` remain separately labelled
checkpoints. Without libc, p05 lacks a memset body. `p05-current/` retains the
mixed-version harness failure: passing pristine old marshalled libc.co to the
candidate OCaml representation drops three struct members and changes the
Core dump hash; the pristine pin correctly rejects it. `p05-candidate-final/`
retains a second harness failure from mixing metadata locations with a different
runtime directory. The final consistent run passes textual Core/JSON to the
candidate and keeps the exact pristine runtime directories for admission. Neither
failed harness is an actual compiler-semantic failure; no pin, metadata label,
source file or location-admission rule is rewritten to bypass either check.

## Shared regression and proof discipline

The Lua abstraction inventory and mandatory task table were read before proof
work. Existing layout stability, serialization equivalence, reconstruction
reference and access-erasure theorems are reused by name at ordinary budgets.
No library instruction proof is newly copied or claimed byte-identical.
Both literal historical reconstruction bodies are restored byte for byte from
the base commit. An explicit representation profile selects historical byte
decoding/first-declared union selection or current field decoding/named-member
selection. Historical recovery and current indexed/linear equivalence retain
their input/fuel quantifiers; updating a frozen body is not historical evidence.
ATTRIBUTION.md records reused sources and policy provenance. Generated Lem
outputs come only from the pinned read-only toolchain; the quotation visitor
comes from its own generator. No generated recovery copy is used.

Builds use the shared root lock and 12 GiB systemd scope after checking memory.
Every failed receipt remains failed. Build-7 passes with all 3,462 frozen input
hashes verified and supplies the focused evidence above, memory entry tests and
quotation controls. The first full gate passed project stages but found three
additional test/proof adaptations (positional byte constructor, shared semantic
leaves in legacy reconstruction, concrete footprint reduction). Build 13 repairs
them without narrowing theorem quantifiers or raising budgets. Its 3,470 frozen inputs are verified unchanged. The full
required focused matrix, direct memory/quotation controls and minimized C
controls pass on that completed executable. Full gate `bitfields-full-2` passes
with all 3,470 frozen inputs verified. The matching immutable OCaml artifact
passes via `bitfields-oracle-5`. The general build service failed
before executing oracle-4; that failure and local input verification are retained.

The final isolated default OCaml oracle succeeds at immutable key
`160ecb1fe4cc54d04820db1f0cff834d7f2db9db946d583adf29cf7f8fe2becf`.
Its completed `bitfields-oracle-5` receipt verifies the same 3,470 frozen
inputs as build 13. All 55 artifact/overlay hashes are verified; manifests are
retained in `oracle-final/`. `compound-ocaml-final/` reruns the seven shared
lowering controls on this exact binary: all six zero-return programs and the
minimal counter returning one pass. This default engine still refuses
bitfields; it is not evidence for cfgTcc bitfield execution.

Earlier immutable oracle keys, including build-7 key `b83f94fb...` and the
lead's oracle-2 seven-source candidate run, remain historical checkpoints.
Pristine upstream's duplicated-lvalue failures and the pre-fix partial-byte
failure are retained beside the successful final Lean/OCaml controls, rather
than reclassified as successes.

## Final gate and audit receipt

`bitfields-full-2` exits zero with every one of its 3,470 frozen input hashes
verified. The complete receipt, exact input manifest and compressed log are in
`bitfields/builds/`; all historical failed receipts remain failed. The gate
passes project/ISA/discipline/abstraction checks, all CSem unit executables,
allocation policy/default controls, union initialization, 92/92 anonymous
controls, the 82/82 required bitfield checks, purity/totality/stop/native-seam
checks, generated-file drift for all 235 outputs, and binding/preprocessor gates.
The default execution lane is exactly its baseline: MATCH 90, UB_MATCH 18,
CERB_SKIP 5 over 113 files. Those skips do not become bitfield support.

The axiom census reports 364 scanned files, zero declarations of axioms, zero
forbidden proof tokens, and permitted standard cones. Its 44 memory-scale and
63 fuel-contract entries pass at the original budgets. `builds/manifest.json`
verifies the archived receipt/log bytes; `oracle-final/` retains the final
immutable artifact/overlay manifests. Direct memory and quotation outputs are
`memory-character-final.stdout` and `quote-character-final.stdout`.

The final independent cfgTcc continuation rerun is 58/58 in
`continuation-character-final/`. The lead's separate final same-source byte-copy
probe is retained under `lead-byte-copy/after/`: 32/32 defined-zero partial-byte
outcomes and one each for full-byte and direct aggregate-copy controls. Its
before/after/native captures remain distinct. There is no claim that character
choices are concrete machine events; the typed abstract volatile domain and its
unproved concrete-value/event refinement remain visible.

## Ledger suggestions and open obligations

Suggested CSEM-08 entry for lead review: retain typed widths including
unnamed fields through shared translation/initialization; implement cfgTcc
layout and semantic access with masked representation bytes, precise field
footprints, single-evaluation updates and persistent volatile events. Cite the
focused, independent native/GCC layout and compiler constraint discrepancies
separately. F1 and whole compiler/memory refinement remain open.

Keep explicit obligations for volatile-event transport to F4/headline
observations, machine access granularity and F6/memory refinement, general
compiler correctness and target constraints. The four GNU trailing-attribute
parse gaps remain genuine whole-compiler coverage obligations. Additional
implementation-defined base types, notably enum bitfields, are not enabled or
validated here: the candidate currently selects basic integer base types. This
is a remaining target-coverage obligation, not a claim that enums violate C11. A future parser
repair must preserve per-declarator ownership, including unnamed fields; do not
lift an attribute to the entire declaration or borrow an OCaml AST. No C11
contract is silently relaxed to classify a GNU extension as support.

## Dead ends retained

Do not enable ignore-bitfields, infer semantics from a reader flag, flatten
anonymous containers, concatenate full base-type representations, or zero
unknown padding. Preserve incomplete-byte known bits through character copying.
Do not fix compound updates only for fields; the inherited ordinary scalar
lowering also duplicates evaluation. Do not treat distinct fields sharing bytes
as the same scalar object, or make volatile events optional diagnostic capture.
Do not run raw probes without the actual injected fslist (that only tests header
failures), use failed-build executables as accepted proof evidence, reapply
already-shared OCaml arity hunks with force/reverse/fuzz, or rewrite generated
quotation output by hand. Preserve p05 formatter/libc failures as failures.

`lead-final-review/` retains the lead's independent historical-body extraction,
24 mixed-width/base/zero-width/packed layout comparisons and eight
float-to-field/bool/compound/promotion/volatile-callback controls. All agree with
GCC on their stated finite checks; none supplies a new native claim. Large raw
stdout captures are losslessly compressed with original hashes retained.

`bitfields/validation-final.json` pins the final binaries and check counts. Run
`python3 results/csem/bitfields/verify-evidence.py` to verify the retained evidence,
66 archived build records and 17 actual native ELFs.
