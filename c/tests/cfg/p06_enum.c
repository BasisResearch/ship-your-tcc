/* cfgTcc probe: tcc's choice of an enum's compatible type. */
#include <stdio.h>

enum e1 { A1 = 1, B1 = 2 };
enum e2 { A2 = -1, B2 = 1 };
enum e3 { A3 = 0x80000000u };
enum e4 { A4 = -1, B4 = 0x7fffffff };
enum e5 { A5 = 0x100000000LL };
enum e6 { A6 = -0x80000001LL };

int main(void) {
    enum e1 x = A1;
    enum e3 y = A3;
    printf("sizes %d %d %d %d %d %d\n", (int)sizeof(enum e1), (int)sizeof(enum e2),
           (int)sizeof(enum e3), (int)sizeof(enum e4), (int)sizeof(enum e5),
           (int)sizeof(enum e6));
    /* signedness of the compatible type: compare with a negative value */
    printf("e1 unsigned=%d e3 unsigned=%d\n", (x - 2) > 0, (y - y - 1) > 0);
    printf("A3>0=%d\n", A3 > 0);
    return (int)sizeof(enum e1);
}
