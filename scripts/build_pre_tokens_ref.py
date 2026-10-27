#!/usr/bin/env python3
"""Generate and build an internal-token observer from pinned, patched tcc.

Only build/pre-tokens-ref is instrumented. It includes the actual tcc sources;
no Lean/model/spec token data are read. The -E printer is never called. The
observer intercepts pragma_parse, collects unconverted tokens once, then
replays them to tcc's unchanged effect parser. Clock and path IO are supplied
by the harness. This host execution is an independent diagnostic reference,
not a Sail or binary refinement proof.
"""
from pathlib import Path
import hashlib
import json
import shutil
import subprocess

ROOT = Path(__file__).resolve().parents[1]

def build():
    src = ROOT/'build/tcc-src'
    subprocess.run([str(ROOT/'scripts/tcc_src.sh'), str(src)],check=True)
    dst = ROOT/'build/pre-tokens-ref'
    dst.mkdir(parents=True,exist_ok=True)
    # Copy; do not modify generated patched sources, vendor, or shared trees.
    for p in src.iterdir():
        if p.is_file(): shutil.copy2(p,dst/p.name)
    config=(ROOT/'c/src/tcc-config/config.h').read_text().replace('#define TCC_BAREMETAL 1', '/* host observer: no newlib binary128 runtime */')
    (dst/'config.h').write_text(config)
    pp=(dst/'tccpp.c').read_text()
    # Retain the image's predefined macros, without compiling its newlib-only
    # phase-7 strtold adapter on the host. No preprocessing function changes.
    pp=pp.replace('#ifdef TCC_BAREMETAL\n/* TCC_BAREMETAL: the <stdint.h>', '#if 1\n/* TCC_BAREMETAL: the <stdint.h>')
    pp=pp.replace('#ifdef TCC_BAREMETAL\n    putdefs(cs, bm_type_defs);', '#if 1\n    putdefs(cs, bm_type_defs);')
    old='static int pragma_parse(TCCState *s1)\n{\n    next_nomacro();'
    assert pp.count(old)==1
    pp=pp.replace(old,'static int raw_pragma_body(TCCState *s1)\n{')
    anchor='/* put alternative filename */'
    assert pp.count(anchor)==1
    pp=pp.replace(anchor,'''/* Instrumentation: spelling observation with a replay to the effect parser. */
static int pragma_parse(TCCState *s1)
{
    int first, flags, handled, expand;
    TokenString *payload;
    next_nomacro();
    first = tok;
    if (!raw_capture_pragmas || first == TOK_once || first == TOK_push_macro || first == TOK_pop_macro)
        return raw_pragma_body(s1);
    flags = parse_flags;
    expand = first == TOK_pack || first == TOK_comment;
    parse_flags = PARSE_FLAG_PREPROCESS | PARSE_FLAG_LINEFEED;
    raw_marker('[');
    payload = tok_str_alloc();
    while (tok != TOK_LINEFEED && tok != TOK_EOF) {
        raw_observe();
        tok_str_add2(payload, tok, &tokc);
        if (expand) next(); else next_nomacro();
    }
    tok_str_add(payload, tok);
    tok_str_add(payload, 0);
    raw_marker(']');
    parse_flags = PARSE_FLAG_TOK_NUM | PARSE_FLAG_TOK_STR | PARSE_FLAG_LINEFEED;
    begin_macro(payload, 1);
    next();
    handled = raw_pragma_body(s1);
    end_macro();
    tok = TOK_LINEFEED;
    parse_flags = flags;
    return handled;
}

'''+anchor)
    # Prototypes come after the tcc.h include so the observer is in this TU.
    pp=pp.replace('#include "tcc.h"','#include "tcc.h"\nstatic void raw_observe(void);\nstatic void raw_marker(int);',1)
    # The patched compiler's zero-width output representation also carries
    # observation metadata. Only capture/replay of the effect parser is added.
    old = '    TokenString *saved_stack, *result;'
    assert pp.count(old) == 1
    pp=pp.replace(old, old+'\n    TokenString *saved_pending;')
    old = '    result = tok_str_alloc();\n    pragma_parse(tcc_state);'
    assert pp.count(old) == 1
    pp=pp.replace(old, '    result = tok_str_alloc();\n    saved_pending = raw_pending;\n    raw_pending = result;\n    pragma_parse(tcc_state);\n    raw_pending = saved_pending;')
    (dst/'tccpp.c').write_text(pp)
    shutil.copy2(ROOT/'scripts/pre_tokens_ref.c',dst/'observer.c')
    command=['gcc','-std=gnu11','-O2','-funsigned-char','-fno-strict-aliasing',
             '-DTCC_RV64I_SOFT','-I'+str(dst),str(dst/'observer.c'),'-lm','-ldl',
             '-o',str(dst/'pre_tokens_ref')]
    subprocess.run(command,check=True)
    pins=[*sorted((ROOT/'patches').glob('*.patch')),ROOT/'vendor/TINYCC_REV',
          ROOT/'scripts/pre_tokens_ref.c',Path(__file__),ROOT/'c/src/tcc-config/config.h']
    meta=dict(command=command,sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in pins},
              source_stamp=(src/'.tcc_src_stamp').read_text().strip(),
              executable_sha256=hashlib.sha256((dst/'pre_tokens_ref').read_bytes()).hexdigest())
    (dst/'build.json').write_text(json.dumps(meta,indent=2)+'\n')
    print(dst/'pre_tokens_ref')

if __name__=='__main__': build()
