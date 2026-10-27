/* C11 phase 1 must precede splicing, comments and tokenization. */
??=include <stdio.h>
??=include "tcc_pp_trigraphs??-.h"
??=define CAT(a,b) a ??=??= b
??=define STR(x) ??=x
??=define MAKE(x) STR(x?=)
??=define Q ?
??=define NUMBER 4??/
2
// A trigraph-produced splice continues this comment: ??/
#error line comment ended too soon
/??/
* This is a block comment. ??=error not a directive *??/
/
static int same(const char *a, const char *b) {
    while (*a && *a == *b) { ++a; ++b; }
    return *a == *b;
}
??=line 300
static int physical_line = __LINE__;??/

int main(void) ??<
    int bad = physical_line != 300;
    int CAT(v,alue)??(2??) = ??< NUMBER, HEADER_VALUE ??>;
    bad += value??(0??) != 42 || value??(1??) != 73;
    bad += header_array??(1??) != 12;
    bad += (1 ??! 2) != 3 || (7 ??' 3) != 4 || (??-0) != -1;
    bad += !same("??=??(??/??/??)??'??<??!??>??-", "#[\\]^{|}~");
    bad += !same("a??/"b", "a\"b") || '??/n' != '\n';
    bad += !same("???=????!??x??", "?#??|??x??");
    /* Phase 2 and macro substitution can create trigraph spellings.
       They must survive stringification; phase 1 never runs a second time. */
    bad += !same(STR(?\
?=), "?" "?=");
    bad += !same(MAKE(Q), "?" "?=");
    printf("trigraphs %d\n", bad);
    return bad;
??>
