# Proving `tcc -run` correct against our own C semantics: design (no code yet)

**Status.** Approved by the user 2026-10-20, with three fixes applied in this
revision (the headline without a refusal disjunct, the preprocessor theorem,
the non-vacuity witness). Building starts at F0; PHASES.md is the ledger.

**Framing.** This is ship-your-interpreter for C. `tcc -run` takes C source
and runs it, so it is an interpreter for C, and the theorem has the
interpreter shape:

> For every C source `P` with no undefined behaviour under **CSem**, the
> `tcc -run` binary on the Sail RV64I model, started on `P`, behaves as one
> of the outcomes CSem allows for `P`.

**CSem** is our C semantics. We get it by forking
[cerberus-lean](https://github.com/OathTech/cerberus-lean), the Lean 4 port
of Cerberus, which provides Cabs → Ail → Core elaboration and Core execution
generated from Cerberus's Lem model. Certora's Clight, `ccomp -interp` and
gcc become testing oracles only. The scope is the whole compiler: no subset
is an end state.

## 1. The theorem

```lean
theorem tcc_run_correct :
  ∀ src : List UInt8,                          -- the bytes of /prog.c
  CSem.NoUB cfgTcc src →                       -- no allowed execution has UB
  Supported src →                              -- decidable; excludes only the §5 list
  Fits src →                                   -- stated resource bound
  ∀ elf, TccImage src elf →                    -- the fixed runtime image, /prog.c = src
  ∀ c₀, ElfReset elf c₀ →
  ∀ obs, Sail.Observes c₀ obs →                -- any machine run's observation: console bytes,
                                              --   exit code, or divergence
  obs ∈ CSem.outcomes cfgTcc src
```

* **`CSem.outcomes cfg src`** is the set of observable outcomes our fork's
  semantics allows for `src`:
  * **Pipeline:** phases 1–4 → tokens → Cabs (our Lean grammar) → Ail →
    Core → Core execution under the concrete memory model.
  * **Nondeterminism:** every allowed resolution of unsequenced evaluation,
    unspecified values and allocation addresses (§3.4).
  * **Configuration:** `cfgTcc` fixes the implementation-defined parameters
    to tcc's RV64 lp64 ABI (§3.3).
  * **Observation:** stdout bytes, the exit status, or divergence with the
    output so far.
* **No refusal disjunct.** There is no escape for tcc refusing:
  * On a `Supported` program that `Fits`, a compile error, a resource abort
    or any output not in `CSem.outcomes` is a counterexample to the theorem.
  * A tcc bug that rejects valid C (or miscompiles it) refutes the statement
    and gets fixed as a patch (§10).
* **`Supported src`** is decidable on the parsed program (the Cabs from the
  fork's grammar, §3.1). It holds unless the program contains one of the
  constructs in §5's explicit, line-referenced list, each of which has no
  CSem meaning (inline `asm`, computed `goto`, …). Nothing else is excluded.
  * Optional lemma `unsupported_rejected`: on an unsupported program, tcc
    stops with a compile error (for the rows where tcc itself refuses) or the
    construct is reported; proved from the model where tcc's code does so.
* **`Fits src`** is a stated resource bound, not a loophole. Its parts:
  * *static* (decidable on `src`): the program's code and data fit tcc's
    static arena (4 MiB at 0x86400000), and tcc's own compile-time limits
    hold: the vstack (`VSTACK_SIZE = 512`), the heap tcc compiles in, the
    include depth, and the macro-expansion depth;
  * *dynamic* (a Prop over CSem's executions): every allowed execution stays
    within the machine stack (call depth times tcc's frame size) and within
    newlib's heap for its own allocations. CompCert's correctness is
    likewise relative to an unbounded stack; here the bound is explicit.

  Each limit is stated as a named field (Law 6), so a reader can see exactly
  what is assumed.
* **`Sail.Observes`** is the existing `Halts` (console bytes and exit code)
  extended with divergence: infinitely many steps, with the console as the
  limit of the output.

### 1.1 The preprocessor theorem

Translation phases 1–4 get their own theorem, composed with the main one:

```lean
theorem tccpp_correct :
  ∀ src, Fits src →
  TccModel.preprocess src = CSem.Pre.phases14 cfgTcc.pre src   -- token sequences
```

* `CSem.Pre.phases14` is the fork's preprocessing spec (§3.2): C11
  §5.1.1.2 phases 1–4 and §6.10, with tcc's predefined macros and include
  path as `cfgTcc.pre`.
* `TccModel.preprocess` is the model of `tccpp` (the token stream
  `next()` delivers to the parser). Layer A proves the binary computes it, as
  for the rest of the model.
* **Composition.** `CSem.outcomes cfg src` is defined as the outcomes of the
  phase-7 pipeline on `CSem.Pre.phases14 cfg.pre src`. The main proof works
  over the token sequence, and `tccpp_correct` connects it to raw bytes.
* **Checked before it is proved** against `tcc -E` and `gcc -E` on every
  corpus file and Phase 1's header programs (phase F2).

### 1.2 Non-vacuity witness

The premises are shown satisfiable by real programs, and the conclusion is
shown to be about a machine run that exists:

```lean
theorem tcc_run_correct_witness_sum :
  CSem.NoUB cfgTcc sumSrc ∧ Supported sumSrc ∧ Fits sumSrc ∧
  ∃ c₀ out, ElfReset sumElf c₀ ∧ Halts c₀ out 0 ∧ (.halts out 0) ∈ CSem.outcomes cfgTcc sumSrc
```

* `sumSrc` is `c/tests/p3/sum.c`; the machine run already exists
  (`tcc_run_sum_derived`).
* `NoUB` for `sum.c` is decided by running CSem exhaustively (it has no
  unsequenced effects, so the ND tree is small); `Supported` and the static
  half of `Fits` are decidable; the dynamic half is a bounded check on the one
  execution.
* More corpus programs are added as witnesses as each phase lands, starting
  with the Phase 1 programs (`hello`, `loops`, `structs`, `fnptr`, `printf`,
  `doubles`, `headers`).

## 2. Where the fork lives

* **Upstream pin.** `OathTech/cerberus-lean`, branch `mdd/cerberus-lean` at
  e7e9bb4 (checked 2026-10-20):
  * Lean 4.32.2;
  * about 65k lines of Lem model, of which about 45k are the
    frontend/Core/memory model the pipeline uses;
  * about 20k lines of hand-written Lean seams: the concrete memory model
    `CerbMem` (3.8k), the ND runner `CerbND`, the Cabs JSON import, fuel,
    and FS stubs;
  * the C parser is still OCaml: menhir `c_parser.mly` (2.6k lines) plus
    `c_lexer.mll`, imported into Lean as JSON.
* **Proposed location: `csem/` in this repository.** It is an import of
  upstream at the pin, keeping Cerberus's LICENSE, the LemLib notices and
  THIRD_PARTY_FILES. It gets rows in ATTRIBUTION.md and a `vendor/CSEM_REV`
  file. Our changes are ordinary commits on top.
  * The generated Lean from Lem is committed, so the build needs no opam/Lem
    toolchain. The Lem → Lean generation is the generator.
  * `scripts/csem_regen.sh --check` checks for drift when the opam toolchain
    (OCaml 5.4, the pinned lem-lean) is present. That follows Law 7, as
    `vendor/certora-export-patched` does.
* **Alternative: a separate fork repository,** e.g.
  `BasisResearch/cerberus-lean`, pinned as a Lake dependency. That needs the
  user to create the repository, so it is listed only as an alternative.
* **Toolchain.** The fork is ported from Lean 4.32.2 to the project's 4.34.0
  so that CSem, the machine layer and the proofs share one Lake project.
  Phase F0 measures this port.

## 3. The fork tasks

### 3.1 A C grammar in Lean (source → Cabs)

* **What.** Port Cerberus's menhir grammar (`parsers/c/c_parser.mly`,
  2,584 lines), its lexer (`c_lexer.mll`) and the typedef-name feedback
  (`lexer_feedback.ml`) to a Lean parser that produces the same Cabs. It
  replaces `CabsImport.lean`'s JSON import.
  * The parser is a total, fuel-bounded Lean function, with the grammar
    written as a recursive-descent parser over a token stream.
  * Cabs is already a Lean datatype in the fork.
* **Why it matters.** tcc's own parser (`tccpp`'s tokenizer and the
  parsing half of `tccgen`) is then inside the proof (§6.2). Without it the
  spec would start at a JSON file produced by OCaml.
* **Check.** Lean parse = menhir parse (Cabs equality, through the existing
  JSON printer) on every corpus file: Cerberus's tests, tcc tests2, the
  CompCert tests, Csmith, and our Phase 2 corpus.
* **GNU extensions tcc accepts.** Where their meaning is a desugaring to
  standard Ail, the grammar accepts them. The list is in §5.

### 3.2 Preprocessing semantics (translation phases 1–4)

* **What.** A Lean specification of C11 §5.1.1.2 phases 1–4 and §6.10:
  * line splicing;
  * tokenisation into preprocessing tokens;
  * directives;
  * `#include` over the image's read-only file system;
  * object- and function-like macros with the rescanning and hide-set rules
    (Prosser's algorithm);
  * `#` and `##`;
  * `#if` expression evaluation in `intmax_t`;
  * `_Pragma`.

  Its parameters are tcc's predefined macros (`tccdefs.h` and the
  target defines), its include search path, and `__FILE__`/`__LINE__`
  semantics.
* **Staging.** Layer B can begin over preprocessed text, so that phase F1
  does not wait on this.
  * In that stage the theorem is stated for `src` = the tcc model's
    preprocessed output.
  * Phase F2 replaces that with the real phases 1–4 and proves the tcc model's
    `tccpp` against this spec. The final theorem is over raw source bytes.
* **Check.** Spec output = `tcc -E` output = `gcc -E` output (modulo the
  predefined-macro set) on the corpus and on the 18-header programs of
  Phase 1.

### 3.3 Implementation-defined parameters for tcc's RV64 lp64

* **Where.** Cerberus keeps them in `implementation.lem`/`Implementation`
  and the fork's `[CerbGlobal]` readers.
* **What `cfgTcc` sets:**

  | parameter | value for tcc RV64 |
  |---|---|
  | `char` | unsigned (`CHAR_IS_UNSIGNED`, riscv64-gen.c) |
  | integer widths | `short` 16, `int` 32, `long` and pointers 64, `long long` 64 |
  | alignments | lp64 |
  | `long double` | IEEE binary128, 16 bytes, aligned 16 |
  | bit-fields | tcc's layout: `struct_layout` in tccgen.c, including `int` bit-field signedness and straddling |
  | enums | tcc's choice of compatible type |
  | signed shifts and narrowing conversions | wraparound (two's complement) |
  | `main` | argc/argv as tcc `-run` passes them (`gen_embed.sh`: `argv = {"--"}`) |
  | stdin | empty |
  | filesystem | the image's embedded read-only FS (§3.4) |
* **Check.** Every parameter is a reader, so the choice is data. One unit
  test per parameter compares CSem with a probe program run by tcc on Sail.

### 3.4 Known bugs and model gaps, fixed in our fork

* **pathleak.** CerbFS compared raw path strings (reported by Kiran; upstream
  hot-fixed it fail-closed on 2026-09-28 by refusing non-plain paths).
  * Our fork replaces CerbFS by a model of the image's actual read-only FS
    (`htif.c`'s embedded table), with proper path resolution: `.`, `..`,
    repeated `/`, absolute and relative paths.
  * A program can then open files as it can under `tcc -run`, and paths
    that resolve to the same file are the same file.
* **Native float seams.** `CerbFloat` maps C floats to Lean's native `Float`,
  whose operations are opaque to the kernel, so no proof can relate them to
  machine code.
  * The fork replaces it with a pure definitional IEEE 754 model for
    binary32, binary64 and binary128. The model is shared with, or proved
    equal to, the spec ship-your-lua's libgcc soft-float proofs use.
  * It is checked against native `Float` by differential tests.
* **Deterministic allocation addresses.** The concrete model's allocator is
  a bump-down cursor from `last_address` (`CerbMem.lean`, "allocator",
  impl_mem.ml:1247–1270).
  * tcc places objects elsewhere: its stack frames, the arena's data
    section, and newlib's heap.
  * The fork makes the allocator take an **allocation oracle**. Any fresh,
    aligned, non-overlapping address range is allowed, and it is part of the
    ND resolution, as in the PNVI-ae-udi model the fork already supports.
  * Programs that print or compare addresses then have every placement as an
    allowed outcome, tcc's included.
* **Native seams in the semantic cone.**
  * `lemSeq` forces unused `let`s natively.
  * The MD5 symbol digest is native (`native/md5.c`).
  * `failwithI` panics are native.

  The theorem cone must use only pure definitions. Each seam is replaced by
  its pure meaning, or shown to be outside the cone by the axiom/purity gate
  the fork already has.
* **The fork's registered deviations and open discrepancies** (VALIDATION
  §2b, TODO) are reviewed one by one. Each is fixed where it touches C
  meaning, or recorded with its ruling.

## 4. Layer structure

```
src bytes ──tcc binary on Sail──▶ handoff state ──generated code on Sail──▶ outcome
    │   Layer A: binary ⊑ TccModel            Layer B: code ⊑ Core(src)       │
    ▼                                                                           ▼
TccModel.compile src  ═══════════  CSem: phases1–4 → Cabs → Ail → Core ═══▶ outcomes
```

* **Layer A (tcc's binary computes `TccModel`).**
  * From reset with `/prog.c = src`, the machine reaches the handoff to
    generated code in a state whose arena holds exactly
    `TccModel.compileAndLink src`: code, data and relocated libc members.
    (On any input the binary computes the model, refusals included; on
    supported, fitting input Layer B shows the model does not refuse.)
  * `TccModel` is a Lean model of tcc, with one Lean function per C function
    in `tccpp`, `tccgen`, `riscv64-gen`, and the parts of
    `tccelf`/`libtcc`/`tccrun` that `-run` executes.
* **Layer B (the generated code simulates Core).**
  * Running the linked code from the handoff on RV64I produces an
    observation in `CSem.outcomes cfgTcc src`, given `NoUB`.
  * This is proved as a simulation between the code `TccModel` emits for the
    program and the Core program CSem elaborates from it.
* **Library calls.** A call to a C library function meets newlib's code in
  the arena on one side and Cerberus's libc meaning on the other.
  * Cerberus defines `printf` and the formatted family in `formatted.lem`,
    and its libc in C run through Core.
  * Each library function a program uses therefore needs a machine-level
    proof that newlib's routine implements the CSem meaning:
    * `printf`: the PHASES `PrintfSpec_Statement` work now in progress, to
      be retargeted from `Tcc.Clight.format` to `formatted.lem` by a
      pure-Lean equality on the conversions both cover;
    * `malloc`/`free`: dlmalloc against CSem allocation, from ship-your-lua's
      dlmalloc proofs;
    * string and memory functions: from ship-your-lua.
  * Library functions without such a proof stay as named
    `LibSpec_<f>_Statement` premises, listed in PHASES.md.

### 4.1 The Lean model of tcc

* **Shape.** One Lean function per C function, same names, over a
  `TccSt` record that mirrors tcc's globals:
  * the input file stack, the macro and token state, `TokenSym`s as a finite
    map;
  * the `Sym` scope stacks;
  * `vstack` (bounded by `VSTACK_SIZE = 512`), `loc`, `ind`, `nocode_wanted`,
    `const_wanted`;
  * sections, symbols, relocations.

  It runs in a state-and-error monad: `tcc_error`/`longjmp` is `throw`.
  Recursion is fuel-bounded by the input length.
* **Ghost output.** The parse functions also return the Cabs they parsed,
  for Layer B's parser agreement (§6.2).
* **Executable and checked byte for byte** against the real tcc on Sail.
  Phase 1's `@@TCCBM code` arena dump gives code, data and relocation bytes
  on every corpus program. This gates Layer A.

## 5. What falls outside: constructs tcc accepts that CSem gives no meaning

Only constructs whose meaning is machine-level, or that cannot run on RV64I,
are outside. GNU extensions that desugar to standard Ail are inside (§3.1).
Line references are to `vendor/tinycc/tccgen.c` at 43c7708.

| construct | where tcc handles it | why it is outside |
|---|---|---|
| inline assembly: `asm`/`__asm__` statements, top-level `asm`, asm labels on declarators | `asm_instr()` 7513–7514; `asm_global_instr()` 8820–8822; asm labels 8883 | meaning is machine code; Cerberus refuses it (contract D9) |
| labels as values and computed goto: `&&l`, `goto *e` | `&&` 6069–6078; `goto *` 7475–7482 | no Core counterpart: Core control transfer is to named save points |
| `__builtin_frame_address`, `__builtin_return_address` | 5915–5937 | expose frame layout; no C meaning |
| `_Atomic`, `__atomic_*` builtins | `VT_ATOMIC` 4868, 5311; templates 5551–5574 | tcc lowers them to libtcc1 routines that use the A extension, which traps on RV64I; the fork's Core has no supported concurrency; outside unless libtcc1 gets a single-hart RV64I implementation, which would then be a library call under §4 |
| `__attribute__((section, visibility, weak, alias))` effects beyond one translation unit | 477–503 | linker-level placement or binding; `tcc -run` of one file observes none of them, except `alias`, which needs a CSem extension (aliasing function symbols); listed so it is not silently accepted |
| `_Complex` | 4837–4838 | tcc itself rejects it; listed for completeness |

**Inside, by desugaring in the fork's grammar/Ail stage** (tccgen refs for
the model):
* statement expressions `({…})` (5765–5773);
* `typeof` (4958–4960) and `_Generic` (6092);
* case ranges `case a ... b` (7453, 7744);
* `?:` with the middle operand omitted;
* `__label__` (7280);
* VLAs (`VT_VLA`: 2772, 3559, 5234) and `alloca` (1704), as allocations with
  block or function lifetime;
* `__builtin_expect`, `__builtin_constant_p`, `__builtin_choose_expr`,
  `__builtin_types_compatible_p`, `__builtin_unreachable` (5861–5909);
* the `cleanup` attribute (3975, 4718): a call at scope exit;
* zero-length and flexible arrays, empty structs (size 0 in tcc), `void *`
  arithmetic as `char *`;
* K&R definitions, `_Static_assert`, `_Alignas`, `_Noreturn`;
* varargs (`__builtin_va_*` 5955–5999): Cerberus's Core gives variadic
  functions a meaning.

`Supported` (§1) excludes exactly the rows of the first table. Everything
in the second list is supported, so tcc refusing it is a counterexample.

## 6. Layer B: the generated code simulates Core

### 6.1 Target semantics

* The emitted code is known only symbolically, as `TccModel`'s output.
* Its semantics is an abstract RV64I: the instruction forms `riscv64-gen`
  emits, plus calls to libgcc helpers specified by their proved summaries.
* It is lifted to Sail through the existing `Tcc.Ccp.run_sound`/`Sim` (46
  instruction kinds proved, extended as `riscv64-gen` needs) or syi's
  `Lift`.

### 6.2 Parser agreement

* For accepted input, the Cabs `TccModel`'s parser returns as ghost output
  equals the Cabs of the fork's Lean grammar (§3.1), up to a stated
  normalisation, e.g. tcc's folding of constant expressions during parsing.
* One theorem by induction on the grammar connects tcc's one-pass parser to
  the spec's syntax.

### 6.3 The simulation relation

The relation holds at each point where tcc has emitted code for a prefix of
a function body. It relates the Core state (environment, continuation, the
concrete memory state with allocations and provenance) to the machine
(registers, memory). Its parts:

* **Memory: identity on addresses.** This is the reason for choosing
  Cerberus's concrete model. With the allocation oracle (§3.4) resolved to
  tcc's placements, each live Cerberus allocation sits at the machine
  address tcc gave it:
  * a stack slot `s0 + off`;
  * a global in the arena data section;
  * a heap block in dlmalloc.

  Its bytes agree with machine memory: Cerberus's abstract bytes (value,
  provenance, or unspecified) match any machine byte when unspecified.
  Pointer provenance is ghost; the machine sees only addresses. No memory
  injection is needed, unlike a CompCert-style backend.
* **Objects and temporaries.**
  * Core's `create` and `kill` for a C object match tcc's frame slot for
    that symbol (`Sym.c` offset), allocated by `loc` arithmetic.
  * Core's pure `let`-bound values match tcc's value stack.
* **`VSMatch`.** Each `SValue` on `vstack` denotes the Core value it was
  generated for, by location:

  | location | meaning |
  |---|---|
  | register `a0`–`a7`/`fa0`–`fa7` | the register holds the value |
  | `VT_CONST` | a constant or a symbol address |
  | `VT_LOCAL`/`VT_LVAL` | an object at `s0 + c` |
  | `VT_LLOCAL` | a spilled lvalue address |
  | `VT_CMP` | a comparison result held in `cmp_r` |
  | `VT_JMP`/`VT_JMPI` | a jump chain whose patched targets reach the continuation with 1 (or 0) |

  `RegsOK` and the spill lemma show `get_reg`/`save_reg` preserve `VSMatch`.
* **Sequencing.**
  * Core's `unseq` and indeterminately sequenced calls allow any
    interleaving. tcc evaluates operands and arguments in one fixed order,
    and that order is one allowed ND resolution.
  * `NoUB` excludes unsequenced races, so the choice is invisible.
* **Control.**
  * Core's `save`/`run` (loops, `goto`, `break`/`continue`, `switch`)
    match tcc's jump chains. Back-patching through `gsym` is handled by
    stating contracts on the final `.text`: a chain is a list of positions
    whose final immediates point at the label.
  * Procedures match functions: the lp64 call ABI, `gfunc_prolog`/
    `gfunc_epilog`, struct passing and return, and varargs (tcc's va_list
    layout against Core's variadic semantics).
* **Floats.** Core float operations under the definitional IEEE model
  (§3.4) match tcc's calls to libgcc soft-float helpers, through
  ship-your-lua's proved summaries.

### 6.4 How the proof is organised

* Core of `P` is `translate(ail(P))`, and tcc's code is `codegen(P)`. Both
  are structural in the program, so the simulation is proved per Ail
  construct:
  * for each expression and statement form, the Core that
    `translation.lem` generates is simulated by the code `tccgen` emits for
    it;
  * these compose by the structure of the program.
* Behaviours come from a forward simulation (termination and divergence)
  flipped to the backward inclusion of §1, using Sail determinism (proved:
  `Halts.deterministic`) and the ND-resolution argument above.

## 7. Layer A: tcc's binary computes the model

* **Input-independent prefix.** `_start`, newlib init, `tcc_new`, tcc's
  predefines, up to the first read of `/prog.c`. It is proved once with the
  `Tcc/Ccp` evaluator, with `/prog.c`'s bytes marked unknown: the
  evaluator faults if it reads one. That gives a lemma for all `src`, at
  1.2–1.7 ms of kernel time per step (measured).
* **The compile proper** (`tccpp`, `tccgen`, `riscv64-gen`) and **`-run`'s
  linker** (`tccelf`, archive loading, `tcc_relocate`): each reached
  function is lifted in ship-your-lua's two levels:
  1. `gen_lua_fn.py`-style generation of a Lean function from the machine
     code, proved by the generator layer (`FnSummary`, `loopFromBody`,
     `callSeg`, `MachWP`/`SymExec`);
  2. a pure-Lean proof that it equals `TccModel`'s function under `TccRepr`.
* **`TccRepr`** relates machine memory to `TccSt`, following ship-your-lua's
  `ValRepr`/`Strs`/`DlHeap`:
  * the `vstack` array;
  * the `TokenSym` hash table;
  * the `Sym` linked stacks;
  * section buffers grown by `realloc` (dlmalloc proofs reused);
  * `CString` buffers;
  * the macro-expansion token strings;
  * tcc's scalar globals.

## 8. Size

The size is estimated from the code sizes above and the sister projects'
measured rates. There is no subset discount: the whole compiler is in scope.

| part | inputs | estimate |
|---|---|---|
| fork: Lean grammar + lexer | menhir 2.6k + lexer | 6–10k lines |
| fork: preprocessing spec | C11 §6.10 | 4–6k lines |
| fork: ABI parameters, FS model, allocation oracle, definitional floats (binary32/64/128), seam removal, bug fixes | | 8–15k lines, plus a 4.34 port of about 65k lines of generated and hand-written code |
| `TccModel` (all of tccpp, tccgen, riscv64-gen, -run linking) | about 22k lines of C (tccpp 4.0k, tccgen 9.0k, riscv64-gen 1.5k, tccelf/libtcc/tccrun 8.1k, partly reachable) | 15–25k lines |
| Layer B (parser agreement, memory and value relations, per-construct simulation for full C, ABI, varargs, bit-fields, floats) | syi's WHILE compiler took 19.5k; CompCert's front and back end proofs are about 100k lines of Coq | 80–150k lines |
| Layer A (whole tcc: `tcc.o` is 75k instructions in 322 function symbols; without the assembler and debug-info code the reachable part is about 50–65k; plus the newlib/libgcc code tcc itself calls) | ship-your-lua: about 3.7 hand lines per path after its kit; 1.37M generated lines for the Lua VM | 300–600k generated + 40–80k hand lines |
| library specs (printf, dlmalloc, string functions; the rest stay premises) | ship-your-lua proofs | 10–30k lines |

**Total:** roughly 0.5–1M lines, mostly generated, and 150–300k by hand.
That is several times one sister project's main lane. Full builds will take
tens of CPU-hours; per-function modules keep peak memory under the 12 GB
cap.

## 9. Phases

Every phase is a step toward `tcc_run_correct` for all of C; none ends at a
subset. Phases run in parallel where they don't depend on each other.

| phase | content | exit |
|---|---|---|
| F0 fork in place | import `csem/` at the pin; port to Lean 4.34.0; build under our Lake with the 12 GB cap; keep the fork's differential lanes against the Cerberus OCaml oracle; `tcc_run_correct` stated as a `def …_Statement` over the fork's current pipeline (JSON Cabs) | the fork's own gates pass on 4.34; statement elaborates |
| F1 fork tasks 3.1, 3.3, 3.4 | Lean grammar; `cfgTcc`; FS model (pathleak fixed properly); allocation oracle; definitional floats; pure seams | Lean parse = menhir parse on all corpora; CSem added to `scripts/diff3.py` as an oracle and agreeing with `tcc -run` on Sail for every UB-free corpus program; differences are classified (tcc bug, fork bug, or parameter) |
| F2 preprocessing (3.2) | phases 1–4 spec with tcc's predefines; the model's `tccpp`; then `tccpp_correct` (§1.1) | spec = `tcc -E` on all corpora, including Phase 1's header programs; then the theorem |
| F3 executable `TccModel` | the whole compiler and `-run` linker, with ghost Cabs | byte-identical arena with tcc on Sail for every corpus program and 1,000 generated ones |
| F4 Layer B framework | Core small-step relation and outcome set from the executable semantics; memory and value relations; abstract RV64I and lift; libgcc helper specs | framework proved; first expression forms simulated |
| F5 Layer B, all of C | parser agreement; expressions (integer, pointer, float); statements and control; functions, ABI, varargs; structs, unions, bit-fields; initialisers; `switch`/`goto`; the GNU desugarings | `model_correct : ∀ src, NoUB → Supported → Fits → TccModel.compile src succeeds ∧ outcomes(asm run of it) ⊆ CSem.outcomes` |
| F6 Layer A | prefix by the evaluator; lexer and preprocessor; parser and codegen; linker; `TccRepr` | `binary_computes_model` for all `src` |
| F7 library | newlib printf ⊑ `formatted.lem` (retargeted PrintfSpec), dlmalloc ⊑ CSem allocation, string functions; the rest named premises | per-function theorems |
| F8 compose | A + B + preprocessor + reset + library; the witnesses (§1.2) | `tcc_run_correct` and `tcc_run_correct_witness_sum`, axioms ⊆ {propext, Classical.choice, Quot.sound} |

**Order.** F0 → F1 ∥ F3 (no proofs; both are differential-testing gates)
→ F4 pilot (one expression family with spills, measured like the
`CompileCut` pilot) → F5 ∥ F6 ∥ F2 → F7 → F8.

**What the existing per-program pieces become.**
* The `sum.c` theorems remain as regressions.
* The evaluator and its soundness become Layer B's target semantics and
  Layer A's prefix engine.
* The Phase 2 differential gains CSem as its reference.

## 10. Risks

* **Known counterexamples.** The statement is refuted wherever tcc
  disagrees with CSem on a UB-free program. Phase 2 already found one:
  the union designated-initialiser override (`results/repro/
  tcc_union_designator_override.c`, also in pristine tcc).
  * The plan fixes such tcc bugs as patches in `patches/` with their
    reproducers. The theorem is about patched tcc, as it already is for the
    RV64I backend.
  * F1's differential testing and F5's proofs will find more. That is the
    point of the exercise.
* **Fork maturity.** cerberus-lean calls itself a research prototype, tested
  differentially but not proved equal to the OCaml oracle.
  * CSem is the specification by definition, so oracle disagreement is a
    fork bug to fix (§3.4), not a proof obligation.
  * The fork's open discrepancies need rulings early (F0/F1).
* **The 4.34 port** of about 65k generated lines may hit elaboration-budget
  limits (Law 1). F0 measures it before any other fork work.
* **The ND-outcome formulation.** The executable runner explores choices.
  The proofs need a relational "allowed outcome" with the allocation oracle.
  Defining it in a form the simulation can use is F4's first deliverable.
* **Scale of Layer A.** ship-your-lua's per-arm cost rose before its kit
  existed. The abstraction gate applies from the first tcc function.
* **Library premises.** The theorem holds modulo `LibSpec` premises for
  library functions without machine proofs. PHASES.md lists them, and the
  set should shrink, not grow.
* **Floats.** binary128 `long double` needs a definitional model and
  libgcc's `__addtf3` family proved against it. ship-your-lua has double,
  not quad.
