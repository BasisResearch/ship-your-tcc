# Image filesystem recovery — 2026-10-27

Recovered the previous agent's `agent-ad868c434f5f27398` source model and
reviewed it against `c/src/htif.c`. The model resolves paths component by
component, retains open unlinked files, tracks descriptor reuse, and models
read/write/seek/stat/unlink plus the image's EMLINK result for link/rename.
`Config.fsFiles` and `F0Params.imageFiles` expose the initial embedded file
table; `/prog.c` is the exact input source, so reading one's own source is
observable. F6 still owes the relationship between this parameter and the ELF.

Review corrections:

- `fstat` is a descriptor operation with its own Core builtin, not a magic
  `/proc/self/fd/N` path. Literal proc-style paths undergo ordinary lookup.
- Filesystem ordering is structural, replacing the inherited all-equal
  comparator. It must distinguish states when nondeterministic results merge.
- The image runner stages matching local Core runtime, libc bodies and Cabs
  metadata; the ordinary oracle lane retains pristine upstream artifacts.
- The pinned `unistd.h` comments out `link` even though a Core proxy exists.
  Restore its declaration and correct the proxy/builtin result to loaded
  integer. `results/repro/csem_link_header.c` fails desugaring with the
  pristine runtime and returns Defined(0) with the image runtime.
- Probe errno reads and append offsets are sequenced after the calls they
  inspect. The Core-parser census now includes the new fstat proxy and tests
  its C-name mapping.

## Validation

Capped builds of `csem_run`, `Tcc.CSem.FsModel` and the fork targets pass.
`scripts/csem_gates.sh --no-build` passes: all 19 unit executables, purity and
totality checks and plants, axiom census, 227-file regeneration check, and
113 baseline oracle classifications (90 MATCH, 18 UB_MATCH, 5 CERB_SKIP).
Core-parser unit tests: 286 pass, zero failures.

The earlier nine-probe Sail batch was reviewed byte-for-byte: eight linkable
programs have identical stdout and exit 0; mkdir fails to link in the image
and triggers the existing unsupported-operation refusal in CSem. This is not
an agreement or a change to Supported. `scripts/fs_diff.py` runs a fresh batch
with source/runtime hashes and retained outputs. It requires eight exact
one-branch agreements plus one separately classified missing-library case.
The fresh batch passed: eight exact one-branch agreements, with mkdir
classified separately. All pinned input/runtime hashes remained unchanged.
Raw outputs, per-case commands and hashes are in `fs-probes/`. This batch
used the pre-ABI-integration executable; the combined RV64 configuration
recheck is recorded separately below.

## Remaining obligations

This is an executable-model increment, not a filesystem refinement theorem or
completion of F1. The driver still contains upstream failure/native surfaces;
unsupported operations retain their existing fail-closed behavior. Any such
operation reachable under the approved Supported/Fits premises needs a real
implementation or an unreachability proof, not a refusal escape. The public
mkdir symbol is absent in this image; do not infer a new Supported exclusion.

The model abstracts allocation failure and assumes offsets fit the target
representation; F1/F4 must connect these conditions to Fits. It currently
decodes the Cerberus header's O_* constants; the F2/raw-image-header integration
must reconcile numeric flag/errno/stat layouts, including programs that inspect
their values. The tests compare one CSem branch and do not prove all-branch
refinement, stdio buffering equivalence or stdout/stderr interleaving. Library
and ABI correctness remain F1/F7 work. No sum compile-cut artifacts changed.

## Combined RV64 configuration validation

After rebasing on the explicit RV64 reader, `scripts/check.sh --csem` passes:
633 targets, 19 unit executables, 230 generated files reproduced, 113 baseline
oracle classifications (90 MATCH, 18 UB_MATCH, 5 CERB_SKIP), and standard-only
axiom cones. The image runner now selects `--impl tcc-rv64` and the 64-bit
address-space top. `scripts/fs_recheck.py` checks the same eight exact byte/exit
agreements plus the separate mkdir classification, with frozen input hashes.
It reuses the captured Sail observations only after checking that base-image,
source and embedded-file bytes match the original pins. New executable,
runtime and output pins are in `fs-rv64-recheck/summary.json`; all nine new
CSem outputs are retained. The Sail compiler image remains the one recorded
in fs-probes, before the subsequent pragma-operand patch increment.

The build-lock integration review found check.sh's worktree-relative default
could permit overlapping capped builds. The main gate now derives its default
from Git's common directory; all subsequent worktree builds use that lock.
