/* cfgTcc probe: plain char is unsigned (riscv64-gen.c CHAR_IS_UNSIGNED). */
#include <limits.h>
#include <stdio.h>

int main(void) {
    char c = (char)200;
    char m = (char)-1;
    int promoted = c;
    printf("c=%d m=%d CHAR_MIN=%d CHAR_MAX=%d\n", promoted, (int)m, CHAR_MIN, CHAR_MAX);
    printf("lt0=%d gt127=%d\n", c < 0, c > 127);
    return c < 0 ? 1 : 0;
}
