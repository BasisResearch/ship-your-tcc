/* Phase F2 (results/pre/f2-diff.md): tcc rejects universal character names
 * in identifiers ("stray '\' in program"). C99 and C11 6.4.2.1p1:
 * identifier-nondigit includes universal-character-name (6.4.3); U+00E9 is
 * in the ranges of Annex D. tcc accepts the UTF-8 spelling (bytes >= 0x80
 * are identifier characters in tccpp.c) but not the UCN.
 * Expected output "3". CSem.Pre: c/tests/pre/ucn_ident.c. */
#include <stdio.h>
int caf\u00e9 = 3;
int main(void) {
  printf("%d\n", caf\u00e9);
  return 0;
}
