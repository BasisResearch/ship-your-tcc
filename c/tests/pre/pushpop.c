/* #pragma push_macro / pop_macro (tcc extension). */
#define X 1
X
#pragma push_macro("X")
#undef X
#define X 2
X
#pragma pop_macro("X")
X
