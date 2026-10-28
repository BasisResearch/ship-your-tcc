"""Retain original p05 runtime/formatter boundaries on the final executable."""
from pathlib import Path
import hashlib,json,os,subprocess
ROOT=Path.cwd();OUT=ROOT/'results/csem/bitfields/p05-pristine-runtime-final';OUT.mkdir(exist_ok=False);SRC=ROOT/'c/tests/cfg/p05_bitfield.c';H=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();B=ROOT/'.lake/build/bin/csem_run';pin=H(B);fork=Path(subprocess.check_output(['scripts/csem_fork_oracle.sh','--path'],text=True).strip());rows=[]
for libc in [False,True]:
 cmd=[str(ROOT/'scripts/csem_run.sh')]+(['--libc'] if libc else [])+['--impl','tcc-rv64','--address-space-top','18446744073709551615',str(SRC)];p=subprocess.run(cmd,capture_output=True,timeout=240,env={**os.environ,'LEAN_ABORT_ON_PANIC':'1'});name='libc' if libc else 'without-libc';(OUT/(name+'.stdout')).write_bytes(p.stdout);(OUT/(name+'.stderr')).write_bytes(p.stderr);rows.append(dict(command=cmd,exit=p.returncode,libc=libc,stdout=p.stdout.decode(),stderr=p.stderr.decode()))
assert H(B)==pin;(OUT/'report.json').write_text(json.dumps(dict(binary_sha256=pin,source_sha256=H(SRC),frontend_and_libc="pinned pristine runtime via ordinary wrapper defaults; separate from raw cfgTcc lane",driver_sha256=H(Path(__file__)),rows=rows),indent=2)+'\n');print([(r['libc'],r['exit'],r['stdout'][:170]) for r in rows])
