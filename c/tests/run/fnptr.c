/* function pointers, callbacks into libc (qsort), recursion */
#include <stdio.h>
#include <stdlib.h>

typedef int (*binop)(int, int);

static int add(int a, int b) { return a + b; }
static int sub(int a, int b) { return a - b; }
static int shl(int a, int b) { return a << b; }

static int cmp_int(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);
}

static int apply(binop f, int a, int b) { return f(a, b); }

static int ack(int m, int n)
{
    if (m == 0) return n + 1;
    if (n == 0) return ack(m - 1, 1);
    return ack(m - 1, ack(m, n - 1));
}

int main(void)
{
    static const binop ops[] = { add, sub, shl };
    static const char *names[] = { "add", "sub", "shl" };
    int v[] = { 42, -7, 19, 0, 3, 3, 100, -50 };
    int i;
    for (i = 0; i < 3; i++)
        printf("%s(12, 3) = %d\n", names[i], apply(ops[i], 12, 3));
    qsort(v, sizeof v / sizeof v[0], sizeof v[0], cmp_int);
    for (i = 0; i < 8; i++)
        printf("%d%c", v[i], i == 7 ? '\n' : ' ');
    printf("ack(2, 3) = %d\n", ack(2, 3));
    return 0;
}
