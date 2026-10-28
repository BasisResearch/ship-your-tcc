#include <stdio.h>
struct S { unsigned a:3; };
int main(void) {
 struct S s={1};
 printf("direct=%d postfix=%d assign=%d prefix=%d comma=%d\n",
   _Generic(+s.a,int:1,unsigned:2,default:3),
   _Generic(+s.a++,int:1,unsigned:2,default:3),
   _Generic(+(s.a=1),int:1,unsigned:2,default:3),
   _Generic(+(++s.a),int:1,unsigned:2,default:3),
   _Generic(+(0,s.a),int:1,unsigned:2,default:3));
 return 0;
}
