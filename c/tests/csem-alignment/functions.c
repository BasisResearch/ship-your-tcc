typedef int Up __attribute__((aligned(16)));
typedef int Down __attribute__((aligned(1)));
typedef Up Again;
typedef Up F(void); typedef F *FP; Up fun(void){return 7;}
int main(void){return (_Alignof(F)!=_Alignof(int(void)) || _Alignof(FP)!=sizeof(void*) || sizeof(FP)!=sizeof(void*) || _Alignof(__typeof__(fun()))!=16 || fun()!=7);}
