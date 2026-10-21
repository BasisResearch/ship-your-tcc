# C11 patch 0003 — validation and handoff

Patch 0003 repairs all seven user-decided deviations without changing CSem's
C11 rules. It applies after 0001/0002 to pinned TinyCC 43c7708. Pristine vendor,
0001/0002, sum compile-cut artifacts, STATUS.md and PHASES.md are unchanged.

Implementation:

- UTF literals: u8 uses char, u uses unsigned short/UTF-16, U uses unsigned
  int/UTF-32. Phase 6 selects the prefix before parsing escapes separately
  for each adjacent literal. Tests include non-BMP surrogate pairs, ordinary
  literals on either side, macro operands, special macros and static/local
  initialization; character constants have the corresponding C11 types.
- Digraphs retain their spelling through stringification and pasting. They
  become punctuation for the compiler; %: also starts skipped directives.
- `_Pragma` expands its operand and destringizes quote/backslash escapes,
  then uses the existing pragma parser with isolated file, macro and token
  buffers. Rescanning executes the operator before following macro expansions.
  Tests cover pack push/pop, push_macro/pop_macro, wide operands, macros,
  expression placement, -E round trips, and tcc's pragma argument expansion.
- UCN identifiers keep both spelling and canonical UTF-8 identity, so macro
  lookup, pasting, variable names and repeated stringification agree. Annex D
  ranges and initial combining restrictions are checked; refill boundaries
  are tested byte by byte.
- Relative #line changes only the presumed filename. A later relative include
  still uses the physical directory.
- Stringification only escapes quote/backslash inside literal tokens; raw
  nonprinting bytes survive repeated stringification.
- Union initialization tracks the selected member per union type/address.
  Switching members clears old bytes, relocations and descendant records;
  repeated designators into one member retain earlier fields. Anonymous,
  nested, array, pointer relocation and automatic-object cases are included.

`gen_embed.sh` supplies `-std=c11` in every image. Cross library/compiler test
invocations, preprocessor oracles and the generated preprocessing configuration
use C11 too. The configuration changed through scripts/gen_pre_cfg.py; the
preprocessor semantics itself was not weakened.

## Commands and evidence

Final primary run: all 22 positive and six negative cases passed; the separate
tcc pragma argument-expansion check passed. Targeted cross-object Sail checks
passed 12/12 against both GCC oracles. Full compiler Sail images passed 7/7,
with exact stdout and exit code 0 in every case.

| source | compile steps | program steps | total steps |
|---|---:|---:|---:|
| utf_literals.c | 12,238,383 | 2,508 | 12,240,916 |
| tcc_pp_digraphs.c | 11,949,858 | 2,549 | 11,952,432 |
| tcc_pp_line_relative_name.c | 11,945,824 | 2,751 | 11,948,600 |
| tcc_pp_pragma_operator.c | 8,447,227 | 1,795 | 8,449,047 |
| tcc_pp_stringize_escapes.c | 11,950,385 | 2,136 | 11,952,546 |
| tcc_pp_ucn_identifier.c | 11,931,388 | 2,138 | 11,933,551 |
| tcc_union_designator_override.c | 11,961,735 | 3,306 | 11,965,066 |

Normalized results are in summary.json (regenerate with
`python3 results/review/c11/freeze_report.py` from the repository root); step metadata is under meta/. The UCN
metadata was copied from its live observation before the stale serial child
removed the working filename (summary.json records that provenance).
The reproducible primary command is:

```
JOBS=3 scripts/test_tcc_c11.py --sail
```

It builds host and RV64I cross tcc, runs 22 positive cases (including generated
raw-byte/8192-byte boundary sources), six negative UCN cases and one tcc pragma
argument-expansion check. Every positive case checks explicit expected output,
GCC C11, the host -E round trip, cross compilation and the object ISA gate.
The Sail part reuses scripts/test_tcc_patch.sh for 12 programs and runs seven
full compiler images, three concurrently. Exact argv, cwd, exit status and
stdout/stderr are in commands.json (copied from build/test-c11/commands.json).
Standalone display logs trim trailing spaces; commands.json retains the exact
captured stdout/stderr.

Additional commands:

```
make -C build/test-c11/host/tests/pp -k TCC="$PWD/build/test-c11/host/tcc -B$PWD/build/test-c11/host -I$PWD/build/test-c11/src/include -std=c11"
make -C build/test-c11/host/tests/tests2 -k TCC="$PWD/build/test-c11/host/tcc -B$PWD/build/test-c11/host -I$PWD/build/test-c11/src/include -std=c11"
scripts/test_tcc_tests2.sh --compile-only
JOBS=4 scripts/test_tcc_patch.sh
scripts/check_isa.sh c/build/base.elf
scripts/check.sh --static-only
```

Host upstream results: 24/24 preprocessing and 124/124 tests2, no residual
.output files. The three-generation self-hosting test (`make .../tests test3`
with the same TCC include flags plus -I for the host/source directories) also
passes: `Auto Test3 OK`; exact command is in selfhost.log. Cross tests2 compile/link/ISA: 103/104 pass, with 118_switch the confirmed
pre-existing failure described below (38 cases skipped by the existing lane). Broader RV64I codegen/Sail: 43/43 match both RV64I GCC and host GCC; all
43 objects and linked ELFs pass ISA checks (codegen-sail.log). Image ISA:
108,867 instructions, zero forbidden instructions; five existing ebreak notes.
Static gate passes, including generator drift, CSem purity/totality, discipline
and abstraction checks. No Lean proof changed; no lake build was run. The static
gate reports that clightgen is unavailable locally and does not re-derive sum.

## Existing failures, dead ends and environment limits

- The full cross tests2 Sail lane reports 101/104 passes, two output-comparison
  failures and one build failure, zero ISA failures. `61_integers` differs only
  by its expected UINT_MAX-redefinition warning (program-only stdout matches
  .expect). `83_utf8_in_identifiers` differs only because this older harness
  filters console noise with grep instead of decoding Sail's UTF-8 byte output;
  decoding its raw log with scripts/sail_console.py matches .expect exactly.
  These are harness classifications, not successful compiler-refusal cases.
  Diagnostics are in tests2-harness-diffs.txt; the 118 build error follows.
- Cross tests2 `118_switch` fails after `<limits.h>` then `<stdio.h>` at newlib
  `sys/_types.h:205` (`__sa_family_t`). The exact same error occurs with the
  pre-0003 source (0001/0002 only), `-std=c11`, the same xPack headers and
  TCC_RV64I_SOFT. Baseline diagnostics: baseline-118.log. It is not fixed here.
- Appending `_Pragma` as an enum keyword after the RISC-V assembler table does
  not work: duplicate spellings in that table make runtime keyword allocation
  diverge from the enum's final number. It is allocated by spelling after the
  table instead; existing token numbers stay unchanged.
- Normalizing UCNs by discarding their spelling fails repeated stringification.
  Keep raw spelling plus canonical identity. The first draft was insufficient.
- Executing `_Pragma` only after a whole macro has expanded gives wrong
  pop_macro/rescanning order. Also isolate tokstr_buf/unget_buf: the temporary
  pragma parser may expand macros while an original macro is suspended.
- Union invalidation by address alone can discard an enclosing union's record.
  Descendant type membership is needed, including coincident addresses/sizes.
- GCC's pack pragma does not expand argument macros as tcc's does. The common
  oracle fixture uses pack(push,1); the tcc-specific equivalence check is separate.
- GCC 15 defaults to C23. The shared comparison harness must pass -std=c11 to
  GCC as well as tcc; otherwise the C11-version assertion fails in the oracle.
- An early serial validation session was interrupted after final edge-case
  fixes, but its child continued writing the older build/c11-sail.log. Its
  partial results are excluded from the primary evidence; duplicate metadata
  records agree. The final primary run is build/c11-sail-final.log and starts
  from the final active patch with three concurrent full-image cases. The
  earlier serial child may still be completing duplicate checks at handoff;
  it only reads this worktree and uses the same fixed compiler image.
- The sandbox denies the user systemd bus (`Operation not permitted`). A scope
  launch was attempted and failed before execution. These C/Sail jobs are small
  (at most four regression Sail processes plus three full-image processes),
  and available memory was 85+ GiB. No Lean build or budget change was attempted.
- Worktree Git metadata is mounted read-only: ordinary git add/commit fails to
  create `.git/worktrees/codex-c11/index.lock`. Commits are therefore in the
  local object store build/c11-review.git and transported in ../c11.bundle.
  The original worktree branch is unchanged. No push or merge was performed.

## Lead integration

The bundle requires 6fb1c2d7607d30de5828b7d0c77d55642a554891, already the
worktree's baseline. From a session allowed to update Git metadata:

```
git fetch .claude/worktrees/codex-c11/results/review/c11.bundle refs/heads/c11-review:refs/heads/c11-review-ready
```

Review the branch, then integrate its commits using the lead's normal process.
The bundle itself is a transport artifact, excluded from its own commits.

Suggested ledger text: patch 0003 repairs the seven C11 deviations and all
compiler/image invocations use -std=c11. Host/cross, preprocessing, targeted
Sail and ISA results are recorded here; CSem stays normative C11. The sum
compile-cut remains pinned. Slot2's binary128 conversion patch 0004 is separate.
