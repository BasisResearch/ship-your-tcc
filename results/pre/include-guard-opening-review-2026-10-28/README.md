# Include-guard opening observations

Twenty retained cases compare patched host tcc `-vv -E`, the executable
compiler model and the independent preprocessor. Host and model agree on
all20 opening counts: 10 recognized/defined guard cases open once;
10 controls open twice. The independent preprocessor opens twice
on every case, identifying a missing target cache policy rather than a
model opening to erase. All processes exit zero.

Leading/trailing null directives, comments and whitespace preserve caching.
A directive before the guard, outer else/elif, trailing declarations or
other directives prevent it; inner else does not. Guard predefinition and
undef/redefinition have distinct retained controls. `#if !defined` is not
recognized by this tcc optimization. Guard identity uses opened spelling,
separately from pragma-once path normalization.

The host and virtual paths are recorded separately. These are finite host
and executable-model observations, not fresh native Sail evidence or a
universal preprocessing proof. Sources, exact commands, outputs, tool hashes,
main executable-refresh provenance and the Lua abstraction inventory are
in the hash-indexed archive. Follow-up: tasks/f2-include-guard-openings.md.
