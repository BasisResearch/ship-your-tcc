union U { int a; struct { int *p; int x; int *v[2]; } s; };
static union U u = { .s.x=3 };
int main(void) {
  union U a = { .a=42, .s.x=4 };
  return u.s.p!=0 || u.s.v[0]!=0 || u.s.v[1]!=0 || u.s.x!=3 ||
         a.s.p!=0 || a.s.v[1]!=0 || a.s.x!=4;
}
