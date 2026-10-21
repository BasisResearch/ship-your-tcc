/* Every keyword formerly omitted by Parser.isTok and used for dispatch. */
_Static_assert(_Generic(0, int: 1, default: 0), "integer generic");
static int first(int a[static 2]) { return a[0]; }
int main(void) {
  int a[2] = {0, 1};
  if (first(a))
    if (a[1]) return 2;
    else return 3;
  else switch (a[1]) {
    case 0: return 4;
    default: return 0;
  }
}
