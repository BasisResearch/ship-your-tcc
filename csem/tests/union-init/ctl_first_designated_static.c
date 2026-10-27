union U { int i; unsigned char c[4]; };
union U u = { .i = 258 };
int main(void) { return u.i; }
