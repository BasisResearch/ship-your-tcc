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
