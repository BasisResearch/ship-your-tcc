# Proving tcc's compiler correct: design (no code yet)

**Status:** plan for review, 2026-10-20. Nothing here is built yet.

**Goal.** For every C program P in a stated subset, the code that tcc's RV64I
binary emits for P refines Certora's Lean Clight of P. This is one theorem
quantified over P, not a per-program check.

**Route.** It uses the sister projects' two layers, composed with the reset and
compile-phase machinery this repository already has:

* **Layer A** (binary ⊑ model): tcc's machine code, running on the Sail model,
  computes a Lean model of its compile function (`tccpp`, `tccgen`,
  `riscv64-gen`, and the parts of `tccelf`/`libtcc` that `-run` needs).
* **Layer B** (model ⊑ semantics): the model's output code, run on RV64I,
  simulates Clight. This is a CompCert-style simulation.

## 1. The theorem

```lean
theorem tcc_correct :
  ∀ src : List UInt8, S0 src →                 -- P is in the subset (decidable)
  ∀ elf, TccImage src elf →                    -- the fixed runtime, /prog.c = src
  ∀ c₀, ElfReset elf c₀ →
  ∀ cl, Elab src = some cl →                    -- Lean front end for S0 (§3)
  Refines (Sail.runFrom c₀) (Clight.behaviours Xbin cl)
```

* **`Refines`** is CompCert's backward simulation stated on observables.
  * If Clight's P terminates with printf trace `t` and exit code `r`, the
    machine halts with console `traceOut t` and exit code `r`.
  * If Clight's P diverges, the machine does not halt, or prints a prefix
    that grows without bound.
  * If Clight's P goes wrong (undefined behaviour), nothing is promised.
  * Every halting machine run is one of these.
* **`Xbin`** is the external-call semantics derived from the machine's printf
  (PHASES `PrintfSpec_Statement`, in progress).
* **`TccImage src elf`** generalises today's `TccSumElf`:
  * the runtime bytes are the fixed ones (hash `9e1142dc96d8db10`, built with
    `BUILD=build`);
  * the `.embed` region holds `src` as `/prog.c`;
  * `src` has a size bound (§8).
* **Link to Certora's Clight** of the actual `clightgen` output. That is an
  unverified OCaml program, so the universal theorem is stated against `Elab`.
  `Elab` is a Lean front end for S0 that produces CCLib `Clight.Program`s in
  the shape `clightgen -normalize` produces. For any given P, a cheap
  decidable check `clightgen(src) = Elab src` (a `decide` on two ASTs,
  seconds) transfers the theorem to Certora's Clight of P. The check also runs
  over the whole S0 corpus as a test of `Elab`.

## 2. What exists today, and what carries over

| piece | where | proves | role in the plan |
|---|---|---|---|
| reset facts | `Tcc/Boot/Startup` (ship-your-ocaml port) | `ElfReset` gives `GoodState`, loader memory, zero GPRs, idle HTIF | used as is |
| RV64I evaluator plus soundness | `Tcc/Ccp/{Eval,Sound}.lean` | a first-order RV64I run without faults gives Sail `Steps` (`run_sound`, 46 instruction kinds) | **the target semantics for Layer B** and the engine for Layer A's input-independent prefix |
| compile-phase run for one program | `Tcc/Ccp/Sum/*` | the 6.6M-step compile phase of `sum.c` | superseded by Layer A; kept as a regression |
| program-phase replay | `Tcc/Prog/Sum/Machine` | `sum_halts` | superseded by Layer B; kept as a regression |
| Clight side | `Tcc/Clight/*`, patched CCLib | `StepWith X`, `ExtDeterm`, printf model, `Prints` | the source semantics |
| syi WHILE→RV64 compiler | syi `Vsa/Compiler/` (83 files, 19.5k lines) | `compile_correct`: `BigStep ↔ Halts`, divergence included | **template for Layer B**: abstract asm `Ins`/`AM`/`astep`, the lift `Corr`/`sim_M`/`step_sim`, value relation `VRepr`, store relation `SRel`, invariant `MS`, per-construct `ESpec`/`SSpec` |
| function lifting | ship-your-lua `gen_lua_fn.py` (1.6k lines), `Lua/Vm/Sim/Kit/Fn.lean` | a machine function computes a generated Lean function `F`; then `F = spec` in pure Lean | **the core of Layer A** |
| `FnSummary`, `loopFromBody`, `callSeg`, `MachWP`, `SymExec`/`StepRules` | `Vsa/Sim/`, syi VsaIris (survivor copy in syi-expD) | generic block, loop and call composition at machine level | Layer A tooling |
| heap representation | ship-your-lua `ValRepr`, `Strs`, `DlHeap`, dlmalloc proofs | machine bytes in dlmalloc chunks represent Lean values | patterns for tcc's `TokenSym`, `Sym`, `CString` and section buffers |
| libgcc and soft-float proofs | ship-your-lua | `__muldi3`, `__divdi3`, … at machine level | S0's `* / %` call libgcc (patch 0001) |
| ocaml run kernel | `OCaml/Run/Kernel.lean` (`iter_transport`) | lockstep squares | lemma patterns |

Gaps nobody has filled:
* a simulation framework (`forward_simulation`, behaviours, divergence) over
  CCLib;
* memory injections (CCLib ports only the operational core of `Memory.v`);
* a semantics of the emitted code as a function of the code bytes, for code
  that is not known in advance.

All three are built here (§5, §6). CCLib's four `externalFunctionsSem`-family
axioms stay outside every theorem, as now (the patch-0002 `ExtEnv`
parameter).

## 3. The starting subset S0

S0 is chosen so that every construct goes through tccgen's general machinery
(the value stack, register allocation and spilling, jump chains, calls),
while avoiding the type zoo:

* **Translation unit.** One file, no preprocessor directives. printf is
  declared by a prototype, as `c/tests/p3/sum.c` does. This sidesteps `tccpp`'s
  macro engine and the 18-header include chains. The lexer, identifier table
  and keywords stay in scope.
* **Types:** `int` only (32-bit signed), plus `void` returns. No pointers,
  arrays, structs, char, unsigned, long, floats, bit-fields or varargs
  definitions.
* **Declarations:** global and local `int` scalars with constant initialisers;
  functions with at most 8 `int` parameters returning `int`. Recursion
  allowed.
* **Statements:** blocks, expression statements, `if`/`else`, `while`, `do`,
  `for`, `break`, `continue`, `return`.
* **Expressions:**
  * `+ - * / %`;
  * `& | ^ << >> ~`;
  * unary `-` and `!`;
  * comparisons, `&& || ?:` (jump chains);
  * `=` and compound assignment, `++`/`--`;
  * calls;
  * `printf` with a string-literal format using only `%d`, `%%` and
    literal bytes.
* **Semantics notes:**
  * CompCert's Clight makes signed `int` arithmetic modular, which matches
    tcc's wrapping code.
  * Division by zero, `INT_MIN / -1` and out-of-range shifts are undefined
    in Clight, so they need no machine obligation.
  * `* / %` call libgcc helpers under patch 0001, so S0 exercises the call
    ABI from the start.

After S0, in order:
1. **S1:** pointers, arrays, `&`, string literals as values, `char`.
2. **S2:** `unsigned`, `long`, `short`, implicit conversions.
3. **S3:** structs and unions, including by value (tcc's struct copying and
   the ABI).
4. **S4:** `switch`, `goto`, `sizeof`, enums, `static` locals.
5. **S5:** floats and doubles through the soft-float helpers, reusing
   ship-your-lua's proofs.
6. **S6:** the preprocessor (macros, `#include` of the shipped headers),
   which brings in `tccpp`'s macro engine.

## 4. The Lean model of tcc

**Shape.** One Lean function per C function in the reached part of tcc. Each
has the same name and control structure, and works over a record that
mirrors tcc's globals. The point is that Layer A becomes "the machine
function computes this Lean function", and Layer B reasons about functions
that read like tcc.

```lean
structure TccSt where
  src : Input               -- file buffer and cursor (tccpp's `file`)
  tok : Tok; tokc : CValue  -- current token
  idents : IdentTable       -- TokenSym table, as a finite map
  scopes : ScopeStack       -- Sym stacks: global, local, labels
  vstack : Array SValue     -- tccgen's value stack (bound VSTACK_SIZE = 512)
  loc : Int                 -- current frame offset
  ind : Nat                 -- code index in .text
  text : ByteArray          -- emitted code
  rsym : Nat                -- return jump chain
  nocode : Bool; constWanted : Bool
  ...

abbrev TccM := StateT TccSt (Except TccError)   -- tcc_error / longjmp = throw
def next : TccM Unit ...; def unary : TccM Unit ...; def gen_op (op : Tok) : TccM Unit ...
def gv (rc : RegClass) : TccM Reg ...; def get_reg ...; def save_reg ...
def block ...; def decl ...; def gen_function ...
def load (r : Reg) (sv : SValue) : TccM Unit ...   -- riscv64-gen: append encoded instructions
def compile (src : List UInt8) : Except TccError Output   -- .text, .data, symbols, relocations
```

* **Termination.** Recursion goes through fuel bounded by the token count, as
  ship-your-lua's generated functions do. A run out of fuel is an error and is
  excluded by the theorem's premise.
* **Executable and checked byte for byte.** Before any proof, a native build
  of `compile` is differential-tested against the real tcc on Sail:
  * the S0 corpus;
  * a Csmith-like S0 generator;
  * Phase 1's existing `@@TCCBM code` arena dump (`FLAGS=1`) supplies the
    reference bytes.

  The model must reproduce `.text` exactly. This catches modelling mistakes
  cheaply, and it is the gate for starting Layer A.
* **Ghost AST.** The model's parse functions also return the AST of what they
  parsed. `Elab` (§1) is defined as that AST mapped to Clight in
  `clightgen -normalize` form.
  * Layer B can then state each parse function's contract against the very AST
    it parsed, with no separate parser-agreement theorem.
  * The per-program `clightgen = Elab` check validates the ghost output
    against CompCert's front end.
* **Linking (`tcc_relocate` for `-run`)** is modelled as a function of the
  compiled output:
  * section layout in the arena;
  * the fixed set of libc members that printf pulls in;
  * relocation application.

  For S0 the member set is constant. Only the program's size and its symbols
  vary.

Expected size: 4–6k lines of Lean model for S0 (tccgen's S0 paths, the lexer,
`riscv64-gen`, linking), plus the ghost-AST and `Elab` code.

## 5. Layer B: the model's code simulates Clight

**Target semantics.** The code is the model's output, so it is known only
symbolically. Following syi:
* define an abstract asm: the RV64I forms `riscv64-gen` emits, plus calls to
  the libgcc helpers specified by their proved summaries;
* decode the emitted bytes into it (`decodeW` facts, as `Tcc/Ccp`'s code table
  does);
* lift abstract runs to Sail with the existing `Tcc.Ccp.run_sound`/`Sim`
  (46 kinds, already proved), or syi's `Lift`.

`riscv64-gen` emits a small instruction set: `addi`, `add`, `sub`, the logic
operations, shifts, `slt(u)`, `ld`/`sd`/`lw`/`sw`, `beq`/`bne`, `jal`/`jalr`,
`lui`/`auipc`. All of it is in the evaluator's 46 kinds.

**Simulation relation** for tccgen's one-pass value stack. At every point
where the model has emitted code up to `ind`, the relation between a Clight
state `(e, le, m)` and the machine `(regs, mem)` is:
* **Frame.** `s0` is the frame pointer. Each Clight local `x` (a block in `e`
  for addressable locals, a temp in `le` after `-normalize`) is mapped to its
  slot `s0 + off(x)`, read off the model's `Sym` for `x`. tcc keeps every
  local in memory, so temps and locals share one rule.
* **Memory.** `ι : Block → Option Addr`, an injection-lite built here:
  * globals map to their `.data`/`.bss` addresses after linking;
  * each frame's stack blocks map into the frame;
  * mapped bytes agree;
  * unmapped machine memory is constrained only by the frame and heap
    footprint.

  S0 has no pointers, so `ι` is never observed by the program. Its theory can
  wait until S1 and start small.
* **`VSMatch`** gives each `SValue` on `vstack` a Clight denotation `v`,
  according to where it lives:

  | location `r` | meaning |
  |---|---|
  | a register `a0`–`a7` (`r < VT_CONST`) | `regs r = v` (sign-extended for `int`, as `riscv64-gen` keeps it) |
  | `VT_CONST` | `c = v` (plus a symbol address, from S1 on) |
  | `VT_LOCAL` | the address `s0 + c`, an lvalue when `VT_LVAL` is set: the value is the bytes there |
  | `VT_LLOCAL` | a spilled lvalue whose address is stored at `s0 + c` |
  | `VT_CMP` | `(cmp_op, cmp_r)`, and `v` is that comparison of `cmp_r` |
  | `VT_JMP`/`VT_JMPI` | every jump in the chain `jtrue` (or `jfalse`), once patched, reaches the continuation with `v = 1` (or `0`) |
* **Register allocation** (`RegsOK`):
  * a register not referenced by `vstack` is free;
  * `get_reg` either finds a free one or calls `save_reg`;
  * `save_reg` moves every referencing entry to a new spill slot (`loc`
    decreases) and preserves `VSMatch`.

  This is the central invariant lemma, proved once for the model's `gv`,
  `get_reg` and `save_reg` (syi's `MS` plays the same role).
* **Back-patching.** tcc threads forward jumps through the immediates of the
  jumps themselves, and `gsym` patches them later. Contracts are therefore
  stated against the **final** `.text`.
  * Each emitted region is append-only.
  * Each chain is a list of positions whose final immediates point at the
    label the matching `gsym` placed.
  * One lemma per chain operation (`gjmp`, `gjmp_cond`, `gjmp_append`,
    `gsym_addr`) keeps this "chain denotes positions" invariant.

**Contracts follow the parser's recursion.**
* *Expressions.* Each parse function (`unary`, `expr_prod`, …, `expr_eq`,
  `gexpr`) has a contract: from a state matching Clight's evaluation context,
  the code it appends ends with the new top of `vstack` denoting
  `eval_expr`/`eval_lvalue` of the ghost AST it returns. Side effects (`=`,
  `++`, calls) are matched by Clight's evaluation-order steps; after
  `-normalize`, Clight expressions are side-effect free and calls are
  statements, which makes this tractable.
* *Statements.* `block` gets a star-simulation lemma per Clight statement, with
  `break`/`continue`/`return` as jump chains.
* *Functions.* `gen_function`, `gfunc_prolog`/`gfunc_epilog` and
  `gfunc_call` (the a0–a7 ABI) get a function-level simulation, with
  recursion by induction on Clight's step count.
* *Whole program.* The `main` entry composes with the `tcc_baremetal_enter`
  handoff, as the end-to-end theorem does today.

**Behaviours.** The plan proves a forward simulation for terminating and
diverging runs, then flips it to the backward `Refines`. The flip needs Sail
determinism (proved: `Halts.deterministic`) and Clight receptiveness under
`ExtDeterm X`. printf steps meet `Xbin`, the PrintfSpec result.

**Size.** syi's WHILE compiler took 19.5k lines: 153 per-rule simulation
theorems (5.0k lines), runtime routines 2.7k, the lift 0.3k, the rest
invariants and generated material. S0 is richer:
* C expression forms;
* a one-pass value stack with spilling, where syi has a direct translation;
* real ABI frames.

Estimate: 25–40k lines for S0, of which the memory and simulation framework
is 3–5k, reusable for S1 onwards.

## 6. Layer A: tcc's binary computes the model

**What runs.** `sum.c`'s compile phase executes 19,515 static instructions in
228 functions:

| part | instructions | functions |
|---|---|---|
| tcc's own code | 12.4k | 143 |
| ↳ tccgen | ≈4.0k | 53 |
| ↳ tccpp | ≈2.4k | 22 |
| ↳ libtcc | ≈2.1k | 16 |
| ↳ tccelf | ≈2.1k | 27 |
| ↳ code generator | ≈1.1k | |
| libc, libgcc, runtime | ≈7.1k | 85 |

(Attributed by symbol name; static helpers are folded into callers.) S0
programs reach more of tccgen: estimate 15–20k static instructions over
about 300 functions.

**Phases of the compile run.** Each gets a different tool:
1. **Input-independent prefix:** `_start`, newlib init, `tcc_new`, tcc's
   built-in defines, up to the first read of `/prog.c`. The run is the same for
   every program. It is proved **once** with the `Tcc/Ccp` evaluator, the
   bytes of `/prog.c` marked unknown: the evaluator faults if it reads one.
   That gives a lemma for all `src`, at about today's cost per step (1.2–1.7
   ms).
2. **Lex, parse, generate** (`tcc_compile`): genuinely input-dependent. Each
   reached tcc function is lifted ship-your-lua style:
   * `gen_lua_fn.py`-style generation of a Lean function `F_f` from the
     machine code, proved by the generator layer (`FnSummary`, `loopFromBody`,
     `callSeg`);
   * a pure-Lean proof that `F_f` equals the model's `f` under the
     representation relation.
3. **Linking and relocation** (`tcc_relocate`, archive loading for the printf
   members): the same lifting. The archive loading reads the same members for
   every S0 program, so much of it should fold into phase 1's
   once-per-image proof.

**Representation relation** (`TccRepr st mem`): the bulk of the hand work,
following ship-your-lua's `ValRepr`/`Strs`/`DlHeap`.
* `vstack` is a global array of `SValue`s.
* `TokenSym`s form a hash table of dlmalloc chunks.
* `Sym`s are linked stacks.
* Sections are buffers grown by `realloc`; the dlmalloc proofs are reused.
* `CString` token buffers.
* tcc's scalar globals (`tok`, `ind`, `loc`, `nocode_wanted`, …).
* `tcc_error` is `longjmp` to `tcc_compile`'s `setjmp`, mapped to the model's
  `throw`.

**Size.** Using ship-your-lua's measured rates (about 3.7 hand lines per path
after its kit, plus generated function bodies):
* 30–60k lines generated;
* 10–15k lines by hand for S0, half of it the representation relation;
* build time is dominated by generated per-function modules: several CPU-hours
  per full build, to be measured in phase C1's pilot.

## 7. Phases and exit criteria

Each phase reports its measurements in PHASES.md. Proof work keeps this
repository's laws: generators, no hand replays, the abstraction gate.

| phase | content | exit |
|---|---|---|
| C0 spec | S0 grammar plus the decidable `S0`; the `Elab` design; `TccImage src`; the theorem statement as a `def …_Statement` | statement elaborates; `clightgen = Elab` is decided on every S0 program of the corpus |
| C1 model | executable Lean model of tcc for S0, with ghost AST | byte-identical `.text`, data and relocations with tcc on Sail for the S0 corpus plus 500 generated S0 programs |
| C2 target | abstract asm for `riscv64-gen`'s forms, decoding of model output, lift through `Tcc.Ccp.run_sound` | lift proved; libgcc helper specs connected |
| C3 Layer B, expressions | `VSMatch`, `RegsOK`, the spill lemma, the jump-chain invariant, contracts for the expression parsers | `int` expressions with calls proved |
| C4 Layer B, statements and functions | `block`, loops, `return`, prolog/epilog/call ABI, recursion | `model_correct : ∀ src ∈ S0, Refines (asm run of (compile src)) (Clight of Elab src)` |
| C5 Layer A, prefix | input-independent prefix by the evaluator with `/prog.c` unknown | lemma for all `src` |
| C6 Layer A, compile | lifting plus `TccRepr` for tccpp's lexer, tccgen's S0 paths, riscv64-gen | `binary_computes_model` for `tcc_compile` |
| C7 Layer A, link | `tcc_relocate`, archive loading, the jump into the arena | handoff state = model's linked image |
| C8 compose | C4 + C7 + reset + PrintfSpec | `tcc_correct` for S0, axioms standard |
| C9+ | S1 … S6, each re-entering C1 (model), C3/C4 (Layer B), C6 (Layer A) for the new constructs | per-subset theorem |

**Order of work.** C0 and C1 come first and need no proofs. Then C2–C3
continue as a measured pilot: the expression fragment `+ - * <` with
register pressure forcing spills, in the style of the `CompileCut` pilot.
C5 can run in parallel with C2–C4, since it reuses the evaluator directly.

## 8. Risks and open questions

* **Resource bounds.**
  * tcc fails (calls `tcc_error`) when the 4 MiB arena, the vstack (512
    entries) or the heap runs out.
  * The theorem therefore carries a size premise on `src`, stated as "the
    model's `compile` succeeds" plus a simple length bound. The model
    reproduces tcc's own error checks.
* **tcc's global state and `nocode_wanted`.** Constant folding and dead code
  (`nocode_wanted`, `const_wanted`) make the vstack contract
  mode-dependent. It is modelled exactly, not abstracted.
* **Front-end mismatch.** `Elab` follows `clightgen -normalize`'s shape
  (temporaries introduced by SimplExpr, the order of side effects). The
  per-program check catches drift. Proving `Elab` equal to CompCert's front
  end is out of scope.
* **Layer A cost growth.** ship-your-lua's per-arm cost rose before its kit
  existed (47→64 lines per arm). The abstraction gate applies: the first
  three hand-lifted tcc functions must leave a factored kit.
* **Clight's memory model versus tcc's frame.** S0 avoids observable
  pointers. S1 is where the injection theory becomes real; it gets its own
  pilot.
* **Toolchain.** Clight is CCLib on Lean 4.34.0. The machine layer and
  generators are already on the same toolchain here.

## 9. Recommendation

**Build it, starting with C0–C1.** The executable model with byte-exact
differential testing is cheap, independently useful, and settles the model's
shape before anything is proved against it. Then run the C2–C3 pilot on
`int` expressions with spilling and decide the rest from its measured cost.

**Effort.** S0 at full strength (C0–C8) is on the order of 80–120k lines,
mostly generated, about the scale of one sister project's main lane.

**What the existing per-program pieces become.** The `CompileCut` and replay
proofs for `sum.c` become regression tests of the composed theorem. In
particular, `Tcc/Ccp`'s evaluator and its soundness become Layer B's target
semantics and Layer A's prefix engine.
