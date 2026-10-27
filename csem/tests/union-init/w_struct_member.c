union U { int i; struct { int x; int y; } s; };
int main(void) { union U u = { .s = { .y = 2 } }; return u.s.y*10 + u.s.x; }   /* gcc 20; both engines a UB036 verdict before */
