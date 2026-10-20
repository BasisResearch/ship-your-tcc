/* PrintfSpec evidence (results/p3/printf-inventory.md): stdout of the
 * program's newlib is line buffered in this image (_fstat(1) reports a
 * character device, _isatty(1) = 1), so printf of output without a newline
 * returns with the bytes still in stdout's buffer, not on the console.
 * _exit skips exit()'s stdio flush: expected console "", exit code 7. */
int printf(const char *fmt, ...);
void _exit(int);

int main(void) {
  printf("%d", 42);
  _exit(7);
}
