/* printf formats: integers, strings, chars, widths, a partial last line
   (flushed by the program's exit), and a nonzero exit status */
#include <stdio.h>
#include <string.h>

int main(void)
{
    char buf[64];
    long big = 0x7fffffffffffffffL;
    printf("[%d] [%5d] [%-5d] [%05d]\n", 42, 42, 42, 42);
    printf("[%x] [%X] [%o] [%#x]\n", 255, 255, 8, 255);
    printf("[%ld] [%lu]\n", big, (unsigned long)-1);
    printf("[%s] [%10s] [%-10s] [%.3s]\n", "abc", "right", "left", "truncate");
    printf("[%c%c%c] [%%]\n", 't', 'c', 'c');
    snprintf(buf, sizeof buf, "%s-%d", "snprintf", 7);
    puts(buf);
    printf("strlen = %d\n", (int)strlen(buf));
    fputs("no newline at the end", stdout);
    return 3;
}
