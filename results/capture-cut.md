# Observational native cut capture

The helper captures the first pre-instruction state at the ELF symbol
`tcc_baremetal_enter`. It imports the frozen `Vsa.setupElf` and
`Vsa.stepOnce` APIs, using OCaml's bootdump loop and complete generated
Sail register enumeration. It fails on early halt, model error, nonempty
console, or failure to reach the cut within 8,000,000 steps.

This is native observation, not a kernel startup certificate. It supplies
candidate data; it proves neither compilation reachability nor agreement
with Clight. No theorem, axiom, opaque semantic predicate, or root proof
project is introduced.

Build only after `free -g` reports at least 25 GB available:

```
systemd-run --user --scope -p MemoryMax=12G python3 scripts/capture_cut.py build
```

The builder runs no Lake command. It reads existing Lua `Vsa.Elf.olean`,
its already-generated C, and the existing emulator link response and
native dependency objects. The small Vsa.Elf object and helper are compiled
into this checkout's `build/capture-cut`. Missing objects cause failure;
the builder never repairs or rebuilds a sister project's dependencies.
Historical absolute checkout prefixes in the copied response file are
rebased onto the existing Lua tree in our local response file.
`build.json` records the Lean version and input/source/binary hashes.

For a separately built `FLAGS=0` sum ELF:

```
systemd-run --user --scope -p MemoryMax=12G python3 scripts/capture_cut.py capture \
  --elf c/build/p3/sum-flags0.elf --source c/tests/p3/sum.c \
  --out results/p3/sum-cut
```

The output directory must not exist. The wrapper checks `embedded_flags`
in the ELF's initialized bytes and resolves the cut and arena from its
symbol table. It hashes source and ELF before and after capture. Source
hashing identifies the supplied file; it is not a proof that the ELF embeds
that file. Preserve the image build command and provenance separately.

Outputs:

| file | contents |
|---|---|
| `arena.bin` | Total-read bytes of `[0x86400000,0x86800000)`, exactly 4 MiB; absent addresses read as zero |
| `arena.present.bin` | Exactly 4 MiB, one byte per address, 0 absent / 1 present |
| `registers.txt` | Every defined Sail register, in OCaml bootdump's typed textual representation, with coverage checked against map size |
| `console.txt`, `console.repr.txt` | Console bytes and Sail output-array representation; both must show empty output |
| `cut.tsv` | PC, steps, tick, cycles, defined-register count, memory-map size, present-arena-byte count |
| `native.stdout`, `native.stderr` | Native diagnostic streams |
| `cut.json` | Boundary, hashes, sizes, counts, and explicit observation-only status; created only after successful validation |

Only the arena is exported from memory. Stack, heap, and other memory bytes
remain uncaptured; this artifact alone is insufficient for a complete
snapshot witness. Register output includes those pointers but does not
certify the bytes they point to. Unsuccessful runs leave diagnostic files
and have no `cut.json`.

The helper built under the 12 GB cap on 2026-10-20 and rejected a `FLAGS=1`
image before execution. The `FLAGS=0` sum capture succeeded after 6,632,342
steps at PC `0x80001b48`, with an empty pre-cut console, 176 defined
registers and 77,864 present arena bytes. All 136 bytes of main match the
earlier generated-code dump. Artifact sizes/hashes and the presence map
were checked by the wrapper and a separate validation pass.

The observation is in [p3/sum-cut](p3/sum-cut/). The published helper build
manifest is [review/capture-helper-build.json](review/capture-helper-build.json);
its hash matches `build_manifest_sha256` in `cut.json`. Build commands,
negative-check output and validation results are retained under `review/`.
The selected ELF was built with:

```sh
make -s -C c PROG=tests/p3/sum.c FLAGS=0 \
  ELF=build/p3/sum-flags0.elf build/p3/sum-flags0.elf
```

This closes the observational arena/register capture task. It does not
close any of the three semantic proof obligations in PHASES.md.
