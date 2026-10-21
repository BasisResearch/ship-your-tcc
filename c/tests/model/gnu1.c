/* F3 model test: GNU extensions and C99/C11 features tcc accepts (tccgen.c) */
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

struct flex { int n; int a[]; };
struct flex fl = { 3, { 1, 2, 3 } };
union U { int i; float f; char c[4]; };
union U uu = { .f = 1.5f };
struct bits { unsigned a : 3; signed b : 5; unsigned long long c : 40; _Bool d : 1; int : 0; char e : 2; };
struct bits bb = { 5, -3, 0x123456789ULL, 1, 1 };
typedef int arr3[3];
arr3 a3 = { [2] = 7, [0] = 1 };
int cases[16] = { [3 ... 7] = 9, [10] = 1 };
_Static_assert(sizeof(int) == 4, "int");
_Alignas(16) char aligned16[3];
static __attribute__((aligned(32))) int al32 = 1;
__attribute__((noreturn)) void die(void);
static inline int inl(int x) { return x * 2; }
extern inline __attribute__((always_inline)) int ainl(int x) { return x + 3; }
int __attribute__((weak)) weakf(void) { return 4; }
static void cleanup_fn(int *p) { printf("cleanup %d\n", *p); }
enum { A = 1, B = A << 3, C = sizeof(long double) };
static int sum(int n, ...) {
    va_list ap; int s = 0;
    va_start(ap, n);
    while (n--) s += va_arg(ap, int);
    va_end(ap);
    return s;
}
static double dsum(int n, ...) {
    va_list ap, aq; double s = 0;
    va_start(ap, n);
    va_copy(aq, ap);
    while (n--) s += va_arg(aq, double);
    va_end(aq);
    va_end(ap);
    return s;
}
struct S2 { char c; double d; };
static struct S2 mk(char c, double d) { struct S2 s = { c, d }; return s; }
static struct S2 pass(struct S2 s) { s.c++; return s; }
int main(void)
{
    int __attribute__((cleanup(cleanup_fn))) cl = 42;
    int n = 4;
    int vla[n * 2];
    char (*pv)[n] = 0;
    __label__ out;
    typeof(n) t = ({ int q = n * 3; q + 1; });
    __auto_type au = 2.5;
    int g = _Generic(au, double: 1, default: 2);
    void *lab = &&out;
    for (int i = 0; i < n * 2; i++) vla[i] = i * i;
    printf("%d %d %zu %d %d\n", t, g, sizeof vla, vla[7], (int)sizeof *pv);
    printf("%d %d %d\n", fl.a[2], uu.c[3], a3[2] + cases[5] + cases[10]);
    printf("%u %d %llx %d %d\n", bb.a, bb.b, (unsigned long long)bb.c, bb.d, bb.e);
    printf("%d %d %d %d\n", inl(3), ainl(4), weakf(), A + B + C);
    printf("%d %g\n", sum(4, 1, 2, 3, 4), dsum(3, 1.5, 2.5, 3.5));
    struct S2 s = pass(mk('a', 2.25));
    printf("%c %g %d\n", s.c, s.d, (int)offsetof(struct S2, d));
    bool bt = 7;
    printf("%d %d %d\n", bt, __builtin_constant_p(n), __builtin_constant_p(3 * 4));
    printf("%d\n", __builtin_types_compatible_p(int, unsigned) + __builtin_choose_expr(1, 10, 20));
    int x = n ?: 9;
    switch (x) {
    case 1 ... 3: printf("low\n"); break;
    case 4 ... 10: printf("mid\n"); goto *lab;
    default: break;
    }
out:
    printf("%d %p\n", x, (void *)0);
    printf("%d %d\n", (int)__alignof__(long double), (int)_Alignof(aligned16));
    return __builtin_expect(0, 0);
}
