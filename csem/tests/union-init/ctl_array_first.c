union U { int i; char c[4]; };
int main(void) { union U a[2] = { [1] = { .i = 9 }, [0] = { 1 } }; return a[0].i*10 + a[1].i; }
