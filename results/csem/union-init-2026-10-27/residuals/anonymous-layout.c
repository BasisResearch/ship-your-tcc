/* Separate pre-existing representation gap: the anonymous union's members
   are flattened into its containing struct, which loses shared storage. */
struct S { int k; union { int i; unsigned char c[4]; }; };
int main(void) { return sizeof(struct S); }
