/* float comparisons incl. NaN */
#include <stdio.h>

int cmp(float a, float b)
{
    return (a == b) | (a != b) << 1 | (a < b) << 2 | (a <= b) << 3 | (a > b) << 4 | (a >= b) << 5;
}

int main(void)
{
    volatile float zero = 0.0f;
    float v[7];
    int i, j, s = 0;
    v[0] = 0.0f; v[1] = -zero; v[2] = 1.5f; v[3] = -1.5f; v[4] = 1.0f / zero; v[5] = zero / zero; v[6] = 1e-40f;
    for (i = 0; i < 7; i++) {
        for (j = 0; j < 7; j++) {
            int r = cmp(v[i], v[j]);
            printf("%02x ", r);
            s += r * (i + 1);
        }
        printf("\n");
    }
    {
        float f = 0.1f; double d = 0.1;
        printf("%d %d %d\n", f == d, f > d, (double)f == (double)0.1f);
        printf("%d %d\n", f < 0.10000001f, f >= 0.1f);
    }
    {
        float x = 100.0f; int n = 0;
        while (x > 1.0f) { x /= 1.5f; n++; }
        printf("n=%d x=%.9g\n", n, x);
    }
    printf("s=%d\n", s);
    return 0;
}
