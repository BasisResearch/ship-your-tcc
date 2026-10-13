/* usual arithmetic conversions across int/unsigned/long/float/double */
#include <stdio.h>

int main(void)
{
    int i = -1; unsigned u = 1; long l = -1; unsigned long ul = 1;
    float f = 0.5f; double d = 0.25; char c = 200; short s = -2;
    printf("%d %d %d %d\n", i < u, l < u, i < ul, l < (long)ul);
    printf("%g %g %g\n", i + f, u + d, l * f);
    printf("%g %g\n", (double)(i + u), (double)(l + u));
    printf("%u %ld %lu\n", i + u, l + u, i + ul);
    printf("%g %g %g\n", c * f, s / d, c / s * d);
    printf("%d %d %d\n", (int)(f + d + c), (int)(-f - 1.5), (int)(u - 2.5));
    printf("%g\n", (double)(unsigned)i + (double)i);
    printf("%g %g\n", (float)u / 3, (double)ul / 3);
    printf("%d %d %d\n", f > i, d < u, -0.0 < i);
    {
        unsigned char a = 255, b = 1;
        int sum = a + b;                 /* promoted to int: 256 */
        unsigned char wrap = a + b;      /* 0 */
        printf("%d %d %g\n", sum, wrap, a * 1.0 / b);
    }
    return 0;
}
