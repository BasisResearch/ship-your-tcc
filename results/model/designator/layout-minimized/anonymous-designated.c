struct S { union { struct { int a,b; }; int n; }; int z; };
static struct S s={.a=1,.b=2,.z=3};
int main(void) { return s.z; }
