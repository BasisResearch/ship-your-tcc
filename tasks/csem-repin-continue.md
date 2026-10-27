Binding rules: read AGENTS.md, CLAUDE.md, BRIEF.md, DESIGN-COMPILER.md and PHASES.md.
Follow MANAGER.md; reuse inventory; no new axioms, sorry, native_decide or budget raises.
Generated files are generated; preserve sum.c's frozen compile-cut artifacts.
One shared-lock Lean build at a time, systemd MemoryMax=12G; free -g >=25GB first.
No external messages. No push/merge main; leave a reviewed candidate for the lead.
Start only after current tasks f3-c11-model and f1-rv64-config land through the gate.
Target: evaluate repinning CSem onto current OathTech/cerberus-lean; CONTINUE now.
Read build/lead-note.txt first: the first worker stopped at resolved sandbox limits.
Use ready local build/upstream-cerberus (704c4e69) and build/upstream-lem (e6d8db56).
Those are lead-fetched exact current upstream snapshots; no network needed.
Upstream uses Lean 4.35; evaluate adapting to our approved 4.34 before toolchain changes.
Freeze a unique request, targets array, sha256 relative-input map in build/worker-build.request.
The lead service builds under the shared 12G lock and writes worker-build.result; wait for it.
Do not exit because the worker cannot use systemd/DNS/Git; lead handles those operations.
If generator rebuild is needed, request exact commands in build/worker-toolchain.request;
use an isolated project-local opam switch, never mutate another lane's toolchain.
Use a fresh worktree, within the project's two-Sol-worker limit.
Read vendor/CSEM_REV, csem/PORT-4.34.md, CSEM-UPSTREAM.md and scripts/csem_import.sh.
Record upstream branch, exact commit/date and compatible lem-lean pin before edits.
Replay/rebase our fork changes on the selected import; preserve provenance.
Identify upstream totalization/trust fixes that absorb our patches; do not duplicate them.
Keep C11, explicit config/digest readers, image FS and the approved theorem shape.
Regenerate Lean from sources with the matching pinned generator, never edit outputs.
Files: csem/, vendor/CSEM_REV, import/regen/gate scripts and root Lake integration.
Done: capped build, fork gates, regeneration check, baseline oracle and focused
config/FS/parser tests pass, with differences explained and no weakened gates.
Write results/csem/repin-2026-10-27.md with old/new pins, patch disposition,
trust-cone comparison, measured regressions, rollback commit and adoption recommendation.
Update CSEM-UPSTREAM.md with minimized reproducers and current-upstream disposition.
If evaluation cannot pass, retain the current main pin and report the concrete gap.
Finish RESULT: landed <commit> or RESULT: stuck <where>; record dead ends.
