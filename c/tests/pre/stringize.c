/* 6.10.3.2: stringizing. */
#define S(x) #x
#define XS(x) S(x)
const char *a = S("a\n" '\0' "\\" '"');
const char *b = S(  leading   and    trailing  );
const char *c = S(a
  b /* comment */ c);
const char *d = S('\'' "\"");
const char *e = S(	tab	);
const char *f = XS(__LINE__);
const char *g = S(@ \ $);
const char *h = S("" '');
