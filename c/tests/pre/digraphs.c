/* 6.4.6p3: digraphs are punctuators (spelled as written). */
%:define DG(a) %:a
%:define CAT(a, b) a %:%: b
int arr<:3:> = <% 1, 2, 3 %>;
const char *s = DG(<:);
int CAT(x, y) = 1;
