/* F3 model test: preprocessor corners (tccpp.c) */
#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#pragma once
#define STR(x) #x
#define XSTR(x) STR(x)
#define CAT(a, b) a ## b
#define CAT3(a, b, c) a ## b ## c
#define VA(fmt, ...) printf(fmt, ##__VA_ARGS__)
#define VA2(...) __VA_ARGS__
#define EMPTY
#define F(x) (x + 1)
#define G F
#define H(x) G(x) * F(x)
#define REC REC + 1
#pragma push_macro("F")
#undef F
#define F(x) (x * 100)
int pushed = F(2);
#pragma pop_macro("F")
int popped = F(2);
#pragma pack(push, 1)
struct packed { char c; int i; long l; };
#pragma pack(pop)
#pragma pack(2)
struct p2 { char c; int i; };
#pragma pack()
#if defined(__TINYC__) && __STDC_VERSION__ >= 199901L && !defined(__x86_64__) && __riscv
int tiny = 1;
#elif 1
int tiny = 2;
#else
#error no
#endif
#if (1 ? 2 : 3) == 2 && (0x10 >> 2) == 4 && -1 < 0 && ~0u > 0 && 'a' == 97 && 10 / 3 == 3 && 10 % 3 == 1
int arith = 1;
#endif
#ifdef UNDEFINED_THING
int nope;
#elif defined UNDEFINED_THING || !defined(__STDC__)
int nope2;
#else
int yes = __LINE__;
#endif
#line 1000 "renamed.c"
int line1000 = __LINE__;
const char *fname = __FILE__;
const char *base = __BASE_FILE__;
const char *date = __DATE__ " " __TIME__;
int counter[] = { __COUNTER__, __COUNTER__, __COUNTER__ };
int REC_val = 0;
#define REC_val_m REC
int main(void)
{
    int CAT(x, 1) = 5, CAT3(y, 2, z) = 6;
    VA("%d %d\n", x1, y2z);
    VA("plain\n");
    printf("%s %s %s\n", STR(a + b), XSTR(__LINE__), XSTR(H(3)));
    printf("%d %d %d %d\n", H(3), pushed, popped, tiny);
    printf("%d %d %d\n", (int)sizeof(struct packed), (int)sizeof(struct p2), arith);
    printf("%d %s %s %d %d %d\n", line1000, fname, base, counter[0], counter[1], counter[2]);
    printf("%d %d %ld\n", VA2(1, 2), (long)INT_MAX);
    printf("%s\n", date);
    printf("%d\n", yes EMPTY);
    { int REC = 1; printf("%d\n", REC_val_m); }
    return 0;
}
