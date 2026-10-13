/* float <-> double conversions (rounding, subnormals, overflow to inf) */
#include <stdio.h>

double vd[] = {0.1, 1.0 / 3.0, 3.4028235677973366e38, 3.4028236e38, 1e39, 1.17549435e-38, 1.4e-45, 7e-46, 1e-50, -2.5, 16777217.0, 16777219.0, 0.30000001192092896};

float d2f(double d) { return d; }
double f2d(float f) { return f; }

int main(void)
{
    int i;
    for (i = 0; i < 13; i++) {
        float f = (float)vd[i];
        double back = f;
        printf("%.17g -> %.9g -> %.17g %d\n", vd[i], f, back, back == vd[i]);
        if (d2f(vd[i]) != f) printf("CALL MISMATCH d2f\n");
        if (f2d(f) != back) printf("CALL MISMATCH f2d\n");
    }
    {
        float a = 0.1f;
        double b = a;          /* implicit widening */
        float c = b * 3.0;     /* double multiply, narrow on store */
        float e = a * 3.0f;    /* float multiply */
        printf("%.17g %.9g %.9g %d\n", b, c, e, c == e);
    }
    {
        float acc = 0.0f;
        double dacc = 0.0;
        for (i = 1; i <= 100; i++) { acc += 1.0 / i; dacc += 1.0f / i; }
        printf("%.9g %.17g\n", acc, dacc);
    }
    return 0;
}
