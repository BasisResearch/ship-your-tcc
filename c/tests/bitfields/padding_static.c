struct S { unsigned a:3; unsigned :5; unsigned b:4; };
int main(void) { static struct S s={3,9}; return s.a!=3 || s.b!=9; }
