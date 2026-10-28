# Compiler-injected bindings (2026-10-28, candidate for lead review)

This checkpoint connects the actual `cfgTcc` tccdefs declarations to named
runtime identities. It retains all 21 located asm-label carriers, complete
C signatures, original spellings and diagnostic coordinates. This is binding
authentication and canonical symbol resolution; complete execution semantics
for all 21 declarations is **not** claimed. The final frozen full gate passes; its receipt and focused evidence are
recorded below.

The supplied compiler environment has full predefined/header byte pins from
`Spec.Pre.Tcc.ImageFS`. `compilerOpening` authenticates the actual initial
predefined source and the header's resolved include edge. Phase 7 marks only
that opening. Neither presumed `#line` labels, builtin prefixes nor ordinary
runtime-library membership supply injection authority. Including the very
same header from user input is a different opening. Changed bytes at the same
path, including an added `#line`, are a different configuration from the fixed
image and cannot claim its compiler bindings. Source and stripped image header
bytes are checked separately against the actual stripping generator and FS.

`InjectedBinding` retains the full declaration, identifier and carrier with a
named `compiler_binding`. Shared Lem desugaring checks the name/target pair.
`Supported` exempts only the authenticated pair on its declarator; generic
asm carriers and user labels remain excluded by DESIGN §5. Empty user input
still retains 21 bindings and has no asm exclusions (execution lacks `main`).
Ordinary `int main(void){return 0;}` executes successfully through raw
preprocessing, Cabs, Ail, Core, linking and the runtime.

External function declarations now share canonical symbols by target linker
name, with normal signature/redeclaration checks retained. The shared
translation proxy decision excludes actual definition **symbols**, preserving
user function bodies and distinguishing a static same-spelled definition from
the external runtime target. Calls, address-taking, stored function pointers
and equality therefore use the same existing function identity. There is no
call-site spelling substitution. CSEM-10's reserved-zero supply is preserved.
`alloca` is classified separately and is not entered as an ordinary runtime
alias; its function-lifetime intrinsic semantics remains an explicit duty.

The five existing location consumers receive runtime membership through the
same identity transport. A Core runtime position can retain its diagnostic
label while carrying resolved membership. The maintained consumer checks
continue to check complete location records, now including `compilerInjected`.

## Runtime inventory and remaining duties

The executable inventory queries the actual loaded Core name/body tables.
The checked raw configuration loads `std.core` and the RV64 implementation
file; it does not load the separately prepared historical whole-libc dump.
The following inventory concerns that actual configuration, not every
Cerberus runtime artifact present on disk.

| Injected target | Current runtime handling | Remaining obligation |
|---|---|---|
| memcpy, memcmp | existing Core proxy and memory operations | image-specific newlib refinement, byte identity, ABI/memory frames (F7) |
| malloc, realloc, free | existing Core proxy and checked allocation policy | dlmalloc refinement, placement/lifetime/ABI/memory frames (F7) |
| memmove, memset | canonical external target; no loaded body | supply library meaning (F1/F7), then machine refinement |
| strlen, strcpy, strncpy, strcmp, strncmp, strcat, strncat, strchr, strrchr, strdup | canonical external target; no loaded body | supply library meaning (F1/F7), then machine refinement |
| calloc, memalign, abort | canonical external target; no loaded body | supply library/termination meaning (F1/F7), then machine refinement |
| alloca | function-allocation intrinsic, separate from runtime aliases | implement function-lifetime allocation and target connection (F1/F5); never substitute malloc |

Missing-body errors are retained observations, not successful C executions,
new Supported exclusions, or a refusal escape. User definitions of canonical
targets execute their own bodies. No library machine theorem is reused in
this checkpoint, so no unverified byte-identity reuse is asserted. Lua's
abstraction inventory and mandatory task-shape table were read before work;
existing semantic identity/translation/memory machinery is reused.

New named open obligations in `Tcc/CSem/RawFrontEnd.lean`:

* `CompilerEnvironmentImage_Statement`: the supplied full bytes/identities
  describe the fixed image.
* `CompilerOpeningConnection_Statement`: independently captured F3 model
  openings agree in full bytes, resolved identity and include edges. The older
  ghost driver compares parser construction using **model-produced positioned
  tokens**; it does not call independent `Raw.checked cfgTcc`. Those component
  controls establish neither independent raw preprocessing agreement nor the
  new compiler-opening authority. The queued F3 task must additionally compare
  actual independent `Raw.checked cfgTcc`, retaining all 21 injected aliases.
* `BindingResolution_Statement`: actual successful raw elaboration resolves
  runtime aliases and any public declaration to the same canonical symbols.
* `RuntimeBindingCoverage_Statement`: every ordinary runtime alias has an
  actual loaded Core body. This remains open for the 15 targets above.

The parser/token/library/pragma transport Statements, whole-C construction,
allocation/address refinement, fuel adequacy and F6 binary refinement remain
open. No theorem is claimed from finite tests; C11, Supported/Fits and all sum
cut artifacts remain unchanged in scope and pins.

## Independent evidence and scope

The isolated OCaml overlay uses the same shared Lem binding/desugaring code
and a source-pinned Menhir carrier patch. Its limited GNU syntax addition
retains the fixed header's aligned integer argument and asm labels; it does
not claim general GNU grammar coverage. Its generated parser error table is
regenerated, not hand edited. Actual complete positioned streams are parsed
independently and full Cabs compared with only historical identity metadata
projected away. The explicit pinned opening witness transports raw-adapter
metadata to this comparison; it is not an independent preprocessing or F3
provenance proof. Both engines retain their documented independent floating
scope; the OCaml overlay does not certify the full local IEEE seam.

Fresh native observations run the **entire tcc compiler on Sail**, not just a
cross-compiled test program. Three binding-base image-pinned runs return 0:

| Probe | Compile steps | Program steps | GCC | Patched host tcc | tcc/Sail |
|---|---:|---:|---:|---:|---:|
| eight alias/call/pointer controls | 3,813,307 | 1,023 | 0 | 0 | 0 |
| definition of builtin spelling, public target call | 3,402,398 | 152 | 1 | 0 | 0 |
| local static/public runtime pointer distinction | 3,448,975 | 167 | 1 | 0 | 0 |

The two GNU differences are retained, not C11 bug claims or grounds to make
GCC a universal alias-identity oracle. All 553 patched compiler-source files
shared by the host and Sail source trees are byte-identical; the precise
compiler, runtime objects, ELF images and commands are pinned. This evidence
is for the binding-base compiler. The later independent main typedef-alignment
change is outside this worktree; combined current-main validation belongs to
the lead. No model Cabs supplies the spec input, and no sum cut is regenerated.

## Final validation and review artifacts

`bindings-next-full-2` completes `scripts/check.sh --csem` with exit 0 and
**3,318 unchanged frozen source pins**, verified again locally. The previous
full-1 gate also passed; full-2 freezes the lead-requested clarification of
component-parser versus independent raw-preprocessing agreement. Both receipts
and logs remain distinct. Generator drift checks reproduce all 235 generated
Lean files; standard axiom audits include all four new obligations and the
runtime inventory. Native floating is absent from both checked cones; the
inherited non-floating seam/totality obligations remain as documented.

The final gate's rebuilt executables and matching isolated OCaml engine
`ee71393d4b36a5ab01cc0a1f838985f669b57605e2e594b9c49b452ead472468`
produce these focused observations:

| Check | Final observation |
|---|---|
| Complete positioned Cabs | 40/40 exact against independent Menhir, retaining all 21 full binding records |
| Defined alias/redeclaration/pointer execution | 13/13 zero exits in Lean and OCaml |
| Declaration constraints | 3/3 expected semantic rejections, separately classified |
| User asm labels/spoofed locations/repeated header opening | 6/6 expected exclusions/rejections |
| Empty user input | 21 retained bindings, zero exclusions, explicit missing-main execution error |
| Missing library/intrinsic executions | 17 explicit gap fixtures covering all 15 missing library targets and alloca; not successful C executions |
| Header/opening authority | 6/6 controls, including changed signatures/targets/bytes, relabeling and ordinary library membership |
| Library identity | 24/24 methods and 5,580 complete consumer-location checks |
| Function redeclarations | 31 passing controls; inherited CSEM-11 remains separately observed |
| Raw frontend methods | 12/12 |
| Native full compiler Sail | 3/3 targeted binding-base observations; two independent GNU GCC differences retained |

`injected-bindings/evidence.tar.gz` contains 2,383 hash-verified files, including
actual complete raw Cabs/semantic captures, positioned streams, commands,
source and binary pins, oracle overlays/manifests, build requests/results/logs,
all three native ELF images and raw Sail streams, plus separate failed
observations. `runtime-inventory.json` records the actual loaded table.
`compiler-input-pins.json` and `tool-pins.json` pin the binding-base image,
553 identical patched compiler-source files, host compiler and native tools.

Run `python3 results/csem/injected-bindings/verify.py --sources` in this
candidate worktree to verify archive members, focused categories, receipts
and all final source pins without printing hash manifests. Lead owns the
subsequent main integration and STATUS/PHASES changes. No push or main merge
was performed.

## Dead ends retained

* Historical `bindings-lean-1/2/3` and `bindings-resume-lean-1/2` failures are
  preserved; they are not validation. The passing resume-3 receipt was checked
  against all 2,196 frozen inputs before continuation.
* The first new oracle invocation used a wrong runtime install prefix; it is
  a harness failure. Correcting it exposed actual ordinary-header GNU parser
  rejection, retained separately. The new carrier overlay repairs that parser
  gap without stripping declarations.
* The first native harness named a nonexistent local host compiler; its
  partial GCC run is not a Sail observation. Corrected runs use the pinned
  existing compiler read-only, with output only in this worktree.
* The first library consumer comparison expected the old JSON record without
  `compilerInjected:false`; complete record comparison correctly failed.
* A pre-overlay redeclaration check mixed current Lean with pristine OCaml's
  old spelling proxy. The maintained gate now selects the matching shared
  oracle; the failed comparison remains separate.
* The inventory helper build `bindings-next-lean-4` failed on `letI`/`do`
  syntax. The corrected `bindings-next-lean-5` passed with 3,318 frozen pins;
  no budget or executable substitution was used.
* The added memmove/memset probe found missing memmove in both semantic
  engines. It remains runtime coverage work; equal errors are not semantic
  agreement. The static/builtin GCC differences remain independent evidence.

## Ledger suggestions for the lead

STATUS/F1: adopt authenticated compiler opening and all 21 retained binding
records, canonical runtime aliases, user asm exclusion repair and exact focused
observations after the frozen gate. Distinguish the five loaded proxies from
15 missing runtime bodies and the separate alloca intrinsic duty.

F3/F6: add the compiler-environment/image and independent model-opening
connection Statements. Existing ghost counts concern parser construction on
model-produced positioned tokens, not independent raw preprocessing. Add a
true independent `Raw.checked cfgTcc` comparison and the new authority
connection; retain the useful component controls separately. Keep the six ghost Statements and physical provenance work.

F7: add runtime-body coverage plus per-target machine refinements from the
inventory. Library instruction-byte equality, ABI/resources and memory/stdio
frames must precede reuse. F8 remains open, with no refusal disjunct or
additional source restriction. Keep CSEM-11 and GNU layout/array constraints
as independent queued tasks. Lead owns STATUS/PHASES changes.
