%:include <stdio.h>
%:define STR(x) %:x
%:define CAT(a,b) a %:%: b
%:if 0
%:if 1
#error skipped
%:endif
%:elif 1
%:define VALUE 7
%:else
#error skipped
%:endif
int main(void) <%
 int CAT(ar,r)<:2:> = <% VALUE, 3 %>;
 const char *s = STR(<: :> <% %> %: %:%:);
 int bad = arr<:0:> != 7 || arr<:1:> != 3;
 printf("%s %d\n", s, bad);
 return bad;
%>
