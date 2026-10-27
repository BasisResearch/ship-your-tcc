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

Queued 2026-10-27: after tasks/f3-c11-model.md and tasks/f1-rv64-config.md
land, assign tasks/csem-repin.md before another CSem semantic-change task.
Keep both current slots on their existing pin; see CSEM-UPSTREAM.md.
