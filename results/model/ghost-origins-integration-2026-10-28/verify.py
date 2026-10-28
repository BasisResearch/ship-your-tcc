#!/usr/bin/env python3
"""Verify the combined checkpoint's artifact hashes and optional source pins."""
import argparse, gzip, hashlib, json, tarfile
from pathlib import Path

def digest(data):
    return hashlib.sha256(data).hexdigest()

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--sources', action='store_true')
    args = ap.parse_args()
    here = Path(__file__).resolve().parent
    root = here.parents[2]
    pins = json.loads((here/'pins.json').read_text())
    for name, expected in pins['artifacts'].items():
        assert digest((here/name).read_bytes()) == expected, name
    with tarfile.open(here/'captures.tar.gz') as archive:
        actual = {m.name: digest(archive.extractfile(m).read())
                  for m in archive.getmembers() if m.isfile()}
    assert actual == pins['archive_members']
    req = json.loads(gzip.decompress((here/'full-gate.request.json.gz').read_bytes()))
    res = json.loads(gzip.decompress((here/'full-gate.result.json.gz').read_bytes()))
    assert req['request_id'] == res['request_id']
    assert res['exit'] == 0 and res['source_hashes_verified']
    if args.sources:
        for name, expected in req['sha256'].items():
            assert digest((root/name).read_bytes()) == expected, name
    def read(name):
        return json.loads((here/(name+'.json')).read_text())
    origin = read('origins')
    assert origin['summary'] == {'exact': 17, 'explicit_differences': 2}
    assert origin['acceptance']['accepted'] and all(origin['harness_checks'].values())
    assert read('fixtures')['summary'] == {'exact': 44, 'explicit-nonsuccess': 12}
    assert not read('raw-strict')['acceptance']['accepted']
    assert read('arenas')['totals'] == {'n': 7, 'same': 7}
    assert read('broad')['totals'] == {'n': 1815, 'same': 1345}
    erasure = read('erasure')['rows']
    assert len(erasure) == 7 and all(r['equal'] and r['ghost_status'] == 'ok' and not r['token_errors'] for r in erasure)
    native = read('native-recomparison')['rows']
    assert len(native) == 3 and all(r['comparison']['status'] == 'equal' and r['comparison']['compared_bytes'] == 4194304 for r in native)
    print(f"Verified {len(actual)} capture members and {len(req['sha256'])} frozen gate pins; open gaps preserved")

if __name__ == '__main__':
    main()
