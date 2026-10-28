# Isolate compiler comparison invocations

`model_diff.py` now serializes invocations within one worktree before image
preparation, cache access, model scratch files and captures. The source-derived
scratch names and cache entries therefore cannot be modified by a second live
comparison. Jobs within the owning invocation retain their configured
parallelism. Another worktree has its own lock and files. Duplicate selected
input paths are rejected before comparison, preventing both an internal race
and duplicated success counts. Argument errors and help do not wait on the lock.

Two real concurrent comparisons of the same C11 union source each finish with
one matching successful arena; one records waiting for the other. Dedicated
process tests verify exclusion, release after normal/error exits, and duplicate
input rejection. They now run in the maintained static gate. Six reference
protocol and seven native-capture protocol methods also pass. Exact source
pins, reports and logs are in model-comparison-isolation-2026-10-28/.

The stopped overlapping run in the earlier corpus-fixture report remains a
failed diagnostic. This change supplies no new native Sail observation and
changes no compiler/model semantics or sum proof artifact. The existing
source-hashed cache naturally distinguishes the updated harness.
