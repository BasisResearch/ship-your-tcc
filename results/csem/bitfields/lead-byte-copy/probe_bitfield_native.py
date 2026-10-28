from pathlib import Path
import hashlib,json,subprocess,os,shutil,re,lzma
r=Path(__file__).resolve().parents[1];out=r/'build/bitfield-byte-copy-native';out.mkdir(exist_ok=False);src=r/'build/bitfield-byte-copy-review/padding/input.c'
H=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
paths=[src,r/'scripts/run_tcc_sail.sh',r/'c/build/base.elf',*sorted((r/'patches').glob('*.patch'))];pins={str(p.relative_to(r)):H(p) for p in paths};(out/'source-pins.json').write_text(json.dumps(pins,indent=2)+'\n')
subprocess.run(['free','-g'],check=True)
cmd=[str(r/'scripts/run_tcc_sail.sh'),str(src)];q=subprocess.run(cmd,cwd=r,capture_output=True,timeout=900,env={**os.environ,'KEEP_WORK':'1','MAX_STEPS':'100000000','ORACLE_META':str(out/'meta')})
(out/'stdout').write_bytes(q.stdout);(out/'stderr').write_bytes(q.stderr);(out/'command.json').write_text(json.dumps(cmd)+'\n');shutil.copyfile(src,out/'input.c')
work=Path(re.search(rb'^work: (.+)$',q.stderr,re.M).group(1).decode())
for fn in ['raw','trace','pcs','make.log','ld.log','embed.S']:shutil.copyfile(work/fn,out/fn)
with lzma.open(out/'prog.elf.xz','wb') as f:f.write((work/'prog.elf').read_bytes())
assert all(H(r/p)==h for p,h in pins.items())
result=dict(exit=q.returncode,elf_sha256=H(work/'prog.elf'),driver_sha256=H(Path(__file__)),artifacts={p.name:H(p) for p in out.iterdir() if p.is_file()})
(out/'result.json').write_text(json.dumps(result,indent=2)+'\n');print(result['exit'],flush=True)
