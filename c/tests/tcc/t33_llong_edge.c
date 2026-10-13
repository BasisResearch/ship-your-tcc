/* 64-bit edge cases: wrap-around of unsigned, extremes, mixed signs */
#include <stdio.h>
#include <limits.h>

long smul(long a, long b) { return a * b; }
unsigned long umul(unsigned long a, unsigned long b) { return a * b; }

int main(void)
{
    unsigned long a = ULONG_MAX, b = 0x8000000000000000UL;
    long c = LONG_MAX, d = LONG_MIN;
    printf("%lu %lu %lu\n", a * a, a * 2, b * 2);
    printf("%lu %lu %lu %lu\n", a / b, a % b, b / 3, (a - 5) % 1000000007UL);
    printf("%ld %ld %ld %ld\n", c / 2, c % 1000, d / 2, d % 1000);
    printf("%ld %ld %ld\n", d / LONG_MAX, d % LONG_MAX, LONG_MAX / d);
    printf("%ld %ld\n", smul(-4294967296L, 2147483647L), smul(3037000499L, 3037000499L));
    printf("%lu\n", umul(0xffffffffUL, 0xffffffffUL));
    printf("%ld %ld %ld %ld\n", -7L / 2, -7L % 2, 7L / -2, 7L % -2);
    {   /* long long is the same as long on lp64 */
        long long x = -1;
        unsigned long long y = (unsigned long long)x / 3;
        printf("%llu %lld %llu\n", y, x / 3, y % 10);
    }
    {
        int i; unsigned long h = 1469598103934665603UL;
        for (i = 0; i < 1000; i++) h = (h ^ (unsigned long)i) * 1099511628211UL;
        printf("%016lx %lu\n", h, h % 9973);
    }
    return 0;
}
