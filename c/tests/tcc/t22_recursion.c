/* recursion with doubles and integer mul/div */
#include <stdio.h>

double power(double x, int n)
{
    double h;
    if (n == 0) return 1.0;
    if (n < 0) return 1.0 / power(x, -n);
    h = power(x, n / 2);
    return n % 2 ? h * h * x : h * h;
}

double fibd(int n) { return n < 2 ? n : fibd(n - 1) + fibd(n - 2); }

long fact(int n) { return n <= 1 ? 1 : n * fact(n - 1); }

long gcd(long a, long b) { return b == 0 ? a : gcd(b, a % b); }

/* adaptive Simpson on x^2 * (1 - x) over [0,1] (exact: 1/12) */
static double f(double x) { return x * x * (1 - x); }
double simpson(double a, double b, double eps, double whole, int depth)
{
    double c = (a + b) / 2, l = (a + c) / 2, r = (c + b) / 2;
    double fa = f(a), fb = f(b), fc = f(c);
    double left = (c - a) / 6 * (fa + 4 * f(l) + fc);
    double right = (b - c) / 6 * (fc + 4 * f(r) + fb);
    if (depth <= 0 || (left + right - whole) * (left + right - whole) < 225 * eps * eps)
        return left + right + (left + right - whole) / 15;
    return simpson(a, c, eps / 2, left, depth - 1) + simpson(c, b, eps / 2, right, depth - 1);
}

int main(void)
{
    printf("%.17g %.17g %.17g\n", power(1.5, 10), power(2.0, -3), power(-0.5, 7));
    printf("%.17g\n", fibd(18));
    printf("%ld %ld\n", fact(20), fact(12) / fact(10));
    printf("%ld %ld\n", gcd(1071, 462), gcd(1234567890123L, 9876543210L));
    printf("%.17g\n", simpson(0, 1, 1e-9, (1.0 / 6) * (0 + 4 * f(0.5) + 0), 20));
    return 0;
}
