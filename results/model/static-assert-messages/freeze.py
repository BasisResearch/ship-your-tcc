#!/usr/bin/env python3
"""Freeze source-pinned final/failed observations without rewriting history."""
import hashlib,json,re,subprocess,tarfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
HERE=Path(__file__).resolve().parent

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 paths=set()
 scratch={'assert-messages-base','assert-messages-edit','assert-messages-patch-check'}
 for p in ROOT.glob('build/assert-messages-*'):
  if p.is_dir():
   paths.update([p/'tccgen.c'] if p.name in scratch else (x for x in p.rglob('*') if x.is_file()))
  elif p.is_file():paths.add(p)
 for p in ROOT.glob('build/worker-build*assert-messages*'):
  if p.is_file():paths.add(p)
 # Include every successful/failed combined gate's own captures and logs.
 for log in ROOT.glob('build/worker-build-assert-messages-full-*.log'):
  for d in set(re.findall(r'build/[A-Za-z0-9-]+-gate\.[A-Za-z0-9]+',log.read_text())):
   paths.update(p for p in (ROOT/d).rglob('*') if p.is_file())
 paths.update(ROOT/'build/test-c11'/n for n in ['commands.json','assert-message-diagnostics.json'])
 paths.update(p for p in (ROOT/'build/test-c11/assert-messages').rglob('*') if p.is_file())
 # The exact historical refusal remains linked to its unchanged full package.
 paths.update(p for p in (ROOT/'build/unevaluated-focused-5/assert-wide').rglob('*') if p.is_file())
 paths.add(ROOT/'build/unevaluated-focused-5/comparison.json')
 paths.add(ROOT/'build/unevaluated-native-tool/build.json')
 # The base commit plus exact changed source files reconstruct this checkpoint.
 sourcepaths=subprocess.check_output(['git','diff','--name-only'],cwd=ROOT,text=True).splitlines()
 sourcepaths+=subprocess.check_output(['git','ls-files','--others','--exclude-standard'],cwd=ROOT,text=True).splitlines()
 snapshot=ROOT/'build/assert-messages-final-sources'
 for name in sorted(set(sourcepaths)):
  if name.startswith(('results/','tasks/')):continue
  p=ROOT/name
  if p.is_file():
   target=snapshot/name;target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(p.read_bytes());paths.add(target)
 paths={p for p in paths if p.is_file()}
 manifest={str(p.relative_to(ROOT)):sha(p) for p in sorted(paths)}
 (HERE/'artifact-manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
 with tarfile.open(HERE/'evidence.tar.xz','w:xz',preset=6,dereference=True) as archive:
  for p in sorted(paths):archive.add(p,arcname=str(p.relative_to(ROOT)),recursive=False)
 metadata=dict(files={p.name:sha(p) for p in [HERE/'artifact-manifest.json',HERE/'evidence.tar.xz',HERE/'freeze.py',HERE/'verify.py']},
  members=len(manifest),base_revision=subprocess.check_output(['git','rev-parse','HEAD'],cwd=ROOT,text=True).strip(),
  explicit_build='assert-messages-lean-1',full_gate='assert-messages-full-1',source_pins=3379,
  prior_evidence={n:sha(ROOT/n) for n in ['results/model/unevaluated-syntax/evidence.tar.xz','results/model/unevaluated-syntax/pins.json']})
 (HERE/'pins.json').write_text(json.dumps(metadata,indent=2)+'\n')
 print('packaged',len(manifest),'members;',round((HERE/'evidence.tar.xz').stat().st_size/1e6,1),'MB')
if __name__=='__main__':main()
