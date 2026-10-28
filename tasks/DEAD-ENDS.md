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

- F2 raw-token worker (2026-10-27): sandbox systemd access fails with
  `Failed to connect to user scope bus ... Operation not permitted`. Keep
  the shared lock and 12 GB cap: frozen `build/worker-build.request` /
  matching result handoff works. Requests 1–3 failed on Lean record layout
  and a Nat/UInt32 driver exit code; their retained logs are not successful
  validation. Later requests supersede them, with source hashes checked.
- Do not observe phase 4 by decoding C literal escapes in `-E` text. That
  conflates distinct spellings such as `"A"`, `"\x41"`, and `"\101"`.
  Moreover, applying phases 1–3 to printer output repeats trigraph
  translation on literals created after phase 1. Use direct internal tokens;
  auxiliary printed-output lexing must use phase 3 alone. Classify printer
  question-mark protection and unprotected trigraph spellings separately.
- A raw pragma recorder that expands every payload changes compiler
  behavior: ignored pragmas must not advance `__COUNTER__`. Retain the
  compiler/spec discrepancy; expand only where the existing effect parser
  does. Do not change Supported or silently change the spec to obtain a pass.
- Appending `_Pragma` observations immediately during eager macro expansion
  puts them before preceding replacement tokens. Carry observations through
  macro token strings to their stream positions. Cached argument reuse can
  duplicate that metadata although the effect ran once; preserve event
  identities rather than executing or reporting the same event twice.
- Deferred observation metadata is zero width for subsequent stringification
  and token pasting. Treating it as an ordinary PPSTR produced `" word"`
  instead of `"word"` and lost a pragma boundary during `##`. Lift events
  before the transformed token and remove newly exposed edge spaces; the
  new focused fixtures compare both observers with plain compiler runs.
  Whole-run observation noninterference remains a typed Statement, not a
  theorem justified by these finite tests.
- The host observer cannot compile the bare-metal newlib binary128 literal
  adapter (`reent.h` is unavailable to host GCC). Its generator retains the
  patched image's predefined type macros directly from tccpp.c while using
  the host cross-target configuration for runtime support. No phase-7 float
  conversion is part of the raw stream; this is diagnostic host execution,
  not evidence that the Sail binary computes the new interface.

- F2 pragma spec repair (2026-10-27): bracket markers cannot participate in
  macro substitution as ordinary tokens. An expanded argument needs one
  opaque event identity across reuse; #/## must lift that event out before
  transforming semantic tokens. Two-argument paste exposed another distinct
  issue: tcc lifts cached events in reverse argument order. Preserve that
  configured order without rerunning the already executed pragma effects.
- The new nested `_Pragma` operand probes are not successful raw observations:
  both ordinary implementations accept them, but both decorated observers
  refuse them. `_Pragma("unknown __COUNTER__") __COUNTER__` also changes
  behavior under observation: final counter 1 observed versus 2 plain.
  These three independent observer noninterference failures are retained in
  `results/pre/pragma-observer-residuals.json`; do not change the spec or
  observers to count them as strict successes. Their independent ISO spec
  regression remains accepted, and the whole-run Statement remains open.
- A single pragma expansion switch is insufficient. Ignored directive
  operands must stay raw, while pack and macro-stack operands do expand.
  Expanding push_macro(NAME)/pop_macro(NAME) without then executing the
  resulting phase-4 action still leaves the macro unrestored. Both directive
  and operator forms have independent exact-token regressions.
- Pragma repair build attempts 1 and 2 are failed receipts, not validation:
  multiline record-update fields must align after `with`, and inside
  `ExpCtx.ofState` the unqualified validator name resolves to the record
  projection. Qualify `CSem.Pre.checkPragma`. No budget changes were used.
- A comparison started while its pinned test file was still being edited
  correctly failed `input changed during run`; discard that attempt. Freeze
  fixture/test inputs before comparing. Corpus regeneration also requires
  `build/compcert-tests`; copy the existing local source into this worktree
  before invoking the existing collector, rather than fetching new sources.
- This pragma worker's final `git add ... && git commit ...` is blocked by
  read-only Git metadata: it cannot create
  `.git/worktrees/sol-c11-trigraph/index.lock`. Final source/evidence is
  validated by frozen request/result `f2-pragmas-5` and left as a reviewable
  worktree diff for the lead to commit, following MANAGER.md. No alternate
  index/object store or permission workaround was attempted. Do not resend
  semantic repair work merely to resolve this infrastructure limitation.

- Observer/-E integration (2026-10-27): a per-operator output sink starts too
  late for an inner pragma executed during function-macro argument prescan.
  Replacing next() with next_nomacro() while draining output also drops the
  macro/unget stacks. Keep both historical failures: opaque, identified output
  events must survive prescan, substitution and rescanning instead.
- Zero-width output is also required by function-macro call lookahead. Merely
  skipping metadata before '(' loses it if that peek exhausts a rescan frame.
  Retain metadata from exhausted frames in the peek buffer, then preserve it
  whether or not the macro is invoked. Lift only pre-parenthesis metadata;
  lifting every argument event changes its position in ordinary replacement
  lists. The new rescan-call/frame fixtures are strict positives.
- The new combined printer fixture exposed independent GCC preprocessing
  choices: it retains _Pragma inside this expanded stringification, duplicates
  the repeated pragma, and leaves the function macro across that pragma as a
  function call. Keep these auxiliary differences; GCC runtime checking uses
  an ordinary identity function for that retained call. Neither raw equality
  nor the spec uses GCC printer bytes or removes those rows.
- Combined integration build attempts 3/4 used unqualified then wrongly Tok-
  qualified TOK_PPSTR in Types; the constants live under C. Attempt 5 exposed
  two mutable-variable shadowing errors. Attempt 6 builds the final model,
  spec, token driver and audit under the shared lock/12 GB cap. Preserve those
  failed receipts/logs; they do not certify the repaired sources.

2026-10-27 lead: a default `lake build`/old full gate does not refresh the
non-default `tcc_model`, `tcc_pre_tokens`, or `pre_spec` executables after a
rebase. The first combined 1,812-case run used the pre-designator model binary
and reported 1,339 matches plus an incomplete int-array/string disagreement.
Retain that run as stale-binary diagnostic evidence; do not patch semantics
from it or claim source-level validation. The gate now explicitly builds the
three executables and PreAudit, then audits the preprocessing definitions.
Repeat comparisons after the capped explicit build; source hashes alone do
not attest which source produced an existing executable.
The explicit capped rebuild removed both minimal incomplete-array
mismatches (implicit int and explicit int); the valid char/fixed-array controls
still match. No compiler/model semantic edit was needed. Full comparisons
are repeated and retained separately from the stale-binary history.


- IEEE recovery (2026-10-27): the draft test runner did not compile on 4.34
  because six mixed Nat/UInt literal additions lacked operand types. Correct
  the runner, never treat its historical missing report as test evidence.
- Recovered `gen_vectors printf` ignored quadmath failures on a format with
  trailing literal text and reused stale output; its 4,000-byte buffer also
  silently truncated large binary128 fixed output. Render a single quadmath
  conversion, append the literal tail separately, check the return and use
  a sufficient buffer. The two failed host runs are retained as harness errors.
- Canonicalizing every conversion NaN is wrong for RV64 libgcc extensions.
  Raw volatile-operand Sail probes preserve shifted payload/sign on widening
  and canonicalize narrowing. Native Float.toBits cannot validate these payloads.
  GCC also folds `NaN * 1` to a copy; compare volatile-operand helper calls
  separately from the compiler's observed expression result.
- Newlib sqrtl differs on nine finite binary128 results in the 32-input Sail
  probe. Retain the failed full comparison and same-input host sqrtq replay;
  do not filter those rows into a successful library claim. Regenerating host
  vectors with the same PRNG seed does not replay identical rows: the host and
  Sail generators consume random conversion inputs in different orders.
- The full recovered tcc/Sail printf probe exceeded 900 seconds with no traced
  program exit. The first focused batch used nonembedded include paths and
  was rejected. Its inline replacement completed 31,317,491 steps but stopped
  honestly at `rounding=1`; formats/bytes passed. Keep that literal-accuracy
  discrepancy and run later probes separately, without weakening CSem.
- Frozen build handoff `command` is not executed by the ordinary worker build
  service. Empty targets triggered a default build, not visitor generation.
  Use the `IEEEVisitor` Lake library target and install its marked output with
  `csem/tools/gen_core_quote_visitor.py`. Inventory is freshly 643 constructors,
  not the old 638. Superseded source manifests are not successful build evidence.
- The old combined repin/config/FS recheck correctly rejected the changed main
  `c/build/base.elf` hash. Do not waive that check or recycle its historical
  Sail records as fresh image evidence. This IEEE task uses independent runs.
- The passing final continuation's `printf=0` is only the C function's return,
  not printf-output agreement. Its binary128 line loses the final precision
  bits and prints `inf` for `1e4000L`; the independent xgcc/Sail probe reproduces
  that newlib path. Preserve the model/target byte difference and the explicit
  `NewlibIEEE_Statement`; do not round CSem through binary64 to hide it.
- The final same-source continuation makes the remaining printf mismatch
  unambiguous: five binary64 formatting fields agree, while binary128 fixed,
  hexadecimal and wide-exponent fields lose precision/range. Its zero return
  is retained separately from output equality. A minimal literal probe prints
  `3f800000 3f800000` on tcc/Sail versus CSem `3f800001 3f800000`; these two
  observed discrepancies block claiming the corresponding library/literal links.

Direct literal review (2026-10-27): binary128 intermediates still double-round
binary32/64, and fixed-width hexadecimal accumulation loses the sticky tail.
Use every source digit and direct destination rounding. The full 189-case
tcc/Sail probe needs 232,926,522 compile steps; the incomplete stopped run
is retained separately from the smaller native boundary probe. The 789-case
fast stress compile requires 841,087,712 steps, beyond its first 400M limit.
Neither exhausted run counts as a passing observation. Build-service review
notes must be read in full: polling only the first eight receipt lines hid
requested corrections until the lead surfaced them explicitly.
- Anonymous members (2026-10-27): flattening an unnamed declaration into its
  parent's physical member list loses both union storage and initializer
  current-object boundaries. Promoted names must resolve to physical paths;
  changing only offsets or only continuation leaves the other defect.
- Anonymous representation: unspellable synthetic FIELD names (`:anonymous:`)
  execute on both engines but break the existing Core text grammar. Mark the
  container with an internal attribute instead and allocate printable names.
  Include descendants' source names in allocation: a promoted field must not
  capture its own container. Preserve every qualifier along the lookup path.
- Anonymous probes: Cerberus parses standard `offsetof`, not GCC's
  `__builtin_offsetof` spelling. Include `<stddef.h>` and use `offsetof` on
  all engines; the first four parse failures were harness inputs, not layout
  answers. Reserved `__...` source identifiers also cannot establish defined
  strict-C11 coverage; final collision controls use ordinary legal names.
- Anonymous qualifier follow-up: valid `_Generic(&s.a, const int *:1,default:0)`
  (and volatile) fails on patched tcc for an anonymous qualified container,
  in both storage variants. Named-container, direct-const-field and
  const-object controls pass. Retain this separate compiler gap; do not
  change CSem qualifiers, Supported or expected results to match it.
- Anonymous build requests 02 changed while the marker/name collision
  revision was underway; the service correctly rejected source drift.
  Those requests are superseded, not build evidence. Freeze the final source
  manifest and verify its matching receipt before recording success.

- Anonymous layout: repairing physical fields exposes the inherited union
  sizeof/alignof adapter asymmetry during `offsetof`: both adapters receive
  the complete explicit table but consult the unset ambient table in their
  union arms. Four strict-C11 layout probes crash on both engines. Thread
  the explicit table in both handwritten adapters; do not seed mutable Tags
  during translation or drop the offset probes. The isolated OCaml overlay
  must pin the corresponding two-line patch as well as the shared Lem files.
- Printable names are not a source-visibility mechanism. The first repair
  accepted a guessed container in a source expression (GCC diagnosed it;
  both models returned one). Filter public paths, consume initializer paths
  as explicit physical edges, and distinguish generated IR member operations.
  Do not simply filter everything and break internal projection. Final
  member/arrow/designator/offsetof guesses in both storage modes all reject.
- A positive `*_auto.c`/`*_static.c` wildcard also picks up the new guessed-name
  negative fixtures. Enumerate positive sources by the generator's invalid
  classification; preserve the negative diagnostics in their own matrix.
- The union-layout adapter correction needs its existing fuel potential and
  two stability cases to read the explicit table too. Reuse layout_stable_aux;
  do not raise fuel or elaboration bounds to get around the old ambient premise.
- Corpus include materialization must provide the actual `/prog.c` and
  parent-directory fixture aliases, including macro self-inclusion. Missing
  fixtures are harness defects, not evidence of compiler or spec refusals.
- Do not conflate resolved physical provenance with the image's opened-path
  pragma-once cache. Its realpath stub preserves spelling, unlike the host.
  The initial repaired-fixture run exposed this mismatch; retained evidence
  is results/pre/once-cache-2026-10-28/before-once-policy-*.json.

- Allocation recovery: do not import the old draft wholesale. It admits
  unbounded Int addresses, treats none as fabricated OOM, and names absent
  proofs. The accepted recovery checks finite bounds and rejects resolutions
  explicitly; BumpPlacementPre is still a reachable-state obligation.
- Allocation reuse must clear stale region bytes when a dead address is
  reused. The old no-write allocation body assumed monotone bump placement.
- Compare default bump executions under the same implementation reader:
  RV64 malloc alignment is 16, versus the default oracle's 8. Retain the
  ABI-mismatched diagnostic rather than normalizing observed addresses.
- Allocation gate scripts must write fresh observations to build/ (explicit
  --out), not rewrite prior frozen results. The combined check caught this
  evidence-management defect; final semantic sources are unchanged.
- Library identity (2026-10-28): suffix/label agreement is not a connection
  premise for raw semantics. Attach explicit resolved identity at phase 7 and
  preserve it through Cabs/Ail/Core; keep historical JSON/text projection
  separate and preserve every diagnostic coordinate. Core quotation has an
  exhaustive Pos constructor match too. Extend its handwritten serializer and
  regenerate the datatype visitor; treat UInt8 as a scalar or its dependent
  representation creates unbounded instantiated visitor types.
- The image initializer replaces earlier duplicate file contents with later
  entries. Merely prepending raw main bytes leaves a stale self-include;
  remove duplicate main identities before inserting the actual input. Keep
  this distinct from presumed labels and opened-path pragma-once keys.
- A runtime-header fixture must materialize nested `posix/` headers and set
  the configured include directory. The initial stdio refusals were missing
  harness inputs, not library-identity semantic failures.
- Redeclaration/pointer residual: a static prototype, definition, second
  prototype, then initialization of a function pointer produces the same
  `null function pointer` driver error for ordinary `mine` and proxy-named
  `printf`. GCC accepts both. Direct redeclared calls and function pointers
  without the extra redeclarations pass. Retain both residuals independently;
  do not narrow Supported or claim all pointer initialization is broken.
- F3 corpus fixtures (2026-10-28): matching compiler refusals can hide absent
  harness inputs. Embed basename/self and parent-directory aliases consistently
  across F2/F3; 18_include and both 95_bitfields cases then compile. Three full
  native arenas confirm the repair; do not count the old refusals as coverage.
- Do not run overlapping model_diff source sets concurrently in one worktree:
  its source-derived scratch names collide. The stopped overlap is diagnostic
  only; the accepted native and broad runs were serialized.
- A 900-second native wall limit is too short for the 34-million-step bitfield
  captures. Keep timeouts as harness errors and rerun with adequate wall time;
  do not alter Lean budgets or classify a timeout as a compiler refusal.
- The model_diff overlap hazard is now repaired (2026-10-28): invocations in
  one worktree serialize cache/scratch access, and duplicate selected paths
  are rejected. Two simultaneous real comparisons and maintained subprocess
  exclusion/release controls pass. The old stopped overlap remains a failure.
- Ghost Cabs continuation (2026-10-28): the inherited checkpoint was never
  validated. The historical ghost-1 receipt failed, and ghost-2 still failed
  on a nested Option/ParsedDeclarator pattern in typeName. Type the intermediate
  or qualify ParsedDeclarator.abstract; no budget change is appropriate.
  The build service writes its current completion to worker-build.result;
  archive it under the request ID only after checking that ID. Waiting only
  for a uniquely named receipt can miss an already completed failed build.
- Ghost build continuation: requests ghost-3 through ghost-8 exposed inherited
  multiline record layout errors in State/PpDir/PpMacro/Gen8/GhostDriver, then
  a reserved `syntax` field name and missing JSON instances for UInt8 lists.
  Use aligned record fields, a non-keyword witness field, and explicit byte
  naturals at the IO boundary. ghost-10 rejected a Lean namespace-alias draft;
  use the explicit CabsExport namespace. These failed receipts are retained,
  not counted as validation.
- Ghost comparison: cfgTcc's predefined text injects real tccdefs.h C
  declarations, starting with a GNU aligned attribute and macro-generated
  builtin prototypes. Both syntax adapters currently fail before the tested
  declaration. The first 56/56 nonsuccess run is diagnostic only. Shared-token
  fixtures now explicitly omit predefined text; actual raw-source runs keep
  the header and fail strict acceptance. Do not strip that header from the
  ordinary compiler or use fixture successes as ordinary-TU coverage.
- Ghost acceptance: a zero exit status with 56 explicit nonsuccesses hid total
  omission. The strict default now requires named exact controls and a minimum
  count, with explicit --diagnostic for residual exploration. Keep source hashes
  as well as executable hashes; a dirty source revision alone is not a pin.
- Saved ghost replay must bind to the specific captured token string/frame,
  not search by structural equality of token arrays. Identical syntax at two
  source occurrences has different identities. Include the saved EOF sentinel's
  original lookahead identity; otherwise replay invents an extra language EOF
  and changes initializer boundaries.
- Ghost full gate 1 passed its Lean/axiom/purity/test stages but stopped because
  this fresh worktree lacked build/fork-oracle/<content-key>. The exact engine
  db28f95fedcb38cac435c74ab6ff3d264b9d597ef2b6182505feb7a76943e953
  already exists in the shared root cache. Its script-derived content key and
  artifact/overlay manifests verify. Link that immutable engine into the local
  ignored build cache; do not repin or mutate the shared toolchain or rebuild
  a different oracle. The subsequent frozen gate retains this same source pin.

- Combined ghost integration: library identity adds an optional Pos field;
  use named fields, preserving missing origins as explicit gaps. Do not invent
  runtime membership to satisfy constructor arity. An erasure-only invocation
  in a fresh integration worktree also needs normal generated image symbols;
  retain its missing-file failure, then prepare them through the harness.

- CSEM-10 (2026-10-28): redeclarations correctly retain the canonical symbol.
  The first prototype gets Lean id 0; storing it emits zero bytes, and loading
  chooses NULL before function-map lookup. OCaml has already consumed runtime
  ids (`mine` = 483 on identical positioned input, 502 with injected builtins). Reserve zero at both Lean pipeline entries; do not change
  valid redeclarations, Supported, proxy names or null decoding to mask this.
- Redeclaration harness: ordinary OCaml preprocessing injects builtins that
  consume ids and hide the Lean zero-seed defect in exported Cabs. A bare
  `--cpp=cat` then fails on automatic `-I` flags. Retain both attempts; use the
  existing independent raw_frontend_cat.py positioned-input hook and check full
  Cabs/digest equality. Neither harness attempt is semantic agreement evidence.
- Redeclaration controls: pinned OCaml and Lean both accept the invalid
  `int mine(int); int mine;` function/object conflict; strict GCC and patched
  tcc reject it. Preserve it separately from three correctly rejected constraint
  controls. Historical name-based printf proxying is also still separate from
  the repaired identity-aware raw path. Neither gap narrows Supported/Fits.

- Redeclaration full-gate attempt 1 correctly stopped because this worktree
  lacked the content key for the already-adopted allocation adapter. Reuse the
  reviewed sol-csem-repin cache at key 9e775864dfd595f06f61463e4ff16d6664b5aa7dea800e471cd821462c73602f
  through a local link, after its artifact/overlay manifest verifies; do not
  repin, rewrite or rebuild a shared oracle cache to bypass the check.

- GNU layout review (2026-10-28): encoding aligned aggregate layout by raising
  its first member's declared alignment conflates independently observable
  properties. Use explicit aggregate fields and separate effective packing;
  do not rely on currently missing expression-alignof to justify the shortcut.
  GCC and tcc also differ on explicitly aligned member expression queries;
  retain both observations instead of declaring GCC the target by assumption.
- GNU typedef alignment: successful offsets/sizeof do not validate type
  alignment queries. The RV64 Sail probe has correct 16/1 member offsets but
  tcc reports 4/4 for `_Alignof(Up/Down)`; chained typedefs also report 4.
  Preserve this failure separately from the GNU layout implementation tests.
- Ghost provenance: physical-token byte equality does not establish an include
  history. An unconditional empty SourceSite.includes list cannot be called
  complete for nested/repeated includes; retain actual directive spans or an
  explicit incomplete-provenance gap. Carrier-only Cabs construction is not
  independent parser agreement or a semantic proof for injected asm aliases.

- Ghost origin continuation: deriving physical offsets after lexing fails for
  tokens crossing the 8 KiB refill. Capture a model-owned physical cursor
  before the lexer consumes the token. Identity-buffer verification still
  rejects trigraph/splice transformations; do not declare them mapped.
- Folded adjacent-string initializer replay must bind its exact saved token
  positions (including lookahead), or decoded A1A reappears as invented source
  tokens. Extra tokens are an origin-transport bug, not a normalization.
- Ordinary header construction needed the modeled name/typedef actions for
  __builtin_va_list. A spec keyword classification must not choose whether the
  model declared a name. Do not strip the actual builtin declaration.
- Physical byte equality does not certify include history. Retaining only
  ModelOrigin.includeFiles while setting SourceSite.includes=[] loses nested
  and repeated directive provenance. Capture each directive's actual introducer
  span before preprocessing and retain empty/repeated source openings too.
- GNU/asm generic attribute carriers have an exact shared data contract: GNU
  attr_ns is its located introducer; asm attr_ns is its located keyword and
  attr_id is the located synthetic asm_label. Earlier ns=none/id=keyword
  carriers retain fragments but cannot establish exact parser agreement.
  Neither carrier proves alignment or linkage semantics. The independent
  typedef-alignment compiler discrepancy remains outside this ghost repair.
- Origin builds 1/4/9/10 failed on Lean code/record layout (including reserved
  prefix and multiline record fields after a same-line field). Origin build 2
  exposed the refill/string replay bugs; 3 repaired them. Preserve receipts;
  no failed request or stale executable is passing evidence.
- New repeated-include origin control exposes an actual observer/spec policy
  difference: equal macro redefinition points at the second definition in the
  modeled compiler (define_push installs a new symbol), while independent
  Phase4 keeps the first definition. Keep the full differing Origin records;
  do not choose the spec origin or normalize the histories to manufacture
  equality. Separate repeated-declaration and nested/#line/alias controls
  check include ancestry without concealing this discrepancy.
- Final origin-harness review: selecting model user sources from the spec's
  source-name set can hide an extra model-only header/non-emitting opening or
  prefix tokens. Select all actual physical model openings and exclude only
  the identified first injected tccdefs opening; reject other prefix anchors
  and compare all remaining opening records. Actual-capture perturbations now
  verify unmatched openings/prefix tokens and missing opening graphs fail.
  These harness controls do not count as successful source fixtures.

- 2026-10-28, ghost-origin integration: `TCC_MODEL_REF_MAX_STEPS` is not a
  recognized runner variable; use `MAX_STEPS=600000000` for the known cleanup
  fixture. The first broad run also used different regenerated mutant inputs
  (400/400 hashes differed), so its counts could not be compared to main.
  Preserve that capture; restore exact hash-pinned corpus inputs for a
  regression claim. The corrected run gives 1,345/1,815, no harness failures.
  A first erasure run lacked the source manifest; restoring the verified
  seven fixtures plus manifest allowed the unchanged driver to pass.
- GNU declaration attributes (2026-10-28): strengthening an aggregate's
  first member alignment is not an exact GNU desugaring: expression-alignof
  can distinguish it from aggregate alignment. The final implementation uses
  explicit aggregate_layout fields, independently of member alignments.
  Preserve the GCC/tcc member-expression observations as a separate unresolved
  GNU meaning connection, not a C11 counterexample.
- Header byte pins: c/build/tcc-src/include/tccdefs.h is pristine source; the
  actual image embeds strip_headers.py output in fs/tccinclude. Both produce
  the same semantic token sequence, but GNU token10 is at column40 versus36.
  Verify actual FS bytes against both the stripper and Spec.ImageFS; never
  silently call the source-header pin the image-header pin.
- GNU oracle2: wholesale local Core/translation overlays pulled in the
  Lean-only Mem.convert_fval ABI. Use shared GNU semantic files and explicit
  arity-only patches against pristine unrelated modules; never stub floating
  operations. Oracle3 then exposed the old ocaml_implementation.mli pseudo-tag
  map type. Close the handwritten interface as well as its implementation.
  Oracle4 builds with every patched path pinned; non-concrete arity shims do
  not establish symbolic/CHERI layout semantics.
- GNU syntax transport: Lex.slice takes an end index, not a length. Retaining
  text token spelling with end-start produced empty attribute names outside
  the raw path. GNU attribute names that lex as keywords must also preserve
  their original alias (__const__), rather than keywordString's const. The
  maintained full raw/text Cabs checks and alias control cover both repairs.
- GNU target comparison: the original complete native batch returns13 on
  patched tcc, not0. The exhaustive21-case matrix reports mask28672 (three
  typedef _Alignof checks) versus GCC0. Physical typedef layout is correct;
  _Alignof(type) loses the attribute, including through a typedef chain. Do
  not alter CSem, hide the cases or count the batch as passing agreement.

- GNU combined review (2026-10-28): a passing full gate rebuilt csem_raw but
  not root csem_cparse. The fresh raw semantics all worked, while its text
  counterpart used the stale pre-GNU executable and failed every attribute
  case. Preserve that comparison and binary pin. Rebuild csem_cparse in the
  CSem gate and run the maintained GNU/header checks there; a default library
  build alone does not refresh every executable used by the comparison.

- GNU evidence recovery (2026-10-28): do not infer the execution category from
  a generic native-program label. scripts/run_tcc_sail.sh executes the full
  compiler on Sail, whereas run_xgcc_sail.sh runs a cross-compiled program.
  The GNU tcc matrix has 8,256,889 compile steps; retained metadata and command
  pins settle the classification. The initial lead prose was corrected without
  changing any captured observation or increasing a success count.
- GNU typedef query repair (2026-10-28): transporting AttributeDef only to the
  immediate query loses `typeof(*(Up *)0)`. Preserve alignment in nested CType
  values, including sym_push and the partial reference-copy paths. A CType
  field alone does not transport explicit object expression alignment.
- VT_SYM survives address-taking and member address calculation. Even a live
  symbol lookup wrongly gives `__alignof__(&x)` the object's alignment. Use a
  separate initialized SValue expression carrier; reset it on address-taking,
  indirection, casts, operations, comma expressions and member selection. Keep
  the explicitly aligned-member expression discrepancy as its separate question.
- New CType fields require a C constructor audit: Lean defaults do not initialize
  C scratch values. Zero fresh partial SValues, including scalar structure-return
  temporaries, copy the full type metadata, and reset backend gfunc_sret's new
  field. Existing backend Sym labels already use `{0}`; origtype is a dominating
  full copy. Source inventory and checked LP64 offsets accompany the result.
- Function derivation must clear inherited alias alignment before storing the
  function reference's attributes. Clearing it only afterwards makes
  `_Alignof(typeof(F))` inherit the return type's alignment. Preserve return
  CType alignment separately. The retained intermediate probe prints `1 16 1`
  against GCC's `1 1 1`; the final function-typeof control repairs it.
- GNU function-type alignment itself is an existing target policy: host GCC
  and tcc return 1; RV64 GCC returns 4. Keep raw values separately and compare
  typedef-derived functions with the same engine's ordinary function type.
  The initial 65/66 Sail batch is retained as a failed observation, not renamed
  to a pass. The final 76/76 batch uses explicit derivation controls.
- Typedef build 1 failed on a multiline Lean record field's indentation. Its
  matching failed receipt and log remain; do not raise budgets. Initial
  __builtin_offsetof/phase4-only CSem probes were harness diagnostics: use
  standard offsetof and inspect semantic verdicts, not the process exit alone.
- The first typedef full gate stopped on the absent adopted oracle cache key
  9e775864dfd595f06f61463e4ff16d6664b5aa7dea800e471cd821462c73602f. The
  artifact/overlay-verified local link repairs setup; no shared cache was rebuilt
  or modified. Optional native arena captures fail immediately on this worker's
  denied systemd user bus. Preserve them as harness errors; full ordinary Sail
  executions and independent fast arenas are separate successful observations.
- The pinned GNU CSem checkpoint still disagrees on dereference/return typedef
  types, unary/common type preservation and function queries; it also accepts
  three invalid array constraints. Its OCaml parser rejects the GNU spellings.
  These are separate required spec/constraint follow-ups, not permission to
  narrow Supported/Fits, strip attributes or count refusals as successes.

- Array-constraint review (2026-10-28): an old host tcc in an integration
  worktree's build/test-c11 directory accepted an overaligned array. Do not
  infer compiler regression from that unrefreshed executable. The final probe
  uses the exact adopted typedef-worker binary (c5b5ae1a...) and rejects it.
  Both initial and corrected records are retained in the array-constraint
  review. Plain invalid typedefs still expose genuine inherited Cerberus
  constraints; direct function arrays crash, rather than diagnose cleanly.

- Generated negation (2026-10-28): concatenating unary minus with a negative
  floating literal produced a decrement token (`--2.5f`). All118 old cgen
  refusals independently fail GCC syntax checks for this reason. Parenthesize
  the generated operand; do not edit generated outputs or replace historical
  inputs/counts in place. The new1200-input snapshot matches all independent
  arenas; historical1815source inputs and463refusal consoles remain archived.
- Main gate cache setup (2026-10-28): the refreshed main executables passed
  build checks, but the first cgen gate lacked the matching adopted GNU oracle
  link. Verify its complete/artifact/overlay manifests and reuse the immutable
  keyed engine; do not rebuild or mutate shared upstream to fix a local link.
  The first failed receipt and the successful new frozen gate remain retained.
- Raw ghost comparison scope (2026-10-28): raw-spec runs the independent
  grammar on the model's positioned tokens. That tests parser construction,
  not independent raw preprocessing or compilerInjected provenance. Preserve
  those valid component counts and add the actual Raw.checked cfgTcc comparison
  for the binding-origin task; never feed spec Cabs back into model construction.
- Physical origins (2026-10-28): physical-1 failed on two multiline Lean
  record layouts; its receipt verifies unchanged inputs and exit 1. Physical-2
  built but its first real comparison panicked when a synthetic lexer unget
  wrote an absent physical map. Preserve generated buffers as synthetic and
  guard shadow writes; never invent a physical map/default offset for them.
- Raw shadow logs retain unspliced CR and LF separately. An early arbitrary
  completed-log premise would equate that representation directly with the
  spec's CRLF-collapsed phase12 and conceal ordinary inputs. Use an explicit
  independent logical view of the actual shadow log and a typed phase-one
  correspondence. Normalize only physically adjacent CRLF: splice deletion
  must not create a second phase-one transformation. Keep lone CR, CR-at-EOF
  and synthesized terminal LF distinct from actual physical reads.

## Explicit multi-TU corpus invocation review, 2026-10-28

Do not concatenate 104_inline.c and 104+_inline.c or retry them as standalone
successes. Upstream orders the companion before `-run` and keeps separate
translation units. The pinned manifest now passes complete native/fast arena
checks; old standalone refusals remain separate. The first development native
capture overlapped a manifest-reader cleanup, so a second frozen capture is
the adopted evidence. The temporary-path test failure was canonical path
comparison, not compiler behavior. Complete multi-TU ghost provenance remains
an explicit follow-up, not established by arena equality.

## Injected binding and authority continuation, 2026-10-28

- Injected bindings, capacity interruption (2026-10-28): bindings-lean-1/2/3
  are failed build observations, not accepted semantics. Request3's exact
  source hashes were verified after the worker stopped. Earlier Phase7
  constructor-layout errors were repaired; request3 still fails in
  Tcc/CSem/RawFrontEnd.lean, including unresolved BindingResolution. Preserve
  the generated/source edits and receipts, then complete the typed interface
  before testing. No oracle, semantic, whole-C or gate success is claimed.

- Injected-binding continuation: the historical OCaml parser still rejected
  the actual stripped image header's `__attribute` and declarator asm labels.
  Preserve both the wrong-runtime-prefix harness error and the corrected GNU
  rejection. An isolated, pinned carrier overlay now parses the full stream;
  never remove these declarations to manufacture raw-source agreement.
- Injected runtime coverage: memmove/strlen/alloca missing-body errors in both
  engines are not execution agreement. Inventory all loaded targets; 15
  ordinary alias bodies and function-lifetime alloca remain F1/F7 duties.
  Alloca must not join the ordinary external-symbol alias map.
- Consumer JSON tests must expect the new explicit compilerInjected false
  field. Mixing current Lean's symbol-aware proxies with pristine OCaml's old
  spelling proxy also creates an honest difference; select the exact shared
  overlay in the gate, preserving the failed mixed-engine observation.
- Binding inventory build bindings-next-lean-4 failed on `letI` before a
  monadic bind: establish the instance outside the `do` block. Its failed
  matching receipt and exact sources are retained. bindings-next-lean-5
  passes at ordinary budgets; no old executable substitutes for that build.

- Injected model authority: binding-origin-lean-2 failed at the new typed
  transport contract because Lean could not infer quantified PTok/Origin
  types before field projection. Its matching receipt verifies 3,327 inputs;
  retain it as a failed build. Add the explicit types, without raising budgets.
- Full injected-origin comparison first found chained `##` losing its left
  ghost derivation: TOK_PLCHLDR also represents a nonempty paste accumulator.
  Preserve that accumulator's origin on the second paste. The initial direct
  Cabs match retained all21 aliases but was not full recursive-origin equality.
- Independent opening comparison must name the compiler's eager main opening
  and the spec's predefine/header-first schedule explicitly. The first full
  comparison retained equal Cabs/tokens/origins but put main before the implicit
  header in its SourceFile projection. Map designated control IDs to the named
  predefine/header stages and retain every other actual opening. Do not choose
  model sources from the spec's source set or sort away repeated openings.
- A bare second tccdefs include repeats `struct __uint128__` and is rejected
  by the model compiler. Preserve that discovery as a nonsuccess, not an
  opening-authority test success. The valid same-byte repeated-header control
  defines `__uint128__` to a fresh user tag before inclusion; it retains all21
  original alias declarations and the actual same-path header bytes unchanged.
- Injected-origin spoof controls caught the inherited GhostMain --files loader
  prepending overrides before default image entries. The shared initializer's
  later entry replaces earlier contents, so both independent engines silently
  saw the original header. The retained controls-4 report correctly fails its
  expected inventory. Append explicit overrides and check actual retained
  opening bytes against each fixture; do not count that run as spoof coverage.
- Identity-role assertions must use CabsExport.ident's actual {loc,name}
  object; identifiers have no `Identifier` JSON tag. The full model/spec Cabs
  equality already passed; retain the failed role assertions and repair only
  their selector, without projecting away identity metadata.
- Injected-origin evidence packaging: the complete gzip encoding exceeded
  GitHub's ordinary per-file limit. Re-encode the same final/failed/superseded
  captures and source snapshots as xz, verify every member hash, and remove
  only the verified redundant gzip encoding. Do not prune observations or put
  an oversized encoding into commit history to solve an encoding problem.

- GNU alignment semantics build alignsem-lean-1: submitted while Lem regeneration
  was still completing. The generated Translation_aux and lem-sync stamp changed
  after the freeze, so the receipt is failed despite successful Lean compilation.
  Retain it; wait for generator completion before freezing any subsequent request.
  No executable from that request supplies accepted semantic evidence.

- GNU alignment semantics alignsem-lean-2 verifies all3366 frozen inputs but
  fails automatic mutual termination inference for expression/statement/generic
  type reification. Match record constructors directly and expose structural
  parameters; do not raise budgets or restore opaque partial recursion.
  The explicit local locked/capped oracle1 attempt fails on the systemd bus
  before building and has its own verified local-result; it certifies no engine.

- GNU type transport: generator structural analysis also rejects the mutual
  statement-record walker (record patterns become field projections). Reuse
  bottom-up shared GenTyping annotations instead: it already has the selected
  typed children and the statement selector. Generic width/sign categories
  should not reconstruct and then rediscover expression type metadata.

- GNU alignment oracle2: copying the now-shared translation_aux.lem and then
  applying its adopted symbol-proxy hunks from patch0006 duplicates the same
  change. Its receipt verifies all3366 inputs and fails honestly. The shared
  source supplies those exact records/helper now; retain only patch0006's
  unrelated translation.lem hunks, with every overlay still hash-pinned.
- Extra GNU array-alias decay probe: GCC returns8 for typeof(a+0), but current
  patched tcc returns32 for an array typedef aligned32. Full compiler Sail
  reproduces32, cross-GCC Sail8 on identical bytes. Preserve as a separate
  compiler discrepancy; do not copy the leaked array alignment into CSem.

- Oracle3: wholesale local translation_aux.lem also imports Lean-only typed
  IEEE constructors (CfvfromintTypedLoaded). Preserve its failed receipt/key.
  Project only the two exact shared type-query definitions after the adopted
  proxy patch; do not pull in unrelated Core or stub floating operations.
- The first repaired raw run fixes deref/unary/common but exposes three
  direct global-object query failures. get_sigma_sofar drops stored alignment
  even though final extraction preserves it. Carry the actual declaration
  alignment there; do not infer it from a containing aggregate or type.

- Oracle4 reaches OCaml compilation with the exact projected type queries,
  but fails its exhaustive pp_errors match for ArrayElementSizeAlignment.
  Extend the handwritten error printer in the isolated patch, not a generated
  output. All3367inputs verify; the failed receipt/key remain diagnostic.

- Final alignment oracle boundary: retain the full adopted proxy patch0006.
  Its intermediate hunk removal was superseded by the IEEE obstruction. Only
  ctype_of/qualified_ctype_of are projected verbatim from shared source; all
  unrelated translation_aux code stays pristine after the adopted patches.
  Oracle5 and the full gate pass; no stale or failed oracle counts as evidence.

- Combined alignment postchecks attempt1: full gate passes3640 source pins, but
  component receipt readers require sha256 while the lead request used the
  service-supported source_hashes spelling. Preserve the successful gate and
  failed postchecks; submit a new uniquely named request with unchanged source
  hashes and the shared schema. Do not rewrite the original request/receipt.
  The extra host-tcc matrix also selected a stale pre-typedef binary; raw Lean,
  isolated OCaml and Cabs agree, but old host tcc fails42rows. Preserve that
  binary/build tree and all observations, rebuild pristine+current patches,
  then rerun. Those old host observations are not current compiler evidence.

- Alignment integration packaging: including both old/current host source trees
  includes filesystem symlinks. Default tar emits symlink records, while the
  portable verifier intentionally requires independently hashed regular files.
  Preserve the failed encoding in build/alignment-package-links.tar.xz;
  repack the same inputs with dereference=True and verify every member. No
  observation is removed and no failed package is adopted.

## Unevaluated compiler-action syntax, 2026-10-28

- Generic selected-only replay cannot construct complete C11 Cabs. The real
  compiler skips unselected associations; retain each scanner-owned saved word
  map and run the model expression actions on an isolated compiler snapshot.
  Never use independent parser Cabs or returned origins as model inputs.
- First independent generic/static-assert observations passed, but shared-token
  generic transport failed. Replay lookup must precede the fixture EOF policy,
  and the fixture runner must update the saved word cursor too. The first two
  builds/comparisons remain separate from the final passing controls.
- The service publishes worker-build.result before the verifier archives a
  request-specific receipt. Waiting only for the latter delayed a completed
  build; overlapping recovery launches then collided in an existing output
  directory. Preserve the collision log and rerun in a fresh unique directory.
- unevaluated-lean-4 fails on a multiline Lean record update's first-field
  layout (Gen4 line66). Its matching receipt verifies all3331 inputs and exit1.
  Put the first field on its own aligned line; do not raise budgets.
- Strict GCC and independent Raw.checked accept `_Static_assert(1,L"wide")`;
  the unchanged compiler/model reject it because parse_mult_str expects TOK_STR.
  This is a valid-C compiler residual, outside successful syntax counts and
  still inside the whole-C obligation. Do not call it an invalid control or
  add a Supported exclusion. Ordinary/adjacent string messages are positive
  controls. A no-message assertion is a distinct non-C11 extension diagnostic.
- Direct native capture hits this sandbox's denied systemd user bus. Retain
  the failed capture as a harness error and service the identical batch through
  the capped frozen full gate; never substitute an uncapped Lean/Sail run.
- Final unevaluated staging is blocked creating this worktree's
  `.git/worktrees/sol-f2-tokens/index.lock` (read-only filesystem). Use the
  existing frozen lead commit handoff with scoped path hashes; no alternate
  index/object store or new semantic task is needed.

## Array alignment decay, 2026-10-28

- Array aliases can legally have alignment32 and size8; deriving `a+0` must
  clear outer alignment while preserving `typeof(a)` and the referenced array
  through `&a`. Never copy the old compiler's wrong32 result into CSem or
  reject that valid source. The original full-compiler Sail32/cross-GCC8
  captures remain unchanged in the GNU alignment evidence.
- Reset only when ARRAY/VLA conversion occurs. Aligned pointer aliases retain
  their own alignment through arithmetic; clearing every pointer result would
  hide this bug by destroying independent pointer metadata.
- The exploratory arraydecay-alignment-1 run used pre-checkpoint Lean
  executables: raw parsing failed before emitting layout.pp, so downstream
  Menhir commands also failed on the absent layout file. These are stale-tool
  harness failures, not semantic differences. Rebuild current executables
  before measuring; refreshed comparison has54 defined-zero/12 constraints,
  two separate function-policy obstructions and three placement limitations.
- The first C11 summary omitted five added negative fixtures from its printed
  count despite executing each diagnostic. Include array_decay_invalid in both
  iteration and summary; final lane is210positive/27negative. Preserve the
  initial log instead of rewriting the old22 summary.
- Reconstruct a patch workspace from the complete vendor tree plus preceding
  patches: copying selected files omitted tccelf.c and failed before edits.
  The local locked/capped build again hits the denied systemd user bus; use
  frozen service receipts, never an uncapped build.
- Array-decay staging again fails creating this worktree's index.lock on
  read-only Git metadata. Use the existing frozen scoped lead commit handoff;
  no alternate index/object store, push, main merge or semantic rerun is needed.

- Array integration lead setup: base-image rebuild and strict C11 checks both
  rebuild build/cross/riscv64-tcc. Running those commands concurrently caused
  PermissionError while the checker tried to execute the cross compiler being
  replaced. The first failure log remains build/array-integrated-c11.log;
  image creation completes before the checker is rerun serially. An attempted
  output-tree move used a root-relative path inside the worktree and failed,
  so the checker reused its build directory; no unchanged first output-tree
  archive is claimed. This setup failure supplies no valid coverage.

## Local association replay erasure, 2026-10-28

- `replay-erasure-lean-1` fails because a bare `split` chooses the outer
  wrapper-result match, leaving the internal parser-result match in an
  equality hypothesis. Split that hypothesis, including its shape check,
  before eliminating result equalities. The shared `erase_syntaxFields`
  record-update law already compiles; no parser-success premise, execution
  unrolling or budget increase is needed. The failed receipt/log/source
  snapshot are retained in the replay-erasure evidence.
- The direct root-locked, systemd-12GB focused attempt fails on the sandbox's
  user scope bus before Lean starts. The existing frozen lead-build service
  compiles the identical proof under the same lock/cap; no uncapped fallback.
- Ordinary staging again fails creating this worktree's read-only
  `.git/worktrees/sol-f2-tokens/index.lock`. The existing scoped lead commit
  handoff supplies the commit; do not resend semantic/proof work or create an
  alternate index/object store to address this environment limitation.

- Completed physical-capture storage verification initially selected the XZ decoder for a gzip archive and stopped before unlinking anything. Rerun with gzip verified all 1,791 files before removing only duplicate loose captures; the committed archive is unchanged.

## C11 assertion message encodings, 2026-10-28

- Do not broaden `parse_mult_str`: asm labels and section/alias/visibility
  names retain their existing char-string policy. Use the assertion-only
  consumer after phase 6, and retain raw scanner spellings for Cabs. Reading
  wide data as a C char string would truncate `before middle after` to `b`.
- Mixed UTF-8/wide strings are constraint diagnostics; differently prefixed
  wide strings are implementation-policy diagnostics. Neither, false
  assertions, malformed inputs nor message-free C11 extensions add successful
  coverage. The original valid-wide refusal remains unchanged in the prior
  evidence archive and is separately reproduced by the pre-repair host build.
- The direct root-locked, systemd-12GB Lean attempt again fails at the sandbox
  user bus before Lean starts. The frozen build service supplies the identical
  capped build; no uncapped fallback is used.
- A diagnostic postcheck wrongly expected `same-refusal` in model_diff totals.
  Totals contain n/successful same; inspect the 61 same-refusal rows and zero
  successes. The original passing observations and failed postcheck remain;
  correct only verification, without a semantic rerun.
- The first fast-arena export included implicit `-std=c11` in the cache key.
  The actual compare/ref_run key records explicit `['-w']` because both
  entry points prepend the standard flag. Reuse that exact key; preserve the
  missing-cache export failure and both independently generated arena files.
- Regenerating a unified patch can reshuffle unchanged context hunks. Compare
  complete reconstructed C trees: all 552 files match except tccgen.c, whose
  exact old bytes return after reverting only the assertion helper/call.
  Two new patch-context blank lines trigger git diff --check's whitespace
  diagnostic; they are context markers, with no unrelated emitted C change.
- Final staging again cannot create this worktree's read-only index.lock.
  Preserve the exact stderr beside the evidence package and use the existing
  frozen scoped lead commit handoff. No alternate index/object store, push,
  main merge or repetition of semantic work addresses this infrastructure limit.

- Bitfields (2026-10-28): the requested p05 filename is singular
  `c/tests/cfg/p05_bitfield.c`. A reader flag or ignore-bitfields switch is
  not execution support. Retain physical unnamed padding but skip it in
  source lookup and initializer cursor siblings; concatenating base-type
  representations overwrites adjacent fields. Track partially known bits
  rather than zeroing an uninitialized container or poisoning a written field.
- Bitfields build1: Lean needed an explicit `Option ctype` result for union
  member-type lookup; its failed matching receipt verifies 3,433 inputs. Build2
  failed the observation erasure adaptation, with all 3,436 inputs verified.
  Persistent volatile events must stay separate from optional diagnostic
  capture and preserve its state-erasure laws. Do not erase volatile effects
  just to restore the old proof. Build3 exposes the remaining layout fold,
  filtered-union membership and store erasure proof adaptations at normal
  budgets; its 3,462 frozen inputs verify. All failed receipts remain failures.
- Bitfield default oracle1 duplicated patch0004 arity hunks already supplied
  by newly shared ailTypesAux/genTypesAux. Preserve that failed key. Oracle2
  verifies the exact shared hunk post-images and omits only those duplicate
  sections, without reverse/force/fuzz or changing completed caches. It passes
  at the pinned toolchain; this is the default unsupported-bitfield oracle,
  never a target-bitfield execution success.
- Compound assignment is an inherited ordinary-scalar bug too: pristine704c4e69
  reports UB035 for valid `a[i++] += 2`, while strict GCC returns zero. The
  shared lowering must bind the actual lvalue address/load once, unsequenced
  with the RHS, then reuse typed arithmetic/store lowering. Index, pointer and
  function-returned-lvalue controls are needed; merely repairing new bitfields
  leaves CSEM-14. The lead independently retains the minimal function-call
  counter1-versus2 reproducer; no external report is sent by this worker.
- Distinct bitfields in one byte are distinct scalar objects for C11's
  unsequenced-side-effect rule. Byte-only footprints falsely report UB035 on
  `(s.a=1)+(s.b=2)`. Preserve bit-level semantic footprints independently of
  backing-byte merging. Same-field and ordinary-byte/field conflicts remain.
- `sizeof(expr)` is folded during Cabs desugaring before the Ail sizeof-expr
  case. A constraint added only to that later case misses `sizeof(s.a)`.
  Check the member-designating expression before folding, and retain value
  widths for promotions after assignment/increment/comma without making every
  such value expression designate the original bitfield object.
- First raw bitfield diagnostic omitted `--fslist c/build/fs.list`, so every
  case stopped at missing injected headers. Retain it as a harness failure;
  never remove the actual header to count success. Subsequent raw captures
  retain all compiler aliases and the complete positioned Cabs in compressed
  stdout archives. GNU trailing attributes on bitfield widths remain four
  separately recorded parser nonsuccesses; they are not C11 agreements.
- Original p05 now exposes separate runtime/format boundaries: without libc,
  its memset body is absent; with the existing libc preparation, the raw-byte
  printf is classified UB153b by the inherited formatter. Keep both raw
  observations; the independent cast-correct layout fixture is a distinct
  source and cannot be renamed as successful execution of original p05.

### Bitfield follow-up: character representation and complete test targets

- A passing narrow target build did not compile every CSem test. The first full
  gate found a positional AbsByte constructor and reconstruction/fuel-exemplar
  proof adaptations. Update reference leaves and named-member selection while
  retaining all quantifiers; use concrete footprint reduction, not weakened
  theorem statements or larger budgets. Keep every failed receipt.
- New partial-known bytes must survive character copying (C11 6.2.6.1p4-6).
  Merely returning MVunspecified for a partially known unsigned-char load loses
  live field bits when that value is stored. Lead's minimized three-bit copy
  returns zero with GCC and fresh full Sail, but the initial candidate returned
  non-integer-return. Preserve before/after and full-byte/aggregate controls.
  Enumerate every concrete character representation consistent with known bits;
  do not zero unknown padding or invent the neighboring field. Entirely unknown
  bytes retain the existing unspecified behavior. Keep this regression focused.
- After editing a handwritten adapter, regenerate its generated copy BEFORE
  freezing a build request. The first character-copy build requests still had
  the previous generated adapter; they do not validate the repair. Do not hand
  copy a recovery output or claim a stale executable tests new semantics.
- Do not update literal `reconstructValueFrozen{,_indexed}_lemFuel` bodies and
  then cite `historical_*_recovers` as unchanged history. The corrected lane
  restores both bodies byte for byte from the base and gives the current
  reference an explicit historical/bitfield representation profile, independent
  of its six diagnostic messages. Both recovery theorems and current all-input
  indexed/linear equivalence compile normally. Reduce only the profile projection
  in proofs; unfolding the entire diagnostic record needlessly obscures the
  induction hypothesis.
- The general worker-build service does not execute the custom isolated OCaml
  command (oracle-4 failed before execution). Keep that actual failure and a
  separate local verification of its frozen hashes; do not fabricate an oracle
  success or reuse a mismatching content key. Manual lead service of the exact
  capped/locked worker-toolchain request is required, as for oracle-1/2/3.
- Keep the pristine-libc preparation on its intended pinned pristine runtime.
  Forcing the repaired shared OCaml engine into that wrapper changes the dumped
  libc Core hash and correctly fails its pristine pin check. Preserve the
  diagnostic capture rather than bypassing the gate or rewriting its pin. The
  ordinary final wrapper repeats p05's unknown-memset (without libc) and reported
  UB153b at the raw-byte %x call (with libc). This is a classified format/runtime
  boundary, not a proved formatter defect or successful p05 run.
- For final candidate p05, keep candidate CSEM_ORACLE for user Cabs but the
  pristine CSEM_RUNTIME that produced the textual libc metadata. Mixing those
  metadata locations with fork runtime directories correctly triggers exact
  library-location admission; do not relax membership or rewrite labels. The
  consistent original still reports UB153b; the separately named unsigned-print
  variant is defined with value1 and exactly matches GCC output. These are
  separate captures, not a source edit or new native claim.
- Patch 0009's five single-space blank context lines are required unified-diff
  markers. The lead reviewed their cached whitespace warnings as acceptable;
  do not strip those markers and corrupt the compatibility patch.
- Bitfield integration gate ordering (2026-10-28): worker full2 passed after
  its focused raw build, but the first combined gate consumed the previous
  assertion checkpoint's csem_raw binary. Its exact hash matched that prior
  image; all 72 required defined bitfield cases failed before elaboration,
  while ten constraint checks passed. csem_gates stage2 built csem_run but
  omitted csem_raw; check.sh rebuilt raw only later, after this new consumer.
  Add csem_raw/csem_cparse to the earlier explicit build. Retain the failed
  frozen receipt, complete observations and binary-hash diagnosis. Do not
  repair this by priming a cache, weakening cases, or treating stale failures
  as semantic regressions. The new combined gate must pass from that state.

## GNU expression compiler-action syntax, 2026-10-28

- Omitted-middle conditionals already have the model's distinct two-child
  condGNU action. The independent raw grammar only handled the text lexer's
  QUESTION_COLON token; real phase four correctly supplies QUESTION and COLON
  separately. Accept that GNU grammar production while retaining both tokens
  and origins. Do not fuse preprocessing tokens, inject spec syntax into the
  compiler model or replace the conditional by duplicated condition syntax.
- The first focused suite finds two GCC-diagnosed GNU policies accepted by both
  compiler/model: a read-only increment inside typeof and a comma condition
  in choose_expr. GCC diagnoses both. Preserve the failed first expectation,
  independently checked complete syntax and exact actual compiler policy;
  classify these outside valid syntax and defined-program arena counts. The
  read-only-location warning is itself a diagnostic; do not claim absent-
  diagnostic C11 conformance evidence for either GNU extension policy.
- The first arena matrix gave invalid snippets no main, so an accepted
  constraint could still become a matching unresolved-main refusal. Its 12
  refusals are retained as superseded observations, not constraint evidence.
  Add main to every executable probe and independently compile each input
  with GCC and the current cross tcc before classifying compiler diagnostics.
  The corrected matrix distinguishes ten actual refusals and two acceptances.
- A bare host tcc -run invocation lacked its configured runtime search path
  and reported missing runmain.o/libtcc1.a. Preserve this host harness failure;
  supplemental host controls use the exact -B build/test-c11/host directory.
  Host observations never substitute for native Sail arena/execution evidence.
- The direct root-locked systemd-12GB focused build again fails at the sandbox
  user bus before Lean starts. Its exact log and the matching frozen-service
  receipts are retained. No uncapped build or dependency-project modification
  is used. The adopted local replay proof remains unchanged.
- GNU staging again cannot create this worktree's read-only index.lock. The
  exact stderr is in results/model/gnu-expression-syntax/git-staging.stderr;
  use the existing frozen scoped lead commit service after archive/source
  verification. No alternate Git index/object store or semantic rerun repairs
  this environment restriction.

- GNU integration semantic review: raw driver process exit0 does not mean
  Defined(0). Inspect the semantics field. Ten source-pinned probes yield three
  defined-zero controls, one GNU conditional startup error and six VLA
  elaboration failures despite GCC/host success. The inherited GNU ?: branch
  records TODO/returns skip; array elaboration refuses nonconstant bounds.
  Keep these as F1 tasks CSEM-15/16, not syntax successes, source exclusions,
  claims of fresh native execution or confirmed pristine wrong answers.


## GNU bitfield declarator attributes, 2026-10-28

- The original four GNU member cases stop parsing at width-suffix attributes.
  They supply no layout result. Cabs's old optional declarator cannot carry
  attributes for unnamed padding; add an explicit existing Annot.attributes
  carrier instead of inventing a source-visible name or lifting to siblings.
- bitattr-lean-1 fails in build-service setup before a verified completion;
  its invalid tcc_model_ghost target is not tcc_ghost. bitattr-lean-2 verifies
  3,531 inputs and fails on shadowing the mutable aggregate attrs variable in
  GhostCabs. Rename the field-local binding; no budget adjustment. lean3 passes.
- GCC rejects the four exploratory pre-width/comma-leading placements. Keep
  their eight storage variants as syntax diagnostics, outside the GCC-defined
  layout batch; do not call them layout disagreements or narrow Supported.
  Named GNU attributes after widths and leading declaration/specifier forms
  have separate valid controls. Menhir placement failures stay separate too.
- A whole 42-function CSem printf-layout batch was interrupted after several
  minutes with no result. Retain the empty captures and exit130; use generated
  individual layout programs and compare every initialized byte. The unchanged
  full batch remains the native compiler/GCC observation, not a CSem success.
- Fresh full-compiler Sail reveals inherited ad1 leakage across comma siblings:
  the middle-aligned guard has tail17 versus GCC9. Reset each compiler/model
  member from the shared specifier attributes, preserving legitimate leading
  attributes on all declarators. Retain the complete before-image and console.
- Zero-width fields ignore packing but still honor explicit GNU aligned
  barriers. A member's explicit alignment also overrides aggregate packing;
  do not infer either property only from the merged numeric alignment.
- Ctrl-C on the combined diagnostic returned wrapper exit 130 while its child
  survived outside the worker PID namespace. The lead verified the exact host
  PID/cwd/command/source and sent SIGTERM after 1,542.695 seconds since capture
  creation. Preserve that stop receipt; bounded direct semantic subprocesses
  in the maintained checker avoid this wrapper/orphan ambiguity.
- The lead's exact early raw-executable gate fix is commit 79e1454. Reuse that
  committed hunk; attempting to apply it again after the lead installed it
  correctly fails without changing source. The final gate has its own new
  frozen manifest rather than relabelling the historical lean4/oracle1 pins.
- Final bitattr staging also cannot create this worktree's read-only index.lock.
  Retain commit-stage.stderr and use the existing frozen scoped lead commit
  handoff; no alternate index/object store or semantic rerun is needed.

Lead attribute packaging, 2026-10-28: tarfile's default link preservation left
five host-build Makefile symbolic links in an otherwise regular-file archive.
The strict member/hash verification rejected it. Use dereference=True for
portable captures and verify all regular bytes; the retry passes 13,141 members.
The initial cause label guessed inode deduplication; retained type-2 entries
identify symbolic links. The failed archive remains in build/bitattr-package-failed-1.

Worker relaunch after enum review: plain `git rebase main` flattened reviewed
merge history even though main was already an ancestor, replaying the adopted
attribute checkpoint and producing redundant documentation conflicts. The
attempt was aborted; exact prior history is restored. The launcher now checks
ancestry first and preserves existing reviewed merges; an actual update uses
`--rebase-merges`. The enum-base documentation conflict helper also initially
used a multiline-greedy marker pattern; restore both append-only sides from
the original merge parents with newline-bounded markers. All enum, upstream,
physical-proof and earlier dead-end sections are retained after that correction.
