#!/usr/bin/env python3
"""Archive final and superseded GNU observations, exact sources and native bytes."""
import hashlib,json,re,subprocess,sys,tarfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
HERE=Path(__file__).resolve().parent
sys.path.insert(0,str(ROOT/'scripts'))
from freeze_sail_arenas import freeze

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
 paths={}
 def add(p,name=None):
  p=Path(p);name=name or str(p.relative_to(ROOT))
  if p.is_dir():
   for f in sorted(p.rglob('*')):
    if f.is_file() and '__pycache__' not in f.parts:add(f,name+'/'+str(f.relative_to(p)))
  else:
   assert p.is_file(),p
   assert name not in paths or sha(paths[name])==sha(p),name
   paths[name]=p
 gate_req=ROOT/'build/worker-build.gnu-expr-full-1.request'
 gate_res=ROOT/'build/worker-build.gnu-expr-full-1.result'
 req=json.loads(gate_req.read_text());res=json.loads(gate_res.read_text())
 assert req['request_id']==res['request_id']=='gnu-expr-full-1' and res['exit']==0 and res['source_hashes_verified']
 assert all(sha(ROOT/p)==h for p,h in req['sha256'].items())
 # Preserve all earlier successes, mismatched expectations and harness failures.
 for p in ROOT.glob('build/gnu-expr-*'):add(p)
 for p in ROOT.glob('build/worker-build*gnu-expr*'):add(p)
 add(ROOT/'build/lead-note.txt')
 add(ROOT/'build/assert-messages-artifacts.json')
 log=(ROOT/'build/worker-build-gnu-expr-full-1.log').read_text()
 gates=sorted(set(re.findall(r'build/[A-Za-z0-9-]+-gate\.[A-Za-z0-9]+',log)))
 for d in gates:add(ROOT/d)
 # Every source byte of both completed receipts is recoverable by digest.
 for rid in ['gnu-expr-lean-1','gnu-expr-full-1']:
  request=json.loads((ROOT/f'build/worker-build.{rid}.request').read_text())
  for name,h in request['sha256'].items():
   p=ROOT/name
   if sha(p)!=h:p=ROOT/'build/gnu-expr-first-build-sources'/name
   assert sha(p)==h,name
   add(p,'sources/'+h)
 # Native exports retain all 4 MiB, presence maps, registers and model bytes.
 native_reports=[]
 for d in gates:
  for p in (ROOT/d).rglob('arenas.json'):
   j=json.loads(p.read_text())
   if j.get('sail_settings') and j['totals'].get('same'):
    native_reports.append(p)
 exports=ROOT/'build/gnu-expr-native-exports'
 assert not exports.exists()
 inventory=[]
 for i,p in enumerate(sorted(native_reports)):
  dest=exports/str(i);info=freeze(p,dest)
  assert info['successes']==1
  inventory.append(dict(report=str(p.relative_to(ROOT)),directory=str(i),steps=info['rows'][0]['comparison']['steps']))
 (exports/'exports.json').write_text(json.dumps(inventory,indent=2)+'\n');add(exports)
 # Pin the unchanged compiler/runtime inputs used by all independent arenas.
 pinpaths=['c/build/base.elf','c/build/tcc.o','c/build/fs.list','build/image.syms','build/cross/riscv64-tcc',
           'build/test-c11/host/tcc','.lake/build/bin/tcc_model','.lake/build/bin/tcc_ghost',
           'build/unevaluated-native-tool/capture_cut','build/unevaluated-native-tool/build.json']
 for line in (ROOT/'c/build/fs.list').read_text().splitlines():pinpaths.append(line.split('=',1)[1])
 artifact_pins={n:sha(ROOT/n) for n in pinpaths}
 pinfile=ROOT/'build/gnu-expr-artifact-pins.json';pinfile.write_text(json.dumps(artifact_pins,indent=2)+'\n');add(pinfile)
 manifest={n:sha(p) for n,p in sorted(paths.items())}
 (HERE/'artifact-manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
 with tarfile.open(HERE/'evidence.tar.xz','w:xz',preset=6,dereference=True) as archive:
  for name,p in sorted(paths.items()):archive.add(p,arcname=name,recursive=False)
 metadata=dict(base_revision=subprocess.check_output(['git','rev-parse','HEAD'],cwd=ROOT,text=True).strip(),
   files={p.name:sha(p) for p in [HERE/'artifact-manifest.json',HERE/'evidence.tar.xz',HERE/'freeze.py',HERE/'verify.py',HERE/'extra.py',HERE/'git-staging.stderr']},
   members=len(manifest),source_pins=len(req['sha256']),gates=gates,native=inventory,
   prior_evidence={n:sha(ROOT/n) for n in ['results/model/static-assert-messages/evidence.tar.xz','results/model/replay-integration-2026-10-28.md']})
 (HERE/'pins.json').write_text(json.dumps(metadata,indent=2)+'\n')
 print('packaged',len(manifest),'members;',round((HERE/'evidence.tar.xz').stat().st_size/1e6,1),'MB;',len(inventory),'native exports')
if __name__=='__main__':main()
