struct S { union { struct { int a,b; }; int n; }; int z; };
int main(void) { static struct S s={.a=1,.b=2,.z=3}; return s.a!=1||s.b!=2||s.z!=3; }
