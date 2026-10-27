/* Separate pre-existing aggregate-expression initializer gap. */
struct T { int x, y; };
union U { int first; struct T s; };
int main(void) {
  struct T v = {1, 2};
  union U u = {.s = v};
  return u.s.y;
}
