struct S {union {struct {int *p; int b;}; long n;}; int z;}; int x=7;
int main(void) { struct S s={.p=&x,2,3}; return s.p!=&x||*s.p!=7||s.b!=2||s.z!=3; }
