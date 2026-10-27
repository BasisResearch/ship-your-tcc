void f(int *p) { (void)p; }
int main(void) { int x __attribute__((cleanup(f))) = 0; return x; }
