# F0: the fork in place (2026-10-21)

CSem = our fork of OathTech/cerberus-lean @e7e9bb4 in `csem/`, LemLib
(lem-lean @2d3a492) vendored in `csem/lemlib`, on Lean 4.34.0 in the root Lake
project. Changes: `csem/PORT-4.34.md`.

## Generation

* Toolchain (`scripts/csem_toolchain.sh`, outside git): opam 2.5.0 switch
  `build/csem-opam` (OCaml 5.4.0, lem pinned to lem-lean 2d3a492 =
  `lean-backend-v0.1.0-alpha.1-65-g2d3a492`, dune 3.23.1, menhir 20260209;
  ~20 min, 765 MB); the OCaml oracle built in a pristine checkout
  `build/csem-src/cerberus-lean` (`git-cerberus-lean-v0.1.0-alpha.1-171-ge7e9bb427`,
  `_build` 334 MB).
* `scripts/csem_regen.sh`: 50 s; `--check` reproduces the committed
  `csem/lean_frontend/generated` byte for byte (225 files: 172 lem outputs,
  84,055 lines; 53 copies of the hand-written files).
* Sizes: Lem sources 56,483 lines; hand-written seams 20,342 lines; unit tests
  5,778; LemLib 9,984; `csem/` in git 16 MB (generated/ 5.6 MB).

## Port and build (Lean 4.34.0, 12 GB cap, shared Lake lock)

* Three proofs changed (five `split` sites, one `simp_all` set pair, one
  case analysis), all from 4.34's `split` on `if`s; no option added; no
  `maxHeartbeats`/`maxRecDepth` anywhere in `csem/`. No elaboration-budget wall.
* From scratch (`scripts/csem_build.sh`, cgroup figures):

  | targets | wall | CPU | peak (whole build) |
  |---|---|---|---|
  | `LemLib CerberusLean` | 379 s | 646 s | 3,335 MiB |
  | then `LemLibTests CerberusLeanTest csem_run TccCSem` | 66 s | 263 s | 2,055 MiB |

  The critical path is one module: `Cabs.lean` (the generated Cabs datatypes
  and their derived instances), 243 s; the next slowest is 20 s.
* Disk: worktree `.lake` 1.0 GB (of which 0.4 GB the copied Vsa artifacts),
  `csem_run` binary 130 MB, the 19 unit executables ~650 MB.

## The fork's gates on 4.34 (`scripts/csem_gates.sh`, `scripts/check.sh --csem`)

All pass:
* copy-set sync (53 files) and the Lean lem-sync stamp;
* 19 unit executables (`effects-proof-test`, `totality-proof-test`,
  `fuel-exemplar-test`, `opaque-failure-test`, `allocator-soundness-test`,
  `memory-access-test`, `pnvi-arms-test`, …) with `test_unit.sh`'s arguments;
* exec purity (11 modules) + its 17-plant selftest; exec totality (22
  generated modules + `CerbND`, empty allowlist) + its plants;
* axiom census (`check_theorem_axioms.sh`): 0 axiom declarations (hand-written,
  generated, LemLib), 19 pinned seam rows exactly, every probed cone (driver2,
  9 entries, 35 mem-scale theorems, 63 fuel-contract lemmas) within
  {propext, Classical.choice, Quot.sound};
* sorry token: 0 in 337 files;
* generator drift: `csem_regen.sh --check` OK.

The F0 statement's own audit (`scripts/axioms_csem.lean`): every row within
{propext, Classical.choice, Quot.sound}.

## Exec lane against the Cerberus OCaml oracle

`scripts/csem_lane.py` (the fork's `test_exec.sh` taxonomy: both sides
`--nolibc`, exhaustive, whole verdict lines compared) on `csem/tests/minimal`:

| MATCH | UB_MATCH | CERB_SKIP | other |
|---|---|---|---|
| 90 | 18 | 5 | 0 |

113/113 as in the fork's committed baseline (`csem/scripts/exec_baseline.txt`,
the same 90 + 18 + 5 its README reports). The 5 CERB_SKIP rows are oracle-side
`Error` lines (exit/abort without libc; the allocator rows where the fork
deliberately differs from upstream).

## `csem_run` on our programs (`scripts/csem_programs.sh [--libc]`)

`--libc` links the C library on both sides (oracle default mode; csem_run with
the fork's `--libc` artifacts from `scripts/csem_libc_prep.sh`, whose dump
matches the fork's content-hash pin). Exhaustive unless noted.

| program | CSem (`csem_run`) | oracle | agree |
|---|---|---|---|
| `c/tests/p3/sum.c` | `Defined 0`, stdout `55\n` (0.2 s nolibc, 7.4 s libc) | same | yes |
| `hello.c` | `Defined 0`, `hello, world\n` | same | yes |
| `loops.c` (libc) | `Defined 6`, the four lines tcc prints | same | yes |
| `structs.c` (libc) | 40 executions, all `Defined 0` with tcc's output | same | yes |
| `fnptr.c` (libc) | exhaustive: timeout (300 s) on both; `--first`: `Defined 0`, tcc's output | oracle `--mode=random`: same | yes (single trace) |
| `printf.c` | `UB153b_illtyped_argument_for_format` at 11:5 | same | yes |
| `doubles.c` | UB `Invalid_format` (`%g`: Cerberus's printf has no `%g`) | same | yes |
| `headers.c` | front end refuses: `<setjmp.h>` "is not currently supported by Cerberus" | same | yes |
| `syscall_errno.c` (libc) | panic: CerbFS refuses `open` (fork contract D2: no file system) | `Defined 0`, `missing file: ENOENT` | no: CerbFS, F1's image-FS model |

Without `--libc`, `loops`, `structs`, `fnptr`, `syscall_errno` stop on the
first libc call on both sides (`calling an unknown procedure`), differing only
in the symbol number printed.

So `sum.c` (the witness program) has the outcome `.halts "55\n" 0` under CSem's
current pipeline, the outcome `tcc -run` produces on Sail (`tcc_run_sum_derived`).

## The statement (`Tcc/CSem/Statement.lean`, lib `TccCSem`)

`TccRunCorrect_Statement (P : F0Params)`, `TccppCorrect_Statement`,
`TccRunCorrectWitnessSum_Statement` elaborate. The F0 parameters and gaps are
listed in PHASES.md (Phase 4, F0).
