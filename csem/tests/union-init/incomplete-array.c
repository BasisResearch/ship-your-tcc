union U { int a; struct { int x,y; } s; };
static union U a[] = { [3].s.y=7, [1].s.x=4 };
int main(void) {
  union U b[] = { [2].s.x=3, [0].s.y=5 };
  return sizeof(a)/sizeof(a[0])!=4 || sizeof(b)/sizeof(b[0])!=3 ||
         a[0].a || a[1].s.y || a[1].s.x!=4 || a[3].s.x || a[3].s.y!=7 ||
         b[0].s.x || b[0].s.y!=5 || b[1].a || b[2].s.x!=3 || b[2].s.y;
}
