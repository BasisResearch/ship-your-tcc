"""Candidate user parser with version-consistent pristine textual libc inputs."""
from pathlib import Path
import hashlib,json,os,subprocess
ROOT=Path.cwd();OUT=ROOT/'results/csem/bitfields/p05-candidate-final';OUT.mkdir(exist_ok=False);SRC=ROOT/'c/tests/cfg/p05_bitfield.c';H=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();B=ROOT/'.lake/build/bin/csem_run';pin=H(B);fork=Path(subprocess.check_output(['scripts/csem_fork_oracle.sh','--path'],text=True).strip())
# This preparer uses its ordinary pristine defaults. Only textual Core/JSON
# crosses to the candidate driver; no old marshalled .co reaches its new types.
args=subprocess.check_output(['scripts/csem_libc_prep.sh'],text=True).splitlines();(OUT/'libc-args.txt').write_text('\n'.join(args)+'\n');pins={x:H(Path(x)) for x in args if not x.startswith('--')};(OUT/'libc-input-pins.json').write_text(json.dumps(pins,indent=2)+'\n')
original=SRC.read_text();variant=original
for i in range(4):variant=variant.replace('raw['+str(i)+']','(unsigned)raw['+str(i)+']')
assert original!=variant;(OUT/'unsigned-print.c').write_text(variant);(OUT/'original.c').write_text(original);rows=[]
for name,src in [('original',OUT/'original.c'),('unsigned-print',OUT/'unsigned-print.c')]:
 cmd=[str(B)]+args+['--impl','tcc-rv64','--address-space-top','18446744073709551615',str(src)];p=subprocess.run(cmd,capture_output=True,timeout=240,env={**os.environ,'CSEM_ORACLE':str(fork/'_build/default/backend/driver/main.exe'),'CSEM_RUNTIME':str(fork/'_build/install/default'),'LEAN_ABORT_ON_PANIC':'1'});(OUT/(name+'.stdout')).write_bytes(p.stdout);(OUT/(name+'.stderr')).write_bytes(p.stderr)
 gcc=['gcc','-std=c11','-pedantic-errors','-O0',str(src),'-o',str(OUT/(name+'.exe'))];g=subprocess.run(gcc,capture_output=True,text=True);q=subprocess.run([str(OUT/(name+'.exe'))],capture_output=True,text=True) if g.returncode==0 else None
 rows.append(dict(case=name,source_sha256=H(src),command=cmd,exit=p.returncode,stdout=p.stdout.decode(),stderr=p.stderr.decode(),gcc_command=gcc,gcc_compile_exit=g.returncode,gcc_compile_stderr=g.stderr,gcc_exit=None if q is None else q.returncode,gcc_stdout=None if q is None else q.stdout,gcc_stderr=None if q is None else q.stderr));print(name,p.returncode,p.stdout.decode()[:220],flush=True)
assert H(B)==pin and all(H(Path(n))==h for n,h in pins.items());(OUT/'report.json').write_text(json.dumps(dict(binary_sha256=pin,original_source_sha256=H(SRC),oracle_key=fork.name,oracle_sha256=H(fork/'_build/default/backend/driver/main.exe'),driver_sha256=H(Path(__file__)),rows=rows),indent=2)+'\n')
assert 'UB153b_illtyped_argument_for_format' in rows[0]['stdout'];assert rows[1]['exit']==0 and 'Defined {value: "Specified(1)"' in rows[1]['stdout'] and rows[1]['gcc_exit']==1
