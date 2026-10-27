/* Values just above halfway: round once to each destination format. */
int printf(const char *, ...);
static float f = 0x1.0000010000000000000000000000000000000001p0f;
static double d = 0x1.0000000000000800000000000000000000000001p0;
static long double q = 0x1.0000000000000000000000000000800000000001p0L;
static float decimal = 1.0000000596046447753906250000008271806f;
static void bytes(const void *p, unsigned n) {
    const unsigned char *b = p;
    unsigned i;
    for (i = n; i; --i) printf("%02x", (unsigned)b[i-1]);
    printf("\n");
}
int main(void) {
    bytes(&f, sizeof f);
    bytes(&d, sizeof d);
    bytes(&q, sizeof q);
    bytes(&decimal, sizeof decimal);
    return 0;
}
