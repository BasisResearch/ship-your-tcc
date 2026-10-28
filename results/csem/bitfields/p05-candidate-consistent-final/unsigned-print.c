/* cfgTcc probe: tcc's bit-field layout (tccgen.c struct_layout): plain int
   bit-fields are signed; straddling and packing into storage units. */
#include <stdio.h>
#include <string.h>

struct b1 { int a : 3; unsigned b : 5; int c : 30; };
struct b2 { char a : 7; int b : 10; };
struct b3 { unsigned a : 1; unsigned : 0; unsigned b : 1; };
struct b4 { char c; int x : 4; };
struct b5 { long long a : 40; int b : 30; };

int main(void) {
    struct b1 s;
    unsigned char raw[sizeof(struct b2)];
    struct b2 t;
    memset(&s, 0, sizeof s);
    s.a = 7;
    s.b = 31;
    s.c = -5;
    printf("a=%d b=%d c=%d\n", s.a, (int)s.b, s.c);
    printf("sizes b1=%d b2=%d b3=%d b4=%d b5=%d\n", (int)sizeof(struct b1),
           (int)sizeof(struct b2), (int)sizeof(struct b3), (int)sizeof(struct b4),
           (int)sizeof(struct b5));
    memset(&t, 0, sizeof t);
    t.a = 0x55;
    t.b = 0x2aa;
    memcpy(raw, &t, sizeof t);
    printf("raw b2 = %02x %02x %02x %02x\n", (unsigned)raw[0], (unsigned)raw[1], (unsigned)raw[2], (unsigned)raw[3]);
    return s.a < 0;
}
