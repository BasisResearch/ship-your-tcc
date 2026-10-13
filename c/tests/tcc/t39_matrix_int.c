/* integer matrices: multiply, fraction-free (Bareiss) determinant */
#include <stdio.h>

#define N 5

static long det_bareiss(long m[N][N])
{
    long a[N][N], prev = 1;
    int i, j, k, sign = 1;
    for (i = 0; i < N; i++) for (j = 0; j < N; j++) a[i][j] = m[i][j];
    for (k = 0; k < N - 1; k++) {
        if (a[k][k] == 0) {
            int r;
            for (r = k + 1; r < N && a[r][k] == 0; r++) ;
            if (r == N) return 0;
            for (j = 0; j < N; j++) { long t = a[k][j]; a[k][j] = a[r][j]; a[r][j] = t; }
            sign = -sign;
        }
        for (i = k + 1; i < N; i++)
            for (j = k + 1; j < N; j++)
                a[i][j] = (a[i][j] * a[k][k] - a[i][k] * a[k][j]) / prev;
        prev = a[k][k];
    }
    return sign * a[N - 1][N - 1];
}

int main(void)
{
    long A[N][N], B[N][N], C[N][N];
    int i, j, k;
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++) {
            A[i][j] = (i * 7 + j * 3) % 11 - 5;
            B[i][j] = (i == j) ? 2 : (i + 2 * j) % 4 - 1;
        }
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++) {
            long s = 0;
            for (k = 0; k < N; k++) s += A[i][k] * B[k][j];
            C[i][j] = s;
        }
    for (i = 0; i < N; i++) { for (j = 0; j < N; j++) printf("%4ld", C[i][j]); printf("\n"); }
    printf("det A=%ld B=%ld C=%ld\n", det_bareiss(A), det_bareiss(B), det_bareiss(C));
    {
        int m[3][3] = {{2, -3, 1}, {2, 0, -1}, {1, 4, 5}}, d;
        d = m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) - m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0])
          + m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
        printf("det3=%d avg=%d rem=%d\n", d, d / 9, d % 9);
    }
    return 0;
}
