struct S {union {struct {int a; int *p;}; long n;}; int z;};
int main(void) { struct S s={.a=1}; return s.a!=1||s.p!=0||s.z!=0; }
