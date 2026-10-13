/* global/static float data, initializers, static locals */
#include <stdio.h>

double gd = 3.25;
float gf = -1.5f;
static double sd[5] = {1, 0.5, 0.25, 0.125};
long double gl = 2.0L;
struct cfg { int n; double scale; float bias; } gcfg = {4, 1.5, 0.25f};
struct cfg gcfgs[2] = {{1, 2.0, 3.0f}, {4, 5.0, 6.0f}};
double *gp = &sd[2];
const float gtab[] = {1.0f / 3, 2.0f / 3, 1.0f};

double counter(void) { static double c = 0.5; c *= 2; return c; }

int main(void)
{
    int i;
    double s = 0;
    gd *= 2; gf += gd; sd[4] = gd + gf; gl = gl * 3 + 1;
    for (i = 0; i < 5; i++) s += sd[i];
    printf("%g %g %g %g\n", gd, (double)gf, s, (double)gl);
    printf("%d %g %g | %g %g\n", gcfg.n, gcfg.scale * gcfg.n, (double)gcfg.bias, gcfgs[1].scale, (double)gcfgs[0].bias);
    *gp += 10; printf("%g %g\n", sd[2], gp[-1]);
    for (i = 0; i < 3; i++) printf("%.9g ", gtab[i]);
    printf("\n");
    for (i = 0; i < 5; i++) printf("%g ", counter());
    printf("\n");
    return 0;
}
