#!/usr/bin/env python3
"""Default bump execution vs the pinned OCaml oracle and retained Sail pointer case."""
from pathlib import Path
import subprocess,json,hashlib,argparse
root=Path(__file__).resolve().parents[1]
ap=argparse.ArgumentParser();ap.add_argument('--out',type=Path,default=root/'results/csem/allocation-oracle')
out=ap.parse_args().out;out.mkdir(parents=True,exist_ok=True)
p=subprocess.check_output(['bash','-c','source scripts/csem_env.sh; printf "%s\\n%s\\n" "$CSEM_ORACLE" "$CSEM_RUNTIME"'],text=True).splitlines();oracle,runtime=p
rows=[]
for src in sorted(Path('csem/tests/allocation').glob('*.c')):
 a=subprocess.run([oracle,'--runtime='+runtime,'--nolibc','--exec','--batch','--mode=exhaustive',str(src)],capture_output=True,text=True,timeout=120)
 b=subprocess.run(['scripts/csem_run.sh',str(src)],capture_output=True,text=True,timeout=120)
 def lines(x):return [s for s in x.splitlines() if s.startswith(('Defined {','Undefined {','Error {'))]
 ok=lines(a.stdout)==lines(b.stdout) and bool(lines(a.stdout))
 rows.append(dict(source=str(src),sha256=hashlib.sha256(src.read_bytes()).hexdigest(),oracle_exit=a.returncode,csem_exit=b.returncode,oracle_stdout=a.stdout,csem_stdout=b.stdout,oracle_stderr=a.stderr,csem_stderr=b.stderr,match=ok))
 print('MATCH' if ok else 'DIFF',src,flush=True)
r=json.loads(Path('results/csem/cfg-probes/p12_pointer.tcc.json').read_text());src=Path('c/tests/cfg/p12_pointer.c');assert hashlib.sha256(src.read_bytes()).hexdigest()==r['source_sha256']
b=subprocess.run(['scripts/csem_run.sh','--impl','tcc-rv64',str(src)],capture_output=True,text=True,timeout=120)
old=json.loads(Path('results/csem/cfg-probes/p12_pointer.csem.json').read_text());ok=lines(b.stdout)==lines(old['raw_stdout']) and b.returncode==0
rows.append(dict(source=str(src),known_sail_record='results/csem/cfg-probes/p12_pointer.tcc.json',source_sha256=r['source_sha256'],sail_outcomes=r['outcomes'],sail_measured_utc=r['measured_utc'],current_csem_stdout=b.stdout,current_csem_stderr=b.stderr,match=ok,scope='Same source, historical target run; no recertification of old image pins'))
print('MATCH' if ok else 'DIFF','historical Sail pointer observation',flush=True)
(out/'default-equivalence.json').write_text(json.dumps(dict(rows=rows,failed=[r['source'] for r in rows if not r['match']],csem_binary_sha256=hashlib.sha256(Path('.lake/build/bin/csem_run').read_bytes()).hexdigest()),indent=2)+'\n')
raise SystemExit(any(not r['match'] for r in rows))
