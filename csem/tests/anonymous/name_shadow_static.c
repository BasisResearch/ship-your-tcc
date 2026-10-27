struct S { struct {int a,b;} named; union {struct {int a,b;}; int n;}; int z;};
int main(void) { static struct S s={.named={7,8},.a=1,2,3}; return s.named.a!=7||s.named.b!=8||s.a!=1||s.b!=2||s.z!=3; }
