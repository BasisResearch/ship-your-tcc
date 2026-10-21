/* Phase F2 (results/pre/f2-diff.md): tcc does not recognise digraphs.
 * C99 and C11 6.4.6p3: "In all aspects of the language, the six tokens
 * <: :> <% %> %: %:%: behave, respectively, the same as [ ] { } # ##
 * except for their spelling." tccpp.c's lexer (next_nomacro1) has no case
 * for them: `<:` is `<` `:`, and `%:` at the start of a line is not a
 * directive. Expected output "2 3"; tcc rejects the program. Also in
 * pristine tcc 43c7708 (tccpp.c has no digraph code). CSem.Pre:
 * c/tests/pre/digraphs.c, Spec/Pre/Test.lean. */
%:include <stdio.h>
%:define CAT(a, b) a %:%: b
int main(void) <%
  int CAT(a, rr)<:2:> = <% 2, 3 %>;
  printf("%d %d\n", arr<:0:>, arr<:1:>);
  return 0;
%>
