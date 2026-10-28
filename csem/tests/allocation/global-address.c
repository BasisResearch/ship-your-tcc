/* Address is an observation, with bytes computed by the ordinary printf path. */
extern int printf(const char *, ...);
int object = 7;
int main(void) {
    printf("object=%lu value=%d\n", (unsigned long)&object, object);
    return 0;
}
