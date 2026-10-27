struct S { union { struct { int a,b; }; int n; }; int z; };
int main(void) { static struct S s={.n=9,.b=2,3}; return s.a!=0||s.b!=2||s.z!=3; }
