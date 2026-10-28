typedef struct { long a,b; } Wide __attribute__((aligned(16))); typedef Wide A[2];
int main(void){return (_Alignof(A)!=16 || sizeof(A)!=32 || _Alignof(Wide)!=16);}
