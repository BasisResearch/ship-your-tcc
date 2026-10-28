# Astra lead and Sol workers

The lead plans, reviews evidence, maintains STATUS.md/PHASES.md, and lands
through `scripts/check.sh` plus the lane's relevant checks. Proof work runs
in external `codex exec -m gpt-6.1-sol` workers, never built-in subagents.
At most two workers run for this project; each owns a git worktree.

From the repository root, launch a background worker with a retained shell:
`cat codexwork.sh | bash -s -- 1 tasks/name.md > /tmp/tcc-sol1-launch.log 2>&1 & wait`
(slot 1 or 2). The pipe snapshots the script before the worker starts;
editing a script while bash is waiting inside it can corrupt its resumption.
The retained shell also prevents this execution environment from reaping
an orphaned launch before its worker starts.
The launcher checks memory, locks the slot, rebases a clean worktree on main,
and records `logs/sol-<slot>-<time>.log`. `WORKTREE=/absolute/path` adopts a
reviewed recovery worktree; dirty worktrees and conflicts stop the launch
without discarding changes. Every Lean build holds the shared
`build/lake.lock` and runs under `systemd-run --user --scope -p MemoryMax=12G`.
Check `free -g` before builds; wait below 25 GB available. The launcher
detaches inherited hard-linked Lake artifacts under that lock and invalidates
affected CSem traces. Never seed another worktree with hard links: Lake's
in-place trace updates can certify another checkout's stale object.

If the worker sandbox cannot connect to the systemd user bus, keep the cap:
report the exact build targets and stable source revision for the lead to
build under the shared lock. Do not fall back to an uncapped Lean build.
If Git metadata is read-only, leave the worktree diff for the lead to
review and commit; the lead has the required access. Neither environment
limitation requires a change to the proof discipline.

Task files contain fewer than 40 lines: binding project rules first, one
target, files, and an explicit completion criterion. Workers commit but do
not push or merge main. Their final line is `RESULT: landed <commit>` (ready
for review) or `RESULT: stuck <where>`. The lead reviews and integrates, then
pushes main without force. Record unsuccessful approaches in
`tasks/DEAD-ENDS.md` before assigning another task.

The launcher follows 4ct-digest's MANAGER.md/codexwork.sh workflow, adapted
to this project's two-worker limit and shared Lean build lock.

Current assignments, 2026-10-28: slot 1 runs tasks/f1-enum-bitfields.md in
sol-csem-repin. Slot 2 has completed physical-proof checkpoint b42f766 in
sol-f2-tokens; it and multi-TU checkpoint 96c520e await combined main integration.
Slot 2 now runs tasks/f1-gnu-conditional-semantics.md from combined base 75a2df8.
Bitfields and GNU expression syntax are adopted after their combined gates.
Member attributes are adopted after rebuilt-image integration.
The local replay-erasure theorem is adopted after its combined gate; eleven
ghost contracts remain open. Both workers use reviewed integration bases,
and the lead independently gates each final combination before adoption.
Original branches and evidence remain preserved. No third worker runs.
Bitfield compiler constraints, general declaration constraints, other upstream
invocation modes, complete multi-TU ghost construction and runtime body coverage
remain queued. The bitfield diagnostic task depends on the F1 native review.

Frozen build requests use the shared 12 GB lock. Changed inputs invalidate
a request; freeze a new hash manifest before retrying. Infrastructure limits
are handled by the lead service, not by uncapped builds or user permission
requests. The adopted pin-addressed opam/oracle toolchain is shared read-only;
workers must not repin or mutate it during independent semantic tasks.

Lead review is delivered in worktree `build/lead-note.txt` and frozen build
receipts. Workers read it before new build requests and before finalizing;
requested corrections must be resolved or returned as an explicit obstruction.

Keep build receipts compact: inspect ID/status/lead notes, and verify large
hash manifests with scripts. Do not repeatedly print whole manifests or logs.

Next integration duty: combine reviewed multi-TU checkpoint 96c520e with the adopted
member attributes/GNU syntax and repeat the full gate plus fresh native104. Assertion-message adoption has a
fresh combined default gate, source-identical worker full gate and fresh native
image observation. Bitfield review must preserve known bits through character
copies and distinguish historical frozen references from current representation
profiles. Keep GNU trailing-attribute parser failures and compiler constraint
diagnostic gaps explicit; their follow-up tasks remain queued. The local replay theorem changes no runtime
behavior and proves no whole-run construction or binary refinement duty.

The active pure physical-transform proof task is
tasks/f3-physical-transform-proofs.md. Retain the unchanged named
Statements, arbitrary-byte scope and standard-axiom audit; a checked obstruction
is preferable to silently weakening a statement. Slot 1 handles enum bitfield policy. Subsequent F1 tasks include general
declaration constraints, GNU conditional lowering and VLA types/storage. Compiler diagnostic and declaration constraints remain
separate tasks. Keep at most two external workers active.

After the enum semantic checkpoint, prioritize tasks/f3-enum-colon-forms.md:
valid unnamed enum fields/direct enum generic associations currently refuse.
Preserve GCC expression-type policy differences as a separate axis. Invalid
bitfield diagnostics, GNU conditional semantics and VLA storage remain queued.
