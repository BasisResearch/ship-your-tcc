/* 32/64-bit multiply, signed and unsigned, incl. wrap-around (unsigned) */
#include <stdio.h>
#include <limits.h>

int vi[] = {0, 1, -1, 2, -2, 7, -13, 46341, -46340, 65535, 100000, INT_MAX, INT_MIN};
unsigned vu[] = {0u, 1u, 2u, 3u, 0x7fffffffu, 0x80000000u, 0xfffffffeu, 0xffffffffu, 65537u, 123456789u};
long vl[] = {0L, 1L, -1L, 3037000499L, -3037000499L, 4294967296L, -4294967296L, 123456789012L};
unsigned long vul[] = {0UL, 1UL, 0xffffffffffffffffUL, 0x8000000000000000UL, 0x100000001UL, 0xdeadbeefcafebabeUL};

int main(void)
{
    unsigned long h = 0;
    int i, j;
    /* signed int: only products that do not overflow */
    for (i = 0; i < 9; i++)
        for (j = 0; j < 9; j++) {
            long p = (long)vi[i] * vi[j];
            if (p >= INT_MIN && p <= INT_MAX) {
                int q = vi[i] * vi[j];
                printf("%d*%d=%d\n", vi[i], vi[j], q);
                h = h * 31 + (unsigned)q;
            }
        }
    printf("INT_MIN*1=%d INT_MAX*-1=%d\n", vi[12] * vi[1], vi[11] * vi[2]);
    for (i = 0; i < 10; i++)
        for (j = 0; j < 10; j++) {
            unsigned q = vu[i] * vu[j];
            h = h * 31 + q;
            if (j % 3 == 0) printf("%u*%u=%u\n", vu[i], vu[j], q);
        }
    for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++) {
            /* keep within range: |a|,|b| < 2^32 and product checked via unsigned */
            unsigned long ua = vl[i] < 0 ? -(unsigned long)vl[i] : (unsigned long)vl[i];
            unsigned long ub = vl[j] < 0 ? -(unsigned long)vl[j] : (unsigned long)vl[j];
            if (ua == 0 || ub <= (unsigned long)LONG_MAX / ua) {
                long q = vl[i] * vl[j];
                printf("%ld*%ld=%ld\n", vl[i], vl[j], q);
                h = h * 31 + (unsigned long)q;
            }
        }
    for (i = 0; i < 6; i++)
        for (j = 0; j < 6; j++) {
            unsigned long q = vul[i] * vul[j];
            printf("%lx*%lx=%lx\n", vul[i], vul[j], q);
            h = h * 31 + q;
        }
    /* mixed widths */
    {
        short s = -300; unsigned short us = 60000; signed char c = -100; unsigned char uc = 250;
        printf("%d %d %d %d\n", s * s, us * 3, c * uc, uc * uc);
        printf("%ld %lu\n", (long)s * 1000000000L, (unsigned long)us * 0x100000000UL);
    }
    printf("hash=%lx\n", h);
    return (int)(h & 0x7f);
}
