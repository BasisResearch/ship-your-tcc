# Array constraint review, 2026-10-28

Two header-free strict-C11 constraint violations are accepted as defined-zero
programs by both the current checked raw Lean frontend and pristine pinned
Cerberus/cerberus-lean. GCC and the adopted patched tcc reject both:

```c
typedef int F(void); typedef F Bad[1]; int main(void){return 0;}
typedef void V; typedef V Bad[1]; int main(void){return 0;}
```

Each line is a separate complete input. Neither uses GNU syntax, a library,
or an alignment attribute. These isolate the previously reported invalid-array
acceptance from GNU parser/layout extensions. The untouched upstream checkout
is `704c4e697845127bced66c93fa0763eb51527ca4`, Lem pin `e6d8db56`; its git status
was empty. The exact oracle executable, runtime path, commands and verdicts
are pinned. No inherited-bug claim relies on the patched GNU OCaml engine.

The closely related direct declarations `int f[1](void);` at file or block
scope trigger `AilTypesAux.is_complete: called an a function type` in both
engines: an uncaught OCaml failure and a Lean panic with
`LEAN_ABORT_ON_PANIC=1`. These are crashes, not successful constraint diagnostics.
Global `void a[1]` diagnoses in both paths. Block-scope `void a[1]` diagnoses
in Lean but produces `UB059_incomplete_no_linkage_identifier` in pristine
Cerberus. Those distinctions remain in the ten-case matrix.

Three ordinary controls—an array of function pointers, an integer array and
an unused incomplete extern array—are accepted with defined zero by both
semantic engines and compile successfully in GCC/tcc. The GNU overaligned-array
case is accepted by local Lean, rejected by GCC/tcc and rejected at parsing
by pristine Menhir; this remains a local extension observation.

The initial eight-case probe accidentally used an older integration-worktree
host tcc, which accepted the overaligned array. Its complete results and binary
pin are retained as a harness selection error. The final ten-case probe uses
the independently archived final typedef-worker binary
`c5b5ae1a042b4c1cda024a9712580590005fa608036e9295ee39e8dab99bc45b`.
It rejects all seven invalid inputs. The raw Lean executable matches the
passing combined gate recorded in typedef-integration-2026-10-28; no new Lean
build or native Sail execution was needed or claimed.

[pins.json](array-constraints-2026-10-28/pins.json) hashes all 173 archive members,
the matrix and the three minimal sources. The retained drivers use exact
source bytes and separate stdout/stderr/status; attributes were not stripped.
[verify.py](array-constraints-2026-10-28/verify.py) checks the portable record.
CSEM-13 in CSEM-UPSTREAM.md is ready for Kiran's review; no external report has
been sent. Repair remains queued with the CSem array/alignment task. Invalid
inputs do not add defined-C coverage, and these findings neither weaken
Supported/Fits nor establish a compiler theorem.
