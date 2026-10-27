union U { int a; struct { int x; int b[3]; int y; } s; };
struct S { union U u; int tail; };
static struct S a = { .u.s.b[1]=2, 3, 4, 5 };
int main(void) {
  union U u = { .s.x=1, 2, 3, 4, 5 };
  union U v[2] = { [0].s.b[1]=7, 8, 9, [1].s.x=6, 1, 2, 3, 4 };
  return a.u.s.x || a.u.s.b[0] || a.u.s.b[1]!=2 || a.u.s.b[2]!=3 ||
         a.u.s.y!=4 || a.tail!=5 || u.s.x!=1 || u.s.b[2]!=4 || u.s.y!=5 ||
         v[0].s.x || v[0].s.b[1]!=7 || v[0].s.b[2]!=8 || v[0].s.y!=9 ||
         v[1].s.x!=6 || v[1].s.b[0]!=1 || v[1].s.b[2]!=3 || v[1].s.y!=4;
}
