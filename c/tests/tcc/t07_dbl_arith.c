/* double + - * / and printing */
#include <stdio.h>

double v[] = {0.0, 1.0, -1.0, 0.5, 3.0, 0.1, -2.75, 1e10, 1e-10, 123456.789};
/* extremes: printing them with %.17g is expensive in dtoa, so only a few ops */
double ext[] = {1.7976931348623157e308, 2.2250738585072014e-308, 4.9406564584124654e-324};

double add(double a, double b) { return a + b; }
double sub(double a, double b) { return a - b; }
double mul(double a, double b) { return a * b; }
double dvd(double a, double b) { return a / b; }

int main(void)
{
    int i, j;
    for (i = 0; i < 10; i++)
        for (j = 0; j < 10; j += 3) {
            double a = v[i], b = v[j];
            printf("%.17g %.17g | %.17g %.17g %.17g", a, b, a + b, a - b, a * b);
            if (b != 0.0) printf(" %.17g", a / b);
            printf("\n");
            if (add(a, b) != a + b || sub(a, b) != a - b || mul(a, b) != a * b) printf("CALL MISMATCH\n");
            if (b != 0.0 && dvd(a, b) != a / b) printf("DIV MISMATCH\n");
        }
    for (i = 0; i < 3; i++)
        printf("%.17g %.17g %.17g %g %g\n", ext[i], ext[i] * 0.5, ext[i] / 3.0, ext[i] * 1e300, ext[i] - ext[i] * 0.75);
    printf("%g %f %e\n", 1.0 / 3.0, 2.0 / 3.0, 1e100 / 7.0);
    printf("%g %g %g\n", 0.1 + 0.2, 0.1 * 3, 1.0 - 0.9);
    {
        double x = 1.0, s = 0.0;
        for (i = 0; i < 50; i++) { s += x; x /= 2.0; }
        printf("geom %.17g\n", s);
        x = 1.0;
        for (i = 0; i < 20; i++) x = x * 1.5 - 0.25;
        printf("iter %.17g %.6f\n", x, x);
    }
    {   /* rounding: ties to even */
        double big = 9007199254740992.0; /* 2^53 */
        printf("%.17g %.17g %.17g\n", big + 1.0, big + 2.0, big + 3.0);
        printf("%.17g\n", (big + 1.0) - big);
    }
    {   /* overflow to inf, underflow to 0 (printed via comparisons) */
        double h = 1e308, t = 1e-308;
        double inf = h * 10.0, z = t / 1e100;
        printf("%d %d %d\n", inf > h, z == 0.0, -inf < -h);
    }
    return 0;
}
