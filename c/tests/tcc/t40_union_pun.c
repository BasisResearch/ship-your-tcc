/* type punning through unions and memcpy: doubles/floats stored by tcc
   must have exactly the IEEE bit patterns */
#include <stdio.h>
#include <string.h>

union du { double d; unsigned long u; unsigned w[2]; };
union fu { float f; unsigned u; unsigned char b[4]; };

double from_bits(unsigned long u) { union du x; x.u = u; return x.d; }

int main(void)
{
    union du a; union fu b;
    double arr[3] = {1.0, -0.5, 1e-310};
    unsigned long raw[3];
    int i;
    a.d = 1.0; printf("%016lx %08x %08x\n", a.u, a.w[1], a.w[0]);
    a.d = -2.5e-3; printf("%016lx\n", a.u);
    a.u = 0x400921fb54442d18UL; printf("%.17g\n", a.d);
    b.f = 1.0f; printf("%08x\n", b.u);
    b.f = -0.1f; printf("%08x %02x %02x\n", b.u, b.b[0], b.b[3]);
    b.u = 0x7f7fffffu; printf("%.9g\n", b.f);
    memcpy(raw, arr, sizeof arr);
    for (i = 0; i < 3; i++) printf("%016lx ", raw[i]);
    printf("\n");
    printf("%.17g %.17g\n", from_bits(0x3fb999999999999aUL), from_bits(0x0000000000000001UL) * 0x1p1074);
    {
        /* build a double from integer arithmetic and back */
        unsigned long m = (1UL << 52) | 12345, e = 1023 + 7;
        double d = from_bits((e << 52) | (m & ((1UL << 52) - 1)));
        printf("%.17g %ld\n", d, (long)d);
    }
    return 0;
}
