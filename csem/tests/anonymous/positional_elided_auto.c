struct S { union { struct { int a,b; }; int n; }; int z; };
int main(void) { struct S s={1,2,3}; return s.a!=1||s.b!=2||s.z!=3; }
