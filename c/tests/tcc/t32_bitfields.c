/* bitfields mixed with multiply/divide and floats (bitfield code itself
   is untouched by the patch) */
#include <stdio.h>

struct bf {
    unsigned a : 3;
    signed b : 5;
    unsigned c : 12;
    signed d : 20;
    unsigned long e : 40;
};

int main(void)
{
    struct bf x = {5, -9, 4000, -300000, 0xfffffffffUL};
    int i;
    double acc = 0;
    printf("%u %d %u %d %lu\n", x.a, x.b, x.c, x.d, (unsigned long)x.e);
    x.a *= 3; x.b /= 2; x.c %= 1000; x.d *= -3; x.e = x.e * 7 / 3;
    printf("%u %d %u %d %lu\n", x.a, x.b, x.c, x.d, (unsigned long)x.e);
    for (i = 0; i < 20; i++) {
        x.b = i - 10;
        x.c = i * 205;
        acc += x.b * 0.5 + x.c / 3.0;
        x.d = x.d / 7 + i * 1000;
    }
    printf("%.17g %d\n", acc, x.d);
    x.c = 3.99; x.b = -7.9;           /* float -> bitfield */
    printf("%u %d %g\n", x.c, x.b, (double)x.d / x.c);
    return 0;
}
