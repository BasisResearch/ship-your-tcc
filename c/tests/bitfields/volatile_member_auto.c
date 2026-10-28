struct S { volatile unsigned a:3; unsigned b:5; };
int main(void) { struct S s={1,19}; s.a+=2; return s.a!=3 || s.b!=19; }
