#!/usr/bin/env python3
"""Freeze the pure physical proof sources and verify compact build receipts."""
import argparse
import hashlib
import json
import subprocess
from pathlib import Path
ROOT = Path(__file__).resolve().parents[1]
def sha(p): return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('request_id')
    ap.add_argument('--gate', action='store_true')
    ap.add_argument('--audit', action='store_true')
    ap.add_argument('--verify', action='store_true')
    args = ap.parse_args()
    assert args.request_id.startswith('physical-proof-')
    reqpath = ROOT/'build'/f'worker-build.{args.request_id}.request'
    if args.verify:
        req = json.loads(reqpath.read_text())
        dst = ROOT/'build'/f'worker-build.{args.request_id}.result'
        if not dst.exists():
            candidate = ROOT/'build/worker-build.result'
            res = json.loads(candidate.read_text())
            assert res.get('request_id', res.get('request')) == args.request_id, 'no matching completed receipt'
            dst.write_bytes(candidate.read_bytes())
        res = json.loads(dst.read_text())
        assert res['request_id'] == req['request_id']
        print(json.dumps({k: v for k, v in res.items() if k not in ['sha256', 'source_hashes', 'binary_hashes']}, indent=2))
        assert res.get('source_hashes_verified')
        bad = [p for p, h in req['sha256'].items() if sha(ROOT/p) != h]
        assert not bad, bad
        print(f"verified {len(req['sha256'])} inputs; exit={res['exit']}")
        return
    note = ROOT/'build/lead-note.txt'
    if note.exists(): print(note.read_text())
    paths = subprocess.check_output(['git', 'ls-files', '--cached', '--others', '--exclude-standard'], cwd=ROOT, text=True).splitlines()
    pins = {p: sha(ROOT/p) for p in sorted(set(paths)) if (ROOT/p).is_file() and
            (p.endswith(('.lean', '.py', '.sh', '.patch', '.tsv', '.c', '.h')) or p in ['lakefile.toml', 'lake-manifest.json', 'lean-toolchain']) and not p.startswith(('results/', 'tasks/'))}
    req = dict(request_id=args.request_id, cwd=str(ROOT),
               source_revision=subprocess.check_output(['git', 'rev-parse', 'HEAD'], cwd=ROOT, text=True).strip(),
               targets=[] if args.gate else ['Tcc.Model.PhysicalProof'] + (['Tcc.Model.GhostAudit'] if args.audit else []), sha256=pins,
               note='Pure physical transform proof only; keep unchanged public Statements and executable behavior. Normal budgets, shared root lock/12GB cap. Freeze all sources until matching completed receipt; preserve failures. No sum cut regeneration.')
    if args.gate: req['kind'] = 'full-gate-validation'
    snapshot = ROOT/'build/physical-proof-evidence'/args.request_id
    changed = subprocess.check_output(['git', 'ls-files', '--modified', '--others', '--exclude-standard'], cwd=ROOT, text=True).splitlines()
    for p in changed:
        if p in pins:
            dst = snapshot/p
            dst.parent.mkdir(parents=True, exist_ok=True)
            dst.write_bytes((ROOT/p).read_bytes())
    reqpath.write_text(json.dumps(req, indent=2)+'\n')
    (ROOT/'build/worker-build.request').write_bytes(reqpath.read_bytes())
    print(f"submitted {args.request_id}; {len(pins)} inputs; targets={req['targets']}")
if __name__ == '__main__': main()
