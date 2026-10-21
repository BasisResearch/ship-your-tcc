/* Phase F2 (results/pre/f2-diff.md): `#line N "name"` with a relative name
 * makes __FILE__ the real file's directory + name. C11 6.10.4p4: the
 * string "changes the presumed name of the source file"; 6.10.8.1:
 * __FILE__ is "the presumed name of the current source file (a character
 * string literal)". tccpp.c tccpp_putfile prepends the directory of the
 * real file when the name is not absolute: for /prog.c in the image,
 * __FILE__ becomes "/foo.c". Expected output "foo.c 10"; tcc prints
 * "/foo.c 10". CSem.Pre: c/tests/pre/line.c. */
#include <stdio.h>
int main(void) {
#line 10 "foo.c"
  printf("%s %d\n", __FILE__, __LINE__);
  return 0;
}
