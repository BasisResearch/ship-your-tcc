#!/usr/bin/env python3
"""Freeze GNU typedef alignment build inputs and verify matching receipts."""
import argparse
import hashlib
import json
import subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser();ap.add_argument('request_id');ap.add_argument('--gate',action='store_true');ap.add_argument('--verify',action='store_true');a=ap.parse_args()
    assert a.request_id.startswith('typedef-')
    reqpath=ROOT/'build'/f'worker-build.{a.request_id}.request'
    if a.verify:
        req=json.loads(reqpath.read_text());dst=ROOT/'build'/f'worker-build.{a.request_id}.result'
        if not dst.exists():
            candidate=ROOT/'build/worker-build.result';res=json.loads(candidate.read_text())
            assert res.get('request_id',res.get('request'))==a.request_id,'no matching completed receipt'
            dst.write_bytes(candidate.read_bytes())
        res=json.loads(dst.read_text());assert res['request_id']==req['request_id']
        print(json.dumps({k:v for k,v in res.items() if k not in ['sha256','source_hashes','binary_hashes']},indent=2))
        assert res.get('source_hashes_verified')
        bad=[p for p,h in req['sha256'].items() if sha(ROOT/p)!=h];assert not bad,bad
        print(f"verified {len(req['sha256'])} inputs; exit={res['exit']}");return
    note=ROOT/'build/lead-note.txt'
    if note.exists():print(note.read_text())
    paths=subprocess.check_output(['git','ls-files','--cached','--others','--exclude-standard'],cwd=ROOT,text=True).splitlines()
    pins={p:sha(ROOT/p) for p in sorted(set(paths)) if (ROOT/p).is_file() and
          (p.endswith(('.lean','.py','.sh','.patch','.tsv','.c','.h')) or p in ['lakefile.toml','lake-manifest.json','lean-toolchain']) and not p.startswith(('results/','tasks/'))}
    req=dict(request_id=a.request_id,cwd=str(ROOT),source_revision=subprocess.check_output(['git','rev-parse','HEAD'],cwd=ROOT,text=True).strip(),
             targets=[] if a.gate else ['tcc_model','tcc_pre_tokens','pre_spec','tcc_ghost','Tcc.Model.PreAudit','Tcc.Model.GhostAudit','Tcc.Model.AlignmentAudit'],sha256=pins,
             note='GNU typedef alignment compiler/model repair. Sources frozen until matching completion. No sum cut regeneration. Read lead-note.txt and receipt notes.')
    if a.gate:req['kind']='full-gate-validation'
    reqpath.write_text(json.dumps(req,indent=2)+'\n');(ROOT/'build/worker-build.request').write_text(reqpath.read_text())
    print(f"submitted {a.request_id}; {len(pins)} inputs; targets={req['targets']}")
if __name__=='__main__':main()
