#include <stdio.h>
int main(void) {
  float f=0.1f;
  long double q=0x1.0000000000000000000000000001p0L;
  printf("[%+.2f][%08.2f][%-9.3e][%#.4g][%a]\n", (double)f, -1.5, 0.125, 1.0, -0.0);
  printf("[%.36Lf][%La][%.4LE][%Lg]\n", q, q, 0x1p100L, 1e4000L);
  return 0;
}
