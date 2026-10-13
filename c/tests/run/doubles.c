/* doubles, floats, multiply/divide: needs the RV64I soft-float codegen
   patch (patches/0001); unpatched tcc emits F/D/M instructions here */
#include <stdio.h>

static double horner(const double *c, int n, double x)
{
    double r = 0;
    while (n--)
        r = r * x + c[n];
    return r;
}

int main(void)
{
    double c[] = { 1.0, -2.5, 0.5 };
    double x = 1.5, s = 0;
    float f = 3.25f;
    long a = 123456789, b = 1000;
    int i;
    for (i = 1; i <= 10; i++)
        s += 1.0 / i;
    printf("H(10) = %.6f\n", s);
    printf("p(1.5) = %g\n", horner(c, 3, x));
    printf("f * 2 = %f, (int)f = %d\n", f * 2, (int)f);
    printf("a * b = %ld, a / b = %ld, a %% b = %ld\n", a * b, a / b, a % b);
    printf("x < s: %d, x == 1.5: %d\n", x < s, x == 1.5);
    printf("(double)a = %.1f\n", (double)a);
    return 0;
}
