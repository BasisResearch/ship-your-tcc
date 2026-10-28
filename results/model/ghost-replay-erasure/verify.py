#!/usr/bin/env python3
"""Check archived evidence, matching receipts, audits and optional live pins."""
import argparse, hashlib, json, tarfile
from pathlib import Path

def sha(data):
    return hashlib.sha256(data).hexdigest()

def main():
    parser = argparse.ArgumentParser(); parser.add_argument('--source-root', type=Path)
    args = parser.parse_args()
    with tarfile.open(Path(__file__).with_name('evidence.tar.xz'), 'r:xz') as archive:
        entries = archive.getmembers()
        assert all(e.isfile() and not Path(e.name).is_absolute() and '..' not in Path(e.name).parts for e in entries)
        assert len({e.name for e in entries}) == len(entries)
        data = {e.name: archive.extractfile(e).read() for e in entries}
    manifest = json.loads(data['manifest.json'])['members']
    assert set(data) == set(manifest) | {'manifest.json'}
    assert all(sha(data[n]) == h for n, h in manifest.items())
    final = None
    requests = {}
    for ident, status in [('replay-erasure-lean-1', 1), ('replay-erasure-lean-2', 0), ('replay-erasure-full-1', 0)]:
        request = json.loads(data[f'{ident}.request.json'])
        receipt = json.loads(data[f'{ident}.result.json'])
        requests[ident] = request
        assert request['request_id'] == receipt['request_id'] == ident
        assert receipt['exit'] == status and receipt['source_hashes_verified']
        if status == 0:
            for name in ['GhostDriver.lean', 'GhostAudit.lean']:
                assert sha(data[f'sources/final/{name}']) == request['sha256'][f'Tcc/Model/{name}']
        else:
            for name in ['GhostDriver.lean', 'GhostAudit.lean']:
                assert sha(data[f'sources/lean-1/{name}']) == request['sha256'][f'Tcc/Model/{name}']
        if ident == 'replay-erasure-full-1':
            assert request['kind'] == 'full-gate-validation'
            assert 'check: OK' in data[f'{ident}.log'].decode()
            final = request
    first = requests['replay-erasure-lean-1']['sha256']
    focused = requests['replay-erasure-lean-2']['sha256']
    assert focused == final['sha256']
    assert set(first) == set(focused)
    assert {p for p in first if first[p] != focused[p]} == {'Tcc/Model/GhostDriver.lean'}
    log = data['replay-erasure-lean-2.log'].decode()
    for name in ['erase_syntaxFields', 'associationReplayErasure']:
        assert f"'Tcc.Model.Ghost.{name}' depends on axioms: [propext, Classical.choice, Quot.sound]" in log
    assert 'Operation not permitted' in data['replay-erasure-local-1.log'].decode()
    if args.source_root:
        bad = [p for p, h in final['sha256'].items() if sha((args.source_root / p).read_bytes()) != h]
        assert not bad, bad
    print(f"verified {len(data)} members, three matching receipts, standard axioms; {len(final['sha256'])} final pins" + (' match live sources' if args.source_root else ' recorded'))

if __name__ == '__main__':
    main()
