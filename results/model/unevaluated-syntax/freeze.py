#!/usr/bin/env python3
"""Retain final/intermediate/failed captures without pruning observations."""
import hashlib,json,re,tarfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
HERE=Path(__file__).resolve().parent

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 paths=set()
 for pattern in ['build/unevaluated-*','build/worker-build*unevaluated*']:
  for p in ROOT.glob(pattern):
   if p.is_dir():paths.update(x for x in p.rglob('*') if x.is_file())
   else:paths.add(p)
 # Tool binaries are reused, with exact byte pins/build manifests in captures;
 # their executable bytes are not new source or an observation being pruned.
 paths={p for p in paths if p.name not in ['capture_cut','unevaluated-batch.host']}
 for log in ROOT.glob('build/worker-build-unevaluated-full-*.log'):
  for d in set(re.findall(r'build/[A-Za-z0-9-]+-gate\.[A-Za-z0-9]+',log.read_text())):
   paths.update(p for p in (ROOT/d).rglob('*') if p.is_file())
 # Keep the failed native attempt and the final native cache's full cut files.
 for p in ROOT.glob('build/model_diff/cache/*.sail.json'):
  x=json.loads(p.read_text())
  if x.get('capture_settings',{}).get('binary','').endswith('/unevaluated-native-tool/capture_cut'):
   stem=p.name.removesuffix('.sail.json')
   paths.update(q for q in p.parent.glob(stem+'*') if q.is_file())
   paths.update(q for q in p.parent.glob(stem+'*') if q.is_dir() for q in q.rglob('*') if q.is_file())
 for p in ROOT.glob('build/model_diff/cache/e51896fb46ef0b32f79c89d1*'):
  if p.is_file():paths.add(p)
 paths.update((ROOT/'build/ghost-byte-inputs').glob('*'))
 paths.update((ROOT/'build/binding-origin-arena-inputs').glob('*'))
 paths.update((ROOT/'build/binding-origin-onoff-6').glob('*.c'))
 paths={p for p in paths if p.is_file()}
 manifest={str(p.relative_to(ROOT)):sha(p) for p in sorted(paths)}
 (HERE/'artifact-manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
 with tarfile.open(HERE/'evidence.tar.xz','w:xz',preset=6,dereference=True) as t:
  for p in sorted(paths):t.add(p,arcname=str(p.relative_to(ROOT)),recursive=False)
 pins={p.name:sha(p) for p in [HERE/'artifact-manifest.json',HERE/'evidence.tar.xz',HERE/'freeze.py',HERE/'verify.py']}
 (HERE/'pins.json').write_text(json.dumps(dict(files=pins,artifacts=len(manifest),source_pins=3331,explicit_build='unevaluated-lean-5',full_gate='unevaluated-full-1'),indent=2)+'\n')
 print('packaged',len(paths),'members;',round((HERE/'evidence.tar.xz').stat().st_size/1e6,1),'MB')
if __name__=='__main__':main()
