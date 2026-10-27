/* fs probe: rename_link -- newlib's rename is link+unlink; the image's link is EMLINK */
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

static const char *ename(int e) {
    if (e == EMLINK) return "EMLINK";
    if (e == ENOENT) return "ENOENT";
    if (e == EEXIST) return "EEXIST";
    return "E?";
}

int main(void) {
    struct stat st;
    int r;
    int fd = open("/a.txt", O_CREAT | O_WRONLY, 0644);
    write(fd, "abc", 3);
    close(fd);
    errno = 0;
    r = rename("/a.txt", "/b.txt"); printf("rename %d %s\n", r, ename(errno));
    errno = 0;
    r = rename("/missing", "/c.txt"); printf("rename missing %d %s\n", r, ename(errno));
    errno = 0;
    r = link("/a.txt", "/d.txt"); printf("link %d %s\n", r, ename(errno));
    printf("stat a %d\n", stat("/a.txt", &st));
    errno = 0;
    r = stat("/b.txt", &st); printf("stat b %d %s\n", r, ename(errno));
    return 0;
}
