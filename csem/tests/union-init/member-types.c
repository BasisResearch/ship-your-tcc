union U { char first; double d; long l; int *p; int (*f)(int); };
static int n = 7;
static int plus(int x) { return x+1; }
static union U fp = { .d=1.5, .f=plus };
static union U p = { .f=plus, .p=&n };
static union U d = { .p=&n, .d=1.5 };
int main(void) {
  union U a = { .first=1, .l=123456789L };
  union U b = { .first=2, .d=2.5 };
  union U c = { .d=3.5, .p=&n };
  return a.l!=123456789L || b.d!=2.5 || c.p!=&n || *p.p!=7 ||
         d.d!=1.5 || fp.f(4)!=5;
}
