/* things that call into libtcc1.a: __builtin_clz & co, alloca; and
   unsigned/signed narrowing conversions */
#include <stdio.h>
#include <alloca.h>

int main(void)
{
    unsigned vals[] = {1u, 2u, 3u, 0x80000000u, 0x00f00000u, 0xffffffffu, 12345u};
    unsigned long lv[] = {1UL, 0x8000000000000000UL, 0x0000ffff00000000UL, 0xfffUL};
    int i, n = 5;
    for (i = 0; i < 7; i++)
        printf("%08x clz=%d ctz=%d pop=%d ffs=%d par=%d\n", vals[i], __builtin_clz(vals[i]), __builtin_ctz(vals[i]),
               __builtin_popcount(vals[i]), __builtin_ffs((int)vals[i]), __builtin_parity(vals[i]));
    for (i = 0; i < 4; i++)
        printf("%016lx clzl=%d ctzl=%d popl=%d\n", lv[i], __builtin_clzl(lv[i]), __builtin_ctzl(lv[i]), __builtin_popcountl(lv[i]));
    {
        double *buf = alloca(n * sizeof(double));
        long *lb = alloca(n * sizeof(long));
        for (i = 0; i < n; i++) { buf[i] = i * 1.5; lb[i] = (long)i * -1000000007L; }
        for (i = 0; i < n; i++) printf("%g %ld ", buf[i], lb[i] / 3);
        printf("\n");
    }
    {
        int neg = -1; long big = 0x123456789L;
        unsigned char uc = neg; unsigned short us = neg; signed char sc = 200 - 256;
        unsigned u = big; int t = (int)(big & 0x7fffffff);
        printf("%d %d %d %u %d %u\n", uc, us, sc, u, t, (unsigned)neg / 2);
    }
    return 0;
}
