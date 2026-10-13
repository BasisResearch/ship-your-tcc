/* integer -> float/double conversions (rounding of wide values) */
#include <stdio.h>
#include <limits.h>

int vi[] = {0, 1, -1, 16777217, -16777217, INT_MAX, INT_MIN, 123456789};
unsigned vu[] = {0u, 1u, 16777217u, 0x7fffffffu, 0x80000000u, 0xffffffffu, 3000000001u};
long vl[] = {0L, -1L, 9007199254740993L, -9007199254740993L, LONG_MAX, LONG_MIN, 1L << 62};
unsigned long vul[] = {0UL, 9007199254740993UL, 0x8000000000000000UL, 0x8000000000000401UL, ULONG_MAX, 0xfffffffffffff800UL};

double i2d(int x) { return x; }
float u2f(unsigned x) { return x; }

int main(void)
{
    int i;
    for (i = 0; i < 8; i++) printf("i %d -> %.17g %.9g\n", vi[i], (double)vi[i], (float)vi[i]);
    for (i = 0; i < 7; i++) printf("u %u -> %.17g %.9g\n", vu[i], (double)vu[i], (float)vu[i]);
    for (i = 0; i < 7; i++) printf("l %ld -> %.17g %.9g\n", vl[i], (double)vl[i], (float)vl[i]);
    for (i = 0; i < 6; i++) printf("ul %lu -> %.17g %.9g\n", vul[i], (double)vul[i], (float)vul[i]);
    {
        signed char sc = -100; unsigned char uc = 200; short s = -30000; unsigned short us = 60000; _Bool b = 1;
        char c = 250; /* unsigned on riscv */
        double d = sc; float f = uc;
        printf("%g %g %g %g %g %g\n", d, (double)f, (double)s, (double)us, (double)b, (double)c);
        f = s; d = us;
        printf("%g %g\n", (double)f, d);
    }
    printf("%.17g %.9g\n", i2d(-7), (double)u2f(0xffffff7fu));
    {   /* implicit conversions in arithmetic and assignment */
        int n = 7; unsigned u = 0x90000000u; long l = -3;
        double x = n / 2 + n / 2.0 + u + l;
        printf("%.17g\n", x);
    }
    return 0;
}
