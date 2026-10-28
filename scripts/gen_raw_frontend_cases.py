#!/usr/bin/env python3
"""Focused raw-frontend inputs. Generated files live under build/, never edited."""
from pathlib import Path
import json
import sys

CASES = {
    'basic': 'int main(void) { return 0; }\n',
    'include': '#include <a.h>\nint main(void){return HEADER;}\n',
    'macro-locations': '#define F(x) (x+1)\n#define C(a,b) a##b\n#define S(x) #x\n#line 90 "virtual.c"\nint C(va,l)=F(\n2);\nchar *s=S(foo + bar);\n',
    'trigraph-splice': '??=define X 1\nint va??/\nlue=X;\nchar *s="??/n";\n',
    'no-rephase': '#define S(x) #x\nchar *s=S(?\\\n?=);\nchar *t="?\\\n?/";\n',
    'ucn': 'int \\u00E9=1;\nint f(void){return \\U000000E9;}\n',
    'ucn-utf8': 'int \\u00E9=1;\nint main(void){return é+\\U000000E9;}\n',
    'ucn-macros': '#define \\u00E9 4\n#define ID(x) x\n#define CAT(a,b) a##b\nint α_tail=1;\nint x=é;\nint f(void){return ID(\\U000003B1_tail)+CAT(\\u03b1,_tail);}\n',
    'utf8-typedef': 'typedef int \\u03B1;\nα x;\n\\U000003B1 f(α arg){return arg;}\n',
    'numbers': 'double d=0x1.8p+2; float f=.5f; long double l=1e-4932L; unsigned long x=0xffffUL;\n',
    'prefixes': 'char *a=u8"a" "b";\nunsigned short *b=u"c" "d";\nunsigned *c=U"e" "f";\nint *d=L"g" "h";\nint x=L\'a\'+u\'b\'+U\'c\';\nchar *e="\\x41" "\\101" "\\?";\n',
    'line': '#line 400 "presumed.c"\nint value=__LINE__;\n',
    'pack': '#pragma pack(push,1)\nstruct S {char c; int i;};\n_Pragma("pack(pop)")\nstruct T {char c; int i;};\n',
    'pack-active': '#pragma pack(push,1)\nstruct S {char c; int i;};\n',
    'pragma-adjacent': 'char *s="a" _Pragma("unknown payload") "b";\n',
    'library': '#include "/runtime/libc/include/h.h"\nint user;\n',
    'line-library-plant': '#line 3 "/runtime/libc/include/plant.h"\nint user;\n',
    'library-line-alias': '#include "/runtime/libc/include/alias.h"\nint user;\n',
    'redefine-library-to-user': '#include "/runtime/libc/include/redefine.h"\n#define DECL extern int chosen(void);\nDECL\n',
    'redefine-user-to-library': '#define DECL extern int chosen(void);\n#include "/runtime/libc/include/redefine.h"\nDECL\n',
    'redefine-repeat-header': '#include "/runtime/libc/include/redefine.h"\n#include "/runtime/libc/include/redefine.h"\nDECL\n',
    'redefine-function': '#define DECL(n) extern int n(void);\n#define DECL(n) extern int n(void);\nDECL(chosen)\n',
    'redefine-after-undef': '#include "/runtime/libc/include/redefine.h"\n#undef DECL\n#define DECL extern int chosen(void);\nDECL\n',
}
INVALID = {
    'number-octal': 'int x=08;\n',
    'number-suffix': 'int x=1u8;\n',
    'number-tail': 'int x=123abc;\n',
    'number-exponent': 'double x=1e+;\n',
    'character-empty': "int x='';\n",
    'character-escape': "int x='\\q';\n",
    'string-escape': 'char *s="\\x";\n',
    'identifier-surrogate': 'int \\uD800;\n',
    'identifier-initial-combining': 'int \\u0301x;\n',
    'character-surrogate': "int x='\\uD800';\n",
    'pack-invalid': '#pragma pack(3)\nstruct S {int x;};\n',
    'adjacent-prefixes': 'void *s=u"a" U"b";\n',
}
HEADERS = {
    '/include/a.h': '#define HEADER 7\nint included;\n#include "b.h"\n',
    '/include/b.h': 'int nested;\n',
    '/runtime/libc/include/h.h': 'int library;\n',
    '/runtime/libc/include/alias.h': '#line 33 "ordinary-label.h"\nint aliased;\n',
    '/runtime/libc/include/redefine.h': '#define DECL extern int chosen(void);\n',
    '/lib/tcc/include/tccdefs.h': '\n',
}


def generate(root):
    root = Path(root)
    root.mkdir(parents=True, exist_ok=True)
    for name, data in {**CASES, **INVALID}.items():
        (root/(name+'.c')).write_text(data)
    listing = []
    for index, (name, data) in enumerate(HEADERS.items()):
        host = root/f'header-{index}.h'
        host.write_text(data)
        listing.append(f'{name}={host.resolve()}')
    (root/'fs.list').write_text('\n'.join(listing)+'\n')
    (root/'manifest.json').write_text(json.dumps(dict(positive=list(CASES), invalid=list(INVALID)), indent=2)+'\n')
    return root/'fs.list'


if __name__ == '__main__':
    generate(Path(sys.argv[1]))
