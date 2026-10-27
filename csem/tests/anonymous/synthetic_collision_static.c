struct S { int anonymous; struct {int a,b;}; int z;};
int main(void) { static struct S s={7,.a=1,2,3}; return s.anonymous!=7||s.a!=1||s.b!=2||s.z!=3; }
