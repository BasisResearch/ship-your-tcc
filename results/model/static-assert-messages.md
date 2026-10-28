# C11 static assertion message encodings

The actual patched compiler and its Lean model now accept ordinary, `u8`,
`u`, `U` and `L` assertion messages, including compatible adjacent strings.
File, block, struct, union and nested aggregate declarations retain complete
unevaluated syntax. The scanner's original message components, escapes,
encoding prefix, locations and physical/include/macro origins construct the
model Cabs independently of the specification parser.

The repair is assertion-specific: `parse_assert_str` consumes the existing
phase-6 string tokens, using `TOK_IS_STR` and `TOK_STR_SIZE`. The C helper in
patch 0003 and its `Gen2.lean` mirror render basic diagnostic characters from
1/2/4-byte code units; nonbasic and embedded-zero units become `?`, preserving
basic text after them. `Gen8.do_Static_assert` retains its original expression,
message and declaration wrappers. No decoded diagnostic buffer constructs syntax.
`parse_mult_str` and its asm-label/section/alias/visibility consumers are
unchanged, including their inherited acceptance of `u8` as a char string.

C11's assertion grammar permits every string-literal encoding. Ordinary
strings may concatenate with one encoded kind, and identical encoded prefixes
may concatenate. UTF-8 plus a wide kind violates a constraint; different wide
kinds have implementation-defined concatenation policy. The existing compiler
and independent grammar reject those mixed-wide combinations, separately from
malformed inputs. False assertions require diagnostics containing basic message
characters. These classifications follow [WG14 N1570, §§6.4.5 and 6.7.10](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf).

The actual reconstructed pre/post C trees contain **552** regular files.
Only `tccgen.c` changes. Removing the helper and restoring its one call site
reproduces every original C source byte; regenerated patch-hunk reshuffling
has no other semantic effect. The portable tree audit records both complete
file maps and the actual C diff. Vendor remains pristine.

## Preserved refusal and pins

The exact source `_Static_assert(1,L"wide"); int x;` and its original model
refusal/independent Raw.checked success remain in the unchanged
[unevaluated evidence](unevaluated-syntax/evidence.tar.xz), with all historical
source, image, tool and binary pins. Its verifier passes all 6,830 members.
This result copies the original focused refusal captures and pins the prior
archive; it does not relabel that historical refusal as a success.

A fresh host build of the exact pre-repair patched source also refuses those
same bytes. The repaired host compiler and strict `gcc -std=c11
-pedantic-errors -c` accept them. That reconstruction is explicitly a fresh
host observation, not an original/native Sail capture. The focused syntax
suite reuses the exact original source without adding `main`.

The current image was rebuilt from pristine vendor plus current patches before
all new native observations. Strict C11 host/cross rebuilding finished before
image creation, avoiding the previously recorded cross-binary replacement race.
The source freeze, compiler-image hash, runtime key, actual stripped injected
header, model/ghost executables, host/cross compiler binaries, xPack GCC
15.2.0 tool and native capture tool/build manifest have separate exact pins.
No sum cuts are regenerated.

## Validation

The focused explicit build `assert-messages-lean-1` passes with **3,379**
verified frozen inputs under the shared root `build/lake.lock` and systemd
MemoryMax=12G, following a memory check above 25 GB available. The combined
full CSem gate **assert-messages-full-1** also passes with the same 3,379
unchanged pins. It includes standard-axiom, purity, discipline, abstraction,
generator, semantic and native controls. Both matching receipts are archived. Sources remain
frozen until each matching completed receipt. No budget or proof changes are
introduced; the reviewed replay-erasure proof is unchanged.

| Check | Result |
|---|---|
| Strict patch regression lane | 274 positive programs / 27 inherited negative controls |
| Message syntax, shared tokens | 65 exact complete Cabs |
| Message syntax, independent grammar on model tokens | 65 exact complete Cabs |
| Message syntax, independent Raw.checked on source/image | 65 exact complete Cabs, recursive origins, openings and all 21 injected aliases |
| Message compiler/model diagnostic comparisons | 61 exact refusals; zero successes |
| New executable fast reference/model arenas | 64 exact pairs |
| Inherited shared / model-token / independent raw suite | 47 / 48 / 47 exact of 56; other rows remain nonsuccesses |
| Inherited physical origins and omission guards | 58 exact / 4 guards pass |
| Inherited ghost on/off pairs | 15 identical arena/console/status/stdout/stderr pairs |
| Inherited independent fast arenas | 18 exact pairs |

The exact historical snippet has no entry point and belongs to syntax and
compile-only checks. The 64 fast executable controls are the other valid
fixtures, including a defined batch. Both independent arena files are retained
for every one of the 82 new/inherited fast pairs. These are fast RV64 diagnostic
observations, distinct from the full native Sail comparisons.

All false/malformed/policy controls are outside successful syntax counts:
30 false assertions (five encodings in file/block/struct/union contexts plus
zero/nonbasic text), seven malformed inputs, six UTF-8/wide constraints,
six mixed-wide policy refusals and twelve ordinary-string-only consumer
refusals. The message-free assertion is one separate extension: host/cross
tcc accepts it, strict GCC diagnoses it, and the C11 syntax comparison records
its nonsuccess. Eight narrow/u8 GNU-consumer controls check unchanged compiler
policy without adding C11 syntax coverage. The compiler matrix stores each
engine's status and diagnostics separately; false-message checks require both
`before` and `after`, including after zero/nonbasic units.

The combined gate passes 18 independent injected-opening controls and six
authority guards. It also repeats
GNU alignment/array semantics, generic syntax and complete native arena/full
execution controls: five complete 4 MiB arena pairs and five separate
exit-zero program executions. Its array lane separately has 29 fast successes
and five constraint refusals; these are not pooled with the new 64 controls. The message batch uses all five encodings with ordinary
concatenation in file/block/struct/union contexts and a defined integer return.
The new message batch matches all **4,194,304** native arena bytes at
**3,605,779** steps. Its separate full compiler/program run exits zero with
empty stdout: **3,602,631** compiler steps and **133** program steps. The
comparator uses its fixture filesystem; the full run uses the ordinary
executable invocation. These have distinct images and commands. Complete-arena
hashes and the inherited native controls are recorded in the portable
inventories; these are observations, not refinement proofs.

## Evidence, dead ends and ledger suggestions

Run `python3 results/model/static-assert-messages/verify.py --source-root .`
to check the [portable package](static-assert-messages/evidence.tar.xz), every
member hash, separated counts, historical package pin, unchanged source freeze,
both fast arena files and every byte of the native exports. All **11,712**
members verify; the xz package is **24.2 MB**. The generator
`freeze.py` records exact membership; failed captures and superseded checks are
retained. Build/receipt IDs and lead-review text are included.

The direct root-locked 12 GB Lean attempt fails on the sandbox's systemd user
bus before Lean starts; the existing capped frozen service supplies the build.
The initial diagnostic postcheck expected a `same-refusal` totals key, but
`model_diff` reports refusals in rows and only successful `same` in totals.
All 61 observed rows were correct; the corrected verifier checks those rows
and zero successes without rerunning or altering observations. The first fast
export used effective `compiler_args` in a cache key that records explicit
arguments only: `-std=c11` is already prepended by both harness entry points.
Reusing `compare`/`ref_run`'s explicit `['-w']` key retrieves the existing pairs.
These are evidence-harness failures, not compiler residuals.

Suggested lead ledger update: retire the valid wide-message compiler residual
and record the finite all-encoding syntax/arena/diagnostic checks. Keep the
whole-C Supported/Fits scope, C11 grammar, source-construction/transport,
physical/opening authority, fuel adequacy, whole-run erasure and F6 binary
refinement obligations unchanged. No universal correctness theorem follows
from these tests, and no refusal exclusion or specification relaxation is used.
Lua's inventory and mandatory table were read; existing scanner, phase-6,
Cabs and fast/native harnesses are reused by name with attribution. No library
machine proof is copied, so no library instruction-byte identity claim is made.

Normal `git add` cannot create this worktree's read-only `index.lock`.
The [exact staging stderr](static-assert-messages/git-staging.stderr) is
retained alongside the package. The existing lead commit service receives
only the final scoped path hashes after validation, as MANAGER.md prescribes;
no alternate Git store, push or main merge is used.
