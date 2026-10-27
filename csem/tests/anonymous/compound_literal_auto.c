struct S { union { struct { int a,b; }; int n; }; int z; };
int main(void) { int dummy=0; return ((struct S){.a=1,2,3}).b!=2||dummy; }
