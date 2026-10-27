union U { struct { int x; int y; } s; long b; };
int main(void) { union U u = { .s = { .y = 2 } }; return u.s.y*10 + u.s.x; }
