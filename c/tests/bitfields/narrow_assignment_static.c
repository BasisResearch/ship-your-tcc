struct S { unsigned a:3; signed int b:3; };
int main(void) { static struct S s={0}; int a=(s.a=15); int b=(s.b=7); return a!=7 || b != -1 || s.a!=7 || s.b != -1; }
