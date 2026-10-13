#include <stdio.h>
/* putchar/puts: supported by the Lean runner's model, not by ccomp -interp */
int main() {
  char buf[4] = { 'a', 'b', 'c', 0 };
  putchar('x');
  putchar('\n');
  puts("line");
  printf("local buffer via %%s: %s\n", buf);
  return 3;
}
