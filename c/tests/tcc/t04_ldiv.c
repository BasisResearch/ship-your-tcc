/* 64-bit division/remainder, signed and unsigned */
#include <stdio.h>
#include <limits.h>

long a[] = {0L, 1L, -1L, 7L, -7L, LONG_MAX, LONG_MIN, 1000000000000L, -999999999999L, 4294967296L, -4294967297L};
long b[] = {1L, -1L, 2L, -3L, 10L, 4294967296L, -4294967296L, LONG_MAX, LONG_MIN, 1000003L};
unsigned long ua[] = {0UL, 1UL, ULONG_MAX, 0x8000000000000000UL, 0xfedcba9876543210UL, 10000000000000000000UL};
unsigned long ub[] = {1UL, 2UL, 3UL, 10UL, 0xffffffffUL, 0x100000000UL, 0x8000000000000000UL, ULONG_MAX};

int main(void)
{
    int i, j;
    unsigned long h = 0;
    for (i = 0; i < 11; i++)
        for (j = 0; j < 10; j++) {
            long q, r;
            if (a[i] == LONG_MIN && b[j] == -1) continue;
            q = a[i] / b[j]; r = a[i] % b[j];
            printf("%ld/%ld=%ld r%ld\n", a[i], b[j], q, r);
            if (q * b[j] + r != a[i]) printf("IDENTITY FAIL\n");
            h = h * 131 + (unsigned long)q + (unsigned long)r;
        }
    for (i = 0; i < 6; i++)
        for (j = 0; j < 8; j++) {
            unsigned long q = ua[i] / ub[j], r = ua[i] % ub[j];
            printf("%lu/%lu=%lu r%lu\n", ua[i], ub[j], q, r);
            h = h * 131 + q + r;
        }
    {
        unsigned long v = ULONG_MAX; char buf[32]; int n = 0;
        do { buf[n++] = '0' + (char)(v % 10); v /= 10; } while (v);
        while (n--) putchar(buf[n]);
        putchar('\n');
    }
    {
        long long x = -123456789012345LL;
        x /= 1000; printf("%lld\n", x);
        x %= 977; printf("%lld\n", x);
    }
    printf("h=%lx\n", h);
    return 0;
}
