union U { int i; unsigned char c[4]; };
struct S { union U u; int k; };
int main(void) { struct S s = { .u = { .c = {1,2,3,4} }, .k = 7 }; return s.k*10 + s.u.c[3]; }   /* gcc 74; pristine: internal error (invalid node) */
