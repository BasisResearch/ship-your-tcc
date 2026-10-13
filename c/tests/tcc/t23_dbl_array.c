/* arrays of doubles: sums, dot products, matrix multiply, sorting */
#include <stdio.h>

#define N 6
double A[N][N], B[N][N], C[N][N];

int main(void)
{
    double v[20], w[20], dot = 0, sum = 0;
    float fv[10];
    int i, j, k;
    for (i = 0; i < 20; i++) { v[i] = i * 0.5 - 3; w[i] = 1.0 / (i + 1); }
    for (i = 0; i < 20; i++) { dot += v[i] * w[i]; sum += v[i]; }
    printf("%.17g %.17g\n", dot, sum);
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++) { A[i][j] = i + j * 0.25; B[i][j] = (i == j) ? 2.0 : 0.5 / (i + j + 1); }
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++) {
            double s = 0;
            for (k = 0; k < N; k++) s += A[i][k] * B[k][j];
            C[i][j] = s;
        }
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) printf("%.6f ", C[i][j]);
        printf("\n");
    }
    for (i = 0; i < 10; i++) fv[i] = (float)((i * 7) % 10) - 4.5f;
    for (i = 0; i < 10; i++)
        for (j = 0; j + 1 < 10 - i; j++)
            if (fv[j] > fv[j + 1]) { float t = fv[j]; fv[j] = fv[j + 1]; fv[j + 1] = t; }
    for (i = 0; i < 10; i++) printf("%g ", fv[i]);
    printf("\n");
    {
        double *p = v + 5, *q = &v[15];
        printf("%ld %g %g\n", (long)(q - p), p[3], *(q - 2));
    }
    return 0;
}
