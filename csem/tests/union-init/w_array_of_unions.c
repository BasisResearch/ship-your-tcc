union U { int i; char c[4]; };
int main(void) { union U a[2] = { [1] = { .c[0] = 9 }, [0] = { .i = 1 } }; return a[0].i*10 + a[1].c[0]; }   /* gcc 19; 10 before */
