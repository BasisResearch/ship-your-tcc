/* Phase 3's program P: a loop that sums and prints with printf. It
 * declares printf itself (no #include), so tcc's compile phase lexes no
 * header: the captured-cut witness (PHASES 3a) is a shorter trace. */
int printf(const char *fmt, ...);

int main(void) {
  int s = 0;
  for (int i = 1; i <= 10; i++)
    s += i;
  printf("%d\n", s);
  return 0;
}
