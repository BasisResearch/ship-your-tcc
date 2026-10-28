# GNU expression syntax integration, 2026-10-28

Compiler actions now retain complete Cabs for omitted-middle conditionals,
expression/type operands of `typeof`, and `__builtin_choose_expr`. The actual
parser actions supply both chosen and unchosen children; no specification AST
enters the compiler model. The independent grammar also accepts separate
question/colon preprocessing tokens, preserving both physical origins.
The C compiler source and adopted local replay-erasure theorem are unchanged.
CSem stays C11 and the model CLI always prepends `-std=c11`.

This combines reviewed checkpoint `7e5f0c7` with adopted bitfield semantics
`358a7af`. Only appended attribution/dead-end notes conflicted; both were kept.
The fresh full `scripts/check.sh --csem` gate passes with **3,480 frozen
inputs**. **269 unchanged prior-main fixture/JSON pins** are checked separately;
they are not extra primary request inputs. Standard-axiom, generator,
discipline, abstraction, semantic and model checks pass.

## Separate evidence scopes

- All 50 positive GNU cases have exact complete Cabs in shared tokens,
  independent parsing of model tokens, and independent raw preprocessing/parser
  scopes. Ten real refusals remain separate from positives.
- Two GNU policy probes remain outside positive coverage: read-only increment
  within typeof (tcc warns, GCC rejects), and a comma choose condition (tcc
  folds it, GCC rejects). A warning is a diagnostic; these observations are
  not evidence of absent-diagnostic C11 nonconformance.
- There are 50 positive independent fast arena pairs, two separate policy-input
  pairs and 62 identical new ghost on/off arena/console/status pairs.
- All six complete native 4 MiB arena comparisons pass, each with a separate
  successful full compiler/program run: the GNU counter batch and inherited
  generic, assertion and three array-alignment controls. Model/native arenas,
  presence maps, registers, manifests and actual execution captures are kept.
- The unchanged original 56 inputs give 50 shared / 51 model-token / 50
  independent-raw exact comparisons. All 58 physical-origin controls and four
  omission guards pass; all fifteen inherited ghost pairs are identical.
- Six additional typeof/VLA controls retain exact syntax in all three scopes
  and return zero on GCC and patched host tcc. These host observations are
  separate from native evidence and from CSem execution.
- Adopted bitfield semantics still pass all 82 required cases. Four trailing
  GNU member-attribute refusals remain in this combination; their reviewed
  repair is a separate checkpoint awaiting rebuilt-image integration.

No historical broad-corpus counts are pooled with these observations. The
worker's full gate, 15,217-member archive and six native exports remain
separately pinned in results/model/gnu-expression-syntax.md. Its initial wrong
policy expectations and missing-main harness failure remain preserved.

## Additional F1 semantic probes

Syntax construction does not establish Cabs/Ail/Core meaning. Ten bounded
independent raw-CSem probes retain their complete outputs and source/binary
pins. All ten return zero with GCC and patched host tcc; no new native Sail
claim is made for this supplemental set.

CSem returns defined zero for ordinary unevaluated typeof, choose single
evaluation, and an ordinary conditional control. The GNU omitted-middle
conditional instead reports `couldn't find the startup function`. The shared
translator's `AilEcond e1 Nothing e3` branch records an unimplemented-lowering
error and returns a skip node. Five typeof/VLA cases and a separate standard
C11 VLA case fail elaboration. Their shared frontend explicitly refuses
nonconstant array bounds. These are retained nonsuccesses, never extra passing
syntax/semantic counts or Supported exclusions.

The corresponding TODO/refusal markers are also present in pristine
Cerberus-lean pin `704c4e69`; exact source hashes and excerpts are retained.
This is a pristine source-level check, not a claim of a new pristine execution
comparison or an upstream wrong defined result. CSEM-15/CSEM-16 in
CSEM-UPSTREAM.md track these coverage limitations for Kiran. Repair tasks are
`tasks/f1-gnu-conditional-semantics.md` and `tasks/f1-vla-semantics.md`.
Nothing was sent externally.

## Audit and open obligations

Run `python3 results/model/gnu-expression-integration-2026-10-28/verify.py --sources`.
The archive contains 10029 hash-verified members, every primary and supplementary
source byte by digest, the full gate receipt, postchecks, all six complete
native exports and the separately classified semantic probes.

The eleven ghost contracts remain open, including complete construction,
parser agreement, whole-run erasure and binary refinement. The local replay
law proves snapshot restoration only. F1 meaning for the recorded GNU/VLA
gaps, enum policy, declaration constraints, concrete memory/volatile transport,
runtime bodies and whole F1-F3 completion remain work. The headline stays
`tcc_run_correct` with Supported/Fits and no refusal disjunct. Sum cuts remain
frozen until the compiler image is stable.
