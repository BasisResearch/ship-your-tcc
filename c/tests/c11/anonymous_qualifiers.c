#include <stdio.h>

struct S {
    const struct { int a[2]; int *p; };
    volatile union { int b; };
};
struct N { volatile union { const struct { int x; }; }; };
struct V { volatile struct { int x; }; };
struct Named { const struct { int x; } n; };
static struct S global = { .a = {1, 2}, .p = 0, .b = 3 };
static struct V counter_value = { .x = 8 };
static int calls;
static struct V *get(void) { ++calls; return &counter_value; }

int main(void) {
    struct S s = { .a = {1, 2}, .p = 0, .b = 3 };
    struct N n = { .x = 4 };
    struct Named named = { .n = { .x = 5 } };
    const struct V cv = { .x = 6 };
    int bad = 0;
    bad += !_Generic(s.a, const int *: 1, default: 0);
    bad += !_Generic(&s.a, const int (*)[2]: 1, default: 0);
    bad += !_Generic(&s.p, int *const *: 1, default: 0);
    bad += !_Generic(&s.b, volatile int *: 1, default: 0);
    bad += !_Generic(&global.a[1], const int *: 1, default: 0);
    bad += !_Generic(&global.b, volatile int *: 1, default: 0);
    bad += !_Generic(&(&n)->x, const volatile int *: 1, default: 0);
    bad += !_Generic(&named.n.x, const int *: 1, default: 0);
    bad += !_Generic(&cv.x, const volatile int *: 1, default: 0);
    bad += !_Generic(&get()->x, volatile int *: 1, default: 0);
    bad += calls != 0;
    bad += get()->x != 8;
    bad += calls != 1;
    printf("anonymous qualifiers %d\n", bad);
    return bad != 0;
}
