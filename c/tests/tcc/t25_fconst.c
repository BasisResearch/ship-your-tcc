/* floating constants: decimal, hex, extremes, subnormals, float suffixes,
   constant folding vs runtime evaluation */
#include <stdio.h>
#include <float.h>
#include <string.h>

static unsigned long bits(double d) { unsigned long u; memcpy(&u, &d, 8); return u; }
static unsigned fbits(float f) { unsigned u; memcpy(&u, &f, 4); return u; }

const double tab[] = {0.0, 1.0, 0.1, 1e300, 1e-300, 0x1.8p3, 0x1p-1074, 0x1.fffffffffffffp1023, 2.2250738585072014e-308, 123.456e-7};
static float ftab[] = {0.1f, 1e38f, 1e-45f, 0x1.fffffep127f, 3.14159265f};

int main(void)
{
    int i;
    volatile double one = 1.0, three = 3.0;
    for (i = 0; i < 10; i++) printf("%016lx %.17g\n", bits(tab[i]), tab[i]);
    for (i = 0; i < 5; i++) printf("%08x %.9g\n", fbits(ftab[i]), ftab[i]);
    printf("%016lx %016lx\n", bits(DBL_MAX), bits(DBL_EPSILON));
    printf("%08x %08x\n", fbits(FLT_MAX), fbits(FLT_MIN));
    /* constant-folded vs computed at run time */
    printf("%016lx %016lx\n", bits(1.0 / 3.0), bits(one / three));
    printf("%016lx %016lx\n", bits(0.1 + 0.2), bits(one / 10 + 2 * one / 10));
    {
        double x = 2.5;          /* local initializers */
        float y = -0.75f;
        double z = 1e-5 * 3;
        long double w = 0.5;
        printf("%g %g %g %g\n", x, (double)y, z, (double)w);
        x = 7; y = 'A'; z = 1 << 20;
        printf("%g %g %g\n", x, (double)y, z);
    }
    return 0;
}
