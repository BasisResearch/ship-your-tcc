"""Fresh expanded compiler/Sail observations with exact images and toolchain pins."""
from pathlib import Path
import hashlib,json,lzma,os,re,shutil,subprocess,time
ROOT=Path.cwd();OUT=ROOT/'results/csem/bitfields/native-final';OUT.mkdir(exist_ok=False)
H=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
old=json.loads((ROOT/'results/csem/bitfields/native-expanded-receipt.json').read_text())['input_pins']
paths={str(p):H(p) for name in old for p in [ROOT/name] if p.is_file() and not name.startswith('/tmp/')}
src=ROOT/'c/tests/bitfields/batch.c';paths[str(src)]=H(src)
(OUT/'input-pins.json').write_text(json.dumps(paths,indent=2)+'\n')
cmd=[str(ROOT/'scripts/run_tcc_sail.sh'),str(src)];start=time.monotonic()
p=subprocess.run(cmd,capture_output=True,timeout=900,env={**os.environ,'KEEP_WORK':'1','MAX_STEPS':'300000000','ORACLE_META':str(OUT/'meta')})
(OUT/'stdout').write_bytes(p.stdout);(OUT/'stderr').write_bytes(p.stderr);shutil.copyfile(src,OUT/'input.c')
work=Path(re.search(rb'^work: (.+)$',p.stderr,re.M).group(1).decode())
for name in ['raw','trace','pcs','make.log','ld.log','embed.S']:shutil.copyfile(work/name,OUT/name)
data=(work/'prog.elf').read_bytes()
with lzma.open(OUT/'prog.elf.xz','wb') as z:z.write(data)
assert hashlib.sha256(lzma.decompress((OUT/'prog.elf.xz').read_bytes())).hexdigest()==H(work/'prog.elf')
gcc=['gcc','-std=c11','-pedantic-errors','-O0',str(src),'-o',str(OUT/'gcc.exe')];g=subprocess.run(gcc,capture_output=True);assert g.returncode==0,g.stderr
q=subprocess.run([str(OUT/'gcc.exe')],capture_output=True);(OUT/'gcc.stdout').write_bytes(q.stdout);(OUT/'gcc.stderr').write_bytes(q.stderr)
with lzma.open(OUT/'gcc.exe.xz','wb') as z:z.write((OUT/'gcc.exe').read_bytes())
(OUT/'gcc.exe').unlink()
assert all(H(Path(n))==h for n,h in paths.items())
report=dict(command=cmd,exit=p.returncode,meta=(OUT/'meta').read_text().splitlines(),wall_seconds=time.monotonic()-start,elf_sha256=hashlib.sha256(data).hexdigest(),gcc_command=gcc,gcc_exit=q.returncode,gcc_compiler_sha256=H(Path(shutil.which('gcc'))),driver_sha256=H(Path(__file__)),artifacts={x.name:H(x) for x in OUT.iterdir() if x.is_file()})
(OUT/'receipt.json').write_text(json.dumps(report,indent=2)+'\n');print({k:report[k] for k in ['exit','gcc_exit','meta','wall_seconds']},flush=True)
assert p.returncode==q.returncode==0
