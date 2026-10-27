#!/usr/bin/env python3
"""Reproduce the extra C11 model fixtures in build/c11-model-focused/.

Raw-byte, UCN refill and negative cases reuse scripts/test_tcc_c11.py's
fixtures. The pragma macro-argument fixture adds main for an arena handoff.
Run from the repository root, then pass the generated *.c to model_diff.py.
"""
from pathlib import Path
import json


def main():
    out = Path(__file__).resolve().parents[2] / 'build/c11-model-focused'
    out.mkdir(parents=True, exist_ok=True)
    (out / 'stringize_raw.c').write_bytes(
        b'#include <stdio.h>\n#define S(x) #x\n#define X(x) S(x)\n'
        b'int main(void){printf("raw %d\\n", (int)sizeof(X(S("\x01\xc3\xa9"))));return 0;}\n')
    for split in range(1, 10):
        tail = b'\nint \\U00010400=7;\nint main(void){return \xf0\x90\x90\x80!=7;}\n'
        (out / f'ucn_boundary_{split}.c').write_bytes(
            b' ' * (8192 - split - len(b'\nint ')) + tail)
    bad = ['\\u0001', '\\uD800', '\\U00110000', '\\u0301', '\\u00A0', '\\U0001FFFE']
    for i, ident in enumerate(bad):
        (out / f'invalid_ucn_{i}.c').write_text(f'int {ident};\n')
    (out / 'pragma_macro_argument.c').write_text(
        '#define LEVEL 1\n_Pragma("pack(push, LEVEL)")\n'
        'struct P {char c; int i;};\n_Pragma("pack(pop)")\n'
        '_Static_assert(sizeof(struct P)==5,"pack macro expansion");\n'
        'int main(void){return 0;}\n')
    options = {
        'quoted': '"-fsigned-char"',
        'unmatched_quote': '"-fsigned-char',
        'escaped_quote': '-I \\" -fsigned-char',
        'escaped_backslash': '-I \\\\ -fsigned-char',
        'trailing_backslash': '-fsigned-char -I trailing\\',
        'wp_ignored': '-Wp,-funsigned-char -fsigned-char',
    }
    for name, option in options.items():
        (out / f'options_{name}.c').write_text(
            '#pragma comment(option, ' + json.dumps(option) + ')\n'
            '_Static_assert((char)-1 < 0,"signed-char option survived splitting");\n'
            '#pragma comment(option, "-fno-signed-char -fno-reverse-funcargs -Wall '
            '-Wno-all -Werror=unsupported -Wno-error=unsupported -mms-bitfields -mno-ms-bitfields")\n'
            '_Static_assert((char)-1 > 0,"inverted option restores unsigned char");\n'
            'int main(void){return 0;}\n')
    (out / 'options_reverse_nested.c').write_text(
        '#pragma comment(option, "-freverse-funcargs")\n'
        'static int n;\nstatic int step(void){return ++n;}\n'
        'static int weighted(int a,int b,int c){return a+10*b+100*c;}\n'
        'int main(void){int (*f)(int,int,int)=weighted;\n'
        'return f(step(),weighted(step(),step(),step()),step())==0;}\n')
    (out / 'pragma_recursive_operand.c').write_text(
        '#define ENTER _Pragma(ENTER)\nENTER\nint main(void){return 0;}\n')


if __name__ == '__main__':
    main()
