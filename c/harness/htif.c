/* HTIF back end for newlib, console only (differential harness, xgcc_sail).
 *
 * Adapted from ~/Documents/code/ship-your-lua/c/src/htif.c: the HTIF
 * console (device 1 / command 1 through `tohost`), _exit (device 0, low
 * payload bit set), _sbrk between _end and __heap_end, and the frozen
 * clock are copied as they are. The in-image file system is dropped: the
 * corpus is restricted to programs whose only I/O is stdout, so open()
 * fails with ENOENT and stdin is empty.
 *
 * Writes to fd 2 (stderr) are DROPPED: the harness compares stdout only
 * (gcc_native's stdout is captured separately from its stderr), and on
 * Sail both would share the one HTIF console. Keep `_exit` as it is: the
 * `sd` to tohost is its 5th instruction, and scripts/run_xgcc_sail.sh
 * traces that pc (_exit+16) to count steps, as ship-your-lua's
 * c/tests/difftest.sh does. */
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

volatile uint64_t tohost __attribute__((section(".tohost"), aligned(8)));
volatile uint64_t fromhost __attribute__((section(".tohost"), aligned(8)));

#define HTIF_DEV_CONSOLE (1ULL << 56)
#define HTIF_CMD_WRITE   (1ULL << 48)

static void htif_putc(char c) {
    tohost = HTIF_DEV_CONSOLE | HTIF_CMD_WRITE | (uint8_t)c;
}

void _exit(int code) {
    tohost = ((uint64_t)(uint32_t)code << 1) | 1;
    for (;;) {}
}

ssize_t _write(int fd, const void *buf, size_t len) {
    const char *p = buf;
    if (fd == 1) {
        for (size_t i = 0; i < len; i++) htif_putc(p[i]);
        return (ssize_t)len;
    }
    if (fd == 2) return (ssize_t)len;   /* stderr: dropped, see above */
    errno = EBADF;
    return -1;
}

ssize_t _read(int fd, void *buf, size_t len) {
    (void)buf; (void)len;
    if (fd == 0) return 0;              /* no console input over HTIF */
    errno = EBADF;
    return -1;
}

int _open(const char *path, int flags, int mode) {
    (void)path; (void)flags; (void)mode;
    errno = ENOENT;
    return -1;
}

int _close(int fd) {
    if (fd >= 0 && fd <= 2) return 0;
    errno = EBADF;
    return -1;
}

int _fstat(int fd, struct stat *st) {
    if (fd >= 0 && fd <= 2) {
        memset(st, 0, sizeof *st);
        st->st_mode = S_IFCHR;
        return 0;
    }
    errno = EBADF;
    return -1;
}

int _isatty(int fd) {
    if (fd >= 0 && fd <= 2) return 1;
    errno = EBADF;
    return 0;
}

off_t _lseek(int fd, off_t off, int whence) {
    (void)fd; (void)off; (void)whence;
    errno = ESPIPE;
    return -1;
}

int _unlink(const char *p) { (void)p; errno = ENOENT; return -1; }
int _link(const char *a, const char *b) { (void)a; (void)b; errno = EMLINK; return -1; }

extern char _end[];       /* from link.ld */
extern char __heap_end[];

void *_sbrk(ptrdiff_t incr) {
    static char *brk;
    if (!brk) brk = _end;
    if (brk + incr > __heap_end) { errno = ENOMEM; return (void *)-1; }
    char *prev = brk;
    brk += incr;
    return prev;
}

/* Frozen clock (newlib's libgloss versions ecall, which traps here). */
int _gettimeofday(struct timeval *tv, void *tz) {
    (void)tz;
    if (tv) { tv->tv_sec = 0; tv->tv_usec = 0; }
    return 0;
}
clock_t _times(struct tms *t) {
    if (t) memset(t, 0, sizeof *t);
    return 0;
}

int _kill(int pid, int sig) { (void)pid; (void)sig; errno = EINVAL; return -1; }
int _getpid(void) { return 1; }
