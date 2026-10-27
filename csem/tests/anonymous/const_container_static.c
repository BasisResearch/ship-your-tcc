struct S { const struct {int a,b;}; int z;};
int main(void) { static struct S s={.a=1,2,3}; return s.a!=1||s.b!=2||s.z!=3; }
