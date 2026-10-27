struct S { struct { union { struct { int a,b; }; long n; }; int c; }; int z; };
int main(void) { struct S s={.a=1,2,3,4}; return s.a!=1||s.b!=2||s.c!=3||s.z!=4; }
