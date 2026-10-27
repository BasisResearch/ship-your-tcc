/* Integer/pointer representation, independently of allocation placement. */
#include <stdio.h>
#include <stdint.h>
int main(void) {
    uintptr_t n = (uintptr_t)(void *)0;
    uintptr_t hi = (uintptr_t)(void *)(uintptr_t)0x1234567887654321ULL;
    printf("null=%lu bits=%d roundtrip=%d\n", (unsigned long)n,
           (int)(8 * sizeof(void *)), hi == 0x1234567887654321ULL);
    return 0;
}
