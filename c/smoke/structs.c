#include <stdio.h>
struct point { int x, y; };
struct rect { struct point lo, hi; char tag; };
union u { int i; unsigned char b[4]; };

static int area(struct rect *r) { return (r->hi.x - r->lo.x) * (r->hi.y - r->lo.y); }
static struct point add(struct point a, struct point b) { struct point c = { a.x + b.x, a.y + b.y }; return c; }
int main(void) {
  struct rect r = { {1, 2}, {11, 7}, 'r' };
  struct rect copy = r;
  copy.hi.x += 5;
  struct point p = add(r.lo, copy.hi);
  union u v; v.i = 0x01020304;


  printf("area=%d area2=%d tag=%c p=(%d,%d)\n", area(&r), area(&copy), copy.tag, p.x, p.y);
  printf("byte0=%d sizeof(rect)=%d sizeof(long)=%d sizeof(void*)=%d\n", v.b[0], (int)sizeof(struct rect), (int)sizeof(long), (int)sizeof(void *));

  char c = (char)200;
  printf("char signed? %d\n", c < 0);
  return 0;
}
