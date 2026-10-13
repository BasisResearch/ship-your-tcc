/* doubles in boolean contexts: _Bool conversion, !, if, &&, ||, ?: */
#include <stdio.h>

int main(void)
{
    volatile double zero = 0.0;
    double v[6];
    float fv[3];
    int i;
    v[0] = 0.0; v[1] = -zero; v[2] = 1e-320; v[3] = -3.5; v[4] = zero / zero; v[5] = 1.0 / zero;
    fv[0] = 0.0f; fv[1] = 0.25f; fv[2] = -1e-40f;
    for (i = 0; i < 6; i++) {
        double d = v[i];
        _Bool b = d;
        int n = !d, nn = !!d;
        int t = 0;
        if (d) t |= 1;
        if (!d) t |= 2;
        if (d && 1) t |= 4;
        if (d || 0) t |= 8;
        if (d && v[3]) t |= 16;
        t |= (d ? 32 : 0);
        while (d) { t |= 64; break; }
        printf("%d: %d %d %d %d\n", i, b, n, nn, t);
    }
    for (i = 0; i < 3; i++) {
        float f = fv[i];
        _Bool b = f;
        printf("f%d: %d %d %d\n", i, b, !f, f ? 1 : 2);
    }
    {
        double x = 3.0; int n = 0;
        do { x -= 0.5; n++; } while (x);
        printf("n=%d\n", n);
        _Bool arr[3] = {0.5, 0.0, -0.0};
        printf("%d %d %d\n", arr[0], arr[1], arr[2]);
    }
    return 0;
}
