/* variadic functions with doubles, floats (promoted), ints, longs, strings */
#include <stdio.h>
#include <stdarg.h>

double sumd(int n, ...)
{
    va_list ap;
    double s = 0;
    int i;
    va_start(ap, n);
    for (i = 0; i < n; i++) s += va_arg(ap, double);
    va_end(ap);
    return s;
}

/* format: d=double i=int l=long s=string */
void show(const char *fmt, ...)
{
    va_list ap, ap2;
    const char *p;
    va_start(ap, fmt);
    va_copy(ap2, ap);
    for (p = fmt; *p; p++) {
        switch (*p) {
        case 'd': printf("[%.17g]", va_arg(ap, double)); break;
        case 'i': printf("[%d]", va_arg(ap, int)); break;
        case 'l': printf("[%ld]", va_arg(ap, long)); break;
        case 's': printf("[%s]", va_arg(ap, char *)); break;
        }
    }
    printf(" first again: ");
    if (*fmt == 'd') printf("%g", va_arg(ap2, double));
    printf("\n");
    va_end(ap2);
    va_end(ap);
}

void myprintf(const char *fmt, ...)
{
    va_list ap;
    char buf[200];
    va_start(ap, fmt);
    vsnprintf(buf, sizeof buf, fmt, ap);
    va_end(ap);
    printf("<%s>\n", buf);
}

int main(void)
{
    float f = 2.5f;
    printf("%.17g\n", sumd(3, 1.0, 2.0, 0.1));
    printf("%.17g\n", sumd(10, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.5));
    printf("%.17g\n", sumd(2, (double)f, f));
    show("dids", 1.5, 42, -0.125, "hi");
    show("ldldldldld", 1L, 1.0, 2L, 2.0, 3L, 3.0, 4L, 4.0, 5L, 5.0);
    show("iiiiiiidd", 1, 2, 3, 4, 5, 6, 7, 8.5, 9.25);
    show("diiiiiiid", 0.5, 1, 2, 3, 4, 5, 6, 7, 1e300);
    myprintf("%g %f %e %d %s %.3f", 1.0 / 7, 2.0 / 7, 3.0 / 7, -5, "x", f);
    printf("%d %g %ld %g %s %g %c\n", 1, 2.0, 3L, (double)4.0f, "five", 6e-6, 'z');
    return 0;
}
