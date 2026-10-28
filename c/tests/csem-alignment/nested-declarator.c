typedef int Up __attribute__((aligned(16)));
typedef int Down __attribute__((aligned(1)));
typedef Up Again;
typedef Down (*P)[3]; typedef Up (*F)(Down); Up fun(Down x){return x+1;}
int main(void){return (_Alignof(P)!=sizeof(void*) || _Alignof(F)!=sizeof(void*) || sizeof(P)!=sizeof(void*) || fun(4)!=5);}
