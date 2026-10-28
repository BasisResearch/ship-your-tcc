struct S { unsigned a:3; unsigned b:5; };
int main(void) { struct S s={1,2}; int n=s.a++ + s.b++; return n!=3 || s.a!=2 || s.b!=3; }
