/* cfgTcc probe: integer and pointer widths (lp64). */
#include <limits.h>
#include <stddef.h>
#include <stdio.h>

int main(void) {
    printf("short=%d int=%d long=%d llong=%d ptr=%d fptr=%d\n",
           (int)sizeof(short), (int)sizeof(int), (int)sizeof(long),
           (int)sizeof(long long), (int)sizeof(void *), (int)sizeof(int (*)(void)));
    printf("size_t=%d ptrdiff_t=%d wchar_t=%d\n",
           (int)sizeof(size_t), (int)sizeof(ptrdiff_t), (int)sizeof(wchar_t));
    printf("INT_MAX=%d LONG_MAX=%ld ULONG_MAX=%lu\n", INT_MAX, LONG_MAX, ULONG_MAX);
    printf("SHRT_MIN=%d USHRT_MAX=%d\n", SHRT_MIN, USHRT_MAX);
    return (int)sizeof(long);
}
