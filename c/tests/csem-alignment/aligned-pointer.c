typedef int Up __attribute__((aligned(16)));
typedef int Down __attribute__((aligned(1)));
typedef Up Again;
typedef Up *P __attribute__((aligned(32))); typedef P *PP;
int main(void){return (_Alignof(P)!=32 || _Alignof(PP)!=sizeof(void*) || sizeof(P)!=sizeof(void*));}
