#include <stdio.h>
int main(void) {
  double h = 0.0;
  for (int i = 1; i <= 20; i++) h += 1.0 / i;
  float f = 1.0f / 3.0f;
  double x = 1e300 * 10.0;
  printf("h=%f h=%.17g h=%e\n", h, h, h);
  printf("f=%f f=%g third=%.3f\n", f, f, 1.0 / 3.0);
  printf("big=%g small=%g neg=%G int=%g\n", x * 1e10, 1e-310, -123456789.0, 100.0);
  printf("conv=%d %lld %u\n", (int)-3.7, (long long)1e18, (unsigned)3.99);
  printf("cmp=%d %d\n", 0.1 + 0.2 == 0.3, 0.5 + 0.25 == 0.75);
  printf("w=[%10.3f] [%-10.2e] [%+.0f]\n", 3.14159, 2.71828, 2.5);
  return 0;
}
