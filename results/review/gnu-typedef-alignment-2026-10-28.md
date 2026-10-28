# GNU typedef alignment query discrepancy

The patched compiler preserves GNU typedef alignment when laying out members,
but loses it in `_Alignof(T)`, including through another typedef. The focused
reproducer is `results/repro/tcc_gnu_typedef_alignment.c`; the repair is queued
in `tasks/f3-typedef-alignment.md`. This is an open GNU extension discrepancy,
not a successful compiler comparison or a restriction on Supported.

| Observation | stdout | steps | exit |
|---|---|---:|---:|
| xPack GCC RV64I program on Sail | `4 16 16 32 \| 4 1 1 5 \| 16` | 9,012 | 0 |
| Patched cross-tcc program on Sail | `4 4 16 32 \| 4 4 1 5 \| 4` | 8,725 | 0 |
| Full patched `tcc -run` on Sail | `4 4 16 32 \| 4 4 1 5 \| 4` | 13,486,910 | 0 |

Each group reports size, type alignment, member offset and aggregate size;
the final value is the alignment through a chained typedef. The full compiler
run takes 13,481,097 compile steps and 5,788 program steps, with the remaining
steps in the existing harness handoff. Cross-tcc object and linked ELF ISA
checks pass. The differential exits **1** because stdout differs; both programs
reach their traced exits without a Sail limit. Host GCC agrees with RV64 GCC.

The native compiler source diagnosis points to separate attribute transport:
`parse_btype` copies typedef attributes to `AttributeDef`, while the unary
alignment query recovers only `CType` and a previous-vstack-symbol heuristic.
This is a repair lead, not a proved cause or completed patch.

GNU expression-alignof is a separate observation: the retained aligned
aggregate/member probe prints `16 1 16 1 16 16` with host GCC, but
`16 1 16 1 16 1` with patched host tcc. Do not conflate that extension choice
with the reproduced type-query failure or silently use GCC as cfgTcc.

The existing static gate passes. `gnu-typedef-alignment-2026-10-28/` contains
the input/binary/source pins, complete cross-program and full-image ELFs,
traces, console bytes, commands and gate log in a verified archive. All archive
members were checked against their SHA-256 hashes. No compiler/model source
changed, no compiler theorem is claimed, and the sum compile-cut is untouched.
