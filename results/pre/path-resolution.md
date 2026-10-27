# Pending preprocessing filesystem repair — 2026-10-27

`results/repro/pre_include_missing_parent.c` includes
`missing/../lib/tcc/include/stddef.h`. The `missing` directory does not exist.
The image resolves components in order, so the include must fail; lexical
normalization must not erase the missing-directory traversal.

The current `FileSys.lookup` in Spec/Pre/Types.lean normalizes the full path
before lookup and incorrectly accepts this input. The strict pre_diff run
reports tcc-error with a successful spec token stream (path-resolution-pre.log).
The independent fast compiler/model comparison records a matching refusal
(path-resolution-model.json): Model.Host.resolve already walks components,
as does CerbFS.walk. This is our preprocessing specification bug, not a
Cerberus/cerberus-lean bug, and not an additional C11 compiler deviation.

Repair must reuse/factor the existing path traversal abstraction before adding
a third implementation. Preserve empty paths, missing intermediate components,
file-as-directory, trailing slashes, repeated slashes, dot/dotdot, root parent,
and initial embedded-file ordering. A pure shared walker can take directory
parent/child operations and retain caller-specific node/error projections.
Do this after the in-flight CSem repin evaluation, since factoring CerbFS while
that fork is being replayed would disrupt the worker. The raw-token observation
issue for trigraph-safe -E printing is a separate F2 task. Neither issue changes
Supported/Fits or the whole-compiler target.
