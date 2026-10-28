extern void *malloc(unsigned long);
extern void free(void *);
extern int printf(const char *, ...);
int main(void) {
    int *p = malloc(sizeof(int));
    if (!p) return 1;
    *p = 37;
    unsigned long old = (unsigned long)p;
    free(p);
    p = malloc(sizeof(int));
    if (!p) return 2;
    *p = 19;
    printf("old=%lu new=%lu reuse=%d value=%d\n", old, (unsigned long)p,
           old == (unsigned long)p, *p);
    free(p);
    return 0;
}
