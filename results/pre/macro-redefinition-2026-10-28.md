# Latest macro definition provenance

`Phase4.doDefine` now installs the latest permitted definition even when its
replacement tokens equal the previous definition. The equality test still
enforces C11 redefinition constraints; token equality no longer discards the
new physical source occurrence. Patched tcc's `define_push` already installs
each definition and uses equality only to decide whether to warn.

The model-origin worker discovered this in a repeated-header comparison.
Independent raw-frontend probes then showed a semantic identity consequence:
an identical user redefinition of a library macro retained library membership,
while an identical library redefinition of a user macro retained user membership.
Both now follow the latest definition. Repeated includes retain the second
directive's ancestry. Function-like macro arguments retain their own operand
identity, and replacement diagnostics still point at the invocation. An
explicit `#undef` control behaved correctly before and after.

Before/after captures run the preserved old executable and freshly built
executable on identical fixed-path inputs. The original regression fails on
both membership directions and repeated-include ancestry; all 12 raw-frontend
test methods now pass. Another 21 token/integration methods pass, as do all
24 library-identity cases and 5,580 semantic consumer checks. The full
`scripts/check.sh --csem` gate passes with 2,297 unchanged input hashes,
including the ordinary axiom audits and C11 preprocessing tests.

The first build attempt used the nonexistent target `PreSpec`; its failure is
retained. The corrected explicit build uses `CSemPre` and passes before any
accepted comparison. Earlier pinned binaries are verified in the local recovery
archive recorded by the manifest. All artifacts in the portable evidence
archive were verified by SHA-256.

This repairs local preprocessing provenance, not an upstream Cerberus defect.
It changes no compiler image or token output and claims no new Sail run or
preprocessing theorem. Combined validation against the new model-origin
checkpoint remains separate; its original failed observation is preserved.
