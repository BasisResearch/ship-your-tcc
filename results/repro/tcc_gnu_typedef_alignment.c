/* GNU typedef alignment: layout and type queries must agree.
   GCC RV64I prints: 4 16 16 32 | 4 1 1 5 | 16
   Kept separate from GNU expression-alignof on an aligned member. */
#include <stddef.h>
#include <stdio.h>

typedef int Up __attribute__((aligned(16)));
typedef int Down __attribute__((aligned(1)));
typedef Up Again;
struct SUp { char c; Up x; };
struct SDown { char c; Down x; };

int main(void)
{
    printf("%zu %zu %zu %zu | %zu %zu %zu %zu | %zu\n",
           sizeof(Up), _Alignof(Up), offsetof(struct SUp, x), sizeof(struct SUp),
           sizeof(Down), _Alignof(Down), offsetof(struct SDown, x), sizeof(struct SDown),
           _Alignof(Again));
    return 0;
}
