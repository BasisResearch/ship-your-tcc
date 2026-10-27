struct S { union { struct { int a,b; }; int n; }; int z; };
int main(void) { static struct S s={.a=1,.b=2,.n=9,3}; return s.n!=9||s.z!=3; }
