#!/usr/bin/env python3
"""Package every frozen source version and build receipt by content hash."""
import hashlib
import io
import json
import subprocess
import tarfile
from pathlib import Path
ROOT = Path(__file__).resolve().parents[3]
DEST = Path(__file__).resolve().parent
sha = lambda b: hashlib.sha256(b).hexdigest()

def main():
    ids = [f'physical-proof-{i}' for i in range(1, 12)] + ['physical-proof-full-1']
    members = {}
    statuses = {}
    for rid in ids:
        reqbytes = (ROOT/'build'/f'worker-build.{rid}.request').read_bytes()
        resbytes = (ROOT/'build'/f'worker-build.{rid}.result').read_bytes()
        req, res = json.loads(reqbytes), json.loads(resbytes)
        assert req['request_id'] == res['request_id'] == rid
        assert res['source_hashes_verified']
        assert res['exit'] == (0 if rid in ['physical-proof-11', 'physical-proof-full-1'] else 1)
        statuses[rid] = res['exit']
        members[f'builds/{rid}/request.json'] = reqbytes
        members[f'builds/{rid}/result.json'] = resbytes
        members[f'builds/{rid}/build.log'] = (ROOT/res['log']).read_bytes()
        for path, digest in req['sha256'].items():
            key = 'objects/'+digest
            if key in members:
                continue
            candidates = [ROOT/'build/physical-proof-evidence'/rid/path, ROOT/path]
            data = next((p.read_bytes() for p in candidates if p.is_file() and sha(p.read_bytes()) == digest), None)
            if data is None:
                data = subprocess.check_output(['git', 'show', req['source_revision']+':'+path], cwd=ROOT)
            assert sha(data) == digest, (rid, path)
            members[key] = data
    supplement_bytes = (ROOT/'build/physical-proof-supplement/pins.json').read_bytes()
    supplement = json.loads(supplement_bytes)
    assert supplement['primary'] == 3374 and supplement['supplementary'] == len(supplement['sha256'])
    for path, digest in supplement['sha256'].items():
        data = (ROOT/path).read_bytes()
        assert sha(data) == digest, ('supplement drift', path)
        blob = subprocess.check_output(['git', 'show', supplement['base']+':'+path], cwd=ROOT)
        assert sha(blob) == digest, ('supplement base drift', path)
        members['objects/'+digest] = data
    members['supplement/pins.json'] = supplement_bytes
    members['supplement/initial-134-pins.json'] = (ROOT/'build/physical-proof-supplement/initial-134-pins.json').read_bytes()
    members['supplement/pin_physical_supplement.py'] = Path('/data/home/kirancodes/Documents/code/ship-your-tcc/build/pin_physical_supplement.py').read_bytes()
    for path in ['build/physical-proof-local-build-1.log', 'build/physical-proof-lua-inventory.txt']:
        members['setup/'+Path(path).name] = (ROOT/path).read_bytes()
    members['setup/lua-mandatory-table.txt'] = subprocess.check_output(['git', 'show', '0bb1880:CLAUDE.md'], cwd='/data/home/kirancodes/Documents/code/ship-your-lua')
    for name in ['verify.py', 'package.py']:
        members['tooling/'+name] = (DEST/name).read_bytes()
    staging = ROOT/'build/physical-proof-staging.stderr'
    if staging.exists(): members['setup/git-staging.stderr'] = staging.read_bytes()
    dest = DEST/'evidence.tar.xz'
    assert not dest.exists(), 'never overwrite a completed package'
    with tarfile.open(dest, 'w:xz', preset=6) as tf:
        for name, data in sorted(members.items()):
            info = tarfile.TarInfo(name)
            info.size = len(data)
            info.mode = 0o644
            info.mtime = 0
            tf.addfile(info, io.BytesIO(data))
    pins = dict(archive_sha256=sha(dest.read_bytes()), members={n: sha(b) for n,b in sorted(members.items())},
                builds=statuses, focused='physical-proof-11', gate='physical-proof-full-1',
                lake_lock='/data/home/kirancodes/Documents/code/ship-your-tcc/build/lake.lock',
                scope='Pure physical transform proofs only; no runtime read/refill/host or compiler refinement.')
    (DEST/'pins.json').write_text(json.dumps(pins, indent=2)+'\n')
    print(f'Packaged {len(members)} regular members; {dest.stat().st_size} archive bytes')
if __name__ == '__main__': main()
