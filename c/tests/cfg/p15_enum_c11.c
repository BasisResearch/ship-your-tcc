/* C11 enums: every enumerator fits int. p06 also measures tcc extensions. */
#include <stdio.h>
enum positive { P0 = 0, P1 = 2147483647 };
enum negative { N0 = -1, N1 = 1 };
int main(void) {
    enum positive p = P0;
    enum negative n = N0;
    printf("size=%d/%d align=%d/%d unsigned=%d/%d\n",
           (int)sizeof p, (int)sizeof n,
           (int)_Alignof(enum positive), (int)_Alignof(enum negative),
           p - 1 > 0, n < 0);
    return 0;
}
