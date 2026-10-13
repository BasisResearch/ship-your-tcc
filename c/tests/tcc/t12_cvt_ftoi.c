/* float/double -> integer conversions (truncation toward zero), only
   values whose result is representable (others are undefined) */
#include <stdio.h>

double vd[] = {0.0, -0.0, 0.9, -0.9, 1.5, -1.5, 2.5, -2.5, 123456789.987, -123456789.987, 2147483647.9, -2147483648.9};
double vdu[] = {0.0, 0.99, 2147483648.0, 3000000000.5, 4294967295.9};
double vdl[] = {9007199254740991.0, -9007199254740991.0, 9.2233720368547748e18, -9.2233720368547758e18, 1e15 + 0.5};
double vdul[] = {9.2233720368547758e18, 1.8446744073709550e19, 1e19, 12345.678};
float vf[] = {0.5f, -0.5f, 3.99f, -3.99f, 16777216.0f, -2147483648.0f, 2147483520.0f};

int d2i(double x) { return x; }
unsigned d2u(double x) { return x; }
long f2l(float x) { return x; }
unsigned long d2ul(double x) { return x; }

int main(void)
{
    int i;
    for (i = 0; i < 12; i++) printf("d %.17g -> %d %ld\n", vd[i], (int)vd[i], (long)vd[i]);
    for (i = 0; i < 5; i++) printf("du %.17g -> %u %lu\n", vdu[i], (unsigned)vdu[i], (unsigned long)vdu[i]);
    for (i = 0; i < 5; i++) printf("dl %.17g -> %ld\n", vdl[i], (long)vdl[i]);
    for (i = 0; i < 4; i++) printf("dul %.17g -> %lu\n", vdul[i], (unsigned long)vdul[i]);
    for (i = 0; i < 7; i++) printf("f %.9g -> %d %ld %u\n", vf[i], (int)vf[i], (long)vf[i], vf[i] >= 0 ? (unsigned)vf[i] : 0u);
    printf("%d %u %ld %lu\n", d2i(-77.7), d2u(4000000000.25), f2l(-1e10f), d2ul(1.5e19));
    {
        double d = 200.7; float f = 65535.5f;
        unsigned char uc = d; short s = -d; unsigned short us = f; signed char sc = -d / 2;
        char c = d / 2;
        printf("%d %d %d %d %d\n", uc, s, us, sc, c);
    }
    {   /* implicit conversion on assignment and in compound assignment */
        int n = 10; long l = 100; unsigned u = 7;
        n *= 2.5; l /= 0.3; u += 0.9;
        printf("%d %ld %u\n", n, l, u);
        n = 3.99; printf("%d\n", n);
    }
    return 0;
}
