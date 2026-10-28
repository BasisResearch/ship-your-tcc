# C-generator negation repair, 2026-10-28

The generator now parenthesizes a unary-minus operand. Previously a negative
floating literal could produce `--2.5f`, which C tokenizes as decrement rather
than nested negation. The generator now emits `-(-2.5f)`. Generated source files
were produced by the repaired generator; none was hand edited.

All **1,200 freshly generated programs** (seeds 1–1200) pass GCC GNU11 syntax
checking and produce identical patched-tcc/model arenas. There are no refusals,
model errors, harness errors or assembly gaps in this new batch. Compilation
uses tcc `-std=c11` with its existing approved GNU extensions. The reference is
the independent RV64 emulator; this batch adds no native Sail observation and
makes no claim that the generated executions are free of undefined behavior.
The full repository/CSem gate passes with **3,577 unchanged frozen inputs**.

The historical 1,815-input run remains 1,345 successful arenas, 463 refusals
and seven assembly gaps. All 1,815 source files and all 463 retained refusal
consoles are now archived here, including the exact 400 mutants. The 118 old
C-generator refusals all say `lvalue expected`; GCC independently rejects every
one for a decrement operand (123 error sites across those programs). The other
345 refusals are not reclassified as valid or invalid by this check. Their
actual diagnostics are retained. Missing entry points, companion translation
units, platform facilities and invalid mutations need distinct treatment.
`tasks/f3-corpus-invocations.md` queues the intended upstream inline bundle;
no test is silently removed or counted successful by classification alone.

The new 1,200-case result is a separate corpus snapshot. It is not added to or
substituted into the historical denominator. The historical and new sources,
exact generator, compiler/model inputs, both arena files per successful case,
GCC diagnostics and commands are retained with 6,133 verified archive members.
The lossless xz recompression retains exactly the verified tar members. The
artifact verifier also checks the equal model/reference arena hashes in the
archive. No compile-cut chain, including sum's witness, was regenerated.

Main's previously stale executables were refreshed under the shared lock and
12 GB cap; seven binary hashes and the completed source receipt are archived.
The base compiler image was rebuilt separately under the same memory cap.
The first full gate reached an absent matching GNU oracle cache link after
its build/check stages; that failed attempt remains archived. The immutable
adopted oracle was hash-verified and linked locally, then a new frozen gate
passed. No shared upstream checkout or oracle was modified.

[verify.py](cgen-negation-2026-10-28/verify.py) checks the portable evidence;
`--sources` additionally checks the exact final gate inputs. The two new
queued tasks concern corpus invocations and actual injected-binding provenance.
The existing 46/56 raw Cabs count remains a parser-construction comparison on
model-produced positioned tokens. It does not establish independent raw
preprocessing or compiler-injection authority; the latter task requires an
additional independent `Raw.checked cfgTcc` comparison. All corresponding
proof Statements and the whole-C target remain open.
