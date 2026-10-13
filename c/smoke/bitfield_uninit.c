#include <stdio.h>
/* Defined C, but STUCK in CompCert's semantics (both oracles): initialising a
   local struct with bit fields stores each bit field as a read-modify-write of
   its container word, which is still undefined (Vundef) for a fresh local. */
struct bf { unsigned a : 3; signed b : 5; unsigned c : 8; };
int main(void) {
  struct bf f = { 5, -3, 200 };
  printf("a=%u b=%d c=%u\n", f.a, f.b, f.c);
  return 0;
}
