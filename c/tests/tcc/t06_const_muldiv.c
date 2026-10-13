/* multiply/divide/modulo by constants (powers of two may become shifts,
   others must call the helpers) */
#include <stdio.h>

int vals[] = {0, 1, -1, 5, -5, 17, -17, 1000, -1000, 123456, -123456, 214748364, -214748364};
int big[] = {2147483647, -2147483647, -2147483647 - 1, 1073741824};

int main(void)
{
    int i;
    long acc = 0;
    for (i = 0; i < 13; i++) {
        int v = vals[i];
        printf("%d: %d %d %d %d %d %d %d %d\n", v, v * 10, v / 10, v % 10, v * -3, v / -3, v % -3, v / 8, v % 8);
        printf("   %u %u %u %u\n", (unsigned)v / 10u, (unsigned)v % 10u, (unsigned)v / 16u, (unsigned)v % 16u);
        printf("   %ld %ld %ld\n", (long)v * 1000000007L, (long)v * 1000000007L / 97L, (long)v * 1000000007L % 97L);
        acc += v / 7 + v % 7;
    }
    for (i = 0; i < 4; i++)
        printf("big %d: %d %d %d %d %d %d\n", big[i], big[i] / 10, big[i] % 10, big[i] / 8, big[i] % 8, big[i] / -3, big[i] % 1000);
    {
        unsigned long x = 0xffffffffffffffffUL;
        printf("%lu %lu %lu %lu\n", x / 10, x % 10, x / 1024, x % 1024);
        long y = -9223372036854775807L;
        printf("%ld %ld %ld %ld\n", y / 10, y % 10, y / 1024, y % 1024);
    }
    printf("acc=%ld\n", acc);
    return 0;
}
