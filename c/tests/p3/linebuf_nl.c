/* Control for linebuf.c: with a newline the line is flushed before printf
 * returns. Expected console "42\n", exit code 7. */
int printf(const char *fmt, ...);
void _exit(int);

int main(void) {
  printf("%d\n", 42);
  _exit(7);
}
