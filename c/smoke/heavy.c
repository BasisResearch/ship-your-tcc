#include <stdio.h>
/* throughput test: ~1M loop iterations with arrays, calls and 64-bit math */
static unsigned long long mix(unsigned long long x) {
  x ^= x >> 33; x *= 0xff51afd7ed558ccdULL; x ^= x >> 33;
  return x;
}
int main(void) {
  static unsigned int tab[256];
  unsigned long long acc = 0;
  for (int i = 0; i < 256; i++) tab[i] = (unsigned)i * 2654435761u;
  for (int r = 0; r < 4000; r++)
    for (int i = 0; i < 256; i++)
      acc += mix(acc ^ tab[(i + r) & 255]);
  printf("acc=%llu\n", acc);
  return 0;
}
