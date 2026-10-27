/* CerbIEEE probe (F1b, results/csem/ieee-2026-10-27.md): what newlib's printf
   prints under `tcc -run` on Sail for double and long double, the NaN bit
   patterns libgcc's RISC-V soft-fp returns, and tcc's literal conversion.
   Run: scripts/run_tcc_sail.sh csem/lean_frontend/test/ieee/sail_probe_print.c */
#include <stdio.h>
#include <string.h>
typedef unsigned long long u64;

static u64 dbits(double x) { u64 u; memcpy(&u, &x, 8); return u; }
static unsigned fbits(float x) { unsigned u; memcpy(&u, &x, 4); return u; }
static void lbits(const char *tag, long double x) {
  u64 u[2]; memcpy(u, &x, 16);
  printf("%s %016llx%016llx\n", tag, u[1], u[0]);
}
static double dfrom(u64 u) { double x; memcpy(&x, &u, 8); return x; }

static void pd(double x) {
  printf("D[%f][%e][%g][%a][%.3f][%.0e][%#.0f][%+.2e][% g][%-12.4g|][%012.3f][%.20g][%A][%.3a][%#g][%G][%E][%.0a][%.1a]\n",
         x, x, x, x, x, x, x, x, x, x, x, x, x, x, x, x, x, x, x);
}
static void pl(long double x) {
  printf("L[%Lf][%Le][%Lg][%La][%.30Le][%.3La][%LA][%.36Lg]\n", x, x, x, x, x, x, x, x);
}

int main(void) {
  volatile double z = 0.0, one = 1.0;
  double inf = one / z, nan = z / z;
  double ds[] = {0.0, -0.0, 1.0, 0.1, 1.5, 2.5, 0.5, 1e300, 1e-300,
                 4.9406564584124654e-324, 2.2250738585072014e-308,
                 2.2250738585072009e-308, 1.7976931348623157e308,
                 123456789.125, 0.000123456, 1e-5, 100000.0, 1000000.0,
                 9.9999995, 0.3, 0.125, 1.0625, 1.03125, 3.0, -7.75};
  int i;
  for (i = 0; i < (int)(sizeof ds / sizeof ds[0]); i++) pd(ds[i]);
  pd(inf); pd(-inf); pd(nan); pd(-nan);
  printf("W[%5.1f][%-8e|][%+f][% f][%08.2f][%-+9.2f|][%+08.2e][%10s|]\n", 3.14159, 2.5, inf, nan, -1.5, 2.25, 31.0, "x");
  printf("W[%08f][%-8f|][%+e][%010e][%#.3g][%#.0e][%.0f][%.0f][%.0f][%.0f]\n", inf, -inf, -nan, inf, 1.0, 5.0, 0.5, 1.5, 2.5, 3.5);

  volatile long double lz = 0.0L, lone = 1.0L;
  long double linf = lone / lz, lnan = lz / lz;
  long double ls[] = {0.0L, -0.0L, 1.0L, 0.1L, 2.5L, 1e300L, 1e-300L, 1e4000L,
                      1e-4000L, 3.6451995318824746025284059336194e-4951L,
                      1.18973149535723176508575932662800702e4932L, 123456789.125L, 0.3L};
  for (i = 0; i < (int)(sizeof ls / sizeof ls[0]); i++) pl(ls[i]);
  pl(lone / 3); pl(linf); pl(-linf); pl(lnan); pl(-lnan);

  /* NaN bit patterns from the soft-fp routines */
  volatile float fz = 0.0f;
  float fnan = fz / fz;
  printf("N f-0/0 %08x\n", fbits(fnan));
  printf("N f-neg %08x\n", fbits(-fnan));
  printf("N d-0/0 %016llx\n", dbits(nan));
  printf("N d-inf-inf %016llx\n", dbits(inf - inf));
  printf("N d-0*inf %016llx\n", dbits(z * inf));
  printf("N d-neg %016llx\n", dbits(-nan));
  lbits("N l-0/0", lnan);
  lbits("N l-neg", -lnan);
  lbits("N l-inf-inf", linf - linf);
  volatile double snan = dfrom(0x7ff0000000000001ULL), pnan = dfrom(0xfff8000000001234ULL);
  printf("N d-snan+1 %016llx\n", dbits(snan + 1.0));
  printf("N d-pnan+1 %016llx\n", dbits(pnan + 1.0));
  printf("N d-pnan*1 %016llx\n", dbits(pnan * 1.0));
  printf("N d2f-snan %08x\n", fbits((float)snan));
  printf("N d2f-pnan %08x\n", fbits((float)pnan));
  printf("N d2f-nan %08x\n", fbits((float)nan));
  printf("N f2d-nan %016llx\n", dbits((double)fnan));
  lbits("N d2l-pnan", (long double)pnan);
  lbits("N d2l-snan", (long double)snan);
  printf("N l2d-nan %016llx\n", dbits((double)lnan));
  printf("N l2d-negnan %016llx\n", dbits((double)-lnan));
  lbits("N f2l-nan", (long double)fnan);
  printf("N l2f-nan %08x\n", fbits((float)lnan));

  /* literal conversion: double rounding in strtof? (correct: 3f800001) */
  printf("F lit-f-dr %08x\n", fbits(1.0000000596046447753906250000008271806f));
  /* halfway, decimal, exactly representable midpoint (ties-to-even: 3f800000) */
  printf("F lit-f-mid %08x\n", fbits(1.000000059604644775390625f));
  printf("F lit-d %016llx\n", dbits(2.2250738585072011e-308));
  printf("F lit-d-hex %016llx\n", dbits(0x1.fffffffffffff8p-1023));
  lbits("F lit-l", 0.1L);
  lbits("F lit-l-hex", 0x1.000000000000000000000000000081p0L);
  lbits("F lit-l-sub", 6.4751751194380251109244389582276465524996e-4966L);
  /* conversions */
  printf("C d2i %lld %lld %lld\n", (long long)2.9, (long long)-2.9, (long long)-0.5);
  printf("C l2i %lld %lld\n", (long long)(lone * 1e18L + 0.5L), (long long)-(lone * 7.99L));
  printf("C i2f %08x %016llx\n", fbits((float)16777217LL), dbits((double)9007199254740993LL));
  lbits("C i2l", (long double)(-9223372036854775807LL - 1));
  printf("C d2f %08x %08x\n", fbits((float)(one * 1.00000005960464477539)), fbits((float)(one * 3.4028235677973366e38)));
  return 0;
}
