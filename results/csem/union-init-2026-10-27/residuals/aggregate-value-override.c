/* Additional lead-review probe: an override must retain p.b. */
struct pair { int a, b; };
union U { struct pair s; int x; };
int main(void) {
  struct pair p = {1, 7};
  union U u = {.s = p, .s.a = 3};
  return u.s.b;
}
