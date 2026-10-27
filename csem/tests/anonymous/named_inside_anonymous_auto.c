struct S { union {struct {struct {int a,b;} v; int c;}; long n;}; int z;};
int main(void) { struct S s={.v.a=1,2,3,4}; return s.v.a!=1||s.v.b!=2||s.c!=3||s.z!=4; }
