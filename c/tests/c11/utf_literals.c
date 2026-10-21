#include <stdio.h>
_Static_assert(__STDC_VERSION__ == 201112L, "C11 invocation");
#define LIT(x) x
#define TYPE(x, t) _Generic((x), t: 1, default: 0)
_Static_assert(TYPE(u""[0], unsigned short), "UTF16 type");
_Static_assert(TYPE(U""[0], unsigned int), "UTF32 type");
_Static_assert(TYPE(u'x', unsigned short), "UTF16 character type");
_Static_assert(TYPE(U'x', unsigned int), "UTF32 character type");
static unsigned short a[] = "é" u"\U0001f600" "z";
static unsigned int b[] = U"é" "\U0001f600" U"z";
static char c[] = u8"é" "\U0001f600";
int main(void) {
  unsigned short local[] = LIT(u"A") "B";
  int bad = sizeof a != 10 || sizeof b != 16 || sizeof c != 7;
  bad |= a[0] != 0xe9 || a[1] != 0xd83d || a[2] != 0xde00 || a[3] != 'z' || a[4];
  bad |= b[0] != 0xe9 || b[1] != 0x1f600 || b[2] != 'z' || b[3];
  bad |= (unsigned char)c[0] != 0xc3 || (unsigned char)c[1] != 0xa9 || (unsigned char)c[5] != 0x80 || c[6];
  bad |= local[0] != 'A' || local[1] != 'B' || local[2];
  bad |= sizeof("a" u"b") != 6 || sizeof(U"a" "b") != 12;
  bad |= u"\x12" "3"[0] != 0x12 || u"\x12" "3"[1] != '3';
  bad |= L"é" "x"[0] != 0xe9 || L"é" "x"[1] != 'x';
  bad |= sizeof(__FILE__ u"x") != 2*(sizeof(__FILE__)+1);
  printf("utf %d\n", bad);
  return bad;
}
