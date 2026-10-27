# Approaches not to repeat

- Inherited `.lake/build` trees use hard links. Lake updates traces in place
  but replaces objects, allowing another checkout's trace to certify a stale
  object (observed: new `Symbol` with old `CerberusFresh`). Detach artifacts
  with `scripts/isolate_lake_cache.py` under the shared build lock. Never seed
  worktree caches with `cp -al`; use independent files or reflinks.

- F3: feeding reference-run long-double stack bytes to the compiler model
  invalidates independent arena equality. Keep such replay diagnostic only;
  fix and model the conversion independently.
- F3: matching compilation refusals are classifications, not successful
  supported-program compilation or evidence for the headline theorem.
- F1 recovery: several old worktrees have stale generated copies. Edit the
  source and regenerate through scripts/csem_regen.sh; never merge stale
  generated files merely because a cached binary runs.
- Decimal binary128 (slot 2, 2026-10-21): simply wrapping pinned newlib
  `_strtodg_l` with FPI `{113,-16494,16271,1,0}` is insufficient. The exact
  decimal for `2 - 2^-113 - 2^-180` returns `3f8fffffffffffff ffffffffffffffff`
  instead of `3fffffffffffffff ffffffffffffffff` (wrong exponent). Keep
  `ld_boundary.c`'s carry-below vector. Do not resend the direct-nearest wrapper.
- Decimal binary128: 114-bit truncation plus guard/sticky passes the initial
  37-vector set but fails 3/156 deterministic stress cases. `_strtodg_l`
  returns short significands for long decimal expansions just below dyadic
  values (e.g. 29 * 2^-15906). Those cases are now committed regressions.
  Reuse the integer mprec helpers directly, with exact quotient/remainder
  rounding; do not resend the higher-precision gdtoa wrapper.
- This worker sandbox cannot connect `systemd-run --user --scope` to the
  user bus (`Operation not permitted`), including after the shared Lake
  lock is acquired. No uncapped Lean build was substituted. Validation
  needs a worker environment with access to the user bus.
- This worker sandbox exposes the worktree's Git metadata read-only.
  `git add` fails creating `.git/worktrees/agent-a05464165befb7074/index.lock`;
  commits require a worker environment with writable Git metadata. No
  alternate index/object-store workaround was attempted.

- F3 reference limits: the 156-vector binary128 stress compilation needs about
  574 million steps. A 400-million-step emulator stop was cached as an exit.
  Limits/traps must be harness errors and old such cache entries rechecked;
  successful compilation refusals and arena equality remain separate.
- Worker launcher: bash resumes a running script by file offset after its
  child exits. Editing that file during the wait corrupted both old launches
  after their workers completed. Parse the whole body in a function or feed
  a snapshot via stdin; retain the parent shell in this execution environment.
- F1 token dispatch: adding ELSE to the handwritten equality list alone misses
  DEFAULT, STATIC, STATIC_ASSERT and ASM too. Derive exact token equality and
  share it across accept/expectT/at?/direct lookahead. The token fix gives exact
  Cabs equality on all 614 Menhir-accepted inputs, including 200 Csmith.
- F1 GNU scope: Menhir rejects approved forms (cleanup/other attributes, GNU
  aliases, empty initializers and K&R declaration lists). BOTH_REJECT does not
  validate Supported. Do not delete attributes/local-label declarations or
  classify their meaning away. Cabs has no local-label-declaration node and
  Lem has no cleanup lowering; extensions need representation/desugaring too.
- F1 cleanup expansion: the hardened run makes the former interruption
  reproducible as a 120-second LEAN_TIMEOUT on tests2/101_cleanup.c (15 MB
  preprocessing expansion). Lexer step closures retain the token prefix during
  array push. Moving the accumulator outside step closures fixed it; capped
  incremental build and full rerun passed (614 AGREE, 41 BOTH_REJECT, 9 CPP_FAIL).
  Merely increasing the
  timeout is not a grammar fix. Making the ident let lazy was considered but
  rejected: generated C already sinks identLen into its branch.
- F1 worker sandbox: systemd user bus returns Operation not permitted. The lead
  successfully built the first stable request under the shared lock/12 GB cap.
  Keep the build request protocol; never fall back to an uncapped build.

- F2 `_Pragma`: collecting all pragma markers and applying them only after a
  text block has expanded gives stale macro values after pop_macro. Thread the
  macro table/stack through rescanning and argument expansion; markers alone
  cannot represent these effects. Operand macros must expand before checking
  for one string literal. Regression: c/tests/c11/pragma.c's RESTORE macro.

- Lead gate launches must set the common absolute SYT_LAKE_LOCK. check.sh's
  old relative default overrode csem_build.sh's shared default, allowing two
  capped builds in separate worktrees to overlap. Observed during ABI/FS
  integration on Oct 27; no OOM, and overlap ended before further builds.
  check.sh and the Certora helper now derive the default from Git's common
  directory, so worktree gates serialize even without the environment.

- F3 arena collection must stop before executing tcc_baremetal_enter. Running
  the generated program first misclassified 198 cgen and 50 Csmith compilations
  as harness limits. The explicit handoff collector recovers all 248. An arena
  file left by a limited/trapping process is still insufficient: require a
  consistent completion record and validate serialized length, including caches.
- Repin workers cannot resolve GitHub or open the systemd bus in their sandbox.
  Lead-fetched immutable upstream/Lem checkouts plus frozen-input build requests
  resolve this; do not resend a task that ends at those infrastructure checks.

- C11 trigraph worker (2026-10-27): translating physical input alone makes
  macro/stringification-produced `??=` in `tcc -E` output translate on a later
  compilation. Preserve the source encoding too: escape the second question
  mark of a string/character trigraph spelling and separate adjacent `?`
  tokens. The direct run initially printed `trigraphs 0`, while its -E round
  trip printed `trigraphs 2`; both now print zero. Do not route synthetic
  paste or `_Pragma` buffers through phase 1.
- C11 trigraph build fixtures: the first capped build compiled both drivers
  but rejected an old test expecting trigraphs disabled and a new include
  test with no `tccdefs.h` in its filesystem. Update the old expectation and
  extend the existing test filesystem; the matching capped rebuild passes.
- Existing tcc physical-line lookahead defect: `#line 300\nint x = __LINE__`
  immediately followed by backslash/newline and `;` initializes x to 301,
  whereas GCC and CSem say 300. A literal backslash reproduces it without
  trigraphs. The trigraph regression checks a splice after the semicolon;
  the CSem regression retains the exact physical line semantics. This is
  separate unfinished work, not a reason to change CSem or Supported.
- Sail command detail: `lean_riscv_emulator --max-steps N` only enforces N
  in its traced loop. Include `--trace-pcs` (even an empty file) for a bounded
  probe. The initial untraced diagnostic completed successfully, but the final
  accepted probe used `--trace-pcs` and a wall timeout on the final image.
