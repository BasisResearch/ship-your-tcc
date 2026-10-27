/* fs probe: create_rw -- create, write, lseek, read back, unlink */
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(void) {
    char buf[32];
    int wrote, opened;
    long pos;
    int fd = open("/new.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
    printf("open %d\n", fd);
    printf("write %d\n", (int)write(fd, "hello world", 11));
    printf("lseek set %ld\n", (long)lseek(fd, 0, SEEK_SET));
    memset(buf, 0, sizeof buf);
    printf("read %d [%s]\n", (int)read(fd, buf, 5), buf);
    printf("lseek cur %ld\n", (long)lseek(fd, 1, SEEK_CUR));
    memset(buf, 0, sizeof buf);
    printf("read %d [%s]\n", (int)read(fd, buf, 31), buf);
    printf("read at eof %d\n", (int)read(fd, buf, 31));
    printf("lseek past end %ld\n", (long)lseek(fd, 20, SEEK_SET));
    printf("write %d\n", (int)write(fd, "X", 1));
    printf("lseek end %ld\n", (long)lseek(fd, 0, SEEK_END));
    printf("lseek end-3 %ld\n", (long)lseek(fd, -3, SEEK_END));
    printf("close %d\n", close(fd));
    fd = open("/new.txt", O_RDONLY);
    int n = (int)read(fd, buf, sizeof buf);
    int zeros = 0;
    for (int i = 0; i < n; i++) if (buf[i] == 0) zeros++;
    printf("reopen %d read %d zeros %d last %c\n", fd, n, zeros, n > 0 ? buf[n - 1] : '?');
    close(fd);
    fd = open("/new.txt", O_WRONLY | O_TRUNC);
    printf("trunc open %d write %d\n", fd, (int)write(fd, "ab", 2));
    close(fd);
    fd = open("/new.txt", O_WRONLY | O_APPEND);
    wrote = (int)write(fd, "cd", 2); pos = (long)lseek(fd, 0, SEEK_CUR);
    printf("append open %d write %d pos %ld\n", fd, wrote, pos);
    lseek(fd, 0, SEEK_SET);
    wrote = (int)write(fd, "ef", 2); pos = (long)lseek(fd, 0, SEEK_CUR);
    printf("append again %d pos %ld\n", wrote, pos);
    close(fd);
    fd = open("/new.txt", O_RDONLY);
    memset(buf, 0, sizeof buf);
    printf("content %d [%s]\n", (int)read(fd, buf, sizeof buf - 1), buf);
    close(fd);
    printf("unlink %d\n", unlink("/new.txt"));
    errno = 0;
    opened = open("/new.txt", O_RDONLY);
    printf("reopen after unlink %d %s\n", opened, errno == ENOENT ? "ENOENT" : "other");
    errno = 0;
    opened = open("/nodir/x", O_CREAT | O_WRONLY, 0644);
    printf("create in missing dir %d %s\n", opened,
           errno == ENOENT ? "ENOENT" : "other");
    fd = open("/lib/made.txt", O_CREAT | O_WRONLY, 0644);
    printf("create in /lib %d write %d\n", fd, (int)write(fd, "z", 1));
    close(fd);
    fd = open("lib/../lib/made.txt", O_RDONLY);
    printf("read via relative %d n %d\n", fd, (int)read(fd, buf, 4));
    close(fd);
    return 0;
}
