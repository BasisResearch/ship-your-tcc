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
