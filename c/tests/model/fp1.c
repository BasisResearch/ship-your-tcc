/* F3 model test: floating-point constant folding and conversions (gen_opif,
   gen_cast, soft-float helper calls, long double via __*tf* routines) */
#include <stdio.h>
float f[] = { 1.0f / 3.0f, 1e38f * 10.0f, -0.0f, 0.1f + 0.2f, (float)1e-45, (float)(double)16777217 };
double d[] = { 1.0 / 3.0, 1e308 * 10.0, 0.0 / 0.0, 0.1 + 0.2, 4.9e-324, (double)9007199254740993LL,
               (double)18446744073709551615ULL, (double)-9223372036854775807LL, 0x1.fffffffffffffp1023 };
long double ld[] = { 1.0L / 3.0L, 0x1.8p-16382L, 2.5L * 4.0L, -1.0L, (long double)1e300 * 1e300 };
int i[] = { (int)3.99, (int)-3.99, (int)2147483647.0, (unsigned)4294967295.0, (int)1e10f };
long long ll[] = { (long long)1e18, (long long)-9.2e18, (unsigned long long)1.8e19, (long long)1e30 };
_Bool b[] = { 0.0, 0.5, -0.0, 1e-300 };
int cmp[] = { 1.0 < 2.0, 2.0f == 2.0, 0.0 / 0.0 == 0.0 / 0.0, 0.0 / 0.0 != 0.0 / 0.0, -0.0 == 0.0,
              1.0L > 0.5L };
float ff(float a, float b) { return a * b - a / b + (a < b) + (a != b); }
double dd(double a, double b) { return a * b - a / b + (a <= b) + (a >= b) + (a == b); }
long double lf(long double a, long double b) { return a * b - a / b + (a > b); }
int fi(double x) { return (int)x + (unsigned)x + (long long)x + (unsigned long long)x; }
double id(int a, unsigned b, long long c, unsigned long long e) { return a + b + c + e; }
float fd(double x) { return (float)x; }
long double fl(float x) { return x; }
double ldd(long double x) { return (double)x; }
int main(void)
{
    volatile double z = 0;
    printf("%g %g %g\n", f[0], d[0], (double)ld[0]);
    printf("%g %g %g\n", ff(1.5f, 2.5f), dd(1.5, 2.5), (double)lf(1.5L, 2.5L));
    printf("%d %g %g %g %g\n", fi(3.7), id(-1, 2u, -3LL, 4ULL), fd(1.1), (double)fl(2.5f), ldd(7.25L));
    printf("%d %d %d %d\n", i[0], i[1], cmp[2], cmp[3]);
    printf("%lld %d %g\n", ll[0], b[1], -z);
    float x = 3.0f; x += 1; x *= x; x /= 3; x -= 0.5f;
    double y = 1.0; y = -y; y = y ? y : 1.0;
    long double w = 2.0L; w++; --w; w = w * w;
    printf("%g %g %g\n", x, y, (double)w);
    return 0;
}
