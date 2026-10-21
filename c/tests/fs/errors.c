/* fs probe: errors -- open/read/write/lseek/close error choices, EMFILE */
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

static const char *ename(int e) {
    if (e == ENOENT) return "ENOENT";
    if (e == ENOTDIR) return "ENOTDIR";
    if (e == EISDIR) return "EISDIR";
    if (e == EEXIST) return "EEXIST";
    if (e == EINVAL) return "EINVAL";
    if (e == EBADF) return "EBADF";
    if (e == EMFILE) return "EMFILE";
    if (e == ESPIPE) return "ESPIPE";
    return "E?";
}

#define SHOW(label, expr) do { errno = 0; long r_ = (long)(expr); \
    printf("%s %ld %s\n", label, r_, r_ < 0 ? ename(errno) : "-"); } while (0)

int main(void) {
    char buf[8];
    SHOW("excl existing", open("/prog.c", O_CREAT | O_EXCL | O_WRONLY, 0644));
    SHOW("creat dir slash", open("/newdir/", O_CREAT | O_WRONLY, 0644));
    SHOW("creat on dir", open("/lib", O_CREAT | O_RDONLY, 0644));
    SHOW("write dir", open("/lib", O_WRONLY));
    SHOW("directory on file", open("/prog.c", O_RDONLY | O_DIRECTORY));
    int dfd = open("/lib", O_RDONLY);
    printf("dir open %d\n", dfd);
    SHOW("read dir", read(dfd, buf, 4));
    SHOW("lseek end dir", lseek(dfd, 0, SEEK_END));
    SHOW("close dir", close(dfd));
    int fd = open("/prog.c", O_RDONLY);
    SHOW("write rdonly", write(fd, "x", 1));
    SHOW("lseek bad whence", lseek(fd, 0, 7));
    SHOW("lseek negative", lseek(fd, -1, SEEK_SET));
    SHOW("close", close(fd));
    SHOW("close again", close(fd));
    SHOW("close 99", close(99));
    SHOW("close -1", close(-1));
    SHOW("read 40", read(40, buf, 1));
    SHOW("lseek stdout", lseek(1, 0, SEEK_SET));
    int wfd = open("/w.txt", O_CREAT | O_WRONLY, 0644);
    SHOW("read wronly", read(wfd, buf, 1));
    SHOW("excl new", open("/w2.txt", O_CREAT | O_EXCL | O_RDWR, 0644));
    int count = 0, last = 0;
    for (int i = 0; i < 40; i++) {
        errno = 0;
        int f = open("/prog.c", O_RDONLY);
        if (f < 0) { printf("open %d failed %s after %d, last fd %d\n", i, ename(errno), count, last); break; }
        count++; last = f;
    }
    SHOW("close 10", close(10));
    SHOW("reuse", open("/prog.c", O_RDONLY));
    return 0;
}
