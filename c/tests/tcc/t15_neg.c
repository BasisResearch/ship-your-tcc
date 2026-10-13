/* unary minus on float/double/long double (a sign flip, not 0-x) */
#include <stdio.h>
#include <string.h>

static unsigned long bits(double d) { unsigned long u; memcpy(&u, &d, 8); return u; }
static unsigned fbits(float f) { unsigned u; memcpy(&u, &f, 4); return u; }

double neg(double x) { return -x; }
float fneg(float x) { return -x; }

int main(void)
{
    volatile double zero = 0.0;
    double v[5];
    int i;
    v[0] = 0.0; v[1] = 1.5; v[2] = -2.25; v[3] = 1.0 / zero; v[4] = zero / zero;
    /* the NaN's bit pattern is target-specific (x86 default NaN is negative,
       RISC-V's canonical NaN positive): only its sign flip is printed */
    printf("nan sign flip %d\n", (bits(v[4]) ^ bits(-v[4])) == 0x8000000000000000UL);
    for (i = 0; i < 4; i++) {
        double d = v[i];
        double n = -d;
        printf("%016lx -> %016lx %016lx %016lx\n", bits(d), bits(n), bits(neg(d)), bits(-(-d)));
    }
    {
        float f = 0.0f, g = 3.5f;
        printf("%08x %08x %08x %08x\n", fbits(-f), fbits(-g), fbits(fneg(g)), fbits(fneg(-0.0f)));
        printf("%g %g\n", 1.0 / -zero, 1.0 / neg(0.0));
    }
    {
        double a = 2.0, b = 3.0;
        printf("%g %g %g\n", -a * b, -(a - b), a - -b);
        a = -a; printf("%g\n", a);
    }
    {
        long double l = 1.25L, m = -l, z = 0.0L, nz = -z;
        printf("%g %d %d %g\n", (double)m, m < 0, -m == l, 1.0 / (double)nz);
    }
    return 0;
}
