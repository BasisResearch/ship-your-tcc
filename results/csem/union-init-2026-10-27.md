# C11 union designated initialization on the repin candidate

The local repair implements selected-member initialization in the shared
Cerberus source, rather than removing its guard and continuing to construct
the first member. This is a candidate for lead review; main's pin and the
frozen sum cuts are unchanged. No push, merge, external message, Supported
restriction, new refusal, proof axiom or elaboration-budget change was made.

The candidate pins remain Cerberus `704c4e697845127bced66c93fa0763eb51527ca4`
and Lem `e6d8db5680962f49060257d6c2bcb13c284b910f`, on approved Lean 4.34.
The repaired `desugaring_init.lem` SHA-256 is
`bc81c6d2f3fd1986ef598bc7d9495c3a929fb55ea5cf736d883836f11e9816d4`.

## Shared semantics

`Elem_union` now carries the selected identifier. Both the cursor and the
initializer-map path therefore identify the selected member at every nesting
level. In source order, a new initializer removes previous entries below the
addressed subobject, and removes an earlier selected member's entire subtree
when a union changes members. Descendants of the same member retain their
siblings; independent unions in different fields or array elements remain
independent. Aggregate markers retain a selection even when its contents are
implicit. Value construction selects that member and recursively zero-initializes
omitted descendants; an untouched union initializes its first member.

Two traversal details are essential: each nested braced list returns its own
aggregate position, leaving its parent to advance once; path identity uses
the map's name-based comparator rather than structural identifier equality,
which includes source locations. The outermost cursor determines the size of
an incomplete array, including when a designator ends inside a nested union.
Character-array strings expand at the selected subobject, reusing Cerberus's
old initializer walker's character-list construction. No pointer or relocation
bytes are synthesized: the selected member's typed Ail initializer reaches
the existing conversion and memory layers.

The Lean output and sync stamp were regenerated through
`scripts/csem_regen.sh`. The isolated OCaml engine overlays the **identical
`desugaring_init.lem` bytes** on pristine current upstream and regenerates
OCaml with the matching Lem. Other local frontend changes include Lean-only
filesystem seams, so they are deliberately absent from that projection;
there are no union-semantic prerequisites outside this shared file.
`scripts/csem_fork_oracle.sh` addresses the build by source/script/pin hash;
`scripts/csem_toolchain.sh fork-oracle` builds it under the common lock and
12 GB scope. The root CSem gate requires this matching engine and uses it for
both positive initialization tests and the existing execution baseline.
Pristine upstream is never overwritten.

## Focused observations

`union-init-2026-10-27/focused.json` records commands, source and binary hashes,
return codes and separate stdout/stderr. All **34 checks** have the specified
GCC result on both patched engines: **33 C11 cases plus one inherited
excess-initializer control**. All 14 former refusal witnesses are now positive,
including the string, nested-struct and compound-literal cases. Coverage adds
same-member repetition, switching away and back, nested descendants, partial
arrays and braced replacements, incomplete outer arrays, differing scalar
and aggregate member types, pointers/function pointers and static relocations,
implicit null pointers, and automatic/static storage. `union-second.c` returns
7 on both engines. Result-checker plants reject first-member/all-zero answers,
refusal/crash, empty output, wrong status, Error and mixed UB branches.

Pristine current upstream observations are separately labelled in the same
JSON: **23 refusals (exit 125), 11 defined results**. These are not repaired
engine results or successful semantic comparisons.

| fresh patched tcc/Sail observation | result | compile / program / total steps |
|---|---|---|
| `c/tests/c11/union.c` | `union 0\n`, exit 0 | 13,310,358 / 5,100 / 13,315,483 |
| `results/repro/tcc_union_designator_override.c` | `0 0 1 0\n`, exit 0 | 12,854,480 / 3,306 / 12,857,811 |
| full 33-case conforming suite | tcc refusal, exit 125 at `continuation.c` (expected 0); preserved counterexample | no program entry |
| remaining 32 cases, separate diagnostic | all 32 agree, exit 0, empty stdout | 5,839,599 / 5,665 / 5,845,289 |

The existing override reproducer also gives exactly
`Defined {value: "Specified(0)", stdout: "0 0 1 0\n", stderr: "", blocked: "false"}`
on both patched CSem engines with libc. The original C11 test is retained
verbatim as a separate diagnostic; see the remaining gaps below.
`scripts/gen_union_c11_observation.py` derives its positive active-member
observation; `scripts/gen_union_suite.py` derives the combined Sail translation
unit. Both generators have drift checks in the CSem gate.

## Gates

- Capped Lean builds 01, 02 and final 03 passed under the shared Lake lock.
- Patched OCaml build 04 passed; its exact source projection is recorded in
  `oracle-request-04.json` and `oracle-build-04.log`.
- Full parser corpus: **614 AGREE, 41 BOTH_REJECT, 9 CPP_FAIL / 664**;
  zero mismatch, crash or timeout. These retain the prior classifications.
- Static gate and full `scripts/check.sh --csem` **passed twice**, requests
  04 and final 05, on the frozen final code.
- Existing baseline: **90 MATCH, 18 UB_MATCH, 5 CERB_SKIP / 113**, all as
  baselined, now against the patched OCaml engine. All unit executables and
  stop tests pass. Matching-Lem regeneration is byte-identical (234 files);
  purity, totality, sorry-token and strengthened axiom gates pass unchanged.
- Additional focused parser check: **35/35 exact Cabs agreements**, including
  every focused fixture and the complete generated 33-case translation unit.

All request/result manifests and logs are under `union-init-2026-10-27/`.
`inventory.json` records the required ship-your-lua abstraction inventory;
no proof was added or copied in this task.

## Remaining gaps and unsuccessful approaches

These are obligations, not Supported exclusions or new refusal policies:

- **Anonymous union representation:** the existing frontend flattens its
  members into the containing struct. `residuals/anonymous-layout.c` returns
  12 on both CSem engines, where GCC returns 8. The positive anonymous-access
  control and the existing C11 test do not establish shared anonymous storage.
- **Aggregate-expression initializers:** `.s = v` for a struct-valued `v`
  still fails typing (GCC accepts and returns 2 in the focused reproducer).
  The old struct-from-struct-value control likewise remains rejected.
  This repair covers braced aggregate initialization, not general value-copy
  initialization. No new guard or refusal was introduced for that gap.
- **Aggregate-copy override probe:** with a first struct member, `.s=p,
  .s.a=3` returns 0 on GCC 15.2 and both CSem engines, but Clang 21.1 returns
  7 (the prior `p.b`). This is not evidence of implemented aggregate-copy
  initialization: the no-override first-member probe fails on **both pristine
  and patched** OCaml and on Lean, while GCC returns 7. The nested union-value
  probe also fails typing while GCC returns 7. The additional oracle disagreement
  requires separate standards review; no answer for it is counted in the
  positive matrix. Raw captures are in `residuals/additional-results.json`,
  `aggregate-value-first.json`, and the Clang capture files.
- **Independent valid-C tcc continuation refusal:** the full 33-case suite
  refuses `static struct S a = { .u.s.b[1]=2, 3, 4, 5 }` before program entry.
  The smaller **non-union** `residuals/tcc-continuation.c` is accepted by GCC
  under `-std=c11 -pedantic-errors`; both patched CSem engines return 0. Fresh
  patched tcc/Sail refuses it at line 5, and the host cross compiler agrees.
  `decl_designator` forgets the nested position after the first designator
  (`cur_field = NULL`), then advances the outer field. The refusal is preserved
  as a compiler counterexample, never counted as agreement. The full valid
  case remains in the positive CSem matrix and in the generated 33-case Sail
  source. `other-cases.py` derives a separate diagnostic to observe the remaining
  32 cases; that diagnostic does not satisfy or weaken the failed full-suite
  comparison. The lead acknowledged this independent compiler gap and queued
  a compiler **and exact model** continuation repair, with no Supported change.
- **Inactive tails:** the unchanged original `c/tests/c11/union.c` reads
  `h.s.z` and `enclosing.s.z` after selecting the integer member. Both engines'
  directly captured stdout is byte-identical: 48 executions reporting
  `UB_CERB004_unspecified__conditional` at line 32. This is not counted as a
  defined agreement with Sail. The smaller-first-member control likewise
  remains `Unspecified('signed int')` on both engines, where GCC chooses 0.
  The generated active-member observation removes only the two inactive-tail
  checks and printf, and passes as a separate positive fixture.
- **Inherited nonconforming control:** `ctl_in_struct_positional.c` supplies
  several positional initializers to a union with scalar first member.
  GCC warns and returns 71; both CSem engines return 71; tcc correctly rejects
  the excess initializers. It stays a legacy comparison, not conforming Sail
  coverage. Its rejection is confirmed by the host cross compiler. Both combined Sail
  runs actually stop earlier at the valid continuation witness below; the
  initial diagnosis attributing their failure to this control was incorrect.
- The first map-only repair exposed nested-list double advancement; the next
  repair exposed location-sensitive structural equality. Their raw Lean
  diagnostic matrices are retained as `first-lean.json` and `second-lean.json`.
  The final 34/34 matrix is `focused.json`.
- Copying the **whole** fork frontend into the OCaml build failed on unrelated
  `fs_console_fd` Lean filesystem seams. Build 02 is retained; build 03 was
  rejected as repeating it. The successful build 04 overlays only the shared
  semantic repair. Request 01 was withdrawn before launch after the cursor
  failure; superseded manifests remain evidence.
- The sandbox denies the systemd user bus. Frozen manifests were serviced by
  the lead under the shared lock/12 GB cap; no uncapped build was substituted.
  Initial 240-second Sail probes timed out before a traced exit; the longer
  bounded runs supplied the observations above.
- `scripts/csem_programs.sh` merges stdout and stderr and reported a false
  textual DIFFER when a diagnostic interrupted one verdict line. Direct
  separate-stream captures show exact original-C11-test stdout equality.

The same dead ends are recorded in `tasks/DEAD-ENDS.md`. The C11 whole-compiler
objective and Supported/Fits remain unchanged; this report makes no claim
that the remaining union/frontend gaps are solved.

The lead's 2026-10-27 disposition permits checkpointing this CSem repair for
review of its own semantics and gates, with the independent tcc counterexample
retained. Main adoption is a lead decision; this worker does not merge or push.

Final acceptance evidence is deliberately split: the 34 GCC/CSem checks and
both full gates pass; 32 runtime-comparable C11 cases agree on fresh Sail;
the full valid continuation case remains a **tcc refusal**, not agreement.
The original C11 and override programs also have fresh Sail records. The
compiler refusal and the independent aggregate-value/anonymous-layout gaps
are explicit review obligations; the shared initializer repair is checkpointed
for lead review under the lead's disposition above.
