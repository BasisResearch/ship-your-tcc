#include <stdio.h>
/* bit fields: a global (static initialiser) and a local whose container word
   is initialised before the bit-field stores.  (A bit-field store into an
   UNinitialised container is stuck in CompCert's semantics: it is a
   read-modify-write of an undefined word.  See results/certora.md.) */
struct bf { unsigned a : 3; signed b : 5; unsigned c : 8; };
struct bf g = { 5, -3, 200 };
int main(void) {
  union { struct bf s; unsigned w; } u;
  u.w = 0;
  u.s.a = 6; u.s.b = -7; u.s.c = 17;
  g.a += 4; g.b -= 20; g.c++;
  printf("g a=%u b=%d c=%u\n", g.a, g.b, g.c);
  printf("u a=%u b=%d c=%u w=%u\n", u.s.a, u.s.b, u.s.c, u.w);
  return 0;
}
