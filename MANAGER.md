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
The launcher checks memory, locks the slot, updates a clean worktree onto main, preserving already reviewed merges,
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

Current assignments, 2026-10-28: slot 1 runs tasks/f3-enum-colon-forms.md in
sol-csem-repin, after independently reviewed enum checkpoint7907c3a. Slot 2
runs tasks/f1-vla-semantics.md in sol-f2-tokens after reviewed GNU checkpoint
2904c35 and main938fa20 were merged at34c8cc2. The GNU checkpoint keeps
its observed GNU bitfield-promotion compiler bug separate; its compiler/model
repair is queued after enum colon forms. Multi-TU96c520e and pure physical
proofsb42f766 are adopted at main925f88d after the combined full gate, independent
postchecks, portable verification and fresh same-ELF native104 execution.
Both physical-origin/read-view Statements are proved for arbitrary bytes;
read/refill/host, raw and whole-command construction/erasure remain open.
Original checkpoints and failed evidence remain preserved. No third worker runs.
The lead now validates tasks/f2-include-guard-openings.md with a related
model EOF identifier repair; three fresh native captures/same-ELF executions
pass, while its full gate/postchecks remain pending.
Bitfield/general declaration constraints, VLA semantics, pack-layout transport
and runtime body coverage remain work items.

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

Next integration duty: combine reviewed enum semantics and the pending compiler
repair with shared GNU conditional lowering after each worker's full gate.
Native GNU bitfield-promotion mismatch remains a compiler repair, not a new
cfgTcc policy. All final combinations receive their own pinned validation;
old full-gate receipts do not certify a newly merged semantic base.

Next F1 work includes tasks/f1-vla-semantics.md and general declaration
constraints. The independent include-guard opening schedule and pack-layout
consumption also need repair without inventing model reads or erasing metadata.
Keep at most two external Sol workers; all Lean builds share the12GB lock.
