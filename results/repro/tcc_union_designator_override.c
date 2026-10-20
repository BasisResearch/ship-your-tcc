/* Reduced by hand from corpus/compcert/regr_initializers2 (diff3: tcc_sail
 * vs gcc_native, xgcc_sail, ccomp_interp and lean_clight). The second
 * designator names a different member of the union, so it initializes `c`
 * and the members not named are zero (C11 6.7.9p17, p19, p21). gcc,
 * CompCert and the Lean Clight port print 0 0 1 0; tcc keeps the bytes of
 * the overridden `.a = 42` and prints 42 0 1 0. Front end (tccgen.c
 * initializers), not the RV64I code generator: pristine tcc 43c7708 built
 * natively for x86_64 (no patches) also prints 42 0 1 0. */
#include <stdio.h>

union U { int a; char c[4]; };
union U u = { .a = 42, .c[2] = 1 };

int main(void) {
  printf("%d %d %d %d\n", u.c[0], u.c[1], u.c[2], u.c[3]);
  return 0;
}
