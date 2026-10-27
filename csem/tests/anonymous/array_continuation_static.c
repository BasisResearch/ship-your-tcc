struct S { union { struct { int a[2], b; }; int n; }; int z; };
int main(void) { static struct S s={.a[0]=1,2,3,4}; return s.a[0]!=1||s.a[1]!=2||s.b!=3||s.z!=4; }
