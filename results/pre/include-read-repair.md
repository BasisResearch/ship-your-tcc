# Reject failed source reads — 2026-10-27

Patch 0003 now reports `could not read source file` when a physical source
read fails, in both the C11 trigraph refiller and the ordinary refiller.
`Tcc.Model.PpLex` makes the same distinction between read failure and an
empty successful read. The read failure now uses the existing compiler
error/longjmp path; it no longer silently compiles a directory as an empty
header. `FileSys.lookup` retains regular-file semantics.

Validation on the patched source:

- All existing 86 positive and six negative host/GCC/preprocessing/cross/ISA
  regressions pass, including trigraph short reads and EOF boundaries.
- `scripts/test_tcc_read_error.py`: 16/16 host/cross observations, covering
  directories, true empty files, regular files and missing files in C11/C99.
- `scripts/test_image_files.py`: 13/13 independent fast image/model
  observations, eight identical successful arenas and five identical
  refusals. Empty and trigraph-containing headers are positive controls.
- The directory include is now rejected by spec, tcc and GCC. Raw table:
  `include-read-repaired.tsv`; refusals are not successful F3 arenas.
- Full compiler on executable Sail: directory read error exits 1 before
  handoff, at traced exit step 1,891,690; the trigraph/EOF positive control
  exits 0 at step 3,304,610 (compiler handoff step 3,304,497). Exact traces,
  diagnostics and source/image pins are retained in `include-read-sail.json`
  and `include-read-sail/`. These are observations, not reachability proofs.
- Full `scripts/check.sh --csem` passes, including standard-axiom checks,
  19 CSem unit executables and the 113-case baseline (90 MATCH, 18 UB_MATCH,
  five CERB_SKIP). The frozen sum chain was checked, never regenerated.

The previous seven-arena/four-refusal filesystem observation remains a dated
record. Its directory case is intentionally a refusal now. No Supported/Fits
restriction, refusal disjunct, proof axiom or elaboration-budget change was
introduced. Other F1/F2/F3 obligations remain open.
