/* Phase F2 (results/pre/f2-diff.md): tcc does not implement the _Pragma
 * operator (C99 and C11 6.10.9): tccpp.c has no TOK__Pragma, so `_Pragma`
 * reaches the parser as an identifier and the line below is a syntax
 * error. Per 6.10.9 it is the directive `#pragma STDC FP_CONTRACT OFF`
 * (a standard pragma, 7.12.2), and the program prints "ok".
 * CSem.Pre: c/tests/pre/pragma_op.c (tcc -E keeps `_Pragma ( "pack(1)" )`
 * as tokens), Spec/Pre/Test.lean (the 6.10.9p2 example). */
#include <stdio.h>
_Pragma("STDC FP_CONTRACT OFF")
int main(void) {
  puts("ok");
  return 0;
}
