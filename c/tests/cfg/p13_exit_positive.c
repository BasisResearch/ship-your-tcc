/* _exit writes (uint32_t)code to HTIF; the Sail host exposes low 8 bits.
   Keep the full C value in stdout so truncation cannot masquerade as agreement. */
#include <stdio.h>
int main(void) {
    int code = 0x12345678;
    printf("exit=%d\n", code);
    return code;
}
