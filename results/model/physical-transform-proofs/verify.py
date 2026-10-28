#!/usr/bin/env python3
"""Verify complete portable pure-transform proof/build evidence without Lean."""
import argparse
import hashlib
import json
import re
import tarfile
from pathlib import Path

def sha(data):
    return hashlib.sha256(data).hexdigest()

def verify(directory):
    pins = json.loads((directory/'pins.json').read_text())
    archive = directory/'evidence.tar.xz'
    assert sha(archive.read_bytes()) == pins['archive_sha256'], 'archive hash'
    members = {}
    with tarfile.open(archive, 'r:xz') as tf:
        for member in tf:
            assert member.isfile(), member.name
            assert not member.name.startswith('/') and '..' not in Path(member.name).parts
            assert member.name not in members, member.name
            data = tf.extractfile(member).read()
            assert pins['members'][member.name] == sha(data), member.name
            if member.name.startswith('objects/'):
                assert member.name == 'objects/'+sha(data), member.name
            members[member.name] = data
    assert set(members) == set(pins['members']), 'member set'
    requests = {}
    for rid, status in pins['builds'].items():
        req = json.loads(members[f'builds/{rid}/request.json'])
        res = json.loads(members[f'builds/{rid}/result.json'])
        assert req['request_id'] == res['request_id'] == rid
        assert res['exit'] == status and res['source_hashes_verified'], rid
        for path, digest in req['sha256'].items():
            assert 'objects/'+digest in members, (rid, path)
        if req.get('kind') == 'full-gate-validation':
            assert res['command'] == ['scripts/check.sh', '--csem']
            assert members[f'builds/{rid}/build.log'].rstrip().endswith(b'check: OK')
        else:
            cmd = res['command']
            assert cmd[0] == 'flock' and cmd[1] == pins['lake_lock']
            assert cmd[2:7] == ['systemd-run', '--user', '--scope', '-p', 'MemoryMax=12G']
            assert cmd[7:9] == ['lake', 'build']
        requests[rid] = req
    focus = requests[pins['focused']]
    gate = requests[pins['gate']]
    assert focus['sha256'] == gate['sha256'], 'focused/gate sources differ'
    supplement = json.loads(members['supplement/pins.json'])
    assert supplement['base'] == focus['source_revision']
    assert supplement['primary'] == len(focus['sha256'])
    assert supplement['supplementary'] == len(supplement['sha256']) == 399
    assert not (set(supplement['sha256']) & set(focus['sha256']))
    initial = json.loads(members['supplement/initial-134-pins.json'])
    assert initial['supplementary'] == len(initial['sha256']) == 134
    assert initial['base'] == supplement['base']
    assert all(supplement['sha256'][p] == h for p, h in initial['sha256'].items())
    assert 'supplement/pin_physical_supplement.py' in members
    for path, digest in supplement['sha256'].items():
        assert 'objects/'+digest in members, ('supplement', path)
    log = members[f"builds/{pins['focused']}/build.log"].decode()
    facts = re.findall(r"'([^']+)' (?:depends on axioms: \[([^\]]*)\]|does not depend on any axioms)", log)
    exports = {'physicalBlockErasure', 'physicalBlockShape', 'physicalOriginMap', 'physicalReadView'}
    audited = [(n, a) for n, a in facts if n.startswith('Tcc.Model.Ghost.PhysicalProof.') or n.rsplit('.', 1)[-1] in exports]
    proof_source = members['objects/'+focus['sha256']['Tcc/Model/PhysicalProof.lean']].decode()
    declarations = re.findall(r'^(?:@\[[^\n]*\]\s+)?theorem\s+(\w+)', proof_source, re.M)
    expected = {'Tcc.Model.Ghost.PhysicalProof.'+n for n in declarations} | {'Tcc.Model.Ghost.'+n for n in exports}
    assert set(n for n, a in audited) == expected and len(expected) == 40
    assert len(audited) == 40, len(audited)
    for name, assumptions in audited:
        assert set(assumptions.split(', '))-{''} <= {'propext', 'Classical.choice', 'Quot.sound'}, name
    base = requests['physical-proof-1']
    for path in ['Tcc/Model/PpLex.lean', 'Tcc/Model/Ghost.lean', 'Spec/Pre/Phase12.lean']:
        assert base['sha256'][path] == focus['sha256'][path], path
    def src(req, path):
        return members['objects/'+req['sha256'][path]].decode()
    def statement(source, name):
        return re.search(r'def '+name+r' : Prop :=\n(.*?)(?=\n(?:/--|def |theorem ))', source, re.S).group(1).rstrip()
    old = src(base, 'Tcc/Model/GhostDriver.lean')
    new = src(focus, 'Tcc/Model/GhostDriver.lean')
    for name in ['PhysicalOriginMap_Statement', 'PhysicalReadView_Statement']:
        assert statement(old, name) == statement(new, name), name
    assert 'theorem physicalOriginMap : PhysicalOriginMap_Statement' in new
    assert 'theorem physicalReadView : PhysicalReadView_Statement' in new
    print(f"Verified {len(members)} members; {len(requests)} receipts; {len(focus['sha256'])} primary frozen inputs + 399 unchanged-base supplementary inputs; 40 standard-axiom theorem audits; unchanged Statements and executable/spec transforms.")

if __name__ == '__main__':
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--directory', type=Path, default=Path(__file__).resolve().parent)
    verify(ap.parse_args().directory)
