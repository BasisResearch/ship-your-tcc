/* RV64 binary128 decimal literals must equal their exact hexadecimal forms.
   Newlib 4.5.0's strtold in the pinned xPack rv64i multilib calls _strtorx_l,
   which writes an x87 80-bit result into a 16-byte binary128 object. */
#include <stdio.h>
int main(void) {
    volatile long double decimal = 1.5L;
    volatile long double hexadecimal = 0x1.8p0L;
    printf("%d %g %g\n", decimal == hexadecimal,
           (double)decimal, (double)hexadecimal);
    return decimal != hexadecimal;
}
