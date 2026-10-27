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
