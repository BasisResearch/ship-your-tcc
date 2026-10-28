/* Historical Menhir attribute syntax, not raw C11 syntax. */
#include <stddef.h>
struct [[gnu::packed]] S {char c;int i;};
int main(void){struct S s={1,37};return sizeof s!=5 || _Alignof(struct S)!=1 || offsetof(struct S,i)!=1 || s.i!=37;}
