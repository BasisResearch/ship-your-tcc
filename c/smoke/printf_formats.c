#include <stdio.h>
/* printf conversions the two oracles must render identically */
int main(void) {
  double vals[] = { 0.0, -0.0, 1.0, -1.5, 0.1, 123456.789, 1e-5, 1e21, 2.5e-308,
                    1.0 / 3.0, 9.9999995, 0.5, 1.5, 2.5, 1e100 };
  int n = (int)(sizeof vals / sizeof vals[0]);
  for (int i = 0; i < n; i++) {
    double v = vals[i];
    printf("%d: %f|%.0f|%.2f|%e|%.3E|%g|%.10g|%G|%#g|%+.1f|% e|%12.4g|%-12.1e|\n",
           i, v, v, v, v, v, v, v, v, v, v, v, v, v);
  }
  double inf = 1e308 * 10.0;
  printf("inf: %f %e %g %F %5.1f\n", inf, -inf, inf, inf, -inf);
  printf("ints: [%5d] [%-5d] [%05d] [%+d] [% d] [%.3d] [%8.3d] [%-+6d]\n", -42, -42, -42, 42, 42, 7, 7, 7);
  printf("unsigned: %u %x %X %o %#x %#o %#X %08x\n", 4294967295u, 3735928559u, 3735928559u, 511u, 255u, 8u, 48879u, 255u);
  printf("long long: %lld %llu %llx %lli %20lld|%-20lld|\n", -9223372036854775807LL - 1, 18446744073709551615ULL,
         0x123456789abcdefULL, 123LL, -1LL, 1LL);
  printf("long: %ld %lu %lx %zu %zd\n", -2147483649L, 4294967296UL, 0xdeadbeefcafeUL, sizeof(double), (long)-5);
  printf("short/char: %hd %hu %hhd %hhu %hx\n", 32768, 65537, 255, 257, 65535);
  printf("star: [%*d] [%-*d] [%.*f] [%*.*e]\n", 6, 1, 6, 2, 3, 3.14159, 12, 2, 314.159);
  printf("strings: [%s] [%10s] [%-10s] [%.3s] [%5.2s]\n", "abc", "right", "left", "truncate", "xyz");
  printf("chars: [%c] [%3c] [%-3c]\n", 'a', 'b', 'c');
  printf("percent: 100%% done\n");
  return 0;
}
