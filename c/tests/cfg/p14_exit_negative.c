/* A negative main result becomes uint32_t on the image's HTIF wire. */
#include <stdio.h>
int main(void) {
    int code = -1;
    printf("exit=%d\n", code);
    return code;
}
