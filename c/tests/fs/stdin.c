/* fs probe: stdin -- the image's stdin is empty; fd 0 is not writable */
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
    char buf[8];
    int c = getchar();
    printf("getchar %s\n", c == EOF ? "EOF" : "char");
    errno = 0;
    printf("read 0: %d\n", (int)read(0, buf, sizeof buf));
    errno = 0;
    int w = (int)write(0, "x", 1);
    printf("write 0: %d %s\n", w, w < 0 && errno == EBADF ? "EBADF" : "other");
    errno = 0;
    int r = (int)read(1, buf, 1);
    printf("read 1: %d %s\n", r, r < 0 && errno == EBADF ? "EBADF" : "other");
    char line[16];
    printf("fgets %s\n", fgets(line, sizeof line, stdin) == NULL ? "NULL" : "line");
    return c == EOF ? 0 : 1;
}
