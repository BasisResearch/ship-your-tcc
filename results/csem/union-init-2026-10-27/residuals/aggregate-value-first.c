/* Same aggregate-expression gap without any non-first designator. */
struct pair { int a, b; };
union U { struct pair s; int x; };
int main(void) {
  struct pair p = {1, 7};
  union U u = {.s = p};
  return u.s.b;
}
