/* function pointers taking/returning doubles; qsort with a double comparator */
#include <stdio.h>
#include <stdlib.h>

typedef double (*binop)(double, double);
typedef float (*fun)(float);

static double add(double a, double b) { return a + b; }
static double sub(double a, double b) { return a - b; }
static double mul(double a, double b) { return a * b; }
static double dvd(double a, double b) { return a / b; }
static float half(float x) { return x / 2; }
static float sq(float x) { return x * x; }

double fold(binop f, const double *v, int n)
{
    double acc = v[0];
    int i;
    for (i = 1; i < n; i++) acc = f(acc, v[i]);
    return acc;
}

static int cmpd(const void *a, const void *b)
{
    double x = *(const double *)a, y = *(const double *)b;
    return (x > y) - (x < y);
}

struct op { const char *name; binop f; };

int main(void)
{
    double v[] = {3.5, -1.25, 8.0, 0.1, 2.0, -7.75, 100.0, 0.5};
    struct op ops[] = {{"add", add}, {"sub", sub}, {"mul", mul}, {"div", dvd}};
    fun fs[2] = {half, sq};
    int i;
    for (i = 0; i < 4; i++) printf("%s %.17g\n", ops[i].name, fold(ops[i].f, v, 8));
    for (i = 0; i < 2; i++) printf("%.9g %.9g\n", fs[i](3.0f), (*fs[i])(-0.5f));
    {
        binop p = &mul;
        printf("%g %g\n", p(1.5, 4), (i ? add : sub)(10, 3));
    }
    qsort(v, 8, sizeof v[0], cmpd);
    for (i = 0; i < 8; i++) printf("%g ", v[i]);
    printf("\n");
    return 0;
}
