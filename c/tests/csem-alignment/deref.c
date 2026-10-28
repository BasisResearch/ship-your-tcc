typedef int Up __attribute__((aligned(16)));
typedef int Down __attribute__((aligned(1)));
typedef Up Again;
typedef Up *P; Up fun(void){return 0;}
int main(void){return (_Alignof(__typeof__(*(Up*)0))!=16 || _Alignof(__typeof__(*(P)0))!=16 || _Alignof(__typeof__(fun()))!=16);}
