/* the in-image file system is generic: most of the standard headers.
   <stdio.h> comes first on purpose. tcc does not define __GNUC__, so
   newlib's machine/_default_types.h (via sys/cdefs.h) includes <limits.h>
   to define its types; when the program's own first header is <limits.h>,
   that nested include re-runs limits.h's unguarded tail, which defines
   ARG_MAX and PATH_MAX as 4096 before sys/syslimits.h defines them
   (65536, 1024): tcc warns "ARG_MAX redefined" on the console, which is
   also stdout here, and the values differ from gcc's. With another header
   first, limits.h is entered whole from _default_types.h and the values
   are sys/syslimits.h's, as with gcc. */
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <inttypes.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static jmp_buf jb;

static int sum(int n, ...)
{
    va_list ap;
    int s = 0;
    va_start(ap, n);
    while (n--)
        s += va_arg(ap, int);
    va_end(ap);
    return s;
}

int main(void)
{
    char buf[32];
    int8_t i8 = INT8_MIN;
    uint64_t u64 = UINT64_MAX;
    bool ok = true;
    char *p;
    printf("INT_MAX=%d CHAR_BIT=%d i8=%d\n", INT_MAX, CHAR_BIT, i8);
    printf("u64=%" PRIu64 " sizeof(intptr_t)=%d\n", u64, (int)sizeof(intptr_t));
    printf("toupper=%c isdigit=%d\n", toupper('q'), isdigit('7') != 0);
    printf("strtol=%ld atoi=%d\n", strtol("-1234", NULL, 10), atoi("77"));
    strcpy(buf, "abc");
    strcat(buf, "def");
    printf("buf=%s cmp=%d chr=%s\n", buf, strcmp(buf, "abcdef"), strchr(buf, 'd'));
    p = malloc(16);
    memset(p, 'x', 15);
    p[15] = 0;
    printf("malloc=%s\n", p);
    free(p);
    printf("sum=%d offsetof=%d ok=%d\n", sum(4, 1, 2, 3, 4), (int)offsetof(struct { char c; int i; }, i), ok);
    if (setjmp(jb) == 0)
        longjmp(jb, 5);
    else
        printf("longjmp back\n");
    errno = 0;
    assert(sizeof(long) == 8);
    printf("abs=%d labs=%ld\n", abs(-5), labs(-6L));
    return EXIT_SUCCESS;
}
