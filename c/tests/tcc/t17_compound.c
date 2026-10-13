/* compound assignment with float/double operands, incl. int lvalues */
#include <stdio.h>

struct pt { double x, y; float w; };

int main(void)
{
    double d = 10.0, a[4] = {1, 2, 3, 4};
    float f = 3.0f;
    int i = 7;
    long l = -100;
    unsigned u = 50;
    struct pt p = {1.0, 2.0, 0.5f}, *pp = &p;
    d += 2.5; d -= 0.25; d *= 3.0; d /= 4.0;
    printf("%.17g\n", d);
    f += 0.1f; f -= 2; f *= 1.5; f /= 0.7f;
    printf("%.9g\n", f);
    d += f; f += d;
    printf("%.17g %.9g\n", d, f);
    i += 2.9; printf("%d\n", i);
    i *= 1.5; printf("%d\n", i);
    i -= 0.5; printf("%d\n", i);
    i /= 0.5; printf("%d\n", i);
    l *= -0.75; printf("%ld\n", l);
    u /= 3.0; printf("%u\n", u);
    for (i = 0; i < 4; i++) { a[i] *= a[i]; a[i] += i; a[i] /= 2; }
    printf("%g %g %g %g\n", a[0], a[1], a[2], a[3]);
    pp->x += 10; pp->y *= pp->x; pp->w -= 0.25; p.x /= p.w;
    printf("%g %g %g\n", p.x, p.y, p.w);
    {
        double *q = a;
        *q++ += 100; *q += *q;
        printf("%g %g\n", a[0], a[1]);
    }
    return 0;
}
