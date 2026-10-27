int main(void) {
  float a=1.0000000596046447753906250000008271806f;
  float b=1.000000059604644775390625f;
  long double c=0x1.000000000000000000000000000081p0L;
  long double d=0x1p-16494L;
  if(a!=0x1.000002p0f || b!=1.0f) return 1;
  if(c!=0x1.0000000000000000000000000001p0L) return 2;
  if(d/2.0L!=0.0L || d*1.5L!=d*2.0L) return 3;
  if((long long)-2.9L!=-2 || (long long)2.9f!=2) return 4;
  return 0;
}
