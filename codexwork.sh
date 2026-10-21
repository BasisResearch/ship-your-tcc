#!/usr/bin/env bash
# One external Sol worker per slot; the lead reviews and integrates commits.
set -euo pipefail
repo=$(cd "$(dirname "$0")" && pwd)
slot=${1:?usage: codexwork.sh 1|2 tasks/file.md}
case "$slot" in 1|2) ;; *) echo 'slot must be 1 or 2' >&2; exit 2;; esac
task=$(realpath "${2:?task file required}")
[[ -f "$task" && $(wc -l < "$task") -lt 40 ]] || {
  echo 'task must exist and contain fewer than 40 lines' >&2; exit 2;
}
mkdir -p "$repo/logs" "$repo/build/workers"
exec 9>"$repo/build/workers/slot-$slot.lock"
flock -n 9 || { echo "slot $slot is already running" >&2; exit 2; }
log="$repo/logs/sol-$slot-$(date -u +%Y%m%d-%H%M%S)-$$.log"
exec > >(tee -a "$log") 2>&1
echo "Task: $task"
echo "Log: $log"
free -g
available=$(awk '/MemAvailable:/ {print int($2/1024/1024)}' /proc/meminfo)
if (( available < 25 )); then
  echo "RESULT: stuck launch: only $available GiB available (need 25)"
  exit 1
fi
wt=${WORKTREE:-$repo/.claude/worktrees/sol-$slot}
if [[ ! -e "$wt" ]]; then
  git -C "$repo" worktree add -b "sol/slot-$slot" "$wt" main
fi
if [[ -n $(git -C "$wt" status --porcelain) ]]; then
  echo "RESULT: stuck launch: dirty worktree $wt; preserve and review first"
  exit 1
fi
if ! git -C "$wt" rebase main; then
  echo "RESULT: stuck launch: rebase conflict in $wt; changes preserved"
  exit 1
fi
codex=${CODEX:-$HOME/.local/share/codex-0.162.0/node_modules/@openai/codex-linux-x64/vendor/x86_64-unknown-linux-musl/bin/codex}
export SYT_LAKE_LOCK="$repo/build/lake.lock"
echo "$$ $wt" > "$repo/build/workers/slot-$slot.pid"
trap 'rm -f "$repo/build/workers/slot-$slot.pid"' EXIT
set +e
"$codex" exec -m gpt-6.1-sol -c model_reasoning_effort=high \
  -s workspace-write --add-dir "$repo" -C "$wt" \
  "$(cat "$task")
Work only in this worktree. Do not push or merge main; commit for lead review.
No built-in subagents. Builds: free -g, then flock \"\$SYT_LAKE_LOCK\" systemd-run --user --scope -p MemoryMax=12G lake build TARGET.
Record dead ends in your result so the lead does not resend them.
Finish with RESULT: landed <commit> or RESULT: stuck <where> (landed means ready for lead review)." \
  >> "$log" 2>&1
rc=$?
set -e
if ! grep -E '^RESULT: (landed|stuck) ' "$log" | tail -1; then
  echo "RESULT: stuck worker exited $rc without a result; inspect $log"
fi
exit "$rc"
