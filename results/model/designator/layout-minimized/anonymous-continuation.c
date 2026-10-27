struct S { union { struct { int a,b; }; int n; }; int z; };
static struct S s={.a=1,2,3};
int main(void) { return s.z; }
