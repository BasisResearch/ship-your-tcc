struct S { union { struct { int a,b; }; int n; }; int z; }; struct S make(void){struct S t={.a=1,2,3};return t;}
int main(void) { static int dummy=0; return make().b!=2||dummy; }
