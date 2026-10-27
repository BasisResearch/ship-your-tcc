/* fs probe: stdio -- fopen/fgets/fputs/fclose over the image's files */
#include <errno.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    char line[128];
    FILE *f = fopen("/prog.c", "r");
    if (!f) { printf("fopen failed\n"); return 1; }
    if (fgets(line, sizeof line, f)) printf("first: %s", line);
    int lines = 1;
    while (fgets(line, sizeof line, f)) if (strchr(line, '\n')) lines++;
    printf("lines %d\n", lines);
    printf("fclose %d\n", fclose(f));
    f = fopen("/out.txt", "w");
    printf("fopen w %s\n", f ? "ok" : "NULL");
    fputs("written by stdio\n", f);
    fputs("second line\n", f);
    printf("fclose %d\n", fclose(f));
    f = fopen("/out.txt", "r");
    while (fgets(line, sizeof line, f)) printf("got: %s", line);
    fclose(f);
    errno = 0;
    f = fopen("/missing.txt", "r");
    printf("missing %s %s\n", f ? "FILE" : "NULL", errno == ENOENT ? "ENOENT" : "other");
    errno = 0;
    f = fopen("/nodir/x.txt", "w");
    printf("nodir %s %s\n", f ? "FILE" : "NULL", errno == ENOENT ? "ENOENT" : "other");
    printf("remove %d\n", remove("/out.txt"));
    errno = 0;
    int removed = remove("/out.txt");
    printf("remove again %d %s\n", removed, errno == ENOENT ? "ENOENT" : "other");
    return 0;
}
