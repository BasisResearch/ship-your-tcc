#!/usr/bin/env python3
"""Patch 0003 C11 regression lane. Uses the existing RV64I/Sail harness.

scripts/test_tcc_c11.py [--sail]
Builds host and cross tcc from pristine vendor + active patches, checks all
seven defect reproducers and extended cases against explicit expected output
and gcc, checks -E round trips, RV64I objects, UCN buffer splits and diagnostics.
--sail also runs cross objects via test_tcc_patch.sh and seven full tcc -run
images (one for each defect). Logs and exact commands: build/test-c11/.
"""
import argparse
import fcntl
from concurrent.futures import ThreadPoolExecutor
import json
import os
from pathlib import Path
import subprocess
import sys
import time

ROOT = Path(__file__).resolve().parent.parent
OUT = ROOT / 'build/test-c11'
CASES = {
    'results/repro/tcc_pp_digraphs.c': '2 3\n',
    'results/repro/tcc_pp_line_relative_name.c': 'foo.c 10\n',
    'results/repro/tcc_pp_pragma_operator.c': 'ok\n',
    'results/repro/tcc_pp_stringize_escapes.c': '9\n',
    'results/repro/tcc_pp_ucn_identifier.c': '3\n',
    'results/repro/tcc_union_designator_override.c': '0 0 1 0\n',
    'c/tests/c11/utf_literals.c': 'utf 0\n',
    'c/tests/c11/digraphs.c': '<: :> <% %> %: %:%: 0\n',
    'c/tests/c11/pragma.c': 'pragma 0\n',
    'c/tests/c11/ucn.c': 'ucn 0\n',
    'c/tests/c11/union.c': 'union 0\n',
    'c/tests/c11/line_include.c': 'line elsewhere/relative.c 0\n',
    'results/repro/tcc_pragma_macro_operand.c': 'pragma operands 0\n',
    'results/repro/tcc_pp_trigraphs.c': 'trigraphs 0\n',
}
COMMANDS = []

def run(cmd, *, cwd=ROOT, expected=0, env=None):
    cmd = [str(c) for c in cmd]
    p = subprocess.run(cmd, cwd=cwd, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                       timeout=3600, env=env)
    COMMANDS.append({'argv': cmd, 'cwd': str(cwd), 'rc': p.returncode,
                     'stdout': p.stdout.decode('utf8', 'replace'),
                     'stderr': p.stderr.decode('utf8', 'replace')})
    if expected is not None and p.returncode != expected:
        raise RuntimeError(f'{cmd}: exit {p.returncode}\n{p.stderr.decode(errors="replace")}')
    return p

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--sail', action='store_true')
    args = ap.parse_args()
    OUT.mkdir(parents=True, exist_ok=True)
    src = OUT / 'src'
    host = OUT / 'host'
    run(['scripts/tcc_src.sh', src])
    host.mkdir(exist_ok=True)
    run([src / 'configure', '--config-backtrace=no', '--config-bcheck=no'], cwd=host)
    run(['make', '-s', '-j8'], cwd=host)
    run(['scripts/build_cross_tcc.sh'])
    tcc = [host / 'tcc', '-std=c11', '-B' + str(host), '-I' + str(src / 'include')]
    cross = [ROOT / 'build/cross/riscv64-tcc', '-std=c11', '-nostdinc',
             '-I' + str(src / 'include')]
    xpack = next((Path.home() / 'toolchains').glob('xpack-riscv-none-elf-gcc-15.2.0-*/bin'))
    cross += ['-I' + str(xpack.parent / 'riscv-none-elf/include')]
    cases = dict(CASES)
    # Raw nonprinting bytes must stay raw through each stringification.
    raw = OUT / 'stringize_raw.c'
    raw.write_bytes(b'#include <stdio.h>\n#define S(x) #x\n#define X(x) S(x)\n'
                    b'int main(void){printf("raw %d\\n", (int)sizeof(X(S("\x01\xc3\xa9"))));return 0;}\n')
    cases[str(raw)] = 'raw 10\n'
    # Put every byte of a leading UCN on each side of the 8192-byte refill.
    for split in range(1, 10):
        f = OUT / f'ucn_boundary_{split}.c'
        tail = b'\nint \\U00010400=7;\nint main(void){return \xf0\x90\x90\x80!=7;}\n'
        f.write_bytes(b' ' * (8192 - split - len(b'\nint ')) + tail)
        cases[str(f)] = ''
    # Every trigraph at both splits of the raw 8192-byte refill, inside
    # strings and comments; also a splice that creates a comment delimiter.
    tri = b"=/'()!<>-"
    repl = b"#\\^[]|{}~"
    for x, y in zip(tri, repl):
        for split in (1, 2):
            f = OUT / f'trigraph_boundary_{x}_{split}.c'
            prefix = b'int main(void){return "'
            tail = b'??' + bytes([x])
            if y == 92:
                tail += b'??/'  # two phase-1 backslashes encode one in C
            tail += b'"[0]!=' + str(y).encode() + b';}\n'
            f.write_bytes(b' ' * (8192 - split - len(prefix)) + prefix + tail)
            cases[str(f)] = ''
    for split in (1, 2):
        f = OUT / f'trigraph_comment_boundary_{split}.c'
        prefix = b'/??/\n* comment '
        tail = b'??/\n still a comment *??/\n/\nint main(void){return 0;}\n'
        f.write_bytes(prefix + b' ' * (8192 - split - len(prefix)) + tail)
        cases[str(f)] = ''
    for suffix in (b'?', b'??', b'???', b'????'):
        f = OUT / f'trigraph_eof_{len(suffix)}.c'
        f.write_bytes(b'int main(void){return 0;}\n// trailing ' + suffix)
        cases[str(f)] = ''
    for f, expected in cases.items():
        name = Path(f).stem
        actual = run(tcc + ['-run', f]).stdout
        if actual != expected.encode():
            raise RuntimeError(f'{f}: host output {actual!r}, expected {expected!r}')
        exe = OUT / (name + '.gcc')
        run(['gcc', '-std=c11', '-w', f, '-o', exe])
        if run([exe]).stdout != actual:
            raise RuntimeError(f'{f}: gcc differs')
        pp = OUT / (name + '.i')
        pp.write_bytes(run(tcc + ['-E', f]).stdout)
        if run(tcc + ['-run', pp]).stdout != actual:
            raise RuntimeError(f'{f}: -E round trip differs')
        obj = OUT / (name + '.o')
        run(cross + ['-c', f, '-o', obj])
        run(['scripts/check_isa.sh', '-q', obj])
        print(f'PASS host/gcc/pp/cross/ISA {name}', flush=True)
    # Relative #line must leave the real directory available to #include.
    # Invalid scalar values and Annex D initial combining marks are errors.
    bad = ['\\u0001', '\\uD800', '\\U00110000', '\\u0301', '\\u00A0', '\\U0001FFFE']
    for i, ident in enumerate(bad):
        f = OUT / f'invalid_ucn_{i}.c'
        f.write_text(f'int {ident};\n')
        for compiler in (tcc, cross):
            if run(compiler + ['-c', f, '-o', OUT / 'invalid.o'], expected=None).returncode == 0:
                raise RuntimeError(f'accepted invalid identifier {ident}')
    # tcc's implementation pragma expands arguments after its first token;
    # GCC chooses a different pragma-pack grammar. Check _Pragma equivalence
    # to tcc's existing #pragma behavior independently of the GCC oracle.
    pf = OUT / 'pragma_macro_argument.c'
    pf.write_text('#define LEVEL 1\n_Pragma("pack(push, LEVEL)")\n'
                  'struct P {char c; int i;};\n_Pragma("pack(pop)")\n'
                  '_Static_assert(sizeof(struct P)==5,"pack macro expansion");\n')
    for compiler in (tcc, cross):
        run(compiler + ['-c', pf, '-o', OUT / 'pragma_macro_argument.o'])
    # Question-mark tokens produced by macros must also retain their token
    # boundaries when -E output is read as physical source a second time.
    ppq = OUT / 'pp_trigraph_macro_tokens.c'
    ppq.write_text('#define Q ?\nQ?=\n')
    for compiler in (tcc, cross):
        first = run(compiler + ['-E', ppq]).stdout
        if b'??=' in first:
            raise RuntimeError('macro tokens form a new phase-1 sequence in -E output')
        ppq_i = OUT / 'trigraph_macro_tokens.i'
        ppq_i.write_bytes(first)
        second = run(compiler + ['-E', ppq_i]).stdout
        def body(data):
            return b''.join(line for line in data.splitlines()
                            if not line.lstrip().startswith(b'#')).split()
        if body(first) != body(second):
            raise RuntimeError('macro question-mark tokens change on -E round trip')
    # Force short physical reads containing only pending question marks.
    chunks = [b'?', b'?', b'=define N 4??', b'/\n2\nint main(void)??<return N!=42;??>\n//', b'?', b'?']
    cmd = [str(c) for c in tcc + ['-run', '-']]
    proc = subprocess.Popen(cmd, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    for chunk in chunks:
        proc.stdin.write(chunk)
        proc.stdin.flush()
        time.sleep(0.05)
    proc.stdin.close()
    proc.stdin = None
    stdout, stderr = proc.communicate(timeout=30)
    COMMANDS.append({'argv': cmd, 'stdin_chunks_hex': [c.hex() for c in chunks],
                     'rc': proc.returncode, 'stdout': stdout.decode(), 'stderr': stderr.decode()})
    if proc.returncode != 0 or stdout:
        raise RuntimeError(f'short-read trigraph stream failed: {proc.returncode}, {stdout!r}, {stderr!r}')
    # libtcc's physical <string> entry is translated too; synthetic paste
    # and _Pragma buffers remain past phase 1.
    api = OUT / 'api_trigraph.c'
    text = '??=define N 4??/\n2\nint api(void)??<return N;??>\n'
    literal = json.dumps(text).replace('?', r'\?')
    api.write_text('#include "libtcc.h"\nint main(int argc, char **argv){\n'
                   'TCCState *s=tcc_new(); tcc_set_lib_path(s,argv[1]);\n'
                   'tcc_set_options(s,"-std=c11"); tcc_set_output_type(s,TCC_OUTPUT_OBJ);\n'
                   'if(tcc_compile_string(s,' + literal + ')) return 1;\n'
                   'if(tcc_output_file(s,argv[2])) return 2; tcc_delete(s); return 0;}\n')
    api_exe = OUT / 'trigraph_api'
    run(['gcc', '-std=c11', '-I' + str(src), api, host / 'libtcc.a', '-ldl', '-lpthread', '-lm', '-o', api_exe])
    api_obj = OUT / 'trigraph_api.o'
    run([api_exe, host, api_obj])
    api_main = OUT / 'api_trigraph_main.c'
    api_main.write_text('int api(void); int main(void){return api()!=42;}\n')
    run(['gcc', api_main, api_obj, '-o', OUT / 'trigraph_api_main'])
    run([OUT / 'trigraph_api_main'])
    print('PASS trigraph macro tokens / short reads / libtcc string input', flush=True)
    if args.sail:
        # The two-stage harness tests generated code; full images also execute
        # the compiler changes on Sail. line_include needs its sibling header
        # in the image; that behavior is covered above on host and cross.
        run(['scripts/test_tcc_patch.sh'] + list(CASES))
        full = list(CASES)[:7] + ['results/repro/tcc_pragma_macro_operand.c']
        # run_tcc_sail.sh embeds only /prog.c; inline the fixture header for
        # this full-compiler probe (host/cross/model also test the include).
        standalone = OUT / 'trigraph_standalone.c'
        standalone.write_bytes((ROOT / 'results/repro/tcc_pp_trigraphs.c').read_bytes().replace(
            b'??=include \"tcc_pp_trigraphs??-.h\"',
            (ROOT / 'results/repro/tcc_pp_trigraphs~.h').read_bytes()))
        full.append(str(standalone))
        def full_image(f):
            meta = OUT / (Path(f).stem + '.meta')
            meta.unlink(missing_ok=True)
            p = run(['scripts/run_tcc_sail.sh', f],
                    env={**os.environ, 'ORACLE_META': str(meta)})
            expected = CASES.get(f, 'trigraphs 0\n')
            if p.stdout != expected.encode():
                raise RuntimeError(f'{f}: full image differs: {p.stdout!r}')
            print(f'PASS tcc -run/Sail {f}', flush=True)
        # Leave room for the other worker; each Sail process is about 120 MB.
        with ThreadPoolExecutor(max_workers=3) as pool:
            list(pool.map(full_image, full))
    print(f'C11: {len(cases)} positive cases, {len(bad)} negative cases; all passed')

if __name__ == '__main__':
    OUT.mkdir(parents=True, exist_ok=True)
    with (OUT / '.lock').open('w') as lock:
        try:
            fcntl.flock(lock, fcntl.LOCK_EX | fcntl.LOCK_NB)
        except BlockingIOError:
            sys.exit('another C11 regression run owns build/test-c11; keep its artifacts intact')
        try:
            main()
        finally:
            (OUT / 'commands.json').write_text(json.dumps(COMMANDS, indent=2) + '\n')
