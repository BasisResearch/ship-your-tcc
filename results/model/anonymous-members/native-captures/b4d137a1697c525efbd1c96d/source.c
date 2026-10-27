#include <stdio.h>
struct S { const struct { int a; }; volatile union { int b; }; };
int main(void) {
    struct S s = { .a = 1, .b = 2 };
    int c = _Generic(&s.a, const int *: 1, default: 0);
    int v = _Generic(&s.b, volatile int *: 1, default: 0);
    printf("const=%d volatile=%d\n", c, v);
    return !(c && v);
}
