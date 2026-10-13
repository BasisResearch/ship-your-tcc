/* ship-your-tcc: the image's /lib/tcc/runmain.o, which tcc_run links into
 * every program (tcc_add_support(s1, "runmain.o")). Adapted from TinyCC's
 * lib/runmain.c (43c7708, LGPL-2.1): the constructors run as there, but
 * where TinyCC's version defines its own exit() and atexit() and returns
 * into tcc through __rt_exit/longjmp, this one ends in the program's own
 * newlib exit() (from the libc.a linked into it), as a bare-metal crt0
 * does: atexit handlers run, the program's stdio is flushed and _exit
 * (the image's htif.c) stops the machine with main's status. Built by
 * c/Makefile with the image's rv64i/lp64 flags, without -ffunction-sections,
 * so its code follows the program's own in .text. */

extern void (*__init_array_start[])(int argc, char **argv, char **envp);
extern void (*__init_array_end[])(int argc, char **argv, char **envp);

int main(int, char **, char **);
void exit(int) __attribute__((noreturn));

int _runmain(int argc, char **argv, char **envp)
{
    int i = 0;
    while (&__init_array_start[i] != __init_array_end)
        (*__init_array_start[i++])(argc, argv, envp);
    exit(main(argc, argv, envp));
}
