# Shared image-file initialization and lookup — 2026-10-27

`Spec/ImageFiles.lean` reuses the existing CerbFS initializer and component
walker. The preprocessing specification no longer erases missing directories
before lookup. The model driver now derives its files and directories from
that same initialized tree, preserving ordered duplicate contents, blocking
file/directory collisions, and the image's 256-node capacity. It does not
create directory prefixes for entries that the image ignores. Paths use a
byte-preserving adapter, not UTF-8 decoding. No CerbFS source changed.

The focused 12 Lean guards pass. `scripts/test_image_files.py` compares the
actual image compiler on the independent fast RV64I interpreter with the
native Lean model: **11/11 observations**, comprising **seven identical arenas
and four identical refusals**. Both duplicate orderings, both collision
orders, missing/file intermediate components, trailing/repeated slash,
dot/dotdot and root-parent traversal are covered. The script pins source,
runtime and model bytes in `image-files-review.json`. No reference arena is
supplied to the model, and no full Sail claim is made for these new cases.

The missing-directory reproducer is now rejected by all three preprocessing
engines (spec/tcc/GCC), retained in `results/pre/image-files-missing-parent.tsv`.
The 618-input corpus has 610 exact tcc agreements and the previous eight
residuals (one signed-character difference, two UB examples, five fixture
errors). The additional input since the 617-case report is line_varargs.c.
GCC has 605 exact agreements and six differences, plus the same seven
UB/fixture classifications. Raw table: `results/pre/image-files-corpus.tsv`.

The directory-not-file observation exposed another compiler issue: `_open`
succeeds for a directory, `_read` returns EISDIR, and tcc's refill code treats
negative reads as EOF. Thus including `/lib` succeeds as an empty header;
the specification correctly rejects it. `results/repro/pre_include_directory.c`
and `results/pre/include-directory.tsv` retain this separate F2 discrepancy.
The collision test checks that the ignored file's macros are absent, rather
than incorrectly expecting the existing compiler to reject the directory.
Correcting compiler/model read-error handling remains work; this report does
not declare all include behavior or the F2 theorem complete.

The full `scripts/check.sh --csem` gate passes: ISA, generated-file drift,
discipline, abstraction gate, capped Lean builds, 19 CSem unit executables,
standard-axiom audits, and 113 baseline oracle cases (90 MATCH, 18 UB_MATCH,
five CERB_SKIP). Frozen sum artifacts were checked, never regenerated.
