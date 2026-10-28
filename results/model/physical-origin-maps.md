# Model-owned physical origin maps

The two inherited splice/trigraph origin failures now pass independent exact
comparisons. The final focused run has **58/58 exact** token, recursive Origin,
opening-graph and logical-byte comparisons, including disjoint physical pieces.
The full frozen CSem gate passes with a matching exit-0 receipt and 3,205
verified source pins, plus 101 unchanged supplementary adopted source pins.

The model captures byte intervals at its actual Host.read calls. Its existing
trigraph loop carries these intervals through replacement and retains the
intervals of up to two held question marks across refills. Each physical
opening owns an append-only character log and a current-buffer index map.
Successful LF/CRLF splice branches mark the actual removed entries. Every
lexer unget saves and restores the byte's read identity, including literal
prefix lookahead and the stray-backslash/CR paths. No origin is recovered by
searching equal spellings or from independent spec preprocessing output.
Compiler byte buffers, token words and compilation decisions retain their
original operations. Generated paste/pragma/predefine buffers remain distinct
from physical sources.

Token starts retain an opening ID and log cursor before consumption. Token
completion selects surviving intervals before the actual end cursor. Adjacent
pieces coalesce; an interior splice leaves distinct segments. The exported
Origin obeys the independent contract's **bounding physical span**, with its
actual stop line/column and presumed start/end lines. Separate structured
`physical_segments` retain the surviving pieces. Include introducers use the
same mapped characters: a trigraph `??=` occupies three physical bytes, and a
spliced `%:` spans its actual separated bytes. Opening IDs, full include
ancestry, macro derivation/replay identities and diagnostics remain retained.

The real log keeps ordinary CR and LF as separate entries. A separate pure
logical view merges physically adjacent CRLF and filters entries deleted by
actual splice branches. It never merges a CR and LF made adjacent only by
splicing. A missing final newline is represented by an explicit synthesized,
zero-width LF in this view, never by a fabricated read or token origin.
Closed includes retain their logs by opening ID. Missing logs export null and
fail comparison; they do not become an empty/default physical map.

## Final finite checks

- 58 required independent exact origin/opening/logical-view fixtures, including
  both original failures, LF/CRLF token/macro/literal/include splices, all nine
  trigraphs, combined trigraph splices, held/trailing question marks, positions
  8190/8191/8192, prefix ungets, plain/split CRLF, lone CR, CR at EOF, terminal LF
  synthesis, nested/repeated/empty headers, aliases and #line labels.
- Four guards reject an unmatched non-emitting opening, an unmatched prefix
  token, missing opening graphs and a missing physical log. These are harness
  perturbations of real captures, not source success counts.
- Selected token-internal splice cases check independently written segment
  endpoint lists as well as exact bounding Origin records. The transformed
  include fixtures compare complete directive spans and source ancestry.
- Existing shared-token Cabs: **45 exact / 56**, with 11 explicit nonsuccesses.
  Existing ordinary-source parser-construction component: **46 exact / 56**,
  with 10 explicit nonsuccesses. Its `raw-spec` mode executes Ghost.runRaw,
  then independently parses the model-produced positioned tokens. It does not
  call independent Raw.checked cfgTcc and does not establish independent raw
  preprocessing, semantic execution or compilerInjected authority. No spec
  grammar result feeds model construction. The 58 focused origin fixtures
  also construct Cabs successfully, but that
  does not close the remaining grammar/construction gaps.
- **15/15** ghost on/off pairs have identical complete arena files, console,
  status and stderr: seven unchanged inherited fixtures and eight physical
  transformation/EOF controls. Every origin fixture also checks section and
  console bytes on/off. These are finite erasure observations.
- **15/15** independent fast RV64I reference arenas agree, with no refusal,
  model error, byte difference, assembly gap or harness failure. These run the
  actual compiler image through rv64emu, not native Sail. No new native Sail
  coverage or broad-corpus result is claimed.

The existing component comparator retains its declared scope: ordinary model
compilation includes its real tccdefs header, while the independent component
spec uses no predefined text. It excludes exactly the identified implicit
header token prefix/opening from this component equality, and compares **all**
remaining physical model openings, selected from model IDs rather than spec
names. Extra model-only openings or prefix tokens fail. Complete captures
retain the injected header and generated buffers too. This does not establish
a separate independent equality claim for that excluded compiler-injected
prefix or the upcoming compilerInjected/runtime-binding contract.

## Open typed obligations and ledger suggestions

`Ghost.PhysicalTransformObligations` names block byte erasure, map shape,
splice erasure and refill erasure. `Ghost.PhysicalOriginMap_Statement` states
the local physical-interval law for the actual trigraph loop.
`Ghost.PhysicalReadView_Statement` relates its independently normalized shadow
to the spec's located phase 1 for arbitrary input, including CRLF. All are
**unproved**. Their axiom census is within the standard set; that census is
not a proof of their propositions. `BufferedFile.eraseGhost` and `Ghost.erase`
clear the file-local histories, held bytes, closed-file logs and saved lexer
cursor by named shape.

Lead ledger suggestion: retire the two finite physical-transformation origin
regressions after review of the final gate and evidence. Add the three typed
physical-transform/view duties alongside the six existing ghost Statements.
Whole-action erasure, successful construction for every Supported/Fits source,
independent full parser agreement, compiler-injected authority/runtime alias
transport, normalization meaning, fuel adequacy and binary refinement remain
open. CSem stays C11; Supported/Fits and the whole-compiler theorem do not
change. No sum cut is regenerated. STATUS.md/PHASES.md are left to the lead.

## Preserved dead ends

`physical-1` failed on two multiline Lean record layouts. Its matching receipt
has exit 1 and verifies unchanged inputs. `physical-2` built successfully, then
the first real comparison aborted because a synthetic lexer unget attempted to
write an absent physical map. The correction preserves the generated-buffer
distinction and restores annotations only where a physical map exists.
`physical-3` passed 51 origin controls, 45 shared/46 raw Cabs controls and 15
on/off pairs; it is superseded finite evidence, not the final source pin.

The early proposed `completed`-parameter full-log equation incorrectly treated
ordinary CRLF as already collapsed. Lead review identified that mismatch.
It was removed; the final explicit logical view and typed read-view obligation
account for CRLF, lone CR, EOF and synthesized LF without constraining away
ordinary input. No spec output is used to construct model origins. Failed
attempts remain in the evidence package and tasks/DEAD-ENDS.md.

## Reproduction and pins

Final explicit build: `physical-5` (3,205 frozen source pins, exit 0).
Final full gate: `physical-gate-1` (`scripts/check.sh --csem`, exit 0), with
3,205 verified frozen inputs and 101 supplementary unchanged adopted sources.
The lead linked the artifact/overlay-verified adopted GNU oracle into this
worktree’s ignored cache; no oracle was rebuilt or repinned.

Run the maintained comparators against the matching completed receipt:

```sh
python3 scripts/ghost_origins_cmp.py --build-id physical-5 --out build/new-physical-origins
python3 scripts/ghost_cabs_cmp.py --build-id physical-5 --out build/new-physical-shared
python3 scripts/ghost_cabs_cmp.py --build-id physical-5 --raw --out build/new-physical-raw
python3 scripts/physical_onoff_cmp.py --build-id physical-5 --out build/new-physical-onoff
```

The on/off report pins source, driver, executable, runtime and image symbols.
The fast report independently pins compiler arguments, runtime, image, source
fixtures and executable. [The evidence package](physical-origin-maps/pins.json) retains complete
observations, superseded runs, receipts, logs, source and binary pins.
[verify.py](physical-origin-maps/verify.py) checks artifact
hashes, receipts and these summary assertions; its optional source check is
only valid against the exact final frozen snapshot.
