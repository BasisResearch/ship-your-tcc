# Assertion-message integration, 2026-10-28

C11 assertion messages now accept ordinary, u8, u, U and L literals in the
patched compiler and its Lean model. The assertion-specific helper preserves
basic diagnostic characters after embedded zeros and nonbasic units. Complete
Cabs comes from original scanner syntax, not the decoded diagnostic buffer.
Only tccgen.c changes in the reconstructed 552-file C tree; the ordinary
string consumers retain their previous policy. Vendor remains pristine.

The reviewed [worker checkpoint](static-assert-messages.md) retains 11,712
verified capture members and passes its full CSem gate with 3,379 frozen inputs.
All 3,379 inputs are byte-identical in this integration, and 269 additional
prior-main inputs are unchanged. The fresh combined **default** gate
`assert-integrated-1` passes with 3,648 pins, rebuilding the explicit observers
and checking discipline, ISA and axioms. The integration reuses the verified
source-identical worker full CSem gate; it does not claim another full CSem run.

The worker evidence distinguishes 65 exact complete Cabs controls in each of
three scopes (shared tokens, independent grammar on model tokens, and independent
Raw.checked source preprocessing/parsing), 64 new successful fast arenas,
61 separate diagnostic refusals and 18 inherited successful fast arenas.
Strict patch regression has 274 positives and 27 inherited negative controls.
The old generic suite now has 22 valid controls, 11 invalid controls and one
separate no-message extension. The previous valid wide-message refusal is
retired; its original evidence remains unchanged. The broader independent raw
suite remains 47/56, with nine explicit nonsuccesses. Physical origins,
opening authority and on/off checks retain their separate counts.

The worker full gate retains five complete native 4 MiB arena observations and
five distinct successful full compiler/program runs. After rebuilding the
integration image, the lead also runs **one fresh assertion batch**: every one
of 4,194,304 arena bytes agrees at 3,605,873 steps. A separate full invocation
returns zero with empty output, taking 3,602,631 compiler steps and 133 program
steps (3,602,789 total). Ghost on/off arena, console and status agree. The
actual execution image, input, embedded filesystem, trace, metadata and full
native handoff capture are retained. This additional observation is distinct
from the worker batch at 3,605,779 steps.

The rebuilt compiler object tcc.o is byte-identical to the worker object.
The base ELF, cross compiler and runtime archive have different whole-file
hashes, so image identity is not claimed. Comparing all four runtime archive
members shows only string/symbol-table differences: each contains one changed
STT_FILE worktree path; all other sections and resolved symbols agree. The
exact comparison drivers, section/symbol records and image pins are retained.
The fresh native comparison uses the exact rebuilt integration image.

Run `python3 results/model/assertion-messages-integration-2026-10-28/verify.py
--sources` to verify all 56 combined captures, source-equivalent gate reuse,
source hashes, the full native arena and capture manifest. The separately
retained worker package has its own complete verifier. Tests establish these
finite observations; source construction, transport, fuel, whole-run erasure,
eleven remaining ghost contracts and binary refinement remain open. The whole
compiler Supported/Fits target and frozen sum compile-cut chain are unchanged.

Slot 2 continues general GNU expression syntax. Slot 1's C11 bitfield/shared
compound-lvalue repair is not adopted: the newly found character-copy issue
and reference provenance require reviewed passing evidence. GNU trailing
bitfield attributes and four compiler constraint diagnostics remain explicit
follow-up tasks. No user decision is needed.
