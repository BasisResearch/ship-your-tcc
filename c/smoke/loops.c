#include <stdio.h>
int main(void) {
  int sum = 0;
  unsigned int h = 2166136261u;
  for (int i = 0; i < 1000; i++) {
    sum += i * i % 7;
    if (i % 3 == 0) continue;
    h = (h ^ (unsigned)i) * 16777619u;
  }
  int j = 0;
  while (j < 10) { j += 3; }
  do { j--; } while (j > 5);
  long long big = 1;
  for (int k = 0; k < 40; k++) big = big * 3 + k;
  printf("sum=%d h=%u j=%d big=%lld\n", sum, h, j, big);
  printf("hex=%x HEX=%08X oct=%o neg=%d char=%c str=%s pct=%%\n", 255u, 48879u, 8, -42, 'Z', "ok");
  printf("width=[%5d] left=[%-5d] plus=[%+d] long=%ld ulong=%lu\n", 42, 42, 42, 1234567890123L, 18446744073709551615UL);
  printf("hh=%hhd h=%hd\n", 300, 70000);
  return sum % 256;
}
