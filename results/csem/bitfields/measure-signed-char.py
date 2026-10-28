"""Independent native signed-character representation-copy controls."""
from pathlib import Path
from concurrent.futures import ThreadPoolExecutor
import hashlib,json,lzma,os,re,shutil,subprocess
ROOT=Path.cwd();OUT=ROOT/'results/csem/bitfields/native-signed-char';OUT.mkdir(exist_ok=False)
H=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
common=json.loads((ROOT/'results/csem/bitfields/native-final/input-pins.json').read_text());common={n:H(Path(n)) for n in common if '/c/tests/bitfields/' not in n};(OUT/'input-pins.json').write_text(json.dumps(common,indent=2)+'\n')
def one(mode):
 src=ROOT/f'c/tests/bitfields/character_copy_schar_{mode}.c';d=OUT/mode;d.mkdir();shutil.copyfile(src,d/'input.c');cmd=[str(ROOT/'scripts/run_tcc_sail.sh'),str(src)]
 p=subprocess.run(cmd,capture_output=True,timeout=900,env={**os.environ,'KEEP_WORK':'1','MAX_STEPS':'100000000','ORACLE_META':str(d/'meta')});(d/'stdout').write_bytes(p.stdout);(d/'stderr').write_bytes(p.stderr)
 work=Path(re.search(rb'^work: (.+)$',p.stderr,re.M).group(1).decode())
 for n in ['raw','trace','pcs','embed.S','make.log','ld.log']:shutil.copyfile(work/n,d/n)
 with lzma.open(d/'prog.elf.xz','wb') as z:z.write((work/'prog.elf').read_bytes())
 r=dict(command=cmd,exit=p.returncode,source_sha256=H(src),elf_sha256=H(work/'prog.elf'),meta=(d/'meta').read_text().splitlines(),artifacts={x.name:H(x) for x in d.iterdir() if x.is_file()});assert hashlib.sha256(lzma.decompress((d/'prog.elf.xz').read_bytes())).hexdigest()==r['elf_sha256'];(d/'receipt.json').write_text(json.dumps(r,indent=2)+'\n');print(mode,p.returncode,r['meta'],flush=True);return r
with ThreadPoolExecutor(max_workers=2) as ex:rows=list(ex.map(one,['auto','static']))
assert all(H(Path(n))==h for n,h in common.items());(OUT/'report.json').write_text(json.dumps(dict(rows=rows,driver_sha256=H(Path(__file__))),indent=2)+'\n');assert all(r['exit']==0 for r in rows)
