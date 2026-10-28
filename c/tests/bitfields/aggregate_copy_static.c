struct S { unsigned a:3; int b:5; };
int main(void) { static struct S s={7,-9}; struct S t=s; return t.a!=7 || t.b != -9; }
