#include <stdio.h>
#line 41 "elsewhere/relative.c"
#include "line_include.h"
int main(void) {
  const char *s = __FILE__;
  int bad = s[0]!='e' || INCLUDED_FROM_REAL_DIRECTORY!=19 || __LINE__!=44;
  printf("line %s %d\n", s, bad);
  return bad;
}
