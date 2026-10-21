/* fs probe: stat -- stat/fstat fields, unlink while open */
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

static const char *ename(int e) {
    if (e == ENOENT) return "ENOENT";
    if (e == ENOTDIR) return "ENOTDIR";
    if (e == EISDIR) return "EISDIR";
    if (e == EBADF) return "EBADF";
    return "E?";
}

static void show(const char *label, int r, struct stat *st) {
    if (r < 0) { printf("%s -1 %s\n", label, ename(errno)); return; }
    printf("%s 0 reg %d dir %d chr %d perm %o nlink %ld size %ld\n", label,
           S_ISREG(st->st_mode) ? 1 : 0, S_ISDIR(st->st_mode) ? 1 : 0,
           S_ISCHR(st->st_mode) ? 1 : 0, (unsigned)(st->st_mode & 0777),
           (long)st->st_nlink, (long)st->st_size);
}

int main(void) {
    struct stat st;
    long total = 0;
    char buf[64];
    int n;
    errno = 0; show("stat /prog.c", stat("/prog.c", &st), &st);
    int fd = open("/prog.c", O_RDONLY);
    while ((n = (int)read(fd, buf, sizeof buf)) > 0) total += n;
    printf("bytes read %ld\n", total);
    errno = 0; show("fstat fd", fstat(fd, &st), &st);
    errno = 0; show("stat /lib", stat("/lib", &st), &st);
    errno = 0; show("stat /lib/tcc", stat("/lib/tcc", &st), &st);
    errno = 0; show("stat /", stat("/", &st), &st);
    errno = 0; show("stat /missing", stat("/missing", &st), &st);
    errno = 0; show("stat /prog.c/", stat("/prog.c/", &st), &st);
    errno = 0; show("stat /missing/x", stat("/missing/x", &st), &st);
    errno = 0; show("fstat 0", fstat(0, &st), &st);
    errno = 0; show("fstat 1", fstat(1, &st), &st);
    errno = 0; show("fstat 30", fstat(30, &st), &st);
    errno = 0; printf("unlink /lib %d %s\n", unlink("/lib"), ename(errno));
    errno = 0; printf("unlink /missing/ %d %s\n", unlink("/missing/"), ename(errno));
    errno = 0; printf("unlink /missing %d %s\n", unlink("/missing"), ename(errno));
    printf("unlink /prog.c %d\n", unlink("/prog.c"));
    errno = 0; show("stat after unlink", stat("/prog.c", &st), &st);
    errno = 0; show("fstat after unlink", fstat(fd, &st), &st);
    lseek(fd, 0, SEEK_SET);
    printf("read after unlink %d\n", (int)read(fd, buf, 10));
    close(fd);
    return 0;
}
