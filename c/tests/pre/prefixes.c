/* 6.4.4.4 / 6.4.5: character-constant and string-literal prefixes. */
#define S(x) #x
const char *a = u8"utf8";
const void *b = u"utf16";
const void *c = U"utf32";
const void *d = L"wide";
int e = u'x' + U'y' + L'z';
const char *f = S(u8"q");
/* the prefix is part of the token: a macro named like a prefix does not apply */
#define u8 PREFIX_IS_MACRO
#define u PREFIX_IS_MACRO
#define U PREFIX_IS_MACRO
#define L PREFIX_IS_MACRO
const void *g[] = { u8"a", u"b", U"c", L"d" };
int h = u'e' + U'f' + L'g';
