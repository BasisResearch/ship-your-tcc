struct S { union { struct { int a,b; }; int n; }; int z; }; int main(void){const struct S s={0}; s.a=1; return 0;}
