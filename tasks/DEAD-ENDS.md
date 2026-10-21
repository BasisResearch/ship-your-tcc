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
