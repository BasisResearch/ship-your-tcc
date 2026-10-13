/* float arithmetic (single precision throughout, no FLT_EVAL widening) */
#include <stdio.h>

float v[] = {0.0f, 1.0f, -1.0f, 0.1f, 3.0f, -2.5f, 1e10f, 1e-10f, 16777216.0f, 3.4028235e38f, 1.17549435e-38f};

float fadd(float a, float b) { return a + b; }
float fmul(float a, float b) { return a * b; }
float fsub(float a, float b) { return a - b; }
float fdiv(float a, float b) { return a / b; }

int main(void)
{
    int i, j;
    for (i = 0; i < 11; i++)
        for (j = 0; j < 11; j += 3) {
            float a = v[i], b = v[j];
            float s = a + b, d = a - b, m = a * b;
            printf("%.9g %.9g | %.9g %.9g %.9g", a, b, s, d, m);
            if (b != 0.0f) { float q = a / b; printf(" %.9g", q); }
            printf("\n");
            if (fadd(a, b) != s || fsub(a, b) != d || fmul(a, b) != m) printf("CALL MISMATCH\n");
            if (b != 0.0f && fdiv(a, b) != a / b) printf("DIV MISMATCH\n");
        }
    {
        float x = 0.0f;
        for (i = 0; i < 1000; i++) x += 0.1f;
        printf("sum %.9g\n", x);
        x = 16777216.0f;
        x = x + 1.0f;
        printf("%.9g\n", x);
        x = 1.0f / 3.0f;
        printf("%.9g %.17g\n", x, (double)x);
    }
    {
        float a = 1.1f, b = 2.2f;
        double d = a * b; /* float multiply, then widen */
        printf("%.17g %.17g\n", d, (double)a * (double)b);
    }
    return 0;
}
