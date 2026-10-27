# Three complete C11 Sail arenas — 2026-10-27

`model_diff.py --sail-capture` now checks every successful fast-reference
arena against a complete native Sail capture before admitting it to the
success cache. It compares every arena byte, including the trailing zero
region, plus entry address, code address and code size. The model receives
source, filesystem and image symbols only, never captured arena bytes.
Matching refusals remain fast-reference diagnostics with no Sail coverage.
Neither this executable comparison nor its host hashes are a Lean proof.

| source | instructions to pre-entry cut | compared bytes | result |
|---|---:|---:|---|
| c/tests/c11/pragma.c | 12,908,292 | 4,194,304 | identical |
| c/tests/c11/utf_literals.c | 13,171,390 | 4,194,304 | identical |
| c/tests/c11/union.c | 13,311,282 | 4,194,304 | identical |

All three also match the independent model and the fast interpreter; zero
refusals, differences, model errors or harness errors in this run. Wall time:
345 seconds with three captures, each in its own 12 GB systemd scope.
Memory was checked before each capture. The runtime is patch 0003 with
trigraph handling, before the separate line-provenance repair.

The capture generator now accepts a finite runtime step limit as an argument
(the wrapper default remains 8,000,000). This is not a Lean elaboration-budget
change. The reused machine setup and step function are unchanged, as are all
sum proof artifacts. Build uses the same shared lock and 12 GB cap.

```sh
free -g
flock build/lake.lock systemd-run --user --scope -p MemoryMax=12G \
  python3 scripts/capture_cut.py build --build-dir build/capture-cut-parametric
python3 scripts/model_diff.py c/tests/c11/pragma.c c/tests/c11/union.c \
  c/tests/c11/utf_literals.c --sail-capture build/capture-cut-parametric/capture_cut \
  --sail-max-steps 30000000 --sail-timeout 1200 --no-report \
  --json-out results/model/sail-c11-batch.json --require-success 3 -j 3 -v
```

The JSON retains every source/image/model/tool pin and every capture result;
all run pins were rechecked after completion. `sail-c11-captures/` retains
complete compressed arenas/presence maps, independent model payloads,
register dumps and manifests. The raw data can be rechecked offline with
`compare_sail_arena.py`; decompress only each `model.arena.gz`, since the
comparator directly accepts compressed capture arrays. Cache reuse also
revalidates the retained complete capture rather than trusting a success tag.

Seven protocol tests pass using the real retained trigraph capture: changed
trailing bytes, model bytes, metadata, trace steps, and cached data are rejected;
a failed capture cannot publish success. The six existing reference-protocol
tests still pass. A real zero-step capture fails without writing `cut.json`.
A real one-second timeout is a harness error with zero successes; the harness
stops its own uniquely named scope and leaves no capture process running.
Its raw result is `sail-timeout-negative.json`.

The earlier trigraph probe supplies one additional complete Sail arena.
All-corpus coverage, ghost Cabs, fuel adequacy and compiler correctness proofs
remain open; four observations do not discharge the F3 exit criterion.
