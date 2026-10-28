# Aggregate pack-policy boundary observations

These are host observations for planning the remaining raw pack-layout consumer,
not Sail execution or a universal semantic connection. Patched host tcc and GCC
agree on six controls (size, alignment, offset of the int member): natural
`8 4 4`, pack(1) `5 1 1`, pack(2) `6 2 2`, push inside the aggregate `5 1 1`,
pop inside `8 4 4`, and pop after the declaration semicolon `5 1 1`.

The completed type uses the active cap at layout time: an opening-token policy
would incorrectly handle the push/pop-inside controls. tcc also processes a
push/pop between closing brace and semicolon before layout (`5 1 1`/`8 4 4`).
GCC rejects those two placements; they are separate implementation observations,
not successful C11 differential controls. Source inspection agrees: pristine
`tccgen.c` calls `struct_layout` after `skip('}')` and attribute parsing, and
`struct_layout` reads `pack_stack_ptr` once before iterating members.

The initial harness lacked tcc's local include path and failed to find stddef.h.
That attempt is retained alongside the corrected command. The archive contains
the eight exact sources, build diagnostics, executables, stdout and tool hashes
for both attempts. No fixture or observed layout is supplied to the semantics.
The implementation task is `tasks/f2-pack-layout-consumption.md`.
