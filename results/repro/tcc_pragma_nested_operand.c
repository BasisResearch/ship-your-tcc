#define P(x) _Pragma(x)
#define OP "unknown outer"
P(P("unknown inner") OP)
int main(void) { return 0; }
