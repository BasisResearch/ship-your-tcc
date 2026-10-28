# C11 bitfield integration, 2026-10-28

The reviewed F1 bitfield checkpoint is combined with main's C11 assertion
repair and local replay-erasure theorem. It implements cfgTcc field layout,
initialization, signedness, promotion and accesses, including unnamed and
zero-width fields. The shared compound-assignment lowering now evaluates its
lvalue once for ordinary objects as well as bitfields (CSEM-14).

Partial bytes retain their known-bit masks. Reading or copying a byte with
unknown padding ranges over compatible representations instead of discarding
known field bits or assuming zero padding. Abstract volatile access receipts
retain the representation domain before padding choice; they are not yet a
proof of concrete machine events. The original frozen reconstruction bodies
remain byte-identical, with explicit historical/current representation profiles.

## Combined validation

The fresh full `scripts/check.sh --csem` gate, `bitfields-integrated-2`, passes
with 3,478 frozen inputs. A separate 269-input manifest verifies unchanged
prior-main fixture/JSON inputs; these are supplementary pins, not additional
primary request inputs. The gate includes the standard-axiom, discipline,
abstraction, generator, purity, semantic and native checks. Seven new memory
observation/erasure laws retain only the allowed standard axioms.

The first combined run is retained as a failure. Its new bitfield suite ran
before `csem_raw` was rebuilt: that executable's hash was exactly the prior
assertion checkpoint's hash. Ten constraint checks passed while 72 defined
checks failed elaboration. The gate now builds `csem_raw` and `csem_cparse`
before their first consumers. The corrected run passes all 82 required checks:
72 defined C11 cases and ten constraints. Four of eight separate GNU controls
pass; four trailing-attribute syntax refusals remain explicit.

Additional checks on the combined binaries pass: eight initialized-byte layout
rows, eleven compound/copy/negative controls and all 58 continuation cases.
The historical 54/58 result is preserved, not rewritten. The negative control
retains same-field unsequenced UB035. The byte-copy repair preserves all
permitted padding choices; the separately retained review found 32 defined
outcomes, all returning zero.

A fresh ordinary full-compiler/program Sail run of the current 80-case
bitfield batch returns zero and prints the same 80 zero-result rows as GCC.
It takes **19,538,427 compiler steps**, **148,419 program steps** and
**19,686,871 total steps**. The actual ELF, embedded sources, commands, tool
pins and native trace are retained. This is execution evidence, not a complete
compiler-handoff arena comparison for that batch. The earlier worker's
48-, 72- and 78-case batches and two individual signed-char cases remain
separately pinned; the new 80-case batch is not relabelled as 78 cases.

The combined gate separately repeats five complete 4 MiB native arena controls:
generic syntax, assertion messages and three array-alignment cases. Each also
has its distinct full compiler/program run. Their model/reference arenas,
native arena, presence maps, registers, manifests and commands are retained.
The reviewed worker checkpoint has its own full gate with 3,470 frozen inputs,
1,438 evidence files, 66 archived build records and 17 actual native ELFs.
Its observations are not pooled into the new batch or historical broad counts.

## Remaining coverage and proof obligations

GNU trailing per-member attributes are the active next F1 task. Implementation-
defined enum bitfield types are separately queued; they are not classified as
invalid C11 merely because this implementation currently selects basic integer
base types. Four actual compiler constraint deviations remain in the F3 queue:
excess-width `_Bool`, taking a field's address, `sizeof` a bitfield and applying
`_Alignas` to one. CSem and GCC diagnose these. Full native compiler captures
show acceptance, and a separate patched-host check without `-w` confirms no
diagnostic for these four inputs. The compiler repair needs reproducers and
matching model changes; no Supported exclusion is introduced.

Original `p05_bitfield.c` remains unchanged. Its promoted signed-int arguments
to `%x` produce UB153b under the retained semantics; this is not established as
a libc defect. A separately named unsigned-print variant is defined, returns
one and agrees with GCC's stdout under the candidate oracle and pristine
textual library metadata. Failed serialized-library and location-admission
harness attempts remain in the worker evidence. Default OCaml bitfield
execution still does not supply cfgTcc bitfield semantics.

Type/layout representation, volatile-event transport to F4 observations,
concrete access granularity and F6 machine-memory refinement remain open.
Finite agreement does not prove the F1 frontend or compiler simulation.
Eleven ghost contracts, the remaining runtime bodies and F1–F3 completion
remain open. CSem stays C11; the whole-compiler `tcc_run_correct` target retains
Supported/Fits premises and no refusal disjunct. Sum cuts are unchanged.

## Reproduce the evidence audit

Run `python3 results/csem/bitfields-integration-2026-10-28/verify.py --sources`
from this integration tree. The portable archive contains both combined gate
receipts/logs, stale-executable diagnosis, postchecks, fresh native batch and
five complete native captures. Its hashes also bind the separately committed
worker evidence manifest; run `results/csem/bitfields/verify-evidence.py` for
that package. No discarded failure is silently replaced by a passing capture.

The combined package contains 622 hash-verified members.
