#include <errno.h>
#include <stdio.h>

int main(void) {
    errno = 0;
    FILE *f = fopen("/missing-file", "r");
    if (f != NULL) {
        fclose(f);
        return 1;
    }
    printf("missing file: %s\n", errno == ENOENT ? "ENOENT" : "wrong errno");
    return errno == ENOENT ? 0 : 2;
}
