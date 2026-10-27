struct S { union { struct { int a,b; }; int n; }; int z; };
int main(void) { struct S s={.a=1,.b=2,.a=4,5,6}; return s.a!=4||s.b!=5||s.z!=6; }
