/* Executed pragmas remain zero width through prescan, reuse, #, ## and rescans. */
#define P(x) _Pragma(x)
#define ID(x) x
#define D(x) x x
#define STR0(x) #x
#define STR(x) STR0(x)
#define CAT0(x,y) x##y
#define CAT(x,y) CAT0(x,y)
#define F(x) x
#define APPLY(x) F x
#define OP "unknown outer"
P(P("unknown inner") OP)
D(P("unknown repeated __COUNTER__"))
const char *s = STR(P("unknown stringify") word);
int CAT(P("unknown paste") wo,rd) = 7;
P("pack(push, 1)")
struct Packed { char c; int x; };
P("pack(pop)")
_Static_assert(sizeof(struct Packed) == 5, "pack effect");
_Static_assert(__COUNTER__ == 0, "ignored operand is raw");
_Static_assert(__COUNTER__ == 1, "ordinary counter");
/* GCC retains F across this pragma; its ordinary function has the same result. */
int (F)(int x) { return x; }
int main(void) {
    int rescan = APPLY(P("unknown rescan") (123));
    int frame = APPLY(P("unknown frame")) (456);
    return word != 7 || rescan != 123 || frame != 456;
}
