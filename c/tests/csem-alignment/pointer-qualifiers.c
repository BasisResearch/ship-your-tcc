typedef int Up __attribute__((aligned(16)));
typedef int Down __attribute__((aligned(1)));
typedef Up Again;
typedef Up *const CP; typedef Down *volatile VP; typedef Up *restrict RP;
int main(void){return (_Alignof(CP)!=sizeof(void*) || _Alignof(VP)!=sizeof(void*) || _Alignof(RP)!=sizeof(void*));}
