/* Phase F2 (results/pre/f2-diff.md): `#` escapes more than C11 6.10.3.2p2
 * allows. The standard: a backslash "is inserted before each" double quote
 * and backslash "of a character constant or string literal"; nothing else
 * changes. tccpp.c macro_arg_subst calls add_char, which also writes every
 * byte outside 32..126 as a three-digit octal escape (and new-line as
 * backslash-n). One stringization has the same string value either way;
 * stringizing the result again shows the spelling. S("<e-acute>") is spelled
 * with the two raw bytes, "\"<C3><A9>\"", and XS(S(...)) stringizes that
 * spelling: the string \"<C3><A9>\" plus quotes, 8 bytes (sizeof 9, as gcc
 * prints). tcc spells the inner result "\"\303\251\"" (octal escapes), so
 * the outer string is 12 bytes (sizeof 13 on the image, where char is
 * unsigned). Expected output "9"; tcc prints "13". The string literal below
 * holds the two raw UTF-8 bytes
 * of U+00E9. CSem.Pre: c/tests/pre/stringize.c. */
#include <stdio.h>
#define S(x) #x
#define XS(x) S(x)
int main(void) {
  printf("%d\n", (int)sizeof(XS(S("é"))));
  return 0;
}
