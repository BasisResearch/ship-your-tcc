typedef int Up __attribute__((aligned(16)));
typedef int Down __attribute__((aligned(1)));
typedef Up Again;
typedef Up F(void); typedef __typeof__(F) TF; typedef F *FP;
int main(void){return (_Alignof(F)!=_Alignof(int(void)) || _Alignof(TF)!=_Alignof(F) || _Alignof(__typeof__(F))!=_Alignof(F) || _Alignof(FP)!=sizeof(void*));}
