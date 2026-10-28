int main(void) {
  int a[2] = {1, 6}; int i = 0;
  a[i++] += 2;
  return i != 1 || a[0] != 3 || a[1] != 6;
}
