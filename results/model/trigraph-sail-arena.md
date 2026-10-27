# Full Sail arena observation — 2026-10-27

The independent model and executable Sail agree on **all 4,194,304 arena
bytes** for the trigraph boundary/header probe. The model's 13,652-byte
payload is zero-extended to the arena extent; the full capture includes the
trailing zero region. The fast interpreter's arena also equals the model's.
Entry `0x8640021c`, code base `0x86400000`, and code size `0x21c` agree with
Sail registers x10/x14/x15 and the independently retained execution trace.
Both Sail observations reach the first pre-instruction state at
`tcc_baremetal_enter` (`0x80001b48`) after **3,390,238 instructions**.

This is one complete native observation, not a Lean reachability proof or
the all-corpus F3 exit criterion. No model input came from the reference
arena. The ELF belongs to the trigraph patch revision; subsequent compiler
changes require fresh captures. No sum compile-cut artifact was regenerated.

The unchanged `scripts/capture_cut.py` / `gen_capture_cut.py` utility was
built under the shared lock and a 12 GB systemd scope, reusing the attributed
Sail/ELFSage objects from ship-your-lua. The local Vsa/Elf.lean source and
the reused copy had identical SHA256. The manifest pins every linked object,
generated wrapper and capture executable. Capture used another 12 GB scope,
FLAGS=0 and the utility's existing 8,000,000-step limit. Compiler console
output is empty; the 13,650 present bytes and absence map are retained.

`trigraph-sail-arena.json` records the comparison and input hashes.
`trigraph-sail-capture/` retains the capture/build manifests, all raw outputs
(large zero-filled arrays gzip-compressed), independent model arena and
probe source/header. The original ELF is retained locally at
`build/sail-arena-review/probe.elf`, pinned by SHA256 in the manifest.
`results/pre/trigraphs-build.json` records its construction and dependencies.
The trace is `results/pre/trigraphs-sail.trace`.

To repeat the observation for that frozen ELF:

```sh
systemd-run --user --scope -p MemoryMax=12G python3 scripts/capture_cut.py capture \
  --elf build/sail-arena-review/probe.elf \
  --source results/model/trigraph-sail-capture/probe.c --out build/fresh-cut
python3 scripts/compare_sail_arena.py --capture build/fresh-cut \
  --model build/sail-arena-review/model.arena --trace results/pre/trigraphs-sail.trace
```

For an offline recheck, decompress the three gzip files into a temporary
copy of the retained capture directory and supply its `model.arena` to the
same comparator. It checks all manifest file hashes, complete arena bytes,
presence encoding, handoff registers and trace step. Negative checks reject
a changed captured byte, a changed model byte, a malformed model header and
a changed trace step. Source, ELF and tool provenance remain explicit host
evidence; the comparator does not certify those hashes in the kernel.
