/* fs probe: read_self -- open+read /prog.c, the program's own source */
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
    char buf[64];
    long total = 0, lines = 0;
    int fd = open("/prog.c", O_RDONLY);
    if (fd < 0) { printf("open failed\n"); return 1; }
    printf("fd %d\n", fd);
    int n = (int)read(fd, buf, 32);
    printf("first read %d\n", n);
    for (int i = 0; i < n && buf[i] != '\n'; i++) putchar(buf[i]);
    putchar('\n');
    total = n;
    for (int i = 0; i < n; i++) if (buf[i] == '\n') lines++;
    while ((n = (int)read(fd, buf, sizeof buf)) > 0) {
        total += n;
        for (int i = 0; i < n; i++) if (buf[i] == '\n') lines++;
    }
    printf("last read %d total %ld lines %ld\n", n, total, lines);
    printf("close %d\n", close(fd));
    return 0;
}
