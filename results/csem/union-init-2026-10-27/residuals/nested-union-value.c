/* Types agree: the initializer expression has exactly the nested union type. */
union I { int a; long b; };
union O { int x; struct { union I u; int y; } s; };
int main(void) {
  union I p = {.b = 7};
  union O o = {.s.u = p};
  return o.s.u.b;
}
