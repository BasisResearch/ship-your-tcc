/* Reduced by hand from corpus/compcert/regr_int64 (diff3: lean_clight vs
 * gcc_native, xgcc_sail and ccomp_interp). Converting a 64-bit integer to
 * float must round once (C11 6.3.1.4p2, IEEE round-to-nearest-even).
 * 0x52ce6b4000000063 is just above the midpoint of two adjacent floats:
 * correct rounding gives 0x5ea59cd7; rounding to double first drops the
 * low bits, lands exactly on the midpoint, and ties-to-even then gives
 * 0x5ea59cd6 (double rounding). */
#include <stdio.h>

union fu { float f; unsigned u; };

int main(void) {
  unsigned long ux = 0x52ce6b4000000063UL;
  long sx = (long)ux;
  union fu a, b;
  a.f = (float)ux;
  b.f = (float)sx;
  printf("utof %x\n", a.u);
  printf("stof %x\n", b.u);
  return 0;
}
