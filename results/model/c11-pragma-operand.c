/* Repaired valid C11: expand the operand macro inside the replacement
   stream, using the caller's active-macro list (patch 0003). */
#define PACK_TEXT "pack(push, 1)"
#define ENTER_PACK _Pragma(PACK_TEXT)
ENTER_PACK
struct P { char c; int x; };
_Pragma("pack(pop)")
_Static_assert(sizeof(struct P) == 5, "macro-expanded pragma operand");
int main(void) { return 0; }
