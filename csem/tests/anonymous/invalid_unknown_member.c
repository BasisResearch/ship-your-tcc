struct S { union { struct { int a,b; }; int n; }; int z; }; int main(void){struct S s={.missing=1}; return 0;}
