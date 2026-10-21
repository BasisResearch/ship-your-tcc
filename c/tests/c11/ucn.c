#include <stdio.h>
#define caf\u00e9 5
#define CAT(a,b) a ## b
#define TEXT(x) #x
#define XTEXT(x) TEXT(x)
int \U00010400 = 7;
int a\u0301 = 3;
int main(void) {
  int bad = café != 5 || 𐐀 != 7 || á != 3 || CAT(caf,\u00e9) != 5;
  bad |= sizeof XTEXT(TEXT(\u00e9)) != 9;
  printf("ucn %d\n", bad);
  return bad;
}
