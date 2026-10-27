struct S { union { struct { int a,b; }; int n; }; int z; };
int main(void) { struct S s={.a=1,2,3}; return &s.a==&s.b||&s.b==&s.z||s.a!=1; }
