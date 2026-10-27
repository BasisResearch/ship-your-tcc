union U { int a; char c[4]; };
int main(void) { union U u = { .a = 1, .a = 2 }; return u.a; }
