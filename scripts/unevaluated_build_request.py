#!/usr/bin/env python3
"""Frozen unevaluated syntax build handoff; inspect/verify compact matching receipts."""
import argparse, hashlib, json, subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser();ap.add_argument('request_id');ap.add_argument('--gate',action='store_true');ap.add_argument('--verify',action='store_true');ap.add_argument('targets',nargs='*');a=ap.parse_args()
    assert a.request_id.startswith(('unevaluated-','assert-messages-'))
    reqpath=ROOT/'build'/f'worker-build.{a.request_id}.request'
    if a.verify:
        req=json.loads(reqpath.read_text())
        resultpath=ROOT/'build'/f'worker-build.{a.request_id}.result'
        if not resultpath.exists():
            candidate=ROOT/'build/worker-build.result'
            result=json.loads(candidate.read_text())
            assert result.get('request_id',result.get('request'))==a.request_id,'no matching completion receipt'
            resultpath.write_bytes(candidate.read_bytes())
        res=json.loads(resultpath.read_text())
        assert res['request_id']==req['request_id']
        print(json.dumps({k:v for k,v in res.items() if k not in ['sha256','source_hashes','binary_hashes']},indent=2))
        assert res.get('source_hashes_verified')
        bad=[p for p,h in req['sha256'].items() if sha(ROOT/p)!=h]
        assert not bad,bad
        print(f"verified {len(req['sha256'])} frozen inputs; exit={res['exit']}")
        return
    note=ROOT/'build/lead-note.txt'
    if note.exists():print("Lead note read: " + sha(note))
    paths=subprocess.check_output(['git','ls-files','--cached','--others','--exclude-standard'],cwd=ROOT,text=True).splitlines()
    pins={p:sha(ROOT/p) for p in sorted(set(paths)) if (ROOT/p).is_file() and
          (p.endswith(('.lean','.lem','.py','.sh','.patch','.tsv','.sha256','.manifest','.c','.h','.ml','.mli','.mly','.mll','.core','.impl')) or p in ['lakefile.toml','lake-manifest.json','lean-toolchain']) and not p.startswith(('results/','tasks/'))}
    req=dict(request_id=a.request_id,cwd=str(ROOT),source_revision=subprocess.check_output(['git','rev-parse','HEAD'],cwd=ROOT,text=True).strip(),
             targets=[] if a.gate else (a.targets or ['tcc_model','tcc_pre_tokens','pre_spec','tcc_ghost','Tcc.Model.PreAudit','Tcc.Model.GhostAudit','csem_cparse','csem_raw']),sha256=pins,
             note='Complete compiler-action syntax and independent Raw.checked comparison. Assertion message lane repairs compiler/model C11 encodings while preserving ordinary-string-only consumers. Rebuild patched C image before native observations; strict C11 checks must run serially with image/cross rebuild. Sources frozen until matching completed receipt; preserve all failed receipts/logs. No cut regeneration or CSem changes.')
    if a.gate:req['kind']='full-gate-validation'
    reqpath.write_text(json.dumps(req,indent=2)+'\n');(ROOT/'build/worker-build.request').write_text(reqpath.read_text())
    print(f"submitted {a.request_id}; {len(pins)} inputs; targets={req['targets']}")
if __name__=='__main__':main()
