# Opened-path once cache and corpus fixtures

The image's `realpath` implementation copies the opened spelling. Consequently
`#pragma once` caches `/prog.c` separately from `/./prog.c`. The preprocessing
spec now selects this policy in `cfgTcc`; the host preprocessing configuration
retains resolved-path keys. Directive and `_Pragma` handling use the same
named `OnceKey` policy. Physical source provenance still uses resolved identity.
The configuration generator pins the image's `realpath` body so a change
requires an explicit policy review. This is implementation-defined pragma
behavior; no compiler patch or C11 relaxation is needed.

The corpus harness now supplies `/prog.c` and sibling fixtures under both
their basename and original parent-directory spelling to all three engines.
This repairs macro self-inclusion and `../tests2/18_include2.h` lookup.
Source and header bytes are recorded; no reference tokens enter the model.
The first rerun exposed the once-policy mismatch instead of hiding it; those
pre-repair tables remain in `once-cache-2026-10-28/before-once-policy-*.json`.

At source revision `596d5941f691652711ef6e799c85a77393a82109`:

- Full `scripts/check.sh --csem` passes, including ordinary axiom audits.
  All 2,284 frozen source hashes and both comparison executable manifests
  were reverified after the gate.
- Fourteen preprocessing integration methods, eleven raw frontend methods
  and seven protocol methods pass. All three focused reproducer inputs
  agree exactly in both comparisons.
- The 622-input corpus has 618 exact raw model/reference agreements and
  four joint refusals; the spec has 616 exact agreements, one compiler
  refusal and five spec errors. There are no token differences or harness
  failures. The residuals are `pp/03.c` (stray backslash), `pp/21.c` (signed
  overflow in a conditional), `pp/22.c` (directives within macro arguments),
  and three inputs requiring unavailable semaphore, mmap or Windows headers.
- The full compiler on executable Sail runs
  `results/repro/tcc_pragma_once_opened_path.c` with `-std=c11`, empty stdout
  and exit zero: 3,383,293 compile steps, 89 program steps, 3,383,407 total.
  The exact ELF, embedded source, traces, output and runtime pins are retained
  under `once-cache-2026-10-28/native/`; every archived file hash was checked.

This is finite validation. The three preprocessing correctness/effect/
noninterference Statements, raw frontend connections and whole-C obligations
remain open. Supported/Fits and the frozen sum compile-cut are unchanged.
