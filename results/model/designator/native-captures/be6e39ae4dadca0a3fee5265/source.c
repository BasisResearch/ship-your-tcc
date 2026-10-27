/* C11 6.7.9p17-20; no union is needed for the independent tcc defect.
   GCC accepts under -std=c11 -pedantic-errors. Both patched CSem engines
   return zero. tcc forgets the inner cursor and advances to z too soon. */
struct S { int a[2]; int z; };
static struct S s = { .a[0] = 1, 2, 3 };
int main(void) { return s.a[0]!=1 || s.a[1]!=2 || s.z!=3; }
