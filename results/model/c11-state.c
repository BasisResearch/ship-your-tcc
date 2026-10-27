/* Independent arena regression: suspended macros, include identity, and
   phase-6 lookahead must survive the temporary _Pragma token stream. */
#include "c11-state.h"
#include "c11-state.h"
_Static_assert(sizeof(struct PackedOnce) == 5, "pragma file and pack state");
struct Unpacked { char c; int x; };
_Static_assert(sizeof(struct Unpacked) == 8, "pack restoration");

#define VALUE 11
_Pragma("push_macro(\"VALUE\")")
#undef VALUE
#define VALUE 22
#define RESTORE _Pragma("pop_macro(\"VALUE\")") VALUE
#define WRAP(x) x
_Static_assert(WRAP(RESTORE) == 11, "pop before following rescan");

#define LITERALS "A" ENTER_PACK u"B" _Pragma("pack(pop)") "C"
static unsigned short text[] = LITERALS;
_Static_assert(sizeof(text) == 8, "phase-6 lookahead and macro buffers");
#define PARAM(\u00e9) \U000000e9
_Static_assert(PARAM(7) == 7, "canonical UCN parameter identity");

int main(void) { return text[0] != 'A' || text[1] != 'B' || text[2] != 'C' || text[3]; }
