# GNU syntax/layout adoption: combined review, 2026-10-28

The reviewed GNU checkpoint 44dc780 is combined with the adopted model-origin,
NULL-symbol and macro-redefinition repairs. The new parser spelling field is
supplied from actual model-observed token bytes; the model-owned origin and
opening records are preserved. The combined full CSem gate passes with all
**3,573 frozen input hashes unchanged**. This is F1 syntax/layout progress;
injected runtime binding meaning and the identity-aware Supported visitor
remain the active next task, not completed semantics or a theorem.

| Check | Combined result |
|---|---|
| GNU raw semantics, full text-Cabs counterpart, independent GCC | 23 pass; eight diagnostics and three semantic residuals retained |
| Actual injected/runtime headers | Six parses pass; each retains 21 asm bindings and the aligned declaration |
| Ordinary model/spec Cabs, actual injected header retained | 3/3 exact, formerly three spec rejections |
| Shared-token model/spec Cabs | 45 exact / 56; 11 explicit nonsuccesses |
| Newly exact attribute fixture | Separate strict one-case control passes after final gate |
| Model origins/opening graph | 17 exact; two physical-map gaps; three omission guards pass |
| Finite ghost-on/off checks | Seven equal arenas, console, stdout and stderr; seven Cabs constructions succeed |
| Independent fast compiler/model arenas | 7/7 equal; no refusals or other errors |
| Existing semantic regressions | 92 anonymous-member checks, 24 library checks/5,580 consumer probes and declaration gate pass |

The ordinary Cabs comparison applies the independent parser to the modeled
positioned token stream. It establishes exact parser-output controls, not an
independent whole-preprocessor theorem or semantic meaning for injected asm
bindings. Separate origin controls and raw-header checks retain their explicit
comparison scopes. All six typed ghost Statements remain unproved. The model
still needs complete construction, including declaration __extension__ markers
and other GNU forms; the independent parser's new support does not supply
model syntax automatically.

## Gate repair and retained failure

The first combined gate passed, but a fresh GNU comparison exposed a stale
standalone `csem_cparse` executable: raw execution succeeded while the old text
parser rejected GNU attributes. Its failed complete comparison and executable
pins are retained. The CSem gate now explicitly builds both `csem_raw` and
`csem_cparse`, then runs the maintained GNU semantics and actual-header suites.
The final gate passes those checks. This changes no compiler image bytes.

The 45-fixture, origin and ordinary Cabs observations were made after gate 1;
the package verifies their ghost executable hash is identical to gate 2's.
The strict new attribute control and seven arena/on/off regressions were run
after gate 2. No broad-corpus or new native Sail execution is claimed here.
The previous 1,345/1,815 broad observation and separately pinned native GNU
query failures remain historical evidence, not silently pooled successes.
No sum compile cuts were regenerated.

## Remaining work

Compiler-injected aliases still diagnose during semantic elaboration. The
legacy Supported visitor's reliance on parser rejection of user asm labels
is stale; tasks/f1-injected-bindings.md must supply real binding meaning and
exclude user asm labels while admitting genuine compiler bindings by resolved
opening/content identity. Blanket admission or blanket exclusion is not a
solution. Nonliteral alignment expressions, pragma pack consumption, other
GNU effects, complete Cabs construction and the whole-C connection proofs
remain open. The compiler/model typedef-alignment repair is independently in
flight, with its own changed-image evidence requirements.

[Evidence](gnu-integration-2026-10-28/pins.json) contains both gate receipts,
final summaries, executable pins and 1,665 hash-verified archive members,
including the stale-parser diagnostic and exact erasure replay commands.
The worker's original checkpoint package remains unchanged; its source
verifier applies only to 44dc780. The combined verifier checks current-source
pins with `--sources`, or artifact integrity alone without that option.
