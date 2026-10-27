struct S { union { struct { int a,b; }; int n; }; int z; }; int check(struct S p){struct S copy=p; return copy.a!=1||copy.b!=2||copy.z!=3;}
int main(void) { struct S s={.a=1,2,3}; return check(s); }
