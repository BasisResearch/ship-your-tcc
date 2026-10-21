/* 6.10.3.3: placemarkers, ## with empty arguments. */
#define c3(a, b, c) a ## b ## c
#define c2(a, b) a ## b
#define pre(a) x ## a
#define post(a) a ## x
#define mid(a, b) a ## - ## b
c3(,,) c3(a,,) c3(,b,) c3(,,c) c2(,) pre() post() mid(,)
#define EMPTY
c2(EMPTY, x) c2(x, EMPTY)
#define str(a) #a
#define xstr(a) str(a)
xstr(c2(,)) xstr(c3(1,,2))
