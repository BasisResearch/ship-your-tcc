# Member attributes on the combined compiler image

Patch 0005 fixes tcc's member attribute state leaking across comma-separated
declarators. Each declarator now starts from the common specifier attributes;
Gen3 performs the same reset. Independent reconstruction of 552 source files
finds only this tccgen.c change. The reviewed checkpoint 5c2f70f also retains
per-member Cabs attributes, unnamed field ownership, packed/aligned layout,
zero-width barriers and the matching independent Menhir printer patch.

The lead combined it with adopted C11 bitfields and GNU expression syntax at
7a6894b, rebuilt the actual compiler image and symbols, and froze 3,577 primary
inputs plus 269 unchanged supplementary fixture/revision pins. Eighteen
additional JSON fixture/manifests were independently checked against their
immutable frozen-base Git blobs; their supplement is separate from the
original service request. The fresh full
repository/CSem gate passes, including generator drift, purity, totality,
standard-axiom audits and all inherited native controls. Strict C11 host,
GCC, preprocessing, cross compiler and ISA checks pass 274 positive and 27
negative cases. Patch context whitespace is retained verbatim for exact
application; it is not stripped to satisfy a whitespace linter.

All 90 required C11/GNU bitfield checks pass. The 54 focused attribute cases
have complete raw/text Cabs equality: 46 defined-zero controls and 8 separate
GCC syntax diagnostics. All 42 bounded raw initialized-byte layout observations
match GCC. Independent Menhir produces 46 exact Cabs and 8 explicit placement
limitations, including valid leading placements; these are not agreement.
The original four GNU failures are now defined-zero. Original 56 ghost syntax
coverage remains 50 shared / 51 model-token / 50 independent raw; the 58 physical
cases, four corruption guards and 15 ghost on/off comparisons pass.

A fresh full compiler/program Sail run of the unchanged 42-row batch matches
GCC byte-for-byte and exits 0. Compile: 21,589,947 steps; program: 679,065;
total: 22,269,037; wall: 748.966 seconds. The actual ELF, exact embedded source,
output and trace are retained. Its ELF hash differs from the earlier worker
image: runtime sections match, while embedded libtcc1.a has symbol/string
metadata differences in four objects and equal allocated object sections.
Both actual archive byte strings were located in their respective native
ELFs. The fresh image/source pins remain authoritative; no observation is
recertified under the old image's hash.

The combined gate separately repeats six complete 4 MiB native Sail arena
comparisons, with handoff registers, and six full compiler/program executions
for GNU expressions, generic/assert syntax, assertion messages and array decay.
The worker checkpoint's gate had zero optional inherited native comparisons;
its separate six actual native ELFs remain historical evidence. Its before/
after batch retains 24/42 old layout differences and 42/42 repaired rows.

No finite comparison proves compiler correctness. Enum fields, compiler
constraint diagnostics, complete grammar/semantics, physical transport, the
existing eleven ghost contracts and F4/F6 memory/volatile connections remain
open. The reviewed multi-TU checkpoint 96c520e is the next combination; it is
not part of this frozen source set. No Supported/Fits restriction or sum-cut
regeneration was introduced.

Run `python3 results/csem/bitfield-attributes-integration-2026-10-28/verify.py`;
`--sources` additionally checks the exact source snapshot. The archive retains
all primary/supplementary source bytes by digest, complete fresh comparisons,
native captures, independent oracle output, image preparation and source audit.
The reviewed worker archive retains its failed attempts and old native cases.

Portable verification passes for 13,141 regular archive members. The first
packaging attempt preserved five host-build Makefile symbolic links; the
regular-member check rejected it. The retry dereferences those links and
verifies every captured byte. The retained first diagnostic's cause label
was provisional (inode deduplication); its explicit type-2 member list records
the actual symbolic-link issue. No compiler or proof source changed.
