_Pragma("unknown __COUNTER__")
_Static_assert(__COUNTER__ == 0, "ignored operator must not expand operands");
#pragma unknown __COUNTER__
_Static_assert(__COUNTER__ == 1, "ignored directive must not expand operands");
int main(void) { return 0; }
