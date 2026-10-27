/* _Pragma once must use the physical file after #line. */
#line 300 "presumed/pragma.h"
_Pragma("once")
#define LEVEL 1
#define ENTER_PACK _Pragma("pack(push, LEVEL)")
ENTER_PACK
struct PackedOnce { char c; int x; };
_Pragma("pack(pop)")
