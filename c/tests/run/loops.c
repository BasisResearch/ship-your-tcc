/* loops, shifts, adds: no multiply/divide in the program itself */
#include <stdio.h>

static unsigned fib(unsigned n)
{
    unsigned a = 0, b = 1;
    while (n--) {
        unsigned t = a + b;
        a = b;
        b = t;
    }
    return a;
}

int main(void)
{
    unsigned sum = 0, x = 1;
    int i, j;
    for (i = 0; i < 100; i++)
        sum += i;
    printf("sum 0..99 = %u\n", sum);
    for (i = 0; i < 10; i++)
        x = (x << 1) ^ (x >> 3) ^ (unsigned)i;
    printf("mix = %u\n", x);
    for (i = 0; i < 4; i++) {
        for (j = 0; j <= i; j++)
            putchar('*');
        putchar('\n');
    }
    printf("fib(40) = %u\n", fib(40));
    i = 0;
    do { i += 3; } while (i < 50);
    printf("i = %d\n", i);
    return sum & 7;
}
