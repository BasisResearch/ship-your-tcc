#!/usr/bin/env python3
"""Check source read failures versus true EOF with built host/cross compilers.
Run scripts/test_tcc_c11.py first to build its host compiler from current patches.
"""
import json
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / 'build/read-error-host'
OUT.mkdir(parents=True, exist_ok=True)
header_dir = OUT / 'directory'
header_dir.mkdir(exist_ok=True)
empty = OUT / 'empty.h'
empty.write_bytes(b'')
normal = OUT / 'normal.h'
normal.write_text('#define VALUE 7\n')
results = []
for name, compiler in [('host', ROOT / 'build/test-c11/host/tcc'),
                       ('cross', ROOT / 'build/cross/riscv64-tcc')]:
    for std in ['c11', 'c99']:
        for label, path, valid in [('directory', header_dir, False), ('empty', empty, True),
                                   ('normal', normal, True), ('missing', OUT / 'missing.h', False)]:
            src = OUT / (label + '.c')
            src.write_text(f'#include "{path}"\nint main(void){{return 0;}}\n')
            cmd = [str(compiler), '-std=' + std, '-c', str(src), '-o', str(OUT / 'test.o')]
            p = subprocess.run(cmd, capture_output=True, timeout=30)
            ok = (p.returncode == 0) == valid
            if label == 'directory':
                ok = ok and b'could not read source file' in p.stderr
            results.append(dict(compiler=name, standard=std, case=label, passed=bool(ok),
                                exit=p.returncode, stderr=p.stderr.decode(errors='replace')))
            print(name, std, label, bool(ok), flush=True)
(OUT / 'results.json').write_text(json.dumps(results, indent=2) + '\n')
assert all(r['passed'] for r in results)
