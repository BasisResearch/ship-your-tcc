# CSem import and Lean 4.34 adaptations

The candidate imports OathTech/cerberus-lean `mdd/cerberus-lean` at
`704c4e697845127bced66c93fa0763eb51527ca4` (2026-10-26), with matching
lem-lean `mdd/lean-backend` at `e6d8db5680962f49060257d6c2bcb13c284b910f`.
`vendor/CSEM_REV` is authoritative. Upstream targets Lean 4.35.0; the root
project remains on its approved **Lean 4.34.0**. No elaboration option or
budget was added or raised. The selected source and all proof groups compile
on 4.34; the evaluation is `results/csem/repin-2026-10-27.md`.

## Generation and layout

`scripts/csem_import.sh --check UPSTREAM LEMLEAN` compares the selected
pristine import with our source changes. `lean_frontend/generated/` is
produced by `scripts/csem_regen.sh`: Lem translates the `LEM_SRC_LEAN` model
list and the manifest copies the handwritten modules. All **234** outputs
are regenerated, never edited. The generator is the exact matching lem pin,
`lean-backend-v0.1.0-alpha.1-78-ge6d8db5`, built with OCaml 5.4.0. The
regeneration check requires that matching generator in the full fork gate.

Toolchains now default to a pin-addressed directory in **this worktree**,
`build/csem-<cerberus-short>-<lem-short>/`, rather than a shared mutable opam
switch. The lead-fetched local snapshots supplied the current evaluation;
no worker network access or toolchain migration was needed.

The root `lakefile.toml` builds LemLib, generated semantics, tests, the C
parser, `csem_run` and `TccCSem`. The imported frontend lakefile/toolchain
are upstream reference material. `Main.lean` is imported as `CerbDriver.lean`
with `main` renamed `cerbMain`; `CSemRun.lean` wraps it for the OCaml-Cabs
oracle bridge. This avoids the root project's existing `Main` module.

## Original F0 port patches absorbed upstream

The original import was Cerberus `e7e9bb4` / lem `2d3a492` (Lean 4.32.2).
Its three port groups are now supplied by upstream and work on 4.34:

| group | current implementation |
|---|---|
| Utils / Defacto measure proofs | keep the Boolean guard intact for `split`, and give the `of_decide_eq_true` bridge an explicit proposition; the local `ite_congr` replacements are retired |
| `CerbMemAccessFacts.loadM_erasure` | upstream includes the contradiction simplifications (`not_true_eq_false`, `Bool.false_eq_true`) and the new refusal arm; retain our existing implementation-reader parameters |
| `LemLibPmapLaws.toList_bal` | upstream zeta-reduces the height lets with `dsimp only` before splitting; retire our explicit height-guard/match proof |

The statements are retained. Our configuration/digest reader parameters in
other proofs and tests remain explicit; no proof premise was weakened.

## Fork changes retained

| source | local purpose |
|---|---|
| `CerberusFresh.lean`, `lean_digest.lem`, affected Lem sources/proofs/tests | total MD5 and an explicit per-TU `Digest` reader; no native MD5 object, mutable digest global or IO forcing barrier in the production pipeline |
| `CerbImplCfg.lean`, `lean_impl.lem`, implementation/decoder/parser/memory/driver readers | explicit RV64I/lp64 configuration, C11 target choices, errno, argv and HTIF exit conventions; the documented float/bitfield/enum gaps remain obligations |
| `CerbFS.lean`, `fs.lem`, `driver.lem`, Core/libc runtime proxies | the image filesystem, descriptor-based fstat, path resolution, explicit embedded files and structural state ordering; operations unavailable in the image still refuse, now with upstream stop metadata |
| `cparser/` | our Lean C grammar and Cabs comparison interface; unchanged by this repin |
| `CerbDriver.lean`, `CSemRun.lean`, manifests, Makefile and root Lake | repository layout and explicit-reader integration, preserving upstream's new calling-convention reader and quotation definitions |
| `scripts/csem_{env,libc_prep,run}.sh` | pin-local generator/oracle paths and a locked libc cache keyed by oracle, source, runtime identity and content, with artifact hashes; the upstream exact location guard is preserved |
| `scripts/check_{exec_purity,sorry_token,theorem_axioms}.sh` and allowlists | root-Lake paths, vendored LemLib, retired-digest seam population; retain upstream's strengthened exact axiom allowlists |

## Total stop encoding

Upstream's `CerbStop.render` calls generic `Json.compress`. The first
candidate's dependency walk found one additional kernel partial and two
additional runtime partials (`Json.compress.go`, `JsonNumber.countDigits.loop`).

Our `CerbStop.Encoding` describes the fixed strings/nulls/arrays protocol
once. The JSON-value instance preserves `toJson`; the wire-string instance
uses Lean's existing **total** `Json.renderString` and array joining. It has
no recursive serializer, new fuel, opaque declaration or native replacement.
`Unit.StopTest` checks byte equality against the upstream generic printer on
its seven samples, including Unicode and every control character. The fresh
cone returns to the baseline 43 kernel / 100 runtime partials. The parser
and float/native seams already recorded in F1 remain; this is no purity
certificate for the whole semantics.

## Gate scope

`scripts/csem_gates.sh` keeps the original root-integrated fork gates and
19 executable tests, adds the structured-stop executable and its 25 codec /
capture tests, and builds/runs the upstream quotation local controls. The
root `CSemConeReport` target makes the dependency census buildable under the
same shared lock and 12 GB cap.

As at F0, upstream standalone lanes requiring its own Lake package and full
OCaml tree are not claimed as root-package gates: fuel-forms/failure-reach,
no-fuel-numerals, lakefile-roots, pin-sites, fork-drift, fixture-freeze, the
non-exec differential lanes, and now the full quotation round-trip/plugin
lane. Their imported scripts remain provenance/reference material. The
new quotation local controls do not claim that full lane passed. The root
exec lane, regeneration, purity/totality plants, axiom census and focused
parser/configuration/filesystem regressions are recorded individually in
the repin report.
