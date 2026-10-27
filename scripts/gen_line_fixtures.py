#!/usr/bin/env python3
"""Generate physical-line regression sources; shared by C11 and model checks.

Each program has an explicit expected value and exits nonzero on disagreement.
The output lives under build/, never among the frozen compile-cut artifacts.
"""
from pathlib import Path


def generate(out):
    out = Path(out)
    out.mkdir(parents=True, exist_ok=True)
    cases = {}
    def add(name, source):
        path = out / ('line_' + name + '.c')
        path.write_bytes(source if isinstance(source, bytes) else source.encode())
        cases[str(path)] = ''
    for name, splice in [('single', '\\\n'), ('repeated', '\\\n' * 4),
                         ('trigraph', '??/\n'), ('mixed', '\\\n??/\n\\\n'),
                         ('crlf', '\\\r\n' * 2)]:
        add(name, '#line 300\nstatic int a=__LINE__' + splice +
            ';\nint main(void){return a!=300;}\n')
    add('inside_token', '#line 300\nstatic int a=__LI\\\nNE__\\\n;\n'
        'int main(void){return a!=300;}\n')
    add('wrappers', '#define L __LINE__\n#define W L\n#define F(x) (__LINE__+(x))\n'
        '#line 400\nstatic int a=W\\\n;\n'
        '#line 410\nstatic int b=F\\\n(0);\n'
        '#line 420\nstatic int c=F(\n0\n);\n'
        'int main(void){return a!=400||b!=410||c!=420;}\n')
    add('arguments', '#define I(x) x\n#define D(x) ((x)+(x))\n#define F(x) (__LINE__+(x))\n'
        '#line 500\nstatic int a=I(\n__LINE__\\\n);\n'
        '#line 510\nstatic int b=D(__LINE__\\\n);\n'
        '#line 520\nstatic int c=I(F)(\n__LINE__);\n'
        '#line 530\nstatic int d=F(I(__LINE__\\\n));\n'
        'int main(void){return a!=501||b!=1020||c!=1041||d!=1060;}\n')
    add('counter', '#define D(x) ((x)+(x))\n#define I(x) x\n'
        '#line 600\nstatic int a=D(__COUNTER__+__LINE__\\\n);\n'
        'static int b=__COUNTER__;\n'
        '#line 610\nstatic int c=I(__LINE__\\\n)+__LINE__;\n'
        'int main(void){return a!=1200||b!=1||c!=1221;}\n')
    add('comments', '#define F(x) (__LINE__+(x))\n#define I(x) x\n'
        '#line 700\nstatic int a=__LINE__\\\n/*\n comment */;\n'
        '#line 710\nstatic int b=F/*\\\n comment\n*/(0);\n'
        '#line 720\nstatic int c=I(/*\n*/__LINE__\\\n);\n'
        '#line 730\nstatic int d=__LINE__\\\n// continued??/\ncomment\n;\n'
        'int main(void){return a!=700||b!=710||c!=721||d!=730;}\n')
    add('paste_stringize', '#define CAT(a,b) a##b\n#define S(x) #x\n#define I(x) x\n'
        '#line 800\nstatic int a=CAT(__LI,NE__)\\\n;\n'
        '#line 810\nstatic int b=CAT(, __LINE__)\\\n;\n'
        '#line 820\nstatic char s[]=S(__LINE__\\\n+ __COUNTER__);\n'
        'static int c=__COUNTER__;\n'
        'int main(void){return a!=800||b!=810||c!=0||sizeof(s)!=22||s[0]!=95;}\n')
    add('paste_multiline', '#define C(a,b) a##b + __LINE__\n'
        '#line 850\nstatic int a=C(\n__LI,\nNE__);\n'
        '#line 860\nstatic int b=C(,\n__LINE__);\n'
        '#line 870\nstatic int c=C(__LINE__,\n);\n'
        'int main(void){return a!=1700||b!=1720||c!=1740;}\n')
    add('rescan', '#define I(x) x\n#define F(x) (__LINE__+(x))\n'
        '#define E(x,y) x y\n#define C(x,y) x##y\n'
        '#line 880\nstatic int a=E(,F)(\n__LINE__);\n'
        '#line 890\nstatic int b=I(C)(__LI,NE__)\\\n;\n'
        'int main(void){return a!=1761||b!=890;}\n')
    add('pragma', '#define L _Pragma("pack(push,1)") static int a=__LINE__;\n'
        '#line 900\nL\\\n\n_Pragma("pack(pop)")\n'
        'int main(void){return a!=900;}\n')
    # Includes resume the parent cursor; #line changes only presumed names.
    (out / 'line_fixture.h').write_text('#line 1200 "virtual/header.h"\n'
        'static int header_line=__LINE__\\\n;\n#define HEADER_L __LINE__\n')
    add('include', '#line 1000 "virtual/parent.c"\n'
        'static int before=__LINE__\\\n;\n#include "line_fixture.h"\n'
        'static int after=HEADER_L\\\n;\n'
        '#line 1100 "another.c"\nstatic int remap=__LINE__??/\n;\n'
        'int main(void){return before!=1000||header_line!=1200||after!=1003||remap!=1100;}\n')
    # End-of-buffer lookahead and trigraph transformation use different raw
    # refill lengths. Split both the identifier and the splice at every byte.
    for tri in (False, True):
        text = b'__LINE__' + (b'??/\n' if tri else b'\\\n')
        for split in range(1, len(text)):
            prefix = b'#line 1300\nstatic int a='
            source = prefix + b' ' * (8192 - split - len(prefix)) + text
            source += b';\nint main(void){return a!=1300;}\n'
            add(f'boundary_{int(tri)}_{split}', source)
    # A compact standalone full-compiler Sail probe covers the original
    # reproducer, argument provenance, #line, phases 1/2 and counter caching.
    add('sail', '#define I(x) x\n#define D(x) ((x)+(x))\n#define L __LINE__\n'
        '#line 300\nstatic int a=__LINE__\\\n;\n'
        '#line 400\nstatic int b=L??/\n\\\n;\n'
        '#line 500\nstatic int c=I(\n__LINE__\\\n);\n'
        '#line 600\nstatic int d=D(__COUNTER__+__LINE__\\\n);\n'
        'int main(void){return a!=300||b!=400||c!=501||d!=1200||__COUNTER__!=1;}\n')
    return cases


if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('out', nargs='?', default='build/test-c11/line')
    args = parser.parse_args()
    for path in generate(args.out):
        print(path)
