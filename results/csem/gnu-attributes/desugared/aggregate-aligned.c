/* Historical Menhir attribute syntax: explicit semantic counterpart only. */
struct [[gnu::aligned("16")]] S {char c;};
int main(void){return sizeof(struct S)!=16 || _Alignof(struct S)!=16;}
