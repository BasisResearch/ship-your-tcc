struct S { union { struct { int a,b; }; int n; }; int z; };
int main(void) { static struct S s[2]={[0].a=1,2,3,4,5,6}; return s[0].a!=1||s[0].b!=2||s[0].z!=3||s[1].a!=4||s[1].b!=5||s[1].z!=6; }
