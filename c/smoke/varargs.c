#include <stdio.h>
#include <stdarg.h>
/* user-defined variadic function: needs __builtin_va_start / va_arg */
static long sum(int n, ...) {
  va_list ap;
  long s = 0;
  va_start(ap, n);
  for (int i = 0; i < n; i++) s += va_arg(ap, int);
  va_end(ap);
  return s;
}
int main() {
  printf("sum=%ld\n", sum(4, 1, 2, 3, 4));
  return 0;
}
