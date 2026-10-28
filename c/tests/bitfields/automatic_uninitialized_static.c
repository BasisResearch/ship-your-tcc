struct S { unsigned a:3; unsigned b:5; };
int main(void) { static struct S s; s.a=5; s.b=27; return s.a!=5 || s.b!=27; }
