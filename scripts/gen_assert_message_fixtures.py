#!/usr/bin/env python3
"""C11 assertion messages; keep diagnostics/policy/extensions out of coverage."""
import argparse
import json
from pathlib import Path

PREFIXES = {'ordinary': '', 'utf8': 'u8', 'utf16': 'u', 'utf32': 'U', 'wide': 'L'}
REPRO = '_Static_assert(1,L"wide"); int x;\n'
HEADER = '#define MSG L"included" " message"\nstruct Header {_Static_assert(1,MSG); int n;};\n'

def wrap(message, context='file', condition='1'):
    assertion = f'_Static_assert({condition},{message});'
    if context == 'block': return f'int main(void){{{assertion} return 0;}}\n'
    declaration = {
        'file': assertion,
        'struct': f'struct S {{{assertion} int n;}};',
        'union': f'union U {{{assertion} int n;}};',
        'nested': f'struct S {{union U {{struct T {{{assertion} int n;}} t;}} u;}};',
    }[context]
    return declaration + '\nint main(void){return 0;}\n'

def fixtures():
    rows = {'assert-wide': (REPRO, 'valid')}
    for name, p in PREFIXES.items():
        for context in ['file', 'block', 'struct', 'union', 'nested']:
            rows[f'{name}-{context}'] = (wrap(p+'"before middle after"', context), 'valid')
        for label, message in {
            'same': p+'"raw\\x41" '+p+'"1" '+p+'"\\101"',
            'ordinary-first': '"before" '+p+'" middle"',
            'ordinary-last': p+'"before" " after"',
            'ordinary-middle': p+'"before" " middle" '+p+'" after"',
            'zero': p+'"before\\0after"',
            'unicode': p+'"before\\u00e9\\U0001f600after"',
            'empty': p+'"" ""',
        }.items(): rows[f'{name}-{label}'] = (wrap(message), 'valid')
        for context in ['file', 'block', 'struct', 'union']:
            rows[f'false-{name}-{context}'] = (wrap(p+'"before middle after"', context, '0'), 'false')
        rows[f'false-{name}-zero'] = (wrap(p+'"before\\0after"', condition='0'), 'false')
        rows[f'false-{name}-unicode'] = (wrap(p+'"before\\u00e9after"', condition='0'), 'false')
    rows['macro-line'] = ('#define CHECK(e,m) _Static_assert((e),m)\n#define MSG U"macro" " spelling"\n#line 80 "messages.c"\nCHECK(1,MSG);\nint main(void){CHECK(1,u"block");return 0;}\n', 'valid')
    rows['physical-splice'] = ('??=define MSG L"tri??/\ngraph"\n_Static_assert(1,MSG " end");\nint main(void){return 0;}\n', 'valid')
    rows['include'] = ('#include "messages.h"\n_Static_assert(1,MSG);\nint main(void){return 0;}\n', 'valid')
    for name, message in {'nonstring':'2', 'character':"L'x'", 'parenthesized':'(L"x")', 'empty':'', 'trailing':'L"x",', 'expression':'L"x"+1'}.items():
        rows['malformed-'+name] = (wrap(message), 'malformed')
    rows['malformed-close'] = ('_Static_assert(1,L"x";\n', 'malformed')
    # UTF-8 + wide is a C11 constraint; different wide prefixes are a
    # documented implementation policy (not intrinsically invalid C11).
    for a, pa in PREFIXES.items():
        for b, pb in PREFIXES.items():
            if not pa or not pb or a == b: continue
            category = 'encoding-constraint' if 'utf8' in (a,b) else 'wide-policy'
            rows[f'mixed-{a}-{b}'] = (wrap(pa+'"a" '+pb+'"b"'), category)
    rows['extension-no-message'] = ('_Static_assert(1); int main(void){return 0;}\n', 'extension')
    # Other consumers must retain their original char-string-only policy.
    for name,p in PREFIXES.items():
        for consumer, text in {
            'asm': 'int x __asm__(MESSAGE);',
            'section': 'int x __attribute__((section(MESSAGE)));',
            'alias': 'int x; extern int y __attribute__((alias(MESSAGE)));',
            'visibility': 'int x __attribute__((visibility(MESSAGE)));',
        }.items():
            message = p+('"default"' if consumer=='visibility' else '"x"')
            category = 'ordinary-consumer' if name in ('ordinary','utf8') else 'ordinary-restriction'
            rows[f'{consumer}-{name}'] = (text.replace('MESSAGE',message)+'\nint main(void){return 0;}\n', category)
    return rows

def batch():
    # Defined executable, using every encoding in every declaration context.
    statements=[]
    for p in PREFIXES.values():
        statements.append(f'_Static_assert(sizeof(int)==4,"before" {p}" middle" " after");')
    assertions='\n'.join(statements)
    return assertions+'\nstruct S {\n'+assertions+'\nint x;};\nunion U {\n'+assertions+'\nint x;};\nint main(void){\n'+assertions+'\nstruct S s={7}; union U u={9}; return s.x+u.x!=16;}\n'

def generate(out):
    out=Path(out);out.mkdir(parents=True,exist_ok=True)
    rows={}
    for name,(source,category) in fixtures().items():
        p=out/(name+'.c');p.write_text(source)
        rows[str(p)]=category
    (out/'messages.h').write_text(HEADER)
    p=out/'batch.c';p.write_text(batch());rows[str(p)]='valid'
    (out/'manifest.json').write_text(json.dumps(rows,indent=2)+'\n')
    return rows

if __name__=='__main__':
    ap=argparse.ArgumentParser();ap.add_argument('out',type=Path);a=ap.parse_args();rows=generate(a.out)
    print({c:list(rows.values()).count(c) for c in sorted(set(rows.values()))})
