typedef int Up __attribute__((aligned(16)));
typedef int Down __attribute__((aligned(1)));
typedef Up Again;
typedef Down A[3]; typedef A B[2]; typedef const A C;
int main(void){return (_Alignof(A)!=1 || _Alignof(B)!=1 || _Alignof(C)!=1 || sizeof(A)!=12 || sizeof(B)!=24 || _Alignof(Down[3])!=1);}
