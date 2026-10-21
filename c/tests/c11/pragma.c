#include <stdio.h>
#define DO(x) _Pragma(#x)
#define OPERAND "pack(push,1)"
_Pragma(OPERAND)
struct P { char c; int i; };
DO(pack(pop))
struct Q { char c; int i; };
#define X 11
DO(push_macro("X"))
#undef X
#define X 22
DO(pop_macro("X"))
DO(STDC FP_CONTRACT OFF)
_Pragma(L"STDC FENV_ACCESS OFF")
#define WRAP(x) x
int main(void) {
  int n = 1 WRAP(_Pragma("STDC FP_CONTRACT OFF")) + 2;
  int bad = sizeof(struct P) != 5 || sizeof(struct Q) != 8 || X != 11 || n != 3;
  printf("pragma %d\n", bad);
  return bad;
}
/* Force pop_macro to run before rescanning the following X. */
#undef X
#define X 33
DO(push_macro("X"))
#undef X
#define X 44
#define RESTORE _Pragma("pop_macro(\"X\")") X
int after_pop[RESTORE == 33 ? 1 : -1];
