"""Independent raw-source cfgTcc initialized-byte layout observations."""
from pathlib import Path
import hashlib,json,lzma,os,subprocess
ROOT=Path.cwd();OUT=ROOT/'results/csem/bitfields/layout-raw-final';OUT.mkdir(exist_ok=False);B=ROOT/'.lake/build/bin/csem_raw';S=ROOT/'c/tests/bitfields/layout.c';H=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();pin=H(B)
cmd=[str(B),'--tcc','--fslist',str(ROOT/'c/build/fs.list'),'--semantic',str(S)];p=subprocess.run(cmd,capture_output=True,timeout=240,env={**os.environ,'LEAN_ABORT_ON_PANIC':'1'})
with lzma.open(OUT/'stdout.xz','wb',preset=0) as z:z.write(p.stdout)
(OUT/'stderr').write_bytes(p.stderr);sem=json.loads(p.stdout).get('semantics') if p.returncode==0 else None;expected=(ROOT/'results/csem/bitfields/gcc-layout.stdout').read_text();ok=isinstance(sem,list) and bool(sem) and all(r.get('status')=='defined' and r.get('exit')==0 and r.get('stdout')==expected for r in sem);assert H(B)==pin
r=dict(command=cmd,exit=p.returncode,source_sha256=H(S),binary_sha256=pin,driver_sha256=H(Path(__file__)),semantics=sem,expected_stdout_sha256=H(ROOT/'results/csem/bitfields/gcc-layout.stdout'),ok=ok);(OUT/'report.json').write_text(json.dumps(r,indent=2)+'\n');print('raw layout',ok,sem);raise SystemExit(int(not ok))
