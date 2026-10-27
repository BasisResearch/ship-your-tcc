/* cfgTcc probe: long double is IEEE binary128, 16 bytes, aligned 16. */
#include <stddef.h>
#include <stdio.h>

struct sl { char c; long double x; };

int main(void) {
    long double one = 1.0L;
    long double tiny = 1.0L;
    int k;
    /* 2^-k still distinguishable from 0 when added to 1: k < 113 */
    for (k = 0; k < 200 && one + tiny != one; k++)
        tiny = tiny / 2;
    printf("sizeof=%d align=%d off=%d mant=%d\n", (int)sizeof(long double),
           (int)_Alignof(long double), (int)offsetof(struct sl, x), k);
    long double third = 1.0L / 3;
    double dthird = 1.0 / 3;
    printf("ne=%d\n", third != (long double)dthird);
    return k;
}
