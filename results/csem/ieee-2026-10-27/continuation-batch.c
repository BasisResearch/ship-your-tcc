/* Focused continuation; retain the original stopped batch separately. */
#define main ieee_0
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
#define main ieee_1
int main(void) {
  float f=0.0f; double d; long double q;
  unsigned char *p=(unsigned char *)&f, *r=(unsigned char *)&d, *s=(unsigned char *)&q;
  unsigned char ed[]={0,0,0,128,70,2,248,255}; unsigned char eq[]={0,0,0,0,0,0,0,0,0,0,0,104,36,128,255,255};
  int i;
  p[0]=52;p[1]=18;p[2]=192;p[3]=255;
  d=f; q=f;
  for(i=0;i<8;i++) if(r[i]!=ed[i]) return 1;
  for(i=0;i<16;i++) if(s[i]!=eq[i]) return 2;
  f=(float)q;
  if(p[0] || p[1] || p[2]!=192 || p[3]!=127) return 3;
  return 0;
}

#undef main
#define main ieee_2
#include <stdio.h>
int main(void) {
  float f=0.1f;
  long double q=0x1.0000000000000000000000000001p0L;
  printf("[%+.2f][%08.2f][%-9.3e][%#.4g][%a]\n", (double)f, -1.5, 0.125, 1.0, -0.0);
  printf("[%.36Lf][%La][%.4LE][%Lg]\n", q, q, 0x1p100L, 1e4000L);
  return 0;
}

#undef main
int main(void) { int r, bad=0;
r=ieee_0(); printf("nan-bytes=%d\n",r);bad+=r!=0;
r=ieee_1(); printf("nan-conversions=%d\n",r);bad+=r!=0;
r=ieee_2(); printf("printf=%d\n",r);bad+=r!=0;
return bad;}
