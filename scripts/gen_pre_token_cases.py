#!/usr/bin/env python3
"""Focused raw phase-4 fixtures. Outputs are generated under build/."""
from pathlib import Path
import json

# Regression probes for repaired observer interference; all now belong
# to the ordinary strict success lane. Historical failures remain in results/.
OBSERVER_REGRESSIONS = {'pragma-nested-operand', 'pragma-nested-macro',
                      'pragma-operator-ignored-counter'}

CASES = {
'empty':'',
'prefixes': 'L"wide" u8"utf8" u"utf16" U"utf32" L\'a\' u\'b\' U\'c\' "plain"\n',
'literal-escapes': r'"\n" "\x41" "\101" "\?" "\\n"'+'\n',
'adjacent-strings': '"a" "b" u8"c" "d" L"e" "f"\n',
'digraphs':'%:define S(x) %:x\n%:define C(a,b) a%:%:b\nint a<:2:> = <%1,2%>; S(a<:0:>) C(ab,cd)\n',
'ucn-spellings':r'int \u00E9; int \U000000E9;'+'\n',
'ucn-identity':r'#define \u00E9 42'+'\né \\U000000E9\n',
'ucn-stringify':r'#define S(x) #x'+'\n'+r'S(\u00E9) S(\U000000E9)'+'\n',
'paste-prefix':'#define C(a,b) a##b\nC(u8,"x") C(L,\'a\') C(u,"x") C(U,"x")\n',
'stringify':'#define S(x) #x\nS( a /* comment */ + b ) S("\\n")\n',
'splice-trigraph-literal':'"?\\\n?=" "?\\\n?/" \'?\\\n?=\'\n',
'stringify-trigraph':'#define S(x) #x\nS(?\\\n?=) S(?\\\n?/)\n',
'phase1-trigraph':'??=define X 7\nX "??="\n',
'question-tokens':'? ? = ? ? /\n',
'counter':'__COUNTER__ __COUNTER__\n#define D(x) x x\nD(__COUNTER__) __COUNTER__\n',
'counter-stringify':'#define S(x) #x\nS(__COUNTER__) __COUNTER__\n',
'line-file':'__LINE__ __FILE__ __BASE_FILE__\n#line 300 "/virtual.c"\n__LINE__ __FILE__ __BASE_FILE__\n',
'line-splice':'#line 300\n__LINE__\\\n;\n',
'line-macro':'#define D(x) x x\n#line 300\nD(\n__LINE__\n)\n',
'date-time':'__DATE__ __TIME__\n',
'include':'#include "state.h"\n__LINE__ __FILE__ __COUNTER__\n#include "once.h"\n#include "once.h"\n',
'pack':'before\n#pragma pack(push,1)\nafter\n#pragma pack(pop)\nend\n',
'pack-literals': '#pragma pack(0x2)\n#pragma pack(push,04)\n#pragma pack(pop)\n#pragma pack()\n',
'error-pack-unsigned': '#pragma pack(1U)\n',
'error-pack-long': '#pragma pack(1L)\n',
'error-pack-overflow': '#pragma pack(push)\n' * 8,
'pack-nested':'#pragma pack(8)\n#pragma pack(push,2)\n#pragma pack(push,4)\n#pragma pack(pop)\nafter\n',
'pragma-two-in-macro':'#define P a _Pragma("pack(push, 2)") b _Pragma("pack(pop)") c\nP\n',
'pragma-argument-twice':'#define D(x) x x\nD(_Pragma("pack(push, 1)")) after\n',
'pack-macro':'#define N 2\n#pragma pack(push,N)\nkept\n',
'pragma-operator':'before _Pragma("pack(push, 1)") after\n_Pragma(L"pack(pop)") end\n',
'pragma-expanded-operand':'#define OP "pack(push, 2)"\n#define P(x) _Pragma(x)\nP(OP) kept\n',
'pragma-push-pop':'#define X 11\n#pragma push_macro("X")\n#undef X\n#define X 22\nX\n#pragma pop_macro("X")\nX\n',
'pragma-push-pop-name': '#define X 11\n#define NAME "X"\n#pragma push_macro(NAME)\n#undef X\n#define X 22\n#pragma pop_macro(NAME)\nX\n',
'pragma-operator-push-pop-name': '#define X 11\n#define NAME "X"\n_Pragma("push_macro(NAME)")\n#undef X\n#define X 22\n_Pragma("pop_macro(NAME)")\nX\n',
'error-push-macro': '#pragma push_macro(42)\n',
'error-pop-macro-operator': '_Pragma("pop_macro(42)")\n',
'pragma-pop-rescan':'#define X 11\n#pragma push_macro("X")\n#undef X\n#define X 22\n#define R _Pragma("pop_macro(\\\"X\\\")") X\nR X\n',
'pragma-unknown':'#define X 7\nbefore\n#pragma unknown X\nafter\n#pragma STDC FENV_ACCESS OFF\n',
'pragma-in-macro':'#define P before _Pragma("pack(push, 1)") after\nP end\n',
'pragma-in-argument':'#define ID(x) x\nID(before _Pragma("pack(push, 1)") after) end\n',
'pragma-ignored-counter':'#pragma unknown __COUNTER__\n__COUNTER__\n',
'pragma-stringify':'#define S0(x) #x\n#define S(x) S0(x)\nS(_Pragma("pack(push, 1)") word) after\n',
'pragma-paste':'#define C0(a,b) a##b\n#define C(a,b) C0(a,b)\nC(_Pragma("pack(push, 1)"),word) after\n',
'pragma-comment':'#pragma comment(lib,"m")\nafter\n',
# Pragma regressions exercise each composition independently. They contain
# raw preprocessing text deliberately; acceptance by the phase-7 C parser is
# not a prerequisite for comparing phase-4 streams.
'pragma-ignored-operator': '#pragma unknown _Pragma("pack(1)") __COUNTER__\n__COUNTER__\n',
'pragma-operator-ignored-counter': '_Pragma("unknown __COUNTER__") __COUNTER__\n',
'pragma-operator-pack-macro': '#define N 2\n_Pragma("pack(push,N)") kept\n_Pragma("pack(pop)")\n',
'pragma-repeated-nonempty': '#define D(x) before x middle x after\nD(_Pragma("pack(push,1)") word) end\n',
'pragma-repeated-distinct': '#define D(x) x x\nD(_Pragma("unknown one")) D(_Pragma("unknown two")) end\n',
'pragma-stringify-two': '#define S0(a,b) #a #b\n#define S(a,b) S0(a,b)\nS(_Pragma("unknown left") ab,_Pragma("unknown right") cd) after\n',
'pragma-stringify-raw': '#define S(x) #x\nS(_Pragma("pack(1)") word) __COUNTER__\n',
'pragma-stringify-middle': '#define S0(x) #x\n#define S(x) S0(x)\nS(left _Pragma("unknown middle") right) after\n',
'pragma-stringify-twice': '#define S0(x) #x\n#define D(x) S0(x) S0(x)\n#define E(x) D(x)\nE(_Pragma("unknown once") word) after\n',
'pragma-paste-right': '#define C0(a,b) a##b\n#define C(a,b) C0(a,b)\nC(word,_Pragma("unknown right")) after\n',
'pragma-paste-two': '#define C0(a,b) a##b\n#define C(a,b) C0(a,b)\nC(_Pragma("unknown left") ab, _Pragma("unknown right") cd) after\n',
'pragma-nested-operand': '_Pragma(_Pragma("unknown inner") "unknown outer") after\n',
'pragma-nested-macro': '#define P(x) _Pragma(x)\n#define OP "unknown outer"\nP(P("unknown inner") OP) after\n',
'pragma-pop-argument': '#define X 11\n#pragma push_macro("X")\n#undef X\n#define X 22\n#define D(x) x X x\nD(_Pragma("pop_macro(\\\"X\\\")")) X\n',
'pragma-once-operator': '#include "operator-once.h"\n#include "operator-once.h"\n__COUNTER__\n',
'pragma-once-prescan': '#include "prescan-once.h"\n#include "prescan-once.h"\n__COUNTER__\n',
'ordinary-argument-transforms': '#define S(x) #x\n#define C(a,b) a##b\n#define D(x) x x\nD(__COUNTER__) S( a + b ) C(ab,cd) __COUNTER__\n',
'ordinary-location-paste': '#define C(a,b) a##b\n#define D(x) x x\n#line 300\nD(\n__LINE__\n) C(\n__LI,\nNE__)\n',
'error-pack-underflow': '#pragma pack(pop)\n',
'error-pack-operator': '_Pragma("pack(3)")\n',
'error-pragma-prefix-u': '_Pragma(u"unknown")\n',
'error-pragma-prefix-U': '_Pragma(U"unknown")\n',
'macro-recursion':'#define A B\n#define B A\n#define F(x) F(x)\nA B F(3)\n',
'prosser':'#define f(a) a*g\n#define g(a) f(a)\nf(2)(9) g(2)(9)\n',
'conditional':'#define N 9\n#if defined(N) && N*3 == 27 && (1ULL << 63) > 0\nyes\n#else\nno\n#endif\n',
'inactive':'#if 0\n#error hidden\n#pragma pack(pop)\n#endif\nalive\n',
'error-directive':'#error no\n',
'error-string':'"unterminated\n',
'error-comment':'/* unterminated\n',
'error-macro':'#define F(x) x\nF(\n',
'error-include':'#include "missing.h"\n',
'error-pragma-operand':'_Pragma(u8"pack(1)")\n',
'error-pack':'#pragma pack(3)\n',
'error-paste':'#define BAD(x) ## x\n',
}

def generate(dst):
    dst.mkdir(parents=True,exist_ok=True)
    (dst/'state.h').write_text('__LINE__ __FILE__ __COUNTER__\n#line 800 "/inside.h"\n__LINE__ __FILE__\n')
    (dst/'once.h').write_text('#pragma once\n__COUNTER__\n')
    (dst/'operator-once.h').write_text('_Pragma("once")\n__COUNTER__\n')
    (dst/'prescan-once.h').write_text('#define D(x) x x\nD(_Pragma("once"))\n__COUNTER__\n')
    for name,text in CASES.items(): (dst/(name+'.c')).write_text(text)
    (dst/'manifest.json').write_text(json.dumps(dict(cases=list(CASES),negative=[n for n in CASES if n.startswith('error-')],observer_regressions=sorted(OBSERVER_REGRESSIONS)),indent=2)+'\n')
    print(f'{len(CASES)} focused inputs generated in {dst}')

if __name__=='__main__':generate(Path('build/pre-token-cases'))
