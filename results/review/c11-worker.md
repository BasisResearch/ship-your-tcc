C11 worker active in codex-c11 (2026-10-21). Patch is now active as
patches/0003-tcc-c11.patch; 0001/0002 and pristine vendor are unchanged.

Slot2 coordination: 0003 touches tcc.h, tccpp.c and tccgen.c.
New tokens remain U16STR=d0 U32STR=d1 U16CHAR=d2 U32CHAR=d3,
digraphs d4..d9; existing token numbers unchanged. `_Pragma` is allocated
by spelling after architecture keywords; appending an enum entry after the
RISC-V keyword table fails because that table repeats some spellings. TokenSym adds a
canonical identifier number to preserve UCN spelling during preprocessing.
Literal concatenation now selects its encoding before escape interpretation.
No binary128 parsing changes: 0004 still belongs to slot2. Apply 0004 after
0003; avoid copying a whole stale tccpp.c over these changes.

Host/cross checks pass: 22 positive cases, six negative UCN cases, -E round
trips and RV64I object ISA checks. Upstream host pp: 24/24; tests2: 124/124.
Sail and broader RV64I validation are in progress. Final results and commit will replace this checkpoint.
