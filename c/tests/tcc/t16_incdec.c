/* ++/-- on doubles and floats (pre/post), float loop counters */
#include <stdio.h>

double g = 0.5;

int main(void)
{
    double d = 1.25, *p = &d, arr[3] = {0.5, 1.5, 2.5};
    float f = -0.5f;
    int i, n = 0;
    double a = d++;
    double b = ++d;
    printf("%g %g %g\n", a, b, d);
    a = d--; b = --d;
    printf("%g %g %g\n", a, b, d);
    (*p)++; ++*p; printf("%g\n", d);
    a = f++; b = ++f;
    printf("%g %g\n", a, b);
    a = f--; b = --f;
    printf("%g %g %g\n", a, b, (double)f);
    g++; g++; --g; printf("%g\n", g);
    for (i = 0; i < 3; i++) arr[i]++, ++arr[i];
    printf("%g %g %g\n", arr[0], arr[1], arr[2]);
    for (d = 0.0; d < 5.0; d++) n++;
    for (f = 10.0f; f > 0.0f; f--) n++;
    printf("n=%d\n", n);
    {
        double x = 1e16;   /* x + 1 rounds back to x */
        x++;
        printf("%.17g\n", x);
        float y = 16777216.0f;
        y++;
        printf("%.9g\n", y);
    }
    return 0;
}
