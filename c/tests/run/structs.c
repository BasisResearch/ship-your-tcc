/* structs by value and by pointer, arrays of structs (power-of-two sizes,
   so indexing is a shift until the RV64I codegen patch lands) */
#include <stdio.h>
#include <string.h>

struct point { int x, y; };                 /* 8 bytes */
struct rect { struct point lo, hi; };       /* 16 bytes */

static struct point add(struct point a, struct point b)
{
    struct point r = { a.x + b.x, a.y + b.y };
    return r;
}

static int area_ish(const struct rect *r)
{
    /* (w + h) << 1: the perimeter, no multiply */
    return ((r->hi.x - r->lo.x) + (r->hi.y - r->lo.y)) << 1;
}

int main(void)
{
    struct point ps[4] = { {1, 2}, {3, 4}, {5, 6}, {7, 8} };
    struct point s = { 0, 0 };
    struct rect r, q;
    int i;
    for (i = 0; i < 4; i++)
        s = add(s, ps[i]);
    printf("sum = (%d, %d)\n", s.x, s.y);
    r.lo = ps[0];
    r.hi = ps[3];
    q = r;
    q.hi.x += 10;
    printf("perimeter r = %d, q = %d\n", area_ish(&r), area_ish(&q));
    memset(&q, 0, sizeof q);
    printf("cleared: %d %d %d %d\n", q.lo.x, q.lo.y, q.hi.x, q.hi.y);
    printf("sizeof: %d %d\n", (int)sizeof(struct point), (int)sizeof(struct rect));
    return 0;
}
