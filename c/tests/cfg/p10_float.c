/* cfgTcc probe: float is binary32, double binary64, evaluated in their own
   types (FLT_EVAL_METHOD 0); conversions round to nearest even. */
#include <stdio.h>

int main(void) {
    float f = 16777216.0f;          /* 2^24 */
    float g = f + 1.0f;             /* rounds back to 2^24 in binary32 */
    double d = 16777216.0;
    float third = 1.0f / 3.0f;
    double dthird = 1.0 / 3.0;
    printf("sizes %d %d\n", (int)sizeof(float), (int)sizeof(double));
    printf("g==f %d d+1==d %d\n", g == f, d + 1.0 == d);
    printf("third %d\n", (double)third == dthird);
    printf("cvt %d %d %ld\n", (int)2.9f, (int)-2.9, (long)1e18);
    printf("round %d\n", (float)0.1 == 0.1f);
    return g == f;
}
