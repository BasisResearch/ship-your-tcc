/* hashing: FNV-1a 32/64, multiplicative hashing, LCG, CRC-ish mixing */
#include <stdio.h>
#include <string.h>

static unsigned fnv32(const char *s) { unsigned h = 2166136261u; while (*s) { h ^= (unsigned char)*s++; h *= 16777619u; } return h; }
static unsigned long fnv64(const char *s) { unsigned long h = 14695981039346656037UL; while (*s) { h ^= (unsigned char)*s++; h *= 1099511628211UL; } return h; }

int main(void)
{
    const char *words[] = {"", "a", "tcc", "riscv", "The quick brown fox jumps over the lazy dog"};
    int i, buckets[13] = {0};
    unsigned seed = 12345;
    for (i = 0; i < 5; i++) printf("%08x %016lx\n", fnv32(words[i]), fnv64(words[i]));
    for (i = 0; i < 1000; i++) {
        seed = seed * 1103515245u + 12345u;            /* 32-bit LCG wraps */
        buckets[(seed >> 16) % 13]++;
    }
    for (i = 0; i < 13; i++) printf("%d ", buckets[i]);
    printf("\n");
    {
        unsigned long x = 0x9e3779b97f4a7c15UL; int k;
        for (k = 0; k < 10; k++) { x ^= x >> 30; x *= 0xbf58476d1ce4e5b9UL; x ^= x >> 27; x *= 0x94d049bb133111ebUL; x ^= x >> 31; }
        printf("%016lx %lu\n", x, x % 1000003UL);
    }
    {
        int signed_mix = 0;
        for (i = -50; i < 50; i++) signed_mix = signed_mix * 31 % 1000003 + i * i % 97 - i / 3;
        printf("%d\n", signed_mix);
    }
    return (int)(fnv32("exit") & 0x7f);
}
