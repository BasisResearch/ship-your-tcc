/* old-style (K&R) definitions with float parameters (default argument
   promotion), unprototyped calls, and long double through varargs */
#include <stdio.h>
#include <stdarg.h>

double kr(a, b, c)
    float a; double b; int c;
{
    return a * 2 + b + c;
}

float krf(x)
    float x;
{
    return x / 4;
}

double ldsum(int n, ...)
{
    va_list ap;
    long double s = 0;
    int i;
    va_start(ap, n);
    for (i = 0; i < n; i++) s += va_arg(ap, long double);
    va_end(ap);
    return (double)s;
}

double mixva(int n, ...)
{
    va_list ap;
    double s = 0;
    int i;
    va_start(ap, n);
    for (i = 0; i < n; i++) {
        if (i % 2) s += (double)va_arg(ap, long double);
        else s += va_arg(ap, int);
    }
    va_end(ap);
    return s;
}

int main(void)
{
    printf("%g\n", kr(1.5f, 0.25, 3));
    printf("%g\n", krf(10.0f));
    printf("%g\n", ldsum(3, 1.5L, 2.25L, -0.75L));
    printf("%g\n", ldsum(5, 1.0L, 2.0L, 3.0L, 4.0L, 5.0L));
    printf("%g\n", mixva(6, 1, 0.5L, 2, 0.25L, 3, 0.125L));
    return 0;
}
