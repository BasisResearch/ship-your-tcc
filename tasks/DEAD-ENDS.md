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

- C11 line-splice worker (2026-10-27): saving the token start alone does
  not fix macro arguments: the argument reader consumes later physical
  lines before substitution. Keep token-line metadata separate from the
  physical lexer cursor and carry it through argument caches/rescanning.
  Pasted tokens (including placemarkers) take the invocation line; copying
  the first raw operand's line gave 301 for `C(\n__LI,\nNE__)` at line 300,
  whereas GCC gives 300. The focused generator now covers this distinction.
- A `_Pragma("pack(...) ")` regression placed inside an initializer is
  rejected by GCC. The valid fixture puts it at a declaration boundary.
  The first build request completed its Lean jobs but failed the lead's
  post-build hash check because that fixture generator changed. Submit only
  final frozen inputs and wait for the matching result. Do not reuse that
  failed request as successful build evidence.
- `c/tests/pre/line.c` is a preprocessing fixture with no main. Including it
  in a successful-arena quota gives a matching linker refusal, not an arena.
  Keep it in preprocessing comparisons; use executable fixtures for the
  independent model arena requirement.
- The line regression spec comparison must run after runtime/cross rebuilds
  finish: a concurrent Makefile-triggered cross rebuild briefly removes
  build/cross/riscv64-tcc and causes harness errors. The sequential rerun is
  authoritative. It finds seven independent CSem.Pre macro-location
  discrepancies: Expand.lean rewrites every argument and invocation to the
  closing parenthesis line. Preserve the strict failures and C11 spec;
  neither normalize the numbers nor change Supported to hide them.
- Adding line metadata exposes an old raw-word assumption in GNU variadic
  comma deletion (enabled by tcc even under -std=c11): payload 44 compares
  equal to the comma token. Scanning backward through words corrupts the
  token stream for an empty argument at #line 44. The repair walks complete
  tokens with TOK_GET to find the comma. `c/tests/c11/line_varargs.c` covers
  this collision, a present argument and a spliced __LINE__ argument.
- Trace-mode Sail emits program bytes only on stdout. A no-output probe
  correctly has empty stdout; an evidence-validator draft expecting the
  untraced emulator's SUCCESS chatter failed. Check verified lead exit,
  empty console, no fuel exhaustion and the expected trace cuts instead.
- CSem repin, 2026-10-27: sandbox DNS/systemd/Git restrictions are serviced
  by the lead's frozen build/toolchain/commit handoff; they are not a reason
  to stop this evaluation. The fetched current snapshots and isolated
  generator build worked. Do not resend the old infrastructure-blocker task.
- The selected upstream 4.35 sources compile on approved Lean 4.34; its
  Utils/Defacto, memory-erasure and LemLib map proof repairs absorb our old
  port patches. Do not migrate the root toolchain or restore those duplicate
  proof patches without new evidence.
- Replaying upstream's generic JSON stop renderer adds partial JSON/number
  printing to the semantics cone. Keep the shared total encoding adapter;
  raising budgets or treating an axiom-only census as a purity certificate
  does not solve that regression.
- Pin changes invalidate `--libc` metadata even when an old args file exists:
  runtime directory identity is checked exactly. Keep the content-addressed
  preparation cache; never relax the runtime-location guard.
- The shared main `c/build/base.elf` can change during another lane's work.
  The repin's first FS comparison verified all retained pins and passed eight
  observations; a later rerun correctly stopped on changed base-image bytes.
  Retain the dated valid measurement; do not recertify changed image inputs.
- Current upstream still refuses a valid C11 second-member union initializer
  (`results/csem/repin-2026-10-27/probes/union-second.c`). The old oracle gives
  7 and the new oracle refuses. This is a concrete adoption concern, not
  grounds to narrow Supported or add a refusal disjunct to the theorem.
- CSem union repair, 2026-10-27: carrying member names and pruning paths is
  insufficient if nested lists advance the cursor before returning to their
  parent. The parent then advances again or leaves its brace frame; a union
  in a struct becomes `AilEinvalid`. Return each braced initializer's own
  aggregate position and let its parent advance exactly once.
- CSem initializer paths: derived structural equality compares identifier
  source locations as well as spellings. It disagrees with the initializer
  map comparator, loses nested selections and deletes same-member siblings.
  Use `setElemCompare` for path component identity; do not use `x = y` here.
- The first fresh union Sail probes hit a 240-second wall timeout before a
  traced exit. They are infrastructure limits, not semantic observations;
  retain them separately and use the longer bounded runs for acceptance.
- Patched OCaml oracle: overlaying all local frontend files onto upstream
  fails Lem's OCaml check on the unrelated Lean-only `fs_console_fd` seam
  (`driver.lem:360`). Overlay the identical shared semantic repair
  `desugaring_init.lem` only; preserve pristine sources for unrelated seams.
  The content-addressed helper records that exact input, without modifying
  the pristine oracle or pretending its refusal is repaired semantics.
- The inherited `ctl_in_struct_positional.c` supplies excess positional
  initializers to a scalar-first union. GCC warns and CSem accepts it, but
  patched tcc correctly rejects it. Keep its legacy engine comparison; do not
  include it in a conforming C11 Sail batch or call that rejection a tcc bug.
- `csem_programs.sh` merges stderr/stdout and can report textual DIFFER when a
  diagnostic interrupts a verdict. Separate captures of the original C11
  union program are byte-identical (48 unspecified-conditional executions).
- Both combined Sail batches stopped at the VALID nested positional
  continuation at line 167, before the excess-initializer control. The first
  diagnosis blaming that control was wrong. The minimal non-union struct/array
  program passes strict GCC and both CSem engines but is refused by patched
  tcc/Sail. Keep the full failing valid case; do not hide it behind a reduced
  successful runtime batch. Lead queued a separate compiler/exact-model repair.
- Aggregate-copy override: do not infer support from `.s=p,.s.a=3` returning
  zero on GCC and CSem. Clang returns 7, while the no-override first-member
  `.s=p` still fails in pristine and patched CSem. Keep it an independently
  recorded aggregate-expression gap requiring standards review.
- This union continuation's `git add` also hit read-only worktree Git metadata
  (`.git/worktrees/sol-csem-repin/index.lock`). Use the unique frozen commit
  request for lead review; no alternate index/object-store workaround.
- Nested-designator continuation (2026-10-27): saving only the outer field,
  or only a flattened byte offset, loses which array/struct/union siblings
  remain. Keep a named path scoped to the current explicit brace list;
  anonymous members need their containing frames too. Brace-elided recursive
  calls must return a subsequent designator to that explicit list.
- Continuation controls: `-std=c11` in tcc retains its inherited GNU optional
  `=` syntax, so a missing-equals control is accepted by the existing extension
  policy. Use a missing-value control for the required invalid-input matrix;
  no diagnostic was suppressed or policy changed. A further valid trailing
  unnamed-bitfield probe initially refused: advance past padding before
  deciding whether an unbraced aggregate is exhausted.
- Continuation harness setup: `model_diff.py all` fails early on absent
  gitignored corpora in a fresh worktree. Generate the local corpus and name
  populated groups explicitly; this setup error is not a compiler refusal.
  Direct CSem runs require `--cabs-json` and `LEAN_ABORT_ON_PANIC=1`; an initial
  wrong export switch and then the missing flag produced harness errors,
  never semantic agreements. The corrected separate-stream captures retain
  anonymous-member and unnamed-bitfield CSem gaps independently of GCC.
