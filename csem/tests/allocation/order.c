/* Integer comparisons expose placement independently of C evaluation order. */
int a, b;
int main(void) {
    unsigned long x = (unsigned long)&a;
    unsigned long y = (unsigned long)&b;
    return x < y ? 11 : 22;
}
