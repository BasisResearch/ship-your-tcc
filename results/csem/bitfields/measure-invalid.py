"""Fresh full-compiler Sail constraint controls; refusals are not positive successes."""
from pathlib import Path
from concurrent.futures import ThreadPoolExecutor
import hashlib,json,lzma,os,re,shutil,subprocess,time
ROOT=Path.cwd();OUT=ROOT/'results/csem/bitfields/native-invalid';OUT.mkdir(exist_ok=True)
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def one(e):
 src=ROOT/e['path'];d=OUT/src.stem;d.mkdir(exist_ok=True);assert not (d/'receipt.json').exists()
 env={**os.environ,'KEEP_WORK':'1','MAX_STEPS':'100000000','ORACLE_META':str(d/'meta')};cmd=[str(ROOT/'scripts/run_tcc_sail.sh'),str(src)]
 t=time.monotonic();p=subprocess.run(cmd,env=env,capture_output=True,timeout=900)
 (d/'stdout').write_bytes(p.stdout);(d/'stderr').write_bytes(p.stderr);shutil.copyfile(src,d/'input.c')
 work=Path(re.search(rb'^work: (.+)$',p.stderr,re.M).group(1).decode())
 for fn in ['raw','trace','pcs','make.log','ld.log','embed.S']:shutil.copyfile(work/fn,d/fn)
 data=(work/'prog.elf').read_bytes()
 with lzma.open(d/'prog.elf.xz','wb') as z:z.write(data)
 assert hashlib.sha256(lzma.open(d/'prog.elf.xz','rb').read()).hexdigest()==hashlib.sha256(data).hexdigest()
 rejected=p.returncode==125 and b'tcc did not run the program' in p.stderr and b'error:' in p.stderr and b'TRACE-FUEL-OUT' not in (d/'trace').read_bytes() and b'TRACE-EXCEPTION' not in (d/'trace').read_bytes()
 row=dict(case=src.stem,command=cmd,source_sha256=sha(src),elf_sha256=hashlib.sha256(data).hexdigest(),exit=p.returncode,constraint_diagnostic=rejected,wall_seconds=round(time.monotonic()-t,2),artifact_pins={q.name:sha(q) for q in d.iterdir() if q.is_file()})
 (d/'receipt.json').write_text(json.dumps(row,indent=2)+'\n');print(src.stem,p.returncode,'diagnostic' if rejected else 'NONSUCCESS',flush=True);return row
rows=json.loads((ROOT/'c/tests/bitfields/manifest.json').read_text());rows=[x for x in rows if x['kind']=='constraint']
with ThreadPoolExecutor(max_workers=2) as pool:report=list(pool.map(one,rows))
(OUT/'report.json').write_text(json.dumps(dict(rows=report,driver_sha256=sha(Path(__file__))),indent=2)+'\n')
print('native constraint diagnostics',sum(x['constraint_diagnostic'] for x in report),'/',len(report))
