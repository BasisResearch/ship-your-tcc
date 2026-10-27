#include <string.h>
int main(void) { char a[3] = "ab"; memcpy(a + 1, a, 2); return 0; }
