/* Newlib 4.5.0 error and termination constants; errno is observable data. */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
int main(void) {
    printf("EDOM=%d ERANGE=%d EILSEQ=%d ENOENT=%d EINVAL=%d\n",
           EDOM, ERANGE, EILSEQ, ENOENT, EINVAL);
    printf("EXIT_SUCCESS=%d EXIT_FAILURE=%d errno=%d\n", EXIT_SUCCESS, EXIT_FAILURE, errno);
    errno = ERANGE;
    printf("set=%d\n", errno);
    return EXIT_FAILURE;
}
