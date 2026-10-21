/* fs probe: paths -- one file under many spellings, and resolution errors */
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
    return "E?";
}

static void probe(const char *p) {
    char buf[16];
    long total = 0;
    int n;
    errno = 0;
    int fd = open(p, O_RDONLY);
    if (fd < 0) { printf("[%s] -1 %s\n", p, ename(errno)); return; }
    while ((n = (int)read(fd, buf, sizeof buf)) > 0) total += n;
    printf("[%s] fd %d size %ld last %d", p, fd, total, n);
    if (n < 0) printf(" %s", ename(errno));
    printf("\n");
    close(fd);
}

int main(void) {
    probe("/prog.c");
    probe("./prog.c");
    probe("//prog.c");
    probe("prog.c");
    probe("/./prog.c");
    probe("/../prog.c");
    probe("../../prog.c");
    probe("/include/../prog.c");
    probe("/lib/tcc/../../prog.c");
    probe("/usr/../prog.c");
    probe("/prog.c/x");
    probe("/prog.c/");
    probe("/prog.c/.");
    probe("/missing");
    probe("/missing/x");
    probe("");
    probe("/lib//tcc///include/stddef.h");
    probe("lib/tcc/include/../include/./stddef.h");
    probe("/lib");
    probe("/");
    return 0;
}
