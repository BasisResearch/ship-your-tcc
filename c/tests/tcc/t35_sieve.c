/* sieve of Eratosthenes, prime factorisation, digit sums (int mul/div/mod) */
#include <stdio.h>

#define N 3000
static char comp[N + 1];

int main(void)
{
    int i, j, cnt = 0;
    long sum = 0;
    for (i = 2; i * i <= N; i++)
        if (!comp[i])
            for (j = i * i; j <= N; j += i) comp[j] = 1;
    for (i = 2; i <= N; i++) if (!comp[i]) { cnt++; sum += i; }
    printf("primes<=%d: %d sum=%ld\n", N, cnt, sum);
    {
        long n = 600851475143L, f = 2, last = 0;
        while (n > 1) {
            if (n % f == 0) { printf("%ld ", f); n /= f; last = f; }
            else f++;
        }
        printf("| largest %ld\n", last);
    }
    {
        unsigned long x = 2; int k, ds = 0;
        for (k = 0; k < 60; k++) x *= 2;   /* 2^61 */
        while (x) { ds += (int)(x % 10); x /= 10; }
        printf("digitsum(2^61)=%d\n", ds);
    }
    {
        /* modular exponentiation */
        unsigned long base = 7, e = 1000000, m = 1000000007, r = 1;
        while (e) { if (e & 1) r = r * base % m; base = base * base % m; e >>= 1; }
        printf("7^1e6 mod p = %lu\n", r);
    }
    return cnt % 128;
}
