#!/usr/bin/env python3
"""Independent image/compiler observations for ordered embedded filesystem lookup.
Run after building tcc_model and c/build/base.elf, under the 12 GB scope.
Artifacts are diagnostics; matching refusals are not successful F3 arenas.
"""
import hashlib
import json
from pathlib import Path
import subprocess
import sys

import model_diff as md

ROOT = Path(md.ROOT)
OUT = ROOT / 'build/image-files-review'


def main():
    OUT.mkdir(parents=True, exist_ok=True)
    # Use the standard image-symbol generator, never a reference arena input.
    with open(md.SYMS, 'wb') as f:
        subprocess.run([str(ROOT / 'scripts/gen_image_syms.sh')], stdout=f, check=True)
    Path(md.CACHE).mkdir(parents=True, exist_ok=True)
    key = md.runtime_key()
    one, two = '#define VALUE 1\n', '#define VALUE 2\n'
    cases = [
        ('duplicate-last', '/same.h', [('/same.h', one), ('/same.h', two)], 2),
        ('duplicate-reversed', '/same.h', [('/same.h', two), ('/same.h', one)], 1),
        ('file-blocks-child', '/x/y.h', [('/x', one), ('/x/y.h', two)], None),
        ('directory-blocks-file', '/x/y.h', [('/x/y.h', two), ('/x', one)], 2),
        ('directory-not-file', '/x', [('/x/y.h', two), ('/x', one)], 'empty'),
        ('missing-parent', '/missing/../root.h', [('/root.h', two)], None),
        ('file-parent', '/root.h/../root.h', [('/root.h', two)], None),
        ('trailing-slash', '/root.h/', [('/root.h', two)], None),
        ('existing-parent', '/dir/../root.h', [('/dir/a.h', one), ('/root.h', two)], 2),
        ('repeated-dot', '//dir///./../root.h', [('/dir/a.h', one), ('/root.h', two)], 2),
        ('root-parent', '/../../root.h', [('/root.h', two)], 2),
    ]
    results = []
    for name, include, entries, expected in cases:
        src = OUT / (name + '.c')
        src.write_text('#include "' + include + '"\n' +
                       ('#ifdef VALUE\n#error directory was replaced by file contents\n#endif\n' if expected == 'empty' else
                        f'#if VALUE != {expected}\n#error wrong embedded contents\n#endif\n' if expected is not None else '') +
                       'int main(void) { return 0; }\n')
        extra = []
        for n, (path, data) in enumerate(entries):
            host = OUT / f'{name}-{n}.h'
            host.write_text(data)
            extra.append((path, str(host)))
        ref, rd, _ = md.ref_run(str(src), extra, ['-w'], [], key)
        model, data, seconds = md.model_run(str(src), extra, ['-w'], [], 'image-files-' + name)
        wanted = 'exit' if expected is None else 'ran'
        equal = ref == model == wanted
        if equal:
            equal = (Path(rd).read_bytes() == Path(data).read_bytes()) if ref == 'ran' else rd == data
        record = dict(case=name, include=include, entries=entries, expected=wanted,
                      reference=ref, model=model, equal=equal, model_seconds=seconds,
                      source=src.read_text())
        if ref == 'ran':
            record['arena_sha256'] = hashlib.sha256(Path(rd).read_bytes()).hexdigest()
        elif ref == 'exit':
            record['reference_exit'], con = rd
            record['reference_console_hex'] = con.hex()
        if model == 'exit':
            record['model_exit'], con = data
            record['model_console_hex'] = con.hex()
        results.append(record)
        print(name, 'same' if equal else 'DIFF', wanted, flush=True)
    pins = {p: hashlib.sha256((ROOT / p).read_bytes()).hexdigest() for p in [
        'Spec/ImageFiles.lean', 'Spec/Pre/Types.lean', 'Tcc/ModelMain.lean',
        'csem/lean_frontend/CerbFS.lean', 'c/src/htif.c', 'c/build/base.elf',
        '.lake/build/bin/tcc_model', 'scripts/test_image_files.py']}
    target = Path(sys.argv[1]) if len(sys.argv) > 1 else OUT / 'result.json'
    target.write_text(json.dumps(dict(runtime_key=key, pins=pins, results=results,
        reference='fast RV64I interpreter; successful arenas and refusals reported separately'), indent=2) + '\n')
    return 0 if all(r['equal'] for r in results) else 1


if __name__ == '__main__':
    sys.exit(main())
