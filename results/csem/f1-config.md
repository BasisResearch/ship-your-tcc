# F1: patched TinyCC RV64I/lp64 implementation configuration

Recovery resumed 2026-10-27 from checkpoint `f7c5019` (unvalidated).
The configuration is `CerbImplCfg.tccRV64`, supplied through the Lem instance
reader `impl_params`; `Tcc.cfgTcc` supplies that reader to both Core library
parsing and Cabs → Ail → Core → execution. `cerberusDefault` remains an explicit
entry-point choice for the upstream differential lane. The TU Digest reader
is retained, with no native MD5/global/barrier reinstated.

## Source and generation

Read-only recovery source:
`.claude/worktrees/agent-a4ab6bbd8c0be5325` in the main checkout.
Reviewed `CerbImplCfg`, `lean_impl.lem`, `CerberusImpl`, `CerbDecode`,
`CoreParser`, `CerbMem` and the probes. The source reader changes were
recovered by the checkpoint; its generated copies were independently
regenerated. No diff3 result corpus was copied. This work adds the missing
reader integration checks, fresh image measurements and explicit residuals.
`ATTRIBUTION.md` records provenance.

Generator: `scripts/csem_regen.sh` (Lem pin
`2d3a492758cb23dc4e417f2961983d25b36ce130`, `vendor/CSEM_REV`). Every generated
change comes from this recipe; generated whitespace is checked by regeneration.
The sister-project `scripts/abs_inventory.sh` was run before proof changes,
and its mandatory tool table was read. This task has no machine-run proof
shape to copy. Existing local proofs are generalized by name:
`CerbMem.layout_stable_aux`, `step_eval_pexpr_measure_sufficient`,
`hack_measure_sufficient` and `CerbND.print_eval_conv_aux_wrapper_defeq`;
their constructions are retained. No machine-library proof is copied, so
there is no new library instruction-identity claim. No Lean elaboration
limit is raised.

## Commands and evidence

Build requests/results are local handoff files under `build/worker-build.*`.
Each request pins the Lean/Lem/runtime source files by SHA256; those sources
stay frozen until the lead returns a matching result. The lead runs the exact
Lake targets under the shared `SYT_LAKE_LOCK`, after `free -g` shows at least
25 GB available, with `systemd-run --user --scope -p MemoryMax=12G`.
No uncapped fallback is used.

```
scripts/csem_regen.sh --check
scripts/check.sh --static-only
python3 scripts/cfg_probes.py --tcc-only --jobs 6
python3 scripts/cfg_probes.py --tcc-only --jobs 2 p13 p14 p15
# After a successful capped build of the current sources:
python3 scripts/cfg_probes.py --csem-only --jobs 4
scripts/csem_gates.sh --no-build
```

Per-probe JSON in `results/csem/cfg-probes/` records UTC measurement time,
source hash, exact command, complete stdout/stderr/verdicts, compile/program
steps and image input pins. New measurements additionally pin the actual
ELF, cross compiler, RV64I multilib archives, runtime objects and embedded
file contents. CSem records the binary hash. Refusals, timeout, panic and
non-Defined branches are residuals, never counted as agreement. Matching
requires exact stdout and exit, with the host's low-eight-bit exit projection
spelled out; full CSem uint32_t exit results are retained.

Image: vendored TinyCC `43c7708` plus patches 0001–0004, built with
`TCC_RV64I_SOFT`, xPack GCC 15.2.0 and newlib 4.5.0.20241231,
`-march=rv64i -mabi=lp64`. Embedded compiler arguments select `-std=c11`.
There is no sum compile-cut regeneration.

The base image SHA256 is
`03e2009ad4fae44bfd3b7eb8ab44e1da619275b9d31168fe85e65f743b63bfae`;
`htif.o` is
`8429dd1c48a8a05da22589272d84c2d02ebfd97e269140de041a08412e2e77b1`.
`cfg-probes/exit-wire.disasm` records the image's `_exit` instruction words:
left-shift a0 by 32, logical right-shift by 31, set bit 0, store to `tohost`.
This is `((uint64_t)(uint32_t)code << 1) | 1`. The fresh positive/negative
probes check `305419896 → host 120` and `-1 → host 255`; full 32-bit wire
encoding is established by this source/disassembly audit, not observed
through the host's eight-bit process status. All 81 configured errno names
were independently compared to the installed `sys/errno.h`, with zero
value differences (including EWOULDBLOCK = EAGAIN = 11).

## Fresh measurements (2026-10-27)

All 15 tcc executions reached the traced program exit; all image/source pins
are fresh. There are **10 exact agreements, one defined mismatch and four
residuals**. `cfg_probes.py` is a measurement/classification command: its
process exit is not a claim that every semantic comparison passes.

| Probe | Measured tcc/Sail result | CSem result |
|---|---|---|
| p01 char | 200/255; CHAR_MIN=0, CHAR_MAX=255; unsigned promotions | exact |
| p02 widths | short/int/long/long long=2/4/8/8; object/function pointer=8; size_t/ptrdiff_t=8; wchar_t=4 | exact |
| p03 align/layout | short/int/long/long long/pointer/float/double=2/4/8/8/8/4/8; all eight struct sizes/offsets in JSON | exact |
| p04 long double | size=16, align=16, offset=16, precision=113, distinct from double | size/align/offset agree; precision=53, third indistinguishable from double; **mismatch** |
| p05 bit-fields | plain int signed; sizes=8/4/8/4/16; bytes=55 55 01 00 | desugaring error at first bit-field |
| p06 enum extensions | sizes=4/4/4/4/8/8; positive compatible type unsigned | C11 constraint violation on 0x80000000u enumerator |
| p07 casts/shifts | -56/4464/-294967296/-1; shifts -4/-1/-5; unsigned char=253 | exact |
| p08 argv | argc=1, argv[0]=/prog.c, argv[argc]=NULL | exact |
| p09 stdin | EOF and feof=1 | inherited CerbFS refusal on read fd 0 count 1024 |
| p10 binary32/64 | sizes=4/8; float rounds at 2^24; double does not; thirds differ; conversions=2/-2/1000000000000000000 | inherited binary64 bit representation fails four-byte store (panic) |
| p11 errno/exit constants | EDOM=33, ERANGE=34, EILSEQ=138, ENOENT=2, EINVAL=22; success/failure=0/1; errno starts 0, writable | exact |
| p12 pointer representation | null=0, width=64, high-bit integer/pointer/integer round trip | exact |
| p13 positive exit | stdout=305419896; host=120; CSem uint32=305419896 | exact under host exit projection |
| p14 negative exit | stdout=-1; host=255; CSem uint32=4294967295 | exact under host exit projection |
| p15 C11 enums | size=4/4, align=4/4; positive enum unsigned, negative enum signed | exact |

C11-only probes: 10 exact, one arithmetic mismatch and three residuals.
p06 measures tcc's extended enum behavior separately; no joint rejection
or extension constraint error is counted as successful C11 compilation.
Raw stdout and every CSem branch are retained in JSON. ASLR-dependent native
panic backtraces are diagnostic evidence, not semantic output agreements.

## Residual obligations (no Supported/Fits change)

* **IEEE execution and memory:** `format_fty` exposes binary32/64/128 data,
  but the inherited `FloatingValue` is native Lean `Float` (binary64).
  `opFval`, `strFval`, `fvfromint`, and `ivfromfloat` remain native/binary64;
  `memValueToBytes` writes its binary64 bits using the configured size,
  and `reconstructValueAbst_lemFuel` reads through `Float.ofBits`. Merely
  changing sizes does not implement binary32/128. Port the definitional
  IEEE value/rounding/conversion/byte model, including signed zeros, NaNs,
  infinities and decimal literals; then rerun p04/p10. Do not truncate a
  binary64 encoding to claim binary32 support.
* **Bit-fields:** `bitfields := .tcc` records the required policy, but
  `desugar_struct_declarator` still reports `Desugar_NotYetSupported
  "bit-fields"`. Layout, lvalue load/store, sign extension, zero-width
  barriers, storage order and straddling need Ail/Core representation and
  lowering. Do not enable the upstream ignore-bitfields switch. p05 is a
  concrete C11 reproducer and remains supported by the headline contract.
* **Enums:** p06 deliberately includes enumerators outside `int`, which
  C11 §6.7.2.2p2 constrains; p15 isolates C11 enums. The compatible-type
  reader follows tcc's 32/64-bit rule; extended enumerator parsing and
  typing are distinct from measuring that rule.
* **Allocation:** the reader carries pointer width and the executable run
  uses the full 64-bit cursor, but deterministic bump allocation is still
  only one resolution. The recovery's allocation-oracle source remains
  untouched in its read-only worktree for the later task. No allocation
  nondeterminism theorem is claimed here.
* **Filesystem/stdin:** no FS source is changed. The current initial state
  is inherited. The lead's `Config.fsFiles`/image filesystem integration
  remains owed; this change leaves its integration point available.
* **Header/library integration:** these probes use the fork's current
  headers in the OCaml Cabs frontend. This does not validate replacing them
  with all embedded image headers. Source audit also finds inherited
  `csem/runtime/libc/include/float.h` with LDBL_MANT_DIG=64 (image binary128
  requires 113), and `wchar.h` with `typedef short wint_t` (the image's
  `__WINT_TYPE__` is `unsigned int`). These are concrete follow-ups for
  the header/preprocessing and library integration, beyond the basic type
  readers measured here; no wide-character agreement is claimed.
* **Existing semantic seams:** partial Core/frontend parsers and native
  floating operations remain inherited. Passing fork purity/census gates
  does not certify the entire semantic cone as pure.

## Validation

`sol-2-rv64-config-20261027-04` passed the capped build (632 jobs), with all
request source hashes verified by the lead. Targets were `LemLib`,
`LemLibTests`, `CerberusLean`, `CerberusLeanTest`, `csem_run`, `TccCSem`, and
all 19 fork unit executables (the list in `scripts/csem_gates.sh`).
`free -g` before the first build showed 92 GB available (85 GB before the
second build handoff). The fourth build needed only the remaining statement
module (5.2 s); the preceding successful reader/proof/driver jobs were cached
from the same capped validation sequence.

Full `scripts/check.sh --csem` passed under request
`sol-2-rv64-config-20261027-05-gates` (exit 0, source hashes verified):
ISA, forbidden tokens, generated sources, discipline, abstraction gate,
serial capped project/replay builds, project/Certora audits, all 19 fork
executables, purity and totality plus their plants, zero-axiom source census,
execution-entry and fuel-contract axiom audits, generator drift (230 files
identical), and the 113-file default-configuration oracle lane (90 MATCH,
18 UB_MATCH, 5 CERB_SKIP; all as baselined). The CSem statement, verdicts,
NoUB/outcomes and Supported decision depend only on propext,
Classical.choice and Quot.sound. This validates the existing fork gates,
not completion of the residual semantics listed here.

`config-validation.log` retains the gate summaries and final axiom rows;
`config-validation.json` records commands, exits and hashes. The full local
log is `build/worker-build-sol-2-rv64-config-20261027-05-gates.log`. All 1495 requested source hashes were rechecked
after the gate; the canonical sorted source-map SHA256 is
`c0706f3a5dbd1dbf6bb664277426f3f105a9750b606f11861d2ed739b13e37e4`. The cumulative source comparison against `f5a8c17`
(pre-checkpoint) finds no new partial/axiom/unsafe declarations or
extern/implemented_by attributes in changed semantic Lean files. This is
a source-change census, alongside the scoped fork audits, not a complete
purity theorem.

## Build failures and dead ends for the lead

The first capped build found missing `CerbImplCfg.Impl` binders in
`Core_eval_lemMeasureProofs`, `Defacto_memory_lemMeasureProofs`,
`Driver_lemMeasureProofs`, CerbMem's joint layout stability lemma and
`Unit.TranslationFuelTest`; the print-conversion wrapper's explicit `@`
application needed the reader argument, and the enum test needed the
qualified target representation name. Fix the handwritten sources and
regenerate; do not patch the generated files or use a global default
instance to hide these failures.

The second build exposed the remaining explicit-call pins in
`Unit.FuelExemplar` and the driver, plus missing local test instances.
A section-level implementation instance also leaked into unrelated
`pull_constrained`/`has_concurRead`/`find_array_index` sufficiency lemmas:
use explicit binders only on the ABI consumers. Do not repeat the broad
section-variable approach. The Part 1 wrapper pins in `TotalityProofTest`
are regenerated by `csem/scripts/gen_fuel_parametricity.py --emit`, including
the digest and implementation reader arguments in generated binder order.
In the mutable parse-core loop, scope the implementation instance inside
the pure parser call; a term-level `letI` over the loop body breaks the
mutable statements' do syntax.

## Suggested ledger text

F1 cfgTcc reader recovery: RV64I/lp64 parameters replace the provisional
x86/default configuration. Source/generator integration, fresh focused
measurements and validation are recorded here. IEEE binary32/128 execution,
bit-field lowering, allocation oracle and image-FS integration remain
explicit obligations; whole-compiler Supported/Fits and the no-refusal
headline are unchanged. Do not mark F1 as complete on these measurements.

## Commit handoff

After all validation passed, `git add -A` failed (exit 128):
`Unable to create .../.git/worktrees/sol-2/index.lock: Read-only file system`.
No alternate index/object-store workaround was used. The validated worktree
diff is left for the lead to review and commit. No push or merge was attempted;
STATUS.md and PHASES.md were not edited. The suggested ledger text and the
unsuccessful integration approaches above are supplied for the lead.
