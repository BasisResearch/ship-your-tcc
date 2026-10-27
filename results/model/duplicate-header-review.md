# Embedded-file order discrepancy — 2026-10-27

The image and `TccModel` disagree when the embedded table contains the same
file path twice. `htif.c:fs_init` replaces the existing file's data with the
later entry; `ModelMain.lean` instead keeps the first entry. The comment
claiming that the first entry matches `fs_init` is incorrect. CerbFS's
ordered initializer already uses the later contents.

The minimized input is `results/repro/tcc_image_duplicate_header.c`. Embed
`tcc_image_duplicate_first.h` and then `tcc_image_duplicate_last.h` under the
same image path `/duplicate.h`. The source asserts that the second header's
value is visible. The real compiler reaches handoff on the independent fast
RV64I interpreter; the model exits 1 with `last embedded file must win`.
`duplicate-header-review.json` retains the model/runtime/source pins.

This extends the existing shared image-path task, not the C dialect. The
normal fixed image table has no duplicate paths, so this finding does not
invalidate the recorded comparisons using that table. Duplicate/conflicting
entries in the executable interface still require correct semantics; no
new Supported condition should hide them. Test both entry orderings and
file-versus-directory collisions while factoring the shared traversal and
initialization logic. Do not merely change first-wins to last-wins in the
hash map: unconditional directory creation also ignores those collisions.
