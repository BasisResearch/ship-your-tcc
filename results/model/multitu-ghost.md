# Ordered multi-TU ghost construction

At the modeled compiler handoff, every compiled translation unit now retains
an immutable `Ghost.UnitRecord`: actual filename/file bytes and configuration,
positioned input, typed parser result/fragments/audit, physical source/read
logs, recursive origins, pragma effects and the compiler's designated opening
authority. `tcc_compile` appends a record after its ordinary cleanup; the next
file resets only the active ghost observation. Earlier records survive linking.
`--ghost-out` exports an ordered versioned bundle, including partial diagnostic
records. Tokens and Cabs from different units are never concatenated.

The unchanged pinned 104 invocation retains `/104+_inline.c` followed by
`/prog.c`. Each record is compared with an independent `Raw.checked` process
receiving that unit's actual source, filename, filesystem bytes and separately
configured predefined environment. Exported tokens, syntax and opening witnesses
are never inputs to either computation. Complete Cabs (all locations and
attributes), spellings, recursive origins, physical opening bytes/edges and
injected authority agree for both units.

The final frozen gate is `multitu-full-1`: **3386 source pins**, exit 0.
The final fixture report retains **15 TU records across eight commands**:

| Command/control | Result |
|---|---|
| Original 104 inline manifest | Two complete independent checked records, in upstream command order. |
| Macro/counter/name resets | Two exact records; TU-local `T`, `Collision`, `collide` and `local` survive differing int/long environments. Two distinct `same.h` paths and repeated actual unguarded openings remain separate. |
| Definition-time snapshot | Two exact records, including an unused inline body calling a function before a later prototype narrows its parameters. |
| User reopening of `tccdefs.h` in both units | Two exact records; each has 21 injected bindings and 21 separately classified user asm carriers, with distinct implicit/user openings. This is syntax/arena evidence, not Supported/NoUB evidence. |
| Pack-layout control | Both model records survive and syntax/origins/openings agree with the separately labeled `Raw.run` component. `Raw.checked` reports `unimplemented-pack-layout`; its event-containing phase-four tokens are retained separately. |
| Cached include-guard control | Cabs, tokens and origins agree in both units. The model's actual opening graph has one guarded-header read per TU; independent raw records two. Exact opening transport remains obstructed here. No phantom model opening or graph normalization is introduced. |
| Early/late invalid units | One/two retained records respectively; ghost on/off status and console agree. Neither diagnostic counts as successful construction or an arena. |

All eight ghost on/off observations match status/console; the six compiler
handoffs also match complete 4 MiB arenas. Those six arenas independently match
the current image's fast RV64 reference. The two compiler diagnostics remain
separate. This task adds no native Sail observation of a multi-TU command and
does not reuse the historical 104 native capture under a different image.
Inherited full-gate native controls are retained separately.

The fresh independent 56-case suite has **51 exact Cabs agreements and five
explicit nonsuccesses**. The old inline-unused classification and all historical
counts/captures remain unchanged. Six bundle perturbation guards reject losing
an earlier unit, reordering units, or dropping earlier syntax, openings,
authority or source bytes.

`isolatedGhostSyntax` is shared by association and deferred-function replay.
`isolatedGhostSyntax_erasure`, the existing `associationReplayErasure`, and
`deferredFunctionErasure` prove local snapshot erasure, including parser
errors/shape guards, using `erase_syntaxFields` and only the three permitted
standard axioms. Replay uses scanner-owned saved words immediately at the
definition's scoped snapshot. Prefix GNU attributes are retained with modeled
specifier actions and attached to their declaration entities; the initial
104 companion failure was this attribute ownership gap.

`MultiTUConstruction_Statement` and `MultiTUErasure_Statement` remain open.
Their named fields require every ordered input, configured positioned stream,
origins/pragmas, checked raw result, openings/authority and complete Cabs to
survive a successful whole-command handoff, plus all erased compiler state
and diagnostics. Existing eleven ghost contracts remain open. The guarded
opening schedule and pack-layout cases above are concrete residuals, not new
source exclusions. F1/F2/F3/F6 still owe complete grammar/environment/transport,
fuel and binary connections; F4/F5 owe cross-unit linkage and semantic meaning;
F7 retains its library duties. No finite comparison supplies a universal theorem,
and the C11/Supported/Fits target and frozen sum cuts are unchanged.

The archive retains all failed receipts, source variants, superseded comparisons,
final observations and gate artifacts. Dead ends include denied local systemd,
three failed Lean builds (record layout/conversion), prefix-attribute omission,
a hard-coded companion lexing filename, the two independently exposed raw
residuals, and a packaging basename collision repaired by retaining full
relative gate paths. The first portable verifier also exposed revision
metadata outside the build-service suffix inventory; its exact postcheck pins
are retained as supplemental source blobs, with shared pins required to agree.
`tasks/DEAD-ENDS.md` records the details so these are not
reassigned. Git staging again cannot create this worktree's read-only
index.lock; its exact stderr is retained and the established scoped lead
commit handoff is used. Main adoption and ledger updates belong to the lead;
this worktree remains on
its reviewed base without a main merge or push.

Run `python3 results/model/multitu-ghost/verify.py`; `--sources` additionally
checks the exact final source snapshot. Evidence is
`results/model/multitu-ghost/evidence.tar.xz` with a per-member hash manifest.
