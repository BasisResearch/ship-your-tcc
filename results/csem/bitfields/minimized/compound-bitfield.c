struct S { unsigned a:3; };
int main(void) {
  struct S a[2] = {{1}, {6}}; int i = 0;
  a[i++].a += 2;
  return i != 1 || a[0].a != 3 || a[1].a != 6;
}
