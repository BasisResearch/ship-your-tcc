/* double comparisons incl. NaN, infinities, signed zero; in value and
   branch contexts */
#include <stdio.h>

int main(void)
{
    volatile double zero = 0.0;
    double nan = zero / zero, inf = 1.0 / zero, ninf = -1.0 / zero, nz = -zero;
    double v[8];
    int i, j, cnt = 0;
    v[0] = 0.0; v[1] = nz; v[2] = 1.0; v[3] = -1.0; v[4] = inf; v[5] = ninf; v[6] = nan; v[7] = 1e-320;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            double a = v[i], b = v[j];
            int r = (a == b) | (a != b) << 1 | (a < b) << 2 | (a <= b) << 3 | (a > b) << 4 | (a >= b) << 5;
            int br = 0;
            if (a == b) br |= 1;
            if (a != b) br |= 2;
            if (a < b) br |= 4;
            if (a <= b) br |= 8;
            if (a > b) br |= 16;
            if (a >= b) br |= 32;
            if (!(a < b)) br |= 64;
            if (!(a == b)) br |= 128;
            printf("%d%d:%02x/%02x ", i, j, r, br);
            cnt += r;
        }
        printf("\n");
    }
    /* comparisons in loops and && || */
    {
        double x = 0.0; int n = 0;
        while (x < 10.0) { x += 0.7; n++; }
        printf("n=%d\n", n);
        for (x = 5.0; x >= -5.0 && n < 100; x -= 1.25) n++;
        printf("n=%d\n", n);
        printf("%d %d %d\n", nan == nan, nan != nan, (nan < 1.0) || (nan >= 1.0));
        printf("%d %d\n", nz == 0.0, inf == inf);
    }
    {   /* compare with constants */
        double a = 2.5;
        printf("%d %d %d %d %d %d\n", a > 2.0, a < 3, 1 < a, a == 2.5, a != 2.5f, -a <= -2.5);
    }
    printf("cnt=%d\n", cnt);
    return cnt & 0x7f;
}
