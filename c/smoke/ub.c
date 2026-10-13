#include <stdio.h>
/* undefined behaviour: out-of-bounds read; both oracles must report STUCK (126) */
int main(void) {
  int a[4] = { 1, 2, 3, 4 };
  int i = 4;
  printf("before\n");
  printf("a[4]=%d\n", a[i]);
  return 0;
}
