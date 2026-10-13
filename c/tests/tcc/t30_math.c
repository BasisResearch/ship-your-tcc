/* libm functions that are exactly specified (correctly rounded or exact),
   so newlib and glibc agree bit for bit */
#include <stdio.h>
#include <math.h>

double v[] = {0.0, 0.5, 1.0, 2.0, 2.5, -2.5, 10.0, 1e-3, 123456.789, -0.75};

int main(void)
{
    int i, e;
    for (i = 0; i < 10; i++) {
        double x = v[i], ip;
        double fr = modf(x, &ip);
        double m = frexp(x, &e);   /* not in the printf: argument order is unspecified */
        printf("%g: sqrt=%.17g floor=%g ceil=%g fabs=%g fmod3=%.17g", x,
               x >= 0 ? sqrt(x) : -1.0, floor(x), ceil(x), fabs(x), fmod(x, 3.0));
        printf(" frexp=%.17g,%d ldexp=%.17g modf=%g,%g trunc=%g round=%g\n",
               m, e, ldexp(x, 5), fr, ip, trunc(x), round(x));
    }
    printf("%.17g %.17g\n", sqrt(2.0), sqrt(1e-300));
    printf("%.9g\n", sqrtf(2.0f));
    printf("%g %g\n", copysign(3.0, -0.0), fmax(1.0, 2.0));
    return 0;
}
