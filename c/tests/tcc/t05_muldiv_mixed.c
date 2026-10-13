/* compound assignment of * / %, pointer differences of odd-sized
   elements (divides by the element size), promotions */
#include <stdio.h>

struct s12 { int a, b, c; };
struct s24 { long x; int y[4]; };

int main(void)
{
    struct s12 arr[50];
    struct s24 arr2[9];
    struct s12 *p = &arr[3], *q = &arr[47];
    struct s24 *p2 = &arr2[1], *q2 = &arr2[8];
    long d = q - p, d2 = p - q;
    char c = 100; short sh = -1234; unsigned short us = 50000;
    int i = 77; unsigned u = 4000000000u; long l = -5000000000L; unsigned long ul = 12345678901234567UL;

    printf("ptrdiff %ld %ld %ld %ld\n", d, d2, (long)(q2 - p2), (long)(p2 - q2));
    c *= 2; printf("c=%d\n", c);       /* unsigned char on riscv: 200 */
    c /= 3; printf("c=%d\n", c);
    sh *= 3; printf("sh=%d\n", sh);
    sh /= -7; printf("sh=%d\n", sh);
    sh %= 10; printf("sh=%d\n", sh);
    us *= 2; printf("us=%u\n", us);    /* wraps to 34464 */
    us %= 1000; printf("us=%u\n", us);
    i *= -3; i /= 2; i %= 13; printf("i=%d\n", i);
    u *= 3u; printf("u=%u\n", u);
    u /= 7u; printf("u=%u\n", u);
    u %= 100000u; printf("u=%u\n", u);
    l *= 3; l /= -7; l %= 100000007L; printf("l=%ld\n", l);
    ul *= 1000; ul /= 3; ul %= 1000000000000UL; printf("ul=%lu\n", ul);
    /* mixed signed/unsigned: usual arithmetic conversions */
    {
        int m = -7; unsigned n = 2;
        printf("%u %u\n", m / n, m % n);   /* -7 converted to unsigned */
        printf("%ld %ld\n", (long)m / (long)n, (long)m % (long)n);
        printf("%lu\n", (unsigned long)m / 3);
    }
    /* array indexing scaled by non-power-of-2 sizes */
    for (i = 0; i < 50; i++) arr[i].a = i * i, arr[i].b = i * 3, arr[i].c = -i;
    {
        int k = 7;
        printf("%d %d %d\n", arr[k * 3].a, (p + k)->b, q[-k].c);
    }
    return 0;
}
