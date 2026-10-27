#!/usr/bin/env python3
"""Freeze raw-frontend evidence only after the exact build and gate receipts."""
from collections import Counter
import csv
import hashlib
import json
from pathlib import Path
import re
import shutil

ROOT = Path(__file__).resolve().parents[1]
DEST = ROOT/'results/csem/raw-frontend'


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def main():
    req = json.loads((ROOT/'build/worker-build.raw-frontend-9.request').read_text())
    result = json.loads((ROOT/'build/worker-build.raw-frontend-9.result').read_text())
    gate_req = json.loads((ROOT/'build/worker-build.request').read_text())
    gate_result = json.loads((ROOT/'build/worker-build.result').read_text())
    assert req['request_id'] == result['request_id'] == 'raw-frontend-9'
    assert gate_req['request_id'] == gate_result['request_id'] == 'raw-frontend-gate-2'
    assert gate_req['kind'] == 'full-gate-validation'
    for request, receipt in [(req, result), (gate_req, gate_result)]:
        assert receipt['exit'] == 0 and receipt['source_hashes_verified']
        assert all(sha(ROOT/path) == h for path, h in request['sha256'].items())
    build_text = (ROOT/result['log']).read_text()
    raw_cone = build_text[build_text.index('== kernel cone: cone of [Tcc.CSem.Raw.frontEnd]'):]
    assert raw_cone.count('PARTIAL 0, OPAQUE 1, EXTERN(opaque) 1') == 2
    assert 'String.Internal.append' in raw_cone
    assert 'String.hash' not in raw_cone and 'CerbFloat' not in raw_cone
    assert 'Build completed successfully' in build_text
    assert 'check: OK' in (ROOT/gate_result['log']).read_text()
    old = list(csv.DictReader((ROOT/'results/csem/f1a-parser.tsv').open(), delimiter='\t'))
    baseline = list(csv.DictReader((ROOT/'build/raw-frontend/parser-baseline-v2.tsv').open(), delimiter='\t'))
    assert {(r['file'],r['status']) for r in old} == {(r['file'],r['status']) for r in baseline}
    assert Counter(r['status'] for r in baseline) == dict(AGREE=614, BOTH_REJECT=41, CPP_FAIL=9)
    full = json.loads((ROOT/'build/raw-frontend/cabs-v2/results.json').read_text())
    assert len(full['rows']) == 18 and all(r['status'] == 'AGREE' for r in full['rows'])
    corpus = json.loads((ROOT/'build/raw-frontend/f2-corpus.json').read_text())
    assert len(corpus['rows']) == 619
    assert corpus['summary']['raw'] == {'exact': 613, 'both-refused': 6}
    assert corpus['summary']['spec'].get('exact') == 611
    assert all(r['raw'] not in ['harness-error','observer-effect-difference'] for r in corpus['rows'])
    assert all(sha(ROOT/p) == h for p, h in corpus['sha256'].items())
    tests = (ROOT/'build/raw-frontend/focused-v2.log').read_text()
    assert re.search(r'Ran 22 tests.*\n\nOK', tests, re.S)
    focused = json.loads((ROOT/'build/raw-frontend/f2-focused.json').read_text())
    assert focused['summary']['raw'] == {'exact':64, 'both-refused':17}
    assert focused['summary']['spec'] == {'exact':64, 'spec-error':17}
    checked = json.loads((ROOT/'build/raw-frontend/checked.json').read_text())
    codes = {r['case']: r['diagnostic'] for r in checked}
    assert codes['pack'] == codes['pack-active'] == 'unimplemented-pack-layout'
    assert codes['line-library-plant'] == codes['library-line-alias'] == 'library-identity-mismatch'
    DEST.mkdir(exist_ok=True)
    for label, request, receipt in [('build',req,result),('gate',gate_req,gate_result)]:
        (DEST/f'{label}-request.json').write_text(json.dumps(request,indent=2)+'\n')
        (DEST/f'{label}-result.json').write_text(json.dumps(receipt,indent=2)+'\n')
        shutil.copyfile(ROOT/receipt['log'], DEST/f'{label}.log')
    for name in ['parser-baseline-v2.tsv','focused-v2.log','harness-tests.log','f2-corpus.json','f2-focused.json','checked.json']:
        shutil.copyfile(ROOT/'build/raw-frontend'/name, DEST/name)
    shutil.copytree(ROOT/'build/raw-frontend/cabs-v2', DEST/'cabs', dirs_exist_ok=True)
    files = sorted(p for p in DEST.rglob('*') if p.is_file() and p.name != 'manifest.json')
    (DEST/'manifest.json').write_text(json.dumps({str(p.relative_to(DEST)):sha(p) for p in files},indent=2)+'\n')
    print('Frozen: 614 parser Cabs agreements; 18 direct raw/full-Cabs agreements; strict F2 retained.')


if __name__ == '__main__':
    main()
