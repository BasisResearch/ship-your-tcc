# Astra lead and Sol workers

The lead plans, reviews evidence, maintains STATUS.md/PHASES.md, and lands
through `scripts/check.sh` plus the lane's relevant checks. Proof work runs
in external `codex exec -m gpt-6.1-sol` workers, never built-in subagents.
At most two workers run for this project; each owns a git worktree.

Launch `nohup ./codexwork.sh 1 tasks/name.md > /dev/null 2>&1 &` (slot 1 or 2).
The launcher checks memory, locks the slot, rebases a clean worktree on main,
and records `logs/sol-<slot>-<time>.log`. `WORKTREE=/absolute/path` adopts a
reviewed recovery worktree; dirty worktrees and conflicts stop the launch
without discarding changes. Every Lean build holds the shared
`build/lake.lock` and runs under `systemd-run --user --scope -p MemoryMax=12G`.
Check `free -g` before builds; wait below 25 GB available.

Task files contain fewer than 40 lines: binding project rules first, one
target, files, and an explicit completion criterion. Workers commit but do
not push or merge main. Their final line is `RESULT: landed <commit>` (ready
for review) or `RESULT: stuck <where>`. The lead reviews and integrates, then
pushes main without force. Record unsuccessful approaches in
`tasks/DEAD-ENDS.md` before assigning another task.

The launcher follows 4ct-digest's MANAGER.md/codexwork.sh workflow, adapted
to this project's two-worker limit and shared Lean build lock.
