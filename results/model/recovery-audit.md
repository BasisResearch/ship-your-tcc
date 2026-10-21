# F3 recovery audit (2026-10-21)

Recovered branch `worktree-agent-a05464165befb7074` at 11481e7, including
six previously unmerged commits and the unsigned folding fix, provisional
ghost instrumentation, corpus generator and focused model tests in c787b3d.
`systemd-run --user --scope -p MemoryMax=12G lake build tcc_model` passes
(58 jobs). This is executable-model progress, not a proof or completed F3.

The audited harness no longer supplies reference-derived `--ld-garbage` to
the model. Matching refusals now have their own category and do not count
as identical arenas; mismatches and harness/model errors give a failing
exit status. The diagnostic driver option remains available, but is never
used for equality evidence by `model_diff.py`.

The checked pre-C11-patch image gives 398 identical arenas, 2 mismatches
and 1 matching refusal on 401 programs (`f3-diff.md`). The seven tests2 asm
failures remain outside Supported; the old run with reference padding gave
536 matches out of 543, including matching refusals, and is **not** valid
F3 exit evidence. Four inherited Sail spot rows compare compile counts,
console and exit only, not every arena byte. The fast reference is
unverified `rv64emu`; the approved exit still requires Sail arena equality
across the corpus and 1,000 generated inputs.

## Concrete runtime defect: decimal binary128 literals

`c/tests/model/ld_literal.c` compares `1.5L` to `0x1.8p0L`. Its expected
console is `1 1.5 1.5`, exit 0. On this image:

```
scripts/ref_arena.sh c/tests/model/ld_literal.c build/ld_literal.arena build/ld_literal.console
0 0 1.5
rv64emu: steps=11408534 compile_steps=11401181 exit=1
```

The pinned ELF's `_strtold_r` at 0x8004bd80 allocates 32 stack bytes,
passes the stack pointer as destination and rounding mode 0, calls
`_strtorx_l` at 0x8004bf0c, then loads all 16 result bytes into a0/a1.
That callee writes x87 80-bit output, while RV64 lp64 long double is
binary128. The final six bytes are prior stack contents. This explains
the existing model's oracle and the two unaided arena mismatches
`t24_ldouble.c` and `t41_oldstyle_ldva.c`. Fix needs proper 113-bit decimal
rounding; rounding through binary64 or zeroing padding is insufficient.
No corrective patch has yet been written. `strtodg` machinery is already
linked as `_strtodg_l`; a proper binary128 wrapper is a possible route,
subject to checking the exact pinned newlib ABI/source.

## Remaining model gaps

- Pending patch 0003 C11 changes must be mirrored, and `-std=c11` added to
  model option parsing and every differential invocation. Current model
  defaults to C99 and rejects the option.
- `GNode` is a provisional, lossy token/tree trace, not Cabs: types, string
  contents and identifiers are not preserved in a form establishing parser
  agreement. F1's grammar must supply the actual ghost target.
- Fuel is `16 * total input file-system bytes + 4096`, used per loop and
  recursion nesting. Its adequacy is not proved; several bounded loops
  can finish without reporting exhaustion. This needs explicit exhaustion
  accounting before a total correctness contract.
- `AsmExpr.lean` and `AsmRiscv.lean` remain untracked inherited WIP outside
  the executable import closure; they were not reviewed or built. Inline
  asm is excluded by Supported and should not distract from the C11 work.
- Linker-script loading and certain CLI options remain explicit model
  errors. The whole compiler endpoint remains open.

No sum compile-cut chain was regenerated. No library machine proof was
reused. The inherited model is a manual translation of patched TinyCC;
its provenance is LGPL-2.1 as TinyCC and needs the root attribution row
when merged.
