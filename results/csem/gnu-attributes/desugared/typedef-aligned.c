/* Historical Menhir attribute syntax, not raw C11 syntax. */
#include <stddef.h>
typedef int T [[gnu::aligned("16")]];
struct S {char c;T i;};
int main(void){return sizeof(T)!=4 || _Alignof(T)!=16 || offsetof(struct S,i)!=16 || sizeof(struct S)!=32;}
