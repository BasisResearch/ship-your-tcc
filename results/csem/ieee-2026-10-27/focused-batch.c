/* Generated from csem/tests/ieee by the focused comparison driver. */
#define main ieee_0
/* Format retention: arithmetic, explicit casts, function args/returns,
   conditional conversions, object stores/loads and integer conversion. */
static float sf(float x) { return x + 1.0f; }
static long double qf(long double x) { return x + 1.0L; }
int main(void) {
  float f = 16777216.0f;
  long double q = 0x1p100L;
  unsigned long long n = 9007199254740993ULL;
  float direct = (float)4611686293305294849LL;
  float narrow = (float)0x1.0000010000000000000000000001p0L;
  long double fromint = (long double)n;
  long double mixed = q + n;
  long double cond = 1 ? q + 1.0L : 1.0;
  if (sizeof f != 4 || sizeof q != 16) return 1;
  if (sf(f) != f || f + 1.0f != f) return 2;
  if (qf(q) - q != 1.0L) return 3;
  if (fromint - 9007199254740992.0L != 1.0L) return 4;
  if (mixed - q != 9007199254740993.0L || cond - q != 1.0L) return 5;
  if (direct != 0x1.000002p62f) return 6;
  if (narrow != 0x1.000002p0f) return 7;
  return 0;
}

#undef main
#define main ieee_1
int main(void) {
  float f = -0.0f;
  double d = -0.0;
  long double q = -0.0L;
  unsigned char *a=(unsigned char *)&f, *b=(unsigned char *)&d, *c=(unsigned char *)&q;
  int i;
  for(i=0;i<3;i++) if(a[i]) return 1;
  for(i=0;i<7;i++) if(b[i]) return 2;
  for(i=0;i<15;i++) if(c[i]) return 3;
  if(a[3]!=128 || b[7]!=128 || c[15]!=128) return 4;
  c[0]=1; c[15]=0;
  if(q != 0x1p-16494L) return 5;
  a[0]=1; a[3]=0;
  if(f != 0x1p-149f) return 6;
  b[0]=1; b[7]=0;
  if(d != 0x1p-1074) return 7;
  return 0;
}

#undef main
#define main ieee_2
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

#undef main
#define main ieee_3
/* Quiet/signalling input payloads survive byte loads/stores; arithmetic and
   real format conversion use the RV64 canonical positive quiet-NaN policy. */
int main(void) {
  double x=0.0, y=0.0;
  unsigned char *p=(unsigned char *)&x;
  unsigned char *r=(unsigned char *)&y;
  int i;
  p[0]=52; p[1]=18; p[6]=248; p[7]=255;
  y=x;
  for(i=0;i<8;i++) if(p[i]!=r[i]) return 1;
  if(x==x || x<0.0 || x>0.0) return 2;
  y=x+1.0;
  for(i=0;i<6;i++) if(r[i]) return 3;
  if(r[6]!=248 || r[7]!=127) return 4;
  return 0;
}

#undef main
#define main ieee_4
#include <stdio.h>
int main(void) {
  float f=0.1f;
  long double q=0x1.0000000000000000000000000001p0L;
  printf("[%+.2f][%08.2f][%-9.3e][%#.4g][%a]\n", (double)f, -1.5, 0.125, 1.0, -0.0);
  printf("[%.36Lf][%La][%.4LE][%Lg]\n", q, q, 0x1p100L, 1e4000L);
  return 0;
}

#undef main
int main(void) { int r;
r=ieee_0(); printf("formats=%d\n",r); if(r) return 1;
r=ieee_1(); printf("bytes=%d\n",r); if(r) return 2;
r=ieee_2(); printf("rounding=%d\n",r); if(r) return 3;
r=ieee_3(); printf("nan-bytes=%d\n",r); if(r) return 4;
r=ieee_4(); printf("printf=%d\n",r); if(r) return 5;
return 0; }
