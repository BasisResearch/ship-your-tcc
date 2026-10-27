/* cfgTcc probe: stdin is empty (htif.c _read on fd 0 returns 0). */
#include <stdio.h>

int main(void) {
    int c = getchar();
    printf("eof=%d feof=%d\n", c == EOF, feof(stdin) != 0);
    return c == EOF ? 0 : 1;
}
