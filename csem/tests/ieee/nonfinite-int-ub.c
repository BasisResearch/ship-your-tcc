int main(void) {
  double x=0.0;
  unsigned char *p=(unsigned char *)&x;
  p[6]=240; p[7]=127;
  return (int)x;
}
