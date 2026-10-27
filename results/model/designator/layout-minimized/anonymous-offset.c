struct S { union { struct { int a[2]; int b; }; int n; }; int z; };
int main(void) { struct S s; return (char *)&s.z - (char *)&s; }
