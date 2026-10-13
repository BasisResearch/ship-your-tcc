#include <stdio.h>
typedef int (*binop)(int, int);
static int add(int a, int b) { return a + b; }
static int sub(int a, int b) { return a - b; }
static int mul(int a, int b) { return a * b; }
static int fold(binop f, const int *xs, int n, int acc) {
  for (int i = 0; i < n; i++) acc = f(acc, xs[i]);
  return acc;
}
static int fib(int n) { return n < 2 ? n : fib(n - 1) + fib(n - 2); }
struct op { const char *name; binop f; };
int main(void) {
  static const int xs[] = { 3, 1, 4, 1, 5, 9, 2, 6 };
  struct op ops[] = { { "add", add }, { "sub", sub }, { "mul", mul } };
  for (int i = 0; i < 3; i++)
    printf("%s -> %d\n", ops[i].name, fold(ops[i].f, xs, 8, i == 2 ? 1 : 0));
  binop g = ops[1].f;
  printf("(*g)(10,3)=%d fib(20)=%d\n", (*g)(10, 3), fib(20));
  return fib(10);
}
