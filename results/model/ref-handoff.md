# Fast F3 reference: stop at compiler handoff

2026-10-27; worktree `sol-2-reference`, based on
`3593922d1a3f891985e710787821331e9b7e62b9`.

`rv64emu --stop-at-handoff` stops with PC equal to the traced
`tcc_baremetal_enter` symbol, before executing its first instruction.
Compilation/relocation has completed, but generated-program execution has not
started. `ref_arena.sh` now uses this mode. This removes program termination
as a prerequisite for collecting a compiler arena.

The original emulator actually dumped at `tcc_baremetal_run`, before the
wrapper leading to `tcc_baremetal_enter`. Ordinary runs still dump there
and execute the program. In the new mode, entry/code metadata is saved at
`tcc_baremetal_run`, then the arena is written only when execution reaches
`tcc_baremetal_enter`. `compile_steps` still counts to
`tcc_baremetal_run`, preserving the Sail runner's accounting. The tested
FLAGS=0 wrapper takes 25 instructions and leaves the arena unchanged.

A handoff run returns host status 0 and reports:

```text
rv64emu: steps=<n> compile_steps=<n> exit=- status=handoff
```

This is compiler completion, with no generated-program exit code. Ordinary
completion retains the numeric parser prefix
`rv64emu: steps=... compile_steps=... exit=...`, followed by
`status=exit`, `status=limit`, or `status=trap`. Access faults retain
their diagnostic and status 126; they do not produce a completion record.

Arena bounds and entry/code metadata are checked before serialization;
header, payload and close failures produce status 2 without a handoff record.
The shell stages the arena privately, requires emulator status 0 and exactly
one handoff completion record, validates the format and payload length, then
publishes it. Old output is removed before work starts. Neither a stale file
nor a partial file after exhaustion/trapping is accepted. The shell returns
124 for a step limit, 126 for a trap, 125 for a completed nonzero compiler
exit before handoff, and 2 for missing/inconsistent records or I/O failures.
A compiler refusal remains an error, not a passing F3 comparison or an escape
from the Supported/Fits correctness statement. The existing automatic -lm
retry is restricted to a completed compiler exit with unresolved references.

## Reproduction and evidence

Commands run from this worktree (the focused suite builds the emulator with
C11 and warnings as errors, creates its C sources/ELF fixtures in temporary
directories, runs each emulator serially, and removes those directories):

```sh
gcc -std=c11 -O2 -Wall -Wextra -Werror -o /tmp/rv64emu-handoff scripts/rv64emu.c
bash -n scripts/ref_arena.sh
python3 scripts/tests/test_ref_handoff.py
free -g
prlimit --as=12884901888 -- python3 -u scripts/tests/test_ref_handoff.py --image
scripts/check.sh --static-only
git diff --check
```

All passed. Available memory before the final image suite was 89 GB
(earlier checks: 87, 81, 84 GB). There were no Lean builds, proof edits,
budget changes, Sail jobs, or sum compile-cut regeneration. The static gate
ran generator checks only; canonical sum ELF-dependent checks were skipped
because build/p3/sum.elf was absent, and Clight re-derivation was skipped
because clightgen was absent. ISA checked the freshly built base image:
106,502 instructions, zero forbidden instructions, five ebreak notes.

The real-image test sources are exactly:

```c
/* terminating */
int main(void) { return 7; }
/* infinite */
int main(void) { for (;;) {} }
/* deliberate post-handoff fault */
int main(void) { return *(volatile int *)0; }
```

| C11 input | compile_steps | handoff steps | ordinary execution |
|---|---:|---:|---|
| terminating | 2,950,206 | 2,950,231 | exit 7 at 2,950,320 steps |
| infinite | 2,950,224 | 2,950,249 | status 124 at the 3,050,249-step limit |
| trapping | 2,951,597 | 2,951,622 | status 126: access fault at 0, PC 0x86400018 |

For all three, the full-run arena file (captured before executing the program)
and the new handoff arena are byte-identical, including headers. The
terminating case deliberately exits 7, demonstrating that host handoff
success does not claim program exit 0. The infinite loop is a valid C11
divergence fixture. The null access is deliberately undefined C execution
used to test the harness; it is not a NoUB/Supported/Fits witness.

The suite also checks exact-marker step exhaustion without publishing an
arena for each real input; synthetic pre-compile limits and traps; exhaustion
between run and enter; missing run/enter symbols; invalid arena bounds;
failed arena opens and buffered close failure via /dev/full; a compiler syntax
error; and unresolved libm references with/without the automatic retry.
Nine controlled harness failures cover a limit/trap with an arena present,
missing/legacy/duplicate records, an inconsistent process status, absent or
partial arenas, and a pre-handoff access fault without a summary. Every
failure removes a preexisting public output file and remains an error.

## Pins and integration

Host compiler: gcc (Ubuntu 15.2.0-16ubuntu1) 15.2.0.
Image compiler: xPack GNU RISC-V Embedded GCC 15.2.0-1, newlib 4.5.0.20241231,
rv64i/lp64 multilib. Bare-metal runtime build flags remain the Makefile's
GNU11/O2/RV64I/lp64/medany flags; embedded tcc runs with -std=c11 and -w,
FLAGS=0. TinyCC is pristine vendor pin
43c7708b85681a2fd4451c8a541af4494a8919b2 plus the existing four patches.

SHA256 pins from this worktree:

```text
5afa37bbab2e6d1767237fc73457a7e7ca510705ebbba571a5790cac76e10217  patches/0001-tcc-rv64i-soft.patch
e27049337b7cf1944dec01b1f594c02c017b55cbea1c8d73c29746e07064cc23  patches/0002-tcc-baremetal-run.patch
8ce084ebb702b1889a0d56603e7a1551d0171225f761bb131926d73d46acc9c5  patches/0003-tcc-c11.patch
4dfcc192c23a777b1b335e9052a416956886b1071c3de1e8da403b5b194e221e  patches/0004-tcc-binary128-literals.patch
7a7191cc117bc15a3943ece43259b94bc3e72a08e0b457c1145032c10d62d397  c/build/base.elf
fa8f247ff8599f302f6597713e48ab519f21202a1faa57c18430253bf1167c33  c/build/tcc.o
7f02dca6bd5c1fe676b2866b64f54a0699b7a73d3032c7dbecb151f0919d90ea  c/build/main.o
8429dd1c48a8a05da22589272d84c2d02ebfd97e269140de041a08412e2e77b1  c/build/htif.o
1f934eb8bf6be0b3e39c823289f91f44e68576f4e0aada5e35f5be45115ff8c8  c/build/crt0.o
```

Neither model_diff.py nor TccModel was edited or rebuilt. Its current numeric
exit regex still parses ordinary exits and compiler refusals. On successful
collection its ref_run branches on process status 0 before requiring a regex
match, so the new exit=- handoff record works on that path.

Required lead/other-worker integration: ref_run must treat every ref_arena
status other than 0 or 125 as a harness error (124/126 already have explicit
checks). In particular, status 2 must not become a cached compiler exit just
because a malformed/legacy summary contains a numeric exit field. A consumer
that wants step metadata on success must parse status=handoff/exit=-
explicitly, rather than inventing a generated-program exit. Fresh evidence
must invalidate/version old fast-reference caches: the existing key does
not include the runner source, and old cached success lacks the new
completion-record validation. The separate runtime-fingerprint correction
and broader F3 arena comparison remain with the other worker.

This runner is diagnostic only. These checks do not establish the Sail F3
exit criterion, whole-compiler correctness, or a proof about program
termination. No broad differential or Sail spot-check was claimed here.

## Dead ends and environment limits

`systemd-run --user --scope -p MemoryMax=12G true` failed with
"Failed to connect to user scope bus via local transport: Operation not
permitted". Do not resend this worker to attempt an uncapped Lean build.
No Lean work was needed for these C/shell changes. C/image tests ran under
the explicit 12 GiB address-space cap shown above; the host C emulator also
built cleanly with -Werror. No other projects' files or processes were
changed.

Commit dead end: `git add scripts/rv64emu.c scripts/ref_arena.sh
scripts/tests/test_ref_handoff.py results/model/ref-handoff.md` failed before
staging with `fatal: Unable to create
'/data/home/kirancodes/Documents/code/ship-your-tcc/.git/worktrees/sol-2-reference/index.lock':
Read-only file system`. The chained commit was never executed. Per MANAGER.md,
the completed four-file worktree diff is left for the lead to review and
commit with writable Git metadata; there is no worker commit to integrate.

