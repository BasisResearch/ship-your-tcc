/* Reduced by hand from corpus/tcc/t_134_double_to_signed (diff3:
 * lean_clight vs gcc_native, xgcc_sail and ccomp_interp). 1e19 lies in
 * [2^63, 2^64), so converting it to unsigned long is defined (C11
 * 6.3.1.4p1) and gives 10000000000000000000; the Lean port prints
 * 9223372036854775807 (it converts as if to signed long and saturates). */
#include <stdio.h>

int main(void) {
  double d = 1e19;
  printf("%lu\n", (unsigned long)d);
  printf("%llu\n", (unsigned long long)1e19);
  return 0;
}
