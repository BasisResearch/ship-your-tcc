# Lane: ship-your-tcc (tcc -run on Sail, three-way differential with CompCert/Certora Clight, then a per-program end-to-end proof)

## Background
ship-your-interpreter (syi; GitHub BasisResearch/ship-your-interpreter, clone ~/Documents/code/syi) proves, in Lean 4 + iris-lean, that a C WHILE interpreter's bare-metal RV64 ELF refines a big-step semantics on the Sail RISC-V model (`endToEnd_refinement`). Sister projects run the same method on Lua 5.4 (~/Documents/code/ship-your-lua) and OCaml (~/Documents/code/ship-your-ocaml). Reuse their machinery: the bare-metal build (crt0, link.ld, HTIF, newlib), the Sail runner, the disassembly → segment generators, MachCSL, the boot witness, and the OCaml project's in-image file system. Read their README, PHASES.md and CLAUDE.md first.

TinyCC (https://github.com/Tiny-C-Compiler/tinycc-mirror-repository) has a script mode: `tcc -run prog.c` compiles the whole file into memory, relocates it and jumps in. It is a compiler, not an interpreter, so the reachable theorem is per program (translation validation), not for all programs.

Certora/clean (https://github.com/Certora/clean) is a CompCert 3.17 fork:
- `clightgen -lean` exports Clight to Lean 4 (they pin 4.32.1; syi is on 4.34);
- `export/` holds a hand-ported Lean small-step Clight semantics (`CCLib/`, ~15k lines, CompCert's block memory, IEEE bit-pattern floats), an interpreter proved sound (`doStep_sound`) and a total-correctness separation logic;
- external calls are axiomatised, and volatile accesses aren't covered;
- the port is not proved equal to Rocq's Clight; it was differential-tested on 516 programs against `ccomp -interp`.

## Phase 1: tcc -run on Sail (report numbers before going on)
1. Pin a tinycc revision from the mob branch, which has the riscv64 backend. Vendor it with its LGPL licence.
2. Build `tcc` for bare-metal rv64gc (tcc emits hardware floating point, unlike syi's rv64i): static, newlib, the HTIF console. The toolchain is ~/toolchains/xpack-riscv-none-elf-gcc-15.2.0-1. Script mode needs:
   - its include headers, `libtcc1.a` and newlib's `libc.a`, served from an in-image read-only file system (copy the OCaml project's approach);
   - a static code arena in place of mmap/mprotect;
   - symbol resolution against libc.a through tcc's archive loader, with no dlsym;
   - `fence.i` after relocation.
   Embed `prog.c` and the argv `tcc -run /prog.c`.
3. Run `hello.c`, then a few programs (loops, structs, function pointers, doubles, printf), on the same executable Sail model the sister projects use. Record the outputs and step counts: how many steps for tcc's compile phase, and how many for the program.

## Phase 2: three-way differential (cheap, and the first publishable result)
1. Build Certora/clean (CompCert 3.17 fork) targeting rv64 linux/newlib-compatible ABI for `clightgen -lean` and `ccomp -interp`, and build its Lean `CCLib` and interpreter. If Lean 4.32.1 vs 4.34 matters, use their pinned toolchain in a separate elan dir.
2. Corpus: Csmith programs restricted to the common subset (no volatile, no unsupported builtins, deterministic; check with `ccomp -interp` that they are free of undefined behaviour), CompCert's test/c programs, and tcc's tests.
3. For each program, compare three outputs: `tcc -run` on Sail, Certora's Lean Clight interpreter on `clightgen -lean`'s output, and `ccomp -interp`. Also run gcc -O0 natively as a sanity check. Classify every disagreement as a tcc bug, a Lean-port bug, a CompCert-frontend issue, unspecified behaviour (evaluation order and the like), or a harness bug. Minimise each one to a small reproducer.
4. Write results to `DIFFERENTIAL.md`: counts, categories, reproducers.

## Phase 3: one end-to-end per-program theorem
For one small P (a loop that sums and prints with printf), aim for a Lean theorem that `tcc -run P` on Sail produces exactly the outputs Certora's Clight semantics gives `clightgen(P)`. Two parts:
- (a) a captured-cut witness: tcc's compile phase reaches the jump into the generated code with code bytes C at a fixed address. Follow ship-your-ocaml's `WhileMin.loaded_fillZero` and its chunked log checks.
- (b) C refines the Lean Clight of P, using syi's machine program logic and generators. libc calls such as printf get machine-level specs, as ship-your-lua's `fwrite` proofs do, linked to Clight's external-call axioms by a stated premise.

Unproved parts are `def …_Statement : Prop` obligations listed in PHASES.md. Never use `sorry` or new axioms; check with `#print axioms`.

## Phase 4 (stretch): validate Certora's Lean Clight against Rocq's
For the same P, compile with Certora's `ccomp` to RISC-V and prove the output refines the Lean Clight of P on Sail. CompCert's Rocq proof already relates that output to Rocq's Clight, so each such proof is a kernel-checked agreement check between the Lean port and the Rocq original on that program, modulo CompCert's assembly model against Sail. State this claim precisely and no more strongly.

## Repo
Work in the local git repo ~/Documents/code/ship-your-tcc. Don't create a GitHub repo or push anywhere without approval: when Phase 2 has results, use Cove `status needs_you` to ask the user about publishing (BasisResearch/ship-your-tcc). Write README.md (plan and numbers), PHASES.md (obligation ledger and exit criteria) and CLAUDE.md/AGENTS.md (discipline).

## Machine rules (aws-dev, shared: 32 cores, 123 GB RAM, about 320 GB of free disk)
- Check `free -g` before big builds. If less than 25 GB is available, wait.
- Cap heavy jobs with `systemd-run --user --scope -p MemoryMax=30G …`. Run at most one `lake build` at a time.
- Keep disk use under about 60 GB. Delete your own build trees and Csmith scratch when done. Never touch other projects' directories.
- Other agents (Lua, OCaml, verus, 4CT) share the box. Don't kill processes you didn't start.

## Reporting
You are a Cove termling. Use the cove MCP `status` tool:
- `needs_you` for decisions only the user can make;
- `done` at the end of each phase, with a short summary of numbers, what worked and blockers.
If Phase 1 can't get tcc -run working on Sail after a real effort, stop and report the blocker.
