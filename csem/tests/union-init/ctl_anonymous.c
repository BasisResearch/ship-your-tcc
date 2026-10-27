struct S { int k; union { int i; unsigned char c[4]; }; };
int main(void) { struct S s = { .c = {1,2,3,4}, .k = 7 }; return s.k*10 + s.c[3]; }
