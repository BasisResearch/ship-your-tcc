"""Fresh final-binary controls, retaining all outcomes including negative controls."""
from pathlib import Path
import hashlib,json,lzma,os,shutil,subprocess
ROOT=Path.cwd();OUT=ROOT/'results/csem/bitfields/controls-final';OUT.mkdir(exist_ok=False);B=ROOT/'.lake/build/bin/csem_raw'
H=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();pin=H(B)
base=ROOT/'results/csem/bitfields';old=json.loads((base/'lead-compound-ocaml/report.json').read_text());cases=[(r['case'],base/'lead-compound-ocaml/sources'/(r['case']+'.c'),r['expected']) for r in old['rows']]
cases += [('byte-copy-'+n,base/'lead-byte-copy/before'/n/'input.c',0) for n in ['padding','full-byte','assignment']]
cases += [('same-field-race',base/'minimized/same-field-race.c',None)]
rows=[]
for name,src,expected in cases:
 d=OUT/name;d.mkdir();shutil.copyfile(src,d/'input.c');cmd=[str(B),'--tcc','--fslist',str(ROOT/'c/build/fs.list'),'--semantic',str(src)]
 p=subprocess.run(cmd,capture_output=True,timeout=240,env={**os.environ,'LEAN_ABORT_ON_PANIC':'1'});sem=json.loads(p.stdout).get('semantics') if p.returncode==0 else None
 with lzma.open(d/'stdout.xz','wb',preset=0) as f:f.write(p.stdout)
 (d/'stderr').write_bytes(p.stderr)
 if expected is None:ok=isinstance(sem,list) and bool(sem) and all('UB035' in json.dumps(x) for x in sem)
 else:ok=isinstance(sem,list) and bool(sem) and all(x.get('status')=='defined' and x.get('exit')==expected and x.get('stdout')=='' for x in sem)
 rows.append(dict(case=name,command=cmd,source_sha256=H(src),exit=p.returncode,semantics=sem,expected=expected,ok=ok));print('PASS' if ok else 'FAIL',name,'outcomes',len(sem) if isinstance(sem,list) else sem,flush=True)
assert H(B)==pin
(OUT/'report.json').write_text(json.dumps(dict(binary_sha256=pin,driver_sha256=H(Path(__file__)),rows=rows),indent=2)+'\n');assert all(x['ok'] for x in rows)
