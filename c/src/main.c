/* Bare-metal entry for tcc -run (TinyCC 43c7708 + patches/) under HTIF.
 *
 * The command line is baked in at build time, in the `.embed` section with
 * the file system (gen_embed.sh, at a fixed address: src/link.ld), so the
 * image's own code and data do not depend on it:
 *   argv = { "tcc", "-std=c11", args..., "-run", "/prog.c", NULL }
 * tcc compiles /prog.c from htif.c's in-image file system, links it in
 * memory against /lib/libc.a, /lib/libgcc.a and /lib/tcc/libtcc1.a through
 * its archive loader, relocates it into the static arena (link.ld,
 * patches/0002) and calls tcc_baremetal_run below, which hands over to the
 * program. The program's main returns into runmain.o's _runmain, which
 * calls the program's own exit(): its stdio is flushed and _exit (htif.c)
 * stops the machine with main's status. tcc itself never resumes.
 *
 * Step accounting (c/tests/run.sh traces these pcs):
 *   _start .. tcc_baremetal_run        the compile phase
 *   tcc_baremetal_run .. _enter        the optional code dump (flag bit 0)
 *   tcc_baremetal_enter .. _exit's sd  the program
 */
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/times.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int tcc_main(int argc, char **argv);   /* tcc.c's main, renamed (Makefile) */

extern char **environ;

/* gen_embed.sh */
extern char **const embedded_argv;
extern char **const embedded_env;
extern const uintptr_t embedded_flags;

int main(void) {
    int argc = 0;
    environ = embedded_env;
    while (embedded_argv[argc])
        argc++;
    return tcc_main(argc, embedded_argv);
}

/* --- what tcc needs beyond newlib ----------------------------------------- */

/* tccrun.c's PAGESIZE (only alignment: there are no pages to protect) */
long sysconf(int name) {
    return name == _SC_PAGESIZE ? 4096 : -1;
}

/* the working directory is the root (htif.c's `resolve`) */
char *getcwd(char *buf, size_t size) {
    if (!buf || size < 2) return NULL;
    buf[0] = '/'; buf[1] = 0;
    return buf;
}

/* libtcc.c's #pragma once comparison (normalized_PATHCMP) only: there are no
 * symbolic links, so a path is its own canonical name (`a/../b` and `b`
 * compare unequal, which at worst reads such a header twice). */
char *realpath(const char *path, char *resolved) {
    size_t n = strlen(path) + 1;
    if (!resolved && !(resolved = malloc(n))) return NULL;
    memcpy(resolved, path, n);
    return resolved;
}

/* --- the program's porting layer ---------------------------------------- */

/* newlib's system-call layer (libgloss's names) for the program's own copy
 * of libc.a, added to tcc's symbol table before the archives are searched
 * (patches/0002, tcc_run). These are htif.c's functions, so the program and
 * tcc share one console, one file system and one sbrk heap. Anything else
 * the program needs comes from the archives; an unresolved name is a link
 * error (tcc's dlsym table is empty under TCC_BAREMETAL). */
ssize_t _read(int, void *, size_t);
ssize_t _write(int, const void *, size_t);
int _open(const char *, int, int);
int _close(int);
off_t _lseek(int, off_t, int);
int _fstat(int, struct stat *);
int _stat(const char *, struct stat *);
int _isatty(int);
int _link(const char *, const char *);
int _unlink(const char *);
void *_sbrk(ptrdiff_t);
void _exit(int);
int _kill(int, int);
int _getpid(void);
int _gettimeofday(struct timeval *, void *);
clock_t _times(struct tms *);
int mkdir(const char *, mode_t);

void tcc_baremetal_sync_icache(void *beg, void *end);

/* The newlib *_r syscall wrappers copy this raw error slot into their
 * own _reent. Share only the slot with the generated program's libc, not
 * its __errno() or _impure_ptr (which also owns stdio and exit handlers). */
#undef errno
extern int errno;

struct tcc_baremetal_sym { const char *name; void *addr; };
const struct tcc_baremetal_sym tcc_baremetal_syms[] = {
    { "errno", (void *)&errno },
    { "_read", (void *)_read },
    { "_write", (void *)_write },
    { "_open", (void *)_open },
    { "_close", (void *)_close },
    { "_lseek", (void *)_lseek },
    { "_fstat", (void *)_fstat },
    { "_stat", (void *)_stat },
    { "_isatty", (void *)_isatty },
    { "_link", (void *)_link },
    { "_unlink", (void *)_unlink },
    { "_sbrk", (void *)_sbrk },
    { "_exit", (void *)_exit },
    { "_kill", (void *)_kill },
    { "_getpid", (void *)_getpid },
    { "_gettimeofday", (void *)_gettimeofday },
    { "_times", (void *)_times },
    { "_mkdir", (void *)mkdir },
    /* see tcc_baremetal_sync_icache */
    { "__clear_cache", (void *)tcc_baremetal_sync_icache },
    { NULL, NULL },
};

/* --- the hand-off ------------------------------------------------------- */

/* tccrun.c's protect_pages calls this once the code is in place, where a
 * RISC-V system would execute fence.i (BRIEF.md asks for it). The executable
 * Sail model has no Zifencei: fence.i is an illegal instruction there
 * (results/isa-probes.json, scripts/probe_sail.py), and it has no
 * instruction cache, so fetches see the relocated code without it. A plain
 * `fence` (base RV64I) stands in for it, ordering the stores of the
 * relocation. The program gets this function as __clear_cache too
 * (tcc_baremetal_syms), so libtcc1.a's armflush.o, whose __clear_cache
 * executes fence.i, is never linked. */
__attribute__((noinline)) void tcc_baremetal_sync_icache(void *beg, void *end) {
    (void)beg; (void)end;
    __asm__ volatile ("fence" ::: "memory");
}

/* The marker: the program starts at this function's first instruction. */
__attribute__((noinline, noipa))
int tcc_baremetal_enter(int (*prog_main)(int, char **, char **),
                        int argc, char **argv, char **envp) {
    return prog_main(argc, argv, envp);
}

static void put_hex(char *p, uintptr_t v, int digits) {
    while (digits--) {
        p[digits] = "0123456789abcdef"[v & 15];
        v >>= 4;
    }
}

/* Flag bit 0: print the program's own generated code (tcc's output for
 * /prog.c, without runmain.o and the archive members linked after it):
 *   @@TCCBM code <addr> <size>
 *   @@<up to 32 bytes as hex>        (one line per 32 bytes)
 * c/tests/run.sh takes these lines out of the program's stdout and
 * disassembles them. */
static void dump_code(const unsigned char *code, unsigned long size) {
    char line[80] = "@@TCCBM code 0x";
    put_hex(line + 15, (uintptr_t)code, 16);
    line[31] = ' ';
    put_hex(line + 32, size, 8);
    line[40] = '\n';
    _write(1, line, 41);
    for (unsigned long i = 0; i < size; i += 32) {
        unsigned long n = size - i < 32 ? size - i : 32;
        line[0] = line[1] = '@';
        for (unsigned long j = 0; j < n; j++)
            put_hex(line + 2 + 2 * j, code[i + j], 2);
        line[2 + 2 * n] = '\n';
        _write(1, line, 3 + 2 * n);
    }
}

/* Called by tcc_run (patches/0002) in place of its call of the program:
 * the compile phase ends at this function's first instruction. */
__attribute__((noinline, noipa))
int tcc_baremetal_run(int (*prog_main)(int, char **, char **),
                      int argc, char **argv, char **envp,
                      void *code, unsigned long code_size) {
    if (embedded_flags & 1)
        dump_code(code, code_size);
    return tcc_baremetal_enter(prog_main, argc, argv, envp);
}
