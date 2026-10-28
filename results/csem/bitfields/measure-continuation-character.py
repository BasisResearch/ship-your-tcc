"""Fresh cfgTcc continuation execution, preserving every outcome and source."""
from pathlib import Path
from concurrent.futures import ThreadPoolExecutor
import hashlib,json,lzma,os,shutil,subprocess
ROOT=Path.cwd();OUT=ROOT/'results/csem/bitfields/continuation-character-final';OUT.mkdir(exist_ok=True);BINARY=ROOT/'.lake/build/bin/csem_raw'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
initial=sha(BINARY)
def one(src):
 d=OUT/src.stem;d.mkdir(exist_ok=True);shutil.copyfile(src,d/'input.c');cmd=[str(BINARY),'--tcc','--fslist',str(ROOT/'c/build/fs.list'),'--semantic',str(src)]
 p=subprocess.run(cmd,capture_output=True,env={**os.environ,'LEAN_ABORT_ON_PANIC':'1'},timeout=180)
 with lzma.open(d/'stdout.xz','wb',preset=0) as z:z.write(p.stdout)
 (d/'stderr').write_bytes(p.stderr);sem=json.loads(p.stdout).get('semantics') if p.returncode==0 else None
 ok=isinstance(sem,list) and bool(sem) and all(x.get('status')=='defined' and x.get('exit')==0 and x.get('stdout')=='' for x in sem)
 row=dict(name=src.name,source_sha256=sha(src),command=cmd,exit=p.returncode,semantics=sem,ok=ok);print('PASS' if ok else 'FAIL',src.name,flush=True);return row
files=sorted((ROOT/'build/bitfields-continuation-fixtures').glob('designator_*.c'));assert len(files)==58
with ThreadPoolExecutor(max_workers=3) as pool:rows=list(pool.map(one,files))
assert initial==sha(BINARY),'executable changed during measurement'
(OUT/'report.json').write_text(json.dumps(dict(rows=rows,binary_sha256=initial,driver_sha256=sha(Path(__file__))),indent=2)+'\n')
print('cfgTcc continuation',sum(x['ok'] for x in rows),'/',len(rows));raise SystemExit(int(not all(x['ok'] for x in rows)))
