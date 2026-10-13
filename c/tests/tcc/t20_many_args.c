/* more arguments than argument registers: doubles/floats on the stack */
#include <stdio.h>

double d12(double a, double b, double c, double d, double e, double f,
           double g, double h, double i, double j, double k, double l)
{
    return a - b + c * 2 - d + e * 3 - f + g * 4 - h + i * 5 - j + k * 6 - l;
}

float f10(float a, int b, float c, long d, float e, int f, float g, long h, float i, float j)
{
    return a + b + c + d + e + f + g + h + i * j;
}

double mixed(int a, double b, char c, float d, short e, double f, unsigned g, float h,
             long i, double j, float k, int l)
{
    return a + b + c + d + e + f + g + h + i + j + k + l;
}

struct big { double x[4]; };
double sbig(int n, struct big b, double tail1, float tail2)
{
    return n + b.x[0] + b.x[1] + b.x[2] + b.x[3] + tail1 + tail2;
}

int main(void)
{
    struct big b = {{1.5, 2.5, 3.5, 4.5}};
    printf("%.17g\n", d12(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12.5));
    printf("%.9g\n", f10(0.5f, 1, 1.5f, 2L, 2.5f, 3, 3.5f, 4L, 4.5f, 5.5f));
    printf("%.17g\n", mixed(1, 0.1, 'a', 0.25f, -3, 1e10, 4000000000u, 0.75f, -5L, 0.001, 1e-3f, 9));
    printf("%.17g\n", sbig(3, b, 0.5, 0.25f));
    printf("%.17g\n", sbig(-3, b, d12(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1), f10(1, 1, 1, 1, 1, 1, 1, 1, 1, 1)));
    return 0;
}
