typedef int Up __attribute__((aligned(16)));
typedef int Down __attribute__((aligned(1)));
typedef Up Again;
typedef Down A[3]; typedef A *P; typedef Up *V[2];
int main(void){return (_Alignof(P)!=sizeof(void*) || _Alignof(V)!=sizeof(void*) || _Alignof(Down(*)[3])!=sizeof(void*) || sizeof(V)!=2*sizeof(void*));}
