union U { int i; unsigned char c[4]; };
struct S { union U u; int k; };
int main(void) { struct S s = { { 1, 2, 3, 4 }, 7 }; return s.k*10 + s.u.i; }
