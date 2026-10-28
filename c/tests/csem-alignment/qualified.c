typedef int Up __attribute__((aligned(16)));
typedef int Down __attribute__((aligned(1)));
typedef Up Again;
typedef const Up C; typedef volatile Down V;
int main(void){return (_Alignof(C)!=16 || _Alignof(V)!=1 || _Alignof(const Again)!=16 || _Alignof(volatile Down)!=1 || sizeof(C)!=4);}
