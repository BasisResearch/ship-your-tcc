#!/usr/bin/env python3
"""Freeze the local replay proof's final and failed evidence without pruning."""
import hashlib, io, json, tarfile
from pathlib import Path
ROOT = Path(__file__).resolve().parents[3]
OUT = Path(__file__).resolve().parent

def main():
    members = {}
    def add(name, path):
        members[name] = path.read_bytes()
    for ident in ['replay-erasure-lean-1', 'replay-erasure-lean-2', 'replay-erasure-full-1']:
        for kind in ['request', 'result']:
            add(f'{ident}.{kind}.json', ROOT / 'build' / f'worker-build.{ident}.{kind}')
        add(f'{ident}.log', ROOT / 'build' / f'worker-build-{ident}.log')
    for kind in ['log', 'result']:
        add(f'replay-erasure-local-1.{kind}', ROOT / 'build' / f'replay-erasure-local-1.{kind}')
    add('replay-erasure-static-1.log', ROOT / 'build/replay-erasure-static-1.log')
    add('request.py', ROOT / 'build/replay-erasure-request.py')
    add('lua-abs-inventory.txt', Path('/tmp/replay-erasure-inventory.txt'))
    add('lead-note.txt', ROOT / 'build/lead-note.txt')
    for name in ['GhostDriver.lean', 'GhostAudit.lean']:
        add(f'sources/lean-1/{name}', ROOT / 'build/replay-erasure-lean-1-sources' / name)
        add(f'sources/final/{name}', ROOT / 'Tcc/Model' / name)
    base = json.loads(members['replay-erasure-full-1.request.json'])['source_revision']
    manifest = {'base_revision': base, 'members': {
        n: hashlib.sha256(b).hexdigest() for n, b in sorted(members.items())}}
    members['manifest.json'] = (json.dumps(manifest, indent=2) + '\n').encode()
    with tarfile.open(OUT / 'evidence.tar.xz', 'w:xz') as archive:
        for name, data in sorted(members.items()):
            info = tarfile.TarInfo(name); info.size = len(data); info.mode = 0o644
            archive.addfile(info, io.BytesIO(data))
    print(f'froze {len(members)} regular members')

if __name__ == '__main__':
    main()
