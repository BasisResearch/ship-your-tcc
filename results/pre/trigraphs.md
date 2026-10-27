# C11 trigraph repair — 2026-10-27

The patch, executable model and generated C11 configuration now translate all
nine trigraphs. Validation is complete; the worktree diff needs the lead's
commit because Git metadata is read-only in the worker sandbox.

Physical source is translated during buffer refill, before every lexer fast
path, splicing, comments and literals. Each file holds at most two trailing
question marks across raw reads; EOF flushes them, and a short read containing
only held marks does not end the file. The libtcc physical `<string>` entry
runs the same transformation. C11 / GNU11 enable it; assembly and synthetic
macro/paste/`_Pragma` buffers do not reenter phase 1. The model independently
implements the same refill and translation functions.

`-E` also preserves its source encoding: adjacent question-mark tokens get a
space, and a trigraph spelling created inside a literal by splicing or
stringification escapes its second question mark. This affects emitted text,
not the preprocessing token stream delivered to the compiler. The active-macro
`pragma_operator` function and all existing tccgen fixes are byte-identical to
the previous patch's applied source.

CSem's phase-1 meaning remains C11. `cfgTcc.trigraphs` is now true through
`scripts/gen_pre_cfg.py`; the generated config was regenerated and checked.
The spec's new guards exercise mapping, overlap, splicing before comments,
stringification, strings, physical lines and header translation. Their expected
phase-4 text is tokenized without repeating phase 1. Neither Supported/Fits nor
any frozen sum compile-cut artifact changed. No theorem, premise or elaboration
budget was added.

## Validation

- `scripts/test_tcc_c11.py`: **48 positive cases and 6 negative cases pass**.
  Each positive case runs under patched host tcc and GCC, survives a tcc `-E`
  round trip, cross-compiles with the RV64I tcc and passes the ISA checker.
  This includes all prior C11 / active-macro `_Pragma` regressions, all nine
  sequences at both 8192-byte splits, comment/splice boundaries, overlap,
  partial EOF sequences, quoted header names and macro-produced question marks.
  Additional checks pass for forced short stdin reads, libtcc string input and
  macro-produced question-mark token boundaries in `-E` output.
- `scripts/model_diff.py --no-report --json-out results/pre/trigraphs-arenas.json
  --require-success 28 -j 2` on the trigraph repro, the 24 generated boundary/EOF
  programs, two `_Pragma` regressions and the final Sail probe:
  **28/28 independent arenas identical**, zero refusals, mismatches, model
  errors, skips or harness errors. Every arena header and byte is compared;
  no reference bytes enter the model. The fingerprint uses checked regular-file
  objcopy and the ordered embedded filesystem. Exact command, per-case outcomes,
  source hashes, runtime key and model binary hash are in the JSON.
- Final full-compiler **Sail probe passes, exit 0**. It removes stdio/printf
  from the main repro to keep the probe small, retains the trigraph header
  include, and prepends 8191 spaces so the opening trigraph crosses a refill.
  Sail reaches `tcc_baremetal_run` at **3,390,213**, exactly the fast reference's
  compile cut, enters the program at 3,390,238, and enters `_exit` at 3,390,950.
  The fast reference exits after 3,390,955 instructions. Sail checks execution,
  status and trace positions; this is not a claim of full Sail arena dumping.
- **Capped Lean build passes** for `CSemPre pre_spec tcc_model`, including all
  spec `#guard`s. Local systemd access was denied; the lead verified the frozen
  inputs and ran the shared-lock / `MemoryMax=12G` command. Final matching
  request/result ID:
  `aa45b9d2999b09161818ec9f1520a7d45368160e1e9f317b67e763f3134e7378`.
  Memory was checked before builds (84 GB available at the initial attempt).
- **`scripts/check.sh --static-only` passes**: ISA, forbidden tokens, generator
  drift, CSem text gates, discipline and abstraction gate. The static run notes
  that clightgen is absent, so it does not rederive the existing Clight source.
  `vendor/tinycc` has no diff. Git's whitespace checker on the source files is
  clean; checking the patch artifact itself reports two legitimate blank
  context lines in newly added unified-diff hunks.

## Pins and retained evidence

- Worktree base: `56372715f9a5b274d218df1eea1ccd768b9c7590`.
- Pristine TinyCC: `43c7708b85681a2fd4451c8a541af4494a8919b2`.
- CSem / Lem: `e7e9bb427a514b6053865c303c4af512907da75b` /
  `2d3a492758cb23dc4e417f2961983d25b36ce130`, unchanged.
- Lean: `leanprover/lean4:v4.34.0`.
- Host GCC: Ubuntu `15.2.0-16ubuntu1`; cross toolchain: xPack RISC-V GCC
  `15.2.0-1`, newlib `4.5.0`, RV64I / LP64.
- Patch 0003 SHA256:
  `c04d8ea9ea276ccd9e9fa48ead302a045ce2d3a1eaa9335c458dd1ee3d887f75`.
- Model binary SHA256:
  `7e3a9fbff199e0815d24693a26937f451f63a01e307ca5618163e724c372d7bc`.
- Final runtime fingerprint:
  `00bc124b91e20ea1d0d94f0f9112840dd0316a07b80775c0c1ee77765a2c236d`.

`trigraphs-build.json` retains the complete frozen input hash map, matching
capped build result, executable/log/ELF hashes and exact Sail invocation.
`trigraphs-arenas.json` retains all 28 arena results and runtime/source pins.
Local logs are `build/test-c11.log`, `build/test-c11/commands.json`,
`build/trigraph-model-diff.log`, `build/static-gate.log`, the matching
`build/worker-build-*.log`, and `build/trigraph-sail-final.{raw,trace,pcs}`.
The generated boundary programs and probe are retained under build/; their
construction is in the test script and build evidence.

## Failed approaches and remaining work

The initial direct translation passed execution but failed an `-E` round trip
(`trigraphs 2`) because stringification can create a new trigraph spelling.
Escaping the emitted source and preserving question-mark token boundaries fixes
it. The first capped build also caught two fixture errors: an old disabled-mode
expectation and a new include filesystem missing tccdefs.h. Both are corrected;
subsequent capped builds pass. These are recorded in `tasks/DEAD-ENDS.md`.

A separate, pre-existing physical-line bug remains: with `#line 300`, an
`__LINE__` token immediately touching a literal backslash/newline expands to 301
in tcc; GCC and CSem give 300. This reproduces without trigraphs. The host repro
checks the splice after the semicolon, while the spec guard retains correct
physical-line semantics. This task does not claim to close that separate bug or
the open whole-compiler / preprocessing theorems.

Systemd user-bus denial was handled by the prescribed frozen build handoff,
with matching successful results. `git add` then failed creating
`.git/worktrees/sol-c11-trigraph/index.lock` (read-only filesystem). No alternate
Git store, uncapped build, push, main merge or external message was attempted.
The lead must review and commit the diff.

Lead review: strict pre_diff also exposes the known distinction between raw
phase-4 string spelling and `-E` source encoding. Stringification of question
marks joined by phase-2 splicing yields a raw trigraph spelling. Patched tcc
escapes a question mark when emitting source. Correction from the later exact
token review: GCC sometimes emits an unprotected trigraph spelling; the old
comparator also incorrectly ran phase 1 again on that emitted text. These are
distinct printer/observer effects (results/pre/token-stream.md, once integrated).
Keep the exact token-stream theorem: do not erase literal escapes to force
equality. The internal-token observation path records raw spelling directly.
The existing local-header fixture discovery also only scans raw #include,
so trigraph-created include directives need explicit fixture handling.
Final Sail raw output, trace and PC list are retained beside this report.

Lead integration: the full `scripts/check.sh --csem` gate passed after
rebasing the recovered change: 633 build targets, all unit/axiom checks,
230 generated files checked and 113 baseline oracle outcomes unchanged.
The lead then captured and compared the complete 4 MiB Sail arena, including
entry/code metadata; see results/model/trigraph-sail-arena.md. The earlier
trace-only claim above describes the worker's original evidence.
