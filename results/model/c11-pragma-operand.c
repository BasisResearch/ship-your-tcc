/* Residual patched-compiler refusal on valid C11: an operand macro inside
   an object-like macro containing _Pragma. Keep separate from successes. */
#define PACK_TEXT "pack(push, 1)"
#define ENTER_PACK _Pragma(PACK_TEXT)
ENTER_PACK
struct P { char c; int x; };
_Pragma("pack(pop)")
_Static_assert(sizeof(struct P) == 5, "macro-expanded pragma operand");
int main(void) { return 0; }
