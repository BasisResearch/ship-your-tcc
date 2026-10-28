int arr[3] __attribute__((aligned(32)));
int main(void){return (__alignof__(arr)!=32 || __alignof__(&arr)!=sizeof(void*) || sizeof(arr)!=12 || _Alignof(__typeof__(arr))!=4);}
