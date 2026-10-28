typedef int Up __attribute__((aligned(16)));
typedef int Down __attribute__((aligned(1)));
typedef Up Again;
Up x; typedef int *P __attribute__((aligned(32))); P p;
int main(void){return (__alignof__(&x)!=sizeof(void*) || __alignof__(&p)!=sizeof(void*) || __alignof__((int)x)!=4 || _Alignof(__typeof__(&x))!=sizeof(void*));}
