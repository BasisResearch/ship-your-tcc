/* run-time conversions to/from long double (every __*tf* conversion
   routine), and float -> unsigned long. Values exact in binary128 and
   x87 extended, printed via double. */
#include <stdio.h>

volatile float vf = 1.5f;
volatile double vd = -2.75;
volatile int vi = -123456;
volatile unsigned vu = 4000000000u;
volatile long vl = -9000000000000000000L;
volatile unsigned long vul = 18000000000000000000UL;

int main(void)
{
    long double a = vf, b = vd, c = vi, d = vu, e = vl, f = vul;
    float bf; double bd;
    printf("%g %g %g %.17g %.17g %.17g\n", (double)a, (double)b, (double)c, (double)d, (double)e, (double)f);
    bf = a * 2; bd = b / 2;
    printf("%g %g\n", (double)bf, bd);
    printf("%d %u %ld %lu\n", (int)c, (unsigned)d, (long)e, (unsigned long)f);
    {
        float big = 1.5e19f;
        unsigned long u = big;           /* __fixunssfdi */
        long double s = a + b + c;
        printf("%lu %g %d\n", u, (double)s, s < c);
    }
    return 0;
}
