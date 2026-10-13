/* unsigned 32-bit division/remainder with values >= 2^31 (needs the
   zero-extension before __udivdi3: tcc keeps 32-bit values sign-extended) */
#include <stdio.h>

unsigned a[] = {0u, 1u, 7u, 0x7fffffffu, 0x80000000u, 0x80000001u, 0xfffffff0u, 0xffffffffu, 3000000000u, 123456789u};
unsigned b[] = {1u, 2u, 3u, 7u, 10u, 0x10000u, 0x7fffffffu, 0x80000000u, 0xffffffffu, 4000000000u};

unsigned udiv(unsigned x, unsigned y) { return x / y; }
unsigned umod(unsigned x, unsigned y) { return x % y; }

int main(void)
{
    int i, j;
    unsigned h = 0;
    for (i = 0; i < 10; i++)
        for (j = 0; j < 10; j++) {
            unsigned q = a[i] / b[j], r = a[i] % b[j];
            printf("%u/%u=%u r%u\n", a[i], b[j], q, r);
            if (q != udiv(a[i], b[j]) || r != umod(a[i], b[j])) printf("MISMATCH\n");
            if (q * b[j] + r != a[i]) printf("IDENTITY FAIL\n");
            h = h * 33 + q + r;
        }
    {
        unsigned x = 0xffffffffu;
        x /= 16u; printf("%u\n", x);
        x %= 1000u; printf("%u\n", x);
        x = 0x80000000u; x /= 3u; printf("%u\n", x);
    }
    /* unsigned short/char promote to (signed) int: signed division */
    {
        unsigned short us = 65535; unsigned char uc = 200;
        printf("%d %d\n", us / 7, us % uc);
    }
    /* digits of UINT_MAX by repeated division */
    {
        unsigned v = 4294967295u; char buf[16]; int n = 0;
        do { buf[n++] = '0' + v % 10u; v /= 10u; } while (v);
        while (n--) putchar(buf[n]);
        putchar('\n');
    }
    printf("h=%u\n", h);
    return (int)(h % 200u);
}
