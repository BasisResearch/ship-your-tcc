union U { int i; char c; double d; };
int main(void) {
    union U u1 = { .i = 100 };
    if (u1.i != 100) return 1;
    union U u2 = { .c = 0x41 };   /* the RETIRED coverage control union3-003-union-init.c, its second declaration ([USER 2026-10-20]) */
    if (u2.c != 0x41) return 2;
    return 0;
}
