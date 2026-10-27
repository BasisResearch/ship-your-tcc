struct S { union { struct { int a[2]; int b; }; int n; }; int z; };
int main(void) { return sizeof(struct S); }
