/* structs containing floats/doubles passed and returned by value (lp64:
   plain integer aggregates, <=16 bytes in a0/a1, larger by reference) */
#include <stdio.h>

struct F1 { float a; };
struct F2 { float a, b; };
struct F3 { float a, b, c; };
struct D1 { double a; };
struct D2 { double a, b; };
struct D3 { double a, b, c; };
struct FI { float f; int i; };
struct ID { int i; double d; };
struct DF { double d; float f; };
struct FA { float v[2]; };
struct DA { double v[2]; };
union U { double d; long l; };

struct F1 f1(struct F1 x) { x.a *= 2; return x; }
struct F2 f2(struct F2 x) { struct F2 r; r.a = x.b; r.b = x.a + 1; return r; }
struct F3 f3(struct F3 x) { x.c = x.a + x.b; return x; }
struct D1 d1(struct D1 x) { x.a /= 3; return x; }
struct D2 d2(struct D2 x) { struct D2 r = {x.a * x.b, x.a - x.b}; return r; }
struct D3 d3(struct D3 x) { x.a += x.b + x.c; return x; }
struct FI fi(struct FI x) { x.f += x.i; x.i = -x.i; return x; }
struct ID id(struct ID x) { x.d *= x.i; x.i++; return x; }
struct DF df(struct DF x) { x.f = x.d; x.d = -x.d; return x; }
struct FA fa(struct FA x) { float t = x.v[0]; x.v[0] = x.v[1]; x.v[1] = t * 10; return x; }
struct DA da(struct DA x) { x.v[1] += x.v[0]; return x; }
union U uu(union U x) { x.d += 1.0; return x; }

/* struct args mixed with scalars so they land in later registers / stack */
double mix(int a, struct D2 b, float c, struct F2 d, double e, struct ID f, struct D2 g, struct F3 h)
{
    return a + b.a + b.b + c + d.a + d.b + e + f.i + f.d + g.a + g.b + h.a + h.b + h.c;
}

int main(void)
{
    struct F1 a = {1.5f}; struct F2 b = {2.5f, -1.0f}; struct F3 c = {1, 2, 3};
    struct D1 d = {1.0}; struct D2 e = {3.0, 0.5}; struct D3 g = {1, 2, 3};
    struct FI h = {0.25f, 7}; struct ID k = {3, 1.25}; struct DF m = {2.75, 0};
    struct FA n = {{1.5f, 2.5f}}; struct DA p = {{0.125, 8}}; union U q;
    q.d = 2.0;
    a = f1(a); b = f2(b); c = f3(c); d = d1(d); e = d2(e); g = d3(g);
    h = fi(h); k = id(k); m = df(m); n = fa(n); p = da(p); q = uu(q);
    printf("%g | %g %g | %g %g %g | %.17g | %g %g | %g %g %g\n", a.a, b.a, b.b, c.a, c.b, c.c, d.a, e.a, e.b, g.a, g.b, g.c);
    printf("%g %d | %d %g | %g %g | %g %g | %g %g | %g\n", h.f, h.i, k.i, k.d, m.d, m.f, n.v[0], n.v[1], p.v[0], p.v[1], q.d);
    printf("%.17g\n", mix(1, e, 0.5f, b, 1e-3, k, e, c));
    printf("%.17g\n", mix(-1, d2(e), f1(a).a, f2(b), d1(d).a, id(k), e, f3(c)));
    return 0;
}
