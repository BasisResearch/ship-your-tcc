/* long double basics (128-bit on riscv64, libgcc __*tf* routines). Only
   results that are exact in both binary128 and x87 80-bit are printed, so
   the host comparison is meaningful too. */
#include <stdio.h>

long double ladd(long double a, long double b) { return a + b; }
long double lmul(long double a, long double b) { return a * b; }
long double ldiv_(long double a, long double b) { return a / b; }
long double lsub(long double a, long double b) { return a - b; }

struct LS { long double v; };
struct LS lstruct(struct LS s) { s.v *= 4; return s; }

int main(void)
{
    long double a = 1.5L, b = 0.25L, c;
    int i;
    double d;
    c = a + b; printf("%g\n", (double)c);
    c = a - b; printf("%g\n", (double)c);
    c = a * b; printf("%g\n", (double)c);
    c = a / b; printf("%g\n", (double)c);
    printf("%g %g %g %g\n", (double)ladd(a, b), (double)lsub(b, a), (double)lmul(a, -b), (double)ldiv_(1, 8));
    printf("%d %d %d %d %d %d\n", a == b, a != b, a < b, a <= b, a > b, a >= b);
    printf("%d %d\n", a == 1.5L, !b);
    /* conversions */
    c = 123456789; printf("%g\n", (double)c);
    c = -42; printf("%d %ld %u\n", (int)c, (long)c, (unsigned)(c * -2));
    c = 4294967295u; printf("%lu\n", (unsigned long)c);
    c = 1e300; d = c; printf("%g\n", d);
    c = 0.1f; printf("%.9g\n", (float)c);
    c = 9007199254740993.0L * 0 + 3.75L; printf("%ld %g\n", (long)c, (double)c);
    {
        struct LS s = {0.5L};
        s = lstruct(s);
        printf("%g\n", (double)s.v);
    }
    /* accumulate exactly representable values */
    c = 0;
    for (i = 0; i < 100; i++) c += 0.5L;
    printf("%g\n", (double)c);
    c = 1;
    for (i = 0; i < 64; i++) c *= 2;
    printf("%.17g %d\n", (double)c, c > 1e19L);
    return 0;
}
