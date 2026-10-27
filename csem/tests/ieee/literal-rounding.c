#include <stdio.h>
#include <string.h>
int main(void) {
  float a=1.0000000596046447753906250000008271806f;
  float b=1.000000059604644775390625f;
  unsigned x,y;
  memcpy(&x,&a,4);memcpy(&y,&b,4);
  printf("%08x %08x\n",x,y);
  return 0;
}
