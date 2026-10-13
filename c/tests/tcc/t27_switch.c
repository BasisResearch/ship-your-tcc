/* switch with arithmetic in cases; switch on converted doubles */
#include <stdio.h>

int classify(double d)
{
    switch ((int)(d * 4)) {
    case 0: return 0;
    case 1: case 2: return 1;
    case 3: return 2;
    case -1: return -1;
    default: return d > 0 ? 9 : -9;
    }
}

long eval(int op, long a, long b)
{
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/': return b ? a / b : 0;
    case '%': return b ? a % b : 0;
    case 'u': return (long)((unsigned)a / (unsigned)(b ? b : 1));
    default: return -1;
    }
}

int main(void)
{
    double ds[] = {0.0, 0.3, 0.6, 0.8, -0.3, 5.0, -5.0};
    const char *ops = "+-*/%ux";
    long as[] = {100, -7, 0x7fffffff, -3000000000L};
    long bs[] = {3, -2, 1000, 0};
    int i, j, k;
    for (i = 0; i < 7; i++) printf("%d ", classify(ds[i]));
    printf("\n");
    for (k = 0; ops[k]; k++)
        for (i = 0; i < 4; i++)
            for (j = 0; j < 4; j++)
                printf("%c %ld %ld = %ld\n", ops[k], as[i], bs[j], eval(ops[k], as[i], bs[j]));
    {
        int n = 0, x;
        for (x = 0; x < 100; x++) {
            switch (x % 7) {
            case 0: n += x / 7; break;
            case 3: n -= x % 5; /* fallthrough */
            case 4: n *= 1; n += 2; break;
            default: n++;
            }
        }
        printf("n=%d\n", n);
    }
    return 0;
}
