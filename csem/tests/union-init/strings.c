union U { int a; char c[5]; struct { char s[3]; int n; } t; };
static union U a = { .a=42, .c="ab" };
static union U b = { .c="abcd", .c={ 'x' }, .c[2]='z' };
static union U c = { .t.s="abc", .t.n=7 };
int main(void) {
  union U x = { .t.n=8, .c={"xy"} };
  return a.c[0]!='a' || a.c[1]!='b' || a.c[2] || a.c[4] ||
         b.c[0]!='x' || b.c[1] || b.c[2]!='z' || b.c[3] ||
         c.t.s[2]!='c' || c.t.n!=7 || x.c[0]!='x' || x.c[2] || x.c[4];
}
