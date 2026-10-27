/* Raw RV64 helper observations. Volatile operands prevent GCC's x*1 fold. */
#include <stdio.h>
#include <string.h>
typedef unsigned long long U;
static void pf(const char *s,float x){unsigned u;memcpy(&u,&x,4);printf("%s %08x\n",s,u);}
static void pd(const char *s,double x){U u;memcpy(&u,&x,8);printf("%s %016llx\n",s,u);}
static void pq(const char *s,long double x){U u[2];memcpy(u,&x,16);printf("%s %016llx%016llx\n",s,u[1],u[0]);}
int main(void){
  unsigned fu=0xffc01234U;U du=0xfff8000000001234ULL;
  U qu[2]={0x1234ULL,0xffff800000000000ULL};
  float f;double d;long double q;
  memcpy(&f,&fu,4);memcpy(&d,&du,8);memcpy(&q,qu,16);
  volatile float vf=f,fo=1.0f;volatile double vd=d,do_=1.0;volatile long double vq=q,qo=1.0L;
  pf("f-add",vf+fo);pf("f-mul",vf*fo);pd("d-add",vd+do_);pd("d-mul",vd*do_);pq("q-add",vq+qo);pq("q-mul",vq*qo);
  pd("f-d",(double)vf);pq("f-q",(long double)vf);pf("d-f",(float)vd);pq("d-q",(long double)vd);pf("q-f",(float)vq);pd("q-d",(double)vq);
  fu=0xff800001U;du=0xfff0000000000001ULL;qu[0]=1;qu[1]=0xffff000000000000ULL;
  memcpy(&f,&fu,4);memcpy(&d,&du,8);memcpy(&q,qu,16);vf=f;vd=d;vq=q;
  pd("sf-d",(double)vf);pq("sf-q",(long double)vf);pf("sd-f",(float)vd);pq("sd-q",(long double)vd);pf("sq-f",(float)vq);pd("sq-d",(double)vq);
  return 0;
}
