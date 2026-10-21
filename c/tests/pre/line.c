/* 6.10.4 #line and 6.10.8.1 __LINE__ / __FILE__. */
int a = __LINE__; const char *fa = __FILE__;
#line 100
int b = __LINE__;
#line 200 "foo.c"
int c = __LINE__; const char *fc = __FILE__;
#define L __LINE__
int d = L;
#define F(x) x
int e = F(
  __LINE__
  );
int f = F(L
);
#line 300 "/abs/bar.c"
const char *fb = __FILE__;
