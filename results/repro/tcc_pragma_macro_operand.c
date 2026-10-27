/* C11 _Pragma operands must expand inside replacement-list streams too. */
#include <stdio.h>
#define PACK_TEXT "pack(push, 1)"
#define ENTER_PACK _Pragma(PACK_TEXT)
ENTER_PACK
struct P { char c; int x; };
_Pragma("pack(pop)")
#define STRINGIZE(x) #x
#define ENTER_FUNCTION _Pragma(STRINGIZE(pack(push, 1)))
ENTER_FUNCTION
struct Q { char c; int x; };
_Pragma("pack(pop)")
#define PASS(x) x
#define ENTER_CHAIN _Pragma(PASS(PACK_TEXT))
ENTER_CHAIN
struct R { char c; int x; };
_Pragma("pack(pop)")
struct S { char c; int x; };
_Static_assert(sizeof(struct P) == 5, "object operand");
_Static_assert(sizeof(struct Q) == 5, "function operand");
_Static_assert(sizeof(struct R) == 5, "nested operand");
_Static_assert(sizeof(struct S) == 8, "restored pack");
int main(void) {
    printf("pragma operands %d\n", sizeof(struct P) != 5 || sizeof(struct Q) != 5 ||
           sizeof(struct R) != 5 || sizeof(struct S) != 8);
    return 0;
}
