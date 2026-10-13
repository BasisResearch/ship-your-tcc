/* small ASCII Mandelbrot: double arithmetic and comparisons in tight loops */
#include <stdio.h>

int main(void)
{
    int x, y, total = 0;
    for (y = -6; y <= 6; y++) {
        for (x = -20; x <= 6; x++) {
            double cr = x / 12.0, ci = y / 8.0, zr = 0, zi = 0;
            int it = 0;
            while (it < 20 && zr * zr + zi * zi <= 4.0) {
                double t = zr * zr - zi * zi + cr;
                zi = 2 * zr * zi + ci;
                zr = t;
                it++;
            }
            total += it;
            putchar(" .:-=+*#%@"[it % 10]);
        }
        putchar('\n');
    }
    printf("total=%d\n", total);
    return 0;
}
