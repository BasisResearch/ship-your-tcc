/* fs probe: mkdir -- the image's newlib has no mkdir/rmdir for the program */
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>

int main(void) {
    errno = 0;
    int r = mkdir("/d", 0755);
    printf("mkdir %d %d\n", r, errno);
    return 0;
}
