#include <string.h>
#ifdef SUBJECT
#include "subject.h"
#define strncpy subject_strncpy
#define strncmp subject_strncmp
#endif
int main(void) {
#if CASE == 1
 char dst[4]={'X','Y','Z',0}; char src[1]={0};
 char *r=strncpy(dst,src,0);
 return r!=dst || dst[0]!='X' || dst[1]!='Y' || dst[2]!='Z';
#elif CASE == 2
 char a[2]={'a',0},b[2]={'b',0};
 return strncmp(a,b,0)!=0;
#elif CASE == 3
 char dst[4]={'X','Y','Z',0}; char src[2]={'a',0};
 char *r=strncpy(dst,src,1);
 return r!=dst || dst[0]!='a' || dst[1]!='Y';
#elif CASE == 4
 char a[2]={'a',0},b[2]={'b',0};
 return !(strncmp(a,b,1)<0);
#else
#error missing case
#endif
}
