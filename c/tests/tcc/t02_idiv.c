/* signed 32-bit division and remainder (truncation toward zero) */
#include <stdio.h>
#include <limits.h>

int a[] = {0, 1, -1, 7, -7, 13, -13, 100, -100, 2147483647, -2147483647, INT_MIN, 65536, -65537};
int b[] = {1, -1, 2, -2, 3, -3, 7, -7, 10, 1000, -1000, 65536, INT_MAX, INT_MIN};

int main(void)
{
    int i, j, s = 0;
    for (i = 0; i < 14; i++)
        for (j = 0; j < 14; j++) {
            int q, r;
            if (a[i] == INT_MIN && b[j] == -1) continue; /* UB */
            q = a[i] / b[j];
            r = a[i] % b[j];
            printf("%d/%d=%d %d%%%d=%d\n", a[i], b[j], q, a[i], b[j], r);
            if (q * b[j] + r != a[i]) printf("IDENTITY FAIL\n");
            s += q ^ r;
        }
    /* division in conditions and as loop step */
    {
        int n = 1000000, steps = 0;
        while (n) { n /= 3; steps++; }
        printf("steps=%d\n", steps);
    }
    {
        int x = -17;
        x /= 4; printf("%d\n", x);
        x %= 3; printf("%d\n", x);
        x = -17; x %= -5; printf("%d\n", x);
    }
    /* char/short operands promote to int */
    {
        signed char c = -128; short sh = -32768; unsigned char uc = 255;
        printf("%d %d %d %d\n", c / 3, sh / 7, uc / 7, sh % 1000);
    }
    printf("s=%d\n", s);
    return s & 0xff;
}
