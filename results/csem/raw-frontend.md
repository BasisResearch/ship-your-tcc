# Raw-source frontend connection

The pure raw-byte path now reaches positioned parser tokens and Cabs, with
per-TU MD5 and retained preprocessing provenance. The checked semantic entry
explicitly diagnoses pending pack layout and library-identity mismatches.
Those remain whole-C obligations; F0Params, Supported and Fits are unchanged.
Final full-gate receipt: `raw-frontend-gate-2` passes with source hashes verified.

## Pins

| Input | Pin |
|---|---|
| Integrated F2/designator base | `e96e2fd` |
| Cerberus | `704c4e697845127bced66c93fa0763eb51527ca4` |
| Lem | `e6d8db5680962f49060257d6c2bcb13c284b910f` |
| TinyCC | pristine `43c7708` plus existing patches, unchanged here |
| Lean | 4.34.0, unchanged budgets |
| Final continuation build | `raw-frontend-9`, shared root lock / 12 GB cap, hashes verified |

The lead adopted pure IEEE separately on main during this task. This candidate
was not rebased mid-check and does not mutate the shared toolchain, pristine
oracle, or semantic float tree. Float spellings reach Cabs intact; the adapter
never decodes them through host Float.

## Data and pure connection

`CSem.Pre.phases14WithOrigins` returns `PreprocessingResult`, with a structural
erasure to the existing `List PPToken` interface. PPToken still contains only
kind and raw spelling. The strict F2 pragma stream and its correctness,
effects and observer-noninterference Statements are preserved.

Physical positions are half-open byte offsets, line and column. CRLF and
trigraph replacements retain their entire physical spans; splicing drops
characters without resetting surviving positions. An Origin records source,
macro replacement with invocation/definition, substituted argument with index,
stringification, paste, builtin expansion, or pragma. SourceSite separates
opened path and resolved image identity from presumed #line name/line.
Physical include directives form the ancestry. A source-opening manifest
retains raw bytes even for empty or macro-only includes. Synthetic lexing
buffers have unavailable positions explicitly; conversion diagnoses missing
semantic-token anchors rather than inventing positions.

`CSemCParser.Phase7` converts complete preprocessing tokens internally using
the existing literal/number/keyword helpers. A pp-number must be one entire
language token: `08`, `1u8`, `123abc` and `1e+` diagnose instead of splitting.
Cabs retains numeric spellings and literal escape fragments for its elaborator.
Adjacent strings use the existing prefix merge, incompatibility check and
component regions. No host compiler, OCaml call, reference AST, serializer,
physical phase rerun, or native float conversion occurs in this pure path.

Valid UCN and UTF-8 spellings become one canonical lower-case ASCII UCN parser
identifier. Declarations, uses, typedef feedback, macro-produced names and
pasted names therefore share identity. Raw preprocessing spellings and original
2/6/10-byte physical widths remain intact. Identifier ranges and initially
forbidden combining ranges use [C11 N1570 §6.4.2.1 / Annex D](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf);
invalid UCN scalar values diagnose. The configured tcc physical source mapping
admits UTF-8; cfgIso retains its specified ASCII physical-input mapping.

`parsePositioned` accepts all CerbLocation.Pos fields without rewriting them.
The raw adapter declares a virtual lexing view: each language token occupies
one line, preceded by a line marker, at its physical anchor's column. End
columns use the emitted spelling, including canonical identifiers. Genuine
physical endpoints and derivations remain separate. This supplies the
preprocessed-stream coordinates that raw phase-four spellings cannot determine;
it does not claim equality with arbitrary GCC/tcc printer coordinates.

`Tcc.CSem.Raw.run` is a parsing/audit entry; runner output is labeled
`parse-audit`. It connects raw bytes through preprocessing, direct conversion
and grammar, and computes pure MD5 over the original byte array, without a
String round trip. Its result retains pragma schedules and library origins.
`checked` is the semantic entry. `frontEnd` is its Option-shaped F0 projection,
not a projection of the audit entry. No universal F0 substitution is made while
the named whole-C gaps remain.

## Pack, library identity and typed obligations

Runtime membership comes from explicit resolved image identities, never from
presumed labels. Results retain both that answer and the inherited classifier's
suffix answer. Tests cover actual runtime headers, a user #line library plant,
and a runtime header relabeled as an ordinary file. The checked entry diagnoses
mismatches as `library-identity-mismatch`; refusal is not semantic agreement.
The byte-list suffix audit avoids partial string helpers; its connection to the
inherited semantic classifier is separately stated.

Cabs and its current elaborator have no pack-state input. The checked entry
returns `unimplemented-pack-layout` for retained pack commands instead of
successful Cabs with dropped effects. This is an open whole-C gap, not a
Supported exclusion. PackLayout supplies a fixed reference: the implementation's
pack-stack policy determines an alignment cap at each token boundary, then
natural member sizes/alignments determine ordinary struct/union offsets, size
and alignment. Kernel guards expose the obstruction: `{char,int}` has natural
size 8 / offsets `[0,4]`, versus pack(1) size 5 / offsets `[0,1]`.
Actual Cabs/Core consumption, natural-field extraction and packed bitfields
remain unfinished. The reference alone does not implement them.

The unproved connections are named and typed:

- ParserConnection_Statement: agreement with the required positioned language
  tokens, supplied by preprocessing/phase-7/grammar correctness proofs.
- LibraryClassifier_Statement: byte-list audit agrees with the inherited
  semantic classifier.
- LibraryConnection_Statement: actual identities agree with the labels the
  current semantics uses, or that semantics must be changed.
- PragmaConnection_Statement: a semantic layout consumer answers every query
  against the fixed layoutReference offsets/size/alignment. The caller cannot
  choose its reference as identity on Cabs.

No ConnectionObligations instance or correctness theorem is asserted. The
structural erasure proof is rfl. GNU extensions and K&R/parser gaps remain
whole-C duties, as in f1a-parser.md; parser rejection is not a Supported fix.

## Independent validation

An IO-only serializer writes the declared positioned view for pinned Menhir's
`--cpp` hook. raw_frontend_cat.py returns those bytes without preprocessing or
automatic includes. Full Cabs, all positions, and raw-file digests are compared;
no location normalization is applied. The unchanged text frontend reads the
same positioned view. The serializer never replaces internal token conversion.

| Check | Final result |
|---|---|
| Raw golden + preserved F2 integration | 22 methods pass, no skips; 18 positive parsing fixtures, 12 required-diagnostic invalid fixtures |
| Independent full Cabs / MD5 | 18/18 agree with Menhir and the unchanged text path |
| Original parser matrix rerun | 614 AGREE / 41 BOTH_REJECT / 9 CPP_FAIL, every classification unchanged across 664 files |
| Strict focused F2 | 64 exact positives; 17 negative controls diagnose on all interfaces |
| Broad F2, 619 inputs | 613 exact raw / 611 exact spec; same six joint raw refusals, one compiler refusal and seven spec errors, no harness/observer failures |
| Checked entry | basic and mixed-name probes accepted; two pack probes explicitly unimplemented; two library plant/alias probes explicitly diagnosed |
| Harness protocol regressions | 24 pass |
| Final full gate | `scripts/check.sh --csem` passes; source hashes verified |

Full-Cabs pack rows are parsing/audit agreements, not successful semantic layout.
No refusal or diagnostic counts as semantic success. New name tests verify
UCN/UTF-8 declaration-use, typedef feedback, argument replacement, macro-name
matching and paste, while asserting the distinct raw spellings and source widths.
Number probes include hexadecimal floats and long-double suffixes.

Both final checked-frontend cones contain 6,777 constants: PARTIAL 0, UNSAFE 0,
no application opaques or native float operations. One Lean-core
String.Internal.append opaque remains through panic-message formatting inside
guarded List.getD, and seven implemented-by core fast paths retain kernel
reference definitions. This is not a zero-opaque syntactic-cone claim.
New definitions, the erasure proof and Statements use only the standard three
axioms. The inherited F0 verdict cone retains its separately recorded seams.

Final evidence is frozen under raw-frontend/ with source manifests, build/gate
receipts, full-Cabs artifacts, classifications and explicit checked diagnostics.
The Lua abstraction inventory and mandatory table were consulted before proof
work. No machine/library proof was copied or claimed instruction-identical;
ATTRIBUTION records reuse of the existing pure MD5 and Cerberus lexer/grammar.

## Dead ends retained for the lead

- Build 1 exposed missing new constructor fields and recursive calls through
  matched aliases; inputs also drifted, invalidating that receipt. Reconstructed
  recursive list shapes and explicit fields fix this without raised budgets.
- Build 2 stopped at duplicate Main roots across srcDirs; Lake requires global
  root uniqueness. RawFrontEndMain fixes it.
- Builds 3/4 caught record layout and pragma-list grouping errors; build 5 caught
  unqualified toJson in the IO runner. These are failed receipts, not evidence.
- Native String.hash in typedef membership and partial String dirname helpers
  appeared in the first cones. Ordered membership and a total byte-list label
  audit remove them. Guarded head!/getLast! preprocessing paths were totalized.
- Full gate 1 passed the semantic checks, then its axiom-output parser joined
  the last axiom row to following cone text. The census now precedes final
  #print rows; no allowed-axiom set was relaxed.
- The first fixture launch incorrectly passed a destination to the old fixed-
  path generator, producing a missing-input harness error. The corrected call
  invokes generate(Path(...)); only its 64/17 results are validation.
- Merely deferring UCN identity and returning semantic Cabs with an opaque pack
  schedule were rejected on review. Build 9 implements identity and a named
  checked diagnostic. The old arbitrary identity pragma premise is replaced
  by the fixed reference relation. Do not revive these shortcuts.
- systemd and Git metadata are serviced by the lead's frozen request protocol;
  no uncapped build, alternate index/object store, push or main merge is used.

## Worker handoff ledger text (historical)

F1/F2: provenance-preserving raw preprocessing now connects directly to
positioned Lean parser tokens/Cabs with pure per-TU Digest and UCN/UTF-8 name
identity. Structural erasure and strict F2 contracts are retained; full C11
preprocessing/parser correctness remains open. The checked semantic entry
explicitly diagnoses missing pack consumption and library-identity disagreement;
these are whole-C obligations, not Supported restrictions. F0 replacement waits
for those components. Preserve the 614/664 classifications and named F2
correctness/effects/noninterference Statements.

The final evidence freezer caught the broad F2 run's token-driver digest changing
when the full gate explicitly rebuilt non-default executables. That pre-gate
run is retained separately and is not the final binary pin. The broad matrix
was rerun against the rebuilt driver before freezing; matching source hashes
alone do not certify a stale executable. The focused matrix already used the
rebuilt driver. No semantic change is inferred from the stale binary.

The final post-gate rerun retains 613 exact raw / 611 exact spec agreements
across all 619 inputs, with the same separately classified residuals. The
freezer passes against the current executable hashes. Git add is denied by
read-only worktree index metadata; the exact frozen review commit request is
served by the lead, with no alternate index or object store.


## Adopted integration, 2026-10-27

The candidate was rebased onto the adopted pure IEEE, direct source-literal
rounding and compiler anonymous-qualifier repairs. The final full CSem gate
passes at `6795946`; all 22 focused methods pass. The 18 full-Cabs / MD5
agreements use the exact raw/text binaries produced by that gate. The final
preprocessing corpus has **622 inputs: 616 exact raw and 614 exact spec**,
with six joint raw refusals, one compiler refusal and seven specification
errors retained separately. Three added inputs exercise direct float rounding
and anonymous-container qualifiers; no observer or harness failure occurred.
The original 664-file grammar baseline above remains the unchanged baseline.

`raw-frontend/integration/` freezes fresh source and executable hashes,
full-Cabs outputs and source copies, corpus classifications and gate logs.
The first integrated focused run rejected a stale C observer; rebuilding it
resolved the diagnostic. Its failed log is retained alongside the successful
run. The earlier gate before the qualifier rebase is also retained separately.
Evidence-only rebasing onto main does not change the verified semantic pins.

This adopts the raw parsing/audit and checked connection implementation, with
pack-layout consumption, library-identity connection and the four named
Statements still open. It does not install a universal F0 frontend, restrict
Supported/Fits, or establish preprocessing/compiler correctness.
