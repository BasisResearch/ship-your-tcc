struct S { unsigned a:3; unsigned b:5; };
int main(void) { struct S s={0,0}; int n=(s.a=1)+(s.b=2); return n!=3 || s.a!=1 || s.b!=2; }
