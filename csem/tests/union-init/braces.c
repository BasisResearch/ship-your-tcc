union U { int a; struct { int x,y; } s; int v[4]; };
static union U u[2] = { [0].s.x=7, [0].s.y=8, [0].s={.y=3},
                         [1].v[0]=9, [1].v={ [2]=4 } };
int main(void) {
  union U a = { .s.x=7, .a=2, .s.y=8, .s.x=9 };
  union U b = { .v[2]=7, .v={1}, .v[3]=9 };
  return u[0].s.x || u[0].s.y!=3 || u[1].v[0] || u[1].v[2]!=4 ||
         a.s.x!=9 || a.s.y!=8 || b.v[0]!=1 || b.v[1] || b.v[2] || b.v[3]!=9;
}
