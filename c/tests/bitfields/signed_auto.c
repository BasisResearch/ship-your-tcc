struct S { signed int a:3; unsigned b:5; int c:30; };
int main(void) { struct S s={-2,27,-123}; return s.a != -2 || s.b != 27 || s.c != -123; }
