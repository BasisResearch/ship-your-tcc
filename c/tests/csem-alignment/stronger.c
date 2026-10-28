typedef int Up __attribute__((aligned(16)));
typedef int Down __attribute__((aligned(1)));
typedef Up Again;
typedef Down Strong __attribute__((aligned(32))); typedef Strong S;
int main(void){return (_Alignof(S)!=32 || sizeof(S)!=4 || _Alignof(Down)!=1);}
