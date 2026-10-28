typedef int Up __attribute__((aligned(16)));
typedef int Down __attribute__((aligned(1)));
typedef Up Again;
typedef Up *P; typedef Down *Q; typedef P R;
int main(void){return (_Alignof(Up*)!=sizeof(void*) || _Alignof(P)!=sizeof(void*) || _Alignof(R)!=sizeof(void*) || _Alignof(Q)!=sizeof(void*) || _Alignof(Up**)!=sizeof(void*) || sizeof(P)!=sizeof(void*));}
