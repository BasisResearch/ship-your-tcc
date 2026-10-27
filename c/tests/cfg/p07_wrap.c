/* cfgTcc probe: implementation-defined integer conversions and shifts are
   two's complement wraparound / arithmetic (C11 6.3.1.3p3, 6.5.7p5). */
#include <stdio.h>

int main(void) {
    signed char sc = (signed char)200;
    short sh = (short)70000;
    int i = (int)4000000000u;
    long l = (long)0xffffffffffffffffull;
    int neg = -8;
    printf("%d %d %d %ld\n", sc, sh, i, l);
    printf("%d %d %ld\n", neg >> 1, (-1) >> 31, (-17L) >> 2);
    printf("%d\n", (int)(unsigned char)-3);
    return sc < 0;
}
