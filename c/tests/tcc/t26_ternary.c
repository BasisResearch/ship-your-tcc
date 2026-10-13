/* ?: on doubles/floats, incl. mixed operand types */
#include <stdio.h>

double pick(int c, double a, double b) { return c ? a : b; }
double dmax(double a, double b) { return a > b ? a : b; }
float fmin_(float a, float b) { return a < b ? a : b; }

int main(void)
{
    int i;
    double s = 0;
    for (i = -3; i <= 3; i++) {
        double d = i ? 1.0 / i : 0.0;
        float f = i > 0 ? (float)i * 0.5f : -0.25f;
        double m = i & 1 ? i : 2.5;        /* int vs double -> double */
        long double l = i < 0 ? -1.0L : 1;  /* long double */
        s += d + f + m + (double)l;
        printf("%d: %.17g %.9g %g %g\n", i, d, f, m, (double)l);
    }
    printf("%.17g\n", s);
    printf("%g %g %g %g\n", pick(1, 2, 3), pick(0, 2, 3), dmax(-1, -2), (double)fmin_(1.5f, -0.5f));
    {
        double a = 3, b = 4;
        double *p = a > b ? &a : &b;
        *p = 10;
        (a < b ? a : b) == 3 ? printf("ok\n") : printf("no\n");
        printf("%g %g %g\n", a, b, (a > 5 ? a : b) * 2);
        int k = a > 2 ? 1.9 : 2.9;           /* double result converted */
        printf("%d\n", k);
    }
    return 0;
}
