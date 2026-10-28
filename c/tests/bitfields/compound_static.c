struct S { unsigned a:3; int b:4; };
int main(void) { static struct S s={6,-3}; s.a+=3; s.b*=2; return s.a!=1 || s.b != -6; }
