# Nested C11 initializer continuation

Patch 0003 and `Tcc/Model/Gen7.lean` retain the selected nested subobject in a
named `InitPath` of `InitCursor` frames. For `struct S { int a[2]; int z; }`,
`{ .a[0]=1, 2, 3 }` now initializes `a[0]`, `a[1]`, then `z`. The nested union
witness `{ .u.s.b[1]=2, 3, 4, 5 }` also compiles and executes successfully.
These valid inputs previously refused before program entry.

Each explicit brace list owns its path. Advancing consumes the deepest
array element or struct member, skips unnamed bitfield layout entries,
and ascends only when that aggregate is exhausted. A union has no next
member. A subsequent designator resets the path relative to that explicit
list; brace-elided recursion returns such a designator to its owner.
Anonymous-member lookup retains each containing aggregate instead of only
its flattened field. Incomplete arrays extend at their outer cursor.
Explicit aggregate braces and whole strings consume their selected object.

The existing union-member switch, descendant-record and relocation
invalidation remain in use. The path selects every enclosing union before
writing its descendant, so a switch still clears the overridden member and
its relocations while repeated initialization of the same member retains
its other subobjects. No diagnostic suppression was added. The original
`c/tests/c11/union.c` and override reproducer remain byte-for-byte unchanged.

`InitCursor`, `InitPath`, path push/advance, child type/address, padding skip,
and anonymous-path traversal have corresponding named C and Lean definitions.
Only ordinary compiler state reaches the model. Reference bytes, padding,
or instructions are never supplied to it. No proof theorem was claimed or
new proof premise introduced: F3 fuel adequacy, compiler/model simulation,
and all-corpus Sail arena equality remain the existing PHASES obligations.
Supported, Fits, the frozen sum cuts and pristine vendor/tinycc are unchanged.

## Final validation

| check | result |
|---|---|
| host/GCC/C11 preprocessing/cross/ISA suite | 144 positives; 14 invalid controls; pass |
| generated programs on Sail against RV64 GCC and host GCC | 58/58; RV64I objects and linked images pass |
| focused independent compiler/model arenas | 62 identical; eight matching refusals; zero differences/errors |
| broad independent compiler/model arenas | 1,306 identical / 1,811 attempts; 498 refusals; seven assembly gaps; zero differences/model/harness errors |
| repaired CSem OCaml and Lean against GCC | 52 defined agreements; six separately recorded semantic gaps |
| capped executable-model build | pass; final build 02, hashes verified |
| full gate | `scripts/check.sh --csem` passes; hashes verified |

Fresh final full compiler Sail observations (empty stdout, exit 0):

| program | compile steps | program steps | total steps |
|---|---:|---:|---:|
| minimal original reproducer | 3,337,665 | 102 | 3,337,792 |
| nested union/array/struct continuation | 3,366,665 | 112 | 3,366,802 |
| all 33 conforming union-repair cases | 6,402,061 | 5,857 | 6,407,943 |

These execute the compiler and then the generated program. Their source and
ELF hashes, exact commands, emulator hash and trace cut rows are retained.
They are native Sail observations, not a kernel proof of reachability.

## Reproduction and evidence

The focused generator is `scripts/gen_designator_fixtures.py`: 29 shapes,
each in automatic and static storage, plus eight invalid controls. It covers
arrays, structs, unions, more than four nested frames (path reallocation),
brace elision, explicit and scalar braces, later designators, reverse
selection, member switches, anonymous fields, bitfield padding, incomplete
arrays, strings and pointer relocations. Every positive must return zero;
invalid/excess controls must refuse with a diagnostic. The C11 harness also
checks preprocessing round trips and RV64I objects. Its GCC checks for these
fixtures use `-std=c11 -pedantic-errors`.

```
python3 scripts/gen_designator_fixtures.py
python3 scripts/test_tcc_c11.py
JOBS=3 scripts/test_tcc_patch.sh build/designator-focused/designator_*.c
python3 scripts/test_designator_csem.py --engine-root /path/to/sol-csem-repin
python3 scripts/gen_model_corpus.py cgen 1200 --seed 1
python3 scripts/gen_model_corpus.py mutants 400 --seed 1
MAX_STEPS=1000000000 python3 scripts/model_diff.py tests run tcc p3 model c11 tests2 gen-cgen gen-mutants -j 4 --no-report --json-out results/model/designator/broad.json --require-success 1000
python3 results/model/freeze_designator_validation.py
```

The independent repaired CSem engines use identical shared initializer source
SHA256 `bc81c6d2f3fd1986ef598bc7d9495c3a929fb55ea5cf736d883836f11e9816d4`
(Cerberus 704c4e69 / Lem e6d8db56). `designator/csem.json` pins their binaries,
commands, source hashes and separate stdout/stderr. **52/58 agree with GCC**.
Six explicit CSem gaps remain: both storage variants of anonymous-member
continuation return 1 instead of 0; four unnamed-bitfield probes fail
CSem desugaring. GCC, patched tcc and the executable compiler model agree
on those six. They are not counted as CSem agreement or excluded from
Supported. This task changes neither CSem source nor its adoption policy.

The reviewed union-repair generator and input fixtures were copied, without
editing, into `build/designator-union-src` and run there to reproduce the
**33-case conforming union suite**. This preserves strings and excludes the
suite's separately documented nonconforming excess-initializer control by
its original generator rule. Its former continuation failure remains in the
suite and now passes. See ATTRIBUTION.md and the union-repair report in the
candidate checkout. This is additional independent semantic coverage.

Final counts, exact source/runtime/model hashes and full compiler Sail cuts
are recorded in `designator/validation.json`. Focused and broad arena JSON
records every attempted source and separates successful arenas, refusals,
unsupported assembly and harness errors. The reference is the same RV64I
bare-metal compiler image, run by the diagnostic rv64emu handoff collector;
it is independent of the Lean model. These fast comparisons do not establish
whole-arena equality on Sail or a kernel compiler/model simulation proof.

The final broad run includes **1,082 successful generated cgen arenas**.
The cleanup input now reaches handoff and matches; there are no final harness
errors. The broad command correctly exits nonzero for its seven explicit
unsupported-assembly model gaps. Matching refusals (including generated
mutants) are diagnostics, never successful compilation evidence.

Both capped model builds have frozen source manifests, matching receipts and
logs in `designator/build-0{1,2}.*`. The sandbox's systemd user bus is denied;
the lead service performs builds with the shared lock and MemoryMax=12G,
after checking at least 25 GiB available. No uncapped build substitutes for
them. The final `scripts/check.sh --csem` gate has a separate frozen request/result/log;
its axiom checks retain only the permitted standard axioms in the main cone.
`freeze_designator_validation.py` rejects changed sources, mismatched
receipts, stale runtime/model hashes and unsuccessful final arena records.

## Dead ends

- An outer cursor or byte offset alone cannot represent nested continuation.
  Explicit brace ownership and anonymous containers are required.
- The first cursor version passed the initial 42 probes but an added valid
  trailing unnamed-bitfield case refused. Advancing now skips layout-only
  entries before testing aggregate exhaustion; both compiler and model use
  one padding helper.
- A missing-`=` negative control was accepted by tcc's inherited GNU extension
  policy even with `-std=c11`. The focused invalid matrix uses a missing value;
  no warning/error policy was changed to force acceptance.
- `model_diff.py all` stopped at an empty gitignored CompCert corpus in this
  worktree. The broad command names populated groups and generates its local
  corpus. This setup failure is not a compiler refusal or successful arena.
- The initial broad run on the first cursor version had **1,305 arenas,
  498 refusals, seven assembly gaps and one cleanup reference instruction
  limit**; zero arena differences/model errors. `broad-01.json` retains it.
  The final run uses final source and the already justified one-billion-step
  reference budget. The old cleanup limit is not counted as agreement.
- Initial direct CSem calls used the wrong Cabs export switch, then omitted
  `LEAN_ABORT_ON_PANIC=1`. Corrected independent captures use `--cabs-json` and
  the required fail-stop flag. Those harness failures are not semantic data.

The required Lua abstraction inventory was run (SHA256 in validation.json),
and its mandatory tool table was read. No sister-project proof was copied;
no axiom, proof escape, or elaboration-budget increase was added. The same
dead ends are recorded in `tasks/DEAD-ENDS.md`.
