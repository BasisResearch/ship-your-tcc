/* cfgTcc probe: main's argc/argv as `tcc -run /prog.c` passes them. */
#include <stdio.h>

int main(int argc, char **argv) {
    int i;
    printf("argc=%d\n", argc);
    for (i = 0; i < argc; i++)
        printf("argv[%d]=%s\n", i, argv[i]);
    printf("null=%d\n", argv[argc] == 0);
    return argc;
}
