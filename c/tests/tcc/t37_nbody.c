/* a few steps of a 3-body leapfrog integration (no libm: sqrt by Newton) */
#include <stdio.h>

struct body { double x, y, vx, vy, m; };

static double my_sqrt(double a)
{
    double x = a > 1 ? a : 1;
    int i;
    if (a == 0) return 0;
    for (i = 0; i < 25; i++) x = 0.5 * (x + a / x);
    return x;
}

static double energy(struct body *b, int n)
{
    double e = 0;
    int i, j;
    for (i = 0; i < n; i++) {
        e += 0.5 * b[i].m * (b[i].vx * b[i].vx + b[i].vy * b[i].vy);
        for (j = i + 1; j < n; j++) {
            double dx = b[i].x - b[j].x, dy = b[i].y - b[j].y;
            e -= b[i].m * b[j].m / my_sqrt(dx * dx + dy * dy);
        }
    }
    return e;
}

int main(void)
{
    struct body b[3] = {{0, 0, 0, 0, 10}, {1, 0, 0, 3, 0.1}, {-2, 0, 0, -2.2, 0.05}};
    double dt = 0.001;
    int s, i, j;
    printf("e0=%.12f\n", energy(b, 3));
    for (s = 0; s < 25; s++) {
        for (i = 0; i < 3; i++)
            for (j = 0; j < 3; j++) {
                double dx, dy, r2, inv;
                if (i == j) continue;
                dx = b[j].x - b[i].x; dy = b[j].y - b[i].y;
                r2 = dx * dx + dy * dy;
                inv = b[j].m / (r2 * my_sqrt(r2));
                b[i].vx += dt * dx * inv;
                b[i].vy += dt * dy * inv;
            }
        for (i = 0; i < 3; i++) { b[i].x += dt * b[i].vx; b[i].y += dt * b[i].vy; }
    }
    for (i = 0; i < 3; i++) printf("%.9f %.9f %.9f %.9f\n", b[i].x, b[i].y, b[i].vx, b[i].vy);
    printf("e1=%.12f\n", energy(b, 3));
    return 0;
}
