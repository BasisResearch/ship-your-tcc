typedef int Up __attribute__((aligned(16)));
typedef int Down __attribute__((aligned(1)));
typedef Up Again;
typedef __typeof__(Up) TU; typedef __typeof__(Down) TD; typedef __typeof__(Again) TA;
int main(void){return (_Alignof(TU)!=16 || _Alignof(TD)!=1 || _Alignof(TA)!=16 || _Alignof(__typeof__(Up*))!=sizeof(void*));}
