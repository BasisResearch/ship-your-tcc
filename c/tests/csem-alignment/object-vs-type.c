int obj __attribute__((aligned(32))); typedef int Alias __attribute__((aligned(16))); Alias a;
int main(void){return (_Alignof(__typeof__(obj))!=4 || _Alignof(__typeof__(a))!=16 || _Alignof(__typeof__(&obj))!=sizeof(void*));}
