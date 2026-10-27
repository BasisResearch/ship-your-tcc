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
