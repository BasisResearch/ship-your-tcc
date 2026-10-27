struct S { struct { union {struct {int a,b;}; int n;}; int c;} v; int z;};
int main(void) { struct S s={.v.a=1,2,3,4}; return s.v.a!=1||s.v.b!=2||s.v.c!=3||s.z!=4; }
