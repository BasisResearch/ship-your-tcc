typedef int Up __attribute__((aligned(16)));
typedef int Down __attribute__((aligned(1)));
typedef Up Again;
typedef Again Weak __attribute__((aligned(2))); typedef Weak W;
int main(void){return (_Alignof(W)!=2 || sizeof(W)!=4 || _Alignof(Up)!=16);}
