#!/usr/bin/env python3
"""Retain complete positioned Cabs and independent GNU bitfield observations.

Menhir placements it cannot parse and GCC's syntax diagnostics are separate
categories. Neither is a semantic agreement or a source-scope exclusion.
"""
import argparse
from collections import Counter
import hashlib
import json
import lzma
import os
from pathlib import Path
import subprocess
import sys
from raw_frontend_cmp import historical_positions

ROOT = Path(__file__).resolve().parents[1]


def sha(p):
    return hashlib.sha256(Path(p).read_bytes()).hexdigest()


def run(cmd, stem, env=None):
    cmd = list(map(str, cmd))
    p = subprocess.run(cmd, cwd=ROOT, capture_output=True, timeout=240,
                       env={**os.environ, 'LEAN_ABORT_ON_PANIC': '1', **(env or {})})
    stem.with_suffix('.command.json').write_text(json.dumps(cmd) + '\n')
    with lzma.open(stem.with_suffix('.stdout.xz'), 'wb', preset=0) as f:
        f.write(p.stdout)
    stem.with_suffix('.stderr').write_bytes(p.stderr)
    return p


def defined_zero(sem):
    return isinstance(sem, list) and bool(sem) and all(
        r.get('status') == 'defined' and r.get('exit') == 0 and r.get('stdout') == '' for r in sem)


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--out', type=Path, required=True)
    ap.add_argument('--oracle', type=Path)
    ap.add_argument('--runtime', type=Path)
    ap.add_argument('--build-id')
    ap.add_argument('--diagnostic', action='store_true')
    a = ap.parse_args()
    if a.build_id:
        from ghost_cabs_cmp import build_receipt
        build = build_receipt(a.build_id)
    else:
        build = None
    out = a.out.resolve(); out.mkdir(parents=True, exist_ok=False)
    raw = ROOT / '.lake/build/bin/csem_raw'
    text = ROOT / '.lake/build/bin/csem_cparse'
    pins = {str(p): sha(p) for p in [raw, text, ROOT / 'c/build/fs.list']}
    if a.oracle:
        assert a.runtime
        a.oracle = a.oracle.resolve(); a.runtime = a.runtime.resolve()
        pins[str(a.oracle)] = sha(a.oracle)
    rows = []
    manifest = json.loads((ROOT / 'c/tests/bitfield-attributes/manifest.json').read_text())
    # Keep all four original failing files byte for byte in this comparison.
    manifest += [dict(name=p.stem, path=str(p.relative_to(ROOT)), sha256=sha(p), kind='defined')
                 for p in sorted((ROOT / 'c/tests/bitfields').glob('gnu_member_*.c'))]
    for entry in manifest:
        src = ROOT / entry['path']; assert sha(src) == entry['sha256']
        pins[str(src)] = sha(src)
        d = out / entry['name']; d.mkdir()
        layout = d / 'layout.pp'
        p = run([raw, '--tcc', '--fslist', ROOT / 'c/build/fs.list', '--semantic',
                 '--layout', layout, src], d / 'raw')
        obj = json.loads(p.stdout) if p.returncode == 0 else None
        row = dict(entry, raw_exit=p.returncode, semantics=obj['semantics'] if obj else None)
        if obj:
            view = d / 'text.pp'; data = layout.read_bytes()
            view.write_bytes(data[:-1] if data.endswith(b'\n') else data)
            t = run([text, '--gnu', '--json', view, src], d / 'text')
            row['full_text_cabs'] = t.returncode == 0 and json.loads(t.stdout) == historical_positions(obj['parse']['cabs'])
            if a.oracle:
                oc = run([a.oracle, '--runtime=' + str(a.runtime), '--nolibc',
                          '--cpp=' + sys.executable + ' ' + str(ROOT / 'scripts/raw_frontend_cat.py'),
                          '--cabs-json', src], d / 'menhir', {'SYT_RAW_LAYOUT': str(layout)})
                row['menhir_exit'] = oc.returncode
                if oc.returncode == 0:
                    cabs = json.loads(oc.stdout); digest = cabs.pop('digest')
                    row['menhir'] = 'exact' if digest == hashlib.md5(src.read_bytes()).hexdigest() and cabs == historical_positions(obj['parse']['cabs']) else 'different'
                else:
                    # Retain stderr: an unsupported placement is never equality.
                    row['menhir'] = 'placement-nonsuccess'
        g = run(['gcc', '-std=c11', '-O0', src, '-o', d / 'gcc.exe'], d / 'gcc-compile')
        gr = run([d / 'gcc.exe'], d / 'gcc-run') if g.returncode == 0 else None
        row.update(gcc_compile_exit=g.returncode, gcc_exit=gr.returncode if gr else None)
        if entry['kind'] == 'placement-diagnostic':
            row['status'] = 'GCC_SYNTAX_DIAGNOSTIC' if g.returncode and obj and row.get('full_text_cabs') else 'FAIL_PLACEMENT'
        else:
            row['status'] = 'DEFINED_ZERO' if gr is not None and gr.returncode == 0 and gr.stdout == b'' and defined_zero(row['semantics']) and row.get('full_text_cabs') else 'FAIL'
        if entry.get('layout_path'):
            ls = ROOT / entry['layout_path']; assert sha(ls) == entry['layout_sha256']
            pins[str(ls)] = sha(ls)
            lp = run([raw, '--tcc', '--fslist', ROOT / 'c/build/fs.list', '--semantic', ls], d / 'layout-raw')
            lo = json.loads(lp.stdout)['semantics'] if lp.returncode == 0 else None
            lg = run(['gcc', '-std=c11', '-O0', ls, '-o', d / 'layout-gcc.exe'], d / 'layout-gcc-compile')
            lr = run([d / 'layout-gcc.exe'], d / 'layout-gcc-run') if lg.returncode == 0 else None
            row['layout_semantics'] = lo
            row['layout_equal'] = lr is not None and lr.returncode == 0 and isinstance(lo, list) and bool(lo) and all(
                r.get('status') == 'defined' and r.get('exit') == 0 and r.get('stdout', '').encode() == lr.stdout for r in lo)
            if not row['layout_equal']: row['status'] = 'FAIL_LAYOUT'
        if row.get('menhir') == 'different': row['status'] = 'FAIL_CABS'
        rows.append(row)
        print(entry['name'], row['status'], row.get('menhir', ''), flush=True)
    assert all(sha(p) == h for p, h in pins.items()), 'measurement inputs changed'
    counts = dict(Counter(r['status'] for r in rows))
    (out / 'report.json').write_text(json.dumps(dict(build=build, diagnostic=a.diagnostic, rows=rows, counts=counts, pins=pins), indent=2) + '\n')
    print(counts)
    return int(any(r['status'].startswith('FAIL') for r in rows))


if __name__ == '__main__':
    raise SystemExit(main())
