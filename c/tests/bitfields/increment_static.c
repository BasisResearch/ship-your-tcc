struct S { unsigned a:3; int b:4; };
int main(void) { static struct S s={7,-2}; int a=s.a++; int b=++s.b; return a!=7 || s.a!=0 || b != -1 || s.b != -1; }
