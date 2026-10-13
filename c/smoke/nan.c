#include <stdio.h>
/* NaN sign/payload: CompCert's RISC-V (and AArch64) semantics produce the
   positive default NaN 0x7ff8000000000000; x86 hardware produces 0xfff8... */
int main(void) {
  double z = 0.0;
  double n = z / z;
  union { double d; unsigned long long u; } b;
  b.d = n;
  printf("nan=%f bits=%llx isnan=%d\n", n, b.u, n != n);
  double m = -n;
  b.d = m;
  printf("neg nan=%f bits=%llx\n", m, b.u);
  return 0;
}
