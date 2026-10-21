# Decimal binary128 literals (slot 2, 2026-10-21)

Recovery base: `177231d` in `worktree-agent-a05464165befb7074`. This changes
compiler literal parsing in patch 0004 and the corresponding TccModel path.
It does not close F3, establish Layer A, or prove IEEE rounding correctness.

## Change and provenance

The pinned xPack GCC 15.2.0/newlib 4.5.0.20241231 RV64I/lp64 library's
`_strtold_r` calls `_strtorx_l` with rounding mode zero, then returns 16
bytes of which only 10 were written. The recovery audit records that ABI.
The existing conversion/build helpers were searched first in ship-your-lua,
syi and the toolchain. The pinned library exports `_strtodg_l`, but its
113-bit nearest and 114-bit directed paths both failed exact tests below.
The upstream [binary128 wrapper proposal](https://sourceware.org/pipermail/newlib/2025/022105.html)
confirms the original format defect; using that wrapper alone is insufficient
with this pinned library.

Patch 0004 reuses the library's integer bignums (`_Balloc`, `_Bfree`,
`__i2b`, `__multadd`, `__pow5mult`, `__lshift`, `__mcmp`, `__mdiff`,
`__hi0bits`). Its original adapter parses an exact decimal integer/exponent,
constructs the exact rational, determines the binary128 quantum, computes
113 quotient bits with binary long division, and compares twice the
remainder with the denominator for ties-to-even. It handles gradual
underflow, carry across binades, overflow, and complete initialization of
all 16 bytes. The compiler handles unary minus separately, including -0.
There is no binary64 intermediate or reference-derived input.

The helper ABI was checked against installed `sys/reent.h` (`__ULong` is
32-bit on LP64, `_Bigint` fields), the pinned ELF's exported helpers and
disassembly, and [newlib's mprec declarations](https://chromium.googlesource.com/native_client/nacl-newlib/+/master/newlib/libc/stdlib/mprec.h).
No library machine proof was reused. No sister-project files were changed.
The public newlib `strtold` remains unchanged; this fixes compiler literals.
Allocation failure remains a resource error, subject to the existing Fits
heap obligation; no Supported exclusion or refusal escape was added.

`Tcc/Model/PpDir.lean` now calls `SF.ofDecimal SF.F128`. Its existing exact
rational model is independent of the compiler's bignum implementation.
The old x87 conversion, stack-byte state and `--ld-garbage` driver option
were removed. `model_diff.py` now builds the runtime before computing its
cache key and regenerates image symbols on every invocation, so a new
patch cannot silently compare against stale model symbol addresses.
`ref_arena.sh` no longer requests stack-padding logs.

## Validation

- `scripts/check.sh --static-only`: passes, including RV64I ISA (106k
  instructions, zero forbidden), forbidden tokens, discipline, abstraction
  gate and generator drift. Existing sum compile-cut files were not changed.
- `python3 scripts/gen_ld_literal_tests.py --check`: passes. All generated
  C edits came through this generator. Python syntax checks pass.
- `ld_boundary.c`: **40/40**, exit 0, on the patched image under rv64emu:
  226,481,879 total steps; 226,475,261 compile steps. GCC 15.2.0, compiling
  the same C11 input for RV64I/lp64 and running under rv64emu, also gives
  **40/40**, exit 0 (7,286 steps).
- Supplementary stress: **156/156**, exit 0, 573,734,073 total steps;
  573,717,803 compile steps. Reproduce with
  `python3 scripts/gen_ld_literal_tests.py --stress-out build/ld-stress.c`,
  then `MAX_STEPS=4000000000 scripts/ref_arena.sh build/ld-stress.c build/ld-stress.arena build/ld-stress.console`.
  Seed 20261021; 100 random decimal rationals plus tests around 11 binades,
  with exact moderate-exponent ties. Expected bits are calculated from the
  input decimal rational, never from an image, model, compiler or arena.
- Original reproducer, with `TCC_ARGS='"-w","-std=c11",'`: **`1 1.5 1.5`**,
  exit 0 on the final image under rv64emu (11,391,289 total steps;
  11,380,787 compile steps). **The final image also passes on Sail**, with
  exactly the same console, exit 0, total steps and compile steps; program
  steps are 10,477. This is a console/step check, not a Sail arena dump.
- The two original mismatching corpus inputs run successfully with the
  patched compiler and `-std=c11`: `t24_ldouble.c` (14,283,704 steps,
  compile 14,105,297), `t41_oldstyle_ldva.c` (12,625,586 steps, compile
  12,591,511). These are execution checks, not model arena equality.

Vectors include both sides of exact halfway cases with even/odd lower
significands, a carry to 2, decimal precision beyond binary64 and x87,
normal/subnormal and zero/subnormal transitions, overflow, signed zero,
and huge exponent text. Exact midpoint decimal spellings at binary128's
extremes exceed TinyCC's existing 1024-byte token buffer; these tests use
160-digit decimal rationals strictly to either side instead. No input
limit was changed.

## Lead validation

The worker sandbox denied the systemd user-bus connection, so the lead
built the unchanged source under the shared lock and 12 GB scope:
`lake build tcc_model` passed (60 jobs). Independent arena comparison:

```
python3 scripts/model_diff.py --no-report -j 4 tcc c/tests/model/ld_literal.c c/tests/model/ld_boundary.c
```

**45/45 identical arenas**, zero mismatches, model errors or refusals
(9 seconds wall with cached reference runs). This includes both former
mismatches, `t24_ldouble.c` and `t41_oldstyle_ldva.c`. Logs are in the
recovery worktree's `build/ld-model-lead-{build,diff}.log`.
A broader `all build/ld-stress.c` run is in progress; no final count is
claimed for it here. The fast runner remains unverified rv64emu; the Sail
output check above does not establish Sail arena equality.

## Coordination and proof limitations

Slot 1 owns patch 0003 and `-std=c11` harness defaults. Corresponding C11
model changes are the next F3 task.
Patch 0004 only inserts helpers immediately before `parse_number` and
replaces that function's `strtold` call. It applies after 0001/0002 here;
0003 is not present in this worktree, so combined application needs the
lead's check. No main-branch merge/push or STATUS.md/PHASES.md edit was made.
The result text above can be used in the lead's ledger update, retaining
the outstanding Sail arena and combined-patch checks.

Ghost `GNode` remains a lossy token/tree trace, not Cabs. Fuel adequacy and
exhaustion accounting remain open as in recovery-audit.md. There is no
proof of the C bignum adapter, its library helpers, or its equality with
SF.ofDecimal. These executable checks do not close those obligations.

## Dead ends

1. FPI `{113,-16494,16271,1,0}`: the exact decimal for
   `2 - 2^-113 - 2^-180` returns `3f8fffffffffffff ffffffffffffffff`,
   expected `3fffffffffffffff ffffffffffffffff`.
2. FPI `{114,-16495,16270,0,0}` plus guard/sticky rounding: 37/37 initial
   boundaries, then 153/156 stress. For long decimal expansions just below
   29 * 2^-15906, 35 * 2^4621 and 17 * 2^10265, the engine returns short
   significands with inconsistent exponent/precision. Those inputs are
   now in the permanent generated regression. Do not reuse either wrapper.
3. An uncapped build or reference-derived padding is not a substitute for
   the blocked capped model build and independent comparison.

## Environment handoff

`git add` was attempted for this task's explicit file list and failed:

```
fatal: Unable to create '.../.git/worktrees/agent-a05464165befb7074/index.lock': Read-only file system
```

The implementation is left as a worktree diff. No alternate index/object
store was used to bypass the read-only metadata. The lead must commit it
from an environment that permits Git writes, after review. The lead has now completed the capped model build and
targeted independent arena comparisons above.

Final artifact SHA-256 pins (the ELF's default embedded program is hello;
per-program `.embed` changes are excluded from the harness runtime key):

- patch 0004: `4dfcc192c23a777b1b335e9052a416956886b1071c3de1e8da403b5b194e221e`
- `c/build/base.elf`: `78a8f93af7a53fdcec2866d2b9059ec86858deac497487477c5c2b0f11cfdc63`
- generated `ld_boundary.c`: `5ddfd00d0cc6d4d1812dd51df0343c509892a8ee0d077ca24351d23282072905`
