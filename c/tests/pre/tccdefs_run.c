/* tccdefs.h as tcc -run reads it (the #ifndef __TCC_PP__ part), which
   tcc -E never shows: re-included here with __TCC_PP__ undefined. */
#undef __TCC_PP__
#define __TCC_RUN__ 1
#include <tccdefs.h>
